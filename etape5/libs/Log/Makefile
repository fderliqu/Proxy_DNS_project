SRC=$(wildcard *.c)
OBJS=$(SRC:.c=.o)
TARGET= libgenericLog.so
MV=mv
CFLAGS += -fPIC
DIRS = strategy

all: $(TARGET)
	for dir in $(DIRS); do \
		($(MAKE) -C $$dir -f Makefile $@;) || exit $$?; \
	done	

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CC) -shared $(OBJS) -o $(TARGET)
	mv ./*.so ../dynamic

clean:
	for dir in $(DIRS); do \
		$(MAKE) -C $$dir -f Makefile $@; \
	done
	rm -f ./*.o
	rm -f ../dynamic/*.so
