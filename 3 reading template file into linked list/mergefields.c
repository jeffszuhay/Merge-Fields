/* MergeFields.c -- Part 3
 * Jeff Szuhay
 * Mobile, Alabama
 * 2025-11-24
 *
 * A line-by-line file reader & field replacement program (think: email merge)
 * Input file: data.txt     -- a line-oriented file with tab delimited fields
 *                             1st line: field names
 *                             2nd thru n-lines: data fields
 * Input file: template.txt -- template (text) file with < and > delimited field names 
 *                             to be replaced by corresponding values.
 * Output file: final.txt   -- a single file with all data field lines merged with template 
 * 
 * This version reads the textfile (template) line-by-line into a singly-linked list
 * and then prints the linked list. Additional data structures were added to support the 
 * requisite linked-list structure. To generate multiple merged-fields, this list will be
 * traversed for each data line, substituting field values, printing out as we go.
 * 
 * Also added is a "CaveManDbg" macro to turn on and off runtime outputs. Caveman debugging
 * is one of the simplest, quickest debugging techniques I know of.
 */


#include "mergefields.h"
#include "linkedlist_line.h"
#include <stdio.h>


void usage( char* cmd )  {
  fprintf( stderr , "usage: %s -d dataFileName [-t templateFileName] [-r resultFileName]\n" , cmd );
  fprintf( stderr , "  -d inputFileName is required.\n" );
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
    //    - output to individual files (by member name) | 1 file | both
  
  while( ( ch = getopt( argc , argv , "d:r:t:h?" ) ) != -1 )  {         /* 1 */
    switch (ch)  {
      case 'd':
        if( NULL == ( dataFile = fopen( optarg , "r") ) )  {
          fprintf( stderr, "data file \"%s\": %s\n", optarg, strerror(errno));
          exit( EXIT_FAILURE );
        }
        CaveManDbg( stderr , "### Using \"%s\" for data.\n" , optarg );
        break;
      case 'r':
        if( NULL == ( resultFile = fopen( optarg , "a" ) ) )  {
          fprintf( stderr, "result file \"%s\": %s\n", optarg, strerror(errno));
          exit( EXIT_FAILURE );
        }
        CaveManDbg( stderr , "### Using \"%s\": for output.\n" , optarg );
        break;
      case 't':
        if( NULL == ( textFile = fopen( optarg , "r" ) ) )  {
          fprintf( stderr, "text file \"%s\": %s\n", optarg, strerror(errno));
          exit( EXIT_FAILURE );
        }
        CaveManDbg( stderr , "### Using \"%s\": as template.\n" , optarg );
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
    CaveManDbg( stderr , "### Using stdout for resulting output.\n" );
  }

  if( !textFile )  {
    CaveManDbg( stderr , "### No template; generating field/value output.\n" );
  }

  /* ======================================================================= */
  
  CaveManDbg( stderr , "### Process template file into lines.\n" );     /* 2 */
  
  size_t numChars = 0;
  size_t numLines = 0;
  bool   useTemplate = (bool)textFile;

  if( textFile ) {
    useTemplate = true;
    char*   line = NULL;
    size_t  lcap = kLineMax;

    ListNode * pNode;
    TextLine*  pTextLine;
    
    LinkedList *pLL = CreateLinkedList(); 

    while( ( numChars = getline( &line , &lcap , textFile ) ) != -1 ) {
      numLines++;
      pTextLine = CreateData( line , numChars, false );
      pNode     = CreateNode( pTextLine );
      AddNode( pLL , pNode , eBack );
      line = NULL;                   // reset so new line is allocated by getline
    } /* while */
    
    CaveManDbg( stderr , "### Printing template file\n" );
    PrintList( pLL , stderr , PrintData );

  } else {
    CaveManDbg( stderr , "### Set switch to generate fields and values.\n" );
    useTemplate = false;
  }
  CaveManDbg( stderr , "### Process data file fields and values.\n" );  /* 3 */
  
  CaveManDbg( stderr , "### Write out result file(s).\n" );             /* 4 */
    
  /* ======================================================================= */
  
  CaveManDbg( stderr , "### Closing files.\n" );                           /* 5 */
  fclose( dataFile );

  fflush( resultFile );
  fclose( resultFile );
  
  if( !textFile ) fclose( textFile );
  CaveManDbg( stderr , "### Done.\n" );
  return 0;
} /* main */


/* eof */
