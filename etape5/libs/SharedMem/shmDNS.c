#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/shm.h>
#include<sys/ipc.h>
#include<sys/types.h>
#include<sys/sem.h>
#include"shmDNS.h"

#define NONE "none"

int get_shm_id(int key,int taille,int option){
	int status=0;
	int flags;
	if(option == 0){
		flags = IPC_CREAT|IPC_EXCL|0666;
	}
	else{
		flags = 0006;
	}
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

int get_semget_id(int key, int taille, int option){
	int id;
	int flags;
	if(option==0){
		flags = 0666|IPC_CREAT;
	}
	else{
		flags = 0666;
	}
	if((id=semget((key_t)key,taille,flags)) < 0){
		perror("semget");exit(1);
	}
	return id;
}
/*
int init_sem(int id, int taille, union semun u){
	int status;
	if((status=semctl(id,0,SETVAL,u)) < 0){
		perror("semctl");exit(2);
	}
	return status;
}
*/

int op_sem(int id,int op){
	if(op!=1 || op!=1)return -1;
	int status=0;
	struct sembuf buf = {0,op,SEM_UNDO};
	if((status=semop(id, &buf, 1)) < 0){
			perror("semop");exit(1);
	}
	return status;
}


void tidy_mgr(struct mgr_s * p_mgr_s, char *ligne)
{
	//Cette méthode ci-dessous ne marche pas pour les cas où il manque des arguments, mais qu'il y en a tout de même d'autres présents derrière
	//memset(p_mgr_s,0,sizeof(struct mgr_s));
	//sscanf(ligne,"%[^=]=%[^,],%[^,],%[^,]",p_mgr_s->domaine,p_mgr_s->ipv4,p_mgr_s->ipv6,p_mgr_s->mx);
 int i = 0, emplacement=0, j=0 ;
 do
 {

   if (emplacement == 0){
     if (ligne[i] == ('=')){
	  p_mgr_s->domaine[j]='\0';
	  emplacement ++;
	  j=0;
	}
     else {
	p_mgr_s->domaine[j]=ligne[i];
	j++;
     }
   }
   
   else if (emplacement == 1){
     if (ligne[i] == (',')){
	  p_mgr_s->ipv4[j]='\0';
	  emplacement ++;
	  j=0;
	}
     else {
	p_mgr_s->ipv4[j]=ligne[i];
	j++;
     }
   }
     
   else if (emplacement == 2){
     if (ligne[i] == (',')){
	  p_mgr_s->ipv6[j]='\0';
	  emplacement ++;
	  j=0;
	}
     else {
	p_mgr_s->ipv6[j]=ligne[i];
	j++;
     }
   }
   else if (emplacement == 3){
     if (ligne[i] == ('\0')){
	  p_mgr_s->mx[j]='\0';
	  emplacement=0;
	  j=0;
	}
     else {
	p_mgr_s->mx[j]=ligne[i];
	j++;
     }
   }
   i++;
 } while (ligne[i] != '\0');

}

void supp_mgr(struct mgr_s * p_mgr_s)
{
	memset(p_mgr_s,0,sizeof(struct mgr_s));
}

void add_mgr(struct mgr_s * p_mgr_s,char * domaine,char * ipv4,char* ipv6,char* mx){
	memset(p_mgr_s,0,sizeof(struct mgr_s));
	strcpy(p_mgr_s->domaine,domaine);
	strcpy(p_mgr_s->ipv4,ipv4);
	strcpy(p_mgr_s->ipv6,ipv6);
	strcpy(p_mgr_s->mx,mx);
}


      
	
      
  




