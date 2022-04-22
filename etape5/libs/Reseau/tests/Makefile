SRC=$(wildcard *.c)
CC= gcc
TARGET= testReseau
CFLAGS = -Wall -Werror
LIBDIR = -L ../../static
INCDIR = -I ../
BIBLIO = -lreseau
export LD_RUN_PATH:

$(TARGET): testReseau.o
	$(CC) -o $(TARGET) $< $(LIBDIR) $(BIBLIO)

testReseau.o : testReseau.c
	$(CC) -c $(CFLAGS) $< $(LIBDIR) $(INCDIR) $(BIBLIO)

test: $(TARGET)
	./$(TARGET)

clean:
	rm -f ./*.o
	rm -f $(TARGET)
