#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include"dns.h"
#include"dns_server.h"
#include"reseau.h"
#include"args.h"

#define DEFAULT_SERVER "193.48.57.48"
#define DEFAULT_PORT "53"
#define MAX_SERVER 1024
#define MAX_PORT 1024

char server[MAX_SERVER] = DEFAULT_SERVER, port[MAX_PORT] = DEFAULT_PORT;

void proxy_dns(int s,unsigned char* requetes,int taille_requetes,struct sockaddr * adresse, int taille){
	int nboctets = messageUDP(server,port,requetes,taille_requetes);
        sendto(s,requetes,nboctets,0,adresse,taille);
}

int main(int argc,char * argv[]){
	int status=args(argc,argv,server,port);
	printf("%s %s\n",server,port);
	//if(status==-1)return 0;
	int s=initialisationSocketUDP(port);

	boucleServeurUDP(s,proxy_dns);
	close(s);
	return 0;
}

