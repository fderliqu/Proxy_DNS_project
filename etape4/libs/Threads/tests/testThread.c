#include<stdio.h>
#include<stdlib.h>
#include<assert.h>

#include"thread.h"
#include<pthread.h>


#ifdef DEBUG
int debug = 1;
#endif
#ifndef DEBUG
int debug = 0;
#endif

#define N_MUTEX 2
pthread_mutex_t myMutex[N_MUTEX];
//Fonction de test mutex

void testInitMutex()
{
	printf("Test de la fonction mutex_init, init d'un tableau de %d mutex\n",N_MUTEX);
	int status = mutex_init();
	assert(status == 0);
	printf("Success\n");
}

void testLockMutex(int i){
	printf("Test de lock d'un mutex\n");
	int status = mutex_P(i);
	assert(status == 0);
	printf("Success\n");
}

void testIsAlreadyLockMutex(int i){
	printf("Test de vérification si un mutex est déja lock");
	int status = mutex_P(i);
	assert(status != 0);
	printf("Success\n");
}

void testUnlockMutex(int i){
	printf("Test d'unlock d'un mutex\n");
	int status = mutex_V(i);
	assert(status == 0);
	printf("Success\n");
}

void testIsAlreadyUnlockMutex(int i){
	printf("Test de vérification si un mutex est déja unlock\n");
	int status = mutex_V(i);
	assert(status != 0);
	printf("Success\n");
}

void testLaunchThread(void *(*fct)(void *),void * arg, size_t argSize){
	printf("Test de vérification de launchThread\n");
	if(arg != NULL)printf("Dans fct principal : arg = %d",*(int*)arg);
	int status = launchThread(fct,arg,argSize);
	assert(status == 0);
	printf("Success\n");
}

void * fctArgsTest(void * arg){
	printf("Dans thread : valeur de l'argument : %d\n",*(int*)arg);
	return NULL;
}

void * fctMutexTest(void * arg){
	if(arg != NULL)printf("ooo\n");
	testLockMutex(0);
	//testIsAlreadyLockMutex(0);
	testUnlockMutex(0);
	//testIsAlreadyUnlockMutex(0);
	return NULL;
}

int main(int argc, char * argv[]){
	assert(argc == 1);
	testInitMutex();
	int x = 123456789;
	testLaunchThread(fctArgsTest,(void *)&x,sizeof(int));
	testLaunchThread(fctMutexTest,NULL,0);
	pthread_exit(NULL);
	return 0;
}

