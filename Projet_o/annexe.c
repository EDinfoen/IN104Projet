#include "constante.h"

typedef struct coup_ {int xi; int yi; int xf; int yf;} coup_t;

int init(int** plateau){
    /*
    plateau = malloc(SIZE*sizeof(int*));
    if(plateau == NULL){
        printf("Erreur init: allocation (plateau)\n");
        return EXIT_FAIL;
    }
    for(int i = 0; i < SIZE; i++){
        plateau[i] = malloc(SIZE*sizeof(int));
        if(plateau[i] == NULL){
            printf("Erreur init: allocation (plateau[%d])\n", i);
            for(int j = 0; j < i; j++){
                free(plateau[i]);
            }
            free(plateau);
            return EXIT_FAIL;
        }
    }*/

    for (int i = 0; i < SIZE; i++){
        plateau[i][0] = J1;
        plateau[i][SIZE-1] = J2;
    }
    for (int i = 0; i < SIZE; i++){
        for(int j = 1; j < SIZE - 1; j++){
            plateau[i][j] = VIDE;
        }
    }
    plateau[SIZE/2][SIZE/2] = BOBAIL;

    return EXIT_SUCCESS;
}

bool licite(int** plateau, coup_t* coup, int J){ // A faire

    return coup->yf >= 0;
}

int localisation_bobail(int** plateau, int* x, int* y){
    if(plateau == NULL){
        printf("Erreur localisation_bobail: plateau manquant\n ");
        return EXIT_FAIL;
    }
    ///////////////////////// VERIF x et y
    for (int i = 0; i < SIZE; i++){
        for(int j = 0; j < SIZE; j++){
            if(plateau[i][j] == BOBAIL){
                *x = i;
                *y = j;
            }
        }
    }
    return EXIT_SUCCESS;
}

int saisie_coup(int** plateau, coup_t* coup, int J){
    if(plateau == NULL){
        printf("Erreur saisie_coup: plateau manquant\n ");
        return EXIT_FAIL;
    }
    if(coup == NULL){
        printf("Erreur saisie_coup: coup manquant\n ");
        return EXIT_FAIL;
    }

    int xi = -1;
    int xf = -1;
    int yi = -1;
    int yf = -1;

    coup->xi = xi;
    coup->yi = yi;
    coup->xf = xf;
    coup->yf = yf;

    int verif = 0;
    while(!licite(plateau, coup, J)){
        char c = ' '; // Séparateur et saut de ligne
        if(J == J1 || J == J2){ // Déplacement Joueur
            char c_xf = ' ';
            char c_xi = ' ';
            printf("Quel pion déplacer ? Où le déplacer ? (Format:A0 A0) :");
            verif = scanf("%c%d%c%c%d%c", &c_xi, &yi, &c, &c_xf, &yf, &c); // Saisie case départ
            if( verif == 6 && c== '\n'){
                xi = (int)c_xi - 65;
                coup->xi = xi;
                coup->yi = yi;
                xf = (int)c_xf - 65;
                coup->xf = xf;
                coup->yf = yf;
                
            }else{
                printf("Mauvais format de saisie \n");
            }
        }
        if(J == BOBAIL){ // Déplacement Bobail
            printf("Où déplacer le bobail ? Case :");
            char c_xf = ' ';
            verif = scanf("%c%d%c", &c_xf, &yf, &c);
            if( verif == 3){
                localisation_bobail(plateau,&(coup->xi), &(coup->yi));
                xf = (int)c_xf - 65;
                //printf("%d,%d\n", xf, yf);
                coup->xf = xf;
                coup->yf = yf;
            }else{
                printf("Mauvais format de saisie \n");
            }
        }
        
    }

    return EXIT_SUCCESS;
}



int mouvement(int** plateau, coup_t* coup){ /// UTILITE BOF
    if(plateau == NULL){
        printf("Erreur mouvement: plateau manquant\n ");
        return EXIT_FAIL;
    }
    if(coup == NULL){
        printf("Erreur mouvement: coup manquant\n ");
        return EXIT_FAIL;
    }
    plateau[coup->xf][coup->yf] = plateau[coup->xi][coup->yi];
    plateau[coup->xi][coup->yi] = VIDE;

    return EXIT_SUCCESS; 
}



int fin(int** plateau, bool* fini, int J_act, int* gagnant){
    if(plateau == NULL){
        printf("Erreur fin: plateau manquant\n ");
        return EXIT_FAIL;
    }
    if(fini == NULL){
        printf("Erreur fin: fini manquant\n ");
        return EXIT_FAIL;
    }
    if(gagnant == NULL){
        printf("Erreur fin: gagnant manquant\n ");
        return EXIT_FAIL;
    }

    int x = -1;
    int y = -1;

    localisation_bobail(plateau, &x, &y);
    if(y == 0){
        *fini = true;
        *gagnant = J2;
    }
    if(y == SIZE -1){
        *fini = true;
        *gagnant = J1;
    }

    bool h = (x > 0)&&(plateau[x-1][y] == VIDE);
    bool hg = (x > 0)&&(y > 0)&&(plateau[x-1][y-1] == VIDE);
    bool hd = (x > 0)&&(y < SIZE - 1)&&(plateau[x-1][y+1] == VIDE);
    bool g = (y > 0)&&(plateau[x][y-1] == VIDE);
    bool d = (y < SIZE - 1)&&(plateau[x][y+1] == VIDE);
    bool b = (x < SIZE - 1)&&(plateau[x+1][y] == VIDE);
    bool bg = (x < SIZE - 1)&&(y > 0)&&(plateau[x+1][y-1] == VIDE);
    bool bd = (x < SIZE - 1)&&(y < SIZE - 1)&&(plateau[x+1][y+1] == VIDE);

    

    if(!(h||hg||hd||g||d||b||bg||bd)){ /////////////////////////////////////// PB
        printf("Bloqué !\n");
        *fini = true;
        *gagnant = J_act;
    }

    return EXIT_SUCCESS; 
}

void destroy(int** plateau){
    for(int j = 0; j < SIZE; j++){
        free(plateau[j]);
    }
    free(plateau);
}