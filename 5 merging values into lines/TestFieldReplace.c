
/*
* testFieldReplace.c
* Jeff Szuhay
* Mobile, Alabama
* 12/2/2025
*
* Supplemental program to mergefield - Part 5. 
* As with everything strings, it is best to be certain your code
* works the way you might assume or think it does. This test
* program proves it.
*
* As always, don't just copy this code. Instead, twiddle, tweak, 
* and frob it so that you understand how it works. Then  you can 
* modify it as needed to work in your situation.
*
* The programs tests parsing field names from a line and replacing 
* it with the corresponding value.
*
* CHALLENGE: Modify this to support Unicode and add Russian, Japanese, 
*            Hebrew, and Arabic as well as proper characters for
*            each of the current languages (Spanish, French, Hungarian)
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG 0
#if DEBUG
#define CaveManDbg(...) fprintf( __VA_ARGS__ )
#else
#define CaveManDbg(...) (void) 0
#endif

#define ResultLine(...) fprintf( __VA_ARGS__ )

enum {
  eFields    = 5 ,
  eLines     = 5 , 
  eBuffSize  = 256 ,
};

const char delim = '|';


  // GetFieldIndex() 
  // Given a field name string, the # of fields and the field names array,
  // find the index of theat field name string. 
  // Otherwise, return -1.
int GetFieldIndex( char * name , int fieldCount , char* names[] ) {
  for( int idx = 0 ; idx < fieldCount ; idx++ ) {
    if( 0 == strcmp( name , names[ idx ] ) ) return idx;
  }
  CaveManDbg( stderr , " ### ERROR could not find field name in field names.\n") ;
  return -1 ; // fail: could not find field name.
}


  // GetFieldValue()
  // Given a field name, the field name array, and the single line values array,
  // return a pointer to the value string. 
  // Otherwise, return null.
char* GetFieldValue( char* pName , char* pFields[] , char* pValues[] ) {
  int idx = GetFieldIndex( pName , eFields , pFields );
  CaveManDbg( stderr , "Idx = [%d]\n" , idx );
  if( idx < 0 ) return NULL;
  else          return pValues[idx];
}


int main(int argc, char *argv[]) {
  char* fieldNames[] = 
    { "1st" , "2nd" , "3rd" , "4th" , "lang"};
  char* fieldValues[eLines][eFields] = {
    { "uno"  , "dos"  , "tres"  , "quattro" , "Espanol"   } ,
    { "eine" , "zwei" , "drei"  , "fier"    , "Deutsch"  } ,
    { "one"  , "two"  , "three" , "four"    , "English"  } ,
    { "edy"  , "ketto", "harom" , "negy"    , "Magyarul" } ,
    { "un"   , "deux" , "trois" , "quatre"  , "Francais" } };

      // test lines
      
    char* text = "[|lang|] The number |1st| comes before |4th| which is after |3rd| (don't forget |2nd|)." ;
    //  Test case where delimiter is 1st char.
//  char* text = "|1st| comes before |4th| which is after |3rd| (don't forget |2nd|)--|lang|." ;

    // NOTE: the line string needs to be modifiable which is why it must be dynamicall allocated.
  char* line = (char*)calloc( strlen(text)+1, sizeof(char) ); // don't forget '\0'
  strcpy( line , text );
  
    // Verify our test data
    
  for( int i = 0 ; i < eFields ; i++)
   CaveManDbg( stderr , "[%7s] " , fieldNames[i] );
  CaveManDbg( stderr , "\n" );
  
  for( int j = 0 ; j < eLines ; j++ ) {
    for( int i = 0 ; i < eFields ; i++ )
      CaveManDbg( stderr , "[%7s] ", fieldValues[j][i] );
    CaveManDbg( stderr , "\n" );
  }
  CaveManDbg( stderr , "%s\n" ,  line );

    // verified. 
    
    // process fields->values creating new merged line.
  
  char outLine[ eBuffSize ] = {0};         // arbitrarily ample line buffer size.
  char* pBegStr   = NULL;
  char* pEndStr   = NULL;
  char* pValueStr = NULL;
  for( int lineNo = 0 ; lineNo < eLines ; lineNo++ ) {
    pBegStr = line;                        // beginning of text
    while( NULL != (pEndStr = strchr(pBegStr, delim ) ) ) {
      *pEndStr = '\0' ;                    // end of text. NOTE: modifies line[]
      strcat( outLine , pBegStr );
      CaveManDbg( stderr , "%s\n" , outLine );

        // process field name
      pBegStr = pEndStr+1;                 // beginning of field name
      pEndStr = strchr( pBegStr , delim );
      *pEndStr = '\0';                     // end of field name.  NOTE: modifies line[]
      if( NULL != (pValueStr = GetFieldValue( pBegStr  , fieldNames , fieldValues[lineNo] ) ) )
        strcat( outLine , pValueStr );
      else
        strcat( outLine , "???");
      CaveManDbg( stderr , "%s\n" , outLine );

        // continue
      pBegStr = pEndStr+1;                 // adjust beginning of text
    }
    strcat( outLine, pBegStr );
    ResultLine( stderr , "%s\n" , outLine );

      // reset input and output lines
    strcpy( line , text );
    memset( outLine , 0 , eBuffSize );
  }  
  return 0;               
}

  /* eof */
  