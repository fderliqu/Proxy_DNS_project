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
	file = fopen ((char *)f,"w"); //Ouverture du fichier passé en paramètre en lecture
	if(file==NULL)	// Si impossible à ouvrir le fichier
	{
		printf("Le fichier n'a pas pu être ouvert.\n");
		return(-1);
	}
	return 0;
}

int addLog(logMsg_t * paquet){
	unsigned char* octet = (unsigned char *)paquet->msg; //Stockage du premier element du message dans un pointeur pour parcourir le message
	for(int i=0;i<paquet->size;i++){
		fprintf(file,"%02x ",*octet); //On print le code hexa
		octet++; //On passe à l'octet suivant
	}
	fprintf(file,"\n");
	fflush(file);
	return 0;
}

int endLog(){
	fclose(file);
	return 0;
}
