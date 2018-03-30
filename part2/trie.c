#include "trie.h"

int err;

typedef struct Trie{
	int ngrams, resultSize;
	char *resultToPrint;
	LinearHTPtr root;
	connectorPtr conn;
}Trie;



TriePtr init_trie(int hashSize, connectorPtr conn){

///
err = 0;
///

	TriePtr temp;
	if((temp = malloc(sizeof(Trie))) == NULL){
		return NULL;
	}
	temp->resultToPrint = NULL;
	temp->root = NULL;
	temp->conn = NULL;
	
	if((temp->resultToPrint = malloc(sizeof(char)*2000)) == NULL){
		clear_trie(temp);
		return NULL;
	}

	if((temp->root = createLinearHash(hashSize, conn)) == NULL){
		clear_trie(temp);
		return NULL;
	}

	temp->conn = conn;
	temp->ngrams = 0;
	strcpy(temp->resultToPrint, "");
	temp->resultSize = 2000;

	return temp;
}


void clear_trie(TriePtr trie){
	if(trie->root!=NULL){
		clearAndDestroyLinearHash(trie->root, trie->conn);
	}
	if(trie->conn!=NULL) deleteConnector(trie->conn);
	if(trie->resultToPrint!=NULL) free(trie->resultToPrint);
	if(trie!=NULL) free(trie);
}


void shrinkStaticTrie(TriePtr trie){
	if(isStatic(trie->conn)){
		shrinkHashTable(trie->root);
	}
}



int getStatic(TriePtr trie){
	return isStatic(trie->conn);
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////INSERT/////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void insert_ngram(TriePtr trie, char **ngram, int numOfTokens){
	void *start;

	if((start = insertLHTrieNode(trie->root, ngram[0], trie->conn)) == NULL) return; //error
	if(numOfTokens == 1){
		(SetIs_FinalPtr(trie->conn))(start, TRUE);
	}

	if(numOfTokens > 1)	search_and_insert(start, ngram+1, numOfTokens-1, trie->conn);

	trie->ngrams += 1;
}

//The insertAChild function returns a position regardless if an actual node was created or not.
//If a node was created, it returns its position in the children array
//If not, it returns the position of the existing node in the children array where that word of the ngram resides 
void search_and_insert(void* root, char **ngram, int numOfTokens, connectorPtr conn){ //5
	int i, position;
	void* current = root;
	
	for(i = 0; i < numOfTokens; i++){
		//if((GetChildrenPtr(conn))(current) == NULL){
		if((GetNumOfChildrenPtr(conn))(current) == 0){
			(SetNumOfChildrenPtr(conn))(current, N);
			(SetChildrenPtr(conn))(current);
		}

		position = (InsertAChild(conn))(current, ngram[i]);

		current = (GetCertainChildPtr(conn))(current, position);
	}

	(SetIs_FinalPtr(conn))(current, TRUE);
}




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////DELETE/////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void delete_ngram(TriePtr trie, char **ngram, int numOfTokens){ 
	void* start;

	start = lookupTrieNode(trie->root, ngram[0], trie->conn);
	if(start != NULL){
		if(numOfTokens == 1){
			if(getIs_FinalPtr(start)) setIs_FinalPtr(start, FALSE);
		}
		else{
			search_and_delete(start, ngram+1, numOfTokens-1);
		}

		if(!getIs_FinalPtr(start) && getChildrenPtr(start) == NULL){
			removeTrieNode(trie->root, ngram[0], trie->conn);
		}
	}

}

void search_and_delete(void* root, char **ngram, int numOfTokens){ //7
	int i, position, result, *positions, toDelete = TRUE;
	void *current = root, **history, *children;

	history = malloc(sizeof(void*)*(numOfTokens));
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



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////SEARCH/////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void search_for_ngrams(TriePtr trie, char **phrase, int numOfTokens, pointerToTreapNode* treapRoot){
	int i, separator = FALSE;
	void *start = NULL;
	pointerToBloomFilter bloom;
	bloom_filterInit(&bloom,/*4, 320,*/ ELEMS, ERRORPROB);

	for(i = 0; i < numOfTokens-1; i++){
		start = lookupTrieNode(trie->root, phrase[i], trie->conn);
		if(start != NULL){
			if((isStatic(trie->conn) && static_getIs_FinalPositionPtr(start, 0)) || (!isStatic(trie->conn) && getIs_FinalPtr(start))){
				if(!bloom_filterCheckAndAdd(bloom, phrase[i])){
					if(separator){
						trie->resultToPrint = setToPrint(trie->resultToPrint, &(trie->resultSize), "|");
					}
					else separator = TRUE;

					trie->resultToPrint = setToPrint(trie->resultToPrint, &(trie->resultSize), phrase[i]);
					insertAChildTreap(treapRoot, phrase[i]);	
				}
			}
			trie->resultToPrint = search(start, phrase+i+1, numOfTokens-i-1, &separator, /*&heap*/ bloom, treapRoot, trie->conn, trie->resultToPrint, &(trie->resultSize));
		}
	}
	start = lookupTrieNode(trie->root, phrase[numOfTokens-1], trie->conn);
	if(start != NULL){
		if((isStatic(trie->conn) && static_getIs_FinalPositionPtr(start, 0)) || (!isStatic(trie->conn) && getIs_FinalPtr(start))){
			if(!bloom_filterCheckAndAdd(bloom, phrase[numOfTokens-1])){
				if(separator){
					trie->resultToPrint = setToPrint(trie->resultToPrint, &(trie->resultSize), "|");
				}
				else separator = TRUE;

				trie->resultToPrint = setToPrint(trie->resultToPrint, &(trie->resultSize), phrase[numOfTokens-1]);
				insertAChildTreap(treapRoot, phrase[numOfTokens-1]);	
			}
		}
	}

	if(separator){
		printf("%s\n", trie->resultToPrint);
		strcpy(trie->resultToPrint, "");
	}
	else printf("-1\n");

	bloom_filterDelete(bloom);
}

char* search(void* root, char **phrase, int numOfTokens, int *separator, /*pointerToBinHeapArray *heap*/
pointerToBloomFilter bloom, pointerToTreapNode* treapRoot, connectorPtr conn, char *resultToPrint, int *resultSize){ //5
	
	int i, position = -1, numOfVisits = 1;
	char result[2000] = "";
	void *children, *current = root;


	if(isStatic(conn)){
		char* temp = (GetWordPositionPtr(conn))(root, 0);
		strcpy(result, temp);
		free(temp);
	}
	else{
		strcpy(result, (GetWordPtr(conn))(root));
	}
		
	for(i = 0; i < numOfTokens; i++){
		children = (GetChildrenPtr(conn))(current);
		position = -1;



		if(isStatic(conn)){
			position = searchStatic(current, phrase[i], numOfVisits);

			if(position == -1) return resultToPrint;

			sprintf(result+strlen(result), " %s",  phrase[i]);

			if(position != -2){
				if(children == NULL) return resultToPrint;
				if(static_getIs_FinalPositionPtr(static_getCertainChildPtr(current, position), 0)){
					if(!bloom_filterCheckAndAdd(bloom,result )){

						if(*separator){
							resultToPrint = setToPrint(resultToPrint, resultSize, "|");
						}
						else *separator = TRUE;

						resultToPrint = setToPrint(resultToPrint, resultSize, result);
						insertAChildTreap(treapRoot, result);	
					}
				}

				current = static_getCertainChildPtr(current, position);
				numOfVisits = 1;
			}
			else{
				if(static_getIs_FinalPositionPtr(current, numOfVisits)){
					if(!bloom_filterCheckAndAdd(bloom,result )){

						if(*separator){
							resultToPrint = setToPrint(resultToPrint, resultSize, "|");
						}
						else *separator = TRUE;

						resultToPrint = setToPrint(resultToPrint, resultSize, result);
						insertAChildTreap(treapRoot, result);	
					}
				}

				numOfVisits++;
			}
		}
		else{

			if(children == NULL) return resultToPrint;

			searchPositionBinary(children, getNumOfChildrenPtr(current), getActualNumOfChildrenPtr(current), phrase[i], &position);

			if(position == -1) return resultToPrint;

			sprintf(result+strlen(result), " %s",  phrase[i]);

			if(getIs_FinalPtr(getCertainChildPtr(current, position))){
				if(!bloom_filterCheckAndAdd(bloom,result )){

					if(*separator){
							resultToPrint = setToPrint(resultToPrint, resultSize, "|");
						}
						else *separator = TRUE;

					resultToPrint = setToPrint(resultToPrint, resultSize, result);
					insertAChildTreap(treapRoot, result);	
				}
			}

			current = getCertainChildPtr(current, position);
		}		
	}

	return resultToPrint;
}

char* setToPrint(char* resultToPrint, int* resultSize, char* toInsert){
	if(strlen(resultToPrint)+strlen(toInsert) >= *resultSize){
		*resultSize *= 2;
		resultToPrint = realloc(resultToPrint, (*resultSize)*sizeof(char));
	}
	strcat(resultToPrint, toInsert);

	return resultToPrint;
}



void printTrie(TriePtr trie){
	printf("\n\nTrie Info:\n\tType: ");
	if(isStatic(trie->conn)) printf("STATIC\n");
	else printf("DYNAMIC\n");
	printf("\tNgrams: %d\n\n\n", trie->ngrams);

	printHashtable(trie->root, trie->conn);


}

