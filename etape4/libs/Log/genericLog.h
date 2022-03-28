#ifndef _GENERICLOG_H__
#define _GENERICLOG_H__

#include "log_dns.h"

int loadStrategy(char *);
int initStrategy(void *);
int logStrategy(logMsg_t *);
int endStrategy();


#endif
