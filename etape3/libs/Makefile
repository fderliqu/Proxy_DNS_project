DIRS = Log Reseau Threads

all:
	for dir in $(DIRS); do \
		($(MAKE) -C $$dir -f Makefile $@;) || exit $$?; \
	done

debug: all
debug: CFLAGS+=-g -DDEBUG	


clean:
	for dir in $(DIRS); do \
		$(MAKE) -C $$dir -f Makefile $@; \
	done

