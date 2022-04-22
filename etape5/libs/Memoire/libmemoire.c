#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#include "memoire.h"

#define MAX_TAMPON 256

#ifdef DEBUG
int dbg = 1;
#endif
#ifndef DEBUG
int dbg = 0;
#endif

typedef struct circ_buff_s {
	uint8_t * buffer;
	size_t head;
	size_t tail;
	size_t max;
	bool full;
} circ_buff_t;

circ_buff_t memory;

int allocateMemory(size_t size)
{
	if(dbg)printf("Entre fonction allocateMem\n");
	memory.buffer = malloc(size);
	memory.max = size;
	circ_buff_reset();
	if (memoryIsEmpty() == false) return -1;

	return 0;
}

int desallocateMemory(){
	free(memory.buffer);
	return 0;
}

int writeMemory(void *data, uint8_t size)
{
	if(dbg)printf("\nDébut fonction writeMem head : %ld / tail : %ld / size msg : %d / size memory max : %ld \n",memory.head,memory.tail,size, memory.max);

	if ((int)availableMemory() < size) return -1; //il n'y a pas assez de place dans la mémoire

	uint8_t *p = data;

	//rajoute la taille de la donnée avant la donnée 
	memory.buffer[memory.head] = size;
	advance_head();

	//on ajoute la donnée octet par octet 
	for (int i=0; i<size; i++)
	{
		memory.buffer[memory.head] = *p;
		p++;
		advance_head();
	}
	if(dbg)printf("Fin fonction writeMem head : %ld / tail : %ld / mémoire restante : %ld / full ? %i \n",memory.head,memory.tail, availableMemory(), memory.full);
	return 0;
}

void *readMemory(uint8_t *size)
{
	if(dbg)printf("\nDébut fonction readMem head : %ld / tail : %ld\n",memory.head,memory.tail);
	
	if (memoryIsEmpty())
	{
		*size = 0;
		return	NULL;
	}

	static uint8_t tampon[MAX_TAMPON];  //static pour ne pas avoir à free

	uint8_t taille = (int)memory.buffer[memory.tail];

	if(dbg)printf("taille pour tampon : %02x\n",memory.buffer[memory.tail]);

	*size = taille;
	advance_tail(); //on passe la valeur de la taille

	if(dbg)printf("copie de tanpon : ");

	for (int i=0; i<taille; i++)
	{
		tampon[i] = memory.buffer[memory.tail]; // "+ 1" car on ignore l'octet signifiant la taille
		if(dbg)printf("%02x ",memory.buffer[memory.tail]);
		advance_tail();
	}
	if(dbg)printf("\nFin fonction readMem head : %ld / tail : %ld / mémoire restante : %ld \n",memory.head,memory.tail, availableMemory());

	return &(tampon);
}

bool memoryIsEmpty(void) 
{
	return (!memory.full && (memory.head == memory.tail));
}

bool memoryIsFull()
{
	return (memory.full);
}

//reset le buffer
void circ_buff_reset()
{
	memory.head=0;
	memory.tail=0;
	memory.full=false;
}	

//donne la taille de mémoire restante
size_t availableMemory(void) 
{
	size_t size;

	if (memory.full) size = 0;
	if (memory.head < memory.tail)
	{
		size = memory.tail - memory.head;
	}
	else 
	{
		size = memory.max + memory.tail - memory.head;
	}
	return size;
}

void advance_head()
{
	if (++(memory.head) == memory.max) memory.head = 0;
	memory.full = (memory.head == memory.tail);
}

void advance_tail()
{
	if (memory.full) memory.full = false;
	if (++(memory.tail) == memory.max) memory.tail = 0;
}
