#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../trie_nodes/trie_node_connector.h"
//#include "trie_node.h"
//#include "semistatic_trie_node.h"
//#include "semistatic_trie_node_v2.h"


#define BUCKET_SIZE 16


struct linearHashTable;
typedef struct linearHashTable* LinearHTPtr;
struct Bucket;
typedef struct Bucket* BucketPtr;


LinearHTPtr createLinearHash(int, connectorPtr);
BucketPtr createBuckets();
int destroyLinearHash(LinearHTPtr);
int destroyBuckets(BucketPtr);
int clearAndDestroyLinearHash(LinearHTPtr, connectorPtr);
int clearAndDestroyBuckets(BucketPtr, connectorPtr);

int hashfunction(char*, int, int);

//int insertLHTrieNode(LinearHTPtr, void*, connectorPtr);
void* insertLHTrieNode(LinearHTPtr, char*, connectorPtr);
int checkAvailabilityAndMove(BucketPtr, int, void*);
int checkAvailabilityAndMoveRecursive(BucketPtr);
int pushNewBucket(BucketPtr);
int splitHashTable(LinearHTPtr, connectorPtr);

void* lookupTrieNode(LinearHTPtr, char*, connectorPtr);
void* lookupBucket(BucketPtr, char*, connectorPtr, int*);

void removeTrieNode(LinearHTPtr, char*, connectorPtr);
void removeFromBucket(BucketPtr, char*, connectorPtr);

void shrinkHashTable(LinearHTPtr);

void printHashtable(LinearHTPtr, connectorPtr);