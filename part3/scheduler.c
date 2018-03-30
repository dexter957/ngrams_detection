#include "scheduler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#define perror2(s,e) fprintf(stderr, "%s: %s\n", s, strerror(e))

/*Some shared variables*/
int actualNumberOfJobs;
int* ROUND;
int* threadRounds;
int JOBSRAN;
int queuePosition=0;
int queuedJobs;/*Number of jobs in the queue*/
int queueLength;/*The length of the queue*/
int activeThreads;/*The number of threads that haven't finished running*/
int EXIT;/*Indicates whether scheduler is done or not*/

pointerToJob* jobsQueue;
pthread_mutex_t jobsQueueMtx;
pthread_mutex_t jobsRanMtx;
pthread_mutex_t workerSleepMtx;
pthread_cond_t jobsRan;
pthread_cond_t workerSleep; 
/*End of shared variables*/


struct Job
{
	funcPointer function;
	void* arguments;
};


struct Scheduler
{
	int numberOfThreads;/*Total number of threads in the pool*/
	int firstRun;
	pthread_t *threads;
};



/*****************************************  JOB CODE  **********************************************************/

void createJob(pointerToJob* aJob, funcPointer theFunction, void* argumentList)
{
	(*aJob)=malloc(sizeof(struct Job));
	(*aJob)->function=theFunction;
	(*aJob)->arguments=argumentList;
}


void executeJob(int position)
{//TriePtr, char**, int
	(jobsQueue[position]->function) ((jobsQueue[position]->arguments)) ;
	return;
}

void deleteJob(pointerToJob aJob)
{
	free(aJob);
}


/***************************************** END OF JOB CODE ****************************************************/




/*Threads' code*/

void *runJob(void* arg)
{
	int myQueuePosition;
	int* myRound=arg;
	(*myRound)=0;
	while(TRUE)
	{
		while(TRUE)
		{
			/*Get your query to run*/
			pthread_mutex_lock(&jobsQueueMtx);
			if(actualNumberOfJobs>0)
			{
				--actualNumberOfJobs;
				myQueuePosition=queuePosition++;
				pthread_mutex_unlock(&jobsQueueMtx);
			}
			else
			{
				pthread_mutex_unlock(&jobsQueueMtx);
				break;
			}
			/*Run query*/
			int i;
			(jobsQueue[myQueuePosition]->function) ((jobsQueue[myQueuePosition]->arguments)) ;
			free(jobsQueue[myQueuePosition]);
		}
		pthread_mutex_lock(&jobsRanMtx);
		--activeThreads;
		if(activeThreads==0)
		{/*Wake up master*/
			JOBSRAN=TRUE;
			pthread_cond_signal(&jobsRan);
		}
		pthread_mutex_unlock(&jobsRanMtx);
		/*Now worker will check exit condition*/
		++(*myRound);
		pthread_mutex_lock(&workerSleepMtx);
		while((*myRound)==(*ROUND))
		{
			pthread_cond_wait(&workerSleep,&workerSleepMtx);/*Workers sleep here until master tells them to run again or exit*/
		}
		if(EXIT==TRUE)
		{
			pthread_mutex_unlock(&workerSleepMtx);
			break;
		}
		pthread_mutex_unlock(&workerSleepMtx);
	}
	pthread_exit(0);
}





/*End of threads' code*/


/************************************ SCHEDULER CODE **************************************************/

int setScheduler(pointerToScheduler* theScheduler, int threads)
{
	//printf("Came to create the scheduler\n");
	(*theScheduler)=malloc(sizeof(struct Scheduler));
	if((*theScheduler)==NULL)
	{
		return FALSE;
	}
	(*theScheduler)->numberOfThreads=threads;
	(*theScheduler)->firstRun=TRUE;
	(*theScheduler)->threads=NULL;

	/*Now initialise the various shared variables*/
	queuePosition=0;
	queuedJobs=0;/*Number of jobs in the queue*/
 	queueLength=QUEUESIZE;/*The length of the queue*/
    activeThreads=0;/*The number of threads that haven't finished running*/
    EXIT=FALSE;
	jobsQueue=malloc(queueLength*sizeof(pointerToJob));
	ROUND=malloc(sizeof(int));
	(*ROUND)=0;

	/*Now initialise mutexes and condition variables*/
	pthread_mutex_init(&jobsQueueMtx, 0);
	pthread_mutex_init(&jobsRanMtx, 0);
	pthread_mutex_init(&workerSleepMtx, 0);
	pthread_cond_init(&jobsRan, 0);
	pthread_cond_init(&workerSleep, 0);

	return TRUE;
}


int submitJob(pointerToScheduler theScheduler, pointerToJob jobToRun )
{
	/*Check if the queue is spacey enough*/
	int newQueuedJobs=queuedJobs+1;
	//printf("The number of new jobs is %d\n",newQueuedJobs );
	if(newQueuedJobs>queueLength)
	{/*Double the queue size*/
		jobsQueue=realloc(jobsQueue,2*queueLength*sizeof(pointerToJob));
		queueLength*=2;
	}
	jobsQueue[queuedJobs]=jobToRun;
	queuedJobs=newQueuedJobs;

	return TRUE;
}



void executeAllJobs(pointerToScheduler theScheduler)
{/*Now the sceduler is left to execute all jobs*/
	queuePosition=0;
	actualNumberOfJobs=queuedJobs;
	activeThreads=theScheduler->numberOfThreads;
	/*First create threads*/
	/*Create thread pool only if it is your first run*/
	if(theScheduler->firstRun==TRUE)
	{
		theScheduler->threads=malloc((theScheduler->numberOfThreads)*sizeof(pthread_t));
		threadRounds=malloc(sizeof(int)*(theScheduler->numberOfThreads));
		int i;
		++(*ROUND);
		for(i=0;i<(theScheduler->numberOfThreads);++i)
		{
			pthread_create(&(theScheduler->threads[i]),NULL,runJob,&(threadRounds[i]));
		}
	}
	else
	{/*Wake up your threads to do the work*/
	    pthread_mutex_lock(&workerSleepMtx);
		++(*ROUND);
		pthread_cond_broadcast(&workerSleep);
		pthread_mutex_unlock(&workerSleepMtx);
	}
	/*Now wait until all queries are ran*/
	pthread_mutex_lock(&jobsRanMtx);
	while(JOBSRAN==FALSE)
	{
		pthread_cond_wait(&jobsRan,&jobsRanMtx);
	}
	JOBSRAN=FALSE;
	pthread_mutex_unlock(&jobsRanMtx);
	if((theScheduler->firstRun)==FALSE)
	{
		;
	}
	else
	{
		theScheduler->firstRun=FALSE;
	}
	queuedJobs=0;
	return;
}


int destroyScheduler(pointerToScheduler theScheduler)
{
	/*Wake up your workers to terminate*/
	pthread_mutex_lock(&workerSleepMtx);
	EXIT=TRUE;
	(++ROUND);
    pthread_cond_broadcast(&workerSleep);
    pthread_mutex_unlock(&workerSleepMtx);
    /*Wait for workers to terminate*/
	int i;
	for(i=0;i<theScheduler->numberOfThreads;++i)
    {
        pthread_join(theScheduler->threads[i], 0);
    }

	free(jobsQueue);
	free(theScheduler->threads);
	free(theScheduler);
	free(threadRounds);

	/*Release the resources*/
	pthread_cond_destroy(&jobsRan);
	pthread_cond_destroy(&workerSleep);
	pthread_mutex_destroy(&jobsRanMtx);
	pthread_mutex_destroy(&jobsQueueMtx);
	pthread_mutex_destroy(&workerSleepMtx);


	return TRUE;
}



/************************************** END OF SCHEDULER CODE **************************************************/




