#include "linear_hash_v3.h"

#if LIN_HASH == 2

size_t memSize;


typedef struct linearHashTable{
	int size, splittingPoint;
	BucketPtr *tables;
}linearHashTable;

typedef struct Bucket{
	int occupied, size;
	void *values;
}Bucket;


unsigned long generator;


LinearHTPtr createLinearHash(int hashSize, int type){
	int i;
	LinearHTPtr hashtable;
	if((hashtable = malloc(sizeof(linearHashTable))) == NULL) return NULL;

	if((hashtable->tables = malloc(sizeof(BucketPtr)*hashSize)) == NULL){
		destroyLinearHash(hashtable);
		return NULL;
	}
	hashtable->size = hashSize;
	hashtable->splittingPoint = 0;
	
	if(type == STATIC){
		memSize = static_getSizeOf();
	
		for(i = 0; i < hashSize; i++){
			if((hashtable->tables[i] = createBucketsS()) == NULL){
				destroyLinearHash(hashtable);
				return NULL;
			}
		}
	}
	else{
		memSize = getSizeOf();
	
		for(i = 0; i < hashSize; i++){
			if((hashtable->tables[i] = createBucketsD()) == NULL){
				destroyLinearHash(hashtable);
				return NULL;
			}
		}
	}

	generator = rand();

	return hashtable;
}

BucketPtr createBucketsD(){
	int i;
	BucketPtr temp;
	
	if((temp = malloc(sizeof(Bucket))) == NULL) return NULL;
	
	temp->occupied = 0;
	temp->size = BUCKET_SIZE;
	temp->values = NULL;
	
	if((temp->values = malloc(memSize*BUCKET_SIZE)) == NULL){ 
		destroyBuckets(temp);
		return NULL;
	}
	
	for(i = 0; i < BUCKET_SIZE; i++){
		initialiseTrie_Node((temp->values)+i*memSize);
	}

	return temp;
}

BucketPtr createBucketsS(){
	int i;
	BucketPtr temp;
	
	if((temp = malloc(sizeof(Bucket))) == NULL) return NULL;
	
	temp->occupied = 0;
	temp->size = BUCKET_SIZE;
	temp->values = NULL;
	
	if((temp->values = malloc(memSize*BUCKET_SIZE)) == NULL){ 
		destroyBuckets(temp);
		return NULL;
	}
	
	for(i = 0; i < BUCKET_SIZE; i++){
		static_initialiseTrie_Node((temp->values)+i*memSize);
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

int clearAndDestroyLinearHashD(LinearHTPtr hashtable){
	int i;
	BucketPtr current, next;

	if(hashtable->tables != NULL) {
		for(i = 0; i < hashtable->size+hashtable->splittingPoint; i++){ //parakolou8hsh < || <=
			if(hashtable->tables[i] != NULL){
				clearAndDestroyBucketsD(hashtable->tables[i]);
			}
		}

		free(hashtable->tables);
	}

	if(hashtable != NULL) free(hashtable);
}

int clearAndDestroyBucketsD(BucketPtr bucket){
	int i;
	for(i = 0; i < bucket->occupied; i++){
		//traversal delete and normal destroy
		traverseAndDestroy((bucket->values)+i*memSize);
		clearNode((bucket->values)+i*memSize);
	}
	free(bucket->values);
	free(bucket);

	return TRUE;
}

int clearAndDestroyLinearHashS(LinearHTPtr hashtable){
	int i;
	BucketPtr current, next;

	if(hashtable->tables != NULL) {
		for(i = 0; i < hashtable->size+hashtable->splittingPoint; i++){ //parakolou8hsh < || <=
			if(hashtable->tables[i] != NULL){
				clearAndDestroyBucketsS(hashtable->tables[i]);
			}
		}

		free(hashtable->tables);
	}

	if(hashtable != NULL) free(hashtable);
}

int clearAndDestroyBucketsS(BucketPtr bucket){
	int i;
	for(i = 0; i < bucket->occupied; i++){
		//traversal delete and normal destroy
		static_traverseAndDestroy((bucket->values)+i*memSize);
		static_clearNode((bucket->values)+i*memSize);
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

	result = sum & (size - 1);
	if(result < splittingPoint) result = sum & ((size << 1) - 1);

	return result;
}



void* insertLHTrieNodeD(LinearHTPtr hashtable, char* word){
	int i, bucketPosition, position, toSplit = FALSE, lowerBound = 0, upperBound, midPoint, res;
	BucketPtr bucket;
	void *ret;
		
	bucketPosition = hashfunction(word, hashtable->size, hashtable->splittingPoint); 
	bucket = hashtable->tables[bucketPosition];
	upperBound = bucket->occupied - 1;

	position = 0;

	if(upperBound >= 0){	
		while(lowerBound <= upperBound){
	
	        midPoint = (upperBound + lowerBound) / 2;
	        res = strcmp(getWordPtr((bucket->values)+midPoint*memSize), word);

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

	    res = strcmp(getWordPtr((bucket->values)+midPoint*memSize), word);
	    
	    if(res < 0){	
	        position=midPoint+1;
	    }
	    else if(res > 0){
	        position=midPoint;
	    }
	    else if(res == 0){
			return (bucket->values)+midPoint*memSize;
	    }
	}


    //in need of extra space
    //realloc the bucket's values to add extra sizeof one bucket
    if(bucket->occupied == bucket->size){

		bucket->values = realloc(bucket->values, memSize*(bucket->size+BUCKET_SIZE));
		bucket->size += BUCKET_SIZE;
		for(i = bucket->occupied; i < bucket->size; i++){

			initialiseTrie_Node((bucket->values)+i*memSize);
		}
		repointToWords(bucket->values, bucket->occupied);


		toSplit = TRUE;
	}

	memmove((bucket->values)+(position+1)*memSize, (bucket->values)+position*memSize, memSize*(bucket->occupied - position));
	
	repointToWords((bucket->values)+(position+1)*memSize, bucket->occupied - position);

	initialiseTrie_Node((bucket->values)+position*memSize);
	setWordPtr((bucket->values)+position*memSize, word);

	bucket->occupied += 1;

	ret = (bucket->values)+position*memSize;

	if(toSplit){ 
		if(bucketPosition == hashtable->splittingPoint){
			ret = splitAndSetHashTableD(hashtable, word);
		}
		else{
			splitHashTableD(hashtable);
		}
	}

	return ret;	
}

void* insertLHTrieNodeS(LinearHTPtr hashtable, char* word){
	int i, bucketPosition, position, toSplit = FALSE, lowerBound = 0, upperBound, midPoint, res;
	BucketPtr bucket;
	void *ret;

	bucketPosition = hashfunction(word, hashtable->size, hashtable->splittingPoint); 
	bucket = hashtable->tables[bucketPosition];
	upperBound = bucket->occupied - 1;

	position = 0;

	if(upperBound >= 0){	
		while(lowerBound <= upperBound){
	
	        midPoint = (upperBound + lowerBound) / 2;
	        res = strcmp(static_getWordPtr((bucket->values)+midPoint*memSize), word);

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

	    res = strcmp(static_getWordPtr((bucket->values)+midPoint*memSize), word);
	    
	    if(res < 0){	
	        position=midPoint+1;
	    }
	    else if(res > 0){
	        position=midPoint;
	    }
	    else if(res == 0){
			return (bucket->values)+midPoint*memSize;
	    }
	}


    //in need of extra space
    //realloc the bucket's values to add extra sizeof one bucket
    if(bucket->occupied == bucket->size){

		bucket->values = realloc(bucket->values, memSize*(bucket->size+BUCKET_SIZE));
		bucket->size += BUCKET_SIZE;
		for(i = bucket->occupied; i < bucket->size; i++){

			static_initialiseTrie_Node((bucket->values)+i*memSize);
		}
		static_repointToWords(bucket->values, bucket->occupied);


		toSplit = TRUE;
	}

	memmove((bucket->values)+(position+1)*memSize, (bucket->values)+position*memSize, memSize*(bucket->occupied - position));

	static_repointToWords((bucket->values)+(position+1)*memSize, bucket->occupied - position);


	static_initialiseTrie_Node((bucket->values)+position*memSize);

	static_setWordPtr((bucket->values)+position*memSize, word);

	bucket->occupied += 1;

	ret = (bucket->values)+position*memSize;

	if(toSplit){ 
		if(bucketPosition == hashtable->splittingPoint){
			ret = splitAndSetHashTableS(hashtable, word);
		}
		else{
			splitHashTableS(hashtable);
		}
	}

	return ret;	
}

//split requires realloc for the hashes in the hashtable
//should it be one realloc per split or one per doubling of the hash size?
//the first one is the one illustrated by the data structure description/outline
//the second one is of course computationally less demanding since it will do a realloc every 2^l*size times
//where size is the starting size of the hash and l is the round of splits, not to be confused with the splittingPoint
//which indicates the cell/list to be split. 
//it's the number of time the splittingPoint has been reset.
int splitHashTableD(LinearHTPtr hashtable){
	int i, j, k, availablePosition, position, split = hashtable->splittingPoint, freeBuckets;
	BucketPtr current, newBucket;

	if(split == 0){
		hashtable->tables = realloc(hashtable->tables, sizeof(BucketPtr)*(hashtable->size*2));
		if(hashtable->tables == NULL) return FALSE;
	}
	hashtable->splittingPoint += 1;
	hashtable->tables[hashtable->size+split] = createBucketsD();
	if(hashtable->tables[hashtable->size+split] == NULL) return FALSE;

	current = hashtable->tables[split];
	newBucket = hashtable->tables[hashtable->size+split];

	for(i = 0, j = 0; i < current->occupied; i++){

		position = hashfunction(getWordPtr((current->values)+i*memSize), hashtable->size, hashtable->splittingPoint);
		
		if(position != split){
			
			if(newBucket->occupied == newBucket->size){
				newBucket->values = realloc(newBucket->values, memSize*(newBucket->size+BUCKET_SIZE));
				if(newBucket->values == NULL) return FALSE;
				repointToWords(newBucket->values, newBucket->occupied);
				newBucket->size += BUCKET_SIZE;
				for(k = newBucket->occupied; k < newBucket->size; k++){
					initialiseTrie_Node((newBucket->values)+k*memSize);
				}

			}

			memmove((newBucket->values)+j*memSize, (current->values)+i*memSize, memSize);
			repointToWords((newBucket->values)+j*memSize, 1);
			newBucket->occupied += 1;
			j++;

			current->occupied -= 1;
			memmove((current->values)+i*memSize, (current->values)+(i+1)*memSize, memSize*(current->occupied - i));
			repointToWords((current->values)+i*memSize, current->occupied - i);
			initialiseTrie_Node((current->values)+current->occupied*memSize);
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
		current->values = realloc(current->values, memSize*current->size);
		if(current->values == NULL) return FALSE;
		repointToWords(current->values, current->occupied);

	}

	return TRUE;
}

void* splitAndSetHashTableD(LinearHTPtr hashtable, char* insertedWord){
	int i, j, k, availablePosition, position, split = hashtable->splittingPoint, freeBuckets;
	BucketPtr current, newBucket, insertedBucket;
	void* ret = NULL;
	int insertedPosition = -1;

	if(split == 0){
		hashtable->tables = realloc(hashtable->tables, sizeof(BucketPtr)*(hashtable->size*2));
		if(hashtable->tables == NULL) return FALSE;
	}
	hashtable->splittingPoint += 1;
	hashtable->tables[hashtable->size+split] = createBucketsD();
	if(hashtable->tables[hashtable->size+split] == NULL) return FALSE;

	current = hashtable->tables[split];
	newBucket = hashtable->tables[hashtable->size+split];

	for(i = 0, j = 0; i < current->occupied; i++){

		position = hashfunction(getWordPtr((current->values)+i*memSize), hashtable->size, hashtable->splittingPoint);
		
		if(position != split){
			
			if(newBucket->occupied == newBucket->size){
				newBucket->values = realloc(newBucket->values, memSize*(newBucket->size+BUCKET_SIZE));
				if(newBucket->values == NULL) return FALSE;
				repointToWords(newBucket->values, newBucket->occupied);
				newBucket->size += BUCKET_SIZE;
				for(k = newBucket->occupied; k < newBucket->size; k++){
					initialiseTrie_Node((newBucket->values)+k*memSize);
				}

			}

			memmove((newBucket->values)+j*memSize, (current->values)+i*memSize, memSize);
			repointToWords((newBucket->values)+j*memSize, 1);
			if(strcmp(insertedWord, getWordPtr((newBucket->values)+j*memSize)) == 0){
				insertedBucket = newBucket;
				insertedPosition = j;
			}
			newBucket->occupied += 1;
			j++;

			current->occupied -= 1;
			memmove((current->values)+i*memSize, (current->values)+(i+1)*memSize, memSize*(current->occupied - i));
			repointToWords((current->values)+i*memSize, current->occupied - i);
			initialiseTrie_Node((current->values)+current->occupied*memSize);
			i--;
		}
		else{
			if(strcmp(insertedWord, getWordPtr((current->values)+i*memSize)) == 0){
				insertedBucket = current;
				insertedPosition = i; 
			}
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
		current->values = realloc(current->values, memSize*current->size);
		if(current->values == NULL) return FALSE;
		repointToWords(current->values, current->occupied);
	}

	ret = (insertedBucket->values)+insertedPosition*memSize;

	return ret;
}

int splitHashTableS(LinearHTPtr hashtable){
	int i, j, k, availablePosition, position, split = hashtable->splittingPoint, freeBuckets;
	BucketPtr current, newBucket;

	if(split == 0){
		hashtable->tables = realloc(hashtable->tables, sizeof(BucketPtr)*(hashtable->size*2));
		if(hashtable->tables == NULL) return FALSE;
	}
	hashtable->splittingPoint += 1;
	hashtable->tables[hashtable->size+split] = createBucketsS();
	if(hashtable->tables[hashtable->size+split] == NULL) return FALSE;

	current = hashtable->tables[split];
	newBucket = hashtable->tables[hashtable->size+split];

	for(i = 0, j = 0; i < current->occupied; i++){

		position = hashfunction(static_getWordPtr((current->values)+i*memSize), hashtable->size, hashtable->splittingPoint);
		
		if(position != split){
			
			if(newBucket->occupied == newBucket->size){
				newBucket->values = realloc(newBucket->values, memSize*(newBucket->size+BUCKET_SIZE));
				if(newBucket->values == NULL) return FALSE;
				static_repointToWords(newBucket->values, newBucket->occupied);
				newBucket->size += BUCKET_SIZE;
				for(k = newBucket->occupied; k < newBucket->size; k++){
					static_initialiseTrie_Node((newBucket->values)+k*memSize);
				}

			}

			memmove((newBucket->values)+j*memSize, (current->values)+i*memSize, memSize);
			static_repointToWords((newBucket->values)+j*memSize, 1);
			newBucket->occupied += 1;
			j++;

			current->occupied -= 1;
			memmove((current->values)+i*memSize, (current->values)+(i+1)*memSize, memSize*(current->occupied - i));
			static_repointToWords((current->values)+i*memSize, current->occupied - i);
			static_initialiseTrie_Node((current->values)+current->occupied*memSize);
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
		current->values = realloc(current->values, memSize*current->size);
		if(current->values == NULL) return FALSE;
		static_repointToWords(current->values, current->occupied);

	}

	return TRUE;
}

void* splitAndSetHashTableS(LinearHTPtr hashtable, char* insertedWord){
	int i, j, k, availablePosition, position, split = hashtable->splittingPoint, freeBuckets;
	BucketPtr current, newBucket, insertedBucket;
	void* ret = NULL;
	int insertedPosition = -1;

	if(split == 0){
		hashtable->tables = realloc(hashtable->tables, sizeof(BucketPtr)*(hashtable->size*2));
		if(hashtable->tables == NULL) return FALSE;
	}
	hashtable->splittingPoint += 1;
	hashtable->tables[hashtable->size+split] = createBucketsS();
	if(hashtable->tables[hashtable->size+split] == NULL) return FALSE;

	current = hashtable->tables[split];
	newBucket = hashtable->tables[hashtable->size+split];

	for(i = 0, j = 0; i < current->occupied; i++){

		position = hashfunction(static_getWordPtr((current->values)+i*memSize), hashtable->size, hashtable->splittingPoint);
		
		if(position != split){
			
			if(newBucket->occupied == newBucket->size){
				newBucket->values = realloc(newBucket->values, memSize*(newBucket->size+BUCKET_SIZE));
				if(newBucket->values == NULL) return FALSE;
				static_repointToWords(newBucket->values, newBucket->occupied);
				newBucket->size += BUCKET_SIZE;
				for(k = newBucket->occupied; k < newBucket->size; k++){
					static_initialiseTrie_Node((newBucket->values)+k*memSize);
				}

			}

			memmove((newBucket->values)+j*memSize, (current->values)+i*memSize, memSize);
			static_repointToWords((newBucket->values)+j*memSize, 1);
			if(strcmp(insertedWord, static_getWordPtr((newBucket->values)+j*memSize)) == 0){
				insertedBucket = newBucket;
				insertedPosition = j;
			}
			newBucket->occupied += 1;
			j++;

			current->occupied -= 1;
			memmove((current->values)+i*memSize, (current->values)+(i+1)*memSize, memSize*(current->occupied - i));
			static_repointToWords((current->values)+i*memSize, current->occupied - i);
			static_initialiseTrie_Node((current->values)+current->occupied*memSize);
			i--;
		}
		else{
			if(strcmp(insertedWord, static_getWordPtr((current->values)+i*memSize)) == 0){
				insertedBucket = current;
				insertedPosition = i; 
			}
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
		current->values = realloc(current->values, memSize*current->size);
		if(current->values == NULL) return FALSE;
		static_repointToWords(current->values, current->occupied);
	}

	ret = (insertedBucket->values)+insertedPosition*memSize;

	return ret;
}



void* lookupTrieNodeD(LinearHTPtr hashtable, char* word){//pointer to node
	int position = hashfunction(word, hashtable->size, hashtable->splittingPoint);
	BucketPtr current;

	current = hashtable->tables[position];
	return lookupBucketD(current, word, NULL);
}

void* lookupBucketD(BucketPtr bucket, char* word, int *exactPos){//pointer to node
	int i, lowerBound = 0, upperBound = bucket->occupied - 1, midPoint = 0, res;
	char *value;
	if(exactPos != NULL) *exactPos = -1;

	if(bucket->occupied > 0){
		while(lowerBound <= upperBound){
	        midPoint = (upperBound + lowerBound) / 2;

			value = getWordPtr((bucket->values)+midPoint*memSize);
	        res = strcmp(value, word);

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

		value = getWordPtr((bucket->values)+midPoint*memSize);
        res = strcmp(value, word);

	    if(res < 0){
	        return NULL;
	    }
	    else if(res > 0){
	        return NULL;
	    }
	    else if(res == 0){
	    	if(exactPos != NULL) *exactPos = midPoint;
	     	return (bucket->values)+midPoint*memSize;   
	    }
	}
	return NULL;
}

void* lookupTrieNodeS(LinearHTPtr hashtable, char* word){//pointer to node
	int position = hashfunction(word, hashtable->size, hashtable->splittingPoint);
	BucketPtr current;

	current = hashtable->tables[position];
	return lookupBucketS(current, word, NULL);
}

void* lookupBucketS(BucketPtr bucket, char* word, int *exactPos){//pointer to node
	int i, lowerBound = 0, upperBound = bucket->occupied - 1, midPoint = 0, res;
	char *value;
	if(exactPos != NULL) *exactPos = -1;

	if(bucket->occupied > 0){
		while(lowerBound <= upperBound){
	        midPoint = (upperBound + lowerBound) / 2;

			value = static_getWordPositionPtr((bucket->values)+midPoint*memSize, 0);
        	res = strcmp(value, word);
        	free(value);
	       
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

		value = static_getWordPositionPtr((bucket->values)+midPoint*memSize, 0);
    	res = strcmp(value, word);
    	free(value);
	    
	    if(res < 0){
	        return NULL;
	    }
	    else if(res > 0){
	        return NULL;
	    }
	    else if(res == 0){
	    	if(exactPos != NULL) *exactPos = midPoint;
	     	return (bucket->values)+midPoint*memSize;   
	    }
	}
	return NULL;
}



//This two functions are only used for the D operation since only the dynamic tries can call it
void removeTrieNodeD(LinearHTPtr hashtable, char* word){
	int position = hashfunction(word, hashtable->size, hashtable->splittingPoint);
	removeFromBucketD(hashtable->tables[position], word);
}

void removeFromBucketD(BucketPtr bucket, char* word){
	int position, freeBuckets;

	lookupBucketD(bucket, word, &position);
	
	if(position < 0) return;

	bucket->occupied -= 1;
	memmove((bucket->values)+position*memSize, (bucket->values)+(position+1)*memSize, memSize*(bucket->occupied - position));
	repointToWords((bucket->values)+position*memSize, bucket->occupied - position);
	initialiseTrie_Node((bucket->values)+bucket->occupied*memSize);

	freeBuckets = (bucket->size - bucket->occupied) / BUCKET_SIZE;
	if(freeBuckets > 0){
		bucket->size = bucket->size - (freeBuckets*BUCKET_SIZE);
		if(bucket->size == 0) bucket->size = BUCKET_SIZE;
		bucket->values = realloc(bucket->values, memSize*bucket->size);
		repointToWords(bucket->values, bucket->occupied);
	}

	return;
}

void removeTrieNodeS(LinearHTPtr hashtable, char* word){
	int position = hashfunction(word, hashtable->size, hashtable->splittingPoint);
	removeFromBucketS(hashtable->tables[position], word);
}

void removeFromBucketS(BucketPtr bucket, char* word){
	int position, freeBuckets;

	lookupBucketS(bucket, word, &position);
	
	if(position < 0) return;

	bucket->occupied -= 1;
	memmove((bucket->values)+position*memSize, (bucket->values)+(position+1)*memSize, memSize*(bucket->occupied - position));
	static_repointToWords((bucket->values)+position*memSize, bucket->occupied - position);
	static_initialiseTrie_Node((bucket->values)+bucket->occupied*memSize);

	freeBuckets = (bucket->size - bucket->occupied) / BUCKET_SIZE;
	if(freeBuckets > 0){
		bucket->size = bucket->size - (freeBuckets*BUCKET_SIZE);
		if(bucket->size == 0) bucket->size = BUCKET_SIZE;
		bucket->values = realloc(bucket->values, memSize*bucket->size);
		static_repointToWords(bucket->values, bucket->occupied);
	}

	return;
}



void shrinkHashTable(LinearHTPtr hashtable, void** history, int* positions){
	int i, j;
	BucketPtr current = NULL;

	for(i = 0; i < hashtable->size+hashtable->splittingPoint; i++){
		current = hashtable->tables[i];

		for(j = 0; j < current->occupied; j++){
			shrink((current->values)+j*memSize, history, positions);
		}
	}
}




void printHashtableD(LinearHTPtr hashtable){
	int i, j, done = FALSE;
	BucketPtr current = NULL;
printf("Hash Table:\n\tSize: %d\n\tSplitting Point: %d\n", hashtable->size, hashtable->splittingPoint);

	for(i = 0; i < hashtable->size+hashtable->splittingPoint; i++){
		current = hashtable->tables[i];
		printf("%d Table (BUCKET_SIZE = %d, Current Size = %d):\n", i, BUCKET_SIZE, current->size);
		
		printf("\t-%d-\n", current->occupied);
		for(j = 0; j < current->occupied; j++){
			printf("\t%s\n", getWordPtr((current->values)+j*memSize));
			printTrieNode((current->values)+j*memSize);
		}
		printf("\t--NULL--\n");
	}
}

void printHashtableS(LinearHTPtr hashtable){
	int i, j, done = FALSE;
	BucketPtr current = NULL;
printf("Hash Table:\n\tSize: %d\n\tSplitting Point: %d\n", hashtable->size, hashtable->splittingPoint);

	for(i = 0; i < hashtable->size+hashtable->splittingPoint; i++){
		current = hashtable->tables[i];
		printf("%d Table (BUCKET_SIZE = %d, Current Size = %d):\n", i, BUCKET_SIZE, current->size);
		
		printf("\t-%d-\n", current->occupied);
		for(j = 0; j < current->occupied; j++){
			char *value = static_getWordPositionPtr((current->values)+j*memSize, 0);
			printf("\t%s - (%s)\n", value, static_getWordPtr((current->values)+j*memSize));
			static_printTrieNode((current->values)+j*memSize);
			free(value);
		}
		printf("\t--NULL--\n");
	}
}


#endif