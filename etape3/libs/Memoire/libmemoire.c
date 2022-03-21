#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "memoire.h"

//static u_int8_t tampon; //pour fonction read memory

#define MAX_TAMPON 256

#ifdef DEBUG
int dbg = 1;
#endif
#ifndef DEBUG
int dbg = 0;
#endif

typedef struct circ_buff_s {
	u_int8_t * buffer;
	size_t head;
	size_t tail;
	size_t max;
	bool full;
} circ_buff_t;

circ_buff_t memory;

int allocateMemory(size_t size)
{
	if(dbg)printf("Entre fonction allocateMem");
	memory.buffer = malloc(size);
	memory.max = size;
	circ_buff_reset();
	if (memoryIsEmpty() == false) return -1;

	return 0;
}

int writeMemory(void *data, u_int8_t size)
{
	if(dbg)printf("Entre fonction writeMem");
	if ((int)availableMemory() < size) return -1; //il n'y a pas assez de place dans la mémoire

	u_int8_t *p = data;

	//rajoute la taille de la donnée avant la donnée 
	*p = size;
	p++;
	advance_head();

	//on ajoute la donnée octet par octet 
	for (int i=0; i<size; i++)
	{
		memory.buffer[memory.head] = *p;
		p++;
		advance_head();
	}
	return 0;
}

void *readMemory(u_int8_t *size)
{
	if(dbg)printf("Entre fonction readMem");
	static u_int8_t tampon[MAX_TAMPON];  //static pour ne pas avoir à free

	u_int8_t taille = (int)memory.buffer[memory.tail];
	*size = taille;
	advance_tail(); //on passe la valeur de la taille

	for (int i=0; i<taille; i++)
	{
		tampon[i] = memory.buffer[i]; // "+ 1" car on ignore l'octet signifiant la taille
		advance_tail();
	}

	return &(tampon);
}

bool memoryIsEmpty(void) 
{
	return (!memory.full && (memory.head == memory.tail));
}

bool memoryIsFull(void)
{
	return memory.full;
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
	if (memory.head > memory.tail)
	{
		size = memory.head - memory.tail;
	}
	else 
	{
		size = memory.max + memory.head - memory.tail;
	}
	return size;
}

void advance_head()
{
	//if (memoryIsFull()){
	memory.head ++;
	if (memory.head == memory.max) memory.head = 0;
	memory.full = (memory.head == memory.tail);
}

void advance_tail()
{
	//if (memoryIsFull())
		memory.tail ++;
		if (memory.tail == memory.max) memory.tail = 0;
	//memory.full = (memory.tail == memory.tail);
}
