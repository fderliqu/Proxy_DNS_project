# Makefile pour le tutorat PSR de Derlique et Dufour

#
# Constantes liees au projet
#

AR=ar crs
# LDFLAGS=
TARGET=../static/libreseau.a

#
# La cible par défaut
#

all: $(TARGET)

#
# Cible de nettoyage 
#

clean:
	rm -f *.o
	rm -f $(TARGET)

#
# Executable
#
reseau.o: reseau.c
	$(CC) -c $^

$(TARGET): reseau.o
	$(AR) $@ $^ 
