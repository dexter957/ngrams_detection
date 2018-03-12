#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "trie.h"


#define TRUE 1
#define FALSE 0
#define SCREEN 2
#define FILEIN 3
#define END_OF_COMMANDS 4/*The user has pressed exit on the command line mode*/
#define BUFFER_MAX 2000 /*For input*/

int screenOrFile(int sOrF, char* fileName, TriePtr trie);
int parser(char* inputBuffer, TriePtr trie);
int initFile(char* fileName, TriePtr trie);
void tokenizer(char* stringToTokenize, char* separator, char*** tokens, int* numOfTokens);