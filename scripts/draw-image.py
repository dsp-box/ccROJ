#! /usr/bin/python3 -B

# *************************************************** *
# This file is a part of ccROJ project                *
# distributed under GNU General Public License v3.0.  *
# Please visit the webpage: github.com/dsp-box/ccROJ  *
# for more information.                               *
#                       contact: Krzysztof Czarnecki  *
#                email: czarnecki.krzysiek@gmail.com  *
# *************************************************** */

# *************************************************** */
# import

from subprocess import Popen, PIPE
from linecache import getline
from getopt import getopt

import re, sys

# *************************************************** */
# help

def print_help(keys):
    print("image-plot.py\n\nhas possible the following options:")
    for k in keys:
        print("--", "\b"+re.sub("=", " arg", k))
    sys.exit(0)

# *************************************************** */
# init guplot

def init_gnuplot():
    proc = Popen(["gnuplot","--persist"], shell=True, stdin=PIPE)
    def write_to_gnuplot(command):
        print("(debug)", command)
        commandn = command + "\n"
        proc.stdin.write(commandn.encode("UTF-8"))
    return write_to_gnuplot

write_to_gnuplot = init_gnuplot()

# *************************************************** */
# options

# keys list contain possible options
keys = []

# file with data to plot which should have the following format:
# #X_MIN=xmin
# #X_MAX=xmax
# #Y_MIN=ymin
# #Y_MAX=ymax
# x0 y0 z00
# x0 y1 z01
# ...
# x0 y9 z09
#
# x1 y0 z10
# x1 y1 z11
# ...
# x1 y9 z19
# ...
# #Z_MIN=zmin
# #Z_MAX=zmax
keys.append("infile=")

# name of output file (extension can be skiped)
keys.append("outfile=")

# file extension. 2 are possible: png and eps
keys.append("format=")

# minimal and maximal values
keys.extend(["min=", "max="])

# switch on logarithmic mode
keys.append("log")

# labels
keys.extend(["xlabel=", "ylabel=", "zlabel="])

# factors
keys.extend(["xfactor=", "yfactor=", "zfactor="])

# help
keys.append("help")

# *************************************************** */
# parse command line

arg_format = "png"
arg_infile = None
arg_outfile = None
arg_min = None
arg_max = None
arg_log = False

arg_xlabel = "X"
arg_ylabel = "Y"
arg_zlabel = "Z"

arg_xfactor = 1
arg_yfactor = 1
arg_zfactor = 1

opts, args = getopt(sys.argv[1:], "", keys)
for opt,arg in opts:
    if opt == "--format": arg_format = arg
    if opt == "--infile": arg_infile = arg
    if opt == "--outfile": arg_outfile = arg
    if opt == "--min": arg_min = float(arg)
    if opt == "--max": arg_max = float(arg)
    if opt == "--log": arg_log = True
    if opt == "--help": print_help(keys)

    if opt == "--xlabel": arg_xlabel = arg
    if opt == "--ylabel": arg_ylabel = arg
    if opt == "--zlabel": arg_zlabel = arg

    if opt == "--xfactor": arg_xfactor = float(arg)
    if opt == "--yfactor": arg_yfactor = float(arg)
    if opt == "--zfactor": arg_zfactor = float(arg)

# *************************************************** */
# check command line

write_to_gnuplot("set fontpath '/usr/share/matplotlib/mpl-data/fonts/ttf/cmr10.ttf'")
if arg_format == "png": write_to_gnuplot("set term pngcairo dashed size 1600,900 font 'cmr10, 32'")
elif arg_format == "eps": write_to_gnuplot("set term postscript eps enhanced color font 'cmr10, 14' size 9.5cm,7.0cm")
else: assert False

assert arg_infile != None
if arg_outfile == None:
    arg_outfile = arg_infile

if arg_outfile[-4:] == "."+arg_format:
    write_to_gnuplot("set output '%s'" % arg_outfile)
else:
    write_to_gnuplot("set output '%s.%s'" % (arg_outfile, arg_format))

assert arg_min != None
if arg_log: arg_max=0
assert arg_min < arg_max

# *************************************************** */
# set margins

def set_margins(l,r,b,t):
    write_to_gnuplot("set lmargin at screen %g" % l)
    write_to_gnuplot("set rmargin at screen %g" % r)
    write_to_gnuplot("set bmargin at screen %g" % b)
    write_to_gnuplot("set tmargin at screen %g" % t)

set_margins(0.15, 0.8, 0.16, 0.955)

# *************************************************** */
# const settings

write_to_gnuplot("set pm3d map")
write_to_gnuplot("unset grid")

# *************************************************** */
# ranges

line = getline(arg_infile, 1)
xmin = float(re.findall("(?<=X_MIN=).*", line)[0])
line = getline(arg_infile, 2)
xmax = float(re.findall("(?<=X_MAX=).*", line)[0])
write_to_gnuplot("set xrange [%g : %g]" % (xmin*arg_xfactor, xmax*arg_xfactor))

line = getline(arg_infile, 3)
ymin = float(re.findall("(?<=Y_MIN=).*", line)[0])
line = getline(arg_infile, 4)
ymax = float(re.findall("(?<=Y_MAX=).*", line)[0])
write_to_gnuplot("set yrange [%g : %g]" % (ymin*arg_yfactor, ymax*arg_yfactor))

write_to_gnuplot("set cbrange [%g : %g]" % (arg_min*arg_zfactor, arg_max*arg_zfactor))
if arg_log:
    ln = sum(1 for line in open(arg_infile))
    line = getline(arg_infile, ln)
    log_max = float(re.findall("(?<=Z_MAX=).*", line)[0])

# *************************************************** */
# palette

roj_palette = """\
0.0  1.0  1.0  1.0, \
0.5  0.9  0.0  0.0, \
2.5  1.0  0.2  0.0, \
3.3  0.9  0.8  0.0, \
3.9  0.3  1.0  0.0, \
4.0  0.0  1.0  0.0, \
4.15  0.0  1.0  0.4, \
4.8  0.0  0.5  1.0, \
5.5  0.0  0.2  1.0, \
7.5  0.0  0.1  0.8, \
8.0  1.0  1.0  1.0"""

if arg_log:
    write_to_gnuplot("set palette rgbformulae 30,31,32 negative")

else:
    write_to_gnuplot("set palette defined(%s) negative" % roj_palette)

# *************************************************** */
# labels

write_to_gnuplot("set xlabel '%s'" % arg_xlabel)
write_to_gnuplot("set ylabel '%s' offset -1,0" % arg_ylabel)
write_to_gnuplot("set cblabel '%s' offset 1,0" % arg_zlabel)

# *************************************************** */
# ploting

if arg_log: 
    write_to_gnuplot("splot '%s' u (%g*$1):(%g*$2):(10*log10($3/%g)) notitle" % (arg_infile, arg_xfactor, arg_yfactor, log_max))       
else: 
    write_to_gnuplot("splot '%s' u (%g*$1):(%g*$2):(%g*$3) notitle" % (arg_infile, arg_xfactor, arg_yfactor, arg_zfactor))
write_to_gnuplot("print 'done!'")       
print("(info) wait a moment for results. It can takes few minutes...")
