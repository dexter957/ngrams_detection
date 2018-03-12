#include "binHeapAr.h"
#include "stdio.h"
#include <stdlib.h>
#include <string.h>


struct binaryHeapArray
{
	void** heapArray;
	int heapLength;
	int heapOccupied;
};

int binaryHeapArrayInit(pointerToBinHeapArray *heap)
{
	(*heap)=malloc(sizeof(struct binaryHeapArray));
	if((*heap)!=NULL)
	{
		(*heap)->heapArray=NULL;
		(*heap)->heapLength=0;
		(*heap)->heapOccupied=0;
		return TRUE;
	}
	return FALSE;
}

void set_HeapLength(pointerToBinHeapArray heap, int length)
{
	heap->heapLength=length;
}

void set_HeapOccupied(pointerToBinHeapArray heap, int occupied)
{
	heap->heapOccupied=occupied;
}

void set_HeapArray(pointerToBinHeapArray heap)
{
	heap->heapArray=malloc((heap->heapLength)*sizeof(void*));
}

int get_HeapLength(pointerToBinHeapArray heap)
{
	return heap->heapLength;
}

int get_HeapOccupied(pointerToBinHeapArray heap)
{
	return heap->heapOccupied;
}


void doubleHeapArrayCapacity(pointerToBinHeapArray heap)
{
	void** newArray=malloc(((heap->heapLength)*2)*sizeof(void*));
	memmove(newArray,heap->heapArray,(heap->heapLength)*sizeof(void*));
	free(heap->heapArray);
	heap->heapArray=newArray;
	heap->heapLength=(heap->heapLength)*2;
}

int insertInBinaryHeapArray(pointerToBinHeapArray heap, void* pointer)
{
	if((heap->heapOccupied+1)>(heap->heapLength))
	{
		doubleHeapArrayCapacity(heap);
	}
	if(heap->heapOccupied==0)
	{
		heap->heapArray[0]=pointer;
	}
	else
	{
		int k=heap->heapOccupied+1;
		heap->heapArray[k-1]=pointer;
		while(heap->heapArray[(k/2)-1]<heap->heapArray[k-1] && k >= 2)
		{
			void* temp=heap->heapArray[(k/2)-1];
			heap->heapArray[(k/2)-1]=heap->heapArray[k-1];
			heap->heapArray[k-1]=temp;
			k=k/2;
		}

	}
	++(heap->heapOccupied);
	return TRUE;
}

void printBinaryHeapArray(pointerToBinHeapArray heap)
{
	printf("The heap array has %d elements length\n",heap->heapLength );
	printf("The heap array has %d elements\n",heap->heapOccupied );
	int i;
	for(i=0;i<heap->heapOccupied;++i)
	{
		printf("%d:%p\n",i+1,heap->heapArray[i] );
	}
}


int searchBinaryHeapArray(pointerToBinHeapArray heap, void* elem)
{
	if(heap->heapArray[0]==elem)
	{
		return TRUE; /*The element is in the top of the heap*/
	}
	else if(heap->heapArray[0]<elem)
	{
		return FALSE; /*The element is not even in the heap*/
	}
	else
	{/*Search left and/or right subtrees*/
		int i=1;
		int ret;
		if((2*i)<=heap->heapOccupied)
		{
			ret=searchSubTree(heap,elem,2*i);/*Search left subtree*/
			if((ret==FALSE)&&(((2*i)+1)<=heap->heapOccupied))
			{
				ret=searchSubTree(heap,elem,2*i+1);/*Search right subtree*/
			}
			return ret;
		}
		else
		{
			return FALSE;
		}
	}
}

int searchSubTree(pointerToBinHeapArray heap, void* elem,int i)
{
	
	if(heap->heapArray[i-1]==elem)
	{
		return TRUE; /*The element is in here in the heap*/
	}
	else if(heap->heapArray[i-1]<elem)
	{
		return FALSE; /*The element is certainly not in this subtree*/
	}
	else
	{/*Search left and/or right subtrees*/
		int ret;
		if((2*i)<=heap->heapOccupied)
		{
			ret=searchSubTree(heap,elem,2*i);/*Search left subtree*/
			if((ret==FALSE)&&(((2*i)+1)<=heap->heapOccupied))
			{
				ret=searchSubTree(heap,elem,2*i+1);/*Search right subtree*/
			}
			return ret;
		}
		else
		{
			return FALSE;
		}
	}
}


void destroyBinaryHeapArray(pointerToBinHeapArray heap)
{
	free(heap->heapArray);
	free(heap);
}



/*
	if(heap->heapArray[i-1]==elem)
	{
		return TRUE;
	}
	else if(heap->heapArray[i-1]<elem)
	{
		return FALSE;
	}
	else
	{
		if((2*i)<=heap->heapOccupied)
		{
			int ret=checkLeftSubTree(heap,2*i,elem);
			if((ret==FALSE)&&((2*i+1)<=heap->heapOccupied))
			{
				return checkRightSubTree(heap,2*i+1,elem);
			}
			else
			{
				return ret;
			}
		}
		else
		{
			return FALSE;
		}
	}
*/