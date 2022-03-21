#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<string.h>
#include"thread.h"

#ifdef SEMAPHORE
pthread_mutex_t myMutex;
#endif

typedef struct arg_s{
	void * arg;
	void * (*fct)(void *);
} arg_t;

int launchThread(void *(*fct)(void *),void * arg, size_t argSize){
	#ifdef SEMAPHORE
		pthread_mutex_init(&myMutex,NULL);
	#endif
	arg_t * args = malloc(sizeof(struct arg_s) + argSize);
	if(arg != NULL){
		memcpy(args->arg,arg,argSize);
	}
	else args->arg = NULL;
	args->fct = endThread;
	pthread_t tid;
	int status = pthread_create(&tid,NULL,fct,(void *)args);
	if(status != 0)return -1;
	pthread_detach(tid);
	return 0;
}

void * endThread(void * arg){
	free(arg);
	return 0;
}
