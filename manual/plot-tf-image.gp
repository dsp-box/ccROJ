#!/usr/bin/gnuplot -c

# *************************************************** *
# This file is a part of ccROJ project                *
# distributed under GNU General Public License v3.0.  *
# Please visit the webpage: github.com/dsp-box/ccROJ  *
# for more information.                               *
#                       contact: Krzysztof Czarnecki  *
#                email: czarnecki.krzysiek@gmail.com  *
# *************************************************** */

# usage: ./plot-ft-energy.gp filename.txt -100 100 "chirp rate (Hz/s)" 

# LaTeX font:
set fontpath "/usr/share/matplotlib/mpl-data/fonts/ttf/cmr10.ttf"
set term pngcairo dashed size 1600,900 font "cmr10, 32"
set output ARG1.".png"

# ##########################################################################

unset grid
set pm3d map

set lmargin at screen 0.15
set rmargin at screen 0.8
set bmargin at screen 0.16
set tmargin at screen 0.955

# ##########################################################################

TIME_MIN = system("head ".ARG1." | grep TIME_MIN | cut -f2 -d=")
TIME_MAX = system("head ".ARG1." | grep TIME_MAX | cut -f2 -d=")
FREQ_MIN = system("head ".ARG1." | grep FREQ_MIN | cut -f2 -d=")
FREQ_MAX = system("head ".ARG1." | grep FREQ_MAX | cut -f2 -d=")

set yrange[FREQ_MIN/1000:FREQ_MAX/1000]
set xrange[TIME_MIN:TIME_MAX]
set cbrange[ARG2:ARG3]

set ylabel "frequency (kHz)" offset -0.5,0
set xlabel "time (s)" offset 0,0.4
set cblabel ARG4 offset 0.9,0

# ##########################################################################

#set palette rgb 21,22,23
set palette defined(    \
    0.0  1.0  1.0  1.0, \
    0.5  0.9  0.0  0.0, \
    2.5  1.0  0.2  0.0, \
    3.3  0.9  0.8  0.0, \
    3.9  0.3  1.0  0.0, \
    4.0  0.0  1.0  0.0, \
    4.1  0.0  1.0  0.4, \
    4.7  0.0  0.5  1.0, \
    5.5  0.0  0.2  1.0, \
    7.5  0.0  0.1  0.8, \
    8.0  1.0  1.0  1.0  \
) negative

# ##########################################################################

splot ARG1 u 1:($2/1000):3  notitle
print "image ... done!"

# ##########################################################################

