/* hacktv - Analogue video transmitter for the HackRF                    */
/*=======================================================================*/
/* Copyright 2017 Philip Heron <phil@sanslogic.co.uk>                    */
/*                                                                       */
/* This program is free software: you can redistribute it and/or modify  */
/* it under the terms of the GNU General Public License as published by  */
/* the Free Software Foundation, either version 3 of the License, or     */
/* (at your option) any later version.                                   */
/*                                                                       */
/* This program is distributed in the hope that it will be useful,       */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of        */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         */
/* GNU General Public License for more details.                          */
/*                                                                       */
/* You should have received a copy of the GNU General Public License     */
/* along with this program.  If not, see <http://www.gnu.org/licenses/>. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "video.h"
#include "nicam728.h"
#include "dance.h"
#include "hacktv.h"
#include "av.h"
#include "video_cfg.h"
#include <assert.h>

/* 
 * Video generation
 * 
 * The output from this encoder is a 16-bit IQ signal which
 * hopefully contains an accurate video and audio signal for
 * display on old analogue TV sets.
 * 
 * The encoder makes liberal use of lookup tables:
 * 
 * - 3x for RGB > gamma corrected Y, I and Q levels.
 * 
 * - A temporary gamma table used while generating the above.
 * 
 * - PAL colour carrier (4 full frames in length + 1 line) or
 *   NTSC colour carrier (2 full lines + 1 line).
*/

#define SECAM_FM_DEV 1000e3
#define SECAM_FM_FREQ 4328125 /* 277 fH */
#define SECAM_CB_FREQ 4250000 /* 272 fH */
#define SECAM_CR_FREQ 4406250 /* 282 fH */

/* Calculate the complex gain for the SECAM chrominance
 * sub-carrier at f Hz (bell curve) */
static void _secam_g(double *g, double f)
{
	const double f0 = 4.286e6;
	double lq, rq, d;
	
	f = f / f0 - f0 / f;
	
	lq = 16.0 * f;
	rq = 1.26 * f;
	d = 1.0 + rq * rq;
	
	g[0] = 0.115 * (1.0 + lq * rq) / d;
	g[1] = 0.115 * (lq - rq) / d;
}

static double _dlimit(double v, double min, double max)
{
	if(v < min) return(min);
	if(v > max) return(max);
	return(v);
}

static int16_t *_burstwin(unsigned int sample_rate, double width, double rise, double level, int *len)
{
	int16_t *win;
	double t;
	int i;
	
	*len = ceil(sample_rate * (width + rise));
	win = malloc(*len * sizeof(int16_t));
	if(!win)
	{
		return(NULL);
	}
	
	for(i = 0; i < *len; i++)
	{
		t = 1.0 / sample_rate * i;
		win[i] = round(rc_window(t, rise / 2, width, rise) * level * INT16_MAX);
	}
	
	return(win);
}

/* FM modulator
 * deviation = peak deviation in Hz (+/-) from frequency */
static int _init_fm_modulator(_mod_fm_t *fm, int sample_rate, double frequency, double deviation, double level)
{
	int r;
	double d;
	
	fm->level   = round(INT16_MAX * level);
	fm->counter = INT16_MAX;
	fm->phase.i = INT32_MAX;
	fm->phase.q = 0;
	fm->lut     = malloc(sizeof(cint32_t) * (UINT16_MAX + 1));
	
	if(!fm->lut)
	{
		return(VID_OUT_OF_MEMORY);
	}
	
	for(r = INT16_MIN; r <= INT16_MAX; r++)
	{
		d = 2.0 * M_PI / sample_rate * (frequency + (double) r / INT16_MAX * deviation);
		
		fm->lut[r - INT16_MIN].i = lround(cos(d) * INT32_MAX);
		fm->lut[r - INT16_MIN].q = lround(sin(d) * INT32_MAX);
	}
	
	return(VID_OK);
}

static int _init_fm_energy_dispersal(_mod_fm_t *fm, int sample_rate, rational_t frequency, int level)
{
	rational_t r;
	
	r = rational_div((rational_t) { level * 4, 1 }, (rational_t) { sample_rate, 1 });
	r = rational_mul(r, frequency);
	
	fm->ed_delta = div(r.num, r.den);
	fm->ed_overflow = (div_t) { level * 4, r.den };
	fm->ed_counter = (div_t) { 0, 0 };
	
	return(VID_OK);
}

static void inline _fm_modulator_add(_mod_fm_t *fm, int16_t *dst, int16_t sample)
{
	cint32_mul(&fm->phase, &fm->phase, &fm->lut[sample - INT16_MIN]);
	
	dst[0] += ((fm->phase.i >> 16) * fm->level) >> 15;
	dst[1] += ((fm->phase.q >> 16) * fm->level) >> 15;
	
	/* Correct the amplitude after INT16_MAX samples */
	if(--fm->counter == 0)
	{
		double ra = atan2(fm->phase.q, fm->phase.i);
		
		fm->phase.i = lround(cos(ra) * INT32_MAX);
		fm->phase.q = lround(sin(ra) * INT32_MAX);
		
		fm->counter = INT16_MAX;
	}
}

static void inline _fm_modulator_cgain(_mod_fm_t *fm, int16_t *dst, int16_t sample, const cint16_t *g)
{
	/* Only used by SECAM */
	
	cint32_mul(&fm->phase, &fm->phase, &fm->lut[sample - INT16_MIN]);
	
	dst[0] = (((((fm->phase.i >> 16) * fm->level) >> 15) * g->i) >> 15)
	       - (((((fm->phase.q >> 16) * fm->level) >> 15) * g->q) >> 15);
	
	/* Correct the amplitude after INT16_MAX samples */
	if(--fm->counter == 0)
	{
		double ra = atan2(fm->phase.q, fm->phase.i);
		
		fm->phase.i = lround(cos(ra) * INT32_MAX);
		fm->phase.q = lround(sin(ra) * INT32_MAX);
		
		fm->counter = INT16_MAX;
	}
}

static void inline _fm_modulator(_mod_fm_t *fm, int16_t *dst, int16_t sample)
{
	if(fm->ed_overflow.quot != 0)
	{
		sample += abs(fm->ed_counter.quot + -fm->ed_overflow.quot / 2) - fm->ed_overflow.quot / 4;
		
		fm->ed_counter.quot += fm->ed_delta.quot;
		fm->ed_counter.rem  += fm->ed_delta.rem;
		
		if(fm->ed_counter.rem >= fm->ed_overflow.rem)
		{
			fm->ed_counter.quot++;
			fm->ed_counter.rem -= fm->ed_overflow.rem;
		}
		
		if(fm->ed_counter.quot >= fm->ed_overflow.quot)
		{
			fm->ed_counter.quot -= fm->ed_overflow.quot;
		}
	}
	
	cint32_mul(&fm->phase, &fm->phase, &fm->lut[sample - INT16_MIN]);
	
	dst[0] = ((fm->phase.i >> 16) * fm->level) >> 15;
	dst[1] = ((fm->phase.q >> 16) * fm->level) >> 15;
	
	/* Correct the amplitude after INT16_MAX samples */
	if(--fm->counter == 0)
	{
		double ra = atan2(fm->phase.q, fm->phase.i);
		
		fm->phase.i = lround(cos(ra) * INT32_MAX);
		fm->phase.q = lround(sin(ra) * INT32_MAX);
		
		fm->counter = INT16_MAX;
	}
}

static void _free_fm_modulator(_mod_fm_t *fm)
{
	free(fm->lut);
}

/* AM modulator */
static int _init_am_modulator(_mod_am_t *am, int sample_rate, double frequency, double level)
{
	double d;
	
	am->level   = round(INT16_MAX * level);
	am->counter = INT16_MAX;
	am->phase.i = INT32_MAX;
	am->phase.q = 0;
	
	d = 2.0 * M_PI / sample_rate * frequency;
	am->delta.i = lround(cos(d) * INT32_MAX);
	am->delta.q = lround(sin(d) * INT32_MAX);
	
	return(VID_OK);
}

static void inline _am_modulator_add(_mod_am_t *am, int16_t *dst, int16_t sample)
{
	cint32_mul(&am->phase, &am->phase, &am->delta);
	
	sample = ((int32_t) sample - INT16_MIN) / 2;
	
	dst[0] += ((((am->phase.i >> 16) * sample) >> 15) * am->level) >> 15;
	dst[1] += ((((am->phase.q >> 16) * sample) >> 15) * am->level) >> 15;
	
	/* Correct the amplitude after INT16_MAX samples */
	if(--am->counter == 0)
	{
		double ra = atan2(am->phase.q, am->phase.i);
		
		am->phase.i = lround(cos(ra) * INT32_MAX);
		am->phase.q = lround(sin(ra) * INT32_MAX);
		
		am->counter = INT16_MAX;
	}
}

static void _free_am_modulator(_mod_am_t *am)
{
	/* Nothing */
}

void _test_sample_rate(const vid_config_t *conf, unsigned int sample_rate)
{
	int m, r;
	
	/* Test if the chosen sample rate results in an exact number of
	 * samples per line. If not, display a warning and show the
	 * previous and next valid sample rates. */
	
	/* Calculate lowest valid sample rate */
	m = conf->lines * conf->frame_rate.num;
	m /= r = gcd(m, conf->frame_rate.den);
	
	/* Is the chosen sample rate good? */
	if(sample_rate % m == 0) return;
	
	/* Not really. Suggest some good sample rates */
	r = sample_rate / m;
	fprintf(stderr, "Warning: Pixel rate %u may not work well with this mode.\n", sample_rate);
	fprintf(stderr, "Next valid pixel rates: %u, %u\n", m * r, m * (r + 1));
}

static int _vid_next_line_rawbb(vid_t *s, void *arg, int nlines, vid_line_t **lines)
{
	vid_line_t *l = lines[0];
	int x, i;
	
	l->width    = s->width;
	l->frame    = s->bframe;
	l->line     = s->bline;
	l->vbialloc = 0;
	l->lut      = NULL;
	
	/* Read the next line */
	x = l->width;
	while(x > 0)
	{
		i = fread(l->output, sizeof(int16_t), x, s->raw_bb_file);
		if(i < x && feof(s->raw_bb_file))
		{
			rewind(s->raw_bb_file);
		}
		
		x -= i;
	}
	
	/* Move samples into I channel and scale for output */
	for(x = l->width - 1; x >= 0; x--)
	{
		l->output[x * 2] = s->blanking_level +
			(((int) l->output[x] - s->conf.raw_bb_blanking_level) * (s->white_level - s->blanking_level) / (s->conf.raw_bb_white_level - s->conf.raw_bb_blanking_level));
	}
	
	/* Clear the Q channel */
	for(x = 0; x < s->max_width; x++)
	{
		l->output[x * 2 + 1] = 0;
	}
	
	return(1);
}

static int _vid_next_line_raster(vid_t *s, void *arg, int nlines, vid_line_t **lines)
{
	const char *seq;
	int x;
	int vy;
	int pal = 0;
	int fsc = 0;
	uint8_t sc = 0;
	int al, ar;
	vid_line_t *l = lines[1];
	
	l->width    = s->width;
	l->frame    = s->bframe;
	l->line     = s->bline;
	l->vbialloc = 0;
	l->lut      = NULL;
	
	/* Sequence codes: abcd
	 * 
	 * a: first sync
	 *    h = horizontal sync pulse
	 *    v = short vertical sync pulse
	 *    V = long vertical sync pulse
	 *    _ = no sync pulse
	 * 
	 * b: colour burst
	 *    0 = line always has a colour burst
	 *    _ = line never has a colour burst
	 *    1 = line has a colour burst on odd frames
	 *    2 = line has a colour burst on even frames
	 * 
	 * c: left content
	 *    _ = blanking
	 *    a = active video
	 * 
	 * d: right content
	 *    _ = blanking
	 *    a = active video
	 *    v = short vertical sync pulse
	 *    V = long vertical sync pulse
	 * 
	 **** I don't like this code, it's overly complicated for all it does.
	*/
	
	vy = -1;
	seq = "____";
	
	if(s->conf.type == VID_RASTER_625)
	{
		switch(l->line)
		{
		case 1:   seq = "V__V"; break;
		case 2:   seq = "V__V"; break;
		case 3:   seq = "V__v"; break;
		case 4:   seq = "v__v"; break;
		case 5:   seq = "v__v"; break;
		case 6:   seq = "h1__"; break;
		case 7:   seq = "h0__"; break;
		case 8:   seq = "h0__"; break;
		case 9:   seq = "h0__"; break;
		case 10:  seq = "h0__"; break;
		case 11:  seq = "h0__"; break;
		case 12:  seq = "h0__"; break;
		case 13:  seq = "h0__"; break;
		case 14:  seq = "h0__"; break;
		case 15:  seq = "h0__"; break;
		case 16:  seq = "h0__"; break;
		case 17:  seq = "h0__"; break;
		case 18:  seq = "h0__"; break;
		case 19:  seq = "h0__"; break;
		case 20:  seq = "h0__"; break;
		case 21:  seq = "h0__"; break;
		case 22:  seq = "h0__"; break;
		case 23:  seq = "h0_a"; break;
		
		case 310: seq = "h1aa"; break;
		case 311: seq = "v__v"; break;
		case 312: seq = "v__v"; break;
		case 313: seq = "v__V"; break;
		case 314: seq = "V__V"; break;
		case 315: seq = "V__V"; break;
		case 316: seq = "v__v"; break;
		case 317: seq = "v__v"; break;
		case 318: seq = "v___"; break;
		case 319: seq = "h2__"; break;
		case 320: seq = "h0__"; break;
		case 321: seq = "h0__"; break;
		case 322: seq = "h0__"; break;
		case 323: seq = "h0__"; break;
		case 324: seq = "h0__"; break;
		case 325: seq = "h0__"; break;
		case 326: seq = "h0__"; break;
		case 327: seq = "h0__"; break;
		case 328: seq = "h0__"; break;
		case 329: seq = "h0__"; break;
		case 330: seq = "h0__"; break;
		case 331: seq = "h0__"; break;
		case 332: seq = "h0__"; break;
		case 333: seq = "h0__"; break;
		case 334: seq = "h0__"; break;
		case 335: seq = "h0__"; break;
		
		case 622: seq = "h1aa"; break;
		case 623: seq = "h_av"; break;
		case 624: seq = "v__v"; break;
		case 625: seq = "v__v"; break;
		
		default:  seq = "h0aa"; break;
		}
		
		/* Calculate the active line number */
		vy = (l->line < 313 ? (l->line - 23) * 2 : (l->line - 336) * 2 + 1);
	}
	else if(s->conf.type == VID_RASTER_525)
	{
		switch(l->line)
		{
		case 1:   seq = "v__v"; break;
		case 2:   seq = "v__v"; break;
		case 3:   seq = "v__v"; break;
		case 4:   seq = "V__V"; break;
		case 5:   seq = "V__V"; break;
		case 6:   seq = "V__V"; break;
		case 7:   seq = "v__v"; break;
		case 8:   seq = "v__v"; break;
		case 9:   seq = "v__v"; break;
		case 10:  seq = "h0__"; break;
		case 11:  seq = "h0__"; break;
		case 12:  seq = "h0__"; break;
		case 13:  seq = "h0__"; break;
		case 14:  seq = "h0__"; break;
		case 15:  seq = "h0__"; break;
		case 16:  seq = "h0__"; break;
		case 17:  seq = "h0__"; break;
		case 18:  seq = "h0__"; break;
		case 19:  seq = "h0__"; break;
		case 20:  seq = "h0__"; break;
		
		case 263: seq = "h0av"; break;
		case 264: seq = "v__v"; break;
		case 265: seq = "v__v"; break;
		case 266: seq = "v__V"; break;
		case 267: seq = "V__V"; break;
		case 268: seq = "V__V"; break;
		case 269: seq = "V__v"; break;
		case 270: seq = "v__v"; break;
		case 271: seq = "v__v"; break;
		case 272: seq = "v___"; break;
		case 273: seq = "h0__"; break;
		case 274: seq = "h0__"; break;
		case 275: seq = "h0__"; break;
		case 276: seq = "h0__"; break;
		case 277: seq = "h0__"; break;
		case 278: seq = "h0__"; break;
		case 279: seq = "h0__"; break;
		case 280: seq = "h0__"; break;
		case 281: seq = "h0__"; break;
		case 282: seq = "h0__"; break;
		case 283: seq = "h0_a"; break;
		
		default:  seq = "h0aa"; break;
		}
		
		/* Calculate the active line number */
		
		/* There are 486 lines in this mode with some active video,
		 * but encoded files normally only have 480 of these. Here
		 * we use the line numbers suggested by SMPTE Recommended
		 * Practice RP-202. Lines 23-262 from the first field and
		 * 286-525 from the second. */
		
		vy = (l->line < 265 ? (l->line - 23) * 2 : (l->line - 286) * 2 + 1);
	}
	else if(s->conf.type == VID_RASTER_819)
	{
		switch(l->line)
		{
		case 817: seq = "h___"; break;
		case 818: seq = "h___"; break;
		case 819: seq = "h___"; break;
		case 1:   seq = "V___"; break;
		case 2:   seq = "h___"; break;
		case 3:   seq = "h___"; break;
		case 4:   seq = "h___"; break;
		case 5:   seq = "h___"; break;
		case 6:   seq = "h___"; break;
		case 7:   seq = "h___"; break;
		case 8:   seq = "h___"; break;
		case 9:   seq = "h___"; break;
		case 10:  seq = "h___"; break;
		case 11:  seq = "h___"; break;
		case 12:  seq = "h___"; break;
		case 13:  seq = "h___"; break;
		case 14:  seq = "h___"; break;
		case 15:  seq = "h___"; break;
		case 16:  seq = "h___"; break;
		case 17:  seq = "h___"; break;
		case 18:  seq = "h___"; break;
		case 19:  seq = "h___"; break;
		case 20:  seq = "h___"; break;
		case 21:  seq = "h___"; break;
		case 22:  seq = "h___"; break;
		case 23:  seq = "h___"; break;
		case 24:  seq = "h___"; break;
		case 25:  seq = "h___"; break;
		case 26:  seq = "h___"; break;
		case 27:  seq = "h___"; break;
		case 28:  seq = "h___"; break;
		case 29:  seq = "h___"; break;
		case 30:  seq = "h___"; break;
		case 31:  seq = "h___"; break;
		case 32:  seq = "h___"; break;
		case 33:  seq = "h___"; break;
		case 34:  seq = "h___"; break;
		case 35:  seq = "h___"; break;
		case 36:  seq = "h___"; break;
		case 37:  seq = "h___"; break;
		case 38:  seq = "h___"; break;
		
		case 406: seq = "h_a_"; break;
		case 407: seq = "h___"; break;
		case 408: seq = "h___"; break;
		case 409: seq = "h__V"; break;
		case 410: seq = "h___"; break;
		case 411: seq = "h___"; break;
		case 412: seq = "h___"; break;
		case 413: seq = "h___"; break;
		case 414: seq = "h___"; break;
		case 415: seq = "h___"; break;
		case 416: seq = "h___"; break;
		case 417: seq = "h___"; break;
		case 418: seq = "h___"; break;
		case 419: seq = "h___"; break;
		case 420: seq = "h___"; break;
		case 421: seq = "h___"; break;
		case 422: seq = "h___"; break;
		case 423: seq = "h___"; break;
		case 424: seq = "h___"; break;
		case 425: seq = "h___"; break;
		case 426: seq = "h___"; break;
		case 427: seq = "h___"; break;
		case 428: seq = "h___"; break;
		case 429: seq = "h___"; break;
		case 430: seq = "h___"; break;
		case 431: seq = "h___"; break;
		case 432: seq = "h___"; break;
		case 433: seq = "h___"; break;
		case 434: seq = "h___"; break;
		case 435: seq = "h___"; break;
		case 436: seq = "h___"; break;
		case 437: seq = "h___"; break;
		case 438: seq = "h___"; break;
		case 439: seq = "h___"; break;
		case 440: seq = "h___"; break;
		case 441: seq = "h___"; break;
		case 442: seq = "h___"; break;
		case 443: seq = "h___"; break;
		case 444: seq = "h___"; break;
		case 445: seq = "h___"; break;
		case 446: seq = "h___"; break;
		case 447: seq = "h__a"; break;
		
		default:  seq = "h_aa"; break;
		}
		
		/* Calculate the active line number */
		vy = (l->line < 406 ? (l->line - 48) * 2 : (l->line - 457) * 2 + 1);
	}
	else if(s->conf.type == VID_RASTER_405)
	{
		switch(l->line)
		{
		case 1:   seq = "V__V"; break;
		case 2:   seq = "V__V"; break;
		case 3:   seq = "V__V"; break;
		case 4:   seq = "V__V"; break;
		case 5:   seq = "h0__"; break;
		case 6:   seq = "h0__"; break;
		case 7:   seq = "h0__"; break;
		case 8:   seq = "h0__"; break;
		case 9:   seq = "h0__"; break;
		case 10:  seq = "h0__"; break;
		case 11:  seq = "h0__"; break;
		case 12:  seq = "h0__"; break;
		case 13:  seq = "h0__"; break;
		case 14:  seq = "h0__"; break;
		case 15:  seq = "h0__"; break;
		
		case 203: seq = "h0aV"; break;
		case 204: seq = "V__V"; break;
		case 205: seq = "V__V"; break;
		case 206: seq = "V__V"; break;
		case 207: seq = "V___"; break;
		case 208: seq = "h0__"; break;
		case 209: seq = "h0__"; break;
		case 210: seq = "h0__"; break;
		case 211: seq = "h0__"; break;
		case 212: seq = "h0__"; break;
		case 213: seq = "h0__"; break;
		case 214: seq = "h0__"; break;
		case 215: seq = "h0__"; break;
		case 216: seq = "h0__"; break;
		case 217: seq = "h0__"; break;
		case 218: seq = "h0_a"; break;
		
		default:  seq = "h0aa"; break;
		}
		
		/* Calculate the active line number */
		vy = (l->line < 210 ? (l->line - 16) * 2 : (l->line - 219) * 2 + 1);
	}
	else if(s->conf.type == VID_CBS_405)
	{
		switch(l->line)
		{
		case 1:   seq = "v__v"; break;
		case 2:   seq = "v__v"; break;
		case 3:   seq = "v__v"; break;
		case 4:   seq = "V__V"; break;
		case 5:   seq = "V__V"; break;
		case 6:   seq = "V__V"; break;
		case 7:   seq = "v__v"; break;
		case 8:   seq = "v__v"; break;
		case 9:   seq = "v__v"; break;
		case 10:  seq = "h___"; break;
		case 11:  seq = "h___"; break;
		case 12:  seq = "h___"; break;
		case 13:  seq = "h___"; break;
		case 14:  seq = "h___"; break;
		
		case 203: seq = "h_av"; break;
		case 204: seq = "v__v"; break;
		case 205: seq = "v__v"; break;
		case 206: seq = "v__V"; break;
		case 207: seq = "V__V"; break;
		case 208: seq = "V__V"; break;
		case 209: seq = "V__v"; break;
		case 210: seq = "v__v"; break;
		case 211: seq = "v__v"; break;
		case 212: seq = "v___"; break;
		case 213: seq = "h___"; break;
		case 214: seq = "h___"; break;
		case 215: seq = "h___"; break;
		case 216: seq = "h___"; break;
		case 217: seq = "h__a"; break;
		
		default:  seq = "h_aa"; break;
		}
		
		/* Calculate the active line number */
		vy = (l->line < 210 ? (l->line - 16) * 2 : (l->line - 219) * 2 + 1);
	}
	else if(s->conf.type == VID_APOLLO_320)
	{
		if(l->line <= 8) seq = "V__v";
		else seq = "h_aa";
		
		vy = l->line - 9;
		if(vy < 0 || vy >= s->conf.active_lines) vy = -1;
	}
	else if(s->conf.type == VID_BAIRD_240)
	{
		switch(l->line)
		{
		case 1:   seq = "V__V"; break;
		case 2:   seq = "V__V"; break;
		case 3:   seq = "V__V"; break;
		case 4:   seq = "V__V"; break;
		case 5:   seq = "V__V"; break;
		case 6:   seq = "V__V"; break;
		case 7:   seq = "V__V"; break;
		case 8:   seq = "V__V"; break;
		case 9:   seq = "V__V"; break;
		case 10:  seq = "V__V"; break;
		case 11:  seq = "V__V"; break;
		case 12:  seq = "V__V"; break;
		case 13:  seq = "h___"; break;
		case 14:  seq = "h___"; break;
		case 15:  seq = "h___"; break;
		case 16:  seq = "h___"; break;
		case 17:  seq = "h___"; break;
		case 18:  seq = "h___"; break;
		case 19:  seq = "h___"; break;
		case 20:  seq = "h___"; break;
		
		default:  seq = "h_aa"; break;
		}
		
		/* Calculate the active line number */
		vy = l->line - 20;
	}
	else if(s->conf.type == VID_BAIRD_30)
	{
		/* The original Baird 30 line standard has no sync pulses */
		seq = "__aa";
		vy = l->line - 1;
	}
	else if(s->conf.type == VID_NBTV_32)
	{
		switch(l->line)
		{
		case 1:  seq = "__aa"; break;
		default: seq = "h_aa"; break;
		}
		
		vy = l->line - 1;
	}
	
	/* Shift the lines by one if the source
	 * video has the bottom field first */
	if(vy >= 0 && s->vframe.interlaced == 2) vy += 1;
	
	/* Centre the video vertically */
	vy -= s->vframe_y;
	
	/* Check for out of bounds */
	if(vy < 0 || vy >= s->vframe.height) vy = -1;
	
	if(s->conf.colour_mode == VID_PAL ||
	   s->conf.colour_mode == VID_NTSC)
	{
		/* Does this line use colour? */
		pal  = seq[1] == '0';
		pal |= seq[1] == '1' && (l->frame & 1) == 0;
		pal |= seq[1] == '2' && (l->frame & 1) == 1;
		
		/* Calculate colour sub-carrier lookup-positions for the start of this line */
		l->lut = &s->colour_lookup[s->colour_lookup_offset];
		
		/* Update offset for the next line */
		s->colour_lookup_offset += s->width;
		s->colour_lookup_offset %= s->colour_lookup_width;
		
		if(s->conf.colour_mode == VID_PAL && pal &&
		   (l->frame + l->line) & 1)
		{
			pal = -1;
		}
	}
	if(s->conf.colour_mode == VID_APOLLO_FSC)
	{
		/* Apollo Field Sequential Colour */
		fsc = (l->frame * 2 + (l->line < 264 ? 0 : 1)) % 3;
		pal = 0;
	}
	else if(s->conf.colour_mode == VID_CBS_FSC)
	{
		/* CBS Field Sequential Colour */
		fsc = (l->frame * 2 + (l->line < 202 ? 0 : 1)) % 3;
		pal = 0;
	}
	
	/* Blank the next line */
	for(x = 0; x < s->width; x++)
	{
		lines[2]->output[x * 2] = s->blanking_level;
	}
	
	x = 0;
	
	/* Draw the sync pulses */
	sc = 0x00;
	
	/* Left sync pulse */
	if(seq[0] == 'h')      sc |= 1 << 0;
	else if(seq[0] == 'v') sc |= 1 << 1;
	else if(seq[0] == 'V') sc |= 1 << 2;
	
	/* Middle sync pulse */
	if(seq[3] == 'v')      sc |= 1 << 3;
	else if(seq[3] == 'V') sc |= 1 << 4;
	
	if(sc)
	{
		vbidata_render(s->syncs, &sc, 0, 5, VBIDATA_LSB_FIRST, l);
	}
	
	/* Render the active video if required */
	if(seq[2] == 'a' || seq[3] == 'a')
	{
		uint32_t rgb = 0x000000;
		uint32_t *prgb = &rgb;
		int stride = 0;
		int16_t *o;
		
		/* Calculate active video portion of this line */
		al = (seq[2] == 'a' ? s->active_left : (seq[3] == 'a' ? s->half_width : -1));
		ar = (seq[3] == 'a' ? s->active_left + s->active_width : (seq[2] == 'a' ? s->half_width : -1));
		
		for(x = al, o = &l->output[al * 2]; x < s->active_left + s->vframe_x; x++, o += 2)
		{
			*o = s->yiq_level_lookup[0x000000].y;
		}
		
		if(s->vframe.framebuffer && vy >= 0)
		{
			prgb  = &s->vframe.framebuffer[vy * s->vframe.line_stride];
			prgb += (x - s->active_left - s->vframe_x) * s->vframe.pixel_stride;
			stride = s->vframe.pixel_stride;
		}
		
		for(; x < s->active_left + s->vframe_x + s->vframe.width && x < ar; x++, o += 2, prgb += stride)
		{
			rgb = *prgb & 0xFFFFFF;
			
			if(s->conf.colour_mode == VID_APOLLO_FSC ||
			   s->conf.colour_mode == VID_CBS_FSC)
			{
				rgb  = (rgb >> (8 * fsc)) & 0xFF;
				rgb |= (rgb << 8) | (rgb << 16);
			}
			
			*o = s->yiq_level_lookup[rgb].y;
			
			if(pal)
			{
				*o += (s->yiq_level_lookup[rgb].i * l->lut[x].q +
				       s->yiq_level_lookup[rgb].q * l->lut[x].i * pal) >> 15;
			}
		}
		
		for(; x < ar; x++, o += 2)
		{
			*o = s->yiq_level_lookup[0x000000].y;
		}
	}
	
	/* Render the colour burst */
	if(pal)
	{
		for(x = s->burst_left; x < s->burst_left + s->burst_width; x++)
		{
			l->output[x * 2] += (((s->burst_phase.i * l->lut[x].q +
			                       s->burst_phase.q * l->lut[x].i * pal) >> 15) * s->burst_win[x - s->burst_left]) >> 15;
		}
	}
	
	/* Render the Apollo FSC flag */
	if(s->conf.colour_mode == VID_APOLLO_FSC && fsc == 1 &&
	  (l->line == 18 || l->line == 281))
	{
		/* The Apollo colour standard transmits one colour per field
		 * (Blue, Red, Green), with the green field indicated by a flag
		 * on field line 18. The flag also indicates the temperature of
		 * the camera by its duration, varying between 5 and 45 μs. The
		 * duration is fixed to 20 μs in hacktv. */
		
		sc = 1;
		vbidata_render(s->fsc_syncs, &sc, 0, 1, VBIDATA_LSB_FIRST, l);
	}
	
	/* Render the CBS FSC flag */
	if(s->conf.colour_mode == VID_CBS_FSC && fsc == 2 &&
	  (l->line == 1 || l->line == 203))
	{
		sc = 1 << (l->line == 1 ? 0 : 1);
		vbidata_render(s->fsc_syncs, &sc, 0, 2, VBIDATA_LSB_FIRST, l);
	}
	
	/* Render the SECAM colour subcarrier */
	if(s->conf.colour_mode == VID_SECAM)
	{
		const cint16_t *g;
		int16_t dmin, dmax;
		int sl = 0, sr = 0;
		
		if(s->conf.secam_field_id &&
		   ((l->line >= 7 && l->line <= 15) ||
		    (l->line >= 320 && l->line <= 328)))
		{
			int16_t level;
			int16_t dev;
			double rw;
			
			if(((l->frame * s->conf.lines) + l->line) & 1)
			{
				level = s->yiq_level_lookup[0x000000].q; // D'r
				dev = s->secam_fsync_level;
				rw = 15e-6;
			}
			else
			{
				level = s->yiq_level_lookup[0x000000].i; // D'b
				dev = -s->secam_fsync_level;
				rw = 18e-6;
			}
			
			for(x = 0; x < s->width; x++)
			{
				double t = (double) (x - s->active_left) / s->pixel_rate / rw;
				
				if(t < 0) t = 0;
				else if(t > 1) t = 1;
				
				l->output[x * 2 + 1] = level + dev * t;
			}
			
			sl = s->burst_left;
			sr = sl + s->burst_width;
			
			l->vbialloc = 1;
		}
		else if(seq[2] == 'a' || seq[3] == 'a')
		{
			uint32_t rgb = 0x000000;
			uint32_t *prgb = &rgb;
			int stride = 0;
			
			if(s->vframe.framebuffer && vy >= 0)
			{
				prgb = &s->vframe.framebuffer[vy * s->vframe.line_stride];
				stride = s->vframe.pixel_stride;
			}
			
			if(((l->frame * s->conf.lines) + l->line) & 1)
			{
				/* D'r */
				
				for(x = 0; x < s->active_left + s->vframe_x; x++)
				{
					l->output[x * 2 + 1] = s->yiq_level_lookup[0x000000].q;
				}
				
				for(; x < s->active_left + s->vframe_x + s->vframe.width; x++, prgb += stride)
				{
					l->output[x * 2 + 1] = s->yiq_level_lookup[*prgb & 0xFFFFFF].q;
				}
				
				for(; x < s->width; x++)
				{
					l->output[x * 2 + 1] = s->yiq_level_lookup[0x000000].q;
				}
			}
			else
			{
				/* D'b */
				
				for(x = 0; x < s->active_left + s->vframe_x; x++)
				{
					l->output[x * 2 + 1] = s->yiq_level_lookup[0x000000].i;
				}
				
				for(; x < s->active_left + s->vframe_x + s->vframe.width; x++, prgb += stride)
				{
					l->output[x * 2 + 1] = s->yiq_level_lookup[*prgb & 0xFFFFFF].i;
				}
				
				for(; x < s->width; x++)
				{
					l->output[x * 2 + 1] = s->yiq_level_lookup[0x000000].i;
				}
			}
			
			sl = s->burst_left;
			sr = seq[3] == 'a' ? sl + s->burst_width : s->half_width;
		}
		
		if(sr > sl)
		{
			fir_int16_process_block(&s->secam_l_fir, l->output + s->active_left * 2, l->output + s->active_left * 2, s->active_width, 2);
			fir_int16_process_block(&s->fm_secam_fir, l->output + 1, l->output + 1, s->width, 2);
			iir_int16_process(&s->fm_secam_iir, l->output + 1, l->output + 1, s->width, 2);
			
			/* Reset the SECAM FM phase every line, alternating every third line */
			s->fm_secam.counter = INT16_MAX;
			s->fm_secam.phase.i = ((l->frame * s->conf.lines) + l->line) % 3 == 0 ? INT32_MAX : -INT32_MAX;
			s->fm_secam.phase.q = 0;
			
			/* Limit the FM deviation */
			dmin = s->fm_secam_dmin[((l->frame * s->conf.lines) + l->line) & 1];
			dmax = s->fm_secam_dmax[((l->frame * s->conf.lines) + l->line) & 1];
			
			for(x = sl; x < sr; x++)
			{
				if(l->output[x * 2 + 1] < dmin) l->output[x * 2 + 1] = dmin;
				else if(l->output[x * 2 + 1] > dmax) l->output[x * 2 + 1] = dmax;
				
				g = &s->fm_secam_bell[(uint16_t) l->output[x * 2 + 1]];
				_fm_modulator_cgain(&s->fm_secam, &l->output[x * 2 + 1], l->output[x * 2 + 1], g);
				
				l->output[x * 2] += (l->output[x * 2 + 1] * s->burst_win[x - s->burst_left]) >> 15;
			}
		}
	}
	
	/* Clear the Q channel */
	for(x = 0; x < s->max_width; x++)
	{
		l->output[x * 2 + 1] = 0;
	}
	
	return(1);
}

static int _vid_filter_process(vid_t *s, void *arg, int nlines, vid_line_t **lines)
{
	_vid_filter_process_t *p = arg;
	vid_line_t *dst = lines[0];
	vid_line_t *src = lines[nlines - 1];
	
	dst->width = fir_int16_process(&p->fir, dst->output, src->output, src->width, 2);
	
	return(1);
}

static void _vid_filter_free(vid_t *s, void *arg)
{
	_vid_filter_process_t *p = arg;
	
	fir_int16_free(&p->fir);
	free(p);
}

static int _vid_audio_process(vid_t *s, void *arg, int nlines, vid_line_t **lines)
{
	vid_line_t *l = lines[0];
	int16_t audio[2] = { 0, 0 };
	int x;
	
	for(x = 0; x < l->width; x++)
	{
		int16_t add[2] = { 0, 0 };
		
		/* TODO: Replace this with a real FIR filter... */
		s->interp += HACKTV_AUDIO_SAMPLE_RATE;
		if(s->interp >= s->sample_rate)
		{
			s->interp -= s->sample_rate;
			
			if(s->audiobuffer_samples == 0)
			{
				s->audiobuffer = av_read_audio(&s->av, &s->audiobuffer_samples);
				
				if(s->conf.systeraudio == 1)
				{
					ng_invert_audio(&s->ng, s->audiobuffer, s->audiobuffer_samples);
				}
			}
			
			if(s->audiobuffer)
			{
				/* Fetch next sample */
				audio[0] = s->audiobuffer[0];
				audio[1] = s->audiobuffer[1];
				s->audiobuffer += 2;
				s->audiobuffer_samples--;
			}
			else
			{
				/* No audio from the source */
				audio[0] = 0;
				audio[1] = 0;
			}
			
			if(s->conf.am_audio_level > 0 && s->conf.am_mono_carrier != 0)
			{
				s->am_mono.sample = (audio[0] + audio[1]) / 2;
			}
			
			if(s->conf.fm_mono_level > 0 && s->conf.fm_mono_carrier != 0)
			{
				s->fm_mono.sample = (audio[0] + audio[1]) / 2;
				if(s->fm_mono.limiter.width)
				{
					limiter_process(&s->fm_mono.limiter, &s->fm_mono.sample, &s->fm_mono.sample, &s->fm_mono.sample, 1, 1);
				}
				
				/* Reduce volume of audio in A2 Stereo mode to
				 * leave room for the pilot/mode signal */
				if(s->conf.a2stereo) s->fm_mono.sample *= 0.95;
			}
			
			if(s->conf.fm_left_level > 0 && s->conf.fm_left_carrier != 0)
			{
				s->fm_left.sample = audio[0];
				if(s->fm_left.limiter.width)
				{
					limiter_process(&s->fm_left.limiter, &s->fm_left.sample, &s->fm_left.sample, &s->fm_left.sample, 1, 1);
				}
			}
			
			if(s->conf.fm_right_level > 0 && s->conf.fm_right_carrier != 0)
			{
				s->fm_right.sample = audio[1];
				if(s->fm_right.limiter.width)
				{
					limiter_process(&s->fm_right.limiter, &s->fm_right.sample, &s->fm_right.sample, &s->fm_right.sample, 1, 1);
				}
				
				/* Reduce volume of audio in A2 Stereo mode to
				 * leave room for the pilot/mode signal */
				if(s->conf.a2stereo) s->fm_right.sample *= 0.95;
			}
			
			if((s->conf.nicam_level > 0 && s->conf.nicam_carrier != 0) ||
			   s->conf.type == VID_MAC || s->conf.sis)
			{
				s->nicam_buf[s->nicam_buf_len++] = audio[0];
				s->nicam_buf[s->nicam_buf_len++] = audio[1];
				
				if(s->nicam_buf_len == NICAM_AUDIO_LEN * 2)
				{
					if(s->conf.nicam_level > 0 && s->conf.nicam_carrier != 0)
					{
						nicam_mod_input(&s->nicam, s->nicam_buf);
					}
					
					if(s->conf.type == VID_MAC)
					{
						mac_write_audio(s, &s->mac.audio, 0, s->nicam_buf, NICAM_AUDIO_LEN * 2);
					}
					
					if(s->conf.sis)
					{
						sis_write_audio(&s->sis, s->nicam_buf);
					}
					
					s->nicam_buf_len = 0;
				}
			}
			
			if(s->conf.dance_level > 0 && s->conf.dance_carrier != 0)
			{
				s->dance_buf[s->dance_buf_len++] = audio[0];
				s->dance_buf[s->dance_buf_len++] = audio[1];
				
				if(s->dance_buf_len == DANCE_A_AUDIO_LEN * 2)
				{
					dance_mod_input(&s->dance, s->dance_buf);
					s->dance_buf_len = 0;
				}
			}
		}
		
		if(s->conf.fm_mono_level > 0 && s->conf.fm_mono_carrier != 0)
		{
			_fm_modulator_add(&s->fm_mono, add, s->fm_mono.sample);
		}
		
		if(s->conf.fm_left_level > 0 && s->conf.fm_left_carrier != 0)
		{
			_fm_modulator_add(&s->fm_left, add, s->fm_left.sample);
		}
		
		if(s->conf.fm_right_level > 0 && s->conf.fm_right_carrier != 0)
		{
			int16_t a2 = s->fm_right.sample;
			
			if(s->conf.a2stereo)
			{
				int16_t s1[2] = { 0, 0 };
				int16_t s2[2] = { 0, 0 };
				
				if(s->a2stereo_system_m)
				{
					/* The System M variant is L-R, not R */
					a2 = s->fm_mono.sample - s->fm_right.sample;
				}
				
				/* Add the pilot tone */
				_am_modulator_add(&s->a2stereo_signal, s1, 0);
				_am_modulator_add(&s->a2stereo_pilot, s2, s1[0]);
				a2 += s2[0];
			}
			
			_fm_modulator_add(&s->fm_right, add, a2);
		}
		
		if(s->conf.am_audio_level > 0 && s->conf.am_mono_carrier != 0)
		{
			_am_modulator_add(&s->am_mono, add, s->am_mono.sample);
		}
		
		l->output[x * 2 + 0] += add[0];
		l->output[x * 2 + 1] += add[1];
	}
	
	if(s->conf.nicam_level > 0 && s->conf.nicam_carrier != 0)
	{
		nicam_mod_output(&s->nicam, l->output, l->width);
	}
	
	if(s->conf.dance_level > 0 && s->conf.dance_carrier != 0)
	{
		dance_mod_output(&s->dance, l->output, l->width);
	}
	
	return(1);
}

static int _vid_fmmod_process(vid_t *s, void *arg, int nlines, vid_line_t **lines)
{
	vid_line_t *l = lines[0];
	int x;
	
	/* FM modulate the video and audio if requested */
	for(x = 0; x < l->width; x++)
	{
		_fm_modulator(&s->fm_video, &l->output[x * 2], l->output[x * 2]);
	}
	
	return(1);
}

static int _vid_swap_iq_process(vid_t *s, void *arg, int nlines, vid_line_t **lines)
{
	vid_line_t *l = lines[0];
	int x;
	int16_t t;
	
	for(x = 0; x < l->width; x++)
	{
		t = l->output[x * 2 + 0];
		l->output[x * 2 + 0] = l->output[x * 2 + 1];
		l->output[x * 2 + 1] = t;
	}
	
	return(1);
}

static int _vid_offset_process(vid_t *s, void *arg, int nlines, vid_line_t **lines)
{
	vid_line_t *l = lines[0];
	int x;
	
	for(x = 0; x < l->width; x++)
	{
		cint16_t a, b;
		
		cint32_mul(&s->offset.phase, &s->offset.phase, &s->offset.delta);
		
		a.i = l->output[x * 2 + 0];
		a.q = l->output[x * 2 + 1];
		b.i = s->offset.phase.i >> 16;
		b.q = s->offset.phase.q >> 16;
		cint16_mul(&a, &a, &b);
		
		l->output[x * 2 + 0] = a.i;
		l->output[x * 2 + 1] = a.q;
		
		/* Correct the amplitude after INT16_MAX samples */
		if(--s->offset.counter == 0)
		{
			double ra = atan2(s->offset.phase.q, s->offset.phase.i);
			
			s->offset.phase.i = lround(cos(ra) * INT32_MAX);
			s->offset.phase.q = lround(sin(ra) * INT32_MAX);
			
			s->offset.counter = INT16_MAX;
		}
	}
	
	return(1);
}

static int _vid_passthru_process(vid_t *s, void *arg, int nlines, vid_line_t **lines)
{
	vid_line_t *l = lines[0];
	int x;
	
	if(feof(s->passthru))
	{
		return(1);
	}

  size_t ret = fread(s->passline, sizeof(int16_t) * 2, l->width, s->passthru);
  assert(ret == l->width);

	for(x = 0; x < l->width * 2; x++)
	{
		l->output[x] += s->passline[x];
	}
	
	return(1);
}

static int _add_lineprocess(vid_t *s, const char *name, int nlines, void *arg, vid_lineprocess_process_t pprocess, vid_lineprocess_free_t pfree)
{
	_lineprocess_t *p;
	
	p = realloc(s->processes, sizeof(_lineprocess_t) * (s->nprocesses + 1));
	if(!p)
	{
		return(VID_OUT_OF_MEMORY);
	}
	
	s->processes = p;
	p = &s->processes[s->nprocesses++];
	
	strncpy(p->name, name, 15);
	p->vid = s;
	p->nlines = nlines;
	p->arg = arg;
	p->process = pprocess;
	p->free = pfree;
	
	p->lines = calloc(sizeof(vid_line_t *), nlines);
	if(!p->lines)
	{
		return(VID_OUT_OF_MEMORY);
	}
	
	/* Update required line total (non-threaded version) */
	s->olines += nlines - 1;
	
	return(VID_OK);
}

static int _calc_filter_delay(int width, int ntaps)
{
	int delay;
	
	/* Calculate the number of samples delay needed
	 * to make filter delay exactly N lines */
	ntaps /= 2;
	
	delay = (ntaps + width - 1) / width;
	delay = width * delay - ntaps;
	
	return(delay);
}

static int _init_vresampler(vid_t *s)
{
	_vid_filter_process_t *p;
	int width;
	
	p = calloc(1, sizeof(_vid_filter_process_t));
	if(!p)
	{
		return(VID_OUT_OF_MEMORY);
	}
	
	fir_int16_resampler_init(&p->fir, s->sample_rate, s->pixel_rate);
	
	/* Update maximum line width */
	width = (s->width * p->fir.interpolation + p->fir.decimation - 1) / p->fir.decimation;
	if(width > s->max_width) s->max_width = width;
	
	_add_lineprocess(s, "vresampler", 2, p, _vid_filter_process, _vid_filter_free);
	
	return(VID_OK);
}	

static int _init_vfilter(vid_t *s)
{
	_vid_filter_process_t *p;
	int ntaps = 0;
	int width;
	int delay;
	
	width = round((double) s->sample_rate / ((double) s->conf.frame_rate.num / s->conf.frame_rate.den) / s->conf.lines);
	
	p = calloc(1, sizeof(_vid_filter_process_t));
	if(!p)
	{
		return(VID_OUT_OF_MEMORY);
	}
	
	if(s->conf.modulation == VID_VSB)
	{
		double taps[51 * 2];
		
		ntaps = 51;
		
		fir_complex_band_pass(taps, ntaps, s->sample_rate, -s->conf.vsb_lower_bw, s->conf.vsb_upper_bw, 750000, 1);
		fir_int16_scomplex_init(&p->fir, taps, ntaps, 1, 1, _calc_filter_delay(width, ntaps));
	}
	else if(s->conf.modulation == VID_FM)
	{
		const double *taps;
		
		if(s->conf.type == VID_MAC)
		{
			if(s->sample_rate != 20250000)
			{
				fprintf(stderr, "Warning: The D/D2-MAC pre-emphasis filter is designed to run at 20.25 MHz.\n");
			}
			
			taps = fm_mac_taps;
			ntaps = sizeof(fm_mac_taps) / sizeof(double);
		}
		else if(s->conf.lines == 525)
		{
			if(s->sample_rate == 18000000)
			{
				taps = fm_525_18_taps;
				ntaps = sizeof(fm_525_18_taps) / sizeof(double);
			}
			else
			{
				if(s->sample_rate != 20250000)
				{
					fprintf(stderr, "Warning: The 525-line FM video pre-emphasis filters are designed to run at 18 MHz or 20.25 MHz.\n");
				}
				
				taps = fm_525_2025_taps;
				ntaps = sizeof(fm_525_2025_taps) / sizeof(double);
			}
		}
		else
		{
			if(s->sample_rate == 14000000)
			{
				taps = fm_625_14_taps;
				ntaps = sizeof(fm_625_14_taps) / sizeof(double);
			}
			else if(s->sample_rate == 20000000)
			{
				taps = fm_625_20_taps;
				ntaps = sizeof(fm_625_20_taps) / sizeof(double);
			}
			else if(s->sample_rate == 28000000)
			{
				taps = fm_625_28_taps;
				ntaps = sizeof(fm_625_28_taps) / sizeof(double);
			}
			else
			{
				if(s->sample_rate != 20250000)
				{
					fprintf(stderr, "Warning: The 625-line FM video pre-emphasis filters are designed to run at 14 MHz, 20 MHz, 20.25 MHz or 28 MHz.\n");
				}
				
				taps = fm_625_2025_taps;
				ntaps = sizeof(fm_625_2025_taps) / sizeof(double);
			}
		}
		
		fir_int16_init(&p->fir, taps, ntaps, 1, 1, _calc_filter_delay(width, ntaps));
	}
	else if(s->conf.modulation == VID_AM ||
	        s->conf.modulation == VID_NONE)
	{
		double taps[51];
		
		ntaps = 51;
		
		fir_low_pass(taps, ntaps, s->sample_rate, s->conf.video_bw, 0.75e6, 1);
		fir_int16_init(&p->fir, taps, ntaps, 1, 1, _calc_filter_delay(width, ntaps));
	}
	
	if(p->fir.type == 0)
	{
		/* No filter has been created */
		free(p);
		return(VID_OK);
	}
	
	delay = (ntaps / 2 + width - 1) / width;
	
	_add_lineprocess(s, "vfilter", 1 + delay, p, _vid_filter_process, _vid_filter_free);
	
	return(VID_OK);
}

vbidata_lut_t *_render_sync_pulses(vid_t *s, const double syncs[][4], int num)
{
	int i, l;
	vbidata_lut_t *lut;
	vbidata_lut_t *lptr;
	
	/* This function pre-renders sync pulses into a vbidata format table */
	
	/* Calculate the memory needed for the sync pulse table */
	for(l = i = 0; i < num; i++)
	{
		l += vbidata_update_step(
			NULL,
			syncs[i][0] * s->pixel_rate,		/* Offset */
			syncs[i][1] * s->pixel_rate,		/* Width */
			syncs[i][2] * RT1090 * s->pixel_rate,	/* Rise-time */
			syncs[i][3]				/* Level */
		);
	}
	l += 1;
	
	/* Allocate memory and render the sync pulses */
	lut = malloc(l * sizeof(int16_t));
	if(!lut)
	{
		return(NULL);
	}
	
	lptr = lut;
	for(i = 0; i < num; i++, lptr = (vbidata_lut_t *) &lptr->value[lptr->length])
	{
		vbidata_update_step(
			lptr,
			syncs[i][0] * s->pixel_rate,		/* Offset */
			syncs[i][1] * s->pixel_rate,		/* Width */
			syncs[i][2] * RT1090 * s->pixel_rate,	/* Rise-time */
			syncs[i][3]				/* Level */
		);
	}
	
	/* Mark the end of the table */
	lptr->length = -1;
	
	return(lut);
}

int vid_init(vid_t *s, unsigned int sample_rate, unsigned int pixel_rate, const vid_config_t * const conf)
{
	int r, x;
	int64_t c;
	double d;
	double glut[0x100];
	double width;
	double level, slevel;
	vid_line_t *l;
	
	/* Seed the system's PRNG, used by some of the video scramblers */
	srand(time(NULL));
	
	memset(s, 0, sizeof(vid_t));
	memcpy(&s->conf, conf, sizeof(vid_config_t));
	
	s->sample_rate = sample_rate;
	s->pixel_rate = pixel_rate ? pixel_rate : sample_rate;
	
	_test_sample_rate(&s->conf, s->pixel_rate);
	
	/* Calculate the number of samples per line */
	width = (double) s->conf.frame_rate.den / s->conf.frame_rate.num / s->conf.lines;
	
	s->width = round((double) s->pixel_rate * width);
	s->half_width = round((double) s->pixel_rate * width / 2);
	s->max_width = s->width;
	
	/* Calculate the active video width and offset */
	s->active_left = round(s->pixel_rate * s->conf.active_left);
	s->active_width = ceil(s->pixel_rate * s->conf.active_width);
	if(s->active_width > s->width) s->active_width = s->width;
	
	/* Calculate signal levels */
	/* slevel is the the sub-carrier level. When FM modulating
	 * this is always 1.0, otherwise it equals the overall level */
	slevel = s->conf.modulation == VID_FM ? 1.0 : s->conf.level;
	
	level = s->conf.video_level * slevel;
	
	/* Invert the video levels if requested */
	if(s->conf.invert_video)
	{
		double t;
		
		/* Swap sync and white levels */
		t = s->conf.white_level;
		s->conf.white_level = s->conf.sync_level;
		s->conf.sync_level = t;
		
		/* Adjust black and blanking levels */
		s->conf.blanking_level = s->conf.sync_level - (s->conf.blanking_level - s->conf.white_level);
		s->conf.black_level = s->conf.sync_level - (s->conf.black_level - s->conf.white_level);
	}
	
	/* Calculate 16-bit blank and sync levels */
	s->white_level    = round(s->conf.white_level    * level * INT16_MAX);
	s->black_level    = round(s->conf.black_level    * level * INT16_MAX);
	s->blanking_level = round(s->conf.blanking_level * level * INT16_MAX);
	s->sync_level     = round(s->conf.sync_level     * level * INT16_MAX);
	
	/* Pre-render the sync pulses */
	d = (s->conf.sync_level - s->conf.blanking_level) * level * INT16_MAX;
	s->syncs = _render_sync_pulses(s, (const double[][4]) {
		{ 0,         s->conf.hsync_width,       s->conf.sync_rise, d },
		{ 0,         s->conf.vsync_short_width, s->conf.sync_rise, d },
		{ 0,         s->conf.vsync_long_width,  s->conf.sync_rise, d },
		{ width / 2, s->conf.vsync_short_width, s->conf.sync_rise, d },
		{ width / 2, s->conf.vsync_long_width,  s->conf.sync_rise, d },
	}, 5);
	if(s->syncs == NULL)
	{
		return(VID_OUT_OF_MEMORY);
	}
	
	/* Allocate memory for YUV lookup tables */
	s->yiq_level_lookup = malloc(0x1000000 * sizeof(_yiq16_t));
	if(s->yiq_level_lookup == NULL)
	{
		vid_free(s);
		return(VID_OUT_OF_MEMORY);
	}
	
	/* Generate the gamma lookup table. LUTception */
	if(s->conf.gamma <= 0)
	{
		s->conf.gamma = 1.0;
	}
	
	for(c = 0; c < 0x100; c++)
	{
		glut[c] = pow((double) c / 255, 1 / s->conf.gamma);
	}
	
	/* Generate the RGB > signal level lookup tables */
	for(c = 0x000000; c <= 0xFFFFFF; c++)
	{
		double r, g, b;
		double y, u, v;
		double i, q;
		
		/* Calculate RGB 0..1 values */
		r = glut[(c & 0xFF0000) >> 16];
		g = glut[(c & 0x00FF00) >> 8];
		b = glut[(c & 0x0000FF) >> 0];
		
		/* Calculate Y, Cb and Cr values */
		y = r * s->conf.rw_co
		  + g * s->conf.gw_co
		  + b * s->conf.bw_co;
		u = (b - y);
		v = (r - y);
		
		i = s->conf.eu_co * u;
		q = s->conf.ev_co * v;
		
		/* Adjust values to correct signal level */
		y = (s->conf.black_level + (y * (s->conf.white_level - s->conf.black_level))) * level;
		
		if(s->conf.colour_mode != VID_SECAM)
		{
			i *= (s->conf.white_level - s->conf.black_level) * level;
			q *= (s->conf.white_level - s->conf.black_level) * level;
		}
		else
		{
			i = (i + SECAM_CB_FREQ - SECAM_FM_FREQ) / SECAM_FM_DEV;
			q = (q + SECAM_CR_FREQ - SECAM_FM_FREQ) / SECAM_FM_DEV;
		}
		
		/* Convert to INT16 range and store in tables */
		s->yiq_level_lookup[c].y = round(_dlimit(y, -1, 1) * INT16_MAX);
		s->yiq_level_lookup[c].i = round(_dlimit(i, -1, 1) * INT16_MAX);
		s->yiq_level_lookup[c].q = round(_dlimit(q, -1, 1) * INT16_MAX);
	}
	
	if(s->conf.colour_mode == VID_PAL ||
	   s->conf.colour_mode == VID_NTSC)
	{
		rational_t a = rational_div((rational_t) { s->pixel_rate, 1 }, s->conf.colour_carrier);
		
		/* Generate the colour subcarrier lookup table */
		/* This carrier is in phase with the U (B-Y) component */
		s->colour_lookup_width = a.num;
		d = 2.0 * M_PI * ((double) s->conf.colour_carrier.num / s->conf.colour_carrier.den) / s->pixel_rate;
		
		/*  To make overflow easier to handle the length of the table is extended by one line */
		s->colour_lookup = malloc((s->colour_lookup_width + s->width) * sizeof(cint16_t));
		if(!s->colour_lookup)
		{
			vid_free(s);
			return(VID_OUT_OF_MEMORY);
		}
		
		for(c = 0; c < s->colour_lookup_width + s->width; c++)
		{
			s->colour_lookup[c] = (cint16_t) {
				round(cos(d * c) * INT16_MAX),
				round(sin(d * c) * INT16_MAX)
			};
		}
		
		s->colour_lookup_offset = 0;
	}
	
	if(s->conf.burst_level > 0)
	{
		/* Generate the colour burst envelope */
		s->burst_left  = round(s->pixel_rate * (s->conf.burst_left - s->conf.burst_rise / 2));
		s->burst_win   = _burstwin(
			s->pixel_rate,
			s->conf.burst_width,
			s->conf.burst_rise,
			s->conf.burst_level * (s->conf.white_level - s->conf.blanking_level) / 2 * level,
			&s->burst_width
		);
		if(!s->burst_win)
		{
			vid_free(s);
			return(VID_OUT_OF_MEMORY);
		}
		
		if(s->conf.colour_mode == VID_PAL)
		{
			/* PAL has a 135° burst, alternating between + and - */
			double p = 135.0 * (M_PI / 180.0);
			s->burst_phase = (cint16_t) {
				round(cos(p) * INT16_MAX),
				round(sin(p) * INT16_MAX)
			};
		}
		else if(s->conf.colour_mode == VID_NTSC)
		{
			/* NTSC has a 180° burst */
			s->burst_phase = (cint16_t) { -INT16_MAX, 0 };
		}
	}
	
	/* Pre-render the FSC pulses */
	if(s->conf.colour_mode == VID_APOLLO_FSC)
	{
		d = (s->conf.fsc_flag_level - s->conf.blanking_level) * level * INT16_MAX;
		s->fsc_syncs = _render_sync_pulses(s, (const double[][4]) {
			{ s->conf.fsc_flag_left, s->conf.fsc_flag_width, s->conf.sync_rise, d },
		}, 1);
		if(s->fsc_syncs == NULL)
		{
			return(VID_OUT_OF_MEMORY);
		}
	}
	else if(s->conf.colour_mode == VID_CBS_FSC)
	{
		d = (s->conf.fsc_flag_level - s->conf.blanking_level) * level * INT16_MAX;
		s->fsc_syncs = _render_sync_pulses(s, (const double[][4]) {
			{ s->conf.fsc_flag_left,             s->conf.fsc_flag_width, s->conf.sync_rise, d },
			{ width / 2 + s->conf.fsc_flag_left, s->conf.fsc_flag_width, s->conf.sync_rise, d }, 
		}, 2);
		if(s->fsc_syncs == NULL)
		{
			return(VID_OUT_OF_MEMORY);
		}
	}
	
	if(s->conf.colour_mode == VID_SECAM)
	{
		double secam_level = (s->conf.white_level - s->conf.blanking_level) * level;
		double taps[51];
		
		r = _init_fm_modulator(&s->fm_secam, s->pixel_rate, SECAM_FM_FREQ, SECAM_FM_DEV, secam_level);
		if(r != VID_OK)
		{
			vid_free(s);
			return(r);
		}
		
		r = iir_int16_init(&s->fm_secam_iir,
			(const double [2]) { 1.0, -0.90456054 },
			(const double [2]) { 2.90456054, -2.80912108 }
		);
		if(r != VID_OK)
		{
			vid_free(s);
			return(r);
		}
		
		fir_low_pass(taps, 51, s->pixel_rate, 1.50e6, 0.50e6, 1.0);
		fir_int16_init(&s->fm_secam_fir, taps, 51, 1, 1, 0);
		
		fir_band_reject(taps, 51, s->pixel_rate, SECAM_FM_FREQ - 1e6, SECAM_FM_FREQ + 1e6, 1e6, 1.0);
		
		/* A little test to see if a weaker luminance filter can improve
		 * SECAM image quality without causing interference to colour */
		taps[51 / 2] += 0.5;
		fir_normalise(taps, 51, 1.0);
		
		fir_int16_init(&s->secam_l_fir, taps, 51, 1, 1, 0);
		
		/* FM deviation limits */
		s->fm_secam_dmin[0] = lround((SECAM_CB_FREQ - SECAM_FM_FREQ - 350e3) / SECAM_FM_DEV * INT16_MAX);
		s->fm_secam_dmax[0] = lround((SECAM_CB_FREQ - SECAM_FM_FREQ + 506e3) / SECAM_FM_DEV * INT16_MAX);
		s->fm_secam_dmin[1] = lround((SECAM_CR_FREQ - SECAM_FM_FREQ - 506e3) / SECAM_FM_DEV * INT16_MAX);
		s->fm_secam_dmax[1] = lround((SECAM_CR_FREQ - SECAM_FM_FREQ + 350e3) / SECAM_FM_DEV * INT16_MAX);
		
		s->fm_secam_bell = malloc(sizeof(cint16_t) * UINT16_MAX);
		if(!s->fm_secam_bell)
		{
			vid_free(s);
			return(VID_OUT_OF_MEMORY);
		}
		
		for(r = INT16_MIN; r <= INT16_MAX; r++)
		{
			double dg[2];
			_secam_g(dg, SECAM_FM_FREQ + (double) r * SECAM_FM_DEV / INT16_MAX);
			s->fm_secam_bell[(uint16_t) r].i = lround(dg[0] * INT16_MAX);
			s->fm_secam_bell[(uint16_t) r].q = lround(dg[1] * INT16_MAX);
		}
		
		/* Field sync levels (optional) */
		s->secam_fsync_level = round(350e3 / SECAM_FM_DEV * INT16_MAX);
		
		/* Generate the colour subcarrier envelope */
		s->burst_left  = round(s->pixel_rate * (s->conf.burst_left - s->conf.burst_rise / 2));
		s->burst_win   = _burstwin(
			s->pixel_rate,
			s->conf.burst_width,
			s->conf.burst_rise,
			1.0,
			&s->burst_width
		);
		if(!s->burst_win)
		{
			vid_free(s);
			return(VID_OUT_OF_MEMORY);
		}
	}
	
	/* Set the next line/frame counter */
	/* NOTE: TV line and frame numbers start at 1 rather than 0 */
	s->bline  = 1;
	s->bframe = 1;
	
	s->vframe = (av_frame_t) {
		.width = s->active_width,
		.height = s->conf.active_lines,
		.framebuffer = NULL,
		.pixel_stride = 0,
		.line_stride = 0,
		.pixel_aspect_ratio = { 1, 1 },
		.interlaced = 0,
	};
	s->olines = 1;
	s->audio = 0;
	
	if(s->conf.raw_bb_file != NULL)
	{
		s->raw_bb_file = fopen(s->conf.raw_bb_file, "rb");
		if(!s->raw_bb_file)
		{
			perror("fopen");
			vid_free(s);
			return(r);
		}
		
		_add_lineprocess(s, "rawbb", 1, NULL, _vid_next_line_rawbb, NULL);
	}
	else if(s->conf.type == VID_MAC)
	{
		/* Initialise D/D2-MAC state */
		r = mac_init(s);
		
		if(r != VID_OK)
		{
			return(r);
		}
		
		_add_lineprocess(s, "macraster", 3, NULL, mac_next_line, NULL);
	}
	else
	{
		_add_lineprocess(s, "raster", 3, NULL, _vid_next_line_raster, NULL);
	}
	
	/* Initialise VITS inserter */
	if(s->conf.vits)
	{
		r = vits_init(
			&s->vits, s->pixel_rate, s->width, s->conf.lines,
			s->conf.colour_mode == VID_PAL,
			s->white_level - s->blanking_level
		);
		
		if(r != VID_OK)
		{
			vid_free(s);
			return(r);
		}
		
		_add_lineprocess(s, "vits", 1, &s->vits, vits_render, NULL);
	}
	
	/* Initialise the WSS system */
	if(s->conf.wss)
	{
		if((r = wss_init(&s->wss, s, s->conf.wss)) != VID_OK)
		{
			vid_free(s);
			return(r);
		}
		
		_add_lineprocess(s, "wss", 1, &s->wss, wss_render, NULL);
	}
	
	/* Initialise videocrypt I/II encoder */
	if(s->conf.videocrypt || s->conf.videocrypt2)
	{
		if((r = vc_init(&s->vc, s, s->conf.videocrypt, s->conf.videocrypt2)) != VID_OK)
		{
			vid_free(s);
			return(r);
		}
		
		_add_lineprocess(s, "videocrypt", 2, &s->vc, vc_render_line, NULL);
	}
	
	/* Initialise videocrypt S encoder */
	if(s->conf.videocrypts)
	{
		if((r = vcs_init(&s->vcs, s, s->conf.videocrypts)) != VID_OK)
		{
			vid_free(s);
			return(r);
		}
		
		_add_lineprocess(s, "videocrypts", VCS_DELAY_LINES, &s->vcs, vcs_render_line, NULL);
	}
	
	/* Initialise syster encoder */
	if(s->conf.syster)
	{
		if((r = ng_init(&s->ng, s)) != VID_OK)
		{
			vid_free(s);
			return(r);
		}
		
		_add_lineprocess(s, "syster", NG_DELAY_LINES, &s->ng, ng_render_line, NULL);
	}
	
	/* Initialise ACP renderer */
	if(s->conf.acp)
	{
		if((r = acp_init(&s->acp, s)) != VID_OK)
		{
			vid_free(s);
			return(r);
		}
		
		_add_lineprocess(s, "acp", 1, &s->acp, acp_render_line, NULL);
	}
	
	/* Initialise VITC timestamp */
	if(s->conf.vitc)
	{
		if((r = vitc_init(&s->vitc, s)) != VID_OK)
		{
			vid_free(s);
			return(r);
		}
		
		_add_lineprocess(s, "vitc", 1, &s->vitc, vitc_render, NULL);
	}
	
	/* Initialise SiS encoder */
	if(s->conf.sis)
	{
		if((r = sis_init(&s->sis, s->conf.sis, s, NICAM_MODE_STEREO, 0)) != VID_OK)
		{
			vid_free(s);
			return(r);
		}
		
		_add_lineprocess(s, "sis", 1, &s->sis, sis_render, NULL);
		
		s->audio = 1;
	}
	
	/* Initialise the teletext system */
	if(s->conf.teletext)
	{
		if((r = tt_init(&s->tt, s, s->conf.teletext)) != VID_OK)
		{
			vid_free(s);
			return(r);
		}
		
		/* Start the teletext renderer thread for non-MAC modes */
		if(s->conf.type != VID_MAC)
		{
			_add_lineprocess(s, "teletext", 1, &s->tt, tt_render_line, NULL);
		}
	}
	
	if(s->pixel_rate != s->sample_rate)
	{
		_init_vresampler(s);
	}
	
	if(s->conf.vfilter)
	{
		_init_vfilter(s);
	}
	
	if(s->conf.a2stereo)
	{
		/* Enable Zweikanalton / A2 Stereo */
		s->a2stereo_system_m = s->conf.fm_mono_carrier == 4500000;
		s->conf.fm_right_level = s->conf.fm_mono_level * 0.446684; /* -7 dB */
		s->conf.fm_right_carrier = s->conf.fm_mono_carrier + (s->a2stereo_system_m ? 224213 : 242000);
		s->conf.fm_right_deviation = s->conf.fm_mono_deviation;
		s->conf.fm_right_preemph = s->conf.fm_mono_preemph;
		
		r = _init_am_modulator(&s->a2stereo_pilot, s->sample_rate, (s->a2stereo_system_m ? 55.06993e3 : 54.6875e3), 0.05);
		if(r != VID_OK)
		{
			vid_free(s);
			return(r);
		}
		
		/* 117.5 Hz == Stereo (149.9 Hz for M variant) */
		r = _init_am_modulator(&s->a2stereo_signal, s->sample_rate, (s->a2stereo_system_m ? 149.9 : 117.5), 1.0);
		if(r != VID_OK)
		{
			vid_free(s);
		}
		
		/* Disable NICAM */
		s->conf.nicam_level = 0;
		s->conf.nicam_carrier = 0;
	}
	
	/* FM audio */
	if(s->conf.fm_mono_level > 0 && s->conf.fm_mono_carrier != 0)
	{
		r = _init_fm_modulator(&s->fm_mono, s->sample_rate, s->conf.fm_mono_carrier, s->conf.fm_mono_deviation, s->conf.fm_mono_level * slevel);
		if(r != VID_OK)
		{
			vid_free(s);
			return(r);
		}
		
		if(s->conf.fm_mono_preemph)
		{
			const double *taps = NULL;
			int ntaps = 0;
			
			switch(s->conf.fm_mono_preemph)
			{
			case VID_50US:
				taps = fm_audio_50us_taps;
				ntaps = sizeof(fm_audio_50us_taps) / sizeof(double);
				break;
			case VID_75US:
				taps = fm_audio_75us_taps;
				ntaps = sizeof(fm_audio_75us_taps) / sizeof(double);
				break;
			case VID_J17:
				taps = fm_audio_j17_taps;
				ntaps = sizeof(fm_audio_j17_taps) / sizeof(double);
				break;
			}
			
			r = limiter_init(&s->fm_mono.limiter, INT16_MAX, 21, taps, fm_audio_flat_taps, ntaps);
			if(r != 0)
			{
				vid_free(s);
				return(VID_OUT_OF_MEMORY);
			}
		}
		
		s->audio = 1;
	}
	
	if(s->conf.fm_left_level > 0 && s->conf.fm_left_carrier != 0)
	{
		r = _init_fm_modulator(&s->fm_left, s->sample_rate, s->conf.fm_left_carrier, s->conf.fm_left_deviation, s->conf.fm_left_level * slevel);
		if(r != VID_OK)
		{
			vid_free(s);
			return(r);
		}
		
		if(s->conf.fm_left_preemph)
		{
			const double *taps = NULL;
			int ntaps = 0;
			
			switch(s->conf.fm_left_preemph)
			{
			case VID_50US:
				taps = fm_audio_50us_taps;
				ntaps = sizeof(fm_audio_50us_taps) / sizeof(double);
				break;
			case VID_75US:
				taps = fm_audio_75us_taps;
				ntaps = sizeof(fm_audio_75us_taps) / sizeof(double);
				break;
			case VID_J17:
				taps = fm_audio_j17_taps;
				ntaps = sizeof(fm_audio_j17_taps) / sizeof(double);
				break;
			}
			
			r = limiter_init(&s->fm_left.limiter, INT16_MAX, 21, taps, fm_audio_flat_taps, ntaps);
			if(r != 0)
			{
				vid_free(s);
				return(VID_OUT_OF_MEMORY);
			}
		}
		
		s->audio = 1;
	}
	
	if(s->conf.fm_right_level > 0 && s->conf.fm_right_carrier != 0)
	{
		r = _init_fm_modulator(&s->fm_right, s->sample_rate, s->conf.fm_right_carrier, s->conf.fm_right_deviation, s->conf.fm_right_level * slevel);
		if(r != VID_OK)
		{
			vid_free(s);
			return(r);
		}
		
		if(s->conf.fm_right_preemph)
		{
			const double *taps = NULL;
			int ntaps = 0;
			
			switch(s->conf.fm_right_preemph)
			{
			case VID_50US:
				taps = fm_audio_50us_taps;
				ntaps = sizeof(fm_audio_50us_taps) / sizeof(double);
				break;
			case VID_75US:
				taps = fm_audio_75us_taps;
				ntaps = sizeof(fm_audio_75us_taps) / sizeof(double);
				break;
			case VID_J17:
				taps = fm_audio_j17_taps;
				ntaps = sizeof(fm_audio_j17_taps) / sizeof(double);
				break;
			}
			
			r = limiter_init(&s->fm_right.limiter, INT16_MAX, 21, taps, fm_audio_flat_taps, ntaps);
			if(r != 0)
			{
				vid_free(s);
				return(VID_OUT_OF_MEMORY);
			}
		}
		
		s->audio = 1;
	}
	
	/* NICAM audio */
	if(s->conf.nicam_level > 0 && s->conf.nicam_carrier != 0)
	{
		r = nicam_mod_init(&s->nicam, NICAM_MODE_STEREO, 1, s->sample_rate, s->conf.nicam_carrier, s->conf.nicam_beta, s->conf.nicam_level * slevel);
		
		if(r != 0)
		{
			vid_free(s);
			return(VID_OUT_OF_MEMORY);
		}
		
		s->nicam_buf_len = 0;
		s->audio = 1;
	}
	
	/* DANCE audio */
	if(s->conf.dance_level > 0 && s->conf.dance_carrier != 0)
	{
		r = dance_mod_init(&s->dance, DANCE_MODE_A, s->sample_rate, s->conf.dance_carrier, s->conf.dance_beta, s->conf.dance_level * slevel);
		
		if(r != 0)
		{
			vid_free(s);
			return(VID_OUT_OF_MEMORY);
		}
		
		s->dance_buf_len = 0;
		s->audio = 1;
	}
	
	/* AM audio */
	if(s->conf.am_audio_level > 0 && s->conf.am_mono_carrier != 0)
	{
		r = _init_am_modulator(&s->am_mono, s->sample_rate, s->conf.am_mono_carrier, s->conf.am_audio_level * slevel);
		if(r != VID_OK)
		{
			vid_free(s);
			return(r);
		}
		
		s->audio = 1;
	}
	
	/* Add the audio process */
	if(s->audio == 1)
	{
		_add_lineprocess(s, "audio", 1, NULL, _vid_audio_process, NULL);
	}
	
	/* FM video */
	if(s->conf.modulation == VID_FM)
	{
		r = _init_fm_modulator(&s->fm_video, s->sample_rate, 0, s->conf.fm_deviation, s->conf.fm_level * s->conf.level);
		if(r != VID_OK)
		{
			vid_free(s);
			return(r);
		}
		
		if(s->conf.fm_energy_dispersal)
		{
			/* Apply a frame locked triangular wave to the FM baseband signal */
			_init_fm_energy_dispersal(
				&s->fm_video,
				s->sample_rate,
				(rational_t) { s->sample_rate, s->width * s->conf.lines }, // s->conf.frame_rate
				round(INT16_MAX * s->conf.fm_energy_dispersal)
			);
		}
		
		_add_lineprocess(s, "fmmod", 1, NULL, _vid_fmmod_process, NULL);
	}
	
	if(s->conf.swap_iq != 0)
	{
		_add_lineprocess(s, "swap_iq", 1, NULL, _vid_swap_iq_process, NULL);
	}
	
	if(s->conf.offset != 0)
	{
		double d;
		
		s->offset.counter = INT16_MAX;
		s->offset.phase.i = INT16_MAX;
		s->offset.phase.q = 0;
		
		d = 2.0 * M_PI / s->sample_rate * s->conf.offset;
		s->offset.delta.i = lround(cos(d) * INT32_MAX);
		s->offset.delta.q = lround(sin(d) * INT32_MAX);
		
		_add_lineprocess(s, "offset", 1, NULL, _vid_offset_process, NULL);
	}
	
	if(s->conf.passthru)
	{
		/* Open the passthru source */
		if(strcmp(s->conf.passthru, "-") == 0)
		{
			s->passthru = stdin;
		}
		else
		{
			s->passthru = fopen(s->conf.passthru, "rb");
		}
		
		if(!s->passthru)
		{
			perror(s->conf.passthru);
			vid_free(s);
			return(VID_ERROR);
		}
		
		/* Allocate memory for the temporary passthru buffer */
		s->passline = calloc(sizeof(int16_t) * 2, s->max_width);
		if(!s->passline)
		{
			vid_free(s);
			return(VID_OUT_OF_MEMORY);
		}
		
		_add_lineprocess(s, "passthru", 1, NULL, _vid_passthru_process, NULL);
	}
	
	/* The final process is only for output */
	_add_lineprocess(s, "output", 1, NULL, NULL, NULL);
	s->output_process = &s->processes[s->nprocesses - 1];
	
	/* Output line buffer(s) */
	s->oline = calloc(sizeof(vid_line_t), s->olines);
	if(!s->oline)
	{
		vid_free(s);
		return(VID_OUT_OF_MEMORY);
	}
	
	for(r = 0; r < s->olines; r++)
	{
		s->oline[r].output = malloc(sizeof(int16_t) * 2 * s->max_width);
		if(!s->oline[r].output)
		{
			vid_free(s);
			return(VID_OUT_OF_MEMORY);
		}
		
		/* Blank the lines */
		for(x = 0; x < s->width; x++)
		{
			s->oline[r].output[x * 2] = s->blanking_level;
		}
		
		s->oline[r].width = 0;
		s->oline[r].frame = 1;
		s->oline[r].line = 0;
		s->oline[r].vbialloc = 0;
		s->oline[r].previous = &s->oline[(r + s->olines - 1) % s->olines];
		s->oline[r].next = &s->oline[(r + 1) % s->olines];
	}
	
	/* Setup lineprocess output windows (non-threaded version) */
	l = &s->oline[s->olines - 1];
	
	for(r = 0; r < s->nprocesses; r++)
	{
		_lineprocess_t *p = &s->processes[r];
		
		l -= p->nlines - 1;
		
		for(x = 0; x < p->nlines; x++)
		{
			p->lines[x] = &l[x];
		}
	}
	
	return(VID_OK);
}

void vid_free(vid_t *s)
{
	int i;
	
	/* Close the AV source */
	av_close(&s->av);
	
	for(i = 0; i < s->nprocesses; i++)
	{
		if(s->processes[i].free)
		{
			s->processes[i].free(s, s->processes[i].arg);
		}
		
		free(s->processes[i].lines);
	}
	free(s->processes);
	
	if(s->conf.passthru)
	{
		fclose(s->passthru);
		free(s->passline);
	}
	
	if(s->conf.teletext)
	{
		tt_free(&s->tt);
	}
	
	if(s->conf.sis)
	{
		sis_free(&s->sis);
	}
	
	if(s->conf.vitc)
	{
		vitc_free(&s->vitc);
	}
	
	if(s->conf.vits)
	{
		vits_free(&s->vits);
	}
	
	if(s->conf.acp)
	{
		acp_free(&s->acp);
	}
	
	if(s->conf.syster)
	{
		ng_free(&s->ng);
	}
	
	if(s->conf.videocrypt || s->conf.videocrypt2)
	{
		vc_free(&s->vc);
	}
	
	if(s->conf.videocrypts)
	{
		vcs_free(&s->vcs);
	}
	
	if(s->conf.wss)
	{
		wss_free(&s->wss);
	}
	
	if(s->conf.type == VID_MAC)
	{
		mac_free(s);
	}
	
	/* Free allocated memory */
	free(s->yiq_level_lookup);
	free(s->colour_lookup);
	fir_int16_free(&s->secam_l_fir);
	fir_int16_free(&s->fm_secam_fir);
	iir_int16_free(&s->fm_secam_iir);
	_free_fm_modulator(&s->fm_secam);
	_free_fm_modulator(&s->fm_video);
	_free_fm_modulator(&s->fm_mono);
	_free_fm_modulator(&s->fm_left);
	_free_fm_modulator(&s->fm_right);
	_free_am_modulator(&s->a2stereo_pilot);
	_free_am_modulator(&s->a2stereo_signal);
	limiter_free(&s->fm_mono.limiter);
	limiter_free(&s->fm_left.limiter);
	limiter_free(&s->fm_right.limiter);
	dance_mod_free(&s->dance);
	nicam_mod_free(&s->nicam);
	_free_am_modulator(&s->am_mono);
	
	if(s->oline)
	{
		for(i = 0; i < s->olines; i++)
		{
			free(s->oline[i].output);
		}
		free(s->oline);
	}
	
	free(s->burst_win);
	free(s->syncs);
	free(s->fsc_syncs);
	
	memset(s, 0, sizeof(vid_t));
}

void vid_info(vid_t *s)
{
	fprintf(stderr, "Video: %dx%d %.2f fps (full frame %dx%d)\n",
		s->active_width, s->conf.active_lines,
		(double) s->conf.frame_rate.num / s->conf.frame_rate.den,
		s->width, s->conf.lines
	);
	
	if(s->sample_rate != s->pixel_rate)
	{
		fprintf(stderr, "Pixel rate: %d\n", s->pixel_rate);
	}
	
	fprintf(stderr, "Sample rate: %d\n", s->sample_rate);
}

size_t vid_get_framebuffer_length(vid_t *s)
{
	return(sizeof(uint32_t) * s->active_width * s->conf.active_lines);
}

static vid_line_t *_vid_next_line(vid_t *s, size_t *samples)
{
	vid_line_t *l = s->output_process->lines[0];
	int i, j;
	
	/* Load the next frame */
	if(s->bline == 1 || (s->conf.interlace && s->bline == s->conf.hline))
	{
		/* Have we reached the end of the video? */
		if(av_eof(&s->av))
		{
			return(NULL);
		}
		
		av_read_video(&s->av, &s->vframe);
		
		av_rotate_frame(&s->vframe, s->conf.frame_orientation & 3);
		if(s->conf.frame_orientation & VID_HFLIP) av_hflip_frame(&s->vframe);
		if(s->conf.frame_orientation & VID_VFLIP) av_vflip_frame(&s->vframe);
		
		/* Crop frame to fit inside active video area */
		av_crop_frame(&s->vframe,
			(s->vframe.width - s->active_width) / 2,
			(s->vframe.height - s->conf.active_lines) / 2,
			s->active_width,
			s->conf.active_lines
		);
		
		/* Calculate frame offset from top left */
		s->vframe_x = (s->active_width - s->vframe.width) / 2;
		s->vframe_y = (s->conf.active_lines - s->vframe.height) / 2;
	}
	
	for(i = 0; i < s->nprocesses; i++)
	{
		_lineprocess_t *p = &s->processes[i];
		
		if(p->process)
		{
			p->process(p->vid, p->arg, p->nlines, p->lines);
		}
		
		for(j = 0; j < p->nlines; j++)
		{
			p->lines[j] = p->lines[j]->next;
		}
	}
	
	/* Advance the next line/frame counter */
	if(s->bline++ == s->conf.lines)
	{
		s->bline = 1;
		s->bframe++;
	}
	
	/* Return a pointer to the output buffer */
	if(samples)
	{
		*samples = l->width;
	}
	
	return(l);
}

int16_t *vid_next_line(vid_t *s, size_t *samples)
{
	vid_line_t *l;
	
	/* Drop any delay lines introduced by scramblers / filters */
	do
	{
		l = _vid_next_line(s, samples);
		if(l == NULL) return(NULL);
	}
	while(l->line < 1);
	
	s->frame = l->frame;
	s->line  = l->line;
	
	return(l->output);
}

