CFLAGS?=-O2
MPICC?=mpicc
MPICPP?=$(MPICC) 
MPICPPFLAGS?=-E

# flags for icc:
#
# -Wall -wd981,810,279
#
# disabled icc warnings:
#
# remark #981: operands are evaluated in unspecified order
# remark #810: conversion from "double" to "int" may lose significant bits
# remark #279: controlling expression is constant

INCLUDEFILES=accounting.h autorefine.h compile.h debug.h exec.h main.h measurement.h mem.h misc.h mpiversiontest.h output.h output_sort.h print.h private_misc.h symboltable.h synchronize.h version.h
CFILES=accounting.c autorefine.c compile.c debug.c exec.c main.c measurement.c mem.c misc.c output.c print.c symboltable.c synchronize.c
SRCFILES=$(CFILES) $(INCLUDEFILES) lexer.lex parser.y
GENERATEDFILES=m.c parser.c parser.h lexer.c
OBJECTS=$(CFILES:.c=.o) m.o lexer.o parser.o
MEASUREMENTS=$(shell ls measurements/*.c)
MEASUREMENTOBJECTS=$(shell cd measurements > /dev/null; ls *.c)
MEASUREMENTOBJECTS:=$(MEASUREMENTOBJECTS:.c=.o)


skampi: Makefile.dep $(MEASUREMENTS)
	make -f Makefile.dep MPICC="$(MPICC)" CFLAGS="$(CFLAGS)" skampi

Makefile.dep: makedep.sh $(SRCFILES) $(GENERATEDFILES) $(MEASUREMENTS)
	rm -f Makefile.dep
	CPP="$(MPICPP)" CPPFLAGS="$(MPICPPFLAGS)" OBJECTS="$(OBJECTS) $(MEASUREMENTOBJECTS)" ./makedep.sh $(CFILES) $(MEASUREMENTS) m.c parser.c lexer.c

clean:
	rm -f $(OBJECTS) $(MEASUREMENTOBJECTS) skampi

mrpropper: clean
	rm -f $(GENERATEDFILES) Makefile.dep

m.c: $(MEASUREMENTS) extract.pl
	./extract.pl --mpicc="$(MPICC)" --cflags="$(CFLAGS) -E" --dir=measurements > m.c

lexer.c: lexer.lex
	flex -olexer.c lexer.lex

parser.c parser.h: parser.y lexer.lex
	bison -d parser.y -o parser.c

# end of distributed part
