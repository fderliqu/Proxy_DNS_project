//Main.c

#include<stdio.h>
#include<signal.h>
#include<stdlib.h>
#include<unistd.h>
#include<dlfcn.h>
#include<string.h>
#include<stdbool.h>
#include"genericLog.h"
#include"dns.h"
#include"dns_server.h"
#include"reseau.h"
#include"args.h"
#include"thread.h"
#include"memoire.h"

#define DEFAULT_SERVER "193.48.57.48"
#define DEFAULT_PORT "53"
#define DEFAULT_STRAT "none"
#define DEFAULT_INIT "default_output.txt"
#define MAX_SERVER 1024
#define MAX_PORT 1024
#define MAX_STRAT 1024

#ifndef DEBUG
int dbg = 0;
#endif

#ifdef DEBUG
int dbg = 1;
#endif

char server[MAX_SERVER] = DEFAULT_SERVER, port[MAX_PORT] = DEFAULT_PORT, strategie[MAX_STRAT]= DEFAULT_STRAT, init_args_strategie[MAX_STRAT] = DEFAULT_INIT;

int s; //La socket de lecture en global pour l'utiliser dans la fonction handler de signal (fonction fn)

typedef struct arg_s{
	int s;
	unsigned char msg[DNS_UDP_MAX_PACKET_SIZE];
	int taille_msg;
	struct sockaddr_storage adresse;
	int taille;
}arg_t;

//Fonction handler pour la réception du signal d'arrêt
void fn(){
	#ifdef DEBUG
	printf("\nKILL BY SIGINT, CLOSE EVERYTHING PROPERLY\n");
	#endif

	close(s); //Arrêt de la socket de lecture

	#ifdef DEBUG
	printf("Arrêt du socket de réception = succès\n");
	#endif
	
	if( strcmp(strategie,DEFAULT_STRAT) != 0 )endStrategy(); //Arrêt de la biblio de stratégie

	#ifdef DEBUG
	printf("Arrêt de la biblio = succès\n\n");
	#endif

	exit(-1);
}
/*
void proxy_dns(int s,unsigned char* requetes,int taille_requetes,struct sockaddr * adresse, int taille){

	#ifdef DEBUG
	printf("Message recu : \n-> ");
	for(int i=0;i<taille_requetes;i++)printf("%x ",requetes[i]);
	printf("\n");
	#endif

	logMsg_t* msg = malloc(sizeof(logMsg_t)-1+taille_requetes); //Allocation de la taille nécéssaire de la structure logmsg_t
	msg->size = taille_requetes;
	memcpy(msg->msg,requetes,taille_requetes);
	if( strcmp( strategie, DEFAULT_STRAT) != 0){ //On lance logStrategy si et seulement l'utilisateur à spécifier une stratégie
		int status = logStrategy(msg);
		if(status == -1)exit(-1);
	}
	free(msg);
	int nboctets = messageUDP(server,port,requetes,taille_requetes);//Envoie du message vers le serveur DNS et réception de la réponse
        sendto(s,requetes,nboctets,0,adresse,taille);//Envoie de la réponse vers le client initial
}
*/

void * log_thread(void * arg){
	if(arg != NULL)return NULL;
	if(dbg)printf("Dans fonction log thread\n");
	u_int8_t taille_msg;
	void * tampon;
	while(1){
		while(memoryIsEmpty()){
			sleep(1);
		}
		tampon = readMemory(&taille_msg);
		logMsg_t* msg = malloc(sizeof(logMsg_t)-1+taille_msg);
		msg->size = taille_msg;
		memcpy(msg->msg,tampon,taille_msg);
		if(dbg){
			printf("message on log thread: ");
			unsigned char* octet = (unsigned char *)msg->msg;
			for(int i=0;i<msg->size;i++){
				printf("%02x ",*octet);
				octet++;
			}
			printf("\n");
		}
		if(strcmp(strategie,DEFAULT_STRAT) != 0){
			int status = logStrategy(msg);
			if(status == -1)exit(-1);
		}
		free(msg);
	}
	return NULL;
}

void * proxy_thread(void * arg){
	#ifdef DEBUG
	printf("Est dans la fonction proxy_fct\n");
	#endif
	arg_t * args = arg;
	#ifdef DEBUG
	printf("message on proxy thread = ");
	for(int i=0;i<args->taille_msg;i++){
		printf("%02x ",args->msg[i]);
	}
	#endif
	writeMemory(args->msg,args->taille_msg);
	int nboctets = messageUDP(server,port,args->msg,args->taille_msg);//Envoie du message vers le serveur DNS et réception de la réponse
        sendto(s,args->msg,nboctets,0,(struct sockaddr *)&args->adresse,args->taille);//Envoie de la réponse vers le client initial
	return NULL;
}

void proxy_dns(int s, unsigned char* message, int taille_message, struct sockaddr * adresse, int taille){
	arg_t arg;
	arg.s = s;
	memcpy(arg.msg,message,taille_message);
	arg.taille_msg = taille_message;
	memcpy(&arg.adresse,adresse,taille);
	arg.taille = taille;
	int status = launchThread(proxy_thread,&arg,sizeof(arg_t));
	if(status != 0){
		printf("Erreur : changement du thread dns\n");
		exit(-1);
	}
}


int main(int argc,char * argv[]){
	int status = args(argc,argv,server,port,strategie,init_args_strategie); //Réception des arguments
	if(status == -1)return 0;

	#ifdef DEBUG
	if(status == 0)printf("ARGS\nserver : %s\nport : %s\nstratgy : %s\nstrategy args : %s\n\n",server,port,strategie,init_args_strategie);
	#endif

	if( strcmp(strategie,DEFAULT_STRAT) != 0 )status = loadStrategy(strategie); //Ouverture de la biblio de la stratégie si et seulement si l'utilisateur à spécifier une stratégie
	if(status == -1){
		printf("Erreur Loggin : La startégie n'a pas pu être lancée\n");
		return(0);
	}	
	
	#ifdef DEBUG
	if(strcmp(strategie,DEFAULT_STRAT) != 0 && status == 0)printf("Lancement de loadStrategy de libgenericLog -> OK\n");
	#endif 

	if( strcmp(strategie,DEFAULT_STRAT) != 0 )status = initStrategy(init_args_strategie); //Ouverture de la biblio de la stratégie si et seulement si l'utilisateur à spécifier une stratégie

	#ifdef DEBUG
	if(strcmp(strategie,DEFAULT_STRAT) != 0 && status == 0)printf("Lancement de initStrategy de libgenericLog -> OK\n");
	#endif

	signal(SIGINT,fn);//Création du signal
	s = initialisationSocketUDP(port); //Création socket de lecture
	size_t size = 256;
	status = allocateMemory(size);
	status = launchThread(log_thread,NULL,0);
	if(status != 0){
		printf("Erreur chargement du thread log");
		exit(-1);
	}
	boucleServeurUDP(s,proxy_dns); //Entrée dans la boucle infini 
	return 0;
}

