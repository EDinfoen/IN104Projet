/*
Fichier qui contiendra le code pour l'IA.
Pour le moment, il n'y a qu'une fonction coup aléatoire qui donne un coup aléatoire pour un joueur donné.
Elle utilise les fonctions avancer et piece_mobile.
Le main a permis de tester coup_aleatoire pour le plateau initial.
*/

#include "annexe.h"
#include "affichage.h"

bool avancer(int xi, int yi, direction dir, int* xf, int* yf, int **plateau);
bool piece_mobile(int** plateau, int x, int y);
coup_t* coup_aleatoire (int** plateau, piece_t joueur);
int simulation(int** plateau, piece_t J, int* deep_max, int* res);