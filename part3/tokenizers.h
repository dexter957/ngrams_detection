#define NUMOFTOKENS 100

void tokenizer(char* stringToTokenize, char* separator, char*** tokens, int* numOfTokens);
void tokenizer2(char* stringToTokenize, char* separator, char*** tokens, int* numOfTokens);

void deleteTokens(char** tokens, int numOfTokens);