#include "linear_hash_v2.h"



typedef struct linearHashTable{
	int size, splittingPoint;
	BucketPtr *tables;
}linearHashTable;

typedef struct Bucket{
	int occupied, size;
	void **values;
}Bucket;


int generator;


LinearHTPtr createLinearHash(int hashSize, connectorPtr conn){
	int i;
	LinearHTPtr hashtable;
	if((hashtable = malloc(sizeof(linearHashTable))) == NULL) return NULL;

	if((hashtable->tables = malloc(sizeof(BucketPtr)*hashSize)) == NULL){
		destroyLinearHash(hashtable);
		return NULL;
	}

	for(i = 0; i < hashSize; i++){
		if((hashtable->tables[i] = createBuckets()) == NULL){
			destroyLinearHash(hashtable);
			return NULL;
		}
	}

	hashtable->size = hashSize;
	hashtable->splittingPoint = 0;

	generator = rand();

	return hashtable;
}

BucketPtr createBuckets(){
	int i;
	BucketPtr temp;
	
	if((temp = malloc(sizeof(Bucket))) == NULL) return NULL;
	
	temp->occupied = 0;
	temp->size = BUCKET_SIZE;
	temp->values = NULL;
	
	if((temp->values = malloc(sizeof(void*)*BUCKET_SIZE)) == NULL){ 
		destroyBuckets(temp);
		return NULL;
	}
	
	for(i = 0; i < BUCKET_SIZE; i++){
		temp->values[i] = NULL;
	}

	return temp;
}



int destroyLinearHash(LinearHTPtr hashtable){
	int i;
	BucketPtr current, next;

	if(hashtable->tables != NULL) {
		for(i = 0; i < hashtable->size+hashtable->splittingPoint; i++){ //parakolou8hsh < || <=
			if(hashtable->tables[i] != NULL){
				destroyBuckets(hashtable->tables[i]);
			}
		}

		free(hashtable->tables);
	}

	if(hashtable != NULL) free(hashtable);
}

int destroyBuckets(BucketPtr bucket){

	free(bucket->values);
	free(bucket);

	return TRUE;
}

int clearAndDestroyLinearHash(LinearHTPtr hashtable, connectorPtr conn){
	int i;
	BucketPtr current, next;

	if(hashtable->tables != NULL) {
		for(i = 0; i < hashtable->size+hashtable->splittingPoint; i++){ //parakolou8hsh < || <=
			if(hashtable->tables[i] != NULL){
				clearAndDestroyBuckets(hashtable->tables[i], conn);
			}
		}

		free(hashtable->tables);
	}

	if(hashtable != NULL) free(hashtable);
}

int clearAndDestroyBuckets(BucketPtr bucket, connectorPtr conn){
	int i;

	for(i = 0; i < bucket->occupied; i++){
		if(bucket->values[i] != NULL) {
			//traversal delete and normal destroy
			(TraverseAndDestroy(conn))(bucket->values[i]);
			(DestroyNode(conn))(bucket->values[i]);

			//second edition only traversal delete test
		}
	}
	free(bucket->values);
	free(bucket);

	return TRUE;
}



//word hashfunction
int hashfunction(char *word, int size, int splittingPoint){
	int i, result;
	unsigned long sum = 0;

	for(i = 0; i < strlen(word); i++){
		sum += word[i];
	}

	sum *= generator; 

	result = sum % size;
	if(result < splittingPoint) result = sum % (size * 2);

	return result;
}



void* insertLHTrieNode(LinearHTPtr hashtable, char* word, connectorPtr conn){//pointer to node
	int i, position, toSplit = FALSE, lowerBound = 0, upperBound, midPoint = 0, res;
	BucketPtr bucket;
	void *node;

	position = hashfunction(word, hashtable->size, hashtable->splittingPoint); //pointer to node -> getwordfunction
	bucket = hashtable->tables[position];
	upperBound = bucket->occupied - 1;
	position = 0;

	if(upperBound >= 0){
		while(lowerBound <= upperBound){

	        midPoint = (upperBound + lowerBound) / 2;
	        res = strcmp((GetWordPtr(conn))(bucket->values[midPoint]), word);

	        if(res < 0){
	            lowerBound=midPoint+1;
	        }
	        else if(res > 0){
	            upperBound=midPoint-1;
	        }
	        else if(res == 0){
	            break;
	        }
	    }

	    res = strcmp((GetWordPtr(conn))(bucket->values[midPoint]), word);
	    
	    if(res < 0){
	        position=midPoint+1;
	    }
	    else if(res > 0){
	        position=midPoint;
	    }
	    else if(res == 0){
	    	return bucket->values[midPoint];
	    }
	}
    //in need of extra space
    //realloc the bucket's values to add extra sizeof one bucket
    if(bucket->occupied == bucket->size){
		bucket->values = realloc(bucket->values, sizeof(void*)*(bucket->size+BUCKET_SIZE));
		bucket->size += BUCKET_SIZE;
		for(i = bucket->occupied; i < bucket->size; i++){
			bucket->values[i] = NULL;
		}
		toSplit = TRUE;
	}

	memmove((bucket->values)+position+1, (bucket->values)+position, sizeof(void*)*(bucket->occupied - position));
	(AllocateTrie_Node(conn))(&node);
	(InitialiseTrie_Node(conn))(node);
	(SetWordPtr(conn))(node, word);
	bucket->values[position] = node;
	bucket->occupied += 1;

	if(toSplit) splitHashTable(hashtable, conn);

	return node;	
}

//split requires realloc for the hashes in the hashtable
//should it be one realloc per split or one per doubling of the hash size?
//the first one is the one illustrated by the data structure description/outline
//the second one is of course computationally less demanding since it will do a realloc every 2^l*size times
//where size is the starting size of the hash and l is the round of splits, not to be confused with the splittingPoint
//which indicates the cell/list to be split. 
//it's the number of time the splittingPoint has been reset.
int splitHashTable(LinearHTPtr hashtable, connectorPtr conn){
	int i, j, k, availablePosition, position, split = hashtable->splittingPoint, freeBuckets;
	BucketPtr current, withAvailability = NULL, newBucket, next, previous;

	if(split == 0){
		hashtable->tables = realloc(hashtable->tables, sizeof(BucketPtr)*(hashtable->size*2));
		if(hashtable->tables == NULL) return FALSE;
	}
	hashtable->splittingPoint += 1;
	hashtable->tables[hashtable->size+split] = createBuckets();
	if(hashtable->tables[hashtable->size+split] == NULL) return FALSE;

	current = hashtable->tables[split];
	newBucket = hashtable->tables[hashtable->size+split];
	
	for(i = 0, j = 0; i < current->occupied; i++){
		
		position = hashfunction((GetWordPtr(conn))(current->values[i]), hashtable->size, hashtable->splittingPoint);
		
		if(position != split){
			if(newBucket->occupied == newBucket->size){
				newBucket->values = realloc(newBucket->values, sizeof(void*)*(newBucket->size+BUCKET_SIZE));
				if(newBucket->values == NULL) return FALSE;
				newBucket->size += BUCKET_SIZE;
				for(k = newBucket->occupied; k < newBucket->size; k++){
					newBucket->values[k] = NULL;
				}
			}

			newBucket->values[j] = current->values[i];
			newBucket->occupied += 1;
			j++;
			current->occupied -= 1;
			memmove((current->values)+i, (current->values)+i+1, sizeof(void*)*(current->occupied - i));
			i--;
		}
	}


	if(hashtable->splittingPoint == hashtable->size){
		hashtable->splittingPoint = 0;
		hashtable->size = hashtable->size*2;
	}

	freeBuckets = (current->size - current->occupied) / BUCKET_SIZE;
	if(freeBuckets > 0){
		current->size = current->size - (freeBuckets*BUCKET_SIZE);
		if(current->size == 0) current->size = BUCKET_SIZE;
		current->values = realloc(current->values, sizeof(void*)*current->size);
		if(current->values == NULL) return FALSE;
	}

	return TRUE;
}



void* lookupTrieNode(LinearHTPtr hashtable, char* word, connectorPtr conn){//pointer to node
	int position = hashfunction(word, hashtable->size, hashtable->splittingPoint);
	BucketPtr current;
	current = hashtable->tables[position];
	return lookupBucket(current, word, conn, NULL);
}

void* lookupBucket(BucketPtr bucket, char* word, connectorPtr conn, int *exactPos){//pointer to node
	int i, lowerBound = 0, upperBound = bucket->occupied - 1, midPoint = 0, res;
	char *value;
	if(exactPos != NULL) *exactPos = -1;

	if(bucket->occupied > 0){
		while(lowerBound <= upperBound){
	    
	        midPoint = (upperBound + lowerBound) / 2;
	        if(isStatic(conn)){
				value = (GetWordPositionPtr(conn))(bucket->values[midPoint], 0);
	        	res = strcmp(value, word);
	        	free(value);
	        }
	        else{
				value = (GetWordPtr(conn))(bucket->values[midPoint]);
		        res = strcmp(value, word);
	        }

	        if(res < 0){
	            lowerBound=midPoint+1;
	        }
	        else if(res > 0){
	            upperBound=midPoint-1;
	        }
	        else if(res == 0){
	            break;
	        }
	    }

	    if(isStatic(conn)){
			value = (GetWordPositionPtr(conn))(bucket->values[midPoint], 0);
	    	res = strcmp(value, word);
	    	free(value);
	    }
	    else{
			value = (GetWordPtr(conn))(bucket->values[midPoint]);
	        res = strcmp(value, word);
	    }
	    
	    if(res < 0){
	        return NULL;
	    }
	    else if(res > 0){
	        return NULL;
	    }
	    else if(res == 0){
	    	if(exactPos != NULL) *exactPos = midPoint;
	     	return bucket->values[midPoint];   
	    }
	}

    return NULL;
}



//This two functions are only used for the D operation since only the dynamic tries can call it
void removeTrieNode(LinearHTPtr hashtable, char* word, connectorPtr conn){
	int position = hashfunction(word, hashtable->size, hashtable->splittingPoint);
	removeFromBucket(hashtable->tables[position], word, conn);
}

void removeFromBucket(BucketPtr bucket, char* word, connectorPtr conn){
	int position;

	lookupBucket(bucket, word, conn, &position);
	
	if(position < 0) return;

	(DestroyNode(conn)(bucket->values[position]));
	bucket->values[position] = NULL;
	bucket->occupied -= 1;
	memmove((bucket->values)+position, (bucket->values)+position+1, sizeof(void*)*(bucket->occupied - position));

	return;
}



void shrinkHashTable(LinearHTPtr hashtable){
	int i, j;
	BucketPtr current = NULL;

	for(i = 0; i < hashtable->size+hashtable->splittingPoint; i++){
		current = hashtable->tables[i];

		for(j = 0; j < current->occupied; j++){
			shrink(current->values[j]);
		}
	}
}







void printHashtable(LinearHTPtr hashtable, connectorPtr conn){
	int i, j, done = FALSE;
	BucketPtr current = NULL;
printf("Hash Table:\n\tSize: %d\n\tSplitting Point: %d\n", hashtable->size, hashtable->splittingPoint);

	for(i = 0; i < hashtable->size+hashtable->splittingPoint; i++){
		current = hashtable->tables[i];
		printf("%d Table (BUCKET_SIZE = %d, Current Size = %d):\n", i, BUCKET_SIZE, current->size);
		
		printf("\t-%d-\n", current->occupied);
		for(j = 0; j < current->occupied; j++){
			if(isStatic(conn)){
				printf("\t%s - (%s)\n", (GetWordPositionPtr(conn))(current->values[j], 0), (GetWordPtr(conn))(current->values[j]));
				static_printTrieNode(current->values[j]);
			}
			else{
				printf("\t%s\n", (GetWordPtr(conn))(current->values[j]));
				printTrieNode(current->values[j]);
			}
		}
		printf("\t--NULL--\n");
	
	}
}