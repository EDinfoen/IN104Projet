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
    #define EXIT_SUCCESS 0

    #define SIZE 5
    #define DEEP 1000000

    typedef enum {J1, J2, BOBAIL, VIDE, B1, B2} piece;

#endif