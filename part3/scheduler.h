//#include "simpleFunc.h"
#include "trie.h"
#include "./trie_nodes/trie_node_connector.h"

#define TRUE 1
#define FALSE 0
#define QUEUESIZE 250
#define NUMBEROFTHREADS 9

typedef void (*funcPointer) (void*);


typedef struct Job* pointerToJob; 
typedef struct Scheduler* pointerToScheduler;


void createJob(pointerToJob* aJob, funcPointer theFunction, void* argumentList);

int setScheduler(pointerToScheduler* theScheduler, int threads);
int submitJob(pointerToScheduler theScheduler, pointerToJob jobToRun );
void executeAllJobs(pointerToScheduler theScheduler);
int destroyScheduler(pointerToScheduler theScheduler);