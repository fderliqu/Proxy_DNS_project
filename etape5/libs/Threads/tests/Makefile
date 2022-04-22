SRC=$(wildcard *.c)
CC= gcc
TARGET= testThread
CFLAGS = -Wall -Werror
LIBDIR = -L ../../dynamic
INCDIR = -I ../
BIBLIO = -lthread -lpthread
export LD_RUN_PATH:

$(TARGET): testThread.o
	$(CC) -o $(TARGET) $< $(LIBDIR) $(BIBLIO)

testThread.o : testThread.c
	$(CC) -c $(CFLAGS) $< $(LIBDIR) $(INCDIR) $(BIBLIO)

test: $(TARGET)
	LD_LIBRARY_PATH=../../dynamic ./$(TARGET)

clean:
	rm -f ./*.o
	rm -f $(TARGET)
