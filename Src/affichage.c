#include "constante.h"

int affichage(int** plateau){
    /*
    Affiche le plateau.
    */

    if( plateau == NULL){
        printf("Erreur affichage: plateau manquant\n ");
        return EXIT_FAIL;
    }
    printf("    0   1   2   3   4  \n");
    printf("  ---------------------\n");
    
    for (int i = 0; i < SIZE; i ++){
        printf("%c |", (char)(i+65));
        for(int j = 0; j < SIZE; j++){
            if(plateau[i][j] == J1){
                printf(" x |");
            }
            if(plateau[i][j] == J2){
                printf(" + |");
            }
            if(plateau[i][j] == BOBAIL){
                printf(" o |");
            }
            if(plateau[i][j] == VIDE){
                printf("   |" );
            }
            
        }
        printf("\n");
        printf("  ---------------------\n");

    }
return EXIT_SUCCESS;
}
