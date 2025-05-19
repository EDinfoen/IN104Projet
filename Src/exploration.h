#include "annexe.h"
#include "affichage.h"
#include "generic_list.h"
#include <math.h>

typedef generic_list_t noeud_list_t; 

typedef struct noeud_ {float n; int N; statut_t statut; int** plateau; piece_t J; int code_coup; noeud_list_t* liste_fils;} noeud_t;
// n : Nombre de victoires de J 
// N : nombre d'explorations 
// statut : état partie pour J
// J : joeur qui vient de jouer pour être dans la sitation actuelle
// plateau : plateau actuel
// code_coup : code pour passer du plateau père au plateau actuel
// liste_fils : liste générique de noeud_t

typedef enum {V, D, EC} statut_t;
    /*V = Victoire assurée;
      D = Défaite assurée;
      EC = partie En Cours (fin de partie avec simulation aléatoire)
    */

void print_noeud(noeud_t* nd);
void print_noeud_list(noeud_list_t* liste);
int saisie_coup_IA(noeud_t* root, coup_t* coup);
int init_noeud(noeud_t* noeud);
void copier_plt(int** source, int** destination);
int generation_fils(int** plateau, piece_t J, noeud_list_t* liste);
piece_t next_J(piece_t J);
int exploration(noeud_t *pere);
