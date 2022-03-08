#include<stdio.h>
#include"args.h"
#include<stdlib.h>
#include<getopt.h>
#include<string.h>

/*
tester si file bien open sinon exit
faire un dossier dynamic en + de static
faire un sous-répertoire des librairies de stratégie

*/

int args(int argc, char * argv[],char * server, char * port, char * strategie, char * init_args_strategie){
	int opt;
	while(1){
		static struct option long_option[] = {
			{"help",0,0,'h'},
			{"port",1,0,'p'},
			{"serveur",1,0,'s'},
			{"logstrategie",1,0,'l'}
			{"initlogstrategie",1,0,'i'}
			{0,0,0,0}
		};
		int option_index = 0;
		opt = getopt_long(argc,argv,"hp:s:l:i:",long_option,&option_index);
		if(opt == -1)break;
		switch(opt){
			case 'h':
				printf("-h et --help : affiche le message d'aide du programme qui explique toutes les options possibles \n
					-p PORT et --port=PORT : permet de modifier le port par défaut (53) utilisé par le programme pour recevoir les questions DNS\n
					-s SERVEUR et --serveur=SERVEUR : permet de spécifier un serveur DNS particulier à la place du serveur DNS par défaut\n
					-l STRATEGIE et --logstrategy=STRATEGIE : permet de choisir la stratégie à charger (DNSinFILE or PrintDomainName) \n
					-i INIT_ARGS_STRATEGIE et --initlogstrategie=INIT_ARGS_STRATEGIE : permet de passer des paramètres d'initialisation à la stratégie choisie\n ");
				return -1;
			case 'p':
				printf("option p with arg %s\n",optarg);
				strcpy(port,optarg);
				break;
			case 's':
				printf("option s with arg %s\n",optarg);
				strcpy(server,optarg);
				break;
			case 'l':
				printf("option l with arg %s\n",optarg);
				strcpy(strategie,optarg);
				break;
			case 'i':
				printf("option i with arg %s\n", optarg);
				strcpy(init_args_strategie,optarg);
				break;
			case '?':
				//printf("erreur arg\n");
				return -1;
		}
	}
	return 0;
}
