/*
 * user.h
 */

#ifndef _user
#define _user

#include "globals.h"
#include "dlist.h"
#include "interface.h"

int readfile( char *filename, double_list *p_L ) ;
int writefile( char *filename, double_list *p_L ) ;
status writeline( char *s ) ;
int insertlines( char *linespec, double_list *p_head, double_list *p_current ) ;
int deletelines( char *linespec, double_list *p_head, double_list *p_current ) ;
int movelines( char *linespec, double_list *p_head, double_list *p_current ) ;
int printlines( char *linespec, double_list *p_head, double_list *p_current ) ;
int  parse_linespec( char *linespec, double_list head, double_list current, double_list *p_start, double_list *p_end ) ;
status parse_number( char *numberspec, double_list head, double_list current, double_list *p_node ) ;

#endif
