#ifndef __SHMDNS_H__
#define __SHMDNS_H___
#define CLE 6666
#define NB_SHM_DATA 1024

#define MAX_DOMAINE 128
#define MAX_IPV4 50
#define MAX_IPV6 50
#define MAX_MX 50

struct mgr_s{
        char domaine[MAX_DOMAINE];
        char ipv4[MAX_IPV4];
        char ipv6[MAX_IPV6];
        char mx[MAX_MX];
};

int get_shm_id(int,int,int);

void * get_shm_addr(int);

int free_shm_addr(void *);

int free_shmid(int);

int get_semget_id(int ,int,int);

//int init_sem(int,int,union);

int op_sem(int,int);

void tidy_mgr(struct mgr_s *, char *);

void supp_mgr(struct mgr_s * );

void add_mgr(struct mgr_s *,char*,char*,char*,char*);


#endif
