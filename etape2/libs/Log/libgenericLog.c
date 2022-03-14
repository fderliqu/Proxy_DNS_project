//code pour bibliotheque dynamique genericLog

#include<stdio.h>
#include<stdlib.h>
#include<dlfcn.h>
#include"genericLog.h"

#define LIB_MAX_SIZE 100

void * handle;

int loadStrategy(char lib_name[LIB_MAX_SIZE]){
	handle = dlopen(lib_name,RTLD_LAZY);
	if(!handle) {
		return -1;
	}
	return 0;
	}

int initStrategy(void * f){
	char * error;
	int (*initLog)(void*);
	initLog = dlsym(handle,"initLog");
	if ((error = dlerror()) != NULL)  {
      		return -1;
	}
	(*initLog)(f);
	return 0;
}

int logStrategy(logMsg_t * msg){
	char * error;
	static int (*addLog)(logMsg_t*) = NULL;
	if(addLog == NULL){
		addLog = dlsym(handle,"addLog");
		if ((error = dlerror()) != NULL)  {
			return -1;
		}
	}
	(*addLog)(msg);
	return 0;
}

int endStrategy(void * f){
	char *error;
	int (*endLog)(void *);
	endLog = dlsym(handle,"endLog");
	if ((error = dlerror()) != NULL)  {
      		return -1;
	}
	(*endLog)(f);
	dlclose(handle);
	return 0;
}
	
