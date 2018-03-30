#include "cli2.h"


int screenOrFile(int sOrF, char* fileName, TriePtr trie)
{/*Decides whether our input comes from the screen or a file, and hands it over to the function*/
    char input[BUFFER_MAX];
    if(sOrF==SCREEN)
    {/*Get input from screen*/
        do
        {
            fgets(input,BUFFER_MAX,stdin);
        } while (parser(input, trie,NULL)!=TRUE);
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
        pointerToTreapNode root;
        treapInit(&root);
        while((read=getline(&buffer,&bufferLen,fp))!=-1)
        {
            parser(buffer, trie,&root);
            free(buffer);
            buffer=NULL;
            bufferLen=0;
            if(root==NULL)
            {
                treapInit(&root);
            }
        }
        fclose(fp);
        return TRUE;
    }
}
 
 
int initFile(char* fileName, TriePtr* trie)
{/*Reads the input file line by line and initialises the trie data structure*/
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
        int firstLine=FALSE;
        while((read=getline(&buffer,&bufferLen,fp))!=-1)
        {
            tokenizer(buffer, separator, &tokens,  &numOfTokens);
            if(firstLine==FALSE)
            {
                connectorPtr conn=createConnector();
                if(strcmp(tokens[0],STATIC_KEYWORD)==0)
                { 
                    setConnector(conn, STATIC);
                }
                else if(strcmp(tokens[0],DYNAMIC_KEYWORD)==0)
                {
                    setConnector(conn, DYNAMIC);
                }
                *trie = init_trie(HASH_SIZE, conn);

                firstLine=TRUE;
            }
            else
            {
                insert_ngram(*trie, tokens, numOfTokens);
            }
            free(buffer);
            buffer=NULL;
            bufferLen=0;
        }
        fclose(fp);
        return TRUE;
    }
}
 
int parser(char* inputBuffer, TriePtr trie, pointerToTreapNode* root)
{
    char instruction[3];
    int numOfLinesRead=0;
    char* stringToTokenize;
    char** tokens;
    int numOfWords;
    if(sscanf(inputBuffer,"%s",instruction))
    {/*Read your instruction successfully*/
        if(strcmp(instruction,"Q")==0)
        {/*Question; now we need to read a text, line by line*/
            stringToTokenize=inputBuffer+2;/*Point at the start of the n gram(s)*/
            tokenizer(stringToTokenize," ",&tokens,&numOfWords);
        
            search_for_ngrams(trie, tokens, numOfWords,root);
        }
        else if(strcmp(instruction,"A")==0)
        {/*Addition*/
            stringToTokenize=inputBuffer+2;/*Point at the start of the n gram(s)*/
            tokenizer(stringToTokenize," ",&tokens,&numOfWords);/*First tokenize based on " " to get the n grams*/
          
            insert_ngram(trie, tokens, numOfWords);
        }
        else if(strcmp(instruction,"D")==0)
        {/*Deletion*/
            stringToTokenize=inputBuffer+2;/*Point at the start of the n gram(s)*/
            tokenizer(stringToTokenize," ",&tokens,&numOfWords);/*First tokenize based on " " to get the n grams*/
        
        	delete_ngram(trie, tokens, numOfWords);
        }
        else if(strcmp(instruction,"F")==0)
        {/*Exit from command line*/
            int k;
            if(sscanf(inputBuffer,"%s %d",instruction,&k)==2)
            {
                topK(*root, &k);
                deleteTreap((*root));
                (*root)=NULL;
            }
            else
            {
                (*root)=NULL;
            }
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
 
 
void tokenizer(char* stringToTokenize, char* separator, char*** tokens, int* numOfTokens)
{/*Gets a token and the separator, and returns an array of the tokes and its length*/
    /*First count how many tokens you have*/
    /*Create a copy of the string to tokenize, for you'll need it*/
    char* stringCopy=malloc((strlen(stringToTokenize)+1)*sizeof(char));
    strcpy(stringCopy,stringToTokenize);
    //char* stringCopy=stringToTokenize;
    (*numOfTokens)=0;
    char* token=strtok(stringToTokenize,separator);
    while(token!=NULL)
    {
        ++(*numOfTokens);
        token=strtok(NULL,separator);
    }
    (*tokens)=malloc((*numOfTokens)*sizeof(char*));
    token=strtok(stringCopy,separator);
    int i=0;
    while(token!=NULL)
    {
        if((i+1)==(*numOfTokens))
        {
            int wordLength=strlen(token);
            token[wordLength-1]='\0';/*Replace \n character with string end*/
            wordLength=strlen(token);
            if(wordLength > 1 && token[wordLength-1] == '\r') {
            	token[wordLength-1]='\0';
            }
            else{
            	wordLength++;
            }
            (*tokens)[i]=malloc(wordLength*sizeof(char));
            strcpy((*tokens)[i],token);
 
        }
        else
        {
            (*tokens)[i]=malloc((strlen(token)+1)*sizeof(char));
            strcpy((*tokens)[i],token);
        }
        token=strtok(NULL,separator);
        ++i;
    }
}