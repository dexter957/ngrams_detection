

#define TRUE 1
#define FALSE 0
#define LEFT 2
#define RIGHT 3
#define STARTLENGTH 1000

typedef struct treapNode* pointerToTreapNode;
typedef struct arrayAssist* pointerToArrayAssist;

int treapInit(pointerToTreapNode* treapRoot);
int treapEmpty(pointerToTreapNode treapRoot);
void insertAChildTreap(pointerToTreapNode* treapRoot, char* ngram);
void balanceTreap(pointerToTreapNode* treapRoot,pointerToTreapNode current, pointerToTreapNode parent, int side);
void deleteTreap(pointerToTreapNode node);
void printTreapIn(pointerToTreapNode node);
void printTreapPre(pointerToTreapNode node);
void printTreapPost(pointerToTreapNode node);
void prettyPrint(pointerToTreapNode node);
void topK(pointerToTreapNode root, int* k);
void printInOrderK(pointerToTreapNode node,int *k, int priority, int* next,int* firstPrint);
