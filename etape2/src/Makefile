#Makefile src

CC=gcc
LIBDIR = -L ../libs/static -L ../libs/dynamic
INCDIR = -I ../libs/Reseau -I ../libs/Log
BIBLIO = -lreseau -lgenericLog -ldl
TARGET = ../bin/proxydns

all: $(TARGET)
	
main.o : main.c
	$(CC) $(CFLAGS) -c $< $(INCDIR) $(LIBDIR) $(BIBLIO)

args.o : args.c
	$(CC) $(CFLAGS) -c $<

$(TARGET): main.o args.o
	$(CC) -o $@ $^ $(LIBDIR) $(BIBLIO)

clean:
	rm -f *.o
	rm -f $(TARGET)
	rm -f ../*.txt

