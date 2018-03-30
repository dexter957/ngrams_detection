#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 32
#define TRUE 1
#define FALSE 0

//Typedef Struct Stuff
struct trie_node;
typedef struct trie_node* pointerToTrieNode;

//Typedef Function Pointers for Connector
typedef int (*funcDynamicAllocateTrie_Node)(pointerToTrieNode*);
typedef void (*funcDynamicInitialiseTrie_Node)(pointerToTrieNode);
typedef pointerToTrieNode (*funcDynamicGetChildrenPtr)(pointerToTrieNode);
typedef int (*funcDynamicGetActualNumOfChildrenPtr)(pointerToTrieNode);
typedef pointerToTrieNode (*funcDynamicGetCertainChildPtr)(pointerToTrieNode, int);
typedef void (*funcDynamicDestroyNode)(pointerToTrieNode);
typedef void (*funcDynamicTraverseAndDestroy)(pointerToTrieNode);
typedef void (*funcDynamicDeleteANode)(pointerToTrieNode, int);
typedef void (*funcDynamicSetNumOfChildrenPtr)(pointerToTrieNode, int);
typedef void (*funcDynamicSetChildrenPtr)(pointerToTrieNode);
typedef int (*funcDynamicInsertAChild)(pointerToTrieNode, char*);
typedef void (*funcDynamicSetIs_FinalPtr)(pointerToTrieNode, int);
typedef int (*funcDynamicSearch)(pointerToTrieNode, char*, int);
typedef int (*funcDynamicGetNumOfChildrenPtr)(pointerToTrieNode);
typedef int (*funcDynamicGetIs_FinalPtr)(pointerToTrieNode);
typedef char* (*funcDynamicGetWordPtr)(pointerToTrieNode);
typedef void (*funcDynamicSetWordPtr)(pointerToTrieNode, char*);
typedef void (*funcDynamicSetActualNumOfChildrenPtr)(pointerToTrieNode, int);
typedef void (*funcDynamicRepointToWords)(pointerToTrieNode, int);
typedef size_t (*funcDynamicGetSizeOf)();


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

void setNumOfChildren(struct trie_node, int);
void setIs_Final(struct trie_node, int);
void setActualNumOfChildren(struct trie_node,int);
void setWord(struct trie_node, char*);
void setChildren(struct trie_node);


void setTrie_NodeValues(pointerToTrieNode, int, int, int, char*);
int doubleNumOfChildren(pointerToTrieNode);
int insertAChild(pointerToTrieNode, char*);
void moveChildren(pointerToTrieNode, int, int);
int searchPositionBinary(pointerToTrieNode, int, int, char*, int*);
void printTrieNode(pointerToTrieNode);
void destroyNode(pointerToTrieNode);
void traverseAndDestroy(pointerToTrieNode);
void deleteANode(pointerToTrieNode, int);
void moveChildrenDel(pointerToTrieNode, int, int);
void repointToWords(pointerToTrieNode, int);