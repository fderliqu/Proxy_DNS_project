#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdint.h>
#include <unistd.h>
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



