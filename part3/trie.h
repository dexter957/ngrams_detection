#ifndef _TRIEH_
#define _TRIEH_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "binHeapAr.h"

#include "./linear_hash/linear_hash_v2.h"
#include "./linear_hash/linear_hash_v3.h"

#include "./trie_nodes/trie_node_connector.h"
#include "bloomFilter.h"
#include "treap.h"
#include "tokenizers.h"


#define N 16
#define HASH_SIZE 512


struct Trie;
typedef struct Trie* TriePtr;


typedef struct Query{
	int ticket, trieVer;
	TriePtr trie;
	char* line;
}Query;

typedef struct Query* QueryPtr;



TriePtr init_trie(int, int);
void clear_trie(TriePtr);

void shrinkStaticTrie(TriePtr);
int getStatic(TriePtr);

void insert_ngram(TriePtr, char**, int);
void insert_ngramD(TriePtr, char**, int, int);
void insert_ngramS(TriePtr, char**, int);
void search_and_insertD(void*, char**, int, int);
void search_and_insertS(void*, char**, int, int*);

void delete_ngram(TriePtr, char **, int, int);
void search_and_delete(void*, char **, int, int);

char* setToPrint(char*, int*, char*);

int prepareResultsArray(TriePtr, int);
void query(QueryPtr);
void queryD(QueryPtr);
void queryS(QueryPtr);
char* searchD(void*, char**, int, int*, pointerToBloomFilter, char*, int*, int);
char* searchS(void*, char**, int, int*, pointerToBloomFilter, char*, int*);
void printResults(TriePtr, pointerToTreapNode*, int);

void printTrie(TriePtr);

void makeTreap(char*, char*, pointerToTreapNode*);

#endif