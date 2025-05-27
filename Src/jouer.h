#include "affichage.h"
#include "exploration.h"
/*
Contient les fonctions pour jouer une partie tour après tour. 
*/

int tour(int** plateau, coup_t* coup, piece_t J );
int jouer_pvp();

int tour_IA(int** plateau, coup_t* coup, noeud_t* root );
int jouer_IA();