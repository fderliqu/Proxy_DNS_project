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


void test_tidy_mgr(struct mgr_s * p_mgr_s, char *ligne)
{
	printf("\nTest de tidy_mgr avec ligne : %s\n",ligne);
	tidy_mgr(p_mgr_s, ligne);
	printf("Nom de domaine : %s\n", p_mgr_s->domaine);
	printf("IPV4 : %s\n", p_mgr_s->ipv4);
	printf("IPV6 : %s\n", p_mgr_s->ipv6);
	printf("MX : %s\n", p_mgr_s->mx);
	printf("Fin du test \n");
}

void test_supp_mgr(struct mgr_s * p_mgr_s)
{
	printf("\nTest de supp_mgr : \n");
	supp_mgr(p_mgr_s);
	printf("Nom de domaine : %s\n", p_mgr_s->domaine);
	printf("IPV4 : %s\n", p_mgr_s->ipv4);
	printf("IPV6 : %s\n", p_mgr_s->ipv6);
	printf("MX : %s\n", p_mgr_s->mx);
	printf("Fin du test \n");
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
	
	struct mgr_s m, n, o;
	test_tidy_mgr(&m, "t.vantroys.plil.fr=172.26.145.55,2001:660:4401:6050:5000::5,172.26.145.55");
	test_tidy_mgr(&n, "www.google.com=172.26.145.56,,");
	test_tidy_mgr(&o, "www.twitter.com=172.26.145.56,,172.26.145.55");
	test_supp_mgr(&n);
	
}

