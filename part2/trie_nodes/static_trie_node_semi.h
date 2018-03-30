#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 32
#define TRUE 1
#define FALSE 0


//Typedef Struct Stuff
struct static_trie_node;
typedef struct static_trie_node* pointerToStaticTrieNode;

//Typedef Function Pointers for Connector
typedef int (*funcStaticAllocateTrie_Node)(pointerToStaticTrieNode*);
typedef void (*funcStaticInitialiseTrie_Node)(pointerToStaticTrieNode);
typedef pointerToStaticTrieNode (*funcStaticGetChildrenPtr)(pointerToStaticTrieNode);
typedef int (*funcStaticGetActualNumOfChildrenPtr)(pointerToStaticTrieNode);
typedef pointerToStaticTrieNode (*funcStaticGetCertainChildPtr)(pointerToStaticTrieNode, int);
typedef void (*funcStaticDestroyNode)(pointerToStaticTrieNode);
typedef void (*funcStaticTraverseAndDestroy)(pointerToStaticTrieNode);
typedef void (*funcStaticDeleteANode)(pointerToStaticTrieNode, int);
typedef void (*funcStaticSetNumOfChildrenPtr)(pointerToStaticTrieNode, int);
typedef void (*funcStaticSetChildrenPtr)(pointerToStaticTrieNode);
typedef int (*funcStaticInsertAChild)(pointerToStaticTrieNode, char*);
typedef void (*funcStaticSetIs_FinalPtr)(pointerToStaticTrieNode, int);
typedef int (*funcStaticSearch)(pointerToStaticTrieNode, char*, int);
typedef int (*funcStaticGetNumOfChildrenPtr)(pointerToStaticTrieNode);
typedef int (*funcStaticGetIs_FinalPtr)(pointerToStaticTrieNode);
typedef int (*funcStaticGetIs_FinalPositionPtr)(pointerToStaticTrieNode, int);
typedef char* (*funcStaticGetWordPtr)(pointerToStaticTrieNode);
typedef char* (*funcStaticGetWordPositionPtr)(pointerToStaticTrieNode, int);
typedef void (*funcStaticSetWordPtr)(pointerToStaticTrieNode, char*);
typedef void (*funcStaticSetActualNumOfChildrenPtr)(pointerToStaticTrieNode, int);
typedef void (*funcStaticRepointToWords)(pointerToStaticTrieNode, int);
typedef size_t (*funcStaticGetSizeOf)();


int static_allocateTrie_Node(pointerToStaticTrieNode*);
void static_initialiseTrie_Node(pointerToStaticTrieNode);

//GETTERS
size_t static_getSizeOf();
int static_getNumOfChildrenPtr(pointerToStaticTrieNode);
int static_getIs_FinalPtr(pointerToStaticTrieNode);
int static_getIs_FinalPositionPtr(pointerToStaticTrieNode, int);
int static_getActualNumOfChildrenPtr(pointerToStaticTrieNode);
char* static_getWordPtr(pointerToStaticTrieNode);
char* static_getWordPositionPtr(pointerToStaticTrieNode, int);
pointerToStaticTrieNode static_getChildrenPtr(pointerToStaticTrieNode);
pointerToStaticTrieNode static_getCertainChildPtr(pointerToStaticTrieNode, int);

int static_getNumOfChildren(struct static_trie_node);
int static_getIs_Final(struct static_trie_node);
int static_getIs_FinalPosition(struct static_trie_node, int);
int static_getActualNumOfChildren(struct static_trie_node);
char* static_getWord(struct static_trie_node);
pointerToStaticTrieNode static_getChildren(struct static_trie_node);


//SETTERS
void static_setNumOfChildrenPtr(pointerToStaticTrieNode, int);
void static_setIs_FinalPtr(pointerToStaticTrieNode, int);
void static_setActualNumOfChildrenPtr(pointerToStaticTrieNode, int);
void static_setWordPtr(pointerToStaticTrieNode, char*);
void static_setChildrenPtr(pointerToStaticTrieNode);

void static_setNumOfChildren(struct static_trie_node, int);
void static_setIs_Final(struct static_trie_node, int);
void static_setActualNumOfChildren(struct static_trie_node, int);
void static_setWord(struct static_trie_node, char*);
void static_setChildren(struct static_trie_node);


void static_setTrie_NodeValues(pointerToStaticTrieNode, int, int, int, char*);
int static_doubleNumOfChildren(pointerToStaticTrieNode);
int static_insertAChild(pointerToStaticTrieNode, char*);
void static_moveChildren(pointerToStaticTrieNode, int, int);
int static_searchPositionBinary(pointerToStaticTrieNode, int, int, char*, int*);
int searchStatic(pointerToStaticTrieNode, char*, int);
int searchPositionBinary2(pointerToStaticTrieNode, int, int, char*);
void static_printTrieNode(pointerToStaticTrieNode);
void static_destroyNode(pointerToStaticTrieNode);
void static_traverseAndDestroy(pointerToStaticTrieNode);
void static_deleteANode(pointerToStaticTrieNode, int);
void static_moveChildrenDel(pointerToStaticTrieNode, int, int);


void static_repointToWords(pointerToStaticTrieNode, int);


void shrink(pointerToStaticTrieNode);
short getAbsoluteValue(short);