#include "affichage.h"
#include "annexe.h"

#define DEBUG

int tour(int** plateau,coup_t* coup, int J ){/// Pas de vérif (flemme)
    if(saisie_coup(plateau, coup, J) == EXIT_FAIL || mouvement(plateau, coup) == EXIT_FAIL || affichage(plateau) == EXIT_FAIL){
        return EXIT_FAIL;
    }

    return EXIT_SUCCESS;
}


int jouer_v1(){

    //////////////////////////////////////////////////////////////////////////////
    
    #ifndef DEBUG
    int** plateau; 
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
    }
    printf("deb\n");
    if(init(plateau) == EXIT_FAIL){
        return EXIT_FAIL;
    }
    #else
    int** plateau = NULL;
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
    }

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


    #endif
    /////////////////////////////////////////////////////////
    if(affichage(plateau) == EXIT_FAIL){
        destroy(plateau); 
        return EXIT_FAIL;
    }

    coup_t* coup = malloc(sizeof(coup_t));/////////////////////
    if (coup==NULL){
        free(coup);
        destroy(plateau);
        printf("Erreur init : allocation coup");
        return EXIT_FAIL;

    }
    printf("Symboles :\nJoueur J1 : x\nJoueur J2 : +\nBobail : o\n");
    if(tour(plateau, coup, J1) == EXIT_FAIL){
        return EXIT_FAIL;
    }
    
    int J_act = J2;
    int gagnant = -1;
    bool fini = false;

    while(!fini){
        
        if(tour(plateau, coup, BOBAIL) == EXIT_FAIL){
            free(coup);
            destroy(plateau);
            return EXIT_FAIL;
        }
        if(fin(plateau, &fini, J_act, &gagnant) == EXIT_FAIL){
            free(coup);
            destroy(plateau);
            return EXIT_FAIL;
        }
        if(fini){
            break;
        }

        if(tour(plateau, coup, J_act) == EXIT_FAIL){
            free(coup);
            destroy(plateau);
            return EXIT_FAIL;
        }
        
        if(fin(plateau, &fini, J_act, &gagnant) == EXIT_FAIL){
            free(coup);
            destroy(plateau);
            return EXIT_FAIL;
        }
       
        J_act = 1 - J_act;
    }

    free(coup);
    destroy(plateau);

    return EXIT_SUCCESS;
}