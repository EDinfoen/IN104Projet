/*
Contient les fonctions pour évaluer un noeud via une simulation aléatoire de la fin de la partie.
*/

#include "annexe.h"
#include "affichage.h"

enum direction {N, NE, E, SE, S, SW, W, NW} ;

void copier_plt(int** source, int** destination);
bool avancer(int xi, int yi, enum direction dir, int* xf, int* yf, int **plateau);
bool piece_mobile(int** plateau, int x, int y);
int coup_aleatoire (int** plateau, piece_t joueur, coup_t* coup);
int simulation(int** plateau, piece_t J, int* deep_max, int* res);
piece_t next_J(piece_t J);