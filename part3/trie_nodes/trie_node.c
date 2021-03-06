#include "trie_node.h"

#if DYN_NODE == 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
A trie node. Contains:
-An integer indicating the number of its children
-A boolean value indicating whether its word is final for an n-gram or not
-The actual word
-An array of its children. It may be larger in size than the actual multitude of children
 */


struct trie_node
{
	int numOfChildren;/*Its values will be N, 2N, 4N, 8N and so on...*/
	int is_Final;
	int A_version, D_version;
	int actualNumOfChildren;
	char* word;
	pointerToTrieNode children;
};



int allocateTrie_Node(pointerToTrieNode* newNode)
{
	(*newNode)=malloc(sizeof(struct trie_node));
	if((*newNode)==NULL)
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}


void initialiseTrie_Node(pointerToTrieNode theNode)
{
	theNode->numOfChildren=0;
	theNode->is_Final=FALSE;
	theNode->A_version = -1;
	theNode->D_version = -1;
	theNode->actualNumOfChildren=0;
	theNode->word=NULL;
	theNode->children=NULL;
}


/*             						GETTERS  			    				*/
/*With pointer to node argument*/
size_t getSizeOf(){
	return sizeof(struct trie_node);
}

int getNumOfChildrenPtr(pointerToTrieNode node)
{
	return node->numOfChildren;
}

int getIs_FinalPtr(pointerToTrieNode node)
{
	return node->is_Final;
}

int getActualNumOfChildrenPtr(pointerToTrieNode node)
{
	return node->actualNumOfChildren;
}

char* getWordPtr(pointerToTrieNode node)
{
	return node->word;
}

pointerToTrieNode getChildrenPtr(pointerToTrieNode node)
{
	return node->children;
}

pointerToTrieNode getCertainChildPtr(pointerToTrieNode node, int position){
	return ((node->children != NULL)? (node->children)+position : NULL);
}

int getA_versionPtr(pointerToTrieNode node){
	return node->A_version;
}

int getD_versionPtr(pointerToTrieNode node){
	return node->D_version;
}

void getVersionsPtr(pointerToTrieNode node,int* A_version, int* D_version){
	*A_version = node->A_version;
	*D_version = node->D_version;
}

/*End of pointer to node argument getters*/

/*With node argument*/

int getNumOfChildren(struct trie_node node)
{
	return node.numOfChildren;
}

int getIs_Final(struct trie_node node)
{
	return node.is_Final;
}

int getActualNumOfChildren(struct trie_node node)
{
	return node.actualNumOfChildren;
}

char* getWord(struct trie_node node)
{
	return node.word;
}

pointerToTrieNode getChildren(struct trie_node node)
{
	return node.children;
}
/*End of node argument getters*/

/*                              END OF GETTERS                              */



/*             						SETTERS  			    				*/
/*With pointer to node argument*/

void setNumOfChildrenPtr(pointerToTrieNode node, int numofchildren)
{
	node->numOfChildren=numofchildren;
	return;
}

void setIs_FinalPtr(pointerToTrieNode node, int finality)
{
	node->is_Final=finality;
}

void setActualNumOfChildrenPtr(pointerToTrieNode node, int actualnumofchildren)
{
	node->actualNumOfChildren=actualnumofchildren;
}

void setWordPtr(pointerToTrieNode node, char* theWord)
{
	node->word=malloc((strlen(theWord)+1)*sizeof(char));
	strcpy(node->word,theWord);
}

void setChildrenPtr(pointerToTrieNode node)
{
	node->children=malloc((node->numOfChildren)*sizeof(struct trie_node));
}

void setA_versionPtr(pointerToTrieNode node, int version){
	node->A_version = version;
}

void setD_versionPtr(pointerToTrieNode node, int version){
	node->D_version = version;
}

void setVersionsPtr(pointerToTrieNode node, int A_version, int D_version){
	node->A_version = A_version;
	node->D_version = D_version;
}

/*End of pointer to node argument setters*/

/*With node argument*/

void setNumOfChildren(struct trie_node node, int numofchildren)
{
	node.numOfChildren=numofchildren;
}

void setIs_Final(struct trie_node node, int finality)
{
	node.is_Final=finality;
}

void setActualNumOfChildren(struct trie_node node,int actualnumofchildren)
{
	node.actualNumOfChildren=actualnumofchildren;
}

void setWord(struct trie_node node, char* theWord)
{
	node.word=malloc((strlen(theWord)+1)*sizeof(char));
	strcpy(node.word,theWord);
}

void setChildren(struct trie_node node)
{
	node.children=malloc((node.numOfChildren)*sizeof(struct trie_node));
}
/*End of node argument setters*/

/*                              END OF SETTERS                              */


void setTrie_NodeValues(pointerToTrieNode theNode, int numofchildren, int isfinal, int version, int actualnumofchildren, char* theword)
{
	theNode->numOfChildren=numofchildren;
	theNode->is_Final=isfinal;
	theNode->A_version = version;
	theNode->D_version = -1;
	theNode->actualNumOfChildren=actualnumofchildren;
	if(theword!=NULL)
	{
		theNode->word=malloc((strlen(theword)+1)*sizeof(char));
		strcpy(theNode->word,theword);
	}
	else
	{
		theNode->word=NULL;
	}
	if(numofchildren>0)
	{
		theNode->children=malloc(numofchildren*sizeof(struct trie_node));/*Allocate the space for the array of children*/
	}
	else
	{
		theNode->children=NULL;
	}
	return;
}


int doubleNumOfChildren(pointerToTrieNode theNode)
{/*Doubles the capacity of the array of children*/
	/*First double the number of children*/
	int newCapacity=(theNode->numOfChildren)*2;
	/*Now allocate a new array with the double capacity*/
	pointerToTrieNode newArray=malloc(newCapacity*sizeof(struct trie_node));
	/*Copy the existing array to the new one*/
	memmove(newArray,theNode->children,(theNode->numOfChildren*sizeof(struct trie_node)));
	/*Update the number of children*/
	theNode->numOfChildren=newCapacity;
	/*Free the previous memory*/
	free(theNode->children);/*You do not need to free the strings*/
	theNode->children=newArray;
	return TRUE;
}


int insertAChild(pointerToTrieNode father, char* wordToInsert)
{/*Inserts a new child in the node father*/
	int newActualNumOfChildren=father->actualNumOfChildren+1;
	int whereToInsert=0, result = -1;
	if(newActualNumOfChildren>father->numOfChildren)
	{
		doubleNumOfChildren(father);
	}
	if(father->actualNumOfChildren>0)
	{
		whereToInsert=searchPositionBinary(father->children,father->numOfChildren,father->actualNumOfChildren,wordToInsert, &result);
	}
	if(whereToInsert>=0)
	{
		if((whereToInsert>(father->actualNumOfChildren-1))||(father->actualNumOfChildren==0))
		{/*No need to move array positions*/
			;
		}
		else
		{
			moveChildren(father->children,whereToInsert,father->actualNumOfChildren);
		}
		/*Insert the child*/
		father->children[whereToInsert].word=malloc(sizeof(char)*(strlen(wordToInsert)+1));
		strcpy(father->children[whereToInsert].word, wordToInsert);
		father->children[whereToInsert].actualNumOfChildren=0;
		father->children[whereToInsert].numOfChildren=0;
		father->children[whereToInsert].is_Final=FALSE;
		father->children[whereToInsert].children=NULL;
		father->children[whereToInsert].A_version = -1;
		father->children[whereToInsert].D_version = -1;
		/*Update father*/
		father->actualNumOfChildren=newActualNumOfChildren;

		return whereToInsert;
	}

	return result;
}


int searchPositionLinear(pointerToTrieNode children, int numofchildren, int actualnumofchildren, char* wordToInsert)
{/*Implement a search algorithm to find the right position. For the time being we use a simple linear search*/
	int i;
	int compResult;
	for(i=0;i<actualnumofchildren;++i)
	{
		compResult=strcmp(wordToInsert,children[i].word);
		if(compResult<0)
		{/*The word should be inserted right here*/
			return i;
		}
		else if(compResult==0)
		{/*Word exists. Do not insert*/
			return -1;
		}
		else
		{
			;
		}
	}
	return i;
}


void moveChildren(pointerToTrieNode children, int positionToFree, int actualnumofchildren)
{/*Moves children to make space for a new child*/
	void* from=children;
	void* to=children;
	from+=(positionToFree*sizeof(struct trie_node));
	to+=((positionToFree+1)*sizeof(struct trie_node));
	int childrenToMove=actualnumofchildren-positionToFree;
	memmove(to,from,childrenToMove*sizeof(struct trie_node));
	return;
}


int searchPositionBinary(pointerToTrieNode children, int numofchildren, int actualnumofchildren, char* wordToInsert, int *exactPos)
{
	int i;
	int lowerBound=0;
	int upperBound=actualnumofchildren-1;
	int midPoint;
	int found=FALSE;
	int position;
	while(lowerBound<=upperBound)
	{
		midPoint=(upperBound+lowerBound)/2;
		if(strcmp(children[midPoint].word,wordToInsert)<0)
		{
			lowerBound=midPoint+1;
		}
		else if(strcmp(children[midPoint].word,wordToInsert)>0)
		{
			upperBound=midPoint-1;
		}
		else if(strcmp(children[midPoint].word,wordToInsert)==0)
		{
			break;
		}
	}
	if(strcmp(children[midPoint].word,wordToInsert)<0)
	{
		position=midPoint+1;
	}
	else if(strcmp(children[midPoint].word,wordToInsert)>0)
	{
		position=midPoint;
	}
	else if(strcmp(children[midPoint].word,wordToInsert)==0)
	{
		position=-1;
		if(exactPos!=NULL)
		{
			(*exactPos)=midPoint;
		}
	}
	return position;
}


void printTrieNode(pointerToTrieNode nodeToPrint)
{
	printf("Printing a trie node\n");
	printf("Num of children:%d\n",nodeToPrint->numOfChildren );
	printf("Real num of children:%d\n",nodeToPrint->actualNumOfChildren);
	if(nodeToPrint->word!=NULL)
	{
		printf("Word:%s\n", nodeToPrint->word);
	}
	else
	{
		printf("No word\n");
	}
	if(nodeToPrint->is_Final==TRUE)
	{
		printf("Final node\n");
		printf("A_version:%d\tD_version:%d\n", nodeToPrint->A_version, nodeToPrint->D_version);
	}
	else
	{
		printf("Not final node\n");
	}
	if(nodeToPrint->actualNumOfChildren>0)
	{
		printf("Going to print the children of this node\n" );
		int i;
		for(i=0;i<nodeToPrint->actualNumOfChildren;++i)
		{
			printf("Child's children:%d\n",nodeToPrint->children[i].numOfChildren );
			printf("Child's actual children:%d\n",nodeToPrint->children[i].actualNumOfChildren );
			printf("Child's finalness:%d\n",nodeToPrint->children[i].is_Final );
			printf("Child's word:%s\n",nodeToPrint->children[i].word );
			printf("\n");
		}
	}
	return;

}


void destroyNode(pointerToTrieNode node){
	if(node->children != NULL) free(node->children);
	if(node->word != NULL) free(node->word);
	free(node);
}

void clearNode(pointerToTrieNode node){
	if(node->children != NULL) free(node->children);
	if(node->word != NULL) free(node->word);
}

void traverseAndDestroy(pointerToTrieNode node){
    int i, size = node->actualNumOfChildren;

    if(node->actualNumOfChildren == 0){
        return;
    }

    for(i = size-1; i >= 0; i--){
        traverseAndDestroy(getCertainChildPtr(node, i));
        deleteANode(node, i);
    }
}


void deleteANode(pointerToTrieNode father, int childToDelete)
{
	if(father->children[childToDelete].children!=NULL)
	{/*If the child had children, release the memory they held*/
		free(father->children[childToDelete].children);
	}
	/*Free the word if it up to a certain length*/
	/*Now memmove the father's children to cover the space this child used to occupy*/
	if(father->children[childToDelete].word != NULL){
    	free(father->children[childToDelete].word);
    }
	moveChildrenDel(father->children, childToDelete, father->actualNumOfChildren);
	--(father->actualNumOfChildren);
	if(father->actualNumOfChildren==0)
	{
		free(father->children);
		father->children=NULL;
		father->numOfChildren=0;
	}
	return;
}


void moveChildrenDel(pointerToTrieNode children, int positionToOccupy, int actualnumofchildren)
{/*Moves children to make space for a new child*/
	void* from=children;
	void* to=children;
	from+=((positionToOccupy+1)*sizeof(struct trie_node));
	to+=(positionToOccupy*sizeof(struct trie_node));
	int childrenToMove=actualnumofchildren-positionToOccupy;
	memmove(to,from,childrenToMove*sizeof(struct trie_node));
	return;
}


void repointToWords(pointerToTrieNode children, int size){}


#endif