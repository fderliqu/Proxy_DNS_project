#define CLE (key_t)1000
#define NB_SHM_DATA 1024

#define MAX_DOMAINE 128
#define MAX_IPV4 4
#define MAX_IPV6 16
#define MAX_MX 128

struct mgr_s{
        char domaine[MAX_DOMAINE];
        char ipv4[MAX_IPV4];
        char ipv6[MAX_IPV6];
        char mx[MAX_MX];
};

int get_shm_id(void *,int,int);

void * get_shm_addr(int);

int free_shm_addr(void *);

int free_shmid(int);
