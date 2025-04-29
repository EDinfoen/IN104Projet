#include "constante.h"

typedef struct coup_ {int xi; int yi; int xf; int yf;} coup_t;

int max(int a, int b){
    if(a>=b){
        return a;
    }
    return b;
}

int abs(int x){
    if(x < 0){
        return -x;
    }
    return x;
}

int init(int** plateau){
    /*
    Initialise le plateau en attribuant les valeurs aux cases contenant des pions. 
    Ne fait pas l'allocation mémoire.
    */
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

bool licite(int** plateau, coup_t* coup, int J){ 
    /*
    Vérifie la légalité d'un coup.
    Affiche par effet de bord : 'Coup non licite' ou 'Coup licite' 
    */ 
    

    int xi = coup->xi;
    int yi = coup->yi;
    int xf = coup->xf;
    int yf = coup->yf;

    if(xi < 0 || xi > SIZE-1 || yi < 0 || yi > SIZE-1 || plateau[xi][yi] != J){ // Case de départ
        printf("Coup non licite\n");
        return false;
    }
    if(xf < 0 || xf > SIZE-1 || yf < 0 || yf > SIZE-1 || plateau[xf][yf] != VIDE){ // Case de fin
        printf("Coup non licite\n");
        return false;
    }

    if( xi == xf && yi == yf){ // Pas de mouvement
        printf("Coup non licite\n");
        return false;
    }

    if( J == BOBAIL){ // Mouvement de Bobail
        if(max(abs(xf - xi), abs(yf - yi)) != 1){ // Bouge de 1 case
            printf("Coup non licite\n");
            return false;
        }

        printf("Coup licite\n");
        return true;
        
    }
    if( J == J1 || J == J2){ // Mouvement de joueur

        int dx = 2;
        int dy = 2;

        if( xf == xi ){
            dx = 0;
            dy = (yf - yi)/abs(yf - yi);
        }
        if(yf == yi){
            dx = (xf - xi)/abs(xf - xi);
            dy = 0;
        }

        if( abs(xi - xf) == abs(yi - yf) ){
            dx = (xf - xi)/abs(xf - xi); 
            dy = (yf - yi)/abs(yf - yi);
        }

        if(dx * dy >= 2){ // La direction n'est pas reconnue (dx et dy on gardé leur valeur par défaut)
            printf("Coup non licite\n");
            return false;
        }

        int x_temp = xi;
        int y_temp = yi;

        while (x_temp != xf && y_temp != yf){ // Chemin libre
            x_temp = x_temp + dx;
            y_temp = y_temp + dy;

            if(plateau[x_temp][y_temp] != VIDE){
                printf("Coup non licite\n");
                return false;
            }
        }

        if(xf+dx >= 0 && xf+dx < SIZE && yf+dy >= 0 && yf+dy < SIZE && plateau[xf+dx][yf+dy] == VIDE ){ // Mouvement pas complet
            printf("Coup non licite\n");
            return false;
        }

        printf("Coup licite\n");
        return true;
    }

    printf("Coup non licite\n"); // Ne doit pas arriver 
    return false;
    
}

int localisation_bobail(int** plateau, int* x, int* y){
    /*
    Attribu aux variables x et y la position du Bobail. 
    */
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

    /*
    Saisie un coup licite. Demande tant que le coup n'est pas licite.
    */

    int xi = -1;
    int xf = -1;
    int yi = -1;
    int yf = -1;

    coup->xi = xi;
    coup->yi = yi;
    coup->xf = xf;
    coup->yf = yf;

    int verif = 0;
    do{
        char c = ' '; // Séparateur et saut de ligne
        if(J == J1 || J == J2){ // Déplacement Joueur
            char c_xf = ' ';
            char c_xi = ' ';
            printf("Joueur %d : mouvement pion (Format:A0 A0) :", J+1);
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
            printf("Joueur %d : mouvement Bobail (Format:A0) :", J+1);
            char c_xf = ' ';
            verif = scanf("%c%d%c", &c_xf, &yf, &c);
            if( verif == 3){
                localisation_bobail(plateau,&(coup->xi), &(coup->yi));
                xf = (int)c_xf - 65;
                coup->xf = xf;
                coup->yf = yf;
            }else{
                printf("Mauvais format de saisie \n");
            }
        }
        
    }while(!licite(plateau, coup, J));

    return EXIT_SUCCESS;
}



int mouvement(int** plateau, coup_t* coup){ 
    /*
    Modifie le plateau en fonction du coup joué.
    */
    plateau[coup->xf][coup->yf] = plateau[coup->xi][coup->yi];
    plateau[coup->xi][coup->yi] = VIDE;

    return EXIT_SUCCESS; 
}



int fin(int** plateau, bool* fini, int J_act, int* gagnant){
    /*
    Verifie si la partie est finie. 
    Attribu au pointeur fini si la partie est finie ou non.
    Attribu au pointeur gagnant le joueur gagnant.
    */
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

    // Verification des cases libres autour du Bobail

    bool h = (x > 0)&&(plateau[x-1][y] == VIDE);
    bool hg = (x > 0)&&(y > 0)&&(plateau[x-1][y-1] == VIDE);
    bool hd = (x > 0)&&(y < SIZE - 1)&&(plateau[x-1][y+1] == VIDE);
    bool g = (y > 0)&&(plateau[x][y-1] == VIDE);
    bool d = (y < SIZE - 1)&&(plateau[x][y+1] == VIDE);
    bool b = (x < SIZE - 1)&&(plateau[x+1][y] == VIDE);
    bool bg = (x < SIZE - 1)&&(y > 0)&&(plateau[x+1][y-1] == VIDE);
    bool bd = (x < SIZE - 1)&&(y < SIZE - 1)&&(plateau[x+1][y+1] == VIDE);

    

    if(!(h||hg||hd||g||d||b||bg||bd)){ 
        printf("Bloqué !\n");
        *fini = true;
        *gagnant = J_act;
    }

    return EXIT_SUCCESS; 
}

void destroy(int** plateau){
    /*
    Désalloue la mémoire du plateau.
    */
    for(int j = 0; j < SIZE; j++){
        free(plateau[j]);
    }
    free(plateau);
}