#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "bloomFilter.h"



struct bloom_filter
{
	int k;     			/*Number of hashes*/
	int m;     			/*Length in bits*/
	int salt;  			/*Salt for the hash function*/
	int *bitSet;		/*The actual bit buffer*/
	int bufferLength;	/*The buffer length*/
};


void bloom_filterInit(pointerToBloomFilter *bloomF,/*int hashes, int lengthInBits,*/ int n, double p)
{/*Bloom filetr initilisation*/
	(*bloomF)=malloc(sizeof(struct bloom_filter));
	/*This code section decides how many hush functions the bloom filter should have, based on the number of elements it expects*/

	double bits=-(n*log(p))/(pow(log(2),2));
	(*bloomF)->m=(int)(bits<0 ? (bits-0.5) : (bits+0.5));
	double hashes=(((*bloomF)->m)/n)*log(2);
	(*bloomF)->k=(int)(hashes<0 ? (hashes-0.5) : (hashes+0.5));
	
	srand(time(NULL));	
	(*bloomF)->salt=rand();
	long int sizeofInt=sizeof(int);
	int bytes=((*bloomF)->m)/BITSINBYTE;

	if((((*bloomF)->m)%BITSINBYTE)!=0)
	{/*m%8!=0*/
		bytes+=1;
	}
	(*bloomF)->bufferLength=bytes/sizeofInt;
	if((bytes%sizeofInt)!=0)
	{
		((*bloomF)->bufferLength)+=1;
	}
	(*bloomF)->bitSet=malloc((*bloomF)->bufferLength*sizeofInt);
	/*Initialise the bit set to zero*/
	int i;
	for(i=0;i<(*bloomF)->bufferLength;++i)
	{
		(*bloomF)->bitSet[i]=0;
	}
}


int bloom_filterCheckAndAdd(pointerToBloomFilter bloomF,char* ngram )
{/*Returns false in case the element is not in the filter, and true in case it is*/
	int ngramLength=strlen(ngram);
	int i;
	int hashHits=0;
	int positionInBitSet=0;
	//register unsigned int a=murmur2(ngram, ngramLength, bloomF->salt);
	register unsigned int a=murmur3_32(ngram, ngramLength, bloomF->salt);
	register unsigned int b=a;
	register unsigned int x;
	for(i=0;i<bloomF->k;++i)
	{
		x=(a+i*b)%(bloomF->m);
		if(testBit(x,bloomF->bitSet)==TRUE)/*Check*/
		{
			++hashHits;
		}
		else
		{/*Add*/
			setBit(x,bloomF->bitSet);
		}
	}
	return (hashHits==bloomF->k);
}

void bloom_filterDelete(pointerToBloomFilter bloomF)
{
	free(bloomF->bitSet);
}

void bloom_filterPrint(pointerToBloomFilter bloomF)
{/*Just for checking and nothing more*/
	int i;
	int counter=0;
	printf("Number of hashes:%d\n",bloomF->k);
	printf("Number of bits:%d\n",bloomF->m);
	printf("Number bufferLength:%d\n",bloomF->bufferLength);
	printf("Salt:%d\n",bloomF->salt);
	//printf("Dummy k %d\n",bloomF->dK );
//	printf("Dummy m %d\n",bloomF->dM );
	//for(i=0;i<bloomF->m;++i)
	//{
	//	printf(" %d ",testBit(i,bloomF->bitSet) );
//		++counter;
	//	if(counter==10)
	//	{
	//		counter=0;
	//		printf("\n");
	//	}
//	}
//	printf("\n");
}





/*Basic binary operations*/

void setBit(int k, int* array)
{
	array[k/32] |=1<< (k%32);
}

void clearBit(int k, int* array)
{
	array[k/32] &=~(1<<(k%32));
}

int testBit(int k, int* array)
{
	return ((array[k/32] & (1<<(k%32) ))!=0);
}

