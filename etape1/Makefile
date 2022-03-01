MAKE = make
MAKECLEAN = make clean
DIRS = libs/Reseau src

all:$(patsubst %,_DIR_%,$(DIRS))

$(patsubst %,_DIR_%,$(DIRS)):
	cd $(patsubst _DIR_%,%,$@) && $(MAKE)

clean:$(patsubst %,_CLEAN_%,$(DIRS))

$(patsubst %,_CLEAN_%,$(DIRS)):
	cd $(patsubst _CLEAN_%,%,$@) && $(MAKECLEAN)


	
	
