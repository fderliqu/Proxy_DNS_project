SRC=$(wildcard *.c)
OBJS=$(SRC:.c=.o)
TARGET= libDNSinFILE.so
MV=mv
CFLAGS += -fPIC

all: $(TARGET)

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CC) -shared libDNSinFILE.o -o libDNSinFILE.so
	$(CC) -shared libPrintDomainName.o -o libPrintDomainName.so
	mv ./*.so ../../dynamic

clean:
	rm -f ./*.o
