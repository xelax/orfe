# makefile per orfe v1.0 by Alex Cozzi 4/9/1990
# versione per amigados, ma non dovrebbe essere difficile da adattare per 
# altri computer
LOPTS = SC SD ND

read:	ansi.o read.o
	blink lib:c.o read.o TO read LIB lib:lc.lib ansi.o $(LOPTS)

ins:  ins.o read.o 
 	blink lib:c.o read.o ins.o TO ins LIB lib:lcm.lib lib:lc.lib ansi.o $(LOPTS)

orfe:  read.o main.o pollicin.o finale.o graphic.o
 	blink lib:c.o main.o pollicin.o read.o finale.o TO orfe LIB lib:lcm.lib lib:lc.lib ansi.o graphic.o $(LOPTS)

ansi.o:	ansi.c
	lc ansi.c

read.o:	globals.h read.c
	lc -b0 read.c

pollicino.o: globals.h pollicin.c
	lc -b0 pollicin.c

finale.o: globals.h finale.c
	lc -b0 finale.c

main.o: globals.h main.c
	lc -b0 main.c

ins.o: ins.c globals.h
	lc -b0 ins.c

graphic.o: graphic.c
	lc graphic.c
