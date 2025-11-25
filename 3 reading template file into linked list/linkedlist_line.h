// linkedlist_card.h
// Taken from: Chapter 27 - One Handed Solitaire
//             Learn C Programming, 2nd Edition
//
//
// use makefile to build.
//
// Modified from (original) linklist_int.h.
//
// Changes for this project:
// 1. typedef'd TextLine ListData for mergefields data structure
// 2. mergefields.h included.
// 3. Changed function prototypes for PrintList and PrintNode to include File Pointer 
// Function prototypes otherwise identical.


#ifndef _LINKED_LIST_LINE_
#define _LINKED_LIST_LINE_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "mergefields.h"

typedef struct _Node ListNode;
typedef struct _Node  {
  ListNode*  next;
  ListData*  data;
} ListNode;


typedef struct  {
  ListNode*  firstNode;
  int        nodeCount;
} LinkedList;


enum  {
  eFront = 0,
  eBack  = -1
};


void              PrintData(  FILE* fp , ListData* pData ); // defined for specific typedef
ListData*         CreateData( char* pLine , size_t len , bool hasField );
  

LinkedList*       CreateLinkedList();
void              AddNode(    LinkedList* pList , ListNode* pNode , int pos );
ListNode*         RemoveNode( LinkedList* pList , int pos );

bool              IsEmpty(    LinkedList* pList );
int               Size(       LinkedList* pList );

ListNode*         GetNode(    LinkedList* pList , int pos );

ListNode*         CreateNode( ListData* pData );
void              DeleteNode( ListNode* pNode );

void              OutOfStorage( void );

void              PrintList(  LinkedList* pList ,
                              FILE* fp , 
                              void (*printData)(FILE* fp , ListData* pData ) );
void              PrintNode(  ListNode* pNode ,
                              FILE* fp , 
                              void (*printData)(FILE* fp , ListData* pData));


#endif

 // eof