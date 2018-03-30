#include "static_trie_node_semi.h"


typedef struct static_trie_node{
	int numOfChildren;/*Its values will be N, 2N, 4N, 8N and so on...*/
    short *lengths, numOfWords;
    int actualNumOfChildren;
    char word[SIZE], *wordptr;
    pointerToStaticTrieNode children;
}static_trie_node;




int static_allocateTrie_Node(pointerToStaticTrieNode *node){
    if(((*node) = malloc(sizeof(static_trie_node))) == NULL) return FALSE;
	else return TRUE;
}


void static_initialiseTrie_Node(pointerToStaticTrieNode node){
	node->numOfChildren = 0;
	node->lengths = NULL;
	node->numOfWords = 0;
	node->actualNumOfChildren = 0;
	strcpy(node->word, "");
	node->wordptr = NULL;
	node->children = NULL;
}




/*                                  GETTERS                                 */
/*With pointer to node argument*/
size_t static_getSizeOf(){
    return sizeof(static_trie_node);
}
 
int static_getNumOfChildrenPtr(pointerToStaticTrieNode node){
    return node->numOfChildren;
}
 
int static_getIs_FinalPtr(pointerToStaticTrieNode node){
    return (node->lengths[node->numOfWords-1] > 0 ? TRUE : FALSE);
}

int static_getIs_FinalPositionPtr(pointerToStaticTrieNode node, int position){
    return (node->lengths[position] > 0 ? TRUE : FALSE);
}
 
int static_getActualNumOfChildrenPtr(pointerToStaticTrieNode node){
    return node->actualNumOfChildren;
}
 
char* static_getWordPtr(pointerToStaticTrieNode node){
    return node->wordptr;
}

char* static_getWordPositionPtr(pointerToStaticTrieNode node, int position){
    int offset = 0, i;
    short len;
    char *word;

    if(position < node->numOfWords){
    	len = getAbsoluteValue(node->lengths[position]);
        //printf("word len is %hi -> (%hi)\n", len, node->lengths[position]);
    	for(i = 0; i < position; i++){
    		offset += node->lengths[i];
    	}

    	if((word = malloc(sizeof(char)*(len+1))) == NULL) return NULL;
    	strncpy(word, (node->wordptr) + offset, len);
        word[len] = '\0';
    	return word;
    }
    else return NULL;
}
 
pointerToStaticTrieNode static_getChildrenPtr(pointerToStaticTrieNode node){
    return node->children;
}

pointerToStaticTrieNode static_getCertainChildPtr(pointerToStaticTrieNode node, int position){
    return ((node->children != NULL)? (node->children)+position : NULL);
}
 
/*End of pointer to node argument getters*/
 
/*With node argument*/
 
int static_getNumOfChildren(struct static_trie_node node){
    return node.numOfChildren;
}
 
int static_getIs_Final(struct static_trie_node node){
    return (node.lengths[node.numOfWords-1] > 0 ? TRUE : FALSE);
}

int static_getIs_FinalPosition(struct static_trie_node node, int position){
    return (node.lengths[position] > 0 ? TRUE : FALSE);
}
 
int static_getActualNumOfChildren(struct static_trie_node node){
    return node.actualNumOfChildren;
}
 
char* static_getWord(struct static_trie_node node){
    return node.wordptr;
}
 
pointerToStaticTrieNode static_getChildren(struct static_trie_node node){
    return node.children;
}
/*End of node argument getters*/
 
/*                              END OF GETTERS                              */



/*                                  SETTERS                                 */
/*With pointer to node argument*/
 
void static_setNumOfChildrenPtr(pointerToStaticTrieNode node, int numofchildren){
    node->numOfChildren=numofchildren;
    return;
}
 
void static_setIs_FinalPtr(pointerToStaticTrieNode node, int finality){
    if(finality == TRUE) node->lengths[node->numOfWords-1] = strlen(node->wordptr);
    else node->lengths[node->numOfWords-1] = strlen(node->wordptr) * (-1);
}
 
void static_setActualNumOfChildrenPtr(pointerToStaticTrieNode node, int actualnumofchildren){
    node->actualNumOfChildren=actualnumofchildren;
}
 
void static_setWordPtr(pointerToStaticTrieNode node, char* theWord){
	if(node->wordptr != NULL && node->wordptr != node->word){
    printf("3free trie node word\n");
		free(node->wordptr);
	}

    if(node->lengths != NULL) free(node->lengths);
    
    node->lengths = malloc(sizeof(short));
    node->lengths[0] =(short)(strlen(theWord) * (-1));
    //printf("%d\n", strlen(theWord));
    //printf("%hi\n", node->lengths[0]);
    node->numOfWords = 1;

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
 
void static_setChildrenPtr(pointerToStaticTrieNode node){
    
    node->children=malloc((node->numOfChildren)*sizeof(struct static_trie_node));
}
 
/*End of pointer to node argument setters*/
 
/*With node argument*/
 
void static_setNumOfChildren(struct static_trie_node node, int numofchildren){
    node.numOfChildren=numofchildren;
}
 
void static_setIs_Final(struct static_trie_node node, int finality){
    if(finality == TRUE) node.lengths[node.numOfWords-1] = strlen(node.wordptr);
    else node.lengths[node.numOfWords-1] = strlen(node.wordptr) * (-1);
}
 
void static_setActualNumOfChildren(struct static_trie_node node,int actualnumofchildren){
    node.actualNumOfChildren=actualnumofchildren;
}
 
void static_setWord(struct static_trie_node node, char* theWord){
	if(strcmp(node.word, "") == 0 && node.wordptr != NULL){
		free(node.wordptr);
	}

    if(node.lengths != NULL) free(node.lengths);

    node.lengths = malloc(sizeof(short));
    node.lengths[0] = strlen(theWord) * (-1);
    node.numOfWords = 1;

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
 
void static_setChildren(struct static_trie_node node){

    node.children=malloc((node.numOfChildren)*sizeof(struct static_trie_node));
}
/*End of node argument setters*/
 
/*                              END OF SETTERS                              */
 


void static_setTrie_NodeValues(pointerToStaticTrieNode node, int numOfChildren, int is_Final, int actualNumOfChildren, char *word){
	node->numOfChildren = numOfChildren;
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
		//node->wordptr = node->word;
	}

	if(numOfChildren > 0){

		node->children = malloc(sizeof(static_trie_node) * numOfChildren);
	}
	else{
		node->children = NULL;
	}

	if(word != NULL){
		node->numOfWords = 1;

		node->lengths = malloc(sizeof(short));
		if(is_Final) node->lengths[0] = strlen(word);
		else node->lengths[0] = strlen(word) * (-1);
	}
	else{
		node->numOfWords = 0;
		node->lengths = NULL;
	}
}


int static_doubleNumOfChildren(pointerToStaticTrieNode node){
	int newCapacity=(node->numOfChildren)*2;

    pointerToStaticTrieNode newArray=malloc(newCapacity*sizeof(static_trie_node));

    memmove(newArray,node->children,node->numOfChildren*sizeof(static_trie_node));
    node->numOfChildren=newCapacity;

    free(node->children);
    node->children=newArray;

    return TRUE;
}

 
int static_insertAChild(pointerToStaticTrieNode father, char* wordToInsert)
{/*Inserts a new child in the node father*/
    int newActualNumOfChildren=father->actualNumOfChildren+1;
    int whereToInsert=0, result = -1;
    if(newActualNumOfChildren>father->numOfChildren)
    {
        static_doubleNumOfChildren(father);
        static_repointToWords(father->children, father->actualNumOfChildren);
    }
    //whereToInsert=searchPositionLinear(father->children,father->numOfChildren,father->actualNumOfChildren,wordToInsert);
    if(father->actualNumOfChildren>0)
    {
        whereToInsert=static_searchPositionBinary(father->children,father->numOfChildren,father->actualNumOfChildren,wordToInsert, &result);
    }
    if(whereToInsert>=0)
    {
        if((whereToInsert>(father->actualNumOfChildren-1))||(father->actualNumOfChildren==0))
        {/*No need to move array positions*/
            //printf("No need to move the children\n");
        }
        else
        {
            static_moveChildren(father->children,whereToInsert,father->actualNumOfChildren);
            static_repointToWords((father->children)+whereToInsert+1, father->actualNumOfChildren-whereToInsert);
        }
        /*Insert the child*/

        if(strlen(wordToInsert) < SIZE){
        	strcpy(father->children[whereToInsert].word, wordToInsert);
        	father->children[whereToInsert].wordptr = father->children[whereToInsert].word;
        }
        else{
        	strcpy(father->children[whereToInsert].word, "");
        	father->children[whereToInsert].wordptr=wordToInsert;
        }

        father->children[whereToInsert].lengths = malloc(sizeof(short));
        father->children[whereToInsert].lengths[0] = strlen(father->children[whereToInsert].wordptr) * (-1);
        father->children[whereToInsert].numOfWords = 1;
        father->children[whereToInsert].actualNumOfChildren=0;
        father->children[whereToInsert].numOfChildren=0;
		father->children[whereToInsert].children=NULL;
        /*Update father*/
        father->actualNumOfChildren=newActualNumOfChildren;

        return whereToInsert;
    }

    return result;
}
 
 
void static_moveChildren(pointerToStaticTrieNode children, int positionToFree, int actualnumofchildren)
{/*Moves children to make space for a new child*/
    void* from=children;
    void* to=children;
    from+=(positionToFree*sizeof(struct static_trie_node));
    to+=((positionToFree+1)*sizeof(struct static_trie_node));
    //from+=positionToFree;
    //to=to+(positionToFree+1);
    int childrenToMove=actualnumofchildren-positionToFree;
    memmove(to,from,childrenToMove*sizeof(struct static_trie_node));
    return;
}
 
 
int static_searchPositionBinary(pointerToStaticTrieNode children, int numofchildren, int actualnumofchildren, char* wordToInsert, int *exactPos)
{
    int i;
    int lowerBound=0;
    int upperBound=actualnumofchildren-1;
    int midPoint;
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


int searchStatic(pointerToStaticTrieNode node, char *wordToInsert, int numOfVisits){
	int position = -1;
	short i, offset = 0;

    if(numOfVisits < node->numOfWords){
		if(getAbsoluteValue(node->lengths[numOfVisits]) == strlen(wordToInsert)){
			for(i = 0; i < numOfVisits; i++){
                offset += getAbsoluteValue(node->lengths[i]);
            }
			if(strncmp(node->wordptr+offset, wordToInsert, getAbsoluteValue(node->lengths[numOfVisits])) == 0){
				return -2;
			}
            else{
                return -1;
            }
		}
		else{
            return -1;
        }
	}
	else{
        if(node->children == NULL){
            return -1;
        }
        else{
            return searchPositionBinary2(node->children, node->numOfChildren, node->actualNumOfChildren, wordToInsert);
	   }
    }
}


int searchPositionBinary2(pointerToStaticTrieNode children, int numofchildren, int actualnumofchildren, char* wordToInsert)
{
    int i;
    int lowerBound=0;
    int upperBound=actualnumofchildren-1;
    int midPoint;
    int position = -1;
    int len = strlen(wordToInsert), test_len = 0;
    while(lowerBound<=upperBound)
    {
        midPoint=(upperBound+lowerBound)/2;
        test_len = getAbsoluteValue(children[midPoint].lengths[0]);
        if(strncmp(children[midPoint].wordptr,wordToInsert,test_len)<0)
        {
            lowerBound=midPoint+1;
        }
        else if(strncmp(children[midPoint].wordptr,wordToInsert,test_len)>0)
        {
            upperBound=midPoint-1;
        }
        else if(strncmp(children[midPoint].wordptr,wordToInsert,test_len)==0)
        {
        	if(test_len < len){
          		lowerBound=midPoint+1;
        	}
        	else{
	            position = midPoint;
	            break;
	        }
        }
    }

    return position;
}
 
 
void static_printTrieNode(pointerToStaticTrieNode nodeToPrint)
{
	int i, j, length, offset = 0;

    printf("Printing a trie node\n");
    printf("Num of children:%d\n",nodeToPrint->numOfChildren );
    printf("Real num of children:%d\n",nodeToPrint->actualNumOfChildren);
    if(nodeToPrint->wordptr!=NULL)
    {
        if(nodeToPrint->numOfWords == 1) printf("Word:%s\n", nodeToPrint->wordptr);
        else{
        	printf("Full:%s\n", nodeToPrint->wordptr);
        	printf("Words:");
        	for(i = 0; i < nodeToPrint->numOfWords; i++){
        		if(nodeToPrint->lengths[i] < 0) length = nodeToPrint->lengths[i] * (-1); 
        		else length = nodeToPrint->lengths[i];
        		printf("%.*s ", length, nodeToPrint->wordptr + offset);
        		offset += length;
        	}
        	printf("\n");
        }
    }
    else
    {
        printf("No word\n");
    }
     printf("Final node:");
    if(nodeToPrint->lengths==NULL)
    {
    	printf("unknown\n");
    }
    else
    {
        for(i = 0; i < nodeToPrint->numOfWords; i++){
        	if(nodeToPrint->lengths[i] < 0) printf("FALSE ");
        	else printf("TRUE ");
        }
    }
    printf("\n");
    if(nodeToPrint->actualNumOfChildren>0)
    {
        printf("Going to print the children of this node\n" );
        int i;
        for(i=0;i<nodeToPrint->actualNumOfChildren;++i)
        {
            printf("Child's children:%d\n",nodeToPrint->children[i].numOfChildren );
            printf("Child's actual children:%d\n",nodeToPrint->children[i].actualNumOfChildren );
            //printf("Child's finalness:%d\n",nodeToPrint->children[i].is_Final );
            for(j = 0; j < nodeToPrint->children[i].numOfWords; j++){
                if(nodeToPrint->children[i].lengths[j] < 0) printf("FALSE ");
                else printf("TRUE ");
            }
            printf("\n");
            printf("Child's word:%s\n",nodeToPrint->children[i].wordptr );
            printf("\n");
        }
    }
 
    return;
}


void static_destroyNode(pointerToStaticTrieNode node){

	if(node->children != NULL) free(node->children);
	if(node->wordptr != node->word) free(node->wordptr);
	if(node->lengths != NULL) free(node->lengths);
	free(node);
}

void static_traverseAndDestroy(pointerToStaticTrieNode node){
    int i, size = node->actualNumOfChildren;

    if(node->actualNumOfChildren == 0){
        return;
    }

    for(i = size-1; i >= 0; i--){
        static_traverseAndDestroy(static_getCertainChildPtr(node, i));
        static_deleteANode(node, i);
    }
}

//void static_traverseAndDestroy(pointerToStaticTrieNode node){ //Second version (that is probably faster)
//    int i, size = node->actualNumOfChildren;
//
//    if(node->actualNumOfChildren == 0){
//        static_destroyNode(node);
//        return;
//    }
//
//    for(i = size-1; i >= 0; i--){
//        static_traverseAndDestroy(static_getCertainChildPtr(node, i));
//    }
//    
//    static_destroyNode(node);
//}


void static_deleteANode(pointerToStaticTrieNode father, int childToDelete)
{
    if(father->children[childToDelete].children!=NULL)
    {/*If the child had children, release the memory they held*/

        free(father->children[childToDelete].children);
    }
    /*Free the word if it up to a certain length*/
    if(strcmp(father->children[childToDelete].word, "") == 0 && father->children[childToDelete].wordptr != NULL){
    	free(father->children[childToDelete].wordptr);
    }
    if(father->children[childToDelete].lengths != NULL){
    	free(father->children[childToDelete].lengths);
    }
    /*Now memmove the father's children to cover the space this child used to occupy*/
    static_moveChildrenDel(father->children, childToDelete, father->actualNumOfChildren);
    --(father->actualNumOfChildren);
    static_repointToWords((father->children)+childToDelete, father->actualNumOfChildren-childToDelete);
    if(father->actualNumOfChildren==0)
    {
        free(father->children);
        father->children=NULL;
    }
    return;
}
 
 
void static_moveChildrenDel(pointerToStaticTrieNode children, int positionToOccupy, int actualnumofchildren)
{/*Moves children to make space for a new child*/
    void* from=children;
    void* to=children;
    from+=((positionToOccupy+1)*sizeof(struct static_trie_node));
    to+=(positionToOccupy*sizeof(struct static_trie_node));
    int childrenToMove=actualnumofchildren-positionToOccupy;
    memmove(to,from,childrenToMove*sizeof(struct static_trie_node));
    return;
}


void static_repointToWords(pointerToStaticTrieNode children, int size){
	int i;

	for(i = 0; i < size; i++){
		if(strcmp(children[i].word, "") != 0){
			children[i].wordptr = children[i].word;
		}
	}
}


void shrink(pointerToStaticTrieNode node){
	int i;

	if(node->actualNumOfChildren == 0){
		if(node->children != NULL){
			free(node->children);
			node->children = NULL;
		}
		return;
	}

	for(i = 0; i < node->actualNumOfChildren; i++){
		shrink((node->children)+i);
	}

	if(node->actualNumOfChildren == 1){
		short *temp_lengths, temp_numOfWords;
		int temp_actualNumOfChildren, temp_numOfChildren;
		pointerToStaticTrieNode child, temp_children;

		child = node->children;

		temp_numOfWords = node->numOfWords + child->numOfWords;

		temp_lengths = malloc(sizeof(short)*temp_numOfWords);
		memmove(temp_lengths, node->lengths, sizeof(short)*node->numOfWords);
		memmove(temp_lengths+node->numOfWords, child->lengths, sizeof(short)*child->numOfWords);
		free(node->lengths);
		node->lengths = temp_lengths;
		node->numOfWords = temp_numOfWords;

		if(strlen(node->wordptr)+strlen(child->wordptr) < SIZE){
			strcat(node->wordptr, child->wordptr);
		}
		else{

			char *temp_word = malloc(sizeof(char)*(strlen(node->wordptr)+strlen(child->wordptr)+1));
			strcpy(temp_word, node->wordptr);
			strcat(temp_word, child->wordptr);

			if(node->wordptr == node->word)	strcpy(node->word, "");
			else free(node->wordptr);

			node->wordptr = temp_word;
		}

		temp_children = child->children;
		child->children = NULL;
		temp_numOfChildren = child->numOfChildren;
		temp_actualNumOfChildren = child->actualNumOfChildren;

		static_deleteANode(node, 0);

		node->children = temp_children;
		node->numOfChildren = temp_numOfChildren;
		node->actualNumOfChildren = temp_actualNumOfChildren;
	}
}


short getAbsoluteValue(short value){
	return ((value < 0)? value*(-1) : value);
}