/* 
Contient les fonctions annexes qui servent à jouer une partie dans le fichier jouer.c .
*/

#include "constante.h"
typedef struct coup_ {int xi; int yi; int xf; int yf;} coup_t;

int init_test(int** plateau);
int init(int** plateau);
void destroy(int ** plateau);

bool licite(int** plateau, coup_t* coup, piece_t J); 
int localisation_bobail(int** plateau, int* x, int* y);
int saisie_coup(int** plateau, coup_t* coup, piece_t J);
int mouvement(int** plateau, coup_t* coup);

bool fin(int** plateau, piece_t J_act, piece_t* gagnant); 