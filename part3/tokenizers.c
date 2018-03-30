#include "tokenizers.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void tokenizer(char* stringToTokenize, char* separator, char*** tokens, int* numOfTokens)
{/*Gets a token and the separator, and returns an array of the tokes and its length*/
    /*First count how many tokens you have*/
    /*Create a copy of the string to tokenize, for you'll need it*/
    char* stringCopy=malloc((strlen(stringToTokenize)+1)*sizeof(char));
    strcpy(stringCopy,stringToTokenize);
    (*numOfTokens)=0;
    char* token=strtok(stringToTokenize,separator);
    char* start1=stringCopy;
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
    free(start1);
}



/*A better tokenizer, for the threads*/

void tokenizer2(char* stringToTokenize, char* separator, char*** tokens, int* numOfTokens)
{/*Gets a token and the separator, and returns an array of the tokes and its length*/
	/*This one reallocs memory as needed*/
	int tokensLength=NUMOFTOKENS;
	(*tokens)=malloc(tokensLength*sizeof(char*));
	char* start=stringToTokenize;/*Remember the start of the buffer*/
	(*numOfTokens)=0;
	char* saveprt;
	/*Using strtok_r bc it's thread safe*/
	char* token=strtok_r(stringToTokenize,separator,&saveprt);
	int i=0;
	while(token!=NULL)
    {
        ++(*numOfTokens);
        if((*numOfTokens)>tokensLength)
        {/*Realloc*/
        	(*tokens)=realloc((*tokens),2*tokensLength*sizeof(char*));
        	tokensLength*=2;
        }
        /*Now put the token in the tokens array*/
        int wordLength=strlen(token);
        if(wordLength>1 && token[wordLength-1] == '\n')
        {
        	token[wordLength-1]='\0';/*Replace \n character with string end*/
            wordLength=strlen(token);
        }
        if(wordLength > 1 && token[wordLength-1] == '\r') {
            	token[wordLength-1]='\0';
            }
            else{
            	wordLength++;
            }
        (*tokens)[i]=malloc((strlen(token)+1)*sizeof(char));
        strcpy((*tokens)[i],token);
       // lastToken=token;
        token=strtok_r(NULL,separator,&saveprt);
        ++i;
    }

    /*Now free your buffer*/
    free(start);
}



void deleteTokens(char** tokens, int numOfTokens)
{
    int i;
    for(i=0;i<numOfTokens;++i)
    {
        free(tokens[i]);
    }
    free(tokens);
}
