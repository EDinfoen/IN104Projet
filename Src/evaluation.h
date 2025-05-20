/*
Fichier qui contiendra le code pour l'IA.
Pour le moment, il n'y a qu'une fonction coup aléatoire qui donne un coup aléatoire pour un joueur donné.
Elle utilise les fonctions avancer et piece_mobile.
Le main a permis de tester coup_aleatoire pour le plateau initial.
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