# *************************************************** */
#                       contact: Krzysztof Czarnecki  *
#                email: czarnecki.krzysiek@gmail.com  *
# *************************************************** */

CC := g++

LIBS := -lm -lsndfile -lfftw3 -ansi -lroj
FLAGS := -pedantic -w -Wall -O2
CXXFLAGS := $(FLAGS) $(LIBS)

all: exe-argand exe-icf exe-ostd

exe-argand: exe-argand.cc
	$(CC) $@.cc $(CXXFLAGS) -o $@
exe-icf: exe-icf.cc
	$(CC) $@.cc $(CXXFLAGS) -o $@
exe-ostd: exe-ostd.cc
	$(CC) $@.cc $(CXXFLAGS) -o $@


clean:
	$(RM) exe-argand exe-icf exe-ostd *~ _* sig.txt
