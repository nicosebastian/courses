/*
 * graph.h
 */

#ifndef _graph
#define _graph

#include "globals.h"

extern status topological_sort( graph G, int vertex_cnt, status ( *p_func_f )()) ;
extern status traverse_graph ( graph G, searchorder order, status (*p_func_f)() ) ;
extern status init_graph ( graph *p_G, int vertex_cnt, graph_type type ) ;
extern void destroy_graph ( graph *p_G );
extern status add_edge ( graph G, vertex vertex1, vertex vertex2, int weight );
extern status delete_edge( graph G, vertex vertex1, vertex vertex2);
extern bool isadjacent( graph G, vertex vertex1, vertex vertex2 );
extern void graph_size( graph G, int *p_vertex_cnt, int *p_edge_cnt );
extern edge *edge_iterator(graph G, vertex vertex_number, edge *p_last_return );

#endif
