#include "annexe.h"
#include "affichage.h"


enum direction {N, NE, E, SE, S, SW, W, NW} ;

void copier_plt(int** source, int** destination){ // Il existe peut-être plus efficasse ?
    for(int i = 0; i < SIZE; i++){
        for(int j = 0; j < SIZE; j++){
            destination[i][j] = source[i][j];
        }
    }
}


bool avancer(int xi, int yi,enum direction dir, int* xf, int* yf, int **plateau){
    /*
    Vérifie que le pion se trouvant en (xi,yi) peut avancer d'une case dans la direction dir.
    Si oui, elle modifie les pointeurs xf et yf pour qu'ils indiquent la case suivante.
    */    
    int x,y;
    switch (dir)
    {
    case N:
        x = xi; y = yi - 1;
        break;
    case NE:
        x = xi + 1; y = yi - 1;
        break;
    case E : 
        x = xi + 1; y = yi;
        break;    
    case SE:
        x = xi + 1; y = yi + 1;
        break;
    case S:
        x = xi; y = yi + 1;
        break;
    case SW:
        x = xi - 1; y = yi + 1;
        break;
    case W:
        x = xi - 1; y = yi;
        break;
    case NW:
        x = xi - 1; y = yi - 1;
        break;
    }

    // Vérification que la nouvelle case est dans le plateau et inoccupée.

    if( 0 > x || x > (SIZE - 1) || y < 0 || y > (SIZE - 1) ){return false;}
    if (plateau[x][y] == VIDE ){ *xf = x; *yf = y; return true;}
    return false;
}

bool piece_mobile(int** plateau, int x, int y){
    /*
    Test si un le pion en (x,y) a au moins une direction dans
    laquelle il peut avancer, c'est-à-dire qu'il y a une case vide
    autour de lui.
    */

    //Parcours des 9 cases alentours du pion.
    for(int i = -1; i < 2; i++){
        for(int j = -1; j < 2; j++){
            if (0 < x+i && x+i < (SIZE - 1) && y+j > 0 && y+j < (SIZE - 1)){
                if (plateau[x + i][y + j] == VIDE){return true;}
            }
        }
    }
    return false;
}
piece_t next_J(piece_t J){
    // Renvoie le prochain joueur ou bobail
    piece_t next_J;
    switch (J){
        case J1:
            next_J = B2;
            break;
        case B1 :
            next_J = J1;
            break;
        case J2:
            next_J = B1;
            break;
        case B2:
            next_J = J2;
            break;
        default:
            next_J = VIDE;
            break;
    }
    return next_J;
}

int coup_aleatoire (int** plateau, piece_t joueur, coup_t* coup){
    /*
    Renvoie un coup aléatoire possible pour joueur.
    Allocation de la mémoire du coup retourné.
    */
    srand(time(NULL));
    int xi, yi;
    if (joueur == J1 || joueur == J2){
        //on récupère les positions de pions
        int pions[SIZE];
        int nbr_pions = 0;
        //printf("position:\n");
        for (int i = 0; i < SIZE; i ++){
            for(int j = 0; j < SIZE; j++){
                if (plateau[i][j] == joueur){
                    pions[nbr_pions] = i*10 + j;
                    //printf("%d,%d\n", i, j);
                    nbr_pions ++;
                }
                if(nbr_pions == SIZE){break;} //tous les pions trouvés
            }
        }
        //printf("trouve\n");
        
        //choix d'un pion pouvant bouge
        bool bloque = true;
        while(bloque){
            int k = rand()%(nbr_pions+1);
            //printf("k=%d\n", k);
            xi = pions[k]/10;
            yi = pions[k]%10;
            //printf("%d,%d", xi, yi);
            if(!piece_mobile(plateau, xi, yi)){
                pions[k] = pions[nbr_pions];
                nbr_pions--;
                //printf("bloque\n");
            }else{bloque = false;}
        }
        //printf("mobile");
    }else{
        localisation_bobail(plateau, &xi, &yi);
        //printf("bobail\n");
    }

    coup->xi = xi;
    coup->yi = yi;

    //choix d'une direction
    bool avance = false;
    enum direction directions[8] = {N, NE, E, SE, S, SW, W, NW};
    int nbr_dir = 7;
    int xf, yf;
    while(!avance){
        int k = rand()%(nbr_dir+1);
        //printf("k2=%d\n", k);
        enum direction dir = directions[k];
        avance = avancer(xi, yi, dir, &xf, &yf, plateau);
        if(!avance){//direction bloquee il faut changer
            //on place la direction testée en bout de tableau pour ne plus la choisir
            directions[k] = directions[nbr_dir];
            nbr_dir--;
        }else if(joueur != B1 && joueur != B2){//deplacement jusqu'au bout de la direction
            bool max = avancer(xf, yf, dir, &xf, &yf, plateau);
            while(max){//on peut peut-être encore avancer
                max = avancer(xf, yf, dir, &xf, &yf, plateau);
            }
        }
    }
    coup->xf = xf;
    coup->yf = yf;

    return EXIT_SUCCESS;
}


int simulation(int** plateau, piece_t J, int* deep_max, int* res){
    /* 
    Alloue au pointeur res le resultat de la simulation. 1 = victoire; 0 = defaite
    Alloue au pointeur deep_max le nombre de coup joué avant la fin de la simaulation. 
    Si deep_max = 0, la partie est finie. 
    */

    int** plateau_temp = malloc(SIZE*sizeof(int*));
    if(plateau_temp == NULL){
        return EXIT_FAIL;
    }
    for (int i = 0; i < SIZE; i++){
        plateau_temp[i] =malloc(SIZE*sizeof(int));
        if(plateau_temp[i] == NULL){
            for(int j = 0; j < i; j++){
                free(plateau_temp[j]);
            }
            free(plateau_temp);
        return EXIT_FAIL;
        }
    }
    copier_plt(plateau, plateau_temp);




   piece_t J_act = next_J(J);
   piece_t gagnant = VIDE;
   coup_t* coup = malloc(sizeof(coup_t));
   if(coup == NULL){
       return EXIT_FAIL;
   }
   while(!fin(plateau_temp, J_act, &gagnant) && (*deep_max <= DEEP)){
        *deep_max += 1;
        printf("deep=%d",*deep_max);
        coup_aleatoire(plateau_temp, J_act, coup); 
        mouvement(plateau_temp, coup);       
        J_act = next_J(J_act);
        //affichage(plateau);
   }
   if(*deep_max > DEEP){
        *res = 0; // Profondeur dépasseé => defaite
        return EXIT_SUCCESS;
   }
   if(gagnant == J){
        *res =  1;
        return EXIT_SUCCESS;
   }else{
        *res = 0;
        return EXIT_SUCCESS;
   }
}





