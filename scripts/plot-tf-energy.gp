#!/usr/bin/gnuplot -c

# This is a suplementary gnuplot script.
# It is an element of the ccROJ project.
# Please see README for more information
# about the ccROJ project.

# usage: ./plot-ft-energy.gp filename.txt -40

# LaTeX font:
set fontpath "/usr/share/matplotlib/mpl-data/fonts/ttf/cmr10.ttf"
set term pngcairo dashed size 1600,900 font "cmr10, 32"
set output ARG1.".png"

# this section is for eps format:
# set fontpath "/usr/share/matplotlib/mpl-data/fonts/ttf/cmr10.tt"
# set term postscript eps enhanced color font "cmr10, 14" size 9.5cm,7.0cm
# set output ARG1.".eps"

# you can convert it to png:
# convert -density 300 img-energy.eps -resize 800x600  -flatten -colorspace RGB img-energy.png

# ##########################################################################
unset grid
set pm3d map
set palette rgbformulae 30,31,32 negative

set lmargin at screen 0.15
set rmargin at screen 0.8
set bmargin at screen 0.16
set tmargin at screen 0.955

# ##########################################################################

TIME_MIN = system("head ".ARG1." | grep TIME_MIN | cut -f2 -d=")
TIME_MAX = system("head ".ARG1." | grep TIME_MAX | cut -f2 -d=")
FREQ_MIN = system("head ".ARG1." | grep FREQ_MIN | cut -f2 -d=")
FREQ_MAX = system("head ".ARG1." | grep FREQ_MAX | cut -f2 -d=")
VAL_MAX = system("tail ".ARG1." | grep VAL_MAX | cut -f2 -d=")

set yrange[FREQ_MIN/1000:FREQ_MAX/1000]
set xrange[TIME_MIN:TIME_MAX]
set cbrange[ARG2:0]

# set y axis to the logarithmic scale:
# set log y

set xlabel "time (s)" offset 0,0.4
set ylabel "frequency (kHz)" offset 1.5,0

##########################################################################

set cblabel "energy (dBc)" offset 0.9,0
splot ARG1 u 1:($2/1000):(10*log10($3/VAL_MAX)) notitle
print "energy ... done!"

##########################################################################
