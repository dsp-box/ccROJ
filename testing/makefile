# *************************************************** *
# This file is a part of ccROJ project                *
# distributed under GNU General Public License v3.0.  *
# Please visit the webpage: github.com/dsp-box/ccROJ  *
# for more information.                               *
#                       contact: Krzysztof Czarnecki  *
#                email: czarnecki.krzysiek@gmail.com  *
# *************************************************** */

CC := g++

LIBS := -lm -lsndfile -lfftw3 -ansi
FLAGS := -pedantic -w -Wall -O2 # -g 
CXXFLAGS := $(FLAGS) $(LIBS)

TESTS := \
	test-complex-signal \
	test-signal-recovery \
	test-fourier-spectr \
	test-hilbert-equiv \
	test-fft-analyzer \
	test-ode-analyzer \
	test-cct-analyzer \
	test-lfm-chirps 


all: $(TESTS) 

DIR := $(notdir $(PWD))
ifeq ($(DIR), testing)	
check_main_dir:
	$(error error: compile only in main project directory)
else
check_main_dir:
endif

CCTFR_DIR := ..
OBJS := $(CCTFR_DIR)/libroj.a

test-signal-recovery: check_main_dir test-signal-recovery.cc $(OBJS)
	$(CC) $@.cc $(OBJS) $(CXXFLAGS) -o $@
test-complex-signal: check_main_dir test-complex-signal.cc $(OBJS)
	$(CC) $@.cc $(OBJS) $(CXXFLAGS) -o $@
test-fourier-spectr: check_main_dir test-fourier-spectr.cc $(OBJS)
	$(CC) $@.cc $(OBJS) $(CXXFLAGS) -o $@
test-hilbert-equiv: check_main_dir test-hilbert-equiv.cc $(OBJS)
	$(CC) $@.cc $(OBJS) $(CXXFLAGS) -o $@
test-fft-analyzer: check_main_dir test-fft-analyzer.cc $(OBJS)
	$(CC) $@.cc $(OBJS) $(CXXFLAGS) -o $@
test-ode-analyzer: check_main_dir test-ode-analyzer.cc $(OBJS)
	$(CC) $@.cc $(OBJS) $(CXXFLAGS) -o $@
test-cct-analyzer: check_main_dir test-cct-analyzer.cc $(OBJS)
	$(CC) $@.cc $(OBJS) $(CXXFLAGS) -o $@
test-lfm-chirps: check_main_dir test-lfm-chirps.cc $(OBJS)
	$(CC) $@.cc $(OBJS) $(CXXFLAGS) -o $@

run: test
test: all
	./test-hilbert-equiv
	./test-signal-recovery
	./test-complex-signal
	./test-fourier-spectr
	./test-fft-analyzer
	./test-ode-analyzer
	./test-cct-analyzer

# user cases:
# please be sure that Gnuplot, Python and Mlayer is installed on your computer

IMAGE_PLOTER := ../scripts/draw-image.py

case-0:
	./test-complex-signal
	echo "plot 'ifreq.txt' u 1:3 wit lines;  pause 3" | gnuplot

case-1:
	./test-ode-analyzer
	$(IMAGE_PLOTER) \
		--infile data-c-rate.txt \
		--zlabel "chirp-rate (Hz/s)" \
		--ylabel "frequency (kHz)" \
		--xlabel "time (s)" \
		--yfactor 0.001 \
	        --min 200 --max 1200
	$(IMAGE_PLOTER) \
		--infile data-s-energy.txt \
		--ylabel "frequency (kHz)" \
		--zlabel "energy (dBc)" \
		--xlabel "time (s)" \
		--yfactor 0.001 \
		--min -60 --log
	$(IMAGE_PLOTER) \
		--infile data-r-energy.txt \
		--ylabel "frequency (kHz)" \
		--zlabel "energy (dBc)" \
		--xlabel "time (s)" \
		--yfactor 0.001 \
		--min -70 --log
	echo "plot 'profile.txt' u 1:2 wit lines;  pause 3" | gnuplot

case-2:
	./test-hilbert-equiv
	$(IMAGE_PLOTER) \
		--infile data-s-energy.txt \
		--ylabel "frequency (kHz)" \
		--zlabel "energy (dBc)" \
		--xlabel "time (s)" \
		--yfactor 0.001 \
		--min -40 --log
	$(IMAGE_PLOTER) \
		--infile data-r-energy.txt \
		--ylabel "frequency (kHz)" \
		--zlabel "energy (dBc)" \
		--xlabel "time (s)" \
		--yfactor 0.001 \
		--min -50 --log

case-3:
	./test-fourier-spectr
	mplayer pink.wav # please check a level of your speaker

case-4:
	./test-fft-analyzer
	$(IMAGE_PLOTER) \
		--infile data-s-energy.txt \
		--ylabel "frequency (kHz)" \
		--zlabel "energy (dBc)" \
		--xlabel "time (s)" \
		--yfactor 0.001 \
		--min -40 --log
	$(IMAGE_PLOTER) \
		--infile data-r-energy.txt \
		--ylabel "frequency (kHz)" \
		--zlabel "energy (dBc)" \
		--xlabel "time (s)" \
		--yfactor 0.001 \
		--min -50 --log
	mplayer out.wav	 # please check a level of your speaker

case-5:
	./test-cct-analyzer
	$(IMAGE_PLOTER) \
		--infile data-s-energy-1.txt \
		--ylabel "frequency (kHz)" \
		--zlabel "energy (dBc)" \
		--xlabel "time (s)" \
		--yfactor 0.001 \
		--min -50 --log
	$(IMAGE_PLOTER) \
		--infile data-s-energy-2.txt \
		--ylabel "frequency (kHz)" \
		--zlabel "energy (dBc)" \
		--xlabel "time (s)" \
		--yfactor 0.001 \
		--min -50 --log
	$(IMAGE_PLOTER) \
		--infile data-s-energy-3.txt \
		--ylabel "frequency (kHz)" \
		--zlabel "energy (dBc)" \
		--xlabel "time (s)" \
		--yfactor 0.001 \
		--min -50 --log

case-6:
	./test-lfm-chirps
	$(IMAGE_PLOTER) \
		--infile data-s-energy.txt \
		--ylabel "frequency (kHz)" \
		--zlabel "energy (dBc)" \
		--xlabel "time (s)" \
		--yfactor 0.001 \
		--min -40 --log

	$(IMAGE_PLOTER) \
		--infile data-c-rate.txt \
		--zlabel "chirp-rate (kHz/s)" \
		--ylabel "frequency (kHz)" \
		--xlabel "time (s)" \
		--yfactor 0.001 \
		--zfactor 0.001 \
	        --min -1800 --max 1800 \
		--xmin 0 --xmax 1 \
		--format png
	mplayer lfm.wav	 # please check a level of your speaker

clean:	
	${RM} $(TESTS) *~ \#* .\#*
clear:
	${RM} *.png *.eps *.txt mail2.wav pink.wav out.wav # lfm.wav

purge: clean clear

