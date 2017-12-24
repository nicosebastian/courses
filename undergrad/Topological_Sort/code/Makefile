#Makefile

CFLAGS = gcc -ansi -pedantic -Wall
OBS = main.o graph.o

Topological_Sort: $(OBS)
	$(CFLAGS) $(OBS) -o TopologicalSort

main.o: globals.h graph.h main.c
	$(CFLAGS) -c main.c

graph.o: globals.h graph.c
	$(CFLAGS) -c graph.c

clean:
	rm -f *.o
	rm -f *~
	rm -f Toposorus_Rex	
