#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netdb.h>

#include"shmDNS.h"

#ifdef DEBUG
int debug = 1;
#endif
#ifndef DEBUG
int debug = 0;
#endif

int test_get_shm_id(int key, int taille, int option){
	int status;
	printf("test get_shm_id :");
	status = get_shm_id(key,taille,option);
	assert(status != -1);
	printf("Succes\n");
	return status;
}

void * test_get_shm_addr(int shmid){
	void * status = NULL;
	printf("test get_shm_addr :");
	status = get_shm_addr(shmid);
	assert(status != NULL);
	printf("succes\n");
	return status;
}

int test_free_shm_addr(void * shmaddr){
	int status;
	printf("test free_shm_addr :");
	status = free_shm_addr(shmaddr);
	assert(status != -1);
	printf("succes\n");
	return status;
}

int test_free_shmid(int shmid){
	int status;
	printf("test free_shmid :");
	status = free_shmid(shmid);
	assert(status != -1);
	printf("succes\n");
	return status;
}

int nomVersAdresse(char *hote,struct sockaddr_storage *padresse)
{
struct addrinfo *resultat,*origine;
statut=getaddrinfo(hote,NULL,NULL,&origine);
if(statut==EAI_NONAME) return -1;
if(statut<0){ perror("nomVersAdresse.getaddrinfo"); exit(EXIT_FAILURE); }
struct addrinfo *p;
for(p=origine,resultat=origine;p!=NULL;p=p->ai_next)
  if(p->ai_family==AF_INET6){ resultat=p; break; }
memcpy(padresse,resultat->ai_addr,resultat->ai_addrlen);
return 0;
}

int main(){
	printf("variables\n");
	int shmid,shmid2;
	struct mgr_s * data;
	struct mgr_s * p_data;
	char * str1="testocc1", *str2="testocc2";
	printf("debut\n");
	shmid = test_get_shm_id(CLE,NB_SHM_DATA*sizeof(struct mgr_s),0);
	printf("shmid : %d\n",shmid);
	data = (struct mgr_s *)test_get_shm_addr(shmid);
	p_data = data;
	printf("addr : %p %p\n",p_data,&data[0]);
	strcpy(p_data->domaine,str1);
	p_data++;
	printf("addr : %p %p\n",p_data,&data[1]);
	strcpy(p_data->domaine,str2);
	printf("data0 : %s\ndata1 : %s\n",data[0].domaine,data[1].domaine);
	shmid2 = test_get_shm_id(CLE,NB_SHM_DATA*sizeof(struct mgr_s),1);
	printf("shmid1 : %d = shmid2 : %d\n",shmid,shmid2);
	test_free_shm_addr((void *)data);
	test_free_shmid(shmid);
	char ip[20];
	nomVersAdresse("172.23.56.21",ip);
	printf("%s\n",ip);
	
}

