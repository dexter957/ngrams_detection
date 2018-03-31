#include "murmurHash.h"
#include <stdio.h>
#include <stdlib.h>


unsigned int murmur2(const void* key, int length, const unsigned int seed)
{
	const unsigned int m = 0x5bd1e995;
	const int r=24;
	/*Initialise hash to a random value*/
	unsigned int h = seed ^ length;
	/*Mix 4 bytes at a time into the hash*/
	const unsigned char* data = (const unsigned char*)key;
	while(length>=4)
	{
		unsigned int k = *(unsigned int *)data;
		k*=m;
		k^=k>>r;
		k*=m;

		h*=m;
		h^=k;

		data +=4;
		length -=4;
	}
	/*Handle the last few bytes of the input array*/
	switch(length)
	{
		case 3:h^=data[2]<<16;
		case 2:h^=data[1]<<8;
		case 1:h^=data[0];
			   h*=m;	
	}
	h^=h>>13;
	h*=m;
	h^=h>>15;
	return h;

}


uint32_t murmur3_32(const void* key, int length, const unsigned int seed)
{
	uint32_t h=seed;
	const uint8_t *data=(const uint8_t*)key;
	if(length>3)
	{
		const uint32_t* key_x4=(const uint32_t*)key;
		size_t i=length>>2;
		do
		{
			uint32_t k=*key_x4++;
			k *=0xcc9e2d51;
			k=(k<<15)|(k>>17);
			k*=0x1b873593;
			h^=k;
			h=(h<<13)|(h>>19);
			h=(h*5)+0xe6546b64;
		} while (--i);
		data=(const uint8_t*)key_x4;
	}
	if(length&3)
	{
		size_t i=length&3;
		uint32_t k=0;
		data=&data[i-1];
		do{
			k<<=8;
			k|=*data--;
		}while(--i);
		k*=0xcc9e2d51;
		k=(k<<15)|(k>>17);
		k*=0x1b873593;
		h^=k;
	}
	h^=length;
	h^=h>>16;
	h*=0x85ebca6b;
	h^=h>>13;
	h*=0xc2b2ae35;
	h^h>>16;
	return h;
}