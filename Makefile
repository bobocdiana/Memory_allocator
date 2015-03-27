# Declaratiile de variabile
CC = gcc
SRC = tema4.c
EXE = allocator
 
# Regula de compilare
all:
	$(CC) -Wall -o $(EXE) $(SRC)
 
# Regulile de "curatenie" (se folosesc pentru stergerea fisierelor intermediare si/sau rezultate)
.PHONY : clean
clean :
	rm -f $(EXE) *~
