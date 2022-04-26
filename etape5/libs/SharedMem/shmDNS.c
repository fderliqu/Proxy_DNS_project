#include<stdio.h>
#include<stdlib.h>
#include<sys/shm.h>
#include<sys/ipc.h>
#include<sys/types.h>
#include<sys/sem.h>
#include"shmDNS.h"
#define MAX_CHAR 500


int get_shm_id(void * key,int taille,int option){
	int status=0;
	int flags;
	if(option == 0){
		flags = IPC_CREAT|IPC_EXCL|0666;
	}
	else{
		flags = IPC_EXCL|0666;
	}

	if((status=shmget(*(key_t*)key, taille, flags)) == -1){
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

void tidy_mgr(struct mgr_s * p_mgr_s, char ligne[MAX_CHAR])
{
  int i = 0, emplacement=0, j=0 ;
  do
  {
 
    if (emplacement == 0){
      if (ligne[i] == (',')){
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

      
	
      
  




