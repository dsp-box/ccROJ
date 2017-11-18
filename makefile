# *************************************************** *
# This file is a part of ccROJ project                *
# distributed under GNU General Public License v3.0.  *
# Please visit the webpage: github.com/dsp-box/ccROJ  *
# for more information.                               *
#                       contact: Krzysztof Czarnecki  *
#                email: czarnecki.krzysiek@gmail.com  *
# *************************************************** */

CC := g++
CP := cp

ZIP := zip

LIBS := -lm -lsndfile -lfftw3 -ansi
FLAGS := -pedantic -w -Wall -O2 # -g
CCFLAGS := $(FLAGS) $(LIBS)

all: libroj.a test

SRCS=$(wildcard *.cc)
HTMLS=$(SRCS:.cc=.html)
OBJS=$(SRCS:.cc=.o)
print_obj:
	$(info $(OBJS))

%.o: %.cc %.hh
	$(CC) $(CCFLAGS) -c -o $@ $<

test: $(OBJS)
	$(MAKE) -w -C testing

libroj.a: $(OBJS)
	ar rcs $@ $^

INSTALL_DIR=/usr/local
INSTALL_BINDIR=$(INSTALL_DIR)/bin
INSTALL_LIBDIR=$(INSTALL_DIR)/lib
INSTALL_INCDIR=$(INSTALL_DIR)/include/roj
install:
	su root -c "make install-as-root"
install-as-root:
	mkdir -p $(INSTALL_INCDIR)
	${CP} *.hh *.h $(INSTALL_INCDIR)
	${CP} libroj.a $(INSTALL_LIBDIR)
	${CP} scripts/roj-draw.py $(INSTALL_BINDIR)

uninstall:
	su root -c "make uninstall-as-root"
uninstall-as-root:
	${RM} -r $(INSTALL_INCDIR)
	${RM} $(INSTALL_LIBDIR)/libroj.a
	${RM} $(INSTALL_BINDIR)/roj-draw.py

VERSION := $(shell cat VERSION)
zip: package
package:
	mkdir ccROJ-$(VERSION)
	mkdir ccROJ-$(VERSION)/manual
	mkdir ccROJ-$(VERSION)/testing
	mkdir ccROJ-$(VERSION)/scripts
	${CP} *.cc *hh makefile ccROJ-$(VERSION)
	${CP} README AUTHORS LICENSE VERSION MANIFEST ccROJ-$(VERSION)
	${CP} INSTALL DEPENDENCIES CONTRIBUTING ccROJ-$(VERSION)
	${CP} testing/makefile ccROJ-$(VERSION)/testing
	${CP} testing/*.wav ccROJ-$(VERSION)/testing
	${CP} testing/*.cc ccROJ-$(VERSION)/testing
	${CP} scripts/*.py ccROJ-$(VERSION)/scripts
	${CP} manual/* ccROJ-$(VERSION)/manual
	zip -mr ccROJ-$(VERSION).zip ccROJ-$(VERSION)

DOC_GENER := ./scripts/doc-gener.py
%.html: %.hh %.cc 
	$(DOC_GENER) $? > manual/$@ 

doc: $(HTMLS)
	${CP} scripts/draw-image.py manual
	${CP} MANIFEST manual/manifest.txt
	${CP} README manual/readme.txt

# this is private server (temporary):
public: www
www:
	scp manual/* git.eti.pg.gda.pl:/var/www/html/ccROJ/

REGEXP_1 := "s/define ROJ_VERSION \"[0-9]-[0-9]\+\"/define ROJ_VERSION \"$(shell cat VERSION)\"/g"
REGEXP_2 := "s@(version [0-9]-[0-9][0-9]\+)@(version $(shell cat VERSION))@g"
version: _version doc  
_version: 
	sed -i $(REGEXP_1) roj-misc.hh
	sed -i $(REGEXP_2) *.hh *.cc testing/*.cc
	sed -i $(REGEXP_2) manual/index.html
	sed -i $(REGEXP_2) manual/head.js

tag:
	git tag -a $(VERSION) -m $(VERSION)

lines:
	wc -l *.cc *.hh makefile | tail -n1
	wc -l *.cc *.hh makefile testing/*.cc testing/makefile scripts/*.py | tail -n1

clean:	
	${RM} ${OBJS} libroj.a $(TESTS) *~ \#* .\#* *.o manual/*~
	$(MAKE) -w -C testing clean

clear:
	${RM} *.png *.txt *.zip
	$(MAKE) -w -C testing clear

purge: clean clear

