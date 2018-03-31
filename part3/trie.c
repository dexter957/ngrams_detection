#include "trie.h"

int err;

typedef struct Trie{
	int ngrams, type, height, arraySize, querySize;
	char **resultsToPrint;//multi-thread
	//char *resultToPrint;//simple
	LinearHTPtr root;
}Trie;



TriePtr init_trie(int hashSize, int type){

	TriePtr temp;
	if((temp = malloc(sizeof(Trie))) == NULL){
		return NULL;
	}
	temp->root = NULL;
	temp->type = type;

	if((temp->root = createLinearHash(hashSize, type)) == NULL){
		clear_trie(temp);
		return NULL;
	}

	temp->ngrams = 0;
	temp->height = 0;
	temp->arraySize = 0;
	temp->querySize = 0;
	temp->resultsToPrint = NULL;

	return temp;
}


void clear_trie(TriePtr trie){
	if(trie->root!=NULL){
		if(trie->type == DYNAMIC){
			clearAndDestroyLinearHashD(trie->root);
		}
		else if(trie->type == STATIC){
			clearAndDestroyLinearHashS(trie->root);	
		}
	}
	//if(trie->resultToPrint != NULL) free(trie->resultToPrint);
	if(trie->resultsToPrint != NULL) free(trie->resultsToPrint);
	if(trie!=NULL) free(trie);
}




//FOR THE ITERATIVE COMPRESS-SHRINK
void shrinkStaticTrie(TriePtr trie){
	void **history;
	int *positions;

	history = malloc(sizeof(void*)*trie->height);
	positions = malloc(sizeof(int)*trie->height);

	shrinkHashTable(trie->root, history, positions);

	free(history);
	free(positions);
}

//FOR THE RECURSIVE COMPRESS-SHRINK
/*
void shrinkStaticTrie(TriePtr trie){
	shrinkHashTable(trie->root, history, positions);
}
*/



int getStatic(TriePtr trie){
	return trie->type;
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////INSERT/////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void insert_ngram(TriePtr trie, char **ngram, int numOfTokens){
	void *start;


	if(trie->type == DYNAMIC){
		if((start = insertLHTrieNodeD(trie->root, ngram[0])) == NULL) return; //error
		if(numOfTokens == 1){
			setIs_FinalPtr(start, TRUE);
			setVersionsPtr(start, 0, -1);
		}

		if(numOfTokens > 1)	search_and_insertD(start, ngram+1, numOfTokens-1, 0);
	}
	else if(trie->type == STATIC){
		int tempHeight = 0;
		if((start = insertLHTrieNodeS(trie->root, ngram[0])) == NULL) return; //error
		tempHeight++;
		if(numOfTokens == 1){
			static_setIs_FinalPtr(start, TRUE);
		}

		if(numOfTokens > 1)	search_and_insertS(start, ngram+1, numOfTokens-1, &tempHeight);

		if(tempHeight > trie->height) trie->height = tempHeight;
	}

	trie->ngrams += 1;

}


//TYPE SPECIFIC

void insert_ngramD(TriePtr trie, char **ngram, int numOfTokens, int version){
	void *start;

	if((start = insertLHTrieNodeD(trie->root, ngram[0])) == NULL) return; //error
	if(numOfTokens == 1){

		setIs_FinalPtr(start, TRUE);

		setVersionsPtr(start, version, -1);
	}

	if(numOfTokens > 1)	search_and_insertD(start, ngram+1, numOfTokens-1, version);


	trie->ngrams += 1;
}

void insert_ngramS(TriePtr trie, char **ngram, int numOfTokens){
	void *start;
	int tempHeight = 0;

	if((start = insertLHTrieNodeS(trie->root, ngram[0])) == NULL) return; //error
	tempHeight++;
	if(numOfTokens == 1){
		static_setIs_FinalPtr(start, TRUE);
	}

	if(numOfTokens > 1)	search_and_insertS(start, ngram+1, numOfTokens-1, &tempHeight);
	
	if(tempHeight > trie->height) trie->height = tempHeight;

	trie->ngrams += 1;
}


//The insertAChild function returns a position regardless if an actual node was created or not.
//If a node was created, it returns its position in the children array
//If not, it returns the position of the existing node in the children array where that word of the ngram resides 
void search_and_insertD(void* root, char **ngram, int numOfTokens, int version){
	int i, position;
	void* current = root;
	
	for(i = 0; i < numOfTokens; i++){
		if(getNumOfChildrenPtr(current) == 0){
			setNumOfChildrenPtr(current, N);
			setChildrenPtr(current);
		}

		position = insertAChild(current, ngram[i]);

		current = getCertainChildPtr(current, position);
	}

	setIs_FinalPtr(current, TRUE);
	setVersionsPtr(current, version, -1);
}


void search_and_insertS(void* root, char **ngram, int numOfTokens, int *height){
	int i, position;
	void* current = root;
	
	for(i = 0; i < numOfTokens; i++){
		if(static_getNumOfChildrenPtr(current) == 0){
			static_setNumOfChildrenPtr(current, N);
			static_setChildrenPtr(current);
		}

		position = static_insertAChild(current, ngram[i]);

		(*height)++;

		current = static_getCertainChildPtr(current, position);

	}

	static_setIs_FinalPtr(current, TRUE);
}




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////DELETE/////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void delete_ngram(TriePtr trie, char **ngram, int numOfTokens, int version){ 
	void* start;

	start = lookupTrieNodeD(trie->root, ngram[0]);
	if(start != NULL){
		if(numOfTokens == 1){
			if(getIs_FinalPtr(start) && getD_versionPtr(start) == -1) setD_versionPtr(start, version);
		}
		else{
			search_and_delete(start, ngram+1, numOfTokens-1, version);
		}
	}
}

void search_and_delete(void* root, char **ngram, int numOfTokens, int version){ //7
	int i, position, result, toDelete = TRUE;
	void *current = root, *children;

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
		current = getCertainChildPtr(current, position);
	}


	if(toDelete){
		if(getIs_FinalPtr(current) && getD_versionPtr(current) == -1){
			setD_versionPtr(current, version);
		}
	}
}




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////SEARCH/////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

char* setToPrint(char* resultToPrint, int* resultSize, char* toInsert){
	if(strlen(resultToPrint)+strlen(toInsert) >= *resultSize){
		*resultSize *= 2;
		resultToPrint = realloc(resultToPrint, (*resultSize)*sizeof(char));
	}
	strcat(resultToPrint, toInsert);

	return resultToPrint;
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////MULTI-THREADING//////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int prepareResultsArray(TriePtr trie, int queries){
	if(queries > trie->arraySize){
		if((trie->resultsToPrint = realloc(trie->resultsToPrint, sizeof(char*)*queries)) == NULL) return FALSE;
		trie->arraySize = queries;
	}
	trie->querySize = queries;

	return TRUE;
}


void query(QueryPtr q){
	int resultSize = 2000;
	TriePtr trie = q->trie;
	char** phrase;
	int numOfTokens;
	tokenizer2(q->line, " ", &phrase, &numOfTokens);
	
	char *resultToPrint = malloc(sizeof(char)*resultSize);
	strcpy(resultToPrint, "");

	int i, separator = FALSE;
	void *start = NULL;
	pointerToBloomFilter bloom;
	bloom_filterInit(&bloom, ELEMS, ERRORPROB);

	if(trie->type == DYNAMIC){
		int trieVer = q->trieVer, A_version, D_version;
		
		for(i = 0; i < numOfTokens-1; i++){
			start = lookupTrieNodeD(trie->root, phrase[i]);
			if(start != NULL){
				if(getIs_FinalPtr(start)){
					getVersionsPtr(start, &A_version, &D_version);
					if(trieVer >= A_version && (D_version == -1 || trieVer < D_version)){
						if(!bloom_filterCheckAndAdd(bloom, phrase[i])){
							if(separator){
								resultToPrint = setToPrint(resultToPrint, &resultSize, "|");
							}
							else separator = TRUE;
	
							resultToPrint = setToPrint(resultToPrint, &resultSize, phrase[i]);	
						}
					}
				}
				resultToPrint = searchD(start, phrase+i+1, numOfTokens-i-1, &separator, bloom, resultToPrint, &resultSize, trieVer);
			}
		}

		start = lookupTrieNodeD(trie->root, phrase[numOfTokens-1]);
		if(start != NULL){
			if(getIs_FinalPtr(start)){
				getVersionsPtr(start, &A_version, &D_version);
				if(trieVer >= A_version && (D_version == -1 || trieVer < D_version)){
					if(!bloom_filterCheckAndAdd(bloom, phrase[numOfTokens-1])){
						if(separator){
							resultToPrint = setToPrint(resultToPrint, &resultSize, "|");
						}
						else separator = TRUE;

						resultToPrint = setToPrint(resultToPrint, &resultSize, phrase[numOfTokens-1]);
					}
				}
			}
		}
	}
	else if(trie->type == STATIC){

		for(i = 0; i < numOfTokens-1; i++){
			start = lookupTrieNodeS(trie->root, phrase[i]);
			if(start != NULL){
				if(static_getIs_FinalPositionPtr(start, 0)){
					if(!bloom_filterCheckAndAdd(bloom, phrase[i])){
						if(separator){
							resultToPrint = setToPrint(resultToPrint, &resultSize, "|");
						}
						else separator = TRUE;

						resultToPrint = setToPrint(resultToPrint, &resultSize, phrase[i]);
					}
				}
				resultToPrint = searchS(start, phrase+i+1, numOfTokens-i-1, &separator, bloom, resultToPrint, &resultSize);
			}
		}

		start = lookupTrieNodeS(trie->root, phrase[numOfTokens-1]);
		if(start != NULL){
			if(static_getIs_FinalPositionPtr(start, 0)){
				if(!bloom_filterCheckAndAdd(bloom, phrase[numOfTokens-1])){
					if(separator){
						resultToPrint = setToPrint(resultToPrint, &resultSize, "|");
					}
					else separator = TRUE;

					resultToPrint = setToPrint(resultToPrint, &resultSize, phrase[numOfTokens-1]);	
				}
			}
		}
	}

	if(!separator){
		resultToPrint = realloc(resultToPrint, sizeof(char)*3);
		strcpy(resultToPrint, "-1");
	}
	trie->resultsToPrint[q->ticket] = resultToPrint;

	bloom_filterDelete(bloom);
}

//TYPE SPECIFIC

void queryD(QueryPtr q){
	int resultSize = 2000, A_version, D_version, trieVer = q->trieVer;
	TriePtr trie = q->trie;
	
	char** phrase;
	int numOfTokens;
	tokenizer2(q->line, " ", &phrase, &numOfTokens);
	char *resultToPrint = malloc(sizeof(char)*resultSize);
	strcpy(resultToPrint, "");

	int i, separator = FALSE;
	void *start = NULL;
	pointerToBloomFilter bloom;
	bloom_filterInit(&bloom, ELEMS, ERRORPROB);
	
	for(i = 0; i < numOfTokens-1; i++){
		start = lookupTrieNodeD(trie->root, phrase[i]);
		if(start != NULL){
			if(getIs_FinalPtr(start)){
				getVersionsPtr(start, &A_version, &D_version);
				if(trieVer >= A_version && (D_version == -1 || trieVer < D_version)){
					if(!bloom_filterCheckAndAdd(bloom, phrase[i])){
						if(separator){
							resultToPrint = setToPrint(resultToPrint, &resultSize, "|");
						}
						else separator = TRUE;

						resultToPrint = setToPrint(resultToPrint, &resultSize, phrase[i]);	
					}
				}
			}
			resultToPrint = searchD(start, phrase+i+1, numOfTokens-i-1, &separator, bloom, resultToPrint, &resultSize, trieVer);
		}
	}

	start = lookupTrieNodeD(trie->root, phrase[numOfTokens-1]);
	if(start != NULL){
		if(getIs_FinalPtr(start)){
			getVersionsPtr(start, &A_version, &D_version);
			if(trieVer >= A_version && (D_version == -1 || trieVer < D_version)){
				if(!bloom_filterCheckAndAdd(bloom, phrase[numOfTokens-1])){
					if(separator){
						resultToPrint = setToPrint(resultToPrint, &resultSize, "|");
					}
					else separator = TRUE;

					resultToPrint = setToPrint(resultToPrint, &resultSize, phrase[numOfTokens-1]);	
				}
			}
		}
	}
	if(!separator){
		resultToPrint = realloc(resultToPrint, sizeof(char)*3);
		strcpy(resultToPrint, "-1");
	}
	trie->resultsToPrint[q->ticket] = resultToPrint;
	bloom_filterDelete(bloom);
	deleteTokens(phrase, numOfTokens);
	free(q);
}

void queryS(QueryPtr q){
	int resultSize = 2000;
	TriePtr trie = q->trie;

	char** phrase;
	int numOfTokens;
	tokenizer2(q->line, " ", &phrase, &numOfTokens);
	char *resultToPrint = malloc(sizeof(char)*resultSize);
	strcpy(resultToPrint, "");

	int i, separator = FALSE;
	void *start = NULL;
	pointerToBloomFilter bloom;
	bloom_filterInit(&bloom, ELEMS, ERRORPROB);

	for(i = 0; i < numOfTokens-1; i++){
		start = lookupTrieNodeS(trie->root, phrase[i]);
		if(start != NULL){
			if(static_getIs_FinalPositionPtr(start, 0)){
				if(!bloom_filterCheckAndAdd(bloom, phrase[i])){
					if(separator){
						resultToPrint = setToPrint(resultToPrint, &resultSize, "|");
					}
					else separator = TRUE;

					resultToPrint = setToPrint(resultToPrint, &resultSize, phrase[i]);	
				}
			}
			resultToPrint = searchS(start, phrase+i+1, numOfTokens-i-1, &separator, bloom, resultToPrint, &resultSize);
		}
	}

	start = lookupTrieNodeS(trie->root, phrase[numOfTokens-1]);
	if(start != NULL){
		if(static_getIs_FinalPositionPtr(start, 0)){
			if(!bloom_filterCheckAndAdd(bloom, phrase[numOfTokens-1])){
				if(separator){
					resultToPrint = setToPrint(resultToPrint, &resultSize, "|");
				}
				else separator = TRUE;
				resultToPrint = setToPrint(resultToPrint, &resultSize, phrase[numOfTokens-1]);	
			}
		}
	}
	
	if(!separator){
		resultToPrint = realloc(resultToPrint, sizeof(char)*3);
		strcpy(resultToPrint, "-1");
	}
	trie->resultsToPrint[q->ticket] = resultToPrint;

	bloom_filterDelete(bloom);
	deleteTokens(phrase, numOfTokens);
	free(q);
}

////////////////////////////////////////////////////////////////////////////////////////////

char* searchD(void* root, char **phrase, int numOfTokens, int *separator, //pointerToBinHeapArray *heap
pointerToBloomFilter bloom, char *resultToPrint, int *resultSize, int trieVer){ //5
	
	int i, position = -1, A_version, D_version;
	char result[2000] = "";
	void *children, *current = root;

	strcpy(result, getWordPtr(root));
		
	for(i = 0; i < numOfTokens; i++){
		children = getChildrenPtr(current);
		position = -1;

		if(children == NULL) return resultToPrint;

		searchPositionBinary(children, getNumOfChildrenPtr(current), getActualNumOfChildrenPtr(current), phrase[i], &position);

		if(position == -1) return resultToPrint;

		sprintf(result+strlen(result), " %s",  phrase[i]);
		
		current = getCertainChildPtr(current, position);		

		if(getIs_FinalPtr(current)){
			getVersionsPtr(current, &A_version, &D_version);
			if(trieVer >= A_version && (D_version == -1 || trieVer < D_version)){
				if(!bloom_filterCheckAndAdd(bloom,result )){
					if(*separator){
						resultToPrint = setToPrint(resultToPrint, resultSize, "|");
					}
					else *separator = TRUE;
	
					resultToPrint = setToPrint(resultToPrint, resultSize, result);
				}
			}
		}
	}

	return resultToPrint;
}

char* searchS(void* root, char **phrase, int numOfTokens, int *separator, 
pointerToBloomFilter bloom, char *resultToPrint, int *resultSize){ 
	
	int i, position = -1, numOfVisits = 1;
	char result[2000] = "";
	void *children, *current = root;

	char* temp = static_getWordPositionPtr(root, 0);
	strcpy(result, temp);
	free(temp);
		
	for(i = 0; i < numOfTokens; i++){
		children = static_getChildrenPtr(current);
		position = -1;
		
		position = searchStatic(current, phrase[i], numOfVisits);

		if(position == -1) return resultToPrint;

		sprintf(result+strlen(result), " %s",  phrase[i]);

		if(position != -2){
			if(children == NULL) return resultToPrint;
			
			current = static_getCertainChildPtr(current, position);
			
			if(static_getIs_FinalPositionPtr(current, 0)){
				if(!bloom_filterCheckAndAdd(bloom,result )){

					if(*separator){
						resultToPrint = setToPrint(resultToPrint, resultSize, "|");
					}
					else *separator = TRUE;

					resultToPrint = setToPrint(resultToPrint, resultSize, result);
				}
			}

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
				}
			}

			numOfVisits++;
		}
			
	}

	return resultToPrint;
}


void printResults(TriePtr trie, pointerToTreapNode* root, int k){
	int i;
	for(i = 0; i < trie->querySize; i++){
		printf("%s\n", trie->resultsToPrint[i]);
		if(k!=0)
		{
			makeTreap(trie->resultsToPrint[i], "|", root);
		}
		free(trie->resultsToPrint[i]);
	}
}






///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////PRINTS FOR DEBUGGING/////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




void printTrie(TriePtr trie){
	printf("\n\nTrie Info:\n\tType: ");
	if(trie->type == STATIC){
		printf("STATIC\n");
		printf("\tNgrams: %d\n\n\n", trie->ngrams);
		printHashtableS(trie->root);
	}
	else{
		printf("DYNAMIC\n");
		printf("\tNgrams: %d\n\n\n", trie->ngrams);
		printHashtableD(trie->root);
	}
}



void makeTreap(char* resultLine, char* separator, pointerToTreapNode *root)
{/*Gets the answer to a query,tokenizes it based on "|", and inserts every word in the treap*/
    /*Remember where you started from, so you can free the memory*/
    char* start=resultLine;
    /*Now tokenize based on "|" and insert every token in the treap*/
    char* token=strtok(resultLine,separator);
    while(token!=NULL)
    {
        insertAChildTreap(root, token);
        token=strtok(NULL,separator);
    }
    /*Now free the memory consumed by the line in the table*/
    /*Done, return*/
    return;
}