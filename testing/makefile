# This makefile is a part of the ccROJ project.
# See README for more informations.

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
	test-cct-analyzer 

all: $(TESTS) 

DIR := $(notdir $(PWD))
ifeq ($(DIR), testing)	
check_main_dir:
	$(error error: compile only in main project directory)
else
check_main_dir:
endif

CCTFR_DIR := ..
OBJS := $(CCTFR_DIR)/*.o

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
case-1:
	./test-complex-signal
	echo "plot 'ifreq.txt' u 1:3 wit lines;  pause 3" | gnuplot

case-2:
	./test-ode-analyzer
	../scripts/plot-tf-image.gp data-c-rate.txt 200 1200 "chirp-rate (Hz/s)"
	../scripts/plot-tf-energy.gp data-s-energy.txt -60
	../scripts/plot-tf-energy.gp data-r-energy.txt -60

case-3:
	./test-hilbert-equiv
	../scripts/plot-tf-energy.gp data-s-energy.txt -40
	../scripts/plot-tf-energy.gp data-r-energy.txt -50

case-4:
	./test-fourier-spectr
	mplayer pink.wav # please check a level of your speaker

case-5:
	./test-fft-analyzer
	../scripts/plot-tf-energy.gp data-s-energy.txt -40
	../scripts/plot-tf-energy.gp data-r-energy.txt -50
	mplayer out.wav	 # please check a level of your speaker

case-6:
	./test-cct-analyzer
	../scripts/plot-tf-energy.gp data-s-energy-1.txt -50
	../scripts/plot-tf-energy.gp data-s-energy-2.txt -50
	../scripts/plot-tf-energy.gp data-s-energy-3.txt -50

clean:	
	${RM} $(TESTS) *~ \#* .\#*
clear:
	${RM} *.png *.txt mail2.wav pink.wav out.wav

purge: clean clear

