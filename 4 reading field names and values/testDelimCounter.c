/*
 * testDelimCounter.c
 * Jeff Szuhay
 * Mobile, Alabama
 * 11/26/25
 *
 * Supplemental program to mergefield - Part 4. 
 * As with everything strings, it is best to be certain your code
 * works the way you might assume or think it does. This test
 * program proves it.
 *
 * This program tests a function to count the number of delimiters in a string (line).
 */


#include <string.h>
#include <stdio.h>
#include <stdlib.h>


#define CaveManDbg(...) fprintf( __VA_ARGS__ )
#define MileMarker(...) fprintf( __VA_ARGS__ )
#define ResultLine(...) fprintf( __VA_ARGS__ )


char* lines[] = {
  "",                                // empty line
  "Hello, World!",                   // 0 fields
  "Hello, |name|!",                  // 1 field
  "|greet|, |name|!",                // 2 fields
  "great|, |name|"                  // mismatched fields
};

const char delim = '|';


ssize_t CountDelim( char* line ) {
  ssize_t nDelim = 0;
  size_t  len    = strlen( line ); // NTS: '\0' not counted.
  if( 3 > len ) return 0;

  CaveManDbg( stderr , "### Line length = [%zu]\n" , len );
  char ch;
  for( size_t i = 0 ;
    i < len ; 
    i++ ) {
      ch = line[i] ;
      if( delim == ch ) nDelim++;
    }
  if( nDelim %2 ) {      // odd #? --> mismatched delimiters
    nDelim = 0 - nDelim; // make it negative to show error
  }
  return nDelim;
}


int main( void ) {
  char*   line  = NULL ;
  ssize_t count = 0 ;
  for( int i = 0 ; i < 5 ; i++ ) {
    line = lines[i];
    MileMarker( stderr , "%s\n" , line );
    count = CountDelim( line );
    if( count < 0 ) {
      ResultLine( stderr , "### You musta' goofed up somewhere! (mismatched delimiters)\n" );
    }
    ResultLine( stderr , "line %d has %zd delimiters\n" , i , labs( count ) );
  }
  return 0;
}

/* eof */
