#include "jouer.h"

int main (int argc, char** argv){
    
    int jeu = 0;
    char c = ' ';

    do{
        printf("Pour jouer contre une IA, entrer 0.\nPour jouer contre un autre joueur, entrer 1.\n");
    }while(scanf("%d%c", &jeu, &c) != 2 && c != '\n');
    
    if (jeu == 0){
        jouer_IA();
    }else{ if(jeu == 1){
        jouer_pvp(); 
    }}
    return EXIT_SUCCESS;
}