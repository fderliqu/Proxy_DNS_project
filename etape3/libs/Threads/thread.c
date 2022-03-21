#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<string.h>
#include"thread.h"

#ifdef SEMAPHORE
pthread_mutex_t myMutex;
#endif

void * fct_Thread(void * arg){
	void * void_arg = ((arg_thread_t *)arg)->arg;
	arg_t * arg_s = (arg_t *)void_arg;
	//memcpy(&args,((arg_thread_t *)arg)->arg,sizeof(((arg_thread_t *)arg)->argSize));
#ifdef DEBUG
	printf("On fct_thread : %d %d %d \n",arg_s->s,arg_s->taille_msg,arg_s->taille);
#endif
	free(arg);
	return 0;
}

int launchThread(void *(*fct)(void *),void * arg, size_t argSize){
	#ifdef SEMAPHORE
		pthread_mutex_init(&myMutex,NULL);
	#endif

	arg_thread_t * args = malloc(sizeof(struct arg_thread_s) + argSize);
	if(arg != NULL){
		memcpy(args->arg,arg,argSize);
	}
	else args->arg = NULL;
	args->fct = fct;
	pthread_t tid;
	int status = pthread_create(&tid,NULL,fct_Thread,(void *)args);
	if(status != 0)return -1;
	pthread_detach(tid);
	return 0;
}

