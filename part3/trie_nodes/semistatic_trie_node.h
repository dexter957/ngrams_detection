#include "specs.h"

#if DYN_NODE == 2

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 32
#define TRUE 1
#define FALSE 0

struct trie_node;
typedef struct trie_node* pointerToTrieNode;


int allocateTrie_Node(pointerToTrieNode*);
void initialiseTrie_Node(pointerToTrieNode);

//GETTERS
size_t getSizeOf();
int getNumOfChildrenPtr(pointerToTrieNode);
int getIs_FinalPtr(pointerToTrieNode);
int getActualNumOfChildrenPtr(pointerToTrieNode);
char* getWordPtr(pointerToTrieNode);
pointerToTrieNode getChildrenPtr(pointerToTrieNode);
pointerToTrieNode getCertainChildPtr(pointerToTrieNode, int);
int getA_versionPtr(pointerToTrieNode);
int getD_versionPtr(pointerToTrieNode);
void getVersionsPtr(pointerToTrieNode,int*, int*);

int getNumOfChildren(struct trie_node);
int getIs_Final(struct trie_node);
int getActualNumOfChildren(struct trie_node);
char* getWord(struct trie_node);
pointerToTrieNode getChildren(struct trie_node);


//SETTERS
void setNumOfChildrenPtr(pointerToTrieNode, int);
void setIs_FinalPtr(pointerToTrieNode, int);
void setActualNumOfChildrenPtr(pointerToTrieNode, int);
void setWordPtr(pointerToTrieNode, char*);
void setChildrenPtr(pointerToTrieNode);
void setA_versionPtr(pointerToTrieNode, int);
void setD_versionPtr(pointerToTrieNode, int);
void setVersionsPtr(pointerToTrieNode, int, int);

void setNumOfChildren(struct trie_node, int);
void setIs_Final(struct trie_node, int);
void setActualNumOfChildren(struct trie_node, int);
void setWord(struct trie_node, char*);
void setChildren(struct trie_node);


void setTrie_NodeValues(pointerToTrieNode, int, int, int, int, char*);
int doubleNumOfChildren(pointerToTrieNode);
int insertAChild(pointerToTrieNode, char*);
void moveChildren(pointerToTrieNode, int, int);
int searchPositionBinary(pointerToTrieNode, int, int, char*, int*);
void printTrieNode(pointerToTrieNode);
void destroyNode(pointerToTrieNode);
void destroyNode(pointerToTrieNode);
void clearNode(pointerToTrieNode);
void traverseAndDestroy(pointerToTrieNode);
void deleteANode(pointerToTrieNode, int);
void moveChildrenDel(pointerToTrieNode, int, int);


void repointToWords(pointerToTrieNode, int);

#endif