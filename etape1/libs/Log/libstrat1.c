#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include"log_dns.h"

int fd;

int initLog(void * f){
	int fd = open ("../../bin/sortie.txt",O_CREAT|O_WRONLY,S_IRUSR|S_IWUSR);
	dup2(fd,1);
	return 0;
}

int addLog(logMsg_t * paquet){
	char* octet = paquet->msg;
	for(int i=0;i<paquet->size;i++){
		printf("%x ",*octet);
		octet++;
	}
	printf("\n");
	return 0;
}
