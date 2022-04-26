#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>

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

int main(){
	printf("variables\n");
	int shmid;
	struct mgr_s * data;
	char * str1="testocc1", *str2="testocc2";
	printf("debut\n");
	shmid = test_get_shm_id(CLE,NB_SHM_DATA*sizeof(struct mgr_s),0);
	printf("shmid : %d\n",shmid);
	data = (struct mgr_s *)test_get_shm_addr(shmid);
	strcpy(data->domaine,str1);
	data++;
	strcpy(data->domaine,str2);
	printf("data0 : %s\ndata1 : %s",data[0].domaine,data[1].domaine);
	test_free_shm_addr((void *)data);
	test_free_shmid(shmid);
}

