/*
 * interface.c
 */

#include <stdlib.h>
#include <string.h>

#include "globals.h"
#include "dlist.h"
#include "interface.h"

status string_double_append( double_list *p_L, char *buffer ) {

  char *p_data = ( char* )malloc(( strlen( buffer ) + 1 ) * sizeof( char )) ;

  if( p_data == NULL )
    return ERROR ;

  strcpy( p_data, buffer ) ;

  double_append( p_L, ( generic_ptr )p_data ) ;

  return OK ;
}
