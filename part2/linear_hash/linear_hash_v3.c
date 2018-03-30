#include "linear_hash_v3.h"


size_t memSize;


typedef struct linearHashTable{
	int size, splittingPoint;
	BucketPtr *tables;
}linearHashTable;

typedef struct Bucket{
	int occupied, size;
	void *values;
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
	hashtable->size = hashSize;
	hashtable->splittingPoint = 0;
	memSize = GetSizeOf(conn);
	
	for(i = 0; i < hashSize; i++){
		if((hashtable->tables[i] = createBuckets(conn)) == NULL){
			destroyLinearHash(hashtable);
			return NULL;
		}
	}


	generator = rand();

	return hashtable;
}

BucketPtr createBuckets(connectorPtr conn){
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
		(InitialiseTrie_Node(conn))((temp->values)+i*memSize);
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
		//traversal delete and normal destroy
		(TraverseAndDestroy(conn))((bucket->values)+i*memSize);
		//(DestroyNode(conn))((bucket->values)+i*memSize);

		//second edition only traversal delete test	
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
	int i, bucketPosition, position, toSplit = FALSE, lowerBound = 0, upperBound, midPoint, res;
	BucketPtr bucket;
	void *ret;
		
	bucketPosition = hashfunction(word, hashtable->size, hashtable->splittingPoint); //pointer to node -> getwordfunction
	bucket = hashtable->tables[bucketPosition];
	upperBound = bucket->occupied - 1;

	position = 0;

	if(upperBound >= 0){	
		while(lowerBound <= upperBound){
	
	        midPoint = (upperBound + lowerBound) / 2;
	        res = strcmp((GetWordPtr(conn))((bucket->values)+midPoint*memSize), word);

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

	    res = strcmp((GetWordPtr(conn))((bucket->values)+midPoint*memSize), word);
	    
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

			(InitialiseTrie_Node(conn))((bucket->values)+i*memSize);
		}
		(RepointToWords(conn))(bucket->values, bucket->occupied);


		toSplit = TRUE;
	}

	memmove((bucket->values)+(position+1)*memSize, (bucket->values)+position*memSize, memSize*(bucket->occupied - position));
	
	(RepointToWords(conn))((bucket->values)+(position+1)*memSize, bucket->occupied - position);

	(InitialiseTrie_Node(conn))((bucket->values)+position*memSize);
	(SetWordPtr(conn))((bucket->values)+position*memSize, word);

	bucket->occupied += 1;

	ret = (bucket->values)+position*memSize;

	if(toSplit){ 
		if(bucketPosition == hashtable->splittingPoint){
			ret = splitAndSetHashTable(hashtable, conn, word);
		}
		else{
			splitHashTable(hashtable, conn);
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
int splitHashTable(LinearHTPtr hashtable, connectorPtr conn){
	int i, j, k, availablePosition, position, split = hashtable->splittingPoint, freeBuckets;
	BucketPtr current, newBucket;

	if(split == 0){
		hashtable->tables = realloc(hashtable->tables, sizeof(BucketPtr)*(hashtable->size*2));
		if(hashtable->tables == NULL) return FALSE;
	}
	hashtable->splittingPoint += 1;
	hashtable->tables[hashtable->size+split] = createBuckets(conn);
	if(hashtable->tables[hashtable->size+split] == NULL) return FALSE;

	current = hashtable->tables[split];
	newBucket = hashtable->tables[hashtable->size+split];

	for(i = 0, j = 0; i < current->occupied; i++){

		position = hashfunction((GetWordPtr(conn))((current->values)+i*memSize), hashtable->size, hashtable->splittingPoint);
		
		if(position != split){
			
			if(newBucket->occupied == newBucket->size){
				newBucket->values = realloc(newBucket->values, memSize*(newBucket->size+BUCKET_SIZE));
				if(newBucket->values == NULL) return FALSE;
				(RepointToWords(conn))(newBucket->values, newBucket->occupied);
				newBucket->size += BUCKET_SIZE;
				for(k = newBucket->occupied; k < newBucket->size; k++){
					(InitialiseTrie_Node(conn))((newBucket->values)+k*memSize);
				}

			}

			memmove((newBucket->values)+j*memSize, (current->values)+i*memSize, memSize);
			(RepointToWords(conn))((newBucket->values)+j*memSize, 1);
			newBucket->occupied += 1;
			j++;

			current->occupied -= 1;
			memmove((current->values)+i*memSize, (current->values)+(i+1)*memSize, memSize*(current->occupied - i));
			(RepointToWords(conn))((current->values)+i*memSize, current->occupied - i);
			(InitialiseTrie_Node(conn))((current->values)+current->occupied*memSize);
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
		(RepointToWords(conn))(current->values, current->occupied);

	}

	return TRUE;
}

void* splitAndSetHashTable(LinearHTPtr hashtable, connectorPtr conn, char* insertedWord){
	int i, j, k, availablePosition, position, split = hashtable->splittingPoint, freeBuckets;
	BucketPtr current, newBucket, insertedBucket;
	void* ret = NULL;
	int insertedPosition = -1;

	if(split == 0){
		hashtable->tables = realloc(hashtable->tables, sizeof(BucketPtr)*(hashtable->size*2));
		if(hashtable->tables == NULL) return FALSE;
	}
	hashtable->splittingPoint += 1;
	hashtable->tables[hashtable->size+split] = createBuckets(conn);
	if(hashtable->tables[hashtable->size+split] == NULL) return FALSE;

	current = hashtable->tables[split];
	newBucket = hashtable->tables[hashtable->size+split];

	for(i = 0, j = 0; i < current->occupied; i++){

		position = hashfunction((GetWordPtr(conn))((current->values)+i*memSize), hashtable->size, hashtable->splittingPoint);
		
		if(position != split){
			
			if(newBucket->occupied == newBucket->size){
				newBucket->values = realloc(newBucket->values, memSize*(newBucket->size+BUCKET_SIZE));
				if(newBucket->values == NULL) return FALSE;
				(RepointToWords(conn))(newBucket->values, newBucket->occupied);
				newBucket->size += BUCKET_SIZE;
				for(k = newBucket->occupied; k < newBucket->size; k++){
					(InitialiseTrie_Node(conn))((newBucket->values)+k*memSize);
				}

			}

			memmove((newBucket->values)+j*memSize, (current->values)+i*memSize, memSize);
			(RepointToWords(conn))((newBucket->values)+j*memSize, 1);
			if(strcmp(insertedWord, (GetWordPtr(conn))((newBucket->values)+j*memSize)) == 0){
				insertedBucket = newBucket;
				insertedPosition = j;
			}
			newBucket->occupied += 1;
			j++;

			current->occupied -= 1;
			memmove((current->values)+i*memSize, (current->values)+(i+1)*memSize, memSize*(current->occupied - i));
			(RepointToWords(conn))((current->values)+i*memSize, current->occupied - i);
			(InitialiseTrie_Node(conn))((current->values)+current->occupied*memSize);
			i--;
		}
		else{
			if(strcmp(insertedWord, (GetWordPtr(conn))((current->values)+i*memSize)) == 0){
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
		(RepointToWords(conn))(current->values, current->occupied);
	}

	ret = (insertedBucket->values)+insertedPosition*memSize;

	return ret;
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
				value = (GetWordPositionPtr(conn))((bucket->values)+midPoint*memSize, 0);
	        	res = strcmp(value, word);
	        	free(value);
	        }
	        else{
				value = (GetWordPtr(conn))((bucket->values)+midPoint*memSize);
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
			value = (GetWordPositionPtr(conn))((bucket->values)+midPoint*memSize, 0);
	    	res = strcmp(value, word);
	    	free(value);
	    }
	    else{
			value = (GetWordPtr(conn))((bucket->values)+midPoint*memSize);
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
	     	return (bucket->values)+midPoint*memSize;   
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
	int position, freeBuckets;

	lookupBucket(bucket, word, conn, &position);
	
	if(position < 0) return;

	//(DestroyNode(conn)(current->values[position]));
	bucket->occupied -= 1;
	memmove((bucket->values)+position*memSize, (bucket->values)+(position+1)*memSize, memSize*(bucket->occupied - position));
	(RepointToWords(conn))((bucket->values)+position*memSize, bucket->occupied - position);
	(InitialiseTrie_Node(conn))((bucket->values)+bucket->occupied*memSize);

	freeBuckets = (bucket->size - bucket->occupied) / BUCKET_SIZE;
	if(freeBuckets > 0){
		bucket->size = bucket->size - (freeBuckets*BUCKET_SIZE);
		if(bucket->size == 0) bucket->size = BUCKET_SIZE;
		bucket->values = realloc(bucket->values, memSize*bucket->size);
		//if(bucket->values == NULL) return;
		(RepointToWords(conn))(bucket->values, bucket->occupied);
	}

	return;
}



void shrinkHashTable(LinearHTPtr hashtable){
	int i, j;
	BucketPtr current = NULL;

	for(i = 0; i < hashtable->size+hashtable->splittingPoint; i++){
		current = hashtable->tables[i];

		for(j = 0; j < current->occupied; j++){
			shrink((current->values)+j*memSize);
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
				char *value = (GetWordPositionPtr(conn))((current->values)+j*memSize, 0);
				printf("\t%s - (%s)\n", value, (GetWordPtr(conn))((current->values)+j*memSize));
				free(value);
			}
			else{
				printf("\t%s\n", (GetWordPtr(conn))((current->values)+j*memSize));
			}
			printTrieNode((current->values)+j*memSize);
		}
		printf("\t--NULL--\n");
	}
}