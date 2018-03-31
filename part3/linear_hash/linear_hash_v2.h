#include "../trie_nodes/specs.h"

#if LIN_HASH == 1

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


LinearHTPtr createLinearHash(int, int);
BucketPtr createBuckets();
int destroyLinearHash(LinearHTPtr);
int destroyBuckets(BucketPtr);
int clearAndDestroyLinearHashD(LinearHTPtr);
int clearAndDestroyBucketsD(BucketPtr);
int clearAndDestroyLinearHashS(LinearHTPtr);
int clearAndDestroyBucketsS(BucketPtr);

int hashfunction(char*, int, int);

void* insertLHTrieNodeD(LinearHTPtr, char*);
void* insertLHTrieNodeS(LinearHTPtr, char*);
int splitHashTableD(LinearHTPtr);
int splitHashTableS(LinearHTPtr);

void* lookupTrieNodeD(LinearHTPtr, char*);
void* lookupBucketD(BucketPtr, char*, int*);
void* lookupTrieNodeS(LinearHTPtr, char*);
void* lookupBucketS(BucketPtr, char*, int*);

void removeTrieNodeD(LinearHTPtr, char*);
void removeFromBucketD(BucketPtr, char*);
void removeTrieNodeS(LinearHTPtr, char*);
void removeFromBucketS(BucketPtr, char*);

void shrinkHashTable(LinearHTPtr, void**, int*);

void printHashtableD(LinearHTPtr);
void printHashtableS(LinearHTPtr);

#endif