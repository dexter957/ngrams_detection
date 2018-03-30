#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
typedef int (*funcDynamicSearch)(pointerToTrieNode, int, int, char*, int*);
typedef int (*funcDynamicGetNumOfChildrenPtr)(pointerToTrieNode);
typedef int (*funcDynamicGetIs_FinalPtr)(pointerToTrieNode);
typedef void (*funcDynamicSetWordPtr)(pointerToTrieNode, char*);
typedef char* (*funcDynamicGetWordPtr)(pointerToTrieNode);
typedef void (*funcDynamicSetActualNumOfChildrenPtr)(pointerToTrieNode, int);
typedef void (*funcDynamicRepointToWords)(pointerToTrieNode, int);
typedef size_t (*funcDynamicGetSizeOf)();


int allocateTrie_Node(pointerToTrieNode* newNode);
void initialiseTrie_Node(pointerToTrieNode theNode);
void setTrie_NodeValues(pointerToTrieNode theNode, int numofchildren, int isfinal, int actualnumofchildren, char* theword);
int doubleNumOfChildren(pointerToTrieNode theNode);
int insertAChild(pointerToTrieNode father, char* wordToInsert);
int searchPositionLinear(pointerToTrieNode children, int numofchildren, int actualnumofchildren, char* wordToInsert);
int searchPositionBinary(pointerToTrieNode children, int numofchildren, int actualnumofchildren, char* wordToInsert, int *exactPos);
void moveChildren(pointerToTrieNode children, int positionToFree, int actualnumofchildren);
void printTrieNode(pointerToTrieNode nodeToPrint);
void destroyNode(pointerToTrieNode node);
void traverseAndDestroy(pointerToTrieNode node);
void deleteANode(pointerToTrieNode father, int childToDelete);
void moveChildrenDel(pointerToTrieNode children, int positionToOccupy, int actualnumofchildren);
void repointToWords(pointerToTrieNode children, int size);

/*             						GETTERS  			    				*/
/*With pointer to node argument*/
size_t getSizeOf();
int getNumOfChildrenPtr(pointerToTrieNode node);
int getIs_FinalPtr(pointerToTrieNode node);
int getActualNumOfChildrenPtr(pointerToTrieNode node);
char* getWordPtr(pointerToTrieNode node);
pointerToTrieNode getChildrenPtr(pointerToTrieNode node);
pointerToTrieNode getCertainChildPtr(pointerToTrieNode node, int position);
/*End of pointer to node argument getters*/

/*With node argument*/
int getNumOfChildren(struct trie_node node);
int getIs_Final(struct trie_node node);
int getActualNumOfChildren(struct trie_node node);
char* getWord(struct trie_node node);
pointerToTrieNode getChildren(struct trie_node node);
/*End of node argument getters*/

/*                              END OF GETTERS                              */

/*             						SETTERS  			    				*/
/*With pointer to node argument*/

void setNumOfChildrenPtr(pointerToTrieNode node, int numofchildren);
void setIs_FinalPtr(pointerToTrieNode node, int finality);
void setActualNumOfChildrenPtr(pointerToTrieNode node, int actualnumofchildren);
void setWordPtr(pointerToTrieNode node, char* theWord);
void setChildrenPtr(pointerToTrieNode node);
/*End of pointer to node argument setters*/

/*With node argument*/
void setNumOfChildren(struct trie_node node, int numofchildren);
void setIs_Final(struct trie_node node, int finality);
void setActualNumOfChildren(struct trie_node node,int actualnumofchildren);
void setWord(struct trie_node node, char* theWord);
void setChildren(struct trie_node node);
/*End of node argument setters*/

/*                               END OF SETTERS                              */