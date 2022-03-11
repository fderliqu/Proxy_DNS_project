MAKE = make
MAKECLEAN = make clean
DIRS = libs/Reseau libs/Log libs/Log/strategy src
export CC=gcc
export CFLAGS=-Wall -Wextra -O2
export LD_RUN_PATH=$$LD_RUN_PATH:$(shell pwd)/libs/dynamic


all:$(patsubst %,_DIR_%,$(DIRS))

$(patsubst %,_DIR_%,$(DIRS)):
	cd $(patsubst _DIR_%,%,$@) && $(MAKE)
debug: all
debug: CFLAGS += -g -DDEBUG

clean:$(patsubst %,_CLEAN_%,$(DIRS))

$(patsubst %,_CLEAN_%,$(DIRS)):
	cd $(patsubst _CLEAN_%,%,$@) && $(MAKECLEAN)
clean:
	rm -f *.txt

