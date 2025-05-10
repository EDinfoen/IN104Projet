CC=gcc
CFLAGS=-Wall -Werror 
all: projet.x

projet.o: projet.c jouer.h
	$(CC) $(CFLAGS) -c projet.c -o projet.o

affichage.o: affichage.c constante.h 
	$(CC) $(CFLAGS) -c affichage.c -o affichage.o

annexe.o: annexe.c constante.h 
	$(CC) $(CFLAGS) -c annexe.c -o annexe.o

jouer.o: jouer.c affichage.h annexe.h
	$(CC) $(CFLAGS)  -c jouer.c -o jouer.o

generic_list.o: generic_list.c constante.h
	$(CC) $(CFLAGS)  -c generic_list.c -o generic_list.o

projet.x: projet.o jouer.o affichage.o annexe.o
	$(CC) projet.o jouer.o annexe.o affichage.o -o projet.x

IA.x: IA.c affichage.o annexe.o constante.h
	$(CC) $(CFLAGS) $^ -o $@

exploration.x: exploration.c affichage.o annexe.o generic_list.o constante.h
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f *~ *.o

realclean: clean
	rm -f *.x
