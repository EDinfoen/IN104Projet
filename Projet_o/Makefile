CC=gcc
CFLAGS=-Wall -Werror 
INC = -Iinclude
PATH = include/
all: projet.x

projet.o: projet.c $(PATH)jouer.h
	$(CC) $(CFLAGS) $(INC) -c projet.c -o projet.o

affichage.o: affichage.c $(PATH)constante.h 
	$(CC) $(CFLAGS) $(INC) -c affichage.c -o affichage.o

annexe.o: annexe.c $(PATH)constante.h 
	$(CC) $(CFLAGS) $(INC) -c annexe.c -o annexe.o

jouer.o: jouer.c $(PATH)affichage.h $(PATH)annexe.h
	$(CC) $(CFLAGS) $(INC) -c jouer.c -o jouer.o

projet.x: projet.o jouer.o affichage.o annexe.o
	$(CC) projet.o jouer.o annexe.o affichage.o -o projet.x


clean:
	rm -f *~ *.o

realclean: clean
	rm -f *.x
