#include "annexe.h"
#include "affichage.h"
#include "generic_list.h"

typedef generic_list_t noeud_list_t; 

typedef struct noeud_ {int n; int N; int end; int** plateau; piece J; int code_coup; noeud_list_t* liste_fils;} noeud_t;
// n : Nombre de victoires de J 
// N : nombre d'explorations 
// end : état partie 
// code_coup : code pour passer plateau père à actuel
// liste_fils : liste générique de noeud_t 

enum direction {N, NE, E, SE, S, SW, W, NW} ; // A supprimer après fusion

void print_noeud(noeud_t* nd){
    printf("#############################################\n");
    printf("Nb_victoire = %d\n", nd->n);
    printf("Nb_exploration = %d\n", nd->N);
    
    printf("Code du coup joué : %d\n", nd->code_coup);
    affichage(nd->plateau);
    //printf("A J%d de jouer.\n", (nd->J)+1);

}

void print_noeud_list(noeud_list_t* liste){
    generic_list_elmt_t* elmt = generic_list_head(liste);
    for(; elmt!=NULL; elmt = generic_list_next(elmt)){
        noeud_t* e = ((noeud_t*)generic_list_data(elmt));
        if( e == NULL){
            printf("sdfghj\n");
        }else{
            print_noeud(e);
        }
  }
}




bool avancer(int xi, int yi, enum direction dir, int* xf, int* yf, int **plateau){ // A supprimer après fusion
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

int init_noeud(noeud_t* noeud){
    noeud->n = 0;
    noeud->N = 0;

    noeud->liste_fils = malloc(sizeof(noeud_list_t));
    if(noeud->liste_fils == NULL){
        return EXIT_FAIL;
    }
    noeud->plateau = malloc(SIZE*sizeof(int*));
    if(noeud->plateau == NULL){
        free(noeud->liste_fils);
        return EXIT_FAIL;
    }
    for (int i = 0; i < SIZE; i++){
        (noeud->plateau)[i] =malloc(SIZE*sizeof(int));
        if((noeud->plateau)[i] == NULL){
            for(int j = 0; j < i; j++){
                free((noeud->plateau)[j]);
            }
            free(noeud->plateau);
            free(noeud->liste_fils);
        return EXIT_FAIL;
        }
    }
    generic_list_init(noeud->liste_fils, NULL, NULL, NULL);
    return EXIT_SUCCESS;
}

void copier_plt(int** source, int** destination){ // Il existe peut-être plus efficasse ?
    for(int i = 0; i < SIZE; i++){
        for(int j = 0; j < SIZE; j++){
            destination[i][j] = source[i][j];
        }
    }
}


int generation_fils(int** plateau, piece J, noeud_list_t* liste){ // PB pour initialiser le joueur sur les nouveaux plateaux.

    int xi;
    int yi;
    int xf;
    int yf;
    if (J == J1 || J == J2){
        //on récupère les positions des pions
        int pions[SIZE];
        int nb_pions = 0;
        
        for (int i = 0; i < SIZE; i ++){
            for(int j = 0; j < SIZE; j++){
                if (plateau[i][j] == J){
                    pions[nb_pions] = i*10 + j;
                    nb_pions ++;
                }
            }
        }
        //choix d'un pion pouvant bouger
        for(int pion_act = 0; pion_act < SIZE; pion_act++){
            xi = pions[pion_act] / 10;
            yi = pions[pion_act] % 10;
            //choix d'une direction
            for(int dir = 0; dir < 8; dir++){
                if(avancer(xi, yi, dir, &xf, &yf, plateau)){ // Si on peut bouger...
                    int x = xf;
                    int y = yf;
                    while(avancer(x, y, dir, &xf, &yf, plateau)){ //On continue tant que possible !
                        x = xf;
                        y = yf;
                    }
                    
                    // Ajout du noeud sur la liste
                    noeud_t* nd = malloc(sizeof(noeud_t));
                    
                    if(init_noeud(nd) == EXIT_SUCCESS){
                        //printf("%p \n", &(nd->plateau));
                        copier_plt(plateau, nd->plateau);
                        nd->plateau[xf][yf] = nd->plateau[xi][yi];
                        nd->plateau[xi][yi] = VIDE;
                        nd->code_coup = ((xi*10 + yi)*10 + xf)*10 + yf;
                        generic_list_ins_next(liste, NULL, nd); // Insertion en tête
                        //print_noeud(&&(nd));
                        


                    }else{
                        return EXIT_FAIL;
                    }
                }
            }
        }
    }else if (J == BOBAIL) {
        localisation_bobail(plateau, &xi, &yi);
        //choix d'une direction
        for(int dir = 0; dir < 8; dir++){
            if(avancer(xi, yi, dir, &xf, &yf, plateau)){ // Si on peut bouger...                
                // Ajout du noeud sur la liste
                noeud_t* nd = malloc(sizeof(noeud_t));
                //printf("%p \n", &nd);
                if(init_noeud(nd) == EXIT_SUCCESS){
                    //printf("%p \n", &(nd->plateau));
                    copier_plt(plateau, nd->plateau);
                    nd->plateau[xf][yf] = nd->plateau[xi][yi];
                    nd->plateau[xi][yi] = VIDE;
                    nd->code_coup = ((xi*10 + yi)*10 + xf)*10 + yf;
                    generic_list_ins_next(liste, NULL, nd); // Insertion en tête
                    //print_noeud(&&(nd));
                    

                }else{
                    return EXIT_FAIL;
                }
                
            }
       }
    }

    return EXIT_SUCCESS;
}








////////////////////////// TESTS ////////////////////////////////
// A supprimer après fusion




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
    coup_t* coup = malloc(sizeof(coup_t));
    if (coup == NULL){
        free(coup);
        destroy(plateau);
        printf("Erreur init : allocation (coup)");
        return EXIT_FAIL;

    }
    
    //Lors du 1er tour, pas de mvt du bobail.
    saisie_coup(plateau, coup, J1);
    mouvement(plateau, coup);
    affichage(plateau);

    noeud_list_t liste;

    generic_list_init(&liste, NULL, NULL, NULL);
    generation_fils(plateau, J1, &liste);
    printf("%d\n", generic_list_size (&liste));
    print_noeud_list(&liste);
    

    destroy(plateau);

    return EXIT_SUCCESS;
}

piece next_J(piece J){
    piece next_J;
    switch (J){
        case J1:
            next_J = B2;
        case B1 :
            next_J = J1;
        case J2:
            next_J = B1;
        case B2:
            next_J = J2;
    }
    return next_J;
}
bool exploration(noeud_t *root){
    int nbr_fils=len(root->liste_fils);
    if (nbr_fils==0){
        // pas de fils à la feuille (jamais exploré) donc à générer


        generation_fils(root->plateau,next_J, root->liste_fils);
    }


    if (nbr_fils>root->N){
        noeud_t fils=generic_list_head(root->liste_fils);
        for(;fils!=generic_list_tail(root->iste_fils);fils=generic_list_next(fils)){
            if (fils->N==0){
                int res=simulation(root->plateau, piece roo)
            }
        }
    }
}