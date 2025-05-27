#include "generic_list.h"
#include <math.h>
#include "evaluation.h"

typedef generic_list_t noeud_list_t; 

typedef enum {V, D, EC} statut_t;
  /*
  V = Victoire assurée;
  D = Défaite assurée;
  EC = partie En Cours (fin de partie avec simulation aléatoire)
  */

typedef struct noeud_ {float n; int N; statut_t statut; int** plateau; piece_t J; int code_coup; noeud_list_t* liste_fils;} noeud_t;
<<<<<<< HEAD
// n : Nombre de victoires de J 
// N : nombre d'explorations 
// statut : état partie pour J
// J : joueur qui vient de jouer pour être dans la sitation actuelle
// plateau : plateau actuel
// code_coup : code pour passer du plateau père au plateau actuel
// liste_fils : liste générique de noeud_t
=======
  /* 
  n : Nombre de victoires de J 
  N : nombre d'explorations du noeud
  statut : état de la partie pour J
  J : joueur qui vient de jouer pour être dans la sitation actuelle
  plateau : plateau actuel
  code_coup : code pour passer du plateau père au plateau actuel
  liste_fils : liste générique de noeud_t correspondant aux coups possibles pour le joueur suivant sur le plateau actuel
  */
>>>>>>> 34d478026e21496a7a495f83015e8b742f6599f0



void print_noeud(noeud_t* nd);
void print_noeud_list(noeud_list_t* liste);

int init_noeud(noeud_t* noeud);
int generation_fils(int** plateau, piece_t J, noeud_list_t* liste);

int exploration(noeud_t *pere);
int saisie_coup_IA(noeud_t* root, coup_t* coup);
int deplacement_arbre(noeud_t* root, coup_t* coup);
