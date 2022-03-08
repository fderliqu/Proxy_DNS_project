#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include"../log_dns.h"

FILE * file;

int initLog(void * f){
	file = fopen (f,"w");
	if(file==NULL)exit(-1);
	return 0;
}

int addLog(logMsg_t * paquet){
	char* octet = paquet->msg;
	for(int i=0;i<paquet->size;i++){
		fprintf(file,"%02x ",*octet);
		octet++;
	}
	printf("\n");
	return 0;
}