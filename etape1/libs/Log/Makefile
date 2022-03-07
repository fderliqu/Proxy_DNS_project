SRC=$(wildcard *.c)
OBJS=$(SRC:.c=.o)
TARGET= libstrat1.so libstrat2.so libgenericLog.so
CFLAGS += -fPIC 

all: $(TARGET)

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@

%.so: %.o
	$(CC) -shared $< -o $@

clean:
	rm ./*.so
	rm ./*.o
