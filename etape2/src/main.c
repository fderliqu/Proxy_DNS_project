#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<dlfcn.h>
#include<string.h>
#include"genericLog.h"
#include"dns.h"
#include"dns_server.h"
#include"reseau.h"
#include"args.h"

#define DEFAULT_SERVER "193.48.57.48"
#define DEFAULT_PORT "53"
#define DEFAULT_STRAT "libDNSinFILE.so"
#define DEFAULT_INIT "../bin/fichier.txt"
#define MAX_SERVER 1024
#define MAX_PORT 1024
#define MAX_STRAT 1024

char server[MAX_SERVER] = DEFAULT_SERVER, port[MAX_PORT] = DEFAULT_PORT, strategie[MAX_STRAT]= DEFAULT_STRAT, init_args_strategie[MAX_STRAT] = DEFAULT_INIT;

void proxy_dns(int s,unsigned char* requetes,int taille_requetes,struct sockaddr * adresse, int taille){
	/*
	printf("%d\n",taille_requetes);
	for(int i=0;i<taille_requetes;i++)printf("%x ",requetes[i]);
	printf("\n");
	*/
	logMsg_t* msg = malloc(sizeof(logMsg_t)-1+taille_requetes);
	msg->size = taille_requetes;
	memcpy(msg->msg,requetes,taille_requetes);
	logStrategy(msg);
	free(msg);
	int nboctets = messageUDP(server,port,requetes,taille_requetes);
        sendto(s,requetes,nboctets,0,adresse,taille);
}

int main(int argc,char * argv[]){
	int status=args(argc,argv,server,port,strategie,init_args_strategie);
	loadStrategy(strategie);
	initStrategy(init_args_strategie);
	//printf("%s %s\n",server,port);
	//if(status==-1)return 0;
	int s=initialisationSocketUDP(port);
	boucleServeurUDP(s,proxy_dns);
	close(s);
	return 0;
}

