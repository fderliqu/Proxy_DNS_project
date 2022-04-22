#ifndef __MEMORY_H__
#define __MEMORY_H__

//Fonctions principales 
int allocateMemory(size_t size);
int desallocateMemory();
int writeMemory(void *data, uint8_t size);
void *readMemory(uint8_t *size);

//Fonctions pour gérer le buffer circulaire
bool memoryIsEmpty(void);
bool memoryIsFull(void);
size_t availableMemory(void);
void circ_buff_reset();
void advance_head();
void advance_tail();

#endif

