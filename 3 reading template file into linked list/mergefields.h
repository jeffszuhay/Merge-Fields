/*
 * mergefields.h
 */

#ifndef _MERGEFIELDS_H_
#define _MERGEFIELDS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>     // for getopt
#include <sys/errno.h>  // for errno
#include <stdbool.h>

#define DEBUG 0
#if DEBUG
#define CaveManDbg(...) fprintf( __VA_ARGS__ )
#else
#define CaveManDbg(...) (void) 0
#endif

enum {
  kLineMax = 1024,   /* maximum line size */
  kWordMax =  256   /* maximum word size */
};

typedef struct {
	char*  pLine;
	size_t lineLen;
	bool   hasField;
} TextLine;

 // required for linked list processing
typedef TextLine ListData;

#endif

/* eof */
