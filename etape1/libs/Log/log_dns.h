#ifndef __LOG_DNS_H__
#define __LOG_DNS_H__

typedef struct logMsg_s {
	int size;
	char * msg;
} logMsg_t;

int initLog(void *);
int addLog(logMsg_t *);

#endif
