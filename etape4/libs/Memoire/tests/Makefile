SRC=$(wildcard *.c)
CC= gcc
TARGET= testMemory
CFLAGS = -Wall -Werror
LIBDIR = -L ../../dynamic
INCDIR = -I ../
BIBLIO = -lmemoire
export LD_RUN_PATH:

testMemory: testMemory.o
	gcc -o testMemory testMemory.o $(LIBDIR) $(BIBLIO)

testMemory.o : testMemory.c
	gcc -c $(CFLAGS) testMemory.c $(LIBDIR) $(INCDIR) $(BIBLIO)

test: testMemory
	LD_LIBRARY_PATH=../../dynamic ./testMemory

clean:
	rm -f ./*.o
	rm -f $(TARGET)
