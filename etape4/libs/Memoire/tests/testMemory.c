#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "memoire.h"

#ifdef DEBUG
int debug = 1;
#endif
#ifndef DEBUG
int debug = 0;
#endif

void test_allocateMemory(size_t size)
{
	printf("\nTest allocateMemory, size=%d : ",(int)size);
	assert(allocateMemory(size)==0);
	printf("Success\n");
}

void test_writeMemory(void* data, uint8_t size)
{
	printf("\nTest writeMemory\nsize=%d et data=",size);
	uint8_t * p = data;
	for (int i=0; i<size; i++)
	{
		printf("%d ", p[i] );
	}
	printf(" : ");

	int test = writeMemory(data,size);
	
	if (test==-1) printf("Taille insuffisante\nPlace restante dans la mémoire : %zu\n", availableMemory());
	else 
		{
			assert(test == 0);
			printf("Succès\n");
		}
}

void test_readMemory()
{
	printf("\nTest readMemory : ");
	uint8_t taille;
	uint8_t * p = readMemory(&taille);
	if (p == NULL) 
		{
			assert(taille == 0);
			printf("Mémoire nulle, rien à lire\n");
		}
	else 
		{
			printf("Success \nsize=%d et data = ", taille);
			for (int i=0; i<taille; i++)
			{
				printf("%d ",p[i]);
			}
			printf("  end\n");
		}

}

int main(void)
{
	printf("\nTEST DE LIBMEMORY\n");
	test_allocateMemory(256);
	test_readMemory();
	uint8_t p[] = {1,22,3};
	test_writeMemory(p,sizeof(p));
	uint8_t q[255];
	test_writeMemory(q,sizeof(q));
	test_readMemory();

	test_writeMemory(q,sizeof(q));

	if (memoryIsFull()) printf("\nmemory is full\n");
	
	test_readMemory();

	desallocateMemory();
	return 0;
	
}
