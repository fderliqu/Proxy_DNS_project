CC=gcc
LIBDIR = -L ../libs/static -L ../libs/dynamic
INCDIR = -I ../libs/Reseau -I ../libs/Log
BIBLIO = -lreseau -lgenericLog -ldl
TARGET = ../bin/main

all: $(TARGET)
	
main.o : main.c
	$(CC) -c $< $(INCDIR) $(LIBDIR) $(BIBLIO)

args.o : args.c
	$(CC) -c $<

$(TARGET): main.o args.o
	$(CC) -o $@ $^ $(LIBDIR) $(BIBLIO)

clean:
	rm -f *.o
	rm -f $(TARGET)

