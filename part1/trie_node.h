#define TRUE 1
#define FALSE 0


struct trie_node;
typedef struct trie_node* pointerToTrieNode;

int allocateTrie_Node(pointerToTrieNode* newNode);
void initialiseTrie_Node(pointerToTrieNode theNode);
void setTrie_NodeValues(pointerToTrieNode theNode, int numofchildren, int isfinal, int actualnumofchildren, char* theword);
int doubleNumOfChildren(pointerToTrieNode theNode);
int insertAChild(pointerToTrieNode father, char* wordToInsert);
int searchPositionLinear(pointerToTrieNode children, int numofchildren, int actualnumofchildren, char* wordToInsert);
int searchPositionBinary(pointerToTrieNode children, int numofchildren, int actualnumofchildren, char* wordToInsert, int *exactPos);
void moveChildren(pointerToTrieNode children, int positionToFree, int actualnumofchildren);
void printTrieNode(pointerToTrieNode nodeToPrint);
void deleteANode(pointerToTrieNode father, int childToDelete);
void moveChildrenDel(pointerToTrieNode children, int positionToOccupy, int actualnumofchildren);

/*             						GETTERS  			    				*/
/*With pointer to node argument*/
int getNumOfChildrenPtr(pointerToTrieNode node);
int getIs_FinalPtr(pointerToTrieNode node);
int getActualNumOfChildrenPtr(pointerToTrieNode node);
char* getWordPtr(pointerToTrieNode node);
pointerToTrieNode getChildrenPtr(pointerToTrieNode node);
pointerToTrieNode getCertainChildPtr(pointerToTrieNode node, int position);
/*End of pointer to node argument getters*/

/*With node argument*/
int getNumOfChildren(struct trie_node node);
int getIs_Final(struct trie_node node);
int getActualNumOfChildren(struct trie_node node);
char* getWord(struct trie_node node);
pointerToTrieNode getChildren(struct trie_node node);
/*End of node argument getters*/

/*                              END OF GETTERS                              */

/*             						SETTERS  			    				*/
/*With pointer to node argument*/

void setNumOfChildrenPtr(pointerToTrieNode node, int numofchildren);
void setIs_FinalPtr(pointerToTrieNode node, int finality);
void setActualNumOfChildrenPtr(pointerToTrieNode node, int actualnumofchildren);
void setWordPtr(pointerToTrieNode node, char* theWord);
void setChildrenPtr(pointerToTrieNode node);
/*End of pointer to node argument setters*/

/*With node argument*/
void setNumOfChildren(struct trie_node node, int numofchildren);
void setIs_Final(struct trie_node node, int finality);
void setActualNumOfChildren(struct trie_node node,int actualnumofchildren);
void setWord(struct trie_node node, char* theWord);
void setChildren(struct trie_node node);
/*End of node argument setters*/

/*                               END OF SETTERS                              */