SRC=$(wildcard *.c)
OBJS=$(SRC:.c=.o)
TARGET= libgenericLog.so
MV=mv
CFLAGS += -fPIC

all: $(TARGET)

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CC) -shared $(OBJS) -o $(TARGET)
	mv ./*.so ../dynamic

clean:
	rm ./*.o
	rm ../dynamic/*.so
