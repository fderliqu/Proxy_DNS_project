int launchThread(void *(*fct)(void *),void *,size_t);
void * endThread(void *);

typedef struct arg_s{
	int s;
	unsigned char * msg;
	int taille_msg;
	struct sockaddr * adresse;
	int taille;
}arg_t;

typedef struct arg_thread_s{
	void * arg;
	void * (*fct)(void *);
	int argSize;
}arg_thread_t;

	
