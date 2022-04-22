#ifndef __THREAD_H__
#define __THREAD_H__
int launchThread(void *(*fct)(void *),void *,size_t);
int mutex_P(int);
int mutex_V(int);
int mutex_init();
int mutex_destroy();
typedef struct arg_thread_s{
	void * arg;
	void * (*fct)(void *);
}arg_thread_t;
#endif

	
