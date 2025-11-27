/* MergeFields.c -- Part 4
 * Jeff Szuhay
 * Mobile, Alabama
 * 2025-11-25
 *
 * A line-by-line file reader & field replacement program (think: email merge)
 * Input file: data.txt     -- a line-oriented file with tab delimited fields
 *                             1st line: field names
 *                             2nd thru n-lines: data fields
 * Input file: template.txt -- template (text) file with < and > delimited field names 
 *                             to be replaced by corresponding values.
 * Output file: result.txt  -- a single file with all data field lines merged with template 
 * 
 * This version reads the parses the data file for field names and field values.
 * If no template file is given, it prints out [field name] = [field value] pairs.
 * If a template file is given, it still just prints out the template file once.
 *
 * I like the idea of the CaveManDbg macro so much that I've added MileMaker macro
 * and ResultLine macro. A more robust logging facility is unnecessary for this project.
 *
 * To complete this project, a findFieldName() function some fiddly string-munching bits
 * are needed. There will be encapsulated in the now placeholder function, doMerge().
 * Then, of course, call to updateResults() to one or more files.
 *
 * The only other noteworthy issue is there are no public/exportable functions. Therefore
 * the header file is pretty bare. We define the functions before they are called to 
 * eliminate the need for function prototypes.
 *
 * At this point, if you run with DEBUG 1, then you may notice that output to stdout is
 * not coordinated with stderr. My 1st guess is they have their own output buffers and
 * so, get spewed each in their own sequence.
 */


#include "mergefields.h"
#include "linkedlist_line.h"

const char delim = '|';

ssize_t CountDelim( char* line ) {
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
  if( nDelim %2 ) {      // odd #? --> mismatched delimiters
    nDelim = 0 - nDelim; // make it negative to show error
  }
  return nDelim;
}


  // Get the number of fields or return 0.
  // Any line with 0 fields (none or mismatched delimiters)
  // will be skipped from field processing.
size_t CountFields( char* line ) {
  ssize_t nDelims = CountDelim( line );
  size_t  nFields = 0;
  
  if( !nDelims || nDelims < 0 ) nFields = 0; // none or error?
  else nFields = nDelims % 2;
  
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


  // placeholder function for merging of template with values
void doMerge() {}

  // placeholder function for creating/appending to result file(s)
void updateResults() {}


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
  // 6. clean up
int main( int argc , char * argv[] ) {
  int   ch;
  FILE* dataFile   = NULL;
  FILE* textFile   = NULL;
  FILE* resultFile = NULL;
  
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
  char*   line = NULL;
  size_t  lcap = kLineMax;
  
  char* fieldNames[  kFieldMax ] = {0};
  char* fieldValues[ kFieldMax ] = {0};
  
  const char delimiters[] = "|";
  
  if( textFile ) {

    /* ===================================================================== */
    
    MileMarker( stderr , "### Read template lines into linked list.\n" );
    
    ListNode * pNode;
    TextLine*  pTextLine;
    
    LinkedList *pLL = CreateLinkedList();                                /* 2 */

    while( ( numChars = getline( &line , &lcap , textFile ) ) != -1 ) {
      numLines++;
      numFields = CountFields( line );
      pTextLine = CreateData( line , numFields );
      pNode     = CreateNode( pTextLine );
      AddNode( pLL , pNode , eBack );
      line = NULL;                  // reset so new line is allocated by getline
    } /* while */
    
    CaveManDbg( stderr , "### Printing template file\n" );
#if defined DEBUG
    PrintList( pLL , stderr , PrintData );
#endif
  
    /* ===================================================================== */
  
    MileMarker( stderr , "### Process data file fields & values.\n" );  /* 3 */
  
    numChars = getline( &line , &lcap , dataFile );
    if( line[numChars-1] != '\0') line[numChars-1]='\0';   // get rid of newline
    getTokens( line , fieldNames , delimiters );
    
    line = NULL;
    while( ( numChars = getline( &line , &lcap , dataFile ) ) != -1 ) {
      numLines++;
      if( line[numChars-1] != '\0') line[numChars-1]='\0'; // get rid of newline
      getTokens( line , fieldValues , delimiters );
      
      /* ==================================================================== */
      
      MileMarker( stderr , "### Merge values into template lines.\n" );  /* 4 */
      
      doMerge();
      
      MileMarker( stderr , "### Write out result file(s).\n" );          /* 5 */
      
      updateResults();
      
      line = NULL;                  // reset so new line is allocated by getline
    } /* while */

  } else {
    CaveManDbg( stderr , "### Set switch to generate fields and values.\n" );
    useTemplate = false;
      
    numChars = getline( &line , &lcap , dataFile );
    if( line[numChars-1] != '\0') line[numChars-1]='\0';   // get rid of newline
    getTokens( line , fieldNames , delimiters );
    
    line = NULL;
    while( ( numChars = getline( &line , &lcap , dataFile ) ) != -1 ) {
      numLines++;
      if( line[numChars-1] != '\0') line[numChars-1]='\0'; // get rid of newline
      getTokens( line , fieldValues , delimiters );
      
      for( int i=0 ; i < kFieldMax ; i++ ){
        if( NULL == fieldNames[i] ) break; // done
        ResultLine( resultFile , " [%s] = [%s]\n" , fieldNames[i] , fieldValues[i] );
      } /* for */
      ResultLine( resultFile , "\n" );
      line = NULL;                   // reset so new line is allocated by getline
    } /* while */
  } /* else */
  
  MileMarker( stderr , "### Closing files.\n" );                        /* 6 */
  fclose( dataFile );

  fflush( resultFile );
  fclose( resultFile );
  
  if( !textFile ) fclose( textFile );
  MileMarker( stderr , "### Done.\n" );
  return 0;
} /* main */


/* eof */
  