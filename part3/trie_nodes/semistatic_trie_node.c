#include "semistatic_trie_node.h"

#if DYN_NODE == 2
/*
A trie node. Contains:
-An integer indicating the number of its children
-A boolean value indicating whether its word is final for an n-gram or not
-The actual word either in the word array or indicated by the word pointer
-An array of its children. It may be larger in size than the actual multitude of children
 */


typedef struct trie_node{
	int numOfChildren;/*Its values will be N, 2N, 4N, 8N and so on...*/
    int is_Final;
    int A_version, D_version;
    int actualNumOfChildren;
    char word[SIZE], *wordptr;
    pointerToTrieNode children;
}trie_node;



int allocateTrie_Node(pointerToTrieNode *node){
	if(((*node) = malloc(sizeof(trie_node))) == NULL) return FALSE;
	else return TRUE;
}


void initialiseTrie_Node(pointerToTrieNode node){
	node->numOfChildren = 0;
	node->is_Final = FALSE;
	node->A_version = -1;
    node->D_version = -1;
    node->actualNumOfChildren = 0;
	strcpy(node->word, "");
	node->wordptr = NULL;
	node->children = NULL;
}


/*                                  GETTERS                                 */
/*With pointer to node argument*/
size_t getSizeOf(){
    return sizeof(trie_node);
}
 
int getNumOfChildrenPtr(pointerToTrieNode node){
    return node->numOfChildren;
}
 
int getIs_FinalPtr(pointerToTrieNode node){
    return node->is_Final;
}
 
int getActualNumOfChildrenPtr(pointerToTrieNode node){
    return node->actualNumOfChildren;
}
 
char* getWordPtr(pointerToTrieNode node){
    return node->wordptr;
}
 
pointerToTrieNode getChildrenPtr(pointerToTrieNode node){
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
 
int getNumOfChildren(struct trie_node node){
    return node.numOfChildren;
}
 
int getIs_Final(struct trie_node node){
    return node.is_Final;
}
 
int getActualNumOfChildren(struct trie_node node){
    return node.actualNumOfChildren;
}
 
char* getWord(struct trie_node node){
    return node.wordptr;
}
 
pointerToTrieNode getChildren(struct trie_node node){
    return node.children;
}
/*End of node argument getters*/
 
/*                              END OF GETTERS                              */



/*                                  SETTERS                                 */
/*With pointer to node argument*/
 
void setNumOfChildrenPtr(pointerToTrieNode node, int numofchildren){
    node->numOfChildren=numofchildren;
    return;
}
 
void setIs_FinalPtr(pointerToTrieNode node, int finality){
    node->is_Final=finality;
}
 
void setActualNumOfChildrenPtr(pointerToTrieNode node, int actualnumofchildren){
    node->actualNumOfChildren=actualnumofchildren;
}
 
void setWordPtr(pointerToTrieNode node, char* theWord){
	if(node->wordptr != NULL && strcmp(node->word, "") == 0){
		free(node->wordptr);
	}

	if(strlen(theWord) < SIZE){
		strcpy(node->word, theWord);
		node->wordptr = node->word;
	}
	else{
    	node->wordptr=malloc((strlen(theWord)+1)*sizeof(char));
    	strcpy(node->wordptr, theWord);
    	strcpy(node->word, "");
	}
}
 
void setChildrenPtr(pointerToTrieNode node){
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
 
void setNumOfChildren(struct trie_node node, int numofchildren){
    node.numOfChildren=numofchildren;
}
 
void setIs_Final(struct trie_node node, int finality){
    node.is_Final=finality;
}
 
void setActualNumOfChildren(struct trie_node node,int actualnumofchildren){
    node.actualNumOfChildren=actualnumofchildren;
}
 
void setWord(struct trie_node node, char* theWord){
	if(strcmp(node.word, "") == 0 && node.wordptr != NULL){
		free(node.wordptr);
	}

	if(strlen(theWord) < SIZE){
		strcpy(node.word, theWord);
		node.wordptr = node.word;
	}
	else{
    	node.wordptr=malloc((strlen(theWord)+1)*sizeof(char));
    	strcpy(node.wordptr, theWord);
    	strcpy(node.word, "");
	}
}
 
void setChildren(struct trie_node node){
    node.children=malloc((node.numOfChildren)*sizeof(struct trie_node));
}
/*End of node argument setters*/
 
/*                              END OF SETTERS                              */
 


void setTrie_NodeValues(pointerToTrieNode node, int numOfChildren, int is_Final, int version, int actualNumOfChildren, char *word){
	node->numOfChildren = numOfChildren;
	node->is_Final = is_Final;
    node->A_version = version;
    node->D_version = -1;
	node->actualNumOfChildren = actualNumOfChildren;
	
	if(word != NULL){
		if(strlen(word) < SIZE){
			strcpy(node->word, word);
			node->wordptr = node->word;
		} 	
		else{
			node->wordptr = malloc(sizeof(char)*(strlen(word) + 1));
			strcpy(node->wordptr, word);
			strcpy(node->word, "");
		}
	}
	else{
		strcpy(node->word, "");
		node->wordptr = NULL;
	}

	if(numOfChildren > 0){
		node->children = malloc(sizeof(trie_node) * numOfChildren);
	}
	else{
		node->children = NULL;
	}
}


int doubleNumOfChildren(pointerToTrieNode node){
	int newCapacity=(node->numOfChildren)*2;
    pointerToTrieNode newArray=malloc(newCapacity*sizeof(trie_node));

    memmove(newArray,node->children,node->numOfChildren*sizeof(trie_node));
    node->numOfChildren=newCapacity;
    free(node->children);
    node->children=newArray;

    return TRUE;
}

 
int insertAChild(pointerToTrieNode father, char* wordToInsert)
{/*Inserts a new child in the node father*/
    int newActualNumOfChildren=father->actualNumOfChildren+1;
    int whereToInsert=0, result = -1;
    if(newActualNumOfChildren>father->numOfChildren)
    {
        doubleNumOfChildren(father);
        repointToWords(father->children, father->actualNumOfChildren);
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
            repointToWords((father->children)+whereToInsert+1, father->actualNumOfChildren-whereToInsert);
        }
        /*Insert the child*/

        if(strlen(wordToInsert) < SIZE){
        	strcpy(father->children[whereToInsert].word, wordToInsert);
        	father->children[whereToInsert].wordptr = father->children[whereToInsert].word;
        }
        else{
        	strcpy(father->children[whereToInsert].word, "");
        	father->children[whereToInsert].wordptr=malloc(sizeof(char)*(strlen(wordToInsert)+1));
            strcpy(father->children[whereToInsert].wordptr, wordToInsert);
        }
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
 
 
void moveChildren(pointerToTrieNode children, int positionToFree, int actualnumofchildren)
{/*Moves children to make space for a new child*/
    void* from=children;
    void* to=children;
    from+=(positionToFree*sizeof(struct trie_node));
    to+=((positionToFree+1)*sizeof(struct trie_node));
    //from+=positionToFree;
    //to=to+(positionToFree+1);
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
        if(strcmp(children[midPoint].wordptr,wordToInsert)<0)
        {
            lowerBound=midPoint+1;
        }
        else if(strcmp(children[midPoint].wordptr,wordToInsert)>0)
        {
            upperBound=midPoint-1;
        }
        else if(strcmp(children[midPoint].wordptr,wordToInsert)==0)
        {
            break;
        }
    }
    if(strcmp(children[midPoint].wordptr,wordToInsert)<0)
    {
        position=midPoint+1;
    }
    else if(strcmp(children[midPoint].wordptr,wordToInsert)>0)
    {
        position=midPoint;
    }
    else if(strcmp(children[midPoint].wordptr,wordToInsert)==0)
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
    if(nodeToPrint->wordptr!=NULL)
    {
        printf("Word:%s\n", nodeToPrint->wordptr);
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
            printf("Child's word:%s\n",nodeToPrint->children[i].wordptr );
            printf("\n");
        }
    }
 
    return;
}


void destroyNode(pointerToTrieNode node){
    if(node->children != NULL) free(node->children);
    if(node->wordptr != node->word) free(node->wordptr);
    free(node);
}


void clearNode(pointerToTrieNode node){
    if(node->children != NULL) free(node->children);
    if(node->wordptr != node->word) free(node->wordptr);
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
    if(strcmp(father->children[childToDelete].word, "") == 0 && father->children[childToDelete].wordptr != NULL){
    	free(father->children[childToDelete].wordptr);
    }
    /*Now memmove the father's children to cover the space this child used to occupy*/
    moveChildrenDel(father->children, childToDelete, father->actualNumOfChildren);
    --(father->actualNumOfChildren);
    repointToWords((father->children)+childToDelete, father->actualNumOfChildren-childToDelete);
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


void repointToWords(pointerToTrieNode children, int size){
	int i;

	for(i = 0; i < size; i++){
		if(strcmp(children[i].word, "") != 0){
			children[i].wordptr = children[i].word;
		}
	}
}


#endif