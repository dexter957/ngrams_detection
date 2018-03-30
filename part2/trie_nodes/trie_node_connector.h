//#include "trie_node.h"
//#include "semistatic_trie_node.h"
#include "semistatic_trie_node_v2.h"

#include "static_trie_node_semi.h"
//#include "static_trie_node_semi_improved.h"

#define DYNAMIC 0
#define STATIC 1


//Typedef Struct Stuff
struct connector;
typedef struct connector* connectorPtr;

//Typedef function pointers to "overload" the others
typedef int (*funcAllocateTrie_Node)(void**);
typedef void(*funcInitialiseTrie_Node)(void*);
typedef void* (*funcGetChildrenPtr)(void*);
typedef int (*funcGetActualNumOfChildrenPtr)(void*);
typedef void* (*funcGetCertainChildPtr)(void*, int);
typedef void (*funcDestroyNode)(void*);
typedef void (*funcTraverseAndDestroy)(void*);
typedef void (*funcDeleteANode)(void*, int);
typedef void (*funcSetNumOfChildrenPtr)(void*, int);
typedef void (*funcSetChildrenPtr)(void*);
typedef int (*funcInsertAChild)(void*, char*);
typedef void (*funcSetIs_FinalPtr)(void*, int);
typedef int (*funcGetNumOfChildrenPtr)(void*);
typedef int (*funcGetIs_FinalPtr)(void*);
typedef char* (*funcGetWordPtr)(void*);
typedef void (*funcSetWordPtr)(void*, char*);
typedef void (*funcSetActualNumOfChildrenPtr)(void*, int);
typedef void (*funcRepointToWords)(void*, int);
typedef size_t (*funcGetSizeOf)();



//Functions
connectorPtr createConnector();
void setConnector(connectorPtr, int);
void deleteConnector(connectorPtr);
int isStatic(connectorPtr);

funcAllocateTrie_Node AllocateTrie_Node(connectorPtr);
funcInitialiseTrie_Node InitialiseTrie_Node(connectorPtr);
funcDestroyNode DestroyNode(connectorPtr);
funcTraverseAndDestroy TraverseAndDestroy(connectorPtr);
funcDeleteANode DeleteANode(connectorPtr);
funcInsertAChild InsertAChild(connectorPtr);
funcRepointToWords RepointToWords(connectorPtr); 

funcSetWordPtr SetWordPtr(connectorPtr);
funcSetNumOfChildrenPtr SetNumOfChildrenPtr(connectorPtr);
funcSetChildrenPtr SetChildrenPtr(connectorPtr);
funcSetIs_FinalPtr SetIs_FinalPtr(connectorPtr);
funcSetActualNumOfChildrenPtr SetActualNumOfChildrenPtr(connectorPtr); 

funcGetChildrenPtr GetChildrenPtr(connectorPtr);
funcGetActualNumOfChildrenPtr GetActualNumOfChildrenPtr(connectorPtr);
funcGetCertainChildPtr GetCertainChildPtr(connectorPtr);
funcGetNumOfChildrenPtr GetNumOfChildrenPtr(connectorPtr);
funcGetIs_FinalPtr GetIs_FinalPtr(connectorPtr);
funcGetWordPtr GetWordPtr(connectorPtr);

size_t GetSizeOf(connectorPtr);


funcDynamicSearch DynamicSearch(connectorPtr);
funcStaticSearch StaticSearch(connectorPtr);
funcStaticGetIs_FinalPositionPtr GetIs_FinalPositionPtr(connectorPtr);
funcStaticGetWordPositionPtr GetWordPositionPtr(connectorPtr); 