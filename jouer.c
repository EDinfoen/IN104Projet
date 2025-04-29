#include "affichage.h"
#include "annexe.h"



int tour(int** plateau,coup_t* coup, int J ){
    saisie_coup(plateau, coup, J);
    mouvement(plateau, coup);
    affichage(plateau);

    return EXIT_SUCCESS;
}


int jouer_v1(){
    
    // Allocation du plateau
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

    // Initialisation du plateau

    init(plateau);

    printf("Symboles :\nJoueur J1 : x\nJoueur J2 : +\nBobail : o\n");

    if(affichage(plateau) == EXIT_FAIL){
        destroy(plateau); 
        return EXIT_FAIL;
    }
    
    // Début de la partie

    coup_t* coup = malloc(sizeof(coup_t));
    if (coup==NULL){
        free(coup);
        destroy(plateau);
        printf("Erreur init : allocation (coup)");
        return EXIT_FAIL;

    }
    
    tour(plateau, coup, J1);
    
    int J_act = J2;
    int gagnant = -1;
    bool fini = false;

    while(!fini){
        
        tour(plateau, coup, BOBAIL); // Mouvement du Bobail
           
        fin(plateau, &fini, J_act, &gagnant);

        if(fini){
            break;
        }

        tour(plateau, coup, J_act); // Mouvement du pion
        
        fin(plateau, &fini, J_act, &gagnant);
       
        J_act = 1 - J_act;
    }

    // Fin de partie

    printf("Joueur %d a gagné !\n", gagnant + 1);

    free(coup);
    destroy(plateau);

    return EXIT_SUCCESS;
}