int launchThread(void *(*fct)(void *),void *,size_t);

typedef struct arg_thread_s{
	void * arg;
	void * (*fct)(void *);
}arg_thread_t;

	
