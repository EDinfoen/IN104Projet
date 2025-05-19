#include "annexe.h"
#include "affichage.h"
#include "generic_list.h"
#include <math.h>

typedef generic_list_t noeud_list_t; 

typedef struct noeud_ {float n; int N; statut_t statut; int** plateau; piece_t J; int code_coup; noeud_list_t* liste_fils;} noeud_t;
// n : Nombre de victoires de J 
// N : nombre d'explorations 
// statut : état partie pour J
// J : joeur qui vient de jouer pour être dans la sitation actuelle
// plateau : plateau actuel
// code_coup : code pour passer du plateau père au plateau actuel
// liste_fils : liste générique de noeud_t 

enum direction {N, NE, E, SE, S, SW, W, NW} ; // A supprimer après fusion

void print_noeud(noeud_t* nd){
    printf("#############################################\n");
    printf("Nb_victoire = %f\n", nd->n);
    printf("Nb_exploration = %d\n", nd->N);
    
    printf("Code du coup joué : %d\n", nd->code_coup);
    affichage(nd->plateau);
    //printf("A J%d de jouer.\n", (nd->J)+1);

}

void print_noeud_list(noeud_list_t* liste){
    generic_list_elmt_t* elmt = generic_list_head(liste);
    for(; elmt!=NULL; elmt = generic_list_next(elmt)){
        noeud_t* nd = ((noeud_t*)generic_list_data(elmt));
        print_noeud(nd);
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
    /*
    Initialise le noeud avec VALEURS PAR DEFAUTS:
        n = 0.0
        N = 0
        statut = EC
        J = VIDE
        code_coup = 9999
        liste_fils = liste vide
    Alloue mémoire de :
        plateau 
        liste_fils
    */
    noeud->n = 0.0;
    noeud->N = 0;
    noeud->statut = EC;
    noeud->code_coup = 9999;
    noeud->J = VIDE;

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
        (noeud->plateau)[i] = malloc(SIZE*sizeof(int));
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


int generation_fils(int** plateau, piece_t J, noeud_list_t* liste){ 
    /*
    
    */

    int xi;
    int yi;
    int xf;
    int yf;
    if (J == J1 || J == J2){
        // On récupère les positions des pions
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
        // Choix d'un pion pouvant bouger
        for(int pion_act = 0; pion_act < SIZE; pion_act++){
            xi = pions[pion_act] / 10;
            yi = pions[pion_act] % 10;
            // Choix d'une direction
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
                        nd->n = 0.0;
                        nd->N = 0;
                        nd->statut = EC;
                        copier_plt(plateau, nd->plateau);
                        nd->plateau[xf][yf] = nd->plateau[xi][yi];
                        nd->plateau[xi][yi] = VIDE;
                        nd->J = J;
                        nd->code_coup = ((xi*10 + yi)*10 + xf)*10 + yf;
                        
                        generic_list_ins_next(liste, NULL, nd); // Insertion en tête
                        //print_noeud(&&(nd));
                        
                    }else{
                        return EXIT_FAIL;
                    }
                }
            }
        }
    }else if (J == B1 || J == B2) {
        localisation_bobail(plateau, &xi, &yi);
        // Choix d'une direction
        for(int dir = 0; dir < 8; dir++){
            if(avancer(xi, yi, dir, &xf, &yf, plateau)){ // Si on peut bouger... on le fait mais on s'arrete            
                // Ajout du noeud sur la liste
                noeud_t* nd = malloc(sizeof(noeud_t));

                if(init_noeud(nd) == EXIT_SUCCESS){
                    nd->n = 0.0;
                    nd->N = 0;
                    nd->statut = EC;
                    copier_plt(plateau, nd->plateau);
                    nd->plateau[xf][yf] = nd->plateau[xi][yi];
                    nd->plateau[xi][yi] = VIDE;
                    nd->J = J;
                    nd->code_coup = ((xi*10 + yi)*10 + xf)*10 + yf;
                    
                    generic_list_ins_next(liste, NULL, nd); // Insertion en tête

                }else{
                    return EXIT_FAIL;
                }
                
            }
       }
    }

    return EXIT_SUCCESS;
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


int exploration(noeud_t *pere){
    /*
    Renvoie le score (1.0 victoire 0.0 défaite) de J sur le plateau après une descente.
    */
    int nbr_fils = generic_list_size(pere->liste_fils);

    if (nbr_fils == 0 && pere->statut == EC){
        // pas de fils à la feuille (jamais exploré) donc à générer
        piece_t nextJ = next_J (pere->J);
        generation_fils(pere->plateau, nextJ, pere->liste_fils);
    }

    int res;

    if (nbr_fils > pere->N){
        //fils générés mais pas tous explorés
        generic_list_elmt_t* elmt = generic_list_head(pere->liste_fils);
        for(; elmt != NULL; elmt = generic_list_next(elmt)){ // Je pense que le dernier elem n'est pas visité ?
            noeud_t* fils=generic_list_data(elmt);
            if (fils->N == 0){ // Premier non vu 
                
                piece_t nextJ = next_J(pere->J);
                int deep_max;
                if (simulation(pere->plateau, nextJ, &deep_max, &res) == 0){
                    return EXIT_FAIL;
                }
                if (res == 0){ 
                    //défaite
                    if (deep_max==0){
                        fils->statut = D; // Statut du noeud fils ? Donc inversion V et D ?
                    }else{
                        fils->statut = EC;
                    }
                }else{
                    //victoire
                    fils->statut = EC; 
                    if (deep_max == 0){
                        fils->statut = V;
                    }else{
                        fils->statut = EC;
                    }
                }
                //mise à jour du fils qui a été exploré
                fils->N = 1;
                fils->n = res;
                if (nextJ == B1 || nextJ == B2){
                    res = 1 - res;
                }
                break; // On ne regard pas les autres fils tout de suite. 
            }
        }
    }else if(nbr_fils == 0){
        // Noeud sans fils donc son statut est défini comme V ou D, partie finie
        if (pere->statut==V){ res = 1;
        }else{ res = 0;}
    }else{
        // Noeud dont tous les fils ont été explorés au moins une fois
        noeud_t* max_elmt;
        float max_MCTS = 0;;
        generic_list_elmt_t* elmt = generic_list_head(pere->liste_fils);
        for(; elmt != NULL; elmt = generic_list_next(elmt)){
            noeud_t* fils=(noeud_t*)generic_list_data(elmt);
            float MCTS = (fils->n) /(fils->N) + sqrt(2)*sqrt(log((pere->N)/ (fils->N))); // Pas de Pb fils->N non nul. 
            if ( MCTS > max_MCTS){ 
                max_MCTS = MCTS;
                max_elmt = fils;
            }
            res = exploration(max_elmt);
        }
    }

// mise à jour du noeud courant
pere->N += 1;
if (pere->J == B1 || pere->J == B2){  
    pere->n += res; // Je pense que c'est 1 - res ? 
    return res;
}
pere->n += 1-res;
return 1-res;
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
    

    noeud_t noeud;
    init_noeud(&noeud);
    noeud.plateau=plateau;
    exploration (&noeud);
    print_noeud_list(noeud.liste_fils);
    
    int c=3;
    float d=4.99;
    d=d+c;
    printf("\nd=%f",d);


    return EXIT_SUCCESS;
}