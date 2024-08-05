const vid_config_t vid_config_pal_i = {

	/* System I (PAL) */
	.output_type    = RF_INT16_COMPLEX,

	.modulation     = VID_VSB,
	.vsb_upper_bw   = 5500000, /* Hz */
	.vsb_lower_bw   = 1250000, /* Hz */

	.level          = 1.0, /* Overall signal level */

	.video_level    = 0.71, /* Power level of video */
	.fm_mono_level  = 0.22, /* FM audio carrier power level */
	.nicam_level    = 0.07 / 2, /* NICAM audio carrier power level */

	.type           = VID_RASTER_625,
	.frame_rate     = { 25, 1 },
	.frame_aspects  = { { 4, 3 } },
	.lines          = 625,
	.hline          = 313,

	.active_lines   = 576,
	.active_width   = 0.00005195, /* 51.95µs */
	.active_left    = 0.00001040, /* |-->| 10.40µs */

	.hsync_width       = 0.00000470, /*  4.70 ±0.20µs */
	.vsync_short_width = 0.00000235, /*  2.35 ±0.10µs */
	.vsync_long_width  = 0.00002730, /* 27.30 ±0.10µs */
	.sync_rise         = 0.00000025, /*  0.25 +0.05µs */

	.white_level    = 0.20,
	.black_level    = 0.76,
	.blanking_level = 0.76,
	.sync_level     = 1.00,

	.colour_mode    = VID_PAL,
	.burst_width    = 0.00000225, /* 2.25 ±0.23µs */
	.burst_rise     = 0.00000030, /* 0.30 ±0.10µs */
	.burst_left     = 0.00000560, /* |-->| 5.6 ±0.1µs */
	.burst_level    = 3.0 / 7.0, /* 3 / 7 of white - blanking level */
	.colour_carrier = { 17734475, 4 }, /* 4433618.75 Hz */

	.rw_co          = 0.299, /* R weight */
	.gw_co          = 0.587, /* G weight */
	.bw_co          = 0.114, /* B weight */
	.ev_co          = 0.877,
	.eu_co          = 0.493,

	.fm_mono_carrier   = 6000000 - 400, /* Hz */
	.fm_mono_deviation = 50000, /* +/- Hz */
	.fm_mono_preemph   = VID_50US,

	.nicam_carrier  = 6552000, /* Hz */
	.nicam_beta     = 1.0,
};

const vid_config_t vid_config_pal_bg = {

	/* System B/G (PAL) */
	.output_type    = RF_INT16_COMPLEX,

	.modulation     = VID_VSB,
	.vsb_upper_bw   = 5000000, /* Hz */
	.vsb_lower_bw   =  750000, /* Hz */

	.level          = 1.0, /* Overall signal level */

	.video_level    = 0.71, /* Power level of video */
	.fm_mono_level  = 0.15, /* FM audio carrier power level */
	.nicam_level    = 0.07 / 2, /* NICAM audio carrier power level */

	.type           = VID_RASTER_625,
	.frame_rate     = { 25, 1 },
	.frame_aspects  = { { 4, 3 } },
	.lines          = 625,
	.hline          = 313,

	.active_lines   = 576,
	.active_width   = 0.00005195, /* 51.95µs */
	.active_left    = 0.00001040, /* |-->| 10.40µs */

	.hsync_width       = 0.00000470, /*  4.70 ±0.20µs */
	.vsync_short_width = 0.00000235, /*  2.35 ±0.10µs */
	.vsync_long_width  = 0.00002730, /* 27.30 ±0.10µs */
	.sync_rise         = 0.00000020, /*  0.20 +0.10µs */

	.white_level    = 0.20,
	.black_level    = 0.76,
	.blanking_level = 0.76,
	.sync_level     = 1.00,

	.colour_mode    = VID_PAL,
	.burst_width    = 0.00000225, /* 2.25 ±0.23µs */
	.burst_rise     = 0.00000030, /* 0.30 ±0.10µs */
	.burst_left     = 0.00000560, /* |-->| 5.6 ±0.1µs */
	.burst_level    = 3.0 / 7.0, /* 3 / 7 of white - blanking level */
	.colour_carrier = { 17734475, 4 }, /* 4433618.75 Hz */

	.rw_co          = 0.299, /* R weight */
	.gw_co          = 0.587, /* G weight */
	.bw_co          = 0.114, /* B weight */
	.ev_co          = 0.877,
	.eu_co          = 0.493,

	.fm_mono_carrier   = 5500000, /* Hz */
	.fm_mono_deviation = 50000, /* +/- Hz */
	.fm_mono_preemph   = VID_50US, /* Seconds */

	.nicam_carrier  = 5850000, /* Hz */
	.nicam_beta     = 0.4,
};

const vid_config_t vid_config_pal_dk = {

	/* System D/K (PAL) */
	.output_type    = RF_INT16_COMPLEX,

	.modulation     = VID_VSB,
	.vsb_upper_bw   = 5500000, /* Hz */
	.vsb_lower_bw   =  750000, /* Hz */

	.level          = 1.0, /* Overall signal level */

	.video_level    = 0.70, /* Power level of video */
	.fm_mono_level  = 0.20, /* FM audio carrier power level */
	.nicam_level    = 0.07 / 2, /* NICAM audio carrier power level */

	.type           = VID_RASTER_625,
	.frame_rate     = { 25, 1 },
	.frame_aspects  = { { 4, 3 } },
	.lines          = 625,
	.hline          = 313,

	.active_lines   = 576,
	.active_width   = 0.00005195, /* 51.95µs */
	.active_left    = 0.00001040, /* |-->| 10.40µs */

	.hsync_width       = 0.00000470, /*  4.70 ±0.20µs */
	.vsync_short_width = 0.00000235, /*  2.35 ±0.10µs */
	.vsync_long_width  = 0.00002730, /* 27.30 ±0.10µs */
	.sync_rise         = 0.00000020, /*  0.20 +0.10µs */

	.white_level    = 0.20,
	.black_level    = 0.76,
	.blanking_level = 0.76,
	.sync_level     = 1.00,

	.colour_mode    = VID_PAL,
	.burst_width    = 0.00000225, /* 2.25 ±0.23µs */
	.burst_rise     = 0.00000030, /* 0.30 ±0.10µs */
	.burst_left     = 0.00000560, /* |-->| 5.6 ±0.1µs */
	.burst_level    = 3.0 / 7.0, /* 3 / 7 of white - blanking level */
	.colour_carrier = { 17734475, 4 }, /* 4433618.75 Hz */

	.rw_co          = 0.299, /* R weight */
	.gw_co          = 0.587, /* G weight */
	.bw_co          = 0.114, /* B weight */
	.ev_co          = 0.877,
	.eu_co          = 0.493,

	.fm_mono_carrier   = 6500000, /* Hz */
	.fm_mono_deviation = 50000, /* +/- Hz */
	.fm_mono_preemph   = VID_50US,

	/* Chinese standard GY/T 129-1997, similar to French standard. */
	.nicam_carrier  = 5850000, /* Hz */
	.nicam_beta     = 0.4,
};

const vid_config_t vid_config_pal_fm = {

	/* PAL FM (satellite) */
	.output_type    = RF_INT16_COMPLEX,

	.modulation     = VID_FM,
	.fm_level       = 1.0,
	.fm_deviation   = 16e6, /* 16 MHz/V */
	//.fm_energy_dispersal = 0.0625, /* 1 MHz deviation (2 MHz p-p) */

	.level          = 1.0, /* Overall signal level */

	.video_level    = 1.00, /* Power level of video */
	.fm_mono_level  = 0.06, /* FM audio carrier power level */
	//.fm_left_level  = 0.04, /* FM stereo left audio carrier power level */
	//.fm_right_level = 0.04, /* FM stereo right audio carrier power level */

	.type           = VID_RASTER_625,
	.frame_rate     = { 25, 1 },
	.frame_aspects  = { { 4, 3 } },
	.lines          = 625,
	.hline          = 313,

	.active_lines   = 576,
	.active_width   = 0.00005195, /* 51.95µs */
	.active_left    = 0.00001040, /* |-->| 10.40µs */

	.hsync_width       = 0.00000470, /*  4.70 ±0.20µs */
	.vsync_short_width = 0.00000235, /*  2.35 ±0.10µs */
	.vsync_long_width  = 0.00002730, /* 27.30 ±0.10µs */
	.sync_rise         = 0.00000020, /*  0.20 +0.10µs */

	.white_level    =  0.50,
	.black_level    = -0.20,
	.blanking_level = -0.20,
	.sync_level     = -0.50,

	.colour_mode    = VID_PAL,
	.burst_width    = 0.00000225, /* 2.25 ±0.23µs */
	.burst_rise     = 0.00000030, /* 0.30 ±0.10µs */
	.burst_left     = 0.00000560, /* |-->| 5.6 ±0.1µs */
	.burst_level    = 3.0 / 7.0, /* 3 / 7 of white - blanking level */
	.colour_carrier = { 17734475, 4 }, /* 4433618.75 Hz */

	.rw_co          = 0.299, /* R weight */
	.gw_co          = 0.587, /* G weight */
	.bw_co          = 0.114, /* B weight */
	.ev_co          = 0.877,
	.eu_co          = 0.493,

	.fm_mono_carrier   = 6500000, /* Hz */
	.fm_mono_deviation = 85000, /* +/- Hz */
	.fm_mono_preemph   = VID_50US, /* Seconds */

	//.fm_left_carrier   = 7020000, /* Hz */
	//.fm_left_deviation = 50000, /* +/- Hz */
	//.fm_left_preemph   = VID_75US, /* Seconds */

	//.fm_right_carrier   = 7200000, /* Hz */
	//.fm_right_deviation = 50000, /* +/- Hz */
	//.fm_right_preemph   = VID_75US, /* Seconds */
};

const vid_config_t vid_config_pal = {

	/* Composite PAL */
	.output_type    = RF_INT16_REAL,

	.level          = 1.0, /* Overall signal level */
	.video_level    = 1.0, /* Power level of video */

	.video_bw       = 6.0e6,

	.type           = VID_RASTER_625,
	.frame_rate     = { 25, 1 },
	.frame_aspects  = { { 4, 3 } },
	.lines          = 625,
	.hline          = 313,

	.active_lines   = 576,
	.active_width   = 0.00005195, /* 51.95µs */
	.active_left    = 0.00001040, /* |-->| 10.40µs */

	.hsync_width       = 0.00000470, /*  4.70 ±0.20µs */
	.vsync_short_width = 0.00000235, /*  2.35 ±0.10µs */
	.vsync_long_width  = 0.00002730, /* 27.30 ±0.10µs */
	.sync_rise         = 0.00000020, /*  0.20 +0.10µs */

	.white_level    =  0.70,
	.black_level    =  0.00,
	.blanking_level =  0.00,
	.sync_level     = -0.30,

	.colour_mode    = VID_PAL,
	.burst_width    = 0.00000225, /* 2.25 ±0.23µs */
	.burst_rise     = 0.00000030, /* 0.30 ±0.10µs */
	.burst_left     = 0.00000560, /* |-->| 5.6 ±0.1µs */
	.burst_level    = 3.0 / 7.0, /* 3 / 7 of white - blanking level */
	.colour_carrier = { 17734475, 4 }, /* 4433618.75 Hz */

	.rw_co          = 0.299, /* R weight */
	.gw_co          = 0.587, /* G weight */
	.bw_co          = 0.114, /* B weight */
	.ev_co          = 0.877,
	.eu_co          = 0.493,
};

const vid_config_t vid_config_pal_m = {

	/* System M (525 PAL) */
	.output_type    = RF_INT16_COMPLEX,

	.modulation     = VID_VSB,
	.vsb_upper_bw   = 4200000, /* Hz */
	.vsb_lower_bw   =  750000, /* Hz */

	.level          = 1.0, /* Overall signal level */

	.video_level    = 0.77, /* Power level of video */
	.fm_mono_level  = 0.15, /* FM audio carrier power level */

	.type           = VID_RASTER_525,
	.frame_rate     = { 30000, 1001, },
	.frame_aspects  = { { 4, 3 } },
	.lines          = 525,
	.hline          = 263,

	.active_lines   = 480,
	.active_width   = 0.00005280, /* 52.80µs */
	.active_left    = 0.00000920, /* |-->| 9.2 +0.2 -0.1µs */

	.hsync_width       = 0.00000470, /*  4.70 ±0.10µs */
	.vsync_short_width = 0.00000230, /*  2.30 ±0.10μs */
	.vsync_long_width  = 0.00002710, /* 27.10μs */
	.sync_rise         = 0.00000020, /*  0.25µs */

	.white_level    = 0.2000,
	.black_level    = 0.7280,
	.blanking_level = 0.7712,
	.sync_level     = 1.0000,

	.colour_mode    = VID_PAL,
	.burst_width    = 0.00000252, /* 2.52 ±0.28 μs */
	.burst_rise     = 0.00000030, /* 0.30 ±0.10µs */
	.burst_left     = 0.00000530, /* |-->| 5.3 ±0.1μs */
	.burst_level    = 33.0 / 73.0, /* Approximation */
	.colour_carrier = { 511312500, 143 }, /* 3575611.888... Hz */

	.rw_co          = 0.299, /* R weight */
	.gw_co          = 0.587, /* G weight */
	.bw_co          = 0.114, /* B weight */
	.ev_co          = 0.877,
	.eu_co          = 0.493,

	.fm_mono_carrier   = 4500000, /* Hz */
	.fm_mono_deviation = 25000, /* +/- Hz */
	.fm_mono_preemph   = VID_75US, /* Seconds */
};

const vid_config_t vid_config_pal_n = {

	/* System N (625 PAL) */
	.output_type    = RF_INT16_COMPLEX,

	.modulation     = VID_VSB,
	.vsb_upper_bw   = 4200000, /* Hz */
	.vsb_lower_bw   =  750000, /* Hz */

	.level          = 1.0, /* Overall signal level */

	.video_level    = 0.77, /* Power level of video */
	.fm_mono_level  = 0.15, /* FM audio carrier power level */

	.type           = VID_RASTER_625,
	.frame_rate     = { 25, 1 },
	.frame_aspects  = { { 4, 3 } },
	.lines          = 625,
	.hline          = 313,

	.active_lines   = 576,
	.active_width   = 0.00005195, /* 51.95µs */
	.active_left    = 0.00001040, /* |-->| 10.40µs */

	.hsync_width       = 0.00000470, /* 4.70 ±0.20µs */
	.vsync_short_width = 0.00000235, /* 2.35 ±0.10µs */
	.vsync_long_width  = 0.00002730, /* 27.3 ±0.10µs */

	.white_level    = 0.2000,
	.black_level    = 0.7280,
	.blanking_level = 0.7712,
	.sync_level     = 1.0000,

	.colour_mode    = VID_PAL,
	.burst_width    = 0.00000252, /* 2.52 ±0.28 μs */
	.burst_rise     = 0.00000030, /* 0.30 ±0.10µs */
	.burst_left     = 0.00000530, /* |-->| 5.3 ±0.1μs */
	.burst_level    = 33.0 / 73.0, /* Approximation */
	.colour_carrier = { 14328225, 4 }, /* 3582056.25 Hz */

	.rw_co          = 0.299, /* R weight */
	.gw_co          = 0.587, /* G weight */
	.bw_co          = 0.114, /* B weight */
	.ev_co          = 0.877,
	.eu_co          = 0.493,

	.fm_mono_carrier   = 4500000, /* Hz */
	.fm_mono_deviation = 25000, /* +/- Hz */
	.fm_mono_preemph   = VID_75US,
};

const vid_config_t vid_config_525pal = {

	/* Composite 525PAL */
	.output_type    = RF_INT16_REAL,

	.level          = 1.0, /* Overall signal level */
	.video_level    = 1.0, /* Power level of video */

	.video_bw       = 6.0e6,

	.type           = VID_RASTER_525,
	.frame_rate     = { 30000, 1001 },
	.frame_aspects  = { { 4, 3 } },
	.lines          = 525,
	.hline          = 263,

	.active_lines   = 480,
	.active_width   = 0.00005280, /* 52.80µs */
	.active_left    = 0.00000920, /* |-->| 9.2 +0.2 -0.1µs */

	.hsync_width       = 0.00000470, /*  4.70 ±0.10µs */
	.vsync_short_width = 0.00000230, /*  2.30 ±0.10μs */
	.vsync_long_width  = 0.00002710, /* 27.10μs */
	.sync_rise         = 0.00000020, /*  0.25µs */

	.white_level    =  0.70,
	.black_level    =  0.00,
	.blanking_level =  0.00,
	.sync_level     = -0.30,

	.colour_mode    = VID_PAL,
	.burst_width    = 0.00000252, /* 2.52 ±0.28 μs */
	.burst_rise     = 0.00000030, /* 0.30 ±0.10µs */
	.burst_left     = 0.00000530, /* |-->| 5.3 ±0.1μs */
	.burst_level    = 33.0 / 73.0, /* Approximation */
	.colour_carrier = { 511312500, 143 }, /* 3575611.888... Hz */

	.rw_co          = 0.299, /* R weight */
	.gw_co          = 0.587, /* G weight */
	.bw_co          = 0.114, /* B weight */
	.ev_co          = 0.877,
	.eu_co          = 0.493,
};

const vid_config_t vid_config_secam_l = {

	/* System L (SECAM) */
	.output_type    = RF_INT16_COMPLEX,

	.modulation     = VID_VSB,
	.vsb_upper_bw   = 6000000, /* Hz */
	.vsb_lower_bw   = 1250000, /* Hz */

	.level          = 1.0, /* Overall signal level */

	.video_level    = 0.80 * (100.0 / 124.0), /* Power level of video (allowing for white + chrominance) */
	.am_audio_level = 0.10, /* AM audio carrier power level */
	.nicam_level    = 0.04, /* NICAM audio carrier power level */

	.type           = VID_RASTER_625,
	.frame_rate     = { 25, 1 },
	.frame_aspects  = { { 4, 3 } },
	.lines          = 625,
	.hline          = 313,

	.active_lines   = 576,
	.active_width   = 0.00005195, /* 51.95µs */
	.active_left    = 0.00001040, /* |-->| 10.40µs */

	.hsync_width       = 0.00000470, /*  4.70 ±0.20µs */
	.vsync_short_width = 0.00000235, /*  2.35 ±0.10µs */
	.vsync_long_width  = 0.00002730, /* 27.30µs */
	.sync_rise         = 0.00000020, /*  0.20 ±0.10µs */

	.white_level    = 1.00,
	.black_level    = 0.30,
	.blanking_level = 0.30,
	.sync_level     = 0.05, /* leave a residual radiated carrier level of 5 ±2% */

	.colour_mode    = VID_SECAM,
	.burst_width    = 0.00005690, /* 56.9μs */
	.burst_rise     = 0.00000100, /* 1.00µs */
	.burst_left     = 0.00000560, /* |-->| 5.6 ±0.1µs */

	.rw_co          =  0.299, /* R weight */
	.gw_co          =  0.587, /* G weight */
	.bw_co          =  0.114, /* B weight */
	.ev_co          = -1.902 * 280e3, /* D'R */
	.eu_co          =  1.505 * 230e3, /* D'B */

	.am_mono_carrier = 6500000, /* Hz */

	.nicam_carrier  = 5850000, /* Hz */
	.nicam_beta     = 0.4,
};

const vid_config_t vid_config_secam_dk = {

	/* System D/K (SECAM) */
	.output_type    = RF_INT16_COMPLEX,

	.modulation     = VID_VSB,
	.vsb_upper_bw   = 5500000, /* Hz */
	.vsb_lower_bw   =  750000, /* Hz */

	.level          = 1.0, /* Overall signal level */

	.video_level    = 0.70, /* Power level of video */
	.fm_mono_level  = 0.20, /* FM audio carrier power level */
	.nicam_level    = 0.07 / 2, /* NICAM audio carrier power level */

	.type           = VID_RASTER_625,
	.frame_rate     = { 25, 1 },
	.frame_aspects  = { { 4, 3 } },
	.lines          = 625,
	.hline          = 313,

	.active_lines   = 576,
	.active_width   = 0.00005195, /* 51.95µs */
	.active_left    = 0.00001040, /* |-->| 10.40µs */

	.hsync_width       = 0.00000470, /*  4.70 ±0.20µs */
	.vsync_short_width = 0.00000235, /*  2.35 ±0.10µs */
	.vsync_long_width  = 0.00002730, /* 27.30 µs */
	.sync_rise         = 0.00000020, /*  0.20 ±0.10µs */

	.white_level    = 0.20,
	.black_level    = 0.76,
	.blanking_level = 0.76,
	.sync_level     = 1.00,

	.colour_mode    = VID_SECAM,
	.burst_width    = 0.00005690, /* 56.9μs */
	.burst_rise     = 0.00000100, /* 1.00µs */
	.burst_left     = 0.00000560, /* |-->| 5.6 ±0.1µs */

	.rw_co          =  0.299, /* R weight */
	.gw_co          =  0.587, /* G weight */
	.bw_co          =  0.114, /* B weight */
	.ev_co          = -1.902 * 280e3, /* D'R */
	.eu_co          =  1.505 * 230e3, /* D'B */

	.fm_mono_carrier   = 6500000, /* Hz */
	.fm_mono_deviation = 50000, /* +/- Hz */
	.fm_mono_preemph   = VID_50US, /* Seconds */

	.nicam_carrier  = 5850000, /* Hz */
	.nicam_beta     = 0.4,
};

const vid_config_t vid_config_secam_i = {

	/* System I (SECAM) */
	.output_type    = RF_INT16_COMPLEX,

	.modulation     = VID_VSB,
	.vsb_upper_bw   = 5500000, /* Hz */
	.vsb_lower_bw   = 1250000, /* Hz */

	.level          = 1.0, /* Overall signal level */

	.video_level    = 0.71, /* Power level of video */
	.fm_mono_level  = 0.15, /* FM audio carrier power level */
	.nicam_level    = 0.07 / 2, /* NICAM audio carrier power level */

	.type           = VID_RASTER_625,
	.frame_rate     = { 25, 1 },
	.frame_aspects  = { { 4, 3 } },
	.lines          = 625,
	.hline          = 313,

	.active_lines   = 576,
	.active_width   = 0.00005195, /* 51.95µs */
	.active_left    = 0.00001040, /* |-->| 10.40µs */

	.hsync_width       = 0.00000470, /*  4.70 ±0.20µs */
	.vsync_short_width = 0.00000235, /*  2.35 ±0.10µs */
	.vsync_long_width  = 0.00002730, /* 27.30 ±0.10µs */
	.sync_rise         = 0.00000025, /*  0.25 +0.05µs */

	.white_level    = 0.20,
	.black_level    = 0.76,
	.blanking_level = 0.76,
	.sync_level     = 1.00,

	.colour_mode    = VID_SECAM,
	.burst_width    = 0.00005690, /* 56.9μs */
	.burst_rise     = 0.00000100, /* 1.00µs */
	.burst_left     = 0.00000560, /* |-->| 5.6 ±0.1µs */

	.rw_co          =  0.299, /* R weight */
	.gw_co          =  0.587, /* G weight */
	.bw_co          =  0.114, /* B weight */
	.ev_co          = -1.902 * 280e3, /* D'R */
	.eu_co          =  1.505 * 230e3, /* D'B */

	.fm_mono_carrier   = 6000000 - 400, /* Hz */
	.fm_mono_deviation = 50000, /* +/- Hz */
	.fm_mono_preemph   = VID_50US,

	.nicam_carrier  = 6552000, /* Hz */
	.nicam_beta     = 1.0,
};

const vid_config_t vid_config_secam_bg = {

	/* System B/G (SECAM) */
	.output_type    = RF_INT16_COMPLEX,

	.modulation     = VID_VSB,
	.vsb_upper_bw   = 5000000, /* Hz */
	.vsb_lower_bw   =  750000, /* Hz */

	.level          = 1.0, /* Overall signal level */

	.video_level    = 0.80 * (100.0 / 124.0), /* Power level of video */
	.fm_mono_level  = 0.15, /* FM audio carrier power level */

	.type           = VID_RASTER_625,
	.frame_rate     = { 25, 1 },
	.frame_aspects  = { { 4, 3 } },
	.lines          = 625,
	.hline          = 313,

	.active_lines   = 576,
	.active_width   = 0.00005195, /* 51.95µs */
	.active_left    = 0.00001040, /* |-->| 10.40µs */

	.hsync_width       = 0.00000470, /*  4.70 ±0.20µs */
	.vsync_short_width = 0.00000235, /*  2.35 ±0.10µs */
	.vsync_long_width  = 0.00002730, /* 27.30 ±0.10µs */
	.sync_rise         = 0.00000020, /*  0.20 +0.10µs */

	.white_level    = 0.20,
	.black_level    = 0.76,
	.blanking_level = 0.76,
	.sync_level     = 1.00,

	.colour_mode    = VID_SECAM,
	.burst_width    = 0.00005690, /* 56.9μs */
	.burst_rise     = 0.00000100, /* 1.00µs */
	.burst_left     = 0.00000560, /* |-->| 5.6 ±0.1µs */

	.rw_co          =  0.299, /* R weight */
	.gw_co          =  0.587, /* G weight */
	.bw_co          =  0.114, /* B weight */
	.ev_co          = -1.902 * 280e3, /* D'R */
	.eu_co          =  1.505 * 230e3, /* D'B */

	.fm_mono_carrier   = 5500000, /* Hz */
	.fm_mono_deviation = 50000, /* +/- Hz */
	.fm_mono_preemph   = VID_50US, /* Seconds */
};

const vid_config_t vid_config_secam_fm = {

	/* SECAM FM (satellite) */
	.output_type    = RF_INT16_COMPLEX,

	.modulation     = VID_FM,
	.fm_level       = 1.0,
	.fm_deviation   = 16e6, /* 16 MHz/V */

	.level          = 1.0, /* Overall signal level */

	.video_level    = 1.00, /* Power level of video */
	.fm_mono_level  = 0.05, /* FM audio carrier power level */
	//.fm_left_level  = 0.025, /* FM stereo left audio carrier power level */
	//.fm_right_level = 0.025, /* FM stereo right audio carrier power level */

	.type           = VID_RASTER_625,
	.frame_rate     = { 25, 1 },
	.frame_aspects  = { { 4, 3 } },
	.lines          = 625,
	.hline          = 313,

	.active_lines   = 576,
	.active_width   = 0.00005195, /* 51.95µs */
	.active_left    = 0.00001040, /* |-->| 10.40µs */

	.hsync_width       = 0.00000470, /*  4.70 ±0.20µs */
	.vsync_short_width = 0.00000235, /*  2.35 ±0.10µs */
	.vsync_long_width  = 0.00002730, /* 27.30µs */
	.sync_rise         = 0.00000020, /*  0.20 +0.10µs */

	.white_level    =  0.50,
	.black_level    = -0.20,
	.blanking_level = -0.20,
	.sync_level     = -0.50,

	.colour_mode    = VID_SECAM,
	.burst_width    = 0.00005690, /* 56.9μs */
	.burst_rise     = 0.00000100, /* 1.00µs */
	.burst_left     = 0.00000560, /* |-->| 5.6 ±0.1µs */

	.rw_co          =  0.299, /* R weight */
	.gw_co          =  0.587, /* G weight */
	.bw_co          =  0.114, /* B weight */
	.ev_co          = -1.902 * 280e3, /* D'R */
	.eu_co          =  1.505 * 230e3, /* D'B */

	.fm_mono_carrier   = 6500000, /* Hz */
	.fm_mono_deviation = 85000, /* +/- Hz */
	.fm_mono_preemph   = VID_50US, /* Seconds */

	//.fm_left_carrier   = 7020000, /* Hz */
	//.fm_left_deviation = 50000, /* +/- Hz */
	//.fm_left_preemph   = VID_50US, /* Seconds */

	//.fm_right_carrier   = 7200000, /* Hz */
	//.fm_right_deviation = 50000, /* +/- Hz */
	//.fm_right_preemph   = VID_50US, /* Seconds */
};

const vid_config_t vid_config_secam = {

	/* Composite SECAM */
	.output_type    = RF_INT16_REAL,

	.level          = 1.0, /* Overall signal level */
	.video_level    = 1.0, /* Power level of video */

	.video_bw       = 6.0e6,

	.type           = VID_RASTER_625,
	.frame_rate     = { 25, 1 },
	.frame_aspects  = { { 4, 3 } },
	.lines          = 625,
	.hline          = 313,

	.active_lines   = 576,
	.active_width   = 0.00005195, /* 51.95µs */
	.active_left    = 0.00001040, /* |-->| 10.40µs */

	.hsync_width       = 0.00000470, /*  4.70 ±0.20µs */
	.vsync_short_width = 0.00000235, /*  2.35 ±0.10µs */
	.vsync_long_width  = 0.00002730, /* 27.30µs */
	.sync_rise         = 0.00000020, /*  0.20 +0.10µs */

	.white_level    =  0.70,
	.black_level    =  0.00,
	.blanking_level =  0.00,
	.sync_level     = -0.30,

	.colour_mode    = VID_SECAM,
	.burst_width    = 0.00005690, /* 56.9μs */
	.burst_rise     = 0.00000100, /* 1.00µs */
	.burst_left     = 0.00000560, /* |-->| 5.6 ±0.1µs */

	.rw_co          =  0.299, /* R weight */
	.gw_co          =  0.587, /* G weight */
	.bw_co          =  0.114, /* B weight */
	.ev_co          = -1.902 * 280e3, /* D'R */
	.eu_co          =  1.505 * 230e3, /* D'B */
};

const vid_config_t vid_config_ntsc_m = {

	/* System M (NTSC) */
	.output_type    = RF_INT16_COMPLEX,

	.modulation     = VID_VSB,
	.vsb_upper_bw   = 4200000, /* Hz */
	.vsb_lower_bw   =  750000, /* Hz */

	.level          = 1.0, /* Overall signal level */

	.video_level    = 0.77, /* Power level of video */
	.fm_mono_level  = 0.15, /* FM audio carrier power level */

	.type           = VID_RASTER_525,
	.frame_rate     = { 30000, 1001 },
	.frame_aspects  = { { 4, 3 } },
	.lines          = 525,
	.hline          = 263,

	.active_lines   = 480,
	.active_width   = 0.00005290, /* 52.90µs */
	.active_left    = 0.00000920, /* |-->| 9.20µs */

	.hsync_width       = 0.00000470, /*  4.70 ±1.00µs */
	.vsync_short_width = 0.00000230, /*  2.30 ±0.10µs */
	.vsync_long_width  = 0.00002710, /* 27.10µs */
	.sync_rise         = 0.00000025, /*  0.25µs */

	.white_level    = 0.125000,
	.black_level    = 0.703125,
	.blanking_level = 0.750000,
	.sync_level     = 1.000000,

	.colour_mode    = VID_NTSC,
	.burst_width    = 0.00000250, /* 2.5 ±0.28µs */
	.burst_rise     = 0.00000030, /* 0.30 ±0.10µs */
	.burst_left     = 0.00000530, /* |-->| 5.3 ±0.1µs */
	.burst_level    = 4.0 / 10.0, /* 4/10 of white - blanking level */
	.colour_carrier = { 39375000, 11 }, /* 3579545.4545... Hz */

	.rw_co          =  0.299, /* R weight */
	.gw_co          =  0.587, /* G weight */
	.bw_co          =  0.114, /* B weight */
	.ev_co          =  0.877,
	.eu_co          =  0.493,

	.fm_mono_carrier   = 4500000, /* Hz */
	.fm_mono_deviation = 25000, /* +/- Hz */
	.fm_mono_preemph   = VID_75US,
};

const vid_config_t vid_config_ntsc_i = {

	/* System I (NTSC) */
	.output_type    = RF_INT16_COMPLEX,

	.modulation     = VID_VSB,
	.vsb_upper_bw   = 5500000, /* Hz */
	.vsb_lower_bw   = 1250000, /* Hz */

	.level          = 1.0, /* Overall signal level */

	.video_level    = 0.71, /* Power level of video */
	.fm_mono_level  = 0.22, /* FM audio carrier power level */
	.nicam_level    = 0.07 / 2, /* NICAM audio carrier power level */

	.type           = VID_RASTER_525,
	.frame_rate     = { 30000, 1001 },
	.frame_aspects  = { { 4, 3 } },
	.lines          = 525,
	.hline          = 263,

	.active_lines   = 480,
	.active_width   = 0.00005290, /* 52.90µs */
	.active_left    = 0.00000920, /* |-->| 9.20µs */

	.hsync_width       = 0.00000470, /*  4.70 ±1.00µs */
	.vsync_short_width = 0.00000230, /*  2.30 ±0.10µs */
	.vsync_long_width  = 0.00002710, /* 27.10 µs */
	.sync_rise         = 0.00000025, /*  0.25 µs */

	.white_level    = 0.200000,
	.black_level    = 0.728571,
	.blanking_level = 0.771428,
	.sync_level     = 1.000000,

	.colour_mode    = VID_NTSC,
	.burst_width    = 0.00000250, /* 2.5 ±0.28µs */
	.burst_rise     = 0.00000030, /* 0.30 ±0.10µs */
	.burst_left     = 0.00000530, /* |-->| 5.3 ±0.1µs */
	.burst_level    = 4.0 / 10.0, /* 4/10 of white - blanking level */
	.colour_carrier = { 39375000, 11 }, /* 3579545.4545... Hz */

	.rw_co          =  0.299, /* R weight */
	.gw_co          =  0.587, /* G weight */
	.bw_co          =  0.114, /* B weight */
	.ev_co          =  0.877,
	.eu_co          =  0.493,

	.fm_mono_carrier   = 6000000 - 400, /* Hz */
	.fm_mono_deviation = 50000, /* +/- Hz */
	.fm_mono_preemph   = VID_50US,

	.nicam_carrier  = 6552000, /* Hz */
	.nicam_beta     = 1.0,
};

const vid_config_t vid_config_ntsc_fm = {

	/* NTSC FM (satellite) */
	.output_type    = RF_INT16_COMPLEX,

	.modulation     = VID_FM,
	.fm_level       = 1.0,
	.fm_deviation   = 16e6, /* 16 MHz/V */

	.level          = 1.0, /* Overall signal level */

	.video_level    = 1.00, /* Power level of video */
	.fm_mono_level  = 0.05, /* FM audio carrier power level */
	//.fm_left_level  = 0.025, /* FM stereo left audio carrier power level */
	//.fm_right_level = 0.025, /* FM stereo right audio carrier power level */

	.type           = VID_RASTER_525,
	.frame_rate     = { 30000, 1001 },
	.frame_aspects  = { { 4, 3 } },
	.lines          = 525,
	.hline          = 263,

	.active_lines   = 480,
	.active_width   = 0.00005290, /* 52.90µs */
	.active_left    = 0.00000920, /* |-->| 9.20µs */

	.hsync_width       = 0.00000470, /*  4.70 ±1.00µs */
	.vsync_short_width = 0.00000230, /*  2.30 ±0.10µs */
	.vsync_long_width  = 0.00002710, /* 27.10 µs */
	.sync_rise         = 0.00000025, /*  0.25 µs */

	.white_level    =  0.5000,
	.black_level    = -0.1607,
	.blanking_level = -0.2143,
	.sync_level     = -0.5000,

	.colour_mode    = VID_NTSC,
	.burst_width    = 0.00000250, /* 2.5 ±0.28µs */
	.burst_rise     = 0.00000030, /* 0.30 ±0.10µs */
	.burst_left     = 0.00000530, /* |-->| 5.3 ±0.1µs */
	.burst_level    = 4.0 / 10.0, /* 4/10 of white - blanking level */
	.colour_carrier = { 39375000, 11 }, /* 3579545.4545... Hz */

	.rw_co          =  0.299, /* R weight */
	.gw_co          =  0.587, /* G weight */
	.bw_co          =  0.114, /* B weight */
	.ev_co          =  0.877,
	.eu_co          =  0.493,

	.fm_mono_carrier   = 6500000, /* Hz */
	.fm_mono_deviation = 85000, /* +/- Hz */
	.fm_mono_preemph   = VID_50US, /* Seconds */

	//.fm_left_carrier   = 7020000, /* Hz */
	//.fm_left_deviation = 50000, /* +/- Hz */
	//.fm_left_preemph   = VID_50US, /* Seconds */

	//.fm_right_carrier   = 7200000, /* Hz */
	//.fm_right_deviation = 50000, /* +/- Hz */
	//.fm_right_preemph   = VID_50US, /* Seconds */
};

const vid_config_t vid_config_ntsc_bs_fm = {

	/* Digital Subcarrier/NTSC FM (satellite) */
	.output_type    = RF_INT16_COMPLEX,

	.modulation     = VID_FM,
	.fm_level       = 1.0,
	.fm_deviation   = 17.0e6, /* 17.0 MHz/V */

	.level          = 1.0, /* Overall signal level */

	.video_level    = 1.00, /* Power level of video */
	.dance_level    = 0.19, /* DANCE audio carrier power level */

	.type           = VID_RASTER_525,
	.frame_rate     = { 30000, 1001 },
	.frame_aspects  = { { 4, 3 } },
	.lines          = 525,
	.hline          = 263,

	.active_lines   = 480,
	.active_width   = 0.00005290, /* 52.90µs */
	.active_left    = 0.00000920, /* |-->| 9.20µs */

	.hsync_width       = 0.00000470, /*  4.70 ±1.00µs */
	.vsync_short_width = 0.00000230, /*  2.30 ±0.10µs */
	.vsync_long_width  = 0.00002710, /* 27.10 µs */
	.sync_rise         = 0.00000025, /*  0.25µs */

	.white_level    =  0.5000,
	.black_level    = -0.2143,
	.blanking_level = -0.2143,
	.sync_level     = -0.5000,

	.colour_mode    = VID_NTSC,
	.burst_width    = 0.00000250, /* 2.5 ±0.28µs */
	.burst_rise     = 0.00000030, /* 0.30 ±0.10µs */
	.burst_left     = 0.00000530, /* |-->| 5.3 ±0.1µs */
	.burst_level    = 4.0 / 10.0, /* 4/10 of white - blanking level */
	.colour_carrier = { 39375000, 11 }, /* 3579545.4545... Hz */

	.rw_co          =  0.299, /* R weight */
	.gw_co          =  0.587, /* G weight */
	.bw_co          =  0.114, /* B weight */
	.ev_co          =  0.877,
	.eu_co          =  0.493,

	.dance_carrier  = 5000000.0 * 63 / 88 * 8 / 5, /* Hz */
	.dance_beta     = 1.0,
};

const vid_config_t vid_config_ntsc = {

	/* Composite NTSC */
	.output_type    = RF_INT16_REAL,

	.level          = 1.0, /* Overall signal level */
	.video_level    = 1.0, /* Power level of video */

	.video_bw       = 6.0e6,

	.type           = VID_RASTER_525,
	.frame_rate     = { 30000, 1001 },
	.frame_aspects  = { { 4, 3 } },
	.lines          = 525,
	.hline          = 263,

	.active_lines   = 480,
	.active_width   = 0.00005290, /* 52.90µs */
	.active_left    = 0.00000920, /* |-->| 9.20µs */

	.hsync_width       = 0.00000470, /*  4.70 ±1.00µs */
	.vsync_short_width = 0.00000230, /*  2.30 ±0.10µs */
	.vsync_long_width  = 0.00002710, /* 27.10 µs */
	.sync_rise         = 0.00000025, /*  0.25 µs */

	.white_level    =  100.0 / 140,
	.black_level    =    7.5 / 140,
	.blanking_level =    0.0 / 140,
	.sync_level     =  -40.0 / 140,

	.colour_mode    = VID_NTSC,
	.burst_width    = 0.00000250, /* 2.5 ±0.28µs */
	.burst_rise     = 0.00000030, /* 0.30 ±0.10µs */
	.burst_left     = 0.00000530, /* |-->| 5.3 ±0.1µs */
	.burst_level    = 4.0 / 10.0, /* 4/10 of white - blanking level */
	.colour_carrier = { 39375000, 11 }, /* 3579545.4545... Hz */

	.rw_co          =  0.299, /* R weight */
	.gw_co          =  0.587, /* G weight */
	.bw_co          =  0.114, /* B weight */
	.ev_co          =  0.877,
	.eu_co          =  0.493,
};

const vid_config_t vid_config_pal60_i = {

	/* System I (525-line PAL) */
	.output_type    = RF_INT16_COMPLEX,

	.modulation     = VID_VSB,
	.vsb_upper_bw   = 5500000, /* Hz */
	.vsb_lower_bw   = 1250000, /* Hz */

	.level          = 1.0, /* Overall signal level */

	.video_level    = 0.71, /* Power level of video */
	.fm_mono_level  = 0.22, /* FM audio carrier power level */
	.nicam_level    = 0.07 / 2, /* NICAM audio carrier power level */

	.type           = VID_RASTER_525,
	.frame_rate     = { 30000, 1001 },
	.frame_aspects  = { { 4, 3 } },
	.lines          = 525,
	.hline          = 263,

	.active_lines   = 480,
	.active_width   = 0.00005290, /* 52.90µs */
	.active_left    = 0.00000920, /* |-->| 9.20µs */

	.hsync_width       = 0.00000470, /*  4.70 ±1.00µs */
	.vsync_short_width = 0.00000230, /*  2.30 ±0.10µs */
	.vsync_long_width  = 0.00002710, /* 27.10 µs */
	.sync_rise         = 0.00000025, /*  0.25 +0.05µs */

	.white_level    = 0.20,
	.black_level    = 0.76,
	.blanking_level = 0.76,
	.sync_level     = 1.00,

	.colour_mode    = VID_PAL,
	.burst_width    = 0.00000225, /* 2.25 ±0.23µs */
	.burst_rise     = 0.00000030, /* 0.30 ±0.10µs */
	.burst_left     = 0.00000560, /* |-->| 5.6 ±0.1µs */
	.burst_level    = 3.0 / 7.0, /* 3 / 7 of white - blanking level */
	.colour_carrier = { 17734475, 4 }, /* 4433618.75 Hz */

	.rw_co          = 0.299, /* R weight */
	.gw_co          = 0.587, /* G weight */
	.bw_co          = 0.114, /* B weight */
	.ev_co          = 0.877,
	.eu_co          = 0.493,

	.fm_mono_carrier   = 6000000 - 400, /* Hz */
	.fm_mono_deviation = 50000, /* +/- Hz */
	.fm_mono_preemph   = VID_50US,

	.nicam_carrier  = 6552000, /* Hz */
	.nicam_beta     = 1.0,
};

const vid_config_t vid_config_pal60 = {

	/* Composite 525-line PAL */
	.output_type    = RF_INT16_REAL,

	.level          = 1.0, /* Overall signal level */
	.video_level    = 1.0, /* Power level of video */

	.video_bw       = 6.0e6,

	.type           = VID_RASTER_525,
	.frame_rate     = { 30000, 1001 },
	.frame_aspects  = { { 4, 3 } },
	.lines          = 525,
	.hline          = 263,

	.active_lines   = 480,
	.active_width   = 0.00005290, /* 52.90µs */
	.active_left    = 0.00000920, /* |-->| 9.20µs */

	.hsync_width       = 0.00000470, /*  4.70 ±1.00µs */
	.vsync_short_width = 0.00000230, /*  2.30 ±0.10µs */
	.vsync_long_width  = 0.00002710, /* 27.10 µs */

	.white_level    =  0.70,
	.black_level    =  0.00,
	.blanking_level =  0.00,
	.sync_level     = -0.30,

	.colour_mode    = VID_PAL,
	.burst_width    = 0.00000225, /* 2.25 ±0.23µs */
	.burst_rise     = 0.00000030, /* 0.30 ±0.10µs */
	.burst_left     = 0.00000560, /* |-->| 5.6 ±0.1µs */
	.burst_level    = 3.0 / 7.0, /* 3 / 7 of white - blanking level */
	.colour_carrier = { 17734475, 4 }, /* 4433618.75 Hz */

	.rw_co          = 0.299, /* R weight */
	.gw_co          = 0.587, /* G weight */
	.bw_co          = 0.114, /* B weight */
	.ev_co          = 0.877,
	.eu_co          = 0.493,
};

const vid_config_t vid_config_d2mac_am = {

	/* D2-MAC AM */
	.output_type    = RF_INT16_COMPLEX,

	.modulation     = VID_AM,

	.type           = VID_MAC,
	.chid           = 0xE8B5,
	.frame_rate     = { 25, 1 },
	.frame_aspects  = { { 4, 3 }, { 16, 9 } },
	.lines          = 625,
	.hline          = 313,

	.active_lines   = 576,
	.active_left    = 585.0 / MAC_CLOCK_RATE,
	.active_width   = 702.0 / MAC_CLOCK_RATE,

	.level          = 1.00, /* Overall signal level */
	.video_level    = 0.85, /* Chrominance may clip if this is set to 1 */

	.white_level    =  0.10,
	.black_level    =  1.00,
	.blanking_level =  0.55,
	.sync_level     =  0.55,

	.mac_mode       = MAC_MODE_D2,

	.rw_co          = 0.299, /* R weight */
	.gw_co          = 0.587, /* G weight */
	.bw_co          = 0.114, /* B weight */
	.ev_co          = 0.927,
	.eu_co          = 0.733,
};

const vid_config_t vid_config_d2mac_fm = {

	/* D2-MAC FM (Satellite) */
	.output_type    = RF_INT16_COMPLEX,

	.modulation     = VID_FM,
	.fm_level       = 1.0,
	.fm_deviation   = 13.5e6, /* 13.5 MHz/V */

	.type           = VID_MAC,
	.chid           = 0xE8B5,
	.frame_rate     = { 25, 1 },
	.frame_aspects  = { { 4, 3 }, { 16, 9 } },
	.lines          = 625,
	.hline          = 313,

	.active_lines   = 576,
	.active_left    = 585.0 / MAC_CLOCK_RATE,
	.active_width   = 702.0 / MAC_CLOCK_RATE,

	.level          = 1.0, /* Overall signal level */
	.video_level    = 1.0, /* Power level of video */

	.white_level    =  0.50,
	.black_level    = -0.50,
	.blanking_level =  0.00,
	.sync_level     =  0.00,

	.mac_mode       = MAC_MODE_D2,

	.rw_co          = 0.299, /* R weight */
	.gw_co          = 0.587, /* G weight */
	.bw_co          = 0.114, /* B weight */
	.ev_co          = 0.927,
	.eu_co          = 0.733,
};

const vid_config_t vid_config_d2mac = {

	/* D2-MAC */
	.output_type    = RF_INT16_REAL,

	.video_bw       = 6.0e6,

	.type           = VID_MAC,
	.chid           = 0xE8B5,
	.frame_rate     = { 25, 1 },
	.frame_aspects  = { { 4, 3 }, { 16, 9 } },
	.lines          = 625,
	.hline          = 313,

	.active_lines   = 576,
	.active_left    = 585.0 / MAC_CLOCK_RATE,
	.active_width   = 702.0 / MAC_CLOCK_RATE,

	.level          = 1.0, /* Overall signal level */
	.video_level    = 1.0, /* Power level of video */

	.white_level    =  0.50,
	.black_level    = -0.50,
	.blanking_level =  0.00,
	.sync_level     =  0.00,

	.mac_mode       = MAC_MODE_D2,

	.rw_co          = 0.299, /* R weight */
	.gw_co          = 0.587, /* G weight */
	.bw_co          = 0.114, /* B weight */
	.ev_co          = 0.927,
	.eu_co          = 0.733,
};

const vid_config_t vid_config_dmac_am = {

	/* D-MAC AM */
	.output_type    = RF_INT16_COMPLEX,

	.modulation     = VID_AM,

	.type           = VID_MAC,
	.chid           = 0xE8B5,
	.frame_rate     = { 25, 1 },
	.frame_aspects  = { { 4, 3 }, { 16, 9 } },
	.lines          = 625,
	.hline          = 313,

	.active_lines   = 576,
	.active_left    = 585.0 / MAC_CLOCK_RATE,
	.active_width   = 702.0 / MAC_CLOCK_RATE,

	.level          = 1.00, /* Overall signal level */
	.video_level    = 0.85, /* Chrominance may clip if this is set to 1 */

	.white_level    =  0.10,
	.black_level    =  1.00,
	.blanking_level =  0.55,
	.sync_level     =  0.55,

	.mac_mode       = MAC_MODE_D,

	.rw_co          = 0.299, /* R weight */
	.gw_co          = 0.587, /* G weight */
	.bw_co          = 0.114, /* B weight */
	.ev_co          = 0.927,
	.eu_co          = 0.733,
};

const vid_config_t vid_config_dmac_fm = {

	/* D2-MAC FM (Satellite) */
	.output_type    = RF_INT16_COMPLEX,

	.modulation     = VID_FM,
	.fm_level       = 1.0,
	.fm_deviation   = 13.5e6, /* 13.5 MHz/V */

	.type           = VID_MAC,
	.chid           = 0xE8B5,
	.frame_rate     = { 25, 1 },
	.frame_aspects  = { { 4, 3 }, { 16, 9 } },
	.lines          = 625,
	.hline          = 313,

	.active_lines   = 576,
	.active_left    = 585.0 / MAC_CLOCK_RATE,
	.active_width   = 702.0 / MAC_CLOCK_RATE,

	.level          = 1.0, /* Overall signal level */
	.video_level    = 1.0, /* Power level of video */

	.white_level    =  0.50,
	.black_level    = -0.50,
	.blanking_level =  0.00,
	.sync_level     =  0.00,

	.mac_mode       = MAC_MODE_D,

	.rw_co          = 0.299, /* R weight */
	.gw_co          = 0.587, /* G weight */
	.bw_co          = 0.114, /* B weight */
	.ev_co          = 0.927,
	.eu_co          = 0.733,
};

const vid_config_t vid_config_dmac = {

	/* D-MAC */
	.output_type    = RF_INT16_REAL,

	.video_bw       = 8.4e6,

	.type           = VID_MAC,
	.chid           = 0xE8B5,
	.frame_rate     = { 25, 1 },
	.frame_aspects  = { { 4, 3 }, { 16, 9 } },
	.lines          = 625,
	.hline          = 313,

	.active_lines   = 576,
	.active_left    = 585.0 / MAC_CLOCK_RATE,
	.active_width   = 702.0 / MAC_CLOCK_RATE,

	.level          = 1.0, /* Overall signal level */
	.video_level    = 1.0, /* Power level of video */

	.white_level    =  0.50,
	.black_level    = -0.50,
	.blanking_level =  0.00,
	.sync_level     =  0.00,

	.mac_mode       = MAC_MODE_D,

	.rw_co          = 0.299, /* R weight */
	.gw_co          = 0.587, /* G weight */
	.bw_co          = 0.114, /* B weight */
	.ev_co          = 0.927,
	.eu_co          = 0.733,
};

const vid_config_t vid_config_819_e = {

	/* System E (819 line monochrome, French variant) */
	.output_type    = RF_INT16_COMPLEX,

	.modulation     = VID_VSB,
	.vsb_upper_bw   =  2000000, /* Hz */
	.vsb_lower_bw   = 10400000, /* Hz */

	.level          = 1.0, /* Overall signal level */
	.video_level    = 0.8, /* Power level of video */
	.am_audio_level = 0.2, /* Power level of audio */

	.type           = VID_RASTER_819,
	.frame_rate     = { 25, 1 },
	.frame_aspects  = { { 4, 3 } },
	.lines          = 819,
	.hline          = 409,

	.active_lines   = 720, /* Normally 738 */
	.active_width   = 0.00003944, /* 39.44µs */
	.active_left    = 0.00000890, /* |-->| 8.9µs */

	.hsync_width      = 0.00000250, /* 2.50 ±0.10µs */
	.vsync_long_width = 0.00002000, /* 20.0 ±1.00µs */

	.white_level    = 1.00,
	.black_level    = 0.35,
	.blanking_level = 0.30,
	.sync_level     = 0.00,

	.rw_co          = 0.299, /* R weight */
	.gw_co          = 0.587, /* G weight */
	.bw_co          = 0.114, /* B weight */

	/* AM modulated */
	.am_mono_carrier   = 11.15e6, /* Hz */
	.am_mono_bandwidth =   10000, /* Hz */
};

const vid_config_t vid_config_819 = {

	/* 819 line video, French variant */
	.output_type    = RF_INT16_REAL,

	.video_bw       = 10.4e6,

	.level          = 1.0, /* Overall signal level */
	.video_level    = 1.0, /* Power level of video */

	.type           = VID_RASTER_819,
	.frame_rate     = { 25, 1 },
	.frame_aspects  = { { 4, 3 } },
	.lines          = 819,
	.hline          = 409,

	.active_lines   = 720, /* Normally 738 */
	.active_width   = 0.00003944, /* 39.44µs */
	.active_left    = 0.00000890, /* |-->| 8.9µs */

	.hsync_width      = 0.00000250, /* 2.50 ±0.10µs */
	.vsync_long_width = 0.00002000, /* 20.0 ±1.00µs */

	.white_level    =  0.70,
	.black_level    =  0.05,
	.blanking_level =  0.00,
	.sync_level     = -0.30,

	.rw_co          = 0.299, /* R weight */
	.gw_co          = 0.587, /* G weight */
	.bw_co          = 0.114, /* B weight */
};

const vid_config_t vid_config_405_a = {

	/* System A (405 line monochrome) */
	.output_type    = RF_INT16_COMPLEX,

	.modulation     = VID_VSB,
	.vsb_upper_bw   =  750000, /* Hz */
	.vsb_lower_bw   = 3000000, /* Hz */

	.level          = 1.0, /* Overall signal level */
	.video_level    = 0.8, /* Power level of video */
	.am_audio_level = 0.2, /* Power level of audio */

	.type           = VID_RASTER_405,
	.frame_rate     = { 25, 1 },
	.frame_aspects  = { { 4, 3 } },
	.lines          = 405,
	.hline          = 203,

	.active_lines   = 376,
	.active_width   = 0.00008030, /* 80.3µs */
	.active_left    = 0.00001680, /* |-->| 16.8µs */

	.hsync_width       = 0.00000900, /*  9.00 ±1.00 µs */
	.vsync_long_width  = 0.00004000, /* 40.00 ±2.00 µs */
	.sync_rise         = 0.00000025, /*  0.25 µs */

	.white_level    = 1.00,
	.black_level    = 0.30,
	.blanking_level = 0.30,
	.sync_level     = 0.00,

	.rw_co          = 0.299, /* R weight */
	.gw_co          = 0.587, /* G weight */
	.bw_co          = 0.114, /* B weight */

	/* AM modulated */
	.am_mono_carrier = -3500000, /* Hz */
	.am_mono_bandwidth = 10000, /* Hz */
};

const vid_config_t vid_config_405_a_ntsc = {

	/* System A (405 line NTSC, based on specs from
	 * BBC Engineering Division Monograph No. 32, Appendix A) */
	.output_type    = RF_INT16_COMPLEX,

	.modulation     = VID_VSB,
	.vsb_upper_bw   =  750000, /* Hz */
	.vsb_lower_bw   = 3000000, /* Hz */

	.level          = 1.0, /* Overall signal level */
	.video_level    = 0.80 / 1.22, /* Power level of video (reduced for NTSC 122% overshoot) */
	.am_audio_level = 0.20, /* Power level of audio */

	.type           = VID_RASTER_405,
	.frame_rate     = { 25, 1 },
	.frame_aspects  = { { 4, 3 } },
	.lines          = 405,
	.hline          = 203,

	.active_lines   = 376,
	.active_width   = 0.00008030, /* 80.3µs */
	.active_left    = 0.00001680, /* |-->| 16.8µs */

	.hsync_width       = 0.00000900, /*  9.00 ±1.00 µs */
	.vsync_long_width  = 0.00004000, /* 40.00 ±2.00 µs */
	.sync_rise         = 0.00000025, /*  0.25 µs */

	.white_level    =  1.00,
	.black_level    =  0.35,
	.blanking_level =  0.30,
	.sync_level     =  0.00,

	.colour_mode    = VID_NTSC,
	.burst_width    = 0.00000339, /* 3.39 ±0.38µs */
	.burst_rise     = 0.00000030, /* 0.30 ±0.10µs */
	.burst_left     = 0.00001050, /* |-->| 10.5 ±0.1µs */
	.burst_level    = 3.0 / 7.0, /* 30% full carrier */
	.colour_carrier = { 5315625, 2 }, /* 2657812.5 Hz */

	.rw_co          =  0.299, /* R weight */
	.gw_co          =  0.587, /* G weight */
	.bw_co          =  0.114, /* B weight */
	.ev_co          =  0.877,
	.eu_co          =  0.493,

	/* AM modulated */
	.am_mono_carrier = -3500000, /* Hz */
	.am_mono_bandwidth = 10000, /* Hz */
};

const vid_config_t vid_config_405_i = {

	/* System A (405 line monochrome) */
	.output_type    = RF_INT16_COMPLEX,

	.modulation     = VID_VSB,
	.vsb_upper_bw   = 5500000, /* Hz */
	.vsb_lower_bw   = 1250000, /* Hz */

	.level          = 1.0, /* Overall signal level */

	.video_level    = 0.80, /* Power level of video */
	.fm_mono_level  = 0.19, /* FM audio carrier power level */

	.type           = VID_RASTER_405,
	.frame_rate     = { 25, 1 },
	.frame_aspects  = { { 4, 3 } },
	.lines          = 405,
	.hline          = 203,

	.active_lines   = 376,
	.active_width   = 0.00008030, /* 80.3µs */
	.active_left    = 0.00001680, /* |-->| 16.8µs */

	.hsync_width       = 0.00000900, /*  9.00 ±1.00 µs */
	.vsync_long_width  = 0.00004000, /* 40.00 ±2.00 µs */
	.sync_rise         = 0.00000025, /*  0.25 µs */

	.white_level    = 0.20,
	.black_level    = 0.76,
	.blanking_level = 0.76,
	.sync_level     = 1.00,

	.rw_co          = 0.299, /* R weight */
	.gw_co          = 0.587, /* G weight */
	.bw_co          = 0.114, /* B weight */

	.fm_mono_carrier   = 6000000 - 400, /* Hz */
	.fm_mono_deviation = 50000, /* +/- Hz */
	.fm_mono_preemph   = VID_50US,
};

const vid_config_t vid_config_405 = {

	/* 405 line video */
	.output_type    = RF_INT16_REAL,

	.level          = 1.0, /* Overall signal level */
	.video_level    = 1.0, /* Power level of video */

	.video_bw       = 3.0e6,

	.type           = VID_RASTER_405,
	.frame_rate     = { 25, 1 },
	.frame_aspects  = { { 4, 3 } },
	.lines          = 405,
	.hline          = 203,

	.active_lines   = 376,
	.active_width   = 0.00008030, /* 80.3µs */
	.active_left    = 0.00001680, /* |-->| 16.8µs */

	.hsync_width       = 0.00000900, /*  9.00 ±1.00µs */
	.vsync_long_width  = 0.00004000, /* 40.00 ±2.00µs */
	.sync_rise         = 0.00000025, /*  0.25 µs */

	.white_level    =  0.70,
	.black_level    =  0.00,
	.blanking_level =  0.00,
	.sync_level     = -0.30,

	.rw_co          = 0.299, /* R weight */
	.gw_co          = 0.587, /* G weight */
	.bw_co          = 0.114, /* B weight */
};

const vid_config_t vid_config_405_ntsc = {

	/* 405 line video (NTSC, based on specs from
	 * BBC Engineering Division Monograph No. 32, Appendix A) */
	.output_type    = RF_INT16_REAL,

	.level          = 1.0, /* Overall signal level */
	.video_level    = 1.0, /* Power level of video */

	.video_bw       = 3.0e6,

	.type           = VID_RASTER_405,
	.frame_rate     = { 25, 1 },
	.frame_aspects  = { { 4, 3 } },
	.lines          = 405,
	.hline          = 203,

	.active_lines   = 376,
	.active_width   = 0.00008030, /* 80.3µs */
	.active_left    = 0.00001680, /* |-->| 16.8µs */

	.hsync_width       = 0.00000900, /*  9.00 ±1.00µs */
	.vsync_long_width  = 0.00004000, /* 40.00 ±2.00µs */
	.sync_rise         = 0.00000025, /*  0.25 µs */

	.white_level    =  0.70,
	.black_level    =  0.05,
	.blanking_level =  0.00,
	.sync_level     = -0.30,

	.colour_mode    = VID_NTSC,
	.burst_width    = 0.00000339, /* 3.39 ±0.38µs */
	.burst_rise     = 0.00000030, /* 0.30 ±0.10µs */
	.burst_left     = 0.00001050, /* |-->| 10.5 ±0.1µs */
	.burst_level    = 3.0 / 7.0, /* 30% full carrier */
	.colour_carrier = { 5315625, 2 }, /* 2657812.5 Hz */

	.rw_co          =  0.299, /* R weight */
	.gw_co          =  0.587, /* G weight */
	.bw_co          =  0.114, /* B weight */
	.ev_co          =  0.877,
	.eu_co          =  0.493,
};

const vid_config_t vid_config_baird_240_am = {

	/* Baird 240 line, AM modulation */
	.output_type    = RF_INT16_COMPLEX,

	.modulation     = VID_AM,

	.level          = 1.0, /* Overall signal level */
	.video_level    = 1.0, /* Power level of video */

	.type           = VID_BAIRD_240,
	.frame_rate     = { 25, 1 },
	.frame_aspects  = { { 4, 3 } },
	.lines          = 240,

	.active_lines   = 220,
	.active_width   = 0.00015,     /* 150µs */
	.active_left    = 0.000016667, /* |-->| 16.667µs */

	.hsync_width      = 0.000013333, /* 13.333µs */
	.vsync_long_width = 0.000166667, /* 166.667µs */

	.white_level    = 1.00,
	.black_level    = 0.40,
	.blanking_level = 0.40,
	.sync_level     = 0.00,

	.rw_co          = 0.299, /* R weight */
	.gw_co          = 0.587, /* G weight */
	.bw_co          = 0.114, /* B weight */
};

const vid_config_t vid_config_baird_240 = {

	/* Baird 240 line */
	.output_type    = RF_INT16_REAL,

	.level          = 1.0, /* Overall signal level */
	.video_level    = 1.0, /* Power level of video */

	.type           = VID_BAIRD_240,
	.frame_rate     = { 25, 1 },
	.frame_aspects  = { { 4, 3 } },
	.lines          = 240,

	.active_lines   = 220,
	.active_width   = 0.00015,     /* 150µs */
	.active_left    = 0.000016667, /* |-->| 16.667µs */

	.hsync_width      = 0.000013333, /* 13.333µs */
	.vsync_long_width = 0.000166667, /* 166.667µs */

	.white_level    = 1.00,
	.black_level    = 0.40,
	.blanking_level = 0.40,
	.sync_level     = 0.00,

	.rw_co          = 0.299, /* R weight */
	.gw_co          = 0.587, /* G weight */
	.bw_co          = 0.114, /* B weight */
};

const vid_config_t vid_config_baird_30_am = {

	/* Baird 30 line, AM modulation */
	.output_type    = RF_INT16_COMPLEX,

	.modulation     = VID_AM,

	.level          = 1.0, /* Overall signal level */
	.video_level    = 1.0, /* Power level of video */

	.type           = VID_BAIRD_30,
	.frame_rate     = { 25, 2 }, /* 12.5 Hz */
	.frame_aspects  = { { 3, 7 } },
	.frame_orientation = VID_ROTATE_270 | VID_HFLIP,
	.lines          = 30,

	.active_lines   = 30,
	.active_width   = 0.002666667, /* 2.667ms */
	.active_left    = 0,

	.white_level    = 1.00,
	.black_level    = 0.00,
	.blanking_level = 0.00,
	.sync_level     = 0.00,

	.rw_co          = 0.299, /* R weight */
	.gw_co          = 0.587, /* G weight */
	.bw_co          = 0.114, /* B weight */
};

const vid_config_t vid_config_baird_30 = {

	/* Baird 30 line */
	.output_type    = RF_INT16_REAL,

	.level          = 1.0, /* Overall signal level */
	.video_level    = 1.0, /* Power level of video */

	.type           = VID_BAIRD_30,
	.frame_rate     = { 25, 2 }, /* 12.5 Hz */
	.frame_aspects  = { { 3, 7 } },
	.frame_orientation = VID_ROTATE_270 | VID_HFLIP,
	.lines          = 30,

	.active_lines   = 30,
	.active_width   = 0.002666667, /* 2.667ms */
	.active_left    = 0,

	.white_level    =  1.00,
	.black_level    = -1.00,
	.blanking_level = -1.00,
	.sync_level     = -1.00,

	.rw_co          = 0.299, /* R weight */
	.gw_co          = 0.587, /* G weight */
	.bw_co          = 0.114, /* B weight */
};

const vid_config_t vid_config_nbtv_32_am = {

	/* NBTV Club standard, AM modulation (negative) */
	.output_type    = RF_INT16_COMPLEX,

	.modulation     = VID_AM,

	.level          = 1.0, /* Overall signal level */
	.video_level    = 1.0, /* Power level of video */

	.type           = VID_NBTV_32,
	.frame_rate     = { 25, 2 }, /* 12.5 Hz */
	.frame_aspects  = { { 2, 3 } },
	.frame_orientation = VID_ROTATE_270 | VID_HFLIP,
	.lines          = 32,

	.active_lines   = 32,
	.active_width   = 2.5e-3 - 0.1e-3, /* 2.5ms - hsync */
	.active_left    = 0.1e-3,

	.hsync_width    = 0.1e-3, /* 0.1 to 0.25ms */

	.white_level    = 0.10,
	.black_level    = 0.73,
	.blanking_level = 0.73,
	.sync_level     = 1.00,

	.rw_co          = 0.299, /* R weight */
	.gw_co          = 0.587, /* G weight */
	.bw_co          = 0.114, /* B weight */
};

const vid_config_t vid_config_nbtv_32 = {

	/* NBTV Club standard */
	.output_type    = RF_INT16_REAL,

	.level          = 1.0, /* Overall signal level */
	.video_level    = 1.0, /* Power level of video */

	.type           = VID_NBTV_32,
	.frame_rate     = { 25, 2 }, /* 12.5 Hz */
	.frame_aspects  = { { 2, 3 } },
	.frame_orientation = VID_ROTATE_270 | VID_HFLIP,
	.lines          = 32,

	.active_lines   = 32,
	.active_width   = 2.5e-3 - 0.1e-3, /* 2.5ms - hsync */
	.active_left    = 0.1e-3,

	.hsync_width    = 0.1e-3, /* 0.1 to 0.25ms */

	.white_level    = 1.00,
	.black_level    = 0.30,
	.blanking_level = 0.30,
	.sync_level     = 0.00,

	.rw_co          = 0.299, /* R weight */
	.gw_co          = 0.587, /* G weight */
	.bw_co          = 0.114, /* B weight */
};

const vid_config_t vid_config_apollo_colour_fm = {

	/* Unified S-Band, Apollo Colour Lunar Television */
	.output_type    = RF_INT16_COMPLEX,

	.level          = 1.000, /* Overall signal level */
	.video_level    = 1.000, /* Power level of video */
	.fm_mono_level  = 0.150, /* Power level of audio */

	.modulation     = VID_FM,
	.fm_level       = 1.0,
	.fm_deviation   = 2e6, /* 2 MHz/V */

	.type           = VID_RASTER_525,
	.frame_rate     = { 30000, 1001 },
	.frame_aspects  = { { 4, 3 } },
	.lines          = 525,
	.hline          = 263,

	.active_lines   = 480,
	.active_width   = 0.00005290, /* 52.90µs */
	.active_left    = 0.00000920, /* |-->| 9.20µs */

	.hsync_width       = 0.00000470, /*  4.70 ±1.00µs */
	.vsync_short_width = 0.00000230, /*  2.30 ±0.10µs */
	.vsync_long_width  = 0.00002710, /* 27.10 µs */
	.sync_rise         = 0.00000025, /*  0.25 µs */

	.white_level    =  0.5000,
	.black_level    = -0.1475,
	.blanking_level = -0.2000,
	.sync_level     = -0.5000,

	.colour_mode    = VID_APOLLO_FSC,
	.fsc_flag_width = 0.00002000, /* 20.00µs */
	.fsc_flag_left  = 0.00001470, /* |-->| 14.70µs */
	.fsc_flag_level = 0.5000,

	.rw_co          =  0.299, /* R weight */
	.gw_co          =  0.587, /* G weight */
	.bw_co          =  0.114, /* B weight */

	/* The audio carrier overlaps the video signal and
	 * requires the video to either be low pass filtered
	 * to 750kHz (Apollo 10 to 14) or cancelled out
	 * in post-processing (Apollo 15-17). */

	.fm_mono_carrier   = 1250000, /* Hz */
	.fm_mono_deviation = 25000, /* +/- Hz */
	.fm_mono_preemph   = VID_NONE,
};

const vid_config_t vid_config_apollo_colour = {

	/* Apollo Colour Lunar Television */
	.output_type    = RF_INT16_REAL,

	.level          = 1.0, /* Overall signal level */
	.video_level    = 1.0, /* Power level of video */

	.type           = VID_RASTER_525,
	.frame_rate     = { 30000, 1001 },
	.frame_aspects  = { { 4, 3 } },
	.lines          = 525,
	.hline          = 263,

	.active_lines   = 480,
	.active_width   = 0.00005290, /* 52.90µs */
	.active_left    = 0.00000920, /* |-->| 9.20µs */

	.hsync_width       = 0.00000470, /*  4.70 ±1.00µs */
	.vsync_short_width = 0.00000230, /*  2.30 ±0.10µs */
	.vsync_long_width  = 0.00002710, /* 27.10 µs */
	.sync_rise         = 0.00000025, /*  0.25 µs */

	.white_level    =  0.70,
	.black_level    =  0.0525,
	.blanking_level =  0.00,
	.sync_level     = -0.30,

	.colour_mode    = VID_APOLLO_FSC,
	.fsc_flag_width = 0.00002000, /* 20.00µs */
	.fsc_flag_left  = 0.00001470, /* |-->| 14.70µs */
	.fsc_flag_level = 0.70,

	.rw_co          =  0.299, /* R weight */
	.gw_co          =  0.587, /* G weight */
	.bw_co          =  0.114, /* B weight */
};

const vid_config_t vid_config_apollo_mono_fm = {

	/* Unified S-Band, Apollo Lunar Television 10 fps video (Mode 1) */
	.output_type    = RF_INT16_COMPLEX,

	.level          = 1.000, /* Overall signal level */
	.video_level    = 1.000, /* Power level of video */
	.fm_mono_level  = 0.150, /* Power level of audio */

	.modulation     = VID_FM,
	.fm_level       = 1.0,
	.fm_deviation   = 2e6, /* 2 MHz/V */

	.type           = VID_APOLLO_320,
	.frame_rate     = { 10, 1 },
	.frame_aspects  = { { 4, 3 } },
	.lines          = 320,
	.active_lines   = 312,
	.active_width   = 0.00028250, /* 282.5µs */
	.active_left    = 0.00002500, /* |-->| 25.0µs */

	.hsync_width       = 0.00002000, /* 20.00µs */
	.vsync_long_width  = 0.00026750, /* 267.5µs */

	/* Hacky: hacktv can't generate a single pulse wider than half a line,
	 * which we need here. Use the vsync short pulse to complete the long */
	.vsync_short_width = 1.0 / 10.0 / 320.0 / 2.0 - 45e-6,

	/* The Apollo TV camera supports a pulse and tone sync mode. The
	 * pulse mode is a normal negative pulse, and the tone mode uses
	 * a 409600 Hz tone. I'm not sure which was used for the live
	 * transmissions from the surface. This implementation uses the
	 * negative pulse mode. */

	.white_level    =  0.50,
	.black_level    = -0.20,
	.blanking_level = -0.20,
	.sync_level     = -0.50,

	/* These are copied from the NTSC values */
	.rw_co          = 0.299, /* R weight */
	.gw_co          = 0.587, /* G weight */
	.bw_co          = 0.114, /* B weight */

	.fm_mono_carrier   = 1250000, /* Hz */
	.fm_mono_deviation = 25000, /* +/- Hz */
	.fm_mono_preemph   = VID_NONE,
};

const vid_config_t vid_config_apollo_mono = {

	/* Apollo Lunar Television 10 fps video (Mode 1) */
	.output_type    = RF_INT16_REAL,

	.level          = 1.0, /* Overall signal level */
	.video_level    = 1.0, /* Power level of video */

	.type           = VID_APOLLO_320,
	.frame_rate     = { 10, 1 },
	.frame_aspects  = { { 4, 3 } },
	.lines          = 320,
	.active_lines   = 312,
	.active_width   = 0.00028250, /* 282.5µs */
	.active_left    = 0.00002500, /* |-->| 25.0µs */

	.hsync_width       = 0.00002000, /* 20.00µs */
	.vsync_long_width  = 0.00026750, /* 267.5µs */

	/* Hacky: hacktv can't generate a single pulse wider than half a line,
	 * which we need here. Use the vsync short pulse to complete the long */
	.vsync_short_width = 1.0 / 10.0 / 320.0 / 2.0 - 45e-6,

	/* The Apollo TV camera supports a pulse and tone sync mode. The
	 * pulse mode is a normal negative pulse, and the tone mode uses
	 * a 409600 Hz tone. I'm not sure which was used for the live
	 * transmissions from the surface. This implementation uses the
	 * negative pulse mode. */

	.white_level    =  0.70,
	.black_level    =  0.00,
	.blanking_level =  0.00,
	.sync_level     = -0.30,

	/* These are copied from the NTSC values */
	.rw_co          = 0.299, /* R weight */
	.gw_co          = 0.587, /* G weight */
	.bw_co          = 0.114, /* B weight */
};

const vid_config_t vid_config_cbs405_m = {

	/* System M (CBS 405-line Colour) */
	.output_type    = RF_INT16_COMPLEX,

	.modulation     = VID_VSB,
	.vsb_upper_bw   = 4200000, /* Hz */
	.vsb_lower_bw   =  750000, /* Hz */

	.level          = 1.0, /* Overall signal level */

	.video_level    = 0.77, /* Power level of video */
	.fm_mono_level  = 0.15, /* FM audio carrier power level */

	.type           = VID_CBS_405,
	.frame_rate     = { 72, 1 },
	.frame_aspects  = { { 4, 3 } },
	.lines          = 405,
	.hline          = 203,

	.active_lines   = 376, /* Estimate */
	.active_width   = 0.00002812, /* 28.12µs */
	.active_left    = 0.00000480, /* |-->| 4.80µs */

	.hsync_width       = 0.000002743, /*  2.743µs */
	.vsync_short_width = 0.000001372, /*  1.372µs */
	.vsync_long_width  = 0.000014746, /* 14.746µs */

	.white_level    = 0.159, /* 15% +0/-15 */
	.black_level    = 0.595, /* 75% +25/-25 */
	.blanking_level = 0.595, /* 75% +25/-25 */
	.sync_level     = 1.000, /* 100% */

	.colour_mode    = VID_CBS_FSC,
	.fsc_flag_width = 0.000001372, /* 1.372µs */
	.fsc_flag_left  = 0.000008573, /* |-->| 8.573µs */
	.fsc_flag_level = 1.000,

	.gamma          =  1.0,
	.rw_co          =  0.299, /* R weight */
	.gw_co          =  0.587, /* G weight */
	.bw_co          =  0.114, /* B weight */

	.fm_mono_carrier   = 4500000, /* Hz */
	.fm_mono_deviation = 25000, /* +/- Hz */
	.fm_mono_preemph   = VID_75US,
};

const vid_config_t vid_config_cbs405 = {

	/* CBS 405-line Colour */
	.output_type    = RF_INT16_REAL,

	.level          = 1.0, /* Overall signal level */
	.video_level    = 1.0, /* Power level of video */

	.type           = VID_CBS_405,
	.frame_rate     = { 72, 1 },
	.frame_aspects  = { { 4, 3 } },
	.lines          = 405,
	.hline          = 203,

	.active_lines   = 376, /* Estimate */
	.active_width   = 0.00002812, /* 28.12µs */
	.active_left    = 0.00000480, /* |-->| 4.80µs */

	.hsync_width       = 0.000002743, /*  2.743µs */
	.vsync_short_width = 0.000001372, /*  1.372µs */
	.vsync_long_width  = 0.000014746, /* 14.746µs */

	.white_level    =  0.70,
	.black_level    =  0.00,
	.blanking_level =  0.00,
	.sync_level     = -0.30,

	.colour_mode    = VID_CBS_FSC,
	.fsc_flag_width = 0.000001372, /* 1.372µs */
	.fsc_flag_left  = 0.000008573, /* |-->| 8.573µs */
	.fsc_flag_level = -0.30,

	.gamma          =  1.0,
	.rw_co          =  0.299, /* R weight */
	.gw_co          =  0.587, /* G weight */
	.bw_co          =  0.114, /* B weight */
};

const vid_configs_t vid_configs[] = {
	{ "i",             &vid_config_pal_i,            "PAL colour, 25 fps, 625 lines, AM (complex), 6.0 MHz FM audio" },
	{ "b",             &vid_config_pal_bg,           "PAL colour, 25 fps, 625 lines, AM (complex), 5.5 MHz FM audio" },
	{ "g",             &vid_config_pal_bg,           "PAL colour, 25 fps, 625 lines, AM (complex), 5.5 MHz FM audio" },
	{ "pal-d",         &vid_config_pal_dk,           "PAL colour, 25 fps, 625 lines, AM (complex), 6.5 MHz FM audio" },
	{ "pal-k",         &vid_config_pal_dk,           "PAL colour, 25 fps, 625 lines, AM (complex), 6.5 MHz FM audio" },
	{ "pal-fm",        &vid_config_pal_fm,           "PAL colour, 25 fps, 625 lines, FM (complex), 6.5 MHz FM audio" },
	{ "pal",           &vid_config_pal,              "PAL colour, 25 fps, 625 lines, unmodulated (real)" },
	{ "pal-m",         &vid_config_pal_m,            "PAL colour, 30/1.001 fps, 525 lines, AM (complex), 4.5 MHz FM audio" },
	{ "pal-n",         &vid_config_pal_n,            "PAL colour, 25 fps, 625 lines, AM (complex), 4.5 MHz FM audio" },
	{ "525pal",        &vid_config_525pal,           "PAL colour, 30/1.001 fps, 525 lines, unmodulated (real)" },
	{ "l",             &vid_config_secam_l,          "SECAM colour, 25 fps, 625 lines, AM (complex), 6.5 MHz AM audio" },
	{ "d",             &vid_config_secam_dk,         "SECAM colour, 25 fps, 625 lines, AM (complex), 6.5 MHz FM audio" },
	{ "k",             &vid_config_secam_dk,         "SECAM colour, 25 fps, 625 lines, AM (complex), 6.5 MHz FM audio" },
	{ "secam-i",       &vid_config_secam_i,          "SECAM colour, 25 fps, 625 lines, AM (complex), 6.0 MHz FM audio" },
	{ "secam-b",       &vid_config_secam_bg,         "SECAM colour, 25 fps, 625 lines, AM (complex), 5.5 MHz FM audio" },
	{ "secam-g",       &vid_config_secam_bg,         "SECAM colour, 25 fps, 625 lines, AM (complex), 5.5 MHz FM audio" },
	{ "secam-fm",      &vid_config_secam_fm,         "SECAM colour, 25 fps, 625 lines, FM (complex), 6.5 MHz FM audio" },
	{ "secam",         &vid_config_secam,            "SECAM colour, 25 fps, 625 lines, unmodulated (real)" },
	{ "m",             &vid_config_ntsc_m,           "NTSC colour, 30/1.001 fps, 525 lines, AM (complex), 4.5 MHz FM audio" },
	{ "ntsc-i",        &vid_config_ntsc_i,           "NTSC colour, 30/1.001 fps, 525 lines, AM (complex), 6.0 MHz FM audio" },
	{ "ntsc-fm",       &vid_config_ntsc_fm,          "NTSC colour, 30/1.001 fps, 525 lines, FM (complex), 6.5 MHz FM audio" },
	{ "ntsc-bs",       &vid_config_ntsc_bs_fm,       "NTSC colour, 30/1.001 fps, 525 lines, FM (complex), BS digital audio" },
	{ "ntsc",          &vid_config_ntsc,             "NTSC colour, 30/1.001 fps, 525 lines, unmodulated (real)" },
	{ "pal60-i",       &vid_config_pal60_i,          "PAL colour, 30/1.001 fps, 525 lines, AM (complex), 6.0 MHz FM audio" },
	{ "pal60",         &vid_config_pal60,            "PAL colour, 30/1.001 fps, 525 lines, unmodulated (real)" },
	{ "d2mac-am",      &vid_config_d2mac_am,         "D2-MAC, 25 fps, 625 lines, AM (complex)" },
	{ "d2mac-fm",      &vid_config_d2mac_fm,         "D2-MAC, 25 fps, 625 lines, FM (complex)" },
	{ "d2mac",         &vid_config_d2mac,            "D2-MAC, 25 fps, 625 lines, unmodulated (real)" },
	{ "dmac-am",       &vid_config_dmac_am,          "D-MAC, 25 fps, 625 lines, AM (complex)" },
	{ "dmac-fm",       &vid_config_dmac_fm,          "D-MAC, 25 fps, 625 lines, FM (complex)" },
	{ "dmac",          &vid_config_dmac,             "D-MAC, 25 fps, 625 lines, unmodulated (real)" },
	{ "e",             &vid_config_819_e,            "No colour, 25 fps, 819 lines, AM (complex), 11.15 MHz AM audio" },
	{ "819",           &vid_config_819,              "No colour, 25 fps, 819 lines, unmodulated (real)" },
	{ "a",             &vid_config_405_a,            "No colour, 25 fps, 405 lines, AM (complex), -3.5 MHz AM audio" },
	{ "ntsc-a",        &vid_config_405_a_ntsc,       "NTSC colour, 25 fps, 405 lines, AM (complex), -3.5 MHz AM audio" },
	{ "405-i",         &vid_config_405_i,            "No colour, 25 fps, 405 lines, AM (complex), 6.0 MHz FM audio" },
	{ "405",           &vid_config_405,              "No colour, 25 fps, 405 lines, unmodulated (real)" },
	{ "ntsc-405",      &vid_config_405_ntsc,         "NTSC colour, 25 fps, 405 lines, unmodulated (real)" },
	{ "240-am",        &vid_config_baird_240_am,     "No colour, 25 fps, 240 lines, AM (complex)" },
	{ "240",           &vid_config_baird_240,        "No colour, 25 fps, 240 lines, unmodulated (real)" },
	{ "30-am",         &vid_config_baird_30_am,      "No colour, 12.5 fps, 30 lines, AM (complex)" },
	{ "30",            &vid_config_baird_30,         "No colour, 12.5 fps, 30 lines, unmodulated (real)" },
	{ "nbtv-am",       &vid_config_nbtv_32_am,       "No colour, 12.5 fps, 32 lines, AM (complex)" },
	{ "nbtv",          &vid_config_nbtv_32,          "No colour, 12.5 fps, 32 lines, unmodulated (real)" },
	{ "apollo-fsc-fm", &vid_config_apollo_colour_fm, "Field sequential colour, 30/1.001 fps, 525 lines, FM (complex), 1.25 MHz FM audio" },
	{ "apollo-fsc",    &vid_config_apollo_colour,    "Field sequential colour, 30/1.001 fps, 525 lines, unmodulated (real)" },
	{ "apollo-fm",     &vid_config_apollo_mono_fm,   "No colour, 10 fps, 320 lines, FM (complex), 1.25 MHz FM audio" },
	{ "apollo",        &vid_config_apollo_mono,      "No colour, 10 fps, 320 lines, unmodulated (real)" },
	{ "m-cbs405",      &vid_config_cbs405_m,         "Field sequential colour, 72 fps, 405 lines, VSB (complex), 4.5MHz FM audio" },
	{ "cbs405",        &vid_config_cbs405,           "Field sequential colour, 72 fps, 405 lines, unmodulated (real)" },
	{ NULL },
};

/* Video filter process */
typedef struct {
	fir_int16_t fir;
} _vid_filter_process_t;

/* Test taps for a CCIR-405 625 line video pre-emphasis filter at 28 MHz (5.0 MHz video) */
const static double fm_625_28_taps[] = {
	-0.000044,-0.000123,-0.000013, 0.000314, 0.000430,-0.000132,-0.000988,
	-0.000896, 0.000719, 0.002308, 0.001357,-0.002190,-0.004444,-0.001393,
	0.005140, 0.007399, 0.000263,-0.010258,-0.010897, 0.003177, 0.018349,
	0.014300,-0.010707,-0.030617,-0.016515, 0.025701, 0.050066, 0.015639,
	-0.058048,-0.089638,-0.006506, 0.173162, 0.332763, 0.345728, 0.185481,
	-0.046213,-0.201920,-0.206786,-0.106002,-0.006900, 0.019743,-0.013618,
	-0.047161,-0.041577,-0.008463, 0.015556, 0.011768,-0.006738,-0.016378,
	-0.009218, 0.003516, 0.008066, 0.002674,-0.004123,-0.005084,-0.001072,
	0.002407, 0.002226,-0.000063,-0.001438,-0.000942, 0.000171, 0.000587,
	0.000255,-0.000129,-0.000176,-0.000044
};

/* Test taps for a CCIR-405 625 line video pre-emphasis filter at 20.25 MHz (5.0 MHz video) */
const static double fm_625_2025_taps[] = {
	0.000054,-0.000091,-0.000167, 0.000245, 0.000400,-0.000491,-0.000796,
	0.000863, 0.001433,-0.001386,-0.002399, 0.002085, 0.003806,-0.002983,
	-0.005790, 0.004096, 0.008525,-0.005443,-0.012247, 0.007042, 0.017295,
	-0.008933,-0.024219, 0.011194, 0.034017,-0.014007,-0.048798, 0.017821,
	0.073996,-0.023913,-0.129317, 0.037282, 0.388340, 0.480175, 0.142026,
	-0.242967,-0.276791,-0.067463, 0.033672,-0.035345,-0.073194,-0.009605,
	0.031429,-0.004353,-0.030396,-0.002519, 0.019121, 0.000684,-0.014885,
	-0.001641, 0.010150, 0.001235,-0.007192,-0.001170, 0.004778, 0.000892,
	-0.003108,-0.000675, 0.001899, 0.000457,-0.001091,-0.000286, 0.000566,
	0.000155,-0.000252,-0.000068, 0.000081
};

/* Test taps for a CCIR-405 625 line video pre-emphasis filter at 20 MHz (5.0 MHz video) */
const static double fm_625_20_taps[] = {
	0.000067,-0.000020,-0.000229, 0.000057, 0.000527,-0.000124,-0.001021,
	0.000233, 0.001784,-0.000398,-0.002907, 0.000638, 0.004493,-0.000973,
	-0.006673, 0.001431, 0.009611,-0.002045,-0.013528, 0.002862, 0.018751,
	-0.003952,-0.025816, 0.005434, 0.035711,-0.007535,-0.050532, 0.010748,
	0.075704,-0.016401,-0.130908, 0.029571, 0.389478, 0.486518, 0.138360,
	-0.252075,-0.274916,-0.058426, 0.033435,-0.042230,-0.071733,-0.002206,
	0.031042,-0.010388,-0.029749, 0.003031, 0.018854,-0.003889,-0.014654,
	0.002173, 0.010064,-0.001768,-0.007159, 0.001135, 0.004797,-0.000788,
	-0.003145, 0.000492, 0.001943,-0.000301,-0.001129, 0.000167, 0.000594,
	-0.000083,-0.000268, 0.000033, 0.000087
};

/* Test taps for a CCIR-405 625 line video pre-emphasis filter at 14 MHz (5.0 MHz video) */
const static double fm_625_14_taps[] = {
	-0.000061, 0.000075, 0.000079,-0.000341, 0.000453,-0.000087,-0.000729,
	0.001376,-0.000973,-0.000755, 0.002778,-0.003139, 0.000548, 0.003914,
	-0.006739, 0.004403, 0.003136,-0.010915, 0.011699,-0.001972,-0.013324,
	0.022221,-0.014160,-0.009877, 0.034089,-0.036545, 0.006183, 0.043539,
	-0.076299, 0.052523, 0.043572,-0.187540, 0.322030, 0.701289,-0.058668,
	-0.430082,-0.018798,-0.015828,-0.101536, 0.044756,-0.013074,-0.034343,
	0.036609,-0.018493,-0.008939, 0.021852,-0.017259, 0.002207, 0.010009,
	-0.012209, 0.005662, 0.002684,-0.006741, 0.005098,-0.000648,-0.002735,
	0.003160,-0.001385,-0.000608, 0.001415,-0.000978, 0.000113, 0.000413,
	-0.000411, 0.000149, 0.000050,-0.000081
};

/* Test taps for a CCIR-405 525 line video pre-emphasis filter at 20.25 Mhz (4.5 MHz video) */
const static double fm_525_2025_taps[] = {
	0.000066, 0.000083,-0.000185,-0.000322, 0.000258, 0.000791,-0.000129,
	-0.001507,-0.000457, 0.002328, 0.001763,-0.002922,-0.003974, 0.002734,
	0.007047,-0.001051,-0.010580,-0.002869, 0.013709, 0.009610,-0.015075,
	-0.019401, 0.012846, 0.031971,-0.004675,-0.046537,-0.012759, 0.061992,
	0.045974,-0.077508,-0.116839, 0.094822, 0.420487, 0.519288, 0.253553,
	-0.127533,-0.283639,-0.168650,-0.016743,-0.005539,-0.073832,-0.082367,
	-0.022195, 0.013864,-0.009368,-0.035356,-0.020826, 0.006427, 0.007010,
	-0.010098,-0.013664,-0.001182, 0.005904, 0.000028,-0.005857,-0.003080,
	0.001892, 0.001679,-0.001316,-0.001809,-0.000049, 0.000738, 0.000022,
	-0.000494,-0.000223, 0.000074, 0.000033,-0.000037,-0.000028,-0.000021,
	-0.000016
};

/* Test taps for a CCIR-405 525 line video pre-emphasis filter at 18 MHz (4.5 MHz video) */
const static double fm_525_18_taps[] = {
	0.000075,-0.000015,-0.000256, 0.000041, 0.000584,-0.000089,-0.001129,
	0.000166, 0.001970,-0.000282,-0.003205, 0.000450, 0.004949,-0.000685,
	-0.007345, 0.001005, 0.010572,-0.001433,-0.014873, 0.002003, 0.020609,
	-0.002764,-0.028371, 0.003800, 0.039250,-0.005274,-0.055571, 0.007540,
	0.083358,-0.011566,-0.144562, 0.021098, 0.433536, 0.585948, 0.232197,
	-0.220703,-0.298368,-0.092016, 0.010141,-0.065412,-0.099798,-0.024986,
	0.018079,-0.020843,-0.041667,-0.005978, 0.014739,-0.007081,-0.019026,
	-0.001051, 0.009059,-0.002545,-0.008713,-0.000001, 0.004617,-0.000929,
	-0.003681, 0.000100, 0.001922,-0.000317,-0.001297, 0.000042, 0.000585,
	-0.000089,-0.000310, 0.000001, 0.000076
};

/* Test taps for D/D2-MAC pre-emphasis at 20.25 MHz (9.0 MHz video) */
const static double fm_mac_taps[] = {
	-0.000056, 0.000132,-0.000222, 0.000306,-0.000336, 0.000260,-0.000018,
	-0.000427, 0.001082,-0.001893, 0.002744,-0.003450, 0.003776,-0.003467,
	0.002302,-0.000147,-0.002980, 0.006866,-0.011076, 0.014960,-0.017703,
	0.018408,-0.016215, 0.010429,-0.000656,-0.013099, 0.030363,-0.050197,
	0.071259,-0.091921, 0.110449,-0.125196, 0.134802, 0.995046,-0.042208,
	-0.230210, 0.051938,-0.129414, 0.053138,-0.064551, 0.025541,-0.018979,
	-0.001657, 0.008139,-0.016559, 0.017856,-0.018185, 0.015194,-0.011648,
	0.007290,-0.003425, 0.000144, 0.002097,-0.003389, 0.003776,-0.003515,
	0.002836,-0.001993, 0.001168,-0.000492, 0.000021, 0.000242,-0.000335,
	0.000313,-0.000233, 0.000141,-0.000065
};

/* Taps for 40-15000Hz low pass filter at 32kHz with no, 50us and 75us
 * pre-emphasis. The phase response of these filters is not a good match for
 * a real FM pre-emphasis circuit, but audio quality seems unaffected */
const static double fm_audio_flat_taps[65] = {
	0.000000,-0.000793, 0.000318,-0.001297, 0.000756,-0.002084, 0.001341,
	-0.003091, 0.001926,-0.004059, 0.002173,-0.004543, 0.001586,-0.003982,
	-0.000386,-0.001819,-0.004219, 0.002351,-0.010158, 0.008641,-0.018108,
	0.016785,-0.027575, 0.026122,-0.037697, 0.035663,-0.047356, 0.044249,
	-0.055360, 0.050742,-0.060650, 0.054238, 0.937500, 0.054238,-0.060650,
	0.050742,-0.055360, 0.044249,-0.047356, 0.035663,-0.037697, 0.026122,
	-0.027575, 0.016785,-0.018108, 0.008641,-0.010158, 0.002351,-0.004219,
	-0.001819,-0.000386,-0.003982, 0.001586,-0.004543, 0.002173,-0.004059,
	0.001926,-0.003091, 0.001341,-0.002084, 0.000756,-0.001297, 0.000318,
	-0.000793,-0.000000
};

const static double fm_audio_50us_taps[65] = {
	0.001234,-0.002637, 0.002903,-0.004810, 0.005412,-0.008091, 0.008855,
	-0.012171, 0.012482,-0.015806, 0.014595,-0.016860, 0.012742,-0.012646,
	0.004202,-0.000532,-0.013336, 0.021334,-0.041037, 0.053332,-0.078322,
	0.093873,-0.122521, 0.139174,-0.168825, 0.183024,-0.210266, 0.214647,
	-0.236618, 0.196560,-0.226183,-0.606600, 2.497308,-0.606600,-0.226183,
	0.196560,-0.236618, 0.214647,-0.210266, 0.183024,-0.168825, 0.139174,
	-0.122521, 0.093873,-0.078322, 0.053332,-0.041037, 0.021334,-0.013336,
	-0.000532, 0.004202,-0.012646, 0.012742,-0.016860, 0.014595,-0.015806,
	0.012482,-0.012171, 0.008855,-0.008091, 0.005412,-0.004810, 0.002903,
	-0.002637, 0.001234
};

const static double fm_audio_75us_taps[65] = {
	0.001981,-0.003755, 0.004472,-0.006942, 0.008239,-0.011739, 0.013420,
	-0.017690, 0.018901,-0.022955, 0.022160,-0.024370, 0.019556,-0.017960,
	0.007049, 0.000170,-0.018791, 0.032752,-0.059706, 0.080325,-0.114856,
	0.140480,-0.180353, 0.207455,-0.249292, 0.271550,-0.312119, 0.315065,
	-0.356561, 0.275266,-0.363286,-0.992136, 3.546394,-0.992136,-0.363286,
	0.275266,-0.356561, 0.315065,-0.312119, 0.271550,-0.249292, 0.207455,
	-0.180353, 0.140480,-0.114856, 0.080325,-0.059706, 0.032752,-0.018791,
	0.000170, 0.007049,-0.017960, 0.019556,-0.024370, 0.022160,-0.022955,
	0.018901,-0.017690, 0.013420,-0.011739, 0.008239,-0.006942, 0.004472,
	-0.003755, 0.001981
};

const static double fm_audio_j17_taps[65] = {
	-0.000119,-0.000175,-0.000162,-0.000232,-0.000223,-0.000310,-0.000309,
	-0.000420,-0.000430,-0.000576,-0.000605,-0.000801,-0.000864,-0.001135,
	-0.001253,-0.001644,-0.001860,-0.002446,-0.002844,-0.003776,-0.004531,
	-0.006130,-0.007663,-0.010705,-0.014141,-0.020784,-0.029556,-0.046668,
	-0.072530,-0.124846,-0.211267,-0.400931, 2.279077,-0.400931,-0.211267,
	-0.124846,-0.072530,-0.046668,-0.029556,-0.020784,-0.014141,-0.010705,
	-0.007663,-0.006130,-0.004531,-0.003776,-0.002844,-0.002446,-0.001860,
	-0.001644,-0.001253,-0.001135,-0.000864,-0.000801,-0.000605,-0.000576,
	-0.000430,-0.000420,-0.000309,-0.000310,-0.000223,-0.000232,-0.000162,
	-0.000175,-0.000119
};
