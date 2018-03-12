#include "trie.h"

#define N 16


typedef struct Trie{
	pointerToTrieNode root;
}Trie;



TriePtr init_trie(){
	TriePtr temp;
	if((temp = malloc(sizeof(Trie))) == NULL){
		return NULL;
	}
	temp->root = NULL;

	if(!allocateTrie_Node(&(temp->root))){
		clear_trie(temp); 
		return NULL;
	}

	initialiseTrie_Node(temp->root);

	return temp;
}


void clear_trie(TriePtr trie){
	if(trie->root!=NULL){
		traverse_and_clear(trie->root);
	}
	if(trie!=NULL) free(trie);
}

void traverse_and_clear(pointerToTrieNode node){
	int i, size; 

	if(getChildrenPtr(node) == NULL){
		return;
	}

	size = getActualNumOfChildrenPtr(node);
	for(i = size - 1; i >=0; i--){
		traverse_and_clear(getCertainChildPtr(node, i));
		deleteANode(node, i);
	}
}




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////INSERT/////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void insert_ngram(TriePtr trie, char **ngram, int numOfTokens){
	search_and_insert(trie->root, ngram, numOfTokens);
}

//The insertAChild function returns a position regardless if an actual node was created or not.
//If a node was created, it returns its position in the children array
//If not, it returns the position of the existing node in the children array where that word of the ngram resides 
void search_and_insert(pointerToTrieNode root, char **ngram, int numOfTokens){
	
	int i, position;
	pointerToTrieNode current = root;

	for(i = 0; i < numOfTokens; i++){
		if(getChildrenPtr(current) == NULL){
			setNumOfChildrenPtr(current, N);
			setChildrenPtr(current);
		}
		position = insertAChild(current, ngram[i]);

		current = getCertainChildPtr(current, position);
	}

	setIs_FinalPtr(current, TRUE);
}




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////DELETE/////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void delete_ngram(TriePtr trie, char **ngram, int numOfTokens){
	search_and_delete(trie->root, ngram, numOfTokens, trie);
}

void search_and_delete(pointerToTrieNode root, char **ngram, int numOfTokens, TriePtr trie){
	int i, position, result, *positions, toDelete = TRUE;
	pointerToTrieNode current = root, *history, children;

	history = malloc(sizeof(pointerToTrieNode)*(numOfTokens));
	positions = malloc(sizeof(int)*(numOfTokens));

	for(i = 0; i < numOfTokens; i++){
		children = getChildrenPtr(current);
		if(children == NULL){
			toDelete = FALSE;
			break;
		}
		result = searchPositionBinary(children, getNumOfChildrenPtr(current), getActualNumOfChildrenPtr(current), ngram[i], &position);
		if(result >= 0){
			toDelete = FALSE;
			break;
		}
		history[i] = current;
		positions[i] = position;
		current = getCertainChildPtr(current, position);
	}


	if(toDelete){
		if(getIs_FinalPtr(current)){
			setIs_FinalPtr(current, FALSE);

			for(i = numOfTokens - 1; i >= 0; i--){
				if(getChildrenPtr(current) == NULL && getIs_FinalPtr(current) == FALSE){
					deleteANode(history[i], positions[i]);
					current = history[i];
				}
				else{
					break;
				}
			}
		}
	}

	free(history);
	free(positions);
}

//RECURSIVE
//int search_and_delete(pointerToTrieNode trie_node, char **ngram, int numOfTokens){
//	int result, newNumOfTokens, position = -1;
//	pointerToTrieNode children;
//	
//	if(numOfTokens == 0){
//		if(!getIs_FinalPtr(trie_node)) return FALSE;
//		else{
//			if(getChildrenPtr(trie_node) == NULL){
//				return TRUE;
//			}
//			else{
//				setIs_FinalPtr(trie_node, FALSE);
//				return FALSE;
//			}
//		}
//	}
//	else{
//
//		if((children = getChildrenPtr(trie_node)) == NULL) return FALSE;
//
//		searchPositionBinary(children, getNumOfChildrenPtr(trie_node), getActualNumOfChildrenPtr(trie_node), ngram[0], &position);
//		
//		if(position < 0) return FALSE;
//
//		newNumOfTokens = numOfTokens - 1;
//		if(newNumOfTokens == 0) result = search_and_delete(getCertainChildPtr(current, position), NULL, newNumOfTokens);
//		else result = search_and_delete(getCertainChildPtr(current, position), ngram + 1, newNumOfTokens);
//
//		if(result){
//			deleteANode(trie_node, position);
//			if(getChildrenPtr(trie_node) == NULL && getIs_FinalPtr(trie_node) == FALSE){
//				return TRUE;
//			}
//		}
//
//		return FALSE;
//	}
//}




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////SEARCH/////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void search_for_ngrams(TriePtr trie, char **phrase, int numOfTokens){
	int i, separator = FALSE;
	pointerToBinHeapArray heap;
	binaryHeapArrayInit(&heap);
	set_HeapLength(heap, 512);
	set_HeapArray(heap);

	for(i = 0; i < numOfTokens; i++){
		search(trie->root, phrase+i, numOfTokens-i, &separator, &heap);
	}
	if(separator == FALSE) printf("-1");
	printf("\n");

	destroyBinaryHeapArray(heap);
}

void search(pointerToTrieNode root, char **phrase, int numOfTokens, int *separator, pointerToBinHeapArray *heap){
	
	int i, position = -1;
	char result[2000] = "";
	pointerToTrieNode current = root, children;

	for(i = 0; i < numOfTokens; i++){
		children = getChildrenPtr(current);
		position = -1;


		if(children == NULL) return;

		searchPositionBinary(children, getNumOfChildrenPtr(current), getActualNumOfChildrenPtr(current), phrase[i], &position);
		if(position < 0) return;

		if(strcmp(result, "") == 0){
			strcpy(result, getWordPtr(getCertainChildPtr(current, position)));
		}
		else{
			sprintf(result+strlen(result), " %s",  getWordPtr(getCertainChildPtr(current, position)));
		}


		if(getIs_FinalPtr(getCertainChildPtr(current, position))){
			if(!searchBinaryHeapArray(*heap, getCertainChildPtr(current, position))){
				insertInBinaryHeapArray(*heap, getCertainChildPtr(current, position));

				if(*separator) printf("|");
				else *separator = TRUE;

				printf("%s", result);	
			}
		}

		current = getCertainChildPtr(current, position);
	}
}


//void search_for_ngrams(TriePtr trie, char **phrase, int numOfTokens){
//	int i, separator = FALSE;
//	char result[2000] = "";
//	pointerToBinHeapArray heap;
//	binaryHeapArrayInit(&heap);
//	set_HeapLength(heap, 512);
//	set_HeapArray(heap);
//
//	for(i = 0; i < numOfTokens; i++){
//		search(trie->root, phrase+i, numOfTokens-i, &separator, &heap, result, 2000);
//	}
//	if(separator == FALSE) printf("-1");
//	printf("\n");
//
//	destroyBinaryHeapArray(heap);
//}
//
//void search(pointerToTrieNode root, char **phrase, int numOfTokens, int *separator, pointerToBinHeapArray *heap, char *result, int size){
//	
//	int i, position = -1;
//	char tempResult[2000] = "";
//	pointerToTrieNode current = root, children;
//
//	for(i = 0; i < numOfTokens; i++){
//		children = getChildrenPtr(current);
//		position = -1;
//
//		//printf("%s - %d\n", phrase[i], *separator);
//
//		if(children == NULL) return;
//
//		searchPositionBinary(children, getNumOfChildrenPtr(current), getActualNumOfChildrenPtr(current), phrase[i], &position);
//		if(position < 0) return;
//
//		if(strcmp(tempResult, "") == 0){
//			strcpy(tempResult, getWordPtr(getCertainChildPtr(current, position)));
//		}
//		else{
//			sprintf(tempResult+strlen(tempResult), " %s",  getWordPtr(getCertainChildPtr(current, position)));
//		}
//
//		//printf("~%s\n", result);
//
//		if(getIs_FinalPtr(getCertainChildPtr(current, position))){
//			//if(strcmp(getWordPtr(getCertainChildPtr(current, position)), "values") == 0){ printf("\nhere %p\n", getCertainChildPtr(current, position)); printBinaryHeapArray(*heap);}
//			if(!searchBinaryHeapArray(*heap, getCertainChildPtr(current, position))){
//				//if(strcmp(getWordPtr(getCertainChildPtr(current, position)), "values") == 0){ printf("\nhere1\n"); printBinaryHeapArray(*heap); }
//				insertInBinaryHeapArray(*heap, getCertainChildPtr(current, position));
//
//				if(*separator) {
//					if(strlen(result) + 1 >= size){
//						printf("%s", result);
//						strcpy(result, "");
//					}
//					strcat(result, "|");
//
//				}
//				else *separator = TRUE;
//
//				if(strlen(result)+strlen(tempResult) < size){
//					strcat(result, tempResult);
//				}
//				else{
//					printf("%s", result);
//					strcpy(result, "");
//				}	
//			}
//			//else{
//				//if(strcmp(getWordPtr(getCertainChildPtr(current, position)), "values") == 0){ printf("\nhere2\n"); printBinaryHeapArray(*heap); }
//			//}
//		}
//
//		current = getCertainChildPtr(current, position);
//	}
//}




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////PRINTS FOR DEBUGGING/////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void printTrie(TriePtr trie){
	printf("Trie:\n\n");
	printTrieNodeTreeMode(trie->root, 0);
}

void printTrieNodeTreeMode(pointerToTrieNode node, int whitespace){
	int i, j, size = -1;

	size = getActualNumOfChildrenPtr(node);

	if(size == 0){
		for(i = 0; i < whitespace; i++){ 
			printf("\t");
		}
		printf("-\n");
	}
	else{
		for(i = 0; i < size; i++){
			for(j = 0; j < whitespace; j++){
				printf("\t");
			}
			printf("%s -> %d\n", getWordPtr(getCertainChildPtr(node, i)), getIs_FinalPtr(getCertainChildPtr(node, i)));
			printTrieNodeTreeMode(getCertainChildPtr(node, i), whitespace+1);
		}
	}
}


void printTrie2(TriePtr trie){
	printf("Trie:\n\n");
	printTrieNode(trie->root);
	printf("\n\n");
	printTrieNodeTreeMode2(trie->root);
}

void printTrieNodeTreeMode2(pointerToTrieNode node){
	int i, j, size = -1;

	size = getActualNumOfChildrenPtr(node);

	if(size == 0){
		printf("-\n");
	}
	else{
		for(i = 0; i < size; i++){
			printTrieNode(getCertainChildPtr(node, i));
		}
		printf("\n\n");
		for(i = 0; i < size; i++){
			printTrieNodeTreeMode2(getCertainChildPtr(node, i));
		}
	}
}