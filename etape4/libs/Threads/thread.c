#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<string.h>
#include"thread.h"

#ifdef DEBUG
	int debug = 1;
#endif
#ifndef DEBUG
	int debug = 0;
#endif

pthread_mutex_t myMutex;

int mutex_init()
{
	int status = pthread_mutex_init(&myMutex,NULL);
	if(status != 0)return -1;
	return 0;
}

int thread_P(){
	int status = pthread_mutex_lock(&myMutex);
	if(status!=0)return -1;
	return 0;
}

int thread_V(){
	int status = pthread_mutex_unlock(&myMutex);
	if(status!=0)return -1;
	return 0;
}

static void * fct_Thread(void * arg){
	int status;
	arg_thread_t * args = arg;
	args->fct(arg);
	free(args->arg);
	free(arg);
	pthread_exit((void *)&status);
}

int launchThread(void *(*fct)(void *),void * arg, size_t argSize){
	if(debug)printf("entre lauchthread\n");
	arg_thread_t * args = malloc(sizeof(arg_thread_t));
	if(arg != NULL){
		args->arg = malloc(argSize);
		memcpy(args->arg,arg,argSize);
	}
	else args->arg = NULL;
	args->fct = fct;
	pthread_t tid;
	int status = pthread_create(&tid,NULL,fct_Thread,args);
	if(status != 0)return -1;
	pthread_detach(tid);
	return 0;
}

