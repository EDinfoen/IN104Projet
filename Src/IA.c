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
piece_t joueur_suiv(piece_t J){
    switch (J){
        case J1 :
        return B2;
        case J2 :
        return B1;
        case B1 :
        return J1;
        case B2 :
        return J2;
        default:
        return VIDE;
    }
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




   piece_t J_act = J;
   piece_t gagnant = VIDE;
   coup_t* coup = malloc(sizeof(coup_t));
   if(coup == NULL){
       return EXIT_FAIL;
   }
   while(!fin(plateau_temp, J_act, &gagnant) && (*deep_max <= DEEP)){
        *deep_max += 1;
        coup_aleatoire(plateau_temp, J_act, coup); 
        mouvement(plateau_temp, coup);       
        J_act = joueur_suiv(J_act);
        //affichage(plateau);
   }
   if(*deep_max > DEEP){
        *res = -1; // Profondeur dépasseé => defaite
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


int tour_IA(){
    noeud_t* root;
    init_noeud(root);

    for (int i = 0; i < 500; i++){
        exploration(root);
    }
    float ratio = 0.0;
    int code = 9999;
    noeud_t* next_root; 

    generic_list_elmt_t* elmt = generic_list_head(root->liste_fils);
    for(; elem != NULL; elem = generic_list_next(elem)){
        noeud_t* nd = ((noeud_t*)generic_list_data(elmt));
        if(nd.n / nd.N > ratio){
            ratio = nd.n / nd.N; //////////////////// DIV euclid
            code = nd.code_coup;
            next_root = nd;
        }
    }
    
    coup_t* coup;
    int temp = code_coup;
    coup->yf = temp%10;
    temp = temp/10;
    coup->xf = temp%10;
    temp = temp/10;
    coup->yi = temp%10;
    temp = temp/10;
    coup->xi = temp%10;
    



}

////////////////////////////////////////////////////////////////////////
//Reprise du code de la fonction jouer pour tester coup aléatoire sur le plateau de départ.

int main(){

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

    if(affichage(plateau) == EXIT_FAIL){
        destroy(plateau); 
        return EXIT_FAIL;
    }

   
    printf("Symboles :\nJoueur J1 : x\nJoueur J2 : +\nBobail : o\n");
    //int xf,yf;
    /*printf("%d,", avancer(0, 3, SE, &xf, &yf, plateau));
    printf("%d,%d\n", xf, yf);
    printf("%d", piece_mobile(plateau, 4, 3));
    coup_t* coup = coup_aleatoire(plateau, BOBAIL);

    mouvement(plateau, coup);
    affichage(plateau);*/

    int deep = 0;
    int res = 0;
    
    simulation(plateau, J1, &deep, &res);
    printf("########################################");
    printf("%d, %d\n",res, deep);


    destroy(plateau);

    return EXIT_SUCCESS;
}