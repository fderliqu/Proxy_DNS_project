SRC=$(wildcard *.c)
CC= gcc
TARGET= testGenericLog
CFLAGS= -Wall -Werror
LIBDIR= -L ../../dynamic
INCDIR= -I ../
BIBLIO= -lgenericLog -ldl
export LD_RUN_PATH:

$(TARGET): testGenericLog.o
	$(CC) -o $(TARGET) $< $(LIBDIR) $(BIBLIO)

testGenericLog.o : testGenericLog.c
	$(CC) -c $(CFLAGS) $< $(LIBDIR) $(INCDIR) $(BIBLIO)

test: $(TARGET)
	LD_LIBRARY_PATH=../../dynamic ./$(TARGET)

clean:
	rm -f ./*.o
	rm -f $(TARGET)
