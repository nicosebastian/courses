/*
 * globals.h
 */

#ifndef _globals
#define _globals

typedef enum { directed, undirected } graph_type ;
typedef enum { DEPTH_FIRST, BREADTH_FIRST, TOPOLOGICAL } searchorder ;
typedef int vertex ;

typedef struct {
  int weight ;
  vertex vertex_number ;
} edge ;

typedef struct {
  graph_type type ;
  int number_of_vertices ;
  edge **matrix ;
} graph_header, *graph ;

#define WEIGHT( e ) (( e ) -> weight )
#define VERTEX( e ) (( e ) -> vertex_number )
#define UNUSED_WEIGHT ( 32767 )

typedef enum { OK, ERROR } status ;
typedef enum { FALSE=0 , TRUE=1 } bool ;
typedef char *generic_ptr ;

#endif
