# Makefile pour le tutorat PSR de Derlique et Dufour

#
# Constantes liees au projet
#

CC=gcc
CFLAGS= -g -Wall -W -Wextra -O2
# LDFLAGS=
TARGET=main

#
# La cible par défaut
#

all: $(TARGET)

#
# Cible de nettoyage 
#

clean:
	rm -f *.o
	rm $(TARGET)

#
# Executable
#

$(TARGET): proxy.o
	$(CC) -o $@ $^ 
#
