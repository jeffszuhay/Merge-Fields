/* MergeFields.c -- Part 5
 * Jeff Szuhay
 * Mobile, Alabama
 * 2025-12-4
 *
 * A line-by-line file reader & field replacement program (think: email merge)
 * Input file: data.txt     -- a line-oriented file with '|' delimited fields
 *                             1st line: field names
 *                             2nd thru n-lines: data fields
 * Input file: template.txt -- template (text) file with '|' delimited field names 
 *                             to be replaced by corresponding values.
 * Output file: result.txt  -- a single file with all data field lines merged with template 
 * 
 * Delimter character is '|'
 *
 * This version reads the parses the data file for field names and field values.
 * If no template file is given, it prints out [field name] = [field value] pairs.
 * If a template file is given, it prints out the template with merged data for 
 * for each line of data.
 *
 * This is the final version for this project.
 *
 * The code could be made cleaner with some refactoring.
 * The code could be made universal by supporting UTF-8.
 * The code could be made to use a variety of delimiters (it only uses '|' now).
 * The code could be enhanced to spit out individual files.
 * I leave those as challenges to you, dear reader.
 */

#include "mergefields.h"
#include "linkedlist_line.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


const char delimiters[] = "|";


ssize_t CountDelim( char* line , const char delim) {
  ssize_t nDelim = 0;
  size_t  len    = strlen( line ); // NTS: '\0' not counted.
  if( 3 > len ) return 0;
  
  char ch;
  for( size_t i = 0 ;
    i < len ; 
    i++ ) {
      ch = line[i] ;
      if( delim == ch ) nDelim++;
  }
  if( nDelim % 2 ) {      // odd #? --> mismatched delimiters
    nDelim = 0 - nDelim; // make it negative to show error
  }
  return nDelim;
}


  // Get the number of fields or return 0.
  // Any line with 0 fields (none or mismatched delimiters)
  // will be skipped from field processing.
size_t CountFields( char* line ) {
  ssize_t nDelims = CountDelim( line , delimiters[0] );
  size_t  nFields = 0;
  
  if( !nDelims || nDelims < 0 ) nFields = 0; // none or error?
  else nFields = nDelims / 2;
  
  return nFields;
}

  // Parse a data file line into individual fields (names or values.
void getTokens( char* line , char* tokens[] , const char* delims ) {
  char* token = strtok(  line , delims );
  int idx = 0;
  while( NULL != token ) {
    tokens[ idx++ ] = token ;
    token = strtok( NULL , delims );
  }
  for( int i = 0 ; i<idx ; i++ ) {
    CaveManDbg( stderr, "[%s] " , tokens[i] );
  };
  CaveManDbg( stderr , "\n" );	
}


  // GetFieldIndex() 
  // Given a field name string, the # of fields and the field names array,
  // find the index of theat field name string. 
  // Otherwise, return -1.
int GetFieldIndex( char* name , int fieldCount , char* names[] ) {
  for( int idx = 0 ; idx < fieldCount ; idx++ ) {
    if( NULL == names[ idx ]) return -1;
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
  int idx = GetFieldIndex( pName , kFieldMax , pFields );
  CaveManDbg( stderr , "Idx = [%d]\n" , idx );
  if( idx < 0 ) return NULL;
  else          return pValues[idx];
}


void FindAndReplace( char* outLine , char* textLine ,
                     char* fieldNames[] , char* fieldValues[] , 
                     const char delim ) {
  char* pBegStr = textLine;
  char* pEndStr = NULL;
  char* pValueStr = NULL;
  
  while( NULL != (pEndStr = strchr(pBegStr, delim ) ) ) {
    *pEndStr = '\0' ;                    // end of text. NOTE: modifies line[]
    strcat( outLine , pBegStr );
    CaveManDbg( stderr , "%s\n" , outLine );
    
    // process field name
    pBegStr = pEndStr+1;                 // beginning of field name
    pEndStr = strchr( pBegStr , delim );
    *pEndStr = '\0';                     // end of field name.  NOTE: modifies line[]
    if( NULL != (pValueStr = GetFieldValue( pBegStr  , fieldNames , fieldValues ) ) )
      strcat( outLine , pValueStr );
    else
      strcat( outLine , "???");
    CaveManDbg( stderr , "%s\n" , outLine );
    
    // continue
    pBegStr = pEndStr+1;                 // adjust beginning of text
  }
  strcat( outLine, pBegStr );
}


void usage( char* cmd )  {
  fprintf( stderr , "usage: %s -d dataFileName [-t templateFileName] [-r resultFileName]\n" , cmd );
  fprintf( stderr , "  -d inputFileName is required.\n" );
  fprintf( stderr , "  If -t templateFileName is not given stdout is used and field/value pairs are output.\n\n" );
  fprintf( stderr , "  If -r resultFileName is not given stdout is used.\n\n" );
  exit( EXIT_SUCCESS );
}

 
  // 1. Read command line for filenames, options
  // 2. Process template lines into linked list.
  // 3. Process data file fields and values.
  // 4. Merging values into template lines.
  // 5. Generating output file(s)
  // 6. No template file: give name|value pairs.
  // 7. Clean up
int main( int argc , char * argv[] ) {
  int   ch;
  FILE* dataFile   = NULL;
  FILE* textFile   = NULL;
  FILE* resultFile = NULL;
  
  char  outBuff[ kLineMax ] = {0};
  
    // TODO: 
    //    - output to individual files (by member name) | 1 file | both
  
  while( ( ch = getopt( argc , argv , "d:r:t:h?" ) ) != -1 )  {         /* 1 */
    switch (ch)  {
      case 'd':
        if( NULL == ( dataFile = fopen( optarg , "r") ) )  {
          fprintf( stderr, "data file \"%s\": %s\n", optarg, strerror(errno));
          exit( EXIT_FAILURE );
        }
        ResultLine( stdout , "### Using \"%s\" for data.\n" , optarg );
        break;
      case 'r':
        if( NULL == ( resultFile = fopen( optarg , "a" ) ) )  {
          fprintf( stderr, "result file \"%s\": %s\n", optarg, strerror(errno));
          exit( EXIT_FAILURE );
        }
        ResultLine( stderr , "### Using \"%s\": for output.\n" , optarg );
        break;
      case 't':
        if( NULL == ( textFile = fopen( optarg , "r" ) ) )  {
          fprintf( stderr, "text file \"%s\": %s\n", optarg, strerror(errno));
          exit( EXIT_FAILURE );
        }
        ResultLine( stdout , "### Using \"%s\": as template.\n" , optarg );
        break;
      case '?':
      case 'h':
      default:
        usage( argv[0] );
        break;
    }
  }
  
  if( !dataFile )  {
    fprintf( stderr , "### using stdin in for input not supported.\n");
    usage( argv[0] ); 
    exit( EXIT_FAILURE );
  }
  
  if( !resultFile )  {
    resultFile = stdout;
    ResultLine( stdout , "### Using stdout for resulting output.\n" );
  }

  if( !textFile )  {
    ResultLine( stdout , "### No template; generating field/value output.\n" );
  }
  ResultLine( stdout , "\n" );
  
  /* ===================================================================== */
  
  ssize_t numFields  = 0;
  size_t  numChars   = 0;
  size_t  numLines   = 0;
  bool    useTemplate = (bool)textFile;

  useTemplate  = true;
  char*   dataLine = NULL;
  char*   textLine = NULL;
  size_t  lcap = kLineMax;
  
  char* fieldNames[  kFieldMax ] = {0};
  char* fieldValues[ kFieldMax ] = {0};
  
  if( !textFile ) {
    /* ===================================================================== */    
    MileMarker( stderr,"### No template file: generate name|value pairs.\n" );
    
    useTemplate = false;                                                /* 6 */
    
    numChars = getline( &dataLine , &lcap , dataFile );
    if( dataLine[numChars-1] != '\0') dataLine[numChars-1]='\0';   // get rid of newline
    getTokens( dataLine , fieldNames , delimiters );
    
    dataLine = NULL;
    while( ( numChars = getline( &dataLine , &lcap , dataFile ) ) != -1 ) {
      numLines++;
      if( dataLine[numChars-1] != '\0') dataLine[numChars-1]='\0'; // get rid of newline
      getTokens( dataLine , fieldValues , delimiters );
      
      for( int i=0 ; i < kFieldMax ; i++ ){
        if( NULL == fieldNames[i] ) break; // done
        ResultLine( resultFile , " [%s] = [%s]\n" , fieldNames[i] , fieldValues[i] );
      }
      ResultLine( resultFile , "\n" );
      dataLine = NULL;                   // reset so new line is allocated by getline
    } /* while */
  } else {
    /* ===================================================================== */    
    MileMarker( stderr , "### Read template lines into linked list.\n" );
    
    ListNode * pNode;
    TextLine*  pTextLine;
    
    LinkedList *pLL = CreateLinkedList();                               /* 2 */

    while( ( numChars = getline( &textLine , &lcap , textFile ) ) != -1 ) {
      numLines++;
      numFields = CountFields( textLine );
      pTextLine = CreateData( textLine , numFields );
      pNode     = CreateNode( pTextLine );
      AddNode( pLL , pNode , eBack );
      textLine = NULL;                  // reset so new line is allocated by getline
    } /* while */
    
    CaveManDbg( stderr , "### Printing template file\n" );
#if DEBUG
    PrintList( pLL , stderr , PrintData );
#endif
  
    /* ===================================================================== */
  
    MileMarker( stderr , "### Process data file fields & values.\n" );  /* 3 */
    
    numChars = getline( &dataLine , &lcap , dataFile );
    if( dataLine[numChars-1] != '\0') dataLine[numChars-1]='\0';   // get rid of newline
    getTokens( dataLine , fieldNames , delimiters );
    
    numLines = 0;
    dataLine = NULL;
    while( ( numChars = getline( &dataLine , &lcap , dataFile ) ) != -1 ) {
      numLines++;
      
      if( dataLine[numChars-1] != '\0') dataLine[numChars-1]='\0'; // get rid of newline
      getTokens( dataLine , fieldValues , delimiters );
      
      /* ==================================================================== */
      MileMarker( stderr , "### Merge values into template lines.\n" );  /* 4 */
      MileMarker( stderr , "### Write out result file(s).\n" );          /* 5 */
            
      int numTextLines = Size( pLL );
      char* outLine = NULL;
      for(int i = 0 ; i < numTextLines ; i++ ) {
        pNode = GetNode( pLL , i );
        
        int numFields = pNode->data->numFields;
        if( 0 < numFields ) {
            // make a copy, we're going to munge it
          char * tempLine = (char*)calloc( strlen(pNode->data->pLine)+1 , sizeof(char ) );
          strcpy( tempLine , pNode->data->pLine );
            // find and replace field names with data in 
          FindAndReplace( outBuff , tempLine , fieldNames , fieldValues , delimiters[0] );
          free( tempLine );
        } else {
          strcpy( outBuff , pNode->data->pLine );
        } /* numFields */

        ResultLine( resultFile , "%s" , outBuff );
        memset( outBuff , 0 , kLineMax );
      } /* for numTextLines */
      ResultLine( resultFile , "\n\n" );
      
      dataLine = NULL;                  // reset so new line is allocated by getline
    } /* while dataLines */
  } /* else */
  
  MileMarker( stderr , "### Closing files.\n" );                        /* 7 */
  fclose( dataFile );

  fflush( resultFile );
  fclose( resultFile );
  
  if( !textFile ) fclose( textFile );
  MileMarker( stderr , "### Done.\n" );
  return 0;
} /* main */


/* eof */
  