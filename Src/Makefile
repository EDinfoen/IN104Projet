CC=gcc
CFLAGS=-Wall -Werror -g
all: projet.x

projet.o: projet.c jouer.h
	$(CC) $(CFLAGS) -c projet.c -o projet.o -lm

affichage.o: affichage.c constante.h 
	$(CC) $(CFLAGS) -c affichage.c -o affichage.o

annexe.o: annexe.c constante.h 
	$(CC) $(CFLAGS) -c annexe.c -o annexe.o

jouer.o: jouer.c affichage.h exploration.h
	$(CC) $(CFLAGS)  -c jouer.c -o jouer.o

generic_list.o: generic_list.c constante.h
	$(CC) $(CFLAGS)  -c generic_list.c -o generic_list.o

exploration.o: exploration.c generic_list.h evaluation.h 
	$(CC) $(CFLAGS) -c exploration.c -o $@ -lm

evaluation.o : evaluation.c affichage.h annexe.h
	$(CC) $(CFLAGS) -c evaluation.c -o $@

projet.x: projet.o exploration.o evaluation.o generic_list.o jouer.o annexe.o affichage.o
	$(CC) $^  -o projet.x -lm

evaluation.x: evaluation.c affichage.o annexe.o constante.h
	$(CC) $(CFLAGS) $^ -o $@

exploration.x: exploration.c affichage.o annexe.o generic_list.o evaluation.o constante.h -lm
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f *~ *.o

realclean: clean
	rm -f *.x
