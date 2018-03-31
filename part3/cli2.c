#include "cli2.h"



int screenOrFile(int sOrF, char* fileName, TriePtr trie)
{/*Decides whether our input comes from the screen or a file, and hands it over to the function*/
    int type;
    char input[BUFFER_MAX];
    if(sOrF==SCREEN)
    {/*Get input from screen*/
        ;
    }
    else if(sOrF==FILEIN)
    {/*Get input from file*/
        FILE* fp=fopen(fileName,"r");
        if(fp==NULL)
        {
            perror("Cannot open input file\n");
            return FALSE;
        }
        char* buffer=NULL;
        size_t bufferLen=0;
        size_t read;
        pointerToScheduler scheduler;
        setScheduler(&scheduler, NUMBEROFTHREADS);
        int numOfQueries=0;
        if((type = getStatic(trie)) == DYNAMIC){
            int trieFlag = FALSE, trieVer = 0;
            while((read=getline(&buffer,&bufferLen,fp))!=-1)
            {
                parser2D(buffer, trie, scheduler,&numOfQueries,&trieVer,&trieFlag);
                free(buffer);
                buffer=NULL;
                bufferLen=0;
            }
        }
        else if(type == STATIC){
            while((read=getline(&buffer,&bufferLen,fp))!=-1)
            {
                parser2S(buffer, trie, scheduler,&numOfQueries);
                free(buffer);
                buffer=NULL;
                bufferLen=0;
            }
        }
        
        destroyScheduler(scheduler);
        fclose(fp);
        return TRUE;
    }
}
 

int initFile(char* fileName, TriePtr* trie)
{//Reads the input file line by line and initialises the trie data structure
    FILE *fp;
    fp=fopen(fileName,"r");
    if(fp==NULL)
    {
        perror("Cannot open input file\n");
        return FALSE;
    }
    else
    {
        char* buffer=NULL;
        size_t bufferLen=0;
        size_t read;
        int numOfTokens=0;
        char** tokens;
        char* separator=" ";
        if((read=getline(&buffer,&bufferLen,fp))!=-1)
        {
            tokenizer(buffer, separator, &tokens,  &numOfTokens);
            if(strcmp(tokens[0],STATIC_KEYWORD)==0)
            { 
                *trie = init_trie(HASH_SIZE, STATIC);
                free(buffer);
                buffer=NULL;
                bufferLen=0;
                initFileS(fp, trie);
            }
            else if(strcmp(tokens[0],DYNAMIC_KEYWORD)==0)
            {
                *trie = init_trie(HASH_SIZE, DYNAMIC);
                free(buffer);
                buffer=NULL;
                bufferLen=0;
                initFileD(fp, trie);
            }
            deleteTokens(tokens, numOfTokens);
        }
        fclose(fp);
        return TRUE;
    }
}


void initFileD(FILE* fp, TriePtr* trie){
    char* buffer=NULL;
    size_t bufferLen=0;
    size_t read;
    int numOfTokens=0;
    char** tokens;
    char* separator=" ";

    while((read=getline(&buffer,&bufferLen,fp))!=-1)
    {
        tokenizer(buffer, separator, &tokens,  &numOfTokens);
   
        insert_ngramD(*trie, tokens, numOfTokens, 0);
          
        free(buffer);
        buffer=NULL;
        bufferLen=0;
        deleteTokens(tokens, numOfTokens);
    }
}

void initFileS(FILE* fp, TriePtr* trie){
    char* buffer=NULL;
    size_t bufferLen=0;
    size_t read;
    int numOfTokens=0;
    char** tokens;
    char* separator=" ";

    while((read=getline(&buffer,&bufferLen,fp))!=-1)
    {
        tokenizer(buffer, separator, &tokens,  &numOfTokens);
   
        insert_ngramS(*trie, tokens, numOfTokens);
          
        free(buffer);
        buffer=NULL;
        bufferLen=0;
        deleteTokens(tokens, numOfTokens);
    }
}
 








int parser2D(char* inputBuffer, TriePtr trie, pointerToScheduler theScheduler, int *numOfQueries, int *trieVer, int *trieFlag)
{
    char instruction[3];
    char* stringToTokenize;
    char** tokens;
    int numOfWords;
    if(sscanf(inputBuffer,"%s",instruction))
    {
        if(strcmp(instruction,"Q")==0)
        {
            stringToTokenize=inputBuffer+2;/*Point at the start of the n gram(s)*/
            char* input=malloc((strlen(stringToTokenize)+1)*sizeof(char));
            strcpy(input,stringToTokenize);
            
            if((*trieFlag) == FALSE) (*trieFlag) = TRUE;
            QueryPtr argumentList=malloc(sizeof(Query));
            pointerToJob aJob;
            argumentList->trie=trie;
           
            argumentList->ticket=(*numOfQueries);
            argumentList->trieVer=(*trieVer);
            argumentList->line=input;

            createJob(&aJob, &queryD, argumentList);
            submitJob(theScheduler, aJob );
            ++(*numOfQueries);
        }
        else if(strcmp(instruction,"A")==0)
        {
            if((*trieFlag) == TRUE){
              (*trieFlag) = FALSE;
              (*trieVer)++;
            }
            stringToTokenize=inputBuffer+2;//Point at the start of the n gram(s)
            tokenizer(stringToTokenize," ",&tokens,&numOfWords);//First tokenize based on " " to get the n grams
            insert_ngramD(trie, tokens, numOfWords, *(trieVer));
            deleteTokens(tokens, numOfWords);
        }
        else if(strcmp(instruction,"D")==0)
        {
            if((*trieFlag) == TRUE){
              (*trieFlag) = FALSE;
              (*trieVer)++;
            }
            stringToTokenize=inputBuffer+2;//Point at the start of the n gram(s)
            tokenizer(stringToTokenize," ",&tokens,&numOfWords);//First tokenize based on " " to get the n grams
            delete_ngram(trie, tokens, numOfWords, (*trieVer));
            deleteTokens(tokens, numOfWords);
        }
        else if(strcmp(instruction,"F")==0)
        {           
            int k;
            if(!(sscanf(inputBuffer,"%s %d",instruction,&k)==2))
            {
              k=0;
            }
            prepareResultsArray(trie, (*numOfQueries));
            executeAllJobs(theScheduler);
            pointerToTreapNode root;
            treapInit(&root);
            printResults(trie,&root,k);
            (*numOfQueries)=0;
           
            if(k!=0)
            {
              topK(root, &k);
            }
            deleteTreap(root);
          
            return TRUE;
        }
        else
        {/*Erroneous command*/
            printf("Erroneous command. Please try again\n" );
            return FALSE;
        }
    }
    else
    {   
        return FALSE;
    }
}

int parser2S(char* inputBuffer, TriePtr trie, pointerToScheduler theScheduler, int *numOfQueries)
{
   // printf("In parser 2\n");
    char instruction[3];
    char* stringToTokenize;
    char** tokens;
    int numOfWords;
    if(sscanf(inputBuffer,"%s",instruction))
    {/*Read your instruction successfully*/
        if(strcmp(instruction,"Q")==0)
        {/*Question; now we need to create a new job and give it to the scheduler*/
            stringToTokenize=inputBuffer+2;/*Point at the start of the n gram(s)*/
            char* input=malloc((strlen(stringToTokenize)+1)*sizeof(char));
            strcpy(input,stringToTokenize);
            QueryPtr argumentList=malloc(sizeof(Query));
            pointerToJob aJob;
            argumentList->trie=trie;
            argumentList->ticket=(*numOfQueries);
            argumentList->trieVer=0;
            argumentList->line=input;
            createJob(&aJob, &queryS, argumentList);
            submitJob(theScheduler, aJob );
            ++(*numOfQueries);
        }
        else if(strcmp(instruction,"F")==0)
        {/*Exit from command line*/
            int k;
            if(!(sscanf(inputBuffer,"%s %d",instruction,&k)==2))
            {
              k=0;
            }
            prepareResultsArray(trie, (*numOfQueries));
            executeAllJobs(theScheduler);
            pointerToTreapNode root;
            treapInit(&root);
            printResults(trie,&root,k);
            (*numOfQueries)=0
            if(k!=0)
            {
              topK(root, &k);
            }
            deleteTreap(root);
            return TRUE;
        }
        else
        {/*Erroneous command*/
            printf("Erroneous command. Please try again\n" );
            return FALSE;
        }
    }
    else
    {   
        return FALSE;
    }
}