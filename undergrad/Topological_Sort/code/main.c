/*
 * main.c
 */

#include <stdio.h>

#include "graph.h"

status write_vertex( int a ) {

  printf( " %d  ", a ) ;

  return OK ;
}

int main ( int argc, char *argv[] ) {

  int weight, from, to, numberofvertices ;
  graph G ;
  FILE *fileptr = fopen( argv[1], "r") ;

  fscanf( fileptr, "%d", &numberofvertices ) ;

  init_graph( &G, numberofvertices, directed ) ;

  while( fscanf( fileptr, "%d %d %d", &from, &to, &weight ) != EOF )
    add_edge( G, from, to, weight ) ;

  printf( "\n Topological Traversal: " ) ;

  traverse_graph( G, TOPOLOGICAL, write_vertex ) ;

  printf("\n\n") ;
  fclose( fileptr ) ;
  return 0 ;
}
