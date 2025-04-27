#include "constante.h"
typedef struct coup_ {int xi; int yi; int xf; int yf;} coup_t;

int init(int** plateau);
void destroy(int ** plateau);

bool licite(int** plateau, coup_t* coup, int J); 
int localisation_bobail(int** plateau, int* x, int* y);
int saisie_coup(int** plateau, coup_t* coup, int J);
int mouvement(int** plateau, coup_t* coup);

int fin(int** plateau, bool* fini,int J_act, int* gagnant); 