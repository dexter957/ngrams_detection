#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "binHeapAr.h"
#include "trie_node.h"
//#include "semistatic_trie_node.h"
//#include "semistatic_trie_node_v2.h"



struct Trie;
typedef struct Trie* TriePtr;



TriePtr init_trie();
void clear_trie(TriePtr);
void traverse_and_clear(pointerToTrieNode);

void insert_ngram(TriePtr, char**, int);
void search_and_insert(pointerToTrieNode, char**, int);

void delete_ngram(TriePtr, char**, int);
void search_and_delete(pointerToTrieNode, char**, int, TriePtr);
//int search_and_delete(pointerToTrieNode, char**, int);  //RECURSIVE


void search_for_ngrams(TriePtr, char**, int);
void search(pointerToTrieNode, char**, int, int*, pointerToBinHeapArray*);
//void search_for_ngrams(TriePtr, char**, int);												//SLOWER
//void search(pointerToTrieNode, char**, int, int*, pointerToBinHeapArray*, char*, int);	//SLOWER




void printTrie(TriePtr);
void printTrieNodeTreeMode(pointerToTrieNode, int);
void printTrie2(TriePtr);
void printTrieNodeTreeMode2(pointerToTrieNode);