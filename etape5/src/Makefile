#Makefile src

CC=gcc
LIBDIR = -L ../libs/static -L ../libs/dynamic
INCDIR = -I ../libs/Reseau -I ../libs/Log -I ../libs/Threads -I ../libs/Memoire -I ../libs/SharedMem	
BIBLIO = -lreseau -lgenericLog -ldl -lpthread -lthread -lmemoire -lshm
TARGET = ../bin/proxydns ../bin/dnsproxy_mgr

all: $(TARGET)

../bin/proxydns: main.o args.o
	$(CC) -o $@ $^ $(LIBDIR) $(BIBLIO)

../bin/dnsproxy_mgr: dnsproxy_mgr.o args.o 
	$(CC) -o $@ $^ -L ../libs/dynamic -lshm


main.o : main.c
	$(CC) $(CFLAGS) -c $< $(INCDIR) $(LIBDIR) $(BIBLIO)

args.o : args.c
	$(CC) $(CFLAGS) -c $<

dnsproxy_mgr.o: dnsproxy_mgr.c
	$(CC) $(CFLAGS) -c $< -I ../libs/SharedMem -L ../libs/dynamic -lshm


clean:
	rm -f *.o
	rm -f $(TARGET)
	rm -f ../*.txt

