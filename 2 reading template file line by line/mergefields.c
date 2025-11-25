/* MergeFields.c -- Part 2
 * Jeff Szuhay
 * Mobile, Alabama
 * 2025-11-23
 *
 * A line-by-line file reader & field replacement program (think: email merge)
 *
 * Input file: value.txt    -- a line-oriented file with tab delimited fields
 *                             1st line: field names
 *                             2nd thru n-lines: values for each field
 * Input file: template.txt -- template (text) file with | delimited field names 
 *                             to be replaced by corresponding values.
 * Output file: results.txt -- a single file with all data field lines merged with template 
 *                          
 */


#include "mergefields.h"


  // Print out parameters
void usage( char* cmd )  {
  fprintf( stderr , "usage: %s [-d dataFileName]  [-t templateFileName] [-r resultFileName]\n" , cmd );
  fprintf( stderr , "  If -d inputFileName is not given, stdin is used.\n" );
  fprintf( stderr , "  If -t templateFileName is not given stdout is used and field/value pairs are output.\n\n" );
  fprintf( stderr , "  If -r resultFileName is not given stdout is used.\n\n" );
  exit( EXIT_SUCCESS );
}

  // 1. read command line for filenames, options
  // 2. process template file into lines
  // 3. Process data file fields and values.
  // 4. Write out result file(s).
  // 5. clean up
int main( int argc , char * argv[] ) {
  int   ch;
  FILE* dataFile   = NULL;
  FILE* textFile   = NULL;
  FILE* resultFile = NULL;
  
    // TODO: 
    //  - get month
    //  - output to individual files (by member name) | 1 file | both
    //  - format to use send mail and send
  
  while( ( ch = getopt( argc , argv , "d:r:t:h?" ) ) != -1 )  {         /* 1 */
    switch (ch)  {
      case 'd':
        if( NULL == ( dataFile = fopen( optarg , "r") ) )  {
          fprintf( stderr, "data file \"%s\": %s\n", optarg, strerror(errno));
          exit( EXIT_FAILURE );
        }
        break;
      case 'r':
        if( NULL == ( resultFile = fopen( optarg , "a" ) ) )  {
          fprintf( stderr, "result file \"%s\": %s\n", optarg, strerror(errno));
          exit( EXIT_FAILURE );
        }
        break;
      case 't':
        if( NULL == ( textFile = fopen( optarg , "r" ) ) )  {
          fprintf( stderr, "text file \"%s\": %s\n", optarg, strerror(errno));
          exit( EXIT_FAILURE );
        }
        break;
      case '?':
      case 'h':
      default:
        usage( argv[0] );
        break;
    }
  }
  
    // These have to be outside of the case statement because all arguments may not be
    // given.
    
  if( !dataFile )  {
    fprintf( stderr , "### using stdin in for input not supported.\n");
    usage( argv[0] ); 
    exit( EXIT_FAILURE );
  } else {
    fprintf( stderr , "### Using \"%s\" for data.\n" , optarg );
  }
  
  if( !textFile )  {
    fprintf( stderr , "### No template; generating field/value output.\n" );
  } else {
    fprintf( stderr , "### Using \"%s\": as template.\n" , optarg );
  }
  
  if( !resultFile )  {
    resultFile = stdout;
    fprintf( stderr , "### Using stdout for resulting output.\n" );
  } else {
    fprintf( stderr , "### Using \"%s\": for results.\n" , optarg );
  }
  
  /* ======================================================================= */
  
  fprintf( stderr , "### Process template file into lines.\n" );        /* 2 */
  
  size_t numChars = 0;
  size_t numLines = 0;
  
  if( textFile ) {
    char*   line = NULL;
    size_t  lcap = kLineMax;
    while( ( numChars = getline( &line , &lcap , textFile ) ) != -1 ) {
      // Process the 'line' here
      fprintf( stderr , "%s", line); // Example: print the line
    } /* while */
  } else {
    fprintf( stderr , "### Generate fields and values.\n" );
  }

  fprintf( stderr , "### Process data file fields and values.\n" );     /* 3 */
  
  fprintf( stderr , "### Write out result file(s).\n" );                /* 4 */
  
  /* ======================================================================= */
  
  fprintf( stderr , "### Closing files.\n" );                           /* 5 */
  fclose( dataFile );
  fflush( resultFile );
  fclose( resultFile );
  if( !textFile ) fclose( textFile );
  fprintf( stderr , "### Done.\n" );
  
  return 0;
} /* main */

/* eof */
