#include "jouer.h"

int main (){
    int jeu;
    do{
        printf("Pour jouer contre une IA, entrer 0. \nPour jouer contre un autre joueur, entrer 1.\n");
    }while(scanf("%d", &jeu) == 0);
    if (jeu == 0){
        jouer_IA();
    }else if (jeu == 1){
        jouer_pvp(); 
    }   
    return EXIT_SUCCESS;
}