//#include "trie_node.h"
//#include "semistatic_trie_node.h"
//#include "semistatic_trie_node_v2.h"
#include <time.h>
//#include "trie.h"
#include "cli2.h"

int main(int argc, char *argv[]){
	
	if(argc<5)
	{
		printf("Not enough input parameters\nExiting . . .\n");
	}
	int initFilePos=-1;
	int queryFilePos=-1;
	int i;
	for(i=1;i<argc;++i)
	{
		if(strcmp(argv[i],"-i")==0)
		{
			initFilePos=i+1;
			continue;
		}
		if(strcmp(argv[i],"-q")==0)
		{
			queryFilePos=i+1;
			continue;
		}
	}
	//TriePtr trie = init_trie();--OLD
	TriePtr trie;
	if(trie == NULL){
		printf("error");
		return 1;
	}

	//initFile(argv[initFilePos], trie);--OLD
	initFile(argv[initFilePos],&trie);
	if(getStatic(trie)) shrinkStaticTrie(trie);
//	shrinkStaticTrie(trie);
	//printTrie(trie);

	//////clock_t start = clock();
	struct timespec start, finish;
	clock_gettime(CLOCK_MONOTONIC,&start);
	screenOrFile(FILEIN, argv[queryFilePos], trie);
	clock_gettime(CLOCK_MONOTONIC,&finish);
	double elapsed=finish.tv_sec-start.tv_sec;
	elapsed+=(finish.tv_sec-start.tv_sec)/1000000000.0;
	//////clock_t finish = clock();

	fprintf(stderr,"\n\nElapsed: %f seconds\n", elapsed);
    //fprintf(stderr,"\n\nElapsed: %f seconds\n", (double)(finish - start) / CLOCKS_PER_SEC);


	clear_trie(trie);

	return 0;
}