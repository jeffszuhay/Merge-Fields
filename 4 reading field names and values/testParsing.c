/*
* testParsing.c
* Jeff Szuhay
* Mobile, Alabama
* 11/26/25
*
* Supplemental program to mergefield - Part 4. 
* As with everything strings, it is best to be certain your code
* works the way you might assume or think it does. This test
* program proves it.
*
* The programs tests parsing field names and values with strtok().
*/


#include <string.h>
#include <stdio.h>
#include <stdlib.h>


#define CaveManDbg(...) fprintf( __VA_ARGS__ )
#define MileMarker(...) fprintf( __VA_ARGS__ )
#define ResultLine(...) fprintf( __VA_ARGS__ )


void getTokens( char* line , char* tokens[] , const char* delims ) {
	char* token = strtok(  line , delims );
	int idx = 0;
	while( NULL != token ) {
		tokens[ idx++ ] = token ;
		token = strtok( NULL , delims );
	}
	for( int i = 0 ; i<idx ; i++ ) {
		MileMarker( stderr, "[%s] " , tokens[i] );
	};
	MileMarker( stderr , "\n" );	
}

  // here, we have to mimic what getline() will do when it reads a line.
char** InitLines( void ) {	
  size_t numLines = 4;
	char** lines = (char**)calloc( numLines , sizeof(char*) );
	
	lines[0] = (char*)calloc(80, sizeof(char) );
  strcpy( lines[0] , "name|email|first|amount|day" );
	lines[1] = (char*)calloc(80, sizeof(char) );
  strcpy( lines[1] , "Dewy Cheattum|Dewey_Cheattum@AndHowe.esq|Dewy|100.00|1st" );
	lines[2] = (char*)calloc(80, sizeof(char) );
  strcpy( lines[2] , "Paige Turner|paige@bookburner.org|Paige|50.00|15th" );
	lines[3] = (char*)calloc(80, sizeof(char) );
  strcpy( lines[3] , "Chuck Roast|chuck@sliceanddice.com|Chuck|100|27th" );
  return lines;
}
	
int main( void ) {
  char** lines = InitLines();
	char*  fields[16] = {0};
	char*  values[16] = {0};

	const char delimiters[] = "|";

	getTokens( lines[0] , fields , delimiters );
	
	for( int lineNo = 1 ; lineNo < 4 ; lineNo++ ) {
		getTokens( lines[ lineNo ] , values , delimiters );
	}
	
	return 1;
} /* main */

/* eof */
