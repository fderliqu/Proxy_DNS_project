//Main.c

#include<stdio.h>
#include<stdint.h>
#include<signal.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<stdbool.h>
#include"genericLog.h"
#include"dns.h"
#include"dns_server.h"
#include"reseau.h"
#include"args.h"
#include"thread.h"
#include"memoire.h"
#include"shmDNS.h"

#define DEFAULT_SERVER "193.48.57.48"
#define DEFAULT_PORT "53"
#define DEFAULT_STRAT "none"
#define DEFAULT_CONFIG "none"
#define DEFAULT_INIT "default_output.txt"
#define MAX_SERVER 1024
#define MAX_PORT 1024
#define MAX_STRAT 1024
#define MAX_CONFIG 1024
#define MAX_MSG 1024

#ifndef DEBUG
int dbg = 0;
#endif

#ifdef DEBUG
int dbg = 1;
#endif

char server[MAX_SERVER] = DEFAULT_SERVER, port[MAX_PORT] = DEFAULT_PORT, strategie[MAX_STRAT]= DEFAULT_STRAT, init_args_strategie[MAX_STRAT] = DEFAULT_INIT, configfile[MAX_CONFIG] = DEFAULT_CONFIG;

int s; //La socket de lecture en global pour l'utiliser dans la fonction handler de signal (fonction fn)

int arret=0;
int fin=0;

typedef struct arg_s{
	int s;
	unsigned char msg[DNS_UDP_MAX_PACKET_SIZE];
	int taille_msg;
	void * adresse;
	int taille;
}arg_t;

int shmid;
struct mgr_s * shared_mem;
int shared_mem_size;
int redirect;

//Fonction handler pour la réception du signal d'arrêt
void fn(){
	arret=1;

	if(dbg)printf("\nKILL BY SIGINT, CLOSE EVERYTHING PROPERLY\n");
	
	close(s); //Arrêt de la socket de lecture
	if(dbg)printf("Arrêt du socket de réception = succès\n");
	
	endStrategy(); //Arrêt de la biblio de stratégie
	if(dbg)printf("Arrêt de la biblio = succès\n\n");
	
	desallocateMemory();//Désalloue la memoire

	free_shm_addr((void*)shared_mem);//Désalloue l'instance de memshared
	free_shmid(shmid);//Suppression de sharedmem ipc

	sleep(2);//On attend 1 ou 2 seconde pour que le thread se termine proprement

	mutex_destroy(); //On détruit les mutex après la fin de ces derniers
	exit(-1);
}

//Fonction thread de log

void * log_thread(void * arg){
	free(arg); //Pour éviter le warning unused variable
	if(dbg)printf("Dans fonction log thread\n");
	uint8_t taille_msg;
	void * tampon;
	while(1){
		//boucle infinie si la mémoire est vide, on ne lit pas
		while(memoryIsEmpty()){
			sleep(1);
			if(arret == 1)break;
		}
		if(arret == 1)break;
		//Arrivée d'un write, donc lecture
		tampon = readMemory(&taille_msg);
		logMsg_t* msg = malloc(sizeof(logMsg_t)-1+taille_msg);//Création de log_msg_t
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
		//Lancement de la logStrat
		if(strcmp(strategie,DEFAULT_STRAT) != 0){
			int status = logStrategy(msg);
			if(status == -1)exit(-1);
		}
		free(msg);
	}
	if(dbg)printf("Arret 3\n");
	return NULL;
}

void * proxy_thread(void * arg){
	arg_t * args = arg;

	#ifdef DEBUG
	printf("Est dans la fonction proxy_fct\n");
	printf("message on proxy thread = ");
	for(int i=0;i<args->taille_msg;i++){
		printf("%02x ",args->msg[i]);
	}
	printf("\n");
	#endif

	//Ecriture vers la mémoire
	int status = writeMemory(args->msg,args->taille_msg);

	if(dbg)printf("status = %d\n",status);
	if(!redirect){
		int nboctets = messageUDP(server,port,args->msg,args->taille_msg);//Envoie du message vers le serveur DNS et réception de la réponse
        	status = send_rep_proxy_dns(s,args->msg,nboctets,args->adresse,args->taille);
	}
	free(args->adresse);
	return NULL;
}

void fake_msg(unsigned char* final_msg, int * p_final_size, unsigned char * data, int data_size, unsigned char type)
{ //avec type = 0x01 pour ipv4 / 0x1c pour IPv6 / 0x0f pour mx
	//modifications du header
	final_msg[2]+=128; //On change pour avoir QR à 1
	final_msg[3]+=128; //On met RA à 1 et Z et RCODE à 0
	final_msg[6]=0x00; //On met ANCOUNT à 0x00 01
	final_msg[7]=0x01; 


	//NAME
	final_msg[*p_final_size] = 0xc0;
	final_msg[++(*p_final_size)] = 0x0c;

	//TYPE
	final_msg[++(*p_final_size)] = 0x00;
	final_msg[++(*p_final_size)] = type;
	//CLASS
	final_msg[++(*p_final_size)] = 0x00;
	if(type != 0x0f)final_msg[++(*p_final_size)] = 0x01;
	else final_msg[++(*p_final_size)] = 0x06;

	//TTL
	final_msg[++(*p_final_size)] = 0x00;
	final_msg[++(*p_final_size)] = 0x00;
	final_msg[++(*p_final_size)] = 0x02;
	final_msg[++(*p_final_size)] = 0x42;

	//RDLENGTH
	final_msg[++(*p_final_size)] = data_size / 255;
	final_msg[++(*p_final_size)] = data_size % 255;

	//RDATA
	for (int i=0; i<data_size; i++)
	{
		final_msg[++(*p_final_size)] = data[i];
	}

	*p_final_size += 1;
	if(dbg){
		printf("%d\n",*p_final_size);
		for(int i=0;i<*p_final_size;i++)printf("%02x ",final_msg[i]);
		printf("\n");
	}
	

}

void proxy_dns(int s, unsigned char* message, int taille_message, void * adresse, int taille){
	//test de redirection
	int size=0;
	unsigned char buffer[50];
	memset(buffer,0,strlen((char*)buffer));
	unsigned char *octet = message;
	struct mgr_s * p_shared_mem = shared_mem;
	octet += 12;
	int cpt=0; //Compteur et test de fin
        while (*octet != 0x00) //RFC1035 : Le QNAME se termine par le caractère NUL de code ascii 0
        {
                cpt=*octet; //Le nombre de caractère dans le groupe de caractères
		octet++;
                for (int i=0; i < cpt; i++)
                {
                        //if(dbg)printf("%c", *octet);
			buffer[size] = *octet;
                        octet++;
			size++;
                }
                if (*octet != 0x00) {
			//if(dbg)printf("."); //Fin du groupe de caractère
			buffer[size] = '.';
			size++;
		} 
	}
	int flag = 0,occ=0;
	if(dbg)printf("dans proxy dns : %s\n",buffer);
	while(strcmp(p_shared_mem->domaine,(char*)buffer) != 0){
		p_shared_mem++;
		occ++;
		if(occ == NB_SHM_DATA){
			flag=1;
			break;
		}
	}
	if(dbg){
		if(flag == 0)printf("redirection trouvée %02x %s",*(octet+2),p_shared_mem->domaine);
	}
	redirect=0;
	if(flag == 0){
		if(*(octet+2) == 0x0f){
			unsigned char data[4];
			memset(data,0,4);
			nomVersAdresse(p_shared_mem->mx,data);
			for(int i=0;i<4;i++){
				if(data[i] != 0x00)redirect=1;
			}
			if(redirect==1)fake_msg(message,&taille_message,data,4,*(octet+2));
		}
		if(*(octet+2) == 0x01){
			unsigned char data[4];
			memset(data,0,4);
			nomVersAdresse(p_shared_mem->ipv4,data);
			for(int i=0;i<4;i++){
				if(data[i] != 0x00)redirect=1;
			}
			if(redirect==1)fake_msg(message,&taille_message,data,4,*(octet+2));
		}
		if(*(octet+2) == 0x1c){
			unsigned char data[16];
			memset(data,0,16);
			nomVersAdresse(p_shared_mem->ipv6,data);
			for(int i=0;i<16;i++){
				if(data[i] != 0x00)redirect=1;
			}
			if(redirect==1)fake_msg(message,&taille_message,data,16,*(octet+2));
		}

		if(redirect)send_rep_proxy_dns(s,message,taille_message,adresse,taille);
	}

	if(dbg)for(int i=0;i<size;i++)printf("%02x ",buffer[i]);

	///Stokage des arguments if pas de redirection
	arg_t arg;
	arg.s = s;
	memcpy(arg.msg,message,taille_message);
	arg.taille_msg = taille_message;
	arg.adresse = malloc(taille);
	memcpy(arg.adresse,adresse,taille);
	arg.taille = taille;
	//Lancement thread de proxy
	int status = launchThread(proxy_thread,&arg,sizeof(arg_t));
	if(status != 0){
		printf("Erreur : changement du thread proxy\n");
		exit(-1);
	}
}



int main(int argc,char * argv[]){	
	int status = args(argc, argv,server, port, strategie, init_args_strategie, configfile); //Réception des arguments
	if(status == -1)return 0;

	#ifdef DEBUG
	if(status == 0)printf("ARGS\nserver : %s\nport : %s\nstratgy : %s\nstrategy args : %sconfigfile : %s\n\n",server,port,strategie,init_args_strategie,configfile);
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

//Partie redirection	
	shmid = get_shm_id(CLE,NB_SHM_DATA*sizeof(struct mgr_s),0);
	struct mgr_s * p_shared_mem;
	shared_mem = (struct mgr_s *)get_shm_addr(shmid);
	if(status < 0){
		perror("get_shm_id");exit(1);
	}
	p_shared_mem = shared_mem;
	if( strcmp(configfile,DEFAULT_CONFIG) != 0){
		FILE * file;
		file = fopen(configfile,"r");
		if(file == NULL){
			perror("fopen");exit(1);
		}
		char buf[500];
		while(fscanf(file,"%s\n",buf) == 1){
			tidy_mgr(p_shared_mem,buf);
			p_shared_mem++;
			shared_mem_size++;
		}
	}
	//Fin partie Redirection

	#ifdef DEBUG
	for(int i=0;i<4;i++)printf("Nom de domaine : %s  IPV4 : %s  IPV6 : %s  MX : %s\n", shared_mem[i].domaine, shared_mem[i].ipv4, shared_mem[i].ipv6, shared_mem[i].mx );
	#endif
	s = initialisationSocketUDP(port); //Création socket de lecture
	//Allocation de la mémoire de partage
	size_t size = 256;
	status = allocateMemory(size);
	//Init mutex
	status = mutex_init();
	//Lancement du thread de log
	status = launchThread(log_thread,NULL,0);
	if(status != 0){
		printf("Erreur chargement du thread log");
		exit(-1);
	}
	boucleServeurUDP(s,proxy_dns); //Entrée dans la boucle infini 
	return 0;
}

