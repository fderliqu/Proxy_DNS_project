#include<stdio.h>
#include<stdlib.h>
#include<sys/shm.h>
#include<sys/ipc.h>
#include<sys/types.h>
#include"dnsproxy_mgr.h"

#define CLE (key_t)1000
#define NB_SHM_DATA 1024 

#define MAX_DOMAINE 128
#define MAX_IPV4 4
#define MAX_IPV6 16
#define MAX_MX 128

struct mgr_s{
	unsigned char domaine[MAX_DOMAINE];
	unsigned char ipv4[MAX_IPV4];
	unsigned char ipv6[MAX_IPV6];
	unsigned char mx[MAX_MX];
};

int get_shm_id(key_t key,int taille,int option){
	int status=0;
	if(option == 0){
		if((status=shmget(key,taille*sizeof(struct mgr_s), IPC_CREAT|IPC_EXCL|0666)) == -1){
			perror("shmget");exit(2);
		}
	}
	if(option == 1){
		if((status=shmget(CLE,taille*sizeof(struct mgr_s), IPC_EXCL|0666)) == -1){
		       perror("shmget");exit(2);
		}
	}
	return status;
}

void * get_shm_addr(int id){
	void * addr;
	if((addr = shmat(id,0,0)) == (void *)(-1)){
		perror("shmat");exit(2);
	}
	return addr;
}

int main(int argc, char* argv[]){
	if(argc==1)return -1;
	FILE * file = fopen(argv[2],"r");
	int id = get_shm_id(CLE,NB_SHM_DATA,1);
	struct mgr_s * addr = (struct mgr_s *)get_shm_addr(id); 
	int cmp_occ=0;
	char c = fgetc(file);
	if(file != NULL){
		do{
		while(c != ':'){
			addr->domaine[cmp_occ] = c;
			cmp_occ++;
			c=fgetc(file);
		}
		cmp_occ=0;
		while(c != ','){
			addr->ipv4[cmp_occ] = c;
			cmp_occ++;
			c=fgetc(file);
		}
		cmp_occ=0;
		while(c != ','){
			addr->ipv6[cmp_occ] = c;
			cmp_occ++;
			c=fgetc(file);
		}
		cmp_occ=0;
		while(c != '\n'){
			addr->mx[cmp_occ] = c;
			cmp_occ++;
			c=fgetc(file);
		}
		cmp_occ=0;
		addr++;
		}while((c=fgetc(file)) != EOF);
		fclose(file);
	}

	return 0;
}




