
#include "murmurHash.h"


#define TRUE 1
#define FALSE 0
#define BITSINBYTE 8
#define ELEMS 100000
#define ERRORPROB 0.00000000000001
#define BLOOM_CONST 0x9747b28c


typedef struct bloom_filter* pointerToBloomFilter;


void bloom_filterInit(pointerToBloomFilter *bloomF,/*int hashes, int lengthInBits,*/  int n, double p);
int bloom_filterCheckAndAdd(pointerToBloomFilter bloomF,char* ngram );
void bloom_filterDelete(pointerToBloomFilter bloomF);
void bloom_filterPrint(pointerToBloomFilter bloomF);


/****************************************************************************************************/
/*Basic binary operations*/
void setBit(int k, int* array);
void clearBit(int k, int* array);
int testBit(int k, int* array);