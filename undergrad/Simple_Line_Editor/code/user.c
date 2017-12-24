/*
 * user.c                                                                    
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "dlist.h"
#include "globals.h"
#include "user.h"
#include "interface.h"

static FILE *outputfd ;

int readfile ( char *filename , double_list *p_L) {

  char buffer[BUFSIZ] ;
  FILE *fd;

  if(( fd = fopen(filename, "r")) == NULL )
    return 0;

  while( fgets(buffer, BUFSIZ, fd ) != NULL ) {

    if ( string_double_append(p_L, buffer ) == ERROR )
      return E_SPACE ;
  }

  fclose(fd);
  return 0 ;
}

int writefile( char *filename, double_list *p_L ) {

  status rc ;

  if(( outputfd = fopen( filename, "w" )) == NULL )
    return E_IO ;

  rc = double_traverse(*p_L, writeline ) ;

  fclose( outputfd );
  return( rc == ERROR ) ? E_IO : 0 ;
}

status writeline( char *s ) {

  printf( "%s", s ) ; 

  if( fputs( s, outputfd ) == EOF )
    return ERROR;

  return OK ;
}

int insertlines ( char *linespec , double_list *p_head, double_list *p_current ) {

  double_list newdata, startnode, endnode ;
  status rc ;
  int cmp, parseerror ;
  char buffer[BUFSIZ] ;

  if( empty_double_list(*p_head) == TRUE ) {

    if( strlen( linespec ) != 0 )
      return E_LINES ;

    startnode = endnode = NULL ;
  }

  else {

    parseerror = parse_linespec( linespec, *p_head, *p_current, &startnode, &endnode ) ;

    if( parseerror )
      return parseerror ;

    if( startnode != endnode )
      return E_LINES ;
  }

  init_double_list ( &newdata ) ;

  do {

    printf( "insert>" ) ;
    fgets(buffer, BUFSIZ, stdin) ;
    cmp = strcmp(buffer, ".\n") ;

    if( cmp != 0 ) {

      rc = string_double_append( &newdata, buffer ) ;
      if( rc == ERROR )
	return E_SPACE ;
    }
  } while( cmp != 0 ) ;

  if( empty_double_list ( newdata ) == TRUE )
    return 0 ;

  if( startnode == NULL ) {

    *p_head = newdata ;
    *p_current = nth_double_node(newdata, -1 );
  }

  else if ( PREV(startnode) == NULL) {

    double_list lastnode = nth_double_node(newdata, -1);
    paste_list( &lastnode, p_head );
    *p_head = newdata ;
    *p_current = startnode ;
  } 

  else {

    paste_list( &PREV(startnode), &newdata ) ;
    *p_current = startnode ;
  }

  return 0;
}

int deletelines ( char *linespec, double_list *p_head, double_list *p_current ) {

  double_list startnode, endnode, tmplist ;
  double_list new_current ;
  int startnumber, endnumber ;
  int rc = parse_linespec( linespec, *p_head, *p_current, &startnode, &endnode ) ;

  if( rc )
    return rc ;

  startnumber = double_node_number( startnode ) ;

  endnumber = double_node_number( endnode ) ;

  if( startnumber > endnumber ) {

    tmplist = startnode ;
    startnode = endnode ;
    endnode = tmplist ;
  }

  new_current = nth_relative_double_node( endnode, 1 ) ;

  if( new_current == NULL )
    new_current = nth_relative_double_node( startnode, -1 ) ;

  cut_list(p_head, &startnode , &endnode ) ;
  *p_current = new_current ;
  destroy_double_list ( &startnode, free ) ;

  return 0 ;
}

int movelines ( char *linespec , double_list *p_head, double_list *p_current ) {

  double_list startnode, endnode, tmpnode ;
  int startnumber, endnumber, rc, currentnumber, tmp;

  rc = parse_linespec( linespec, *p_head, *p_current, &startnode, &endnode ) ;

  if( rc )
    return rc ;

  startnumber = double_node_number( startnode ) ;
  endnumber = double_node_number( endnode ) ;
  currentnumber = double_node_number( *p_current ) ;

  if( startnumber > endnumber ) {

    tmp = startnumber ;
    startnumber = endnumber ;
    endnumber = tmp ;
    tmpnode = startnode ;
    startnode = endnode ;
    endnode = tmpnode ;
  }

  if(( currentnumber >= startnumber ) && ( currentnumber <= endnumber ))
    return E_LINES;

  cut_list( p_head, &startnode, &endnode ) ;
  paste_list( &PREV( *p_current ), &startnode ) ;
 
  return 0;
}

int printlines( char *linespec, double_list *p_head, double_list *p_current ) {

  double_list startnode, endnode;
  int startnumber, endnumber, count, direction, rc;

  rc = parse_linespec( linespec, *p_head, *p_current, &startnode, &endnode );

  if ( rc )
    return rc ;

  startnumber = double_node_number( startnode ) ;
  endnumber   = double_node_number( endnode ) ;
  direction   = ( startnumber < endnumber ) ? 1 : -1 ;
  count = ( endnumber - startnumber ) * direction + 1 ;

  while ( count-- > 0 ) {

    printf( "%d %s", startnumber, ( char* )DATA( startnode )) ;
    startnumber += direction ;
    startnode = nth_relative_double_node( startnode, direction ) ;
  }

  *p_current = endnode ;

  return 0 ;
}

int parse_linespec ( char *linespec, double_list head, double_list current, double_list *p_start, double_list *p_end ) {

  int rc ;
  char *nextnumber ;

  if (linespec == '\0')
    *p_start = current ;

  else {

    rc = parse_number( linespec, head, current, p_start );

    if( rc )
      return rc ;
  }

  nextnumber = ( char* )strchr( linespec, ',') ;

  if( nextnumber == NULL )
    *p_end = *p_start ;

  else {

    rc = parse_number( nextnumber + 1, head, current, p_end ) ;

    if (rc)
      return rc ;
  }

  if(( *p_start == NULL ) || ( *p_end == NULL ))
    return E_LINES ;

  return 0 ;
} 

status parse_number( char *numberspec, double_list head, double_list current, double_list *p_node ) {

  char numberbuffer[BUFSIZ], *p_num ;
  int nodenumber, direction ;

  if( *numberspec == '.' ) {

    *p_node = current ;
    numberspec++;
  } 

  else if( *numberspec == '$') {

    *p_node = nth_double_node( head, -1 ) ;

    if( *p_node == NULL )
      return E_LINES ;

    numberspec++ ;
  } 

  else if( isdigit( *numberspec )) {

    p_num = numberbuffer ;

    while( isdigit( *numberspec ))
      *p_num++ = *numberspec++ ;

    *p_num = '\0' ;
    nodenumber = atoi( numberbuffer ) ;
    *p_node = nth_double_node( head, nodenumber ) ;

    if ( *p_node == NULL )
      return E_LINES ;

  } 

  else
    return E_LINES ;


  if( *numberspec == '+' ) { 

    direction = 1 ;
    numberspec++;
  }

  else if( *numberspec == '-' ) {

    direction = -1 ;
    numberspec++ ;
  }

  else
    direction = 0 ;

  if( isdigit( *numberspec ) && direction != 0 ) {

    p_num = numberbuffer ;

    while ( isdigit( *numberspec ))
      *p_num++ = *numberspec++ ;

    *p_num = '\0' ;
    nodenumber = atoi( numberbuffer ) * direction ;
    *p_node = nth_relative_double_node ( *p_node, nodenumber ) ;

    if( p_node == NULL )
      return E_LINES ;

    direction = 0 ;
  }

  if( direction == 0 && (*numberspec == '\0' || *numberspec == ',')) 
    return 0 ;

  else
    return E_LINES ;
}
