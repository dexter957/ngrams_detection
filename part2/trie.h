#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "binHeapAr.h"

#include "./linear_hash/linear_hash_v2.h"
//#include "./linear_hash/linear_hash_v3.h"

#include "./trie_nodes/trie_node_connector.h"
#include "bloomFilter.h"
#include "treap.h"

#define N 16
#define HASH_SIZE 512


struct Trie;
typedef struct Trie* TriePtr;



TriePtr init_trie(int, connectorPtr);
void clear_trie(TriePtr);
//void traverse_and_clear(void*);
void shrinkStaticTrie(TriePtr);
int getStatic(TriePtr);

void insert_ngram(TriePtr, char**, int);
void search_and_insert(void*, char**, int, connectorPtr);

void delete_ngram(TriePtr, char**, int);
void search_and_delete(void*, char**, int);


void search_for_ngrams(TriePtr, char**, int, pointerToTreapNode*);
char* search(void*, char**, int, int*, /*pointerToBinHeapArray**/ pointerToBloomFilter, pointerToTreapNode*, connectorPtr, char*, int*);
char* setToPrint(char*, int*, char*);
//void search_for_ngrams(TriePtr, char**, int);												//SLOWER
//void search(void*, char**, int, int*, pointerToBinHeapArray*, char*, int);	//SLOWER




void printTrie(TriePtr);
void printTrieNodeTreeMode(void*, int);
void printTrie2(TriePtr);
void printTrieNodeTreeMode2(void*);