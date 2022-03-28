int launchThread(void *(*fct)(void *),void *,size_t);
int mutex_P();
int mutex_V();
int mutex_init();
typedef struct arg_thread_s{
	void * arg;
	void * (*fct)(void *);
}arg_thread_t;

	
