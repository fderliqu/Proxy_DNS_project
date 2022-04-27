#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"args.h"
#include"shmDNS.h"

#define NONE "none"

#ifdef DEBUG
int dgb = 1;
#endif
#ifndef DEBUG
int dgb = 0;
#endif

int main(int argc, char * argv[]){
	char domain[MAX_DOMAINE] = NONE, ipv4[MAX_IPV4] = NONE, ipv6[MAX_IPV6]=NONE, mx[MAX_MX]=NONE;
	int flag;
	if(args_mgr(argc,argv,domain,ipv4,ipv6,mx,&flag) == -1)return -1;
	int shmid = get_shm_id(CLE,NB_SHM_DATA*sizeof(struct mgr_s),1);
	struct mgr_s * shared_mem;
	shared_mem = (struct mgr_s *)get_shm_addr(shmid);
	if(dgb){
		for(int i=0;i<NB_SHM_DATA;i++){
			if(shared_mem[i].domaine[0] != 0)printf("emplacement %d : domaine->%s ipv4->%s ipv6->%s mx->%s\n",i,shared_mem[i].domaine,shared_mem[i].ipv4,shared_mem[i].ipv6,shared_mem[i].mx);
		}
	}
	if(dgb){
		printf("args : \n\tdomaine = %s\n\tipv4 = %s\n\tipv6 = %s\n\tmx = %s\n\t flag = %d\n",domain,ipv4,ipv6,mx,flag);
	}
	struct mgr_s * p_shared_mem = shared_mem;
	if(flag == 0){ //ADD
		printf("ADD OR MODIFY\n");
		int i=0;
		while(p_shared_mem->domaine[0] != 0){
			if(strcmp(p_shared_mem->domaine,domain)==0)break;
			printf("%d ",p_shared_mem->domaine[0]);
			p_shared_mem++;
			i++;
		}
		add_mgr(p_shared_mem,domain,ipv4,ipv6,mx);
	}
	else if(flag == 1){
		printf("REMOVE\n");
		int i=0;
		int status;
		while((status=strcmp(p_shared_mem->domaine,domain))!=0){
			if(dgb)printf("%d\n",status);
			p_shared_mem++;
			i++;
			if(i==NB_SHM_DATA)exit(-1);
		}
		supp_mgr(p_shared_mem);
	}
	if(dgb){
		for(int i=0;i<NB_SHM_DATA;i++){
			if(shared_mem[i].domaine[0] != 0)printf("emplacement %d : domaine->%s ipv4->%s ipv6->%s mx->%s\n",i,shared_mem[i].domaine,shared_mem[i].ipv4,shared_mem[i].ipv6,shared_mem[i].mx);
		}
	}
	free_shm_addr(shared_mem);
	return 0;
}

	

