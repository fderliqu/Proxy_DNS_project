#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "memoire.h"

typedef struct circ_buff_s
{
	void * buffer;
	size_t head;
	size_t tail;
	size_t max;
	bool full;
} circ_buff_t;

circ_buff_t memory;

int allocateMemory(size_t size)
{
	memory.buffer = malloc(size);
	memory.max = size;
	circ_buff_reset();
	if (memoryIsEmpty() == false) return -1;

	return 0;
}

int writeMemory(void *data, u_int8_t size)
{
	if ((int)availableMemory() < size) return -1;
}

void *readMemory(u_int8_t *size);

bool memoryIsEmpty(void) 
{
	return (!memory.full && (memory.head == memory.tail));
}

bool memoryIsFull(void)
{
	return memory.full;
}

void circ_buff_reset()
{
	memory.head=0;
	memory.tail=0;
	memory.full=false;
}	

size_t availableMemory(void)
{
	size_t size;

	if (memory.full) size = 0;
	if (memory.head >= memory.tail)
	{
		size = memory.head - memory.tail;
	}
	else 
	{
		size = memory.max + memory.head - memory.tail;
	}
	return size;
}