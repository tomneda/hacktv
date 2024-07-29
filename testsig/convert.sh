#!/bin/bash
ffmpeg -loop 1 -i image.png -c:v libx264 -t 5 -pix_fmt yuv420p image.mp4

# not working (adding several images to one output video)
# ffmpeg -f concat -i input.txt -vsync vfr -pix_fmt yuv420p output.mp4

