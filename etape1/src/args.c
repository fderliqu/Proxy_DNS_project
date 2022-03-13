#include<stdio.h>
#include"args.h"
#include<stdlib.h>
#include<getopt.h>
#include<string.h>

int args(int argc, char * argv[],char * server, char * port){
	int opt;
	while(1){
		static struct option long_option[] = {
			{"help",0,0,'h'},
			{"port",1,0,'p'},
			{"serveur",1,0,'s'},
			{0,0,0,0}
		};
		int option_index = 0;
		opt = getopt_long(argc,argv,"hp:s:",long_option,&option_index);
		if(opt == -1)break;
		switch(opt){
			case 'h':
				printf("-h et --help : affiche le message d'aide du programme qui explique toutes les options possibles \n");
				printf("-p PORT et --port=PORT : permet de modifier le port par défaut (53) utilisé par le programme pour recevoir les questions DNS\n");
				printf("-s SERVEUR et --serveur=SERVEUR : permet de spécifier un serveur DNS particulier à la place du serveur DNS par défaut\n");
				return -1;
			case 'p':
				printf("option p with arg %s\n",optarg);
				strcpy(port,optarg);
				break;
			case 's':
				printf("option s with arg %s\n",optarg);
				strcpy(server,optarg);
				break;
			case '?':
				return -1;
		}
	}
	return 0;
}
