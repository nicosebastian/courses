/*
 * dlist.c
 */

#include "dlist.h"
#include "globals.h"
#include <stdlib.h>
#include <string.h>

extern status allocate_double_node( double_list *p_L, generic_ptr data ) {

  double_list L = ( double_list )malloc( sizeof( double_node )) ;

  if( L == NULL )
    return ERROR ;

  *p_L = L ;
  DATA( L ) = data ;
  NEXT( L ) = NULL ;
  PREV( L ) = NULL ;

  return OK ;
}

extern void free_double_node( double_list *p_L ) {

  free( *p_L ) ;
  *p_L = NULL ;
}

extern status init_double_list( double_list *p_L ) {

  *p_L = NULL ;
  return OK ;
}

extern bool empty_double_list( double_list L ) {
  
  return( L == NULL ) ? TRUE : FALSE ;
}

extern status double_insert( double_list *p_L, generic_ptr data ) {

  double_list L ;
  
  if( allocate_double_node( &L, data ) == ERROR )
    return ERROR ;

  if( empty_double_list( *p_L ) == TRUE )
    PREV( L ) = NEXT( L ) = NULL ;

  else {
    NEXT( L ) = *p_L ;
    PREV( L ) = PREV( *p_L ) ;
    PREV( *p_L ) = L ;
    
    if( PREV( L ) != NULL )
      NEXT( PREV( L ) ) = L ;
  }

  *p_L = L ;

  return OK ;
}
   
      
extern status double_delete( double_list *p_L, generic_ptr *p_data ) {

  if( empty_double_list( *p_L ) == TRUE )
    return ERROR ;

  *p_data = DATA( *p_L ) ;

  return double_delete_node( p_L, *p_L ) ;
}

extern status double_delete_node( double_list *p_L, double_list node ) {

  double_list prev, next ;
  
  if( empty_double_list( *p_L ) == TRUE )
    return ERROR ;

  prev = PREV( node ) ;
  next = NEXT( node ) ;

  if( prev != NULL )
    NEXT( prev ) = next ;

  if( next != NULL )
    PREV( next ) = prev ;
  
  if( node == *p_L ){
    
    if( next != NULL )
      *p_L = next ;

    else
      *p_L = prev ;
  }

  free_double_node( p_L ) ;
  return OK ;
}

extern void cut_list( double_list *p_L, double_list *p_start, double_list *p_end ) {
  
  double_list start = *p_start ;
  double_list end = *p_end ;
  
  if( PREV( start ))
    NEXT( PREV( start )) = NEXT( end ) ;

  if( NEXT( end ))
    PREV( NEXT( end )) = PREV( start ) ;

  if( *p_L == start )
    *p_L = NEXT( end ) ;
  
  PREV( start ) = NEXT( end ) = NULL ;
}
  
extern void paste_list( double_list *p_target, double_list *p_source ) {

  double_list target, source, lastnode ;

  if( empty_double_list( *p_source ) == TRUE )
    return ;

  if( empty_double_list( *p_target ) == TRUE )
    *p_target = *p_source ;

  else {

    source = *p_source ;
    target = *p_target ;
    lastnode = nth_double_node( source, -1 ) ;
    NEXT ( lastnode ) = NEXT( target ) ;

    if ( NEXT( target ) != NULL )
      PREV( NEXT( target ) ) = lastnode ;

    PREV( source ) = target ;
    NEXT( target ) = source ;
  }

  *p_source = NULL ;
}

extern status double_append( double_list *p_L, generic_ptr data ) {
  
  double_list L, temp ;
   
  if( allocate_double_node( &L, data ) == ERROR )
    return ERROR ;

  if( empty_double_list( *p_L ) == TRUE )
    *p_L = L ;

  else {
    
    for( temp = *p_L ; NEXT( temp ) != NULL ; temp = NEXT( temp )) ;

    NEXT( temp ) = L ;
    PREV( L ) = temp ;
  }

  return OK ;
}

extern int double_length( double_list L ) {

  int n ;
  
  if( empty_double_list( L ) == TRUE )
    return 0 ; 

  for( n = 0 ; NEXT( L ) != NULL ; L = NEXT( L ), n++ ) ;
    
  return ++n ;  
}

extern status double_traverse( double_list L, status (*p_func_f)()) {

  if( empty_double_list( L ) == TRUE )
    return OK ;

  if(( *p_func_f ) ( DATA( L )) == ERROR )
    return ERROR ;

  return double_traverse( NEXT( L ), p_func_f ) ;
}

extern void destroy_double_list( double_list *p_L, void ( *p_func_f )()) {

  if( empty_double_list( *p_L ) == FALSE ) {

    if( p_func_f != NULL ) ( *p_func_f )( DATA( *p_L )) ;  
  
    destroy_double_list( &NEXT( *p_L ), p_func_f ) ;

    free_double_node( p_L ) ;
  }
}

extern int double_node_number( double_list L ) {
  
  int n ;

  empty_double_list( L ) ;

  for( n = 1 ; PREV( L ) != NULL ; n++, L = PREV( L )) ;
      
  return n ; 
}

extern double_list nth_double_node( double_list L, int n ){
  
  double_list temp ;
  int i ;

  if( L == NULL )
    return NULL ; 

  if( n == 1 )
    return L ; 

  if( n == -1 ) {
    
    for( temp = L ; NEXT( temp ) != NULL ; temp = NEXT( temp ) ) ;

    return temp ;
  }
  
  for( i = 0, temp = L ; i < n && NEXT( temp ) != NULL ; i++, temp = NEXT( temp ) ) ;
  
  return PREV( temp ) ;   
}

extern double_list nth_relative_double_node( double_list L, int n ) {

  int i ;
 
  if( !n )
    return L ;

  if( n > 0 ) {
    
    for( i = 0; i < n && NEXT( L ) != NULL ; i++, L = NEXT( L ) ) ;     
    return L ; 
  }

  for( i = n ; i < 0 && PREV( L ) != NULL ; i++, L = PREV( L ) ) ;
  
  return L ;
}
