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

