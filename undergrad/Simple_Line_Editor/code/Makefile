# Makefile

CFLAGS = gcc -ansi -pedantic -Wall
OBS = dlist.o interface.o user.o main.o

Line_Editor: $(OBS)
	gcc $(OBS) -o Line_Editor

main.o: globals.h dlist.h interface.h user.h main.c
	$(CFLAGS) -c main.c

user.o: globals.h dlist.h interface.h user.c
	$(CFLAGS) -c user.c

interface.o: globals.h dlist.h interface.c
	$(CFLAGS) -c interface.c

dlist.o: globals.h dlist.c
	$(CFLAGS) -c dlist.c

clean:
	rm -f *~
	rm -f *.o
	rm -f Line_Editor 
