SRC=$(wildcard *.c)
CC= gcc
TARGET= testshmDNS
CFLAGS = -Wall -Werror
LIBDIR = -L ../../dynamic
INCDIR = -I ../
BIBLIO = -lshm
export LD_RUN_PATH:

$(TARGET): testshmDNS.o
	$(CC) -o $(TARGET) $< $(LIBDIR) $(BIBLIO)

testshmDNS.o : testshmDNS.c
	$(CC) -c $(CFLAGS) $< $(LIBDIR) $(INCDIR) $(BIBLIO)

test: $(TARGET)
	LD_LIBRARY_PATH=../../dynamic ./$(TARGET)

clean:
	rm -f ./*.o
	rm -f $(TARGET)
