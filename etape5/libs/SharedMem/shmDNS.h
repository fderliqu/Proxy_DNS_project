#define CLE 6666
#define NB_SHM_DATA 1024

#define MAX_DOMAINE 128
#define MAX_IPV4 4
#define MAX_IPV6 16
#define MAX_MX 128
#define MAX_CHAR 500

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

void tidy_mgr(struct mgr_s *, char ligne[MAX_CHAR]);
