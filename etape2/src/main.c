//Main.c

#include<stdio.h>
#include<signal.h>
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
#define DEFAULT_STRAT "none"
#define DEFAULT_INIT "fichier.txt"
#define MAX_SERVER 1024
#define MAX_PORT 1024
#define MAX_STRAT 1024

char server[MAX_SERVER] = DEFAULT_SERVER, port[MAX_PORT] = DEFAULT_PORT, strategie[MAX_STRAT]= DEFAULT_STRAT, init_args_strategie[MAX_STRAT] = DEFAULT_INIT;
/*
void fn(void * k){
	printf("KILL BY SIGINT, CLOSE EVERYTHING PROPERLY\n");
	close(socket);
	if( strcmp(strategie,DEFAULT_STRAT) != 0 )endStrategy();
	return;
}
*/
void proxy_dns(int s,unsigned char* requetes,int taille_requetes,struct sockaddr * adresse, int taille){
	#ifdef DEBUG
	printf("Message recu : \n-> ");
	for(int i=0;i<taille_requetes;i++)printf("%x ",requetes[i]);
	printf("\n");
	#endif
	logMsg_t* msg = malloc(sizeof(logMsg_t)-1+taille_requetes);
	msg->size = taille_requetes;
	memcpy(msg->msg,requetes,taille_requetes);
	if( strcmp( strategie, DEFAULT_STRAT) != 0){
		int status = logStrategy(msg);
		if(status == -1)exit(-1);
	}
	free(msg);
	int nboctets = messageUDP(server,port,requetes,taille_requetes);
        sendto(s,requetes,nboctets,0,adresse,taille);
}

int main(int argc,char * argv[]){
	int status = args(argc,argv,server,port,strategie,init_args_strategie);
	if(status == -1)return 0;

	#ifdef DEBUG
	if(status == 0)printf("Args : \nserver : %s\nport : %s\nstratgy : %s\n strategy args : %s\n",server,port,strategie,init_args_strategie);
	#endif

	status = loadStrategy(strategie);
       	if(status == -1)printf("Erreur Loggin : La startégie n'a pas pu être lancée ou aucune stratégie n'a été mis en paramètre -> Lancement du programme sans stratégie de loggin\n");	
	
	#ifdef DEBUG
	if(status == 0)printf("Lancement de loadStrategy de libgenericLog -> OK\n");
	#endif 

	if( strcmp(strategie,DEFAULT_STRAT) != 0 )status = initStrategy(init_args_strategie);
	
	#ifdef DEBUG
	if(status == 0)printf("Lancement de initStrategy de libgenericLog -> OK\n");
	#endif
	//signal(SIGINT,fn);
	int s=initialisationSocketUDP(port);
	boucleServeurUDP(s,proxy_dns);
	close(s);
	endStrategy();
	return 0;
}

