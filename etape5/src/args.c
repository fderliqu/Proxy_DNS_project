#include<stdio.h>
#include"args.h"
#include<stdlib.h>
#include<getopt.h>
#include<string.h>

int args(int argc, char * argv[],char * server, char * port, char * strategie, char * init_args_strategie, char * configfile){
	int opt;
	while(1){
		static struct option long_option[] = {
			{"help",0,0,'h'},
			{"port",1,0,'p'},
			{"serveur",1,0,'s'},
			{"logstrategie",1,0,'l'},
			{"initlogstrategie",1,0,'i'},
			{"configfile",1,0,'c'},
			{0,0,0,0}
		};

		int option_index = 0;
		opt = getopt_long(argc,argv,"hp:s:l:i:c:",long_option,&option_index);
		if(opt == -1)break;
		switch(opt){
			case 'h':
				printf("SYNOPSIS\n\tproxydns [OPTIONS]\nDESCRIPTION\n");
				printf("\t-h et --help : affiche le message d'aide du programme qui explique toutes les options possibles\n");
				printf("\t-p PORT et --port=PORT : permet de modifier le port par défaut (53) utilisé par le programme pour recevoir les questions DNS\n");
				printf("\t-s SERVEUR et --serveur=SERVEUR : permet de spécifier un serveur DNS particulier à la place du serveur DNS par défaut\n");
				printf("\t-l STRATEGIE et --logstrategy=STRATEGIE : permet de choisir la stratégie à charger (DNSinFILE/1 or PrintDomainName/2)\n");
				printf("\t-i INIT_ARGS_STRATEGIE et --initlogstrategie=INIT_ARGS_STRATEGIE : permet de passer des paramètres d'initialisation à la stratégie choisie \n");
				printf("\t-c CONFIG_FILE et --configfile=CONFIG_FILE : permet de spécifier le fichier de configuration\n");
				printf("EXAMPLE\n\t<path>/proxydns -p 53 -s 193.48.57.48 -l 2 -i fichier.txt\n\t<path>/proxydns --port=53 --serveur=193.48.57.48 -l PrintDNSinFILE -i fichier.txt\n\n");

				return -1;

			case 'p':
				#ifdef DEBUG
				printf("option p with arg %s\n",optarg);
				#endif

				strcpy(port,optarg);
				break;

			case 's':
				#ifdef DEBUG
				printf("option s with arg %s\n",optarg);
				#endif

				strcpy(server,optarg);
				break;

			case 'l':
				#ifdef DEBUG
				printf("option l with arg %s\n",optarg);
				#endif

				if ((strcmp(optarg, "1")==0) || (strcmp(optarg, "DNSinFILE")==0)) strcpy(strategie, "libDNSinFILE.so");
				else if ((strcmp(optarg, "2")==0) || (strcmp(optarg, "PrintDomainName")==0)) strcpy(strategie, "libPrintDomainName.so");
				else strcpy(strategie,"none");

				break;

			case 'i':
				#ifdef DEBUG
				printf("option i with arg %s\n", optarg);
				#endif

				strcpy(init_args_strategie,optarg);
				break;

			case 'c':
				#ifdef DEBUG
				printf("option c with arg %s\n", optarg);
				#endif

				strcpy(configfile,optarg);
				break;

			case '?':
				#ifdef DEBUG
				printf("erreur arg\n");
				#endif

				return -1;
			
		}
	}
	return 0;
}

int args_mgr(int argc, char * argv[],char * domain, char * ipv4, char * ipv6, char * mx, int * flag){
	int opt;
	while(1){
		static struct option long_option[] = {
			{"help",0,0,'h'},
			{"add",0,0,'a'},
			{"remove",0,0,'r'},
			{"domain",1,0,'d'},
			{"inet",1,0,'i'},
			{"inet6",1,0,'I'},
			{"mx",1,0,'m'},
			{0,0,0,0}
		};

		int option_index = 0;
		opt = getopt_long(argc,argv,"hard:i:I:m:",long_option,&option_index);
		if(opt == -1)break;
		switch(opt){
			case 'h':
				printf("SYNOPSIS\n\tproxydns_mgr [OPTIONS]\nDESCRIPTION\n");

				printf("\t-a et --add : ajoute ou modifie si existe une redirection selon le domaine --domain les adresses definies avec --inet --inet6 et --mx\n");
				printf("\t-r et --remove : supprime une redirection selon l'option --domain\n");

				printf("\t-d et --domain : nom de domaine\n");
				printf("\t-i et --inet : adresse ipv4\n");
				printf("\t-I et --inet6 : adresse ipv6\n");
			       	printf("\t-m et --mx : champ mx\n");
				printf("EXEMPLE\n\t<path>/dnsproxy_mgr -r -d www.google.com -> supprime la redirection www.google.com\n\t<path>/dnsproxy_mgr -d www.twitter.com -i 172.26.26.26 -I fe::6 -m 172.23.23.48 -a -> ajoute ou modifie le domaine www.twitter.com avec les adresses définies avec -i -I -m\n");
				return -1;	

			case 'a':
				*flag = 0;
				break;
			case 'r':
				*flag = 1;
				break;
			case 'd':
				strcpy(domain,optarg);
				break;
			case 'i':
				strcpy(ipv4,optarg);
				break;
			case 'I':
				strcpy(ipv6,optarg);
				break;
			case 'm':
				strcpy(mx,optarg);
				break;
			case '?':
				return -1;

		}
	}
	return 0;
}

