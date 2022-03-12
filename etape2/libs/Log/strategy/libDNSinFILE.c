//Strategie mettre les messages dns recu dans un ficher texte

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include"../log_dns.h"

FILE * file;

int initLog(void * f){
	file = fopen ((char *)f,"w");
	if(file==NULL)
	{
		printf("Le fichier n'a pas pu être ouvert.\n");
		exit(-1);
	}
	return 0;
}

int addLog(logMsg_t * paquet){
	unsigned char* octet = (unsigned char *)paquet->msg;
	for(int i=0;i<paquet->size;i++){
		fprintf(file,"%02x ",*octet);
		octet++;
	}
	fprintf(file,"\n");
	fflush(file);
	return 0;
}
