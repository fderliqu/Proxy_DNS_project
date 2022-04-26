#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/shm.h>
#include<sys/ipc.h>
#include<sys/types.h>
#include<sys/sem.h>
#include"shmDNS.h"



int get_shm_id(int key,int taille,int option){
	int status=0;
	int flags;
	if(option == 0){
		flags = IPC_CREAT|IPC_EXCL|0666;
	}
	else{
		flags = 0006;
	}
	printf("%d,%d\n",(key_t)key,taille);
	if((status=shmget((key_t)key, taille, flags)) == -1){
			perror("shmget");exit(2);
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

int free_shm_addr(void * shmaddr){
	int status=0;
	if((status=shmdt(shmaddr)) == -1){
		perror("shmdt");exit(1);
	}
	return status;
}

int free_shmid(int shmid){
	int status=0;
	if((status=shmctl(shmid,IPC_RMID,0))==-1){
	perror("shmctl");exit(1);
	}
	return status;
}

void tidy_mgr(struct mgr_s * p_mgr_s, char *ligne)
{
	memset(p_mgr_s,0,sizeof(struct mgr_s));
	sscanf(ligne,"%[^=]=%[^,],%[^,],%s",p_mgr_s->domaine,p_mgr_s->ipv4,p_mgr_s->ipv6,p_mgr_s->mx);
}

void supp_mgr(struct mgr_s * p_mgr_s)
{
	memset(p_mgr_s,0,sizeof(struct mgr_s));
}


      
	
      
  




