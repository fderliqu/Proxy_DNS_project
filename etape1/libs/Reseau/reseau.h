#ifndef _RESEAU_H_
#define _RESEAU_H_
#include<sys/types.h>
#include<sys/socket.h>
#include<netdb.h>
#include"dns.h"
#include"dns_server.h"
int initialisationSocketUDP(char *);

int boucleServeurUDP(int,void (* traitement)(int,unsigned char*,int,struct sockaddr*,int));

int messageUDP(
		char *,
		char *,
		unsigned char *,
		int);

#endif

