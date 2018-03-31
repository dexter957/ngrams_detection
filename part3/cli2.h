#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "trie.h"
#include "scheduler.h"
#include "tokenizers.h"
//#include "./trie_nodes/trie_node_connector.h"

#define TRUE 1
#define FALSE 0
#define SCREEN 2
#define FILEIN 3
#define END_OF_COMMANDS 4/*The user has pressed exit on the command line mode*/
#define BUFFER_MAX 2000 /*For input*/
#define STATIC_KEYWORD "STATIC"
#define DYNAMIC_KEYWORD "DYNAMIC"

int screenOrFile(int sOrF, char* fileName, TriePtr trie);
int parser(char* inputBuffer, TriePtr trie,pointerToTreapNode* root);
int initFile(char* fileName, TriePtr* trie);
void initFileD(FILE* fp, TriePtr* trie);
void initFileS(FILE* fp, TriePtr* trie);

/*New functions for part3*/

int parser2D(char* inputBuffer, TriePtr trie, pointerToScheduler theScheduler, int* numberOfQueries, int *trieVer, int *trieFlag);
int parser2S(char* inputBuffer, TriePtr trie, pointerToScheduler theScheduler, int* numberOfQueries);






