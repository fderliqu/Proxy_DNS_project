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

static void * fct_Thread(void * arg){
	arg_thread_t * args = arg;
	args->fct(args->arg);
	free(args->arg);
	free(args);
	return 0;
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

