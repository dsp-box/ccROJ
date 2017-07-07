# This makefile is a part of the ccROJ project.
# See README for more informations.

CC := g++

LIBS := -lm -lsndfile -lfftw3 -ansi
FLAGS := -pedantic -w -Wall -O2 # -g
CCFLAGS := $(FLAGS) $(LIBS)

SRCS=$(wildcard *.cc)
HTMLS=$(SRCS:.cc=.html)
OBJS=$(SRCS:.cc=.o)

all: $(OBJS) test

%.o: %.cc %.hh
	$(CC) $(CCFLAGS) -c -o $@ $<

.PHONY: test *.o
test: $(OBJS)
	$(MAKE) -w -C testing

VERSION := $(shell cat VERSION)
zip: package
package:
	mkdir ccROJ-$(VERSION)
	mkdir ccROJ-$(VERSION)/manual
	mkdir ccROJ-$(VERSION)/testing
	mkdir ccROJ-$(VERSION)/scripts
	cp *.cc *hh *.py makefile ccROJ-$(VERSION)
	cp README AUTHORS LICENSE VERSION MANIFEST ccROJ-$(VERSION)
	cp testing/makefile ccROJ-$(VERSION)/testing
	cp testing/*.wav ccROJ-$(VERSION)/testing
	cp testing/*.cc ccROJ-$(VERSION)/testing
	cp scripts/*.gp ccROJ-$(VERSION)/scripts
	cp manual/* ccROJ-$(VERSION)/manual
	zip -mr ccROJ-$(VERSION).zip ccROJ-$(VERSION)

DOC_GENER := ./doc-gener.py
%.html: %.hh %.cc 
	$(DOC_GENER) $? > manual/$@ 

doc: $(HTMLS)
	cp scripts/plot-tf-energy.gp manual
	cp scripts/plot-tf-image.gp manual
	cp MANIFEST manual/manifest.txt
	cp README manual/readme.txt

public: www
www:
	scp manual/* git.eti.pg.gda.pl:/var/www/html/ccROJ/

REGEXP_1 := "s/define ROJ_VERSION \"[0-9]-[0-9][0-9]\"/define ROJ_VERSION \"$(shell cat VERSION)\"/g"
REGEXP_2 := "s@(version [0-9]-[0-9][0-9])@(version $(shell cat VERSION))@g"
version: _version doc  
_version: 
	sed -i $(REGEXP_1) roj-misc.hh
	sed -i $(REGEXP_2) manual/index.html
	sed -i $(REGEXP_2) manual/head.html

tag: commit
	git tag -a $(VERSION) -m $(VERSION)

commit:	
	git commit -a -m "+"
	git push

lines:
	wc -l *.py *.cc *.hh makefile testing/*.cc testing/makefile | tail -n1
	wc -l *.cc *.hh makefile | tail -n1

clean:	
	${RM} ${OBJS} $(TESTS) *~ \#* .\#* *.o manual/*~
	$(MAKE) -w -C testing clean

purge: clean
	${RM} *.png *.txt *.zip

