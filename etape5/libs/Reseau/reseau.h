#ifndef _RESEAU_H_
#define _RESEAU_H_

int initialisationSocketUDP(char *);

//int send_rep_proxy_dns(int,unsigned char*,int,struct sockaddr *,int);
int send_rep_proxy_dns(int,unsigned char*,int,void *,int);


//int boucleServeurUDP(int,void (* traitement)(int,unsigned char*,int,struct sockaddr*,int));
int boucleServeurUDP(int,void (* traitement)(int,unsigned char*,int,void *,int));

int messageUDP(
		char *,
		char *,
		unsigned char *,
		int);

#endif

