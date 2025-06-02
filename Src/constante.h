/*
Contient les constantes, les includes de bibliothèques standarts et certain types utiles à plusieurs reprises.
*/
#ifndef CONSTANTE

    #define CONSTANTE

    #include <stdio.h>
    #include <stdlib.h>
    #include <stdbool.h> // 0 = False
    #include <time.h>

    #define EXIT_FAIL 1

    #define SIZE 5 // Taille du plateau
    #define DEEP 1000 // Nombre de coup maximal joué pour simuler une fin de partie
    #define NB_DESC 10000 // Nombre de descentes lors d'une exploration de l'arbre

    typedef enum {J1, J2, BOBAIL, VIDE, B1, B2} piece_t;
    

#endif