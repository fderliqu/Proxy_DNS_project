//Fonctions principales 
int allocateMemory(size_t size);
int writeMemory(void *data, u_int8_t size);
void *readMemory(u_int8_t *size);

//Fonctions pour gérer le buffer circulaire
bool memoryIsEmpty(void);
bool memoryIsFull(void);
size_t availableMemory(void);
void circ_buff_reset();
void advance_head();
void advance_tail();

#define MAX_TAMPON 256

typedef struct circ_buff_s {
	u_int8_t * buffer;
	size_t head;
	size_t tail;
	size_t max;
	bool full; 
} circ_buff_t;

circ_buff_t memory;
