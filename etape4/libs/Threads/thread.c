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
#define N_MUTEX 8
pthread_mutex_t myMutex[N_MUTEX];

int mutex_init()
{
	int status;
	for(int i =0;i<N_MUTEX;i++){
		status = pthread_mutex_init(&myMutex[i],NULL);
		if(status != 0)return -1;
	}
	return 0;
}

int mutex_destroy(){
	int status;
	for(int i =0;i<N_MUTEX;i++){
		status = pthread_mutex_destroy(&myMutex[i]);
		if(status != 0)return -1;
	}
	return 0;
}

int mutex_P(int i){
	int status = pthread_mutex_lock(&myMutex[i]);
	if(status!=0)return -1;
	return 0;
}

int mutex_V(int i){
	int status = pthread_mutex_unlock(&myMutex[i]);
	if(status!=0)return -1;
	return 0;
}

static void * fct_Thread(void * arg){
	int status,launchMutex=0;
	arg_thread_t * args = arg;
	if(args->arg != NULL)launchMutex=1; //On ne lock de mutex pour le thread de log car boucle infini
	if(launchMutex)mutex_P(0);
	args->fct(args->arg);
	free(args->arg);
	free(args);
	if(launchMutex)mutex_V(0);
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

