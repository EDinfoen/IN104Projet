/*
Fichier qui contiendra le code pour évaluaer les parties non finale dans l'arbre dans l'arbre.
Pour le moment, il n'y a qu'une fonction coup aléatoire qui donne un coup aléatoire pour un joueur donné.
Elle utilise les fonctions avancer et piece_mobile.
*/

#include "annexe.h"
#include "affichage.h"

void copier_plt(int** source, int** destination);
piece_t next_J(piece_t J);

bool avancer(int xi, int yi, direction dir, int* xf, int* yf, int **plateau);
bool piece_mobile(int** plateau, int x, int y);

int coup_aleatoire (int** plateau, piece_t joueur, coup_t* coup);
int simulation(int** plateau, piece_t J, int* deep_max, int* res);
