#include "generic_list.h"
#include <math.h>
#include "evaluation.h"

typedef generic_list_t noeud_list_t; 

typedef enum {V, D, EC} statut_t;
    /*
    V = Victoire assurée;
    D = Défaite assurée;
    EC = partie En Cours (fin de partie avec simulation aléatoire)
    */

typedef struct noeud_ {float n; int N; statut_t statut; int** plateau; piece_t J; int code_coup; noeud_list_t* liste_fils;} noeud_t;
    /* 
    n : Nombre de victoires de J 
    N : nombre d'explorations du noeud
    statut : état de la partie pour J
    J : joueur qui vient de jouer pour être dans la sitation actuelle
    plateau : plateau actuel
    code_coup : code pour passer du plateau père au plateau actuel
    liste_fils : liste générique de noeud_t correspondant aux coups possibles pour le joueur suivant sur le plateau actuel
    */



void print_noeud(noeud_t* nd){
    /*
    Affiche le contenu d'un noeud.
    */
    printf("#############################################\n");
    printf("Nb_victoire = %f\n", nd->n);
    printf("Nb_exploration = %f\n", nd->N);
    printf("Statut = %d\n", nd->statut);
    printf("Nb_fils = %d\n", generic_list_size(nd->liste_fils));
    printf("J%d vient de jouer.\n", (nd->J)+1);
    printf("Code du coup joué : %d\n", nd->code_coup);
    affichage(nd->plateau);
    printf("#############################################\n");
}

void print_noeud_list(noeud_list_t* liste){
    /*
    Affiche une liste de noeud.
    */
    generic_list_elmt_t* elmt = generic_list_head(liste);
    for(; elmt!=NULL; elmt = generic_list_next(elmt)){
        noeud_t* nd = ((noeud_t*)generic_list_data(elmt));
        print_noeud(nd);
  }
}



int init_noeud(noeud_t* noeud){
    /*
    Initialise le noeud avec des VALEURS PAR DEFAUTS:
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



int generation_fils(int** plateau, piece_t J, noeud_list_t* liste){ 
    /*
    Ajoute à liste les noeuds correspondants aux coups possibles pour J sur le plateau. 
    */

    int xi = 0; // Par défaut
    int yi = 0;
    int xf = 0;
    int yf = 0;

    if(J == BOBAIL || J == VIDE){
        printf("STOP : J == BOBAIL || J == VIDE \n");
        return EXIT_FAIL;
    }

    ////////////////////// Coups de pions //////////////////////////////

    if (J == J1 || J == J2){ 
        
        // Obtention des positions
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
                    nbr_fils_generes++;
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
                    }else{
                        return EXIT_FAIL;
                    }
                }
            }
        }
    }
    
    ////////////////////// Coups de Bobail //////////////////////////////
    
    if (J == B1 || J == B2) {
        // Obtention des positions
        localisation_bobail(plateau, &xi, &yi);
        
        // Choix d'une direction
        for(int dir = 0; dir < 8; dir++){
            if(avancer(xi, yi, dir, &xf, &yf, plateau)){ // Si on peut bouger... on le fait mais on s'arrete            
                
                // Ajout du noeud sur la liste
                noeud_t* nd = malloc(sizeof(noeud_t));
                nbr_fils_generes++;
                
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




int exploration(noeud_t *pere){
    /*
    Renvoie le score (1.0 victoire 0.0 défaite) de J sur le plateau après une descente.
    */

    if(pere == NULL){
        printf("STOP pere == NULL\n ");
    }
    if(pere->liste_fils == NULL){
        printf("STOP pere->liste_fils == NULL\n ");
    }
    if(pere->J == VIDE){
        printf("STOP pere->J == VIDE\n");
    }
    
    /////////////////// Génération des fils des feuilles sans fils ///////////////////////

    int nbr_fils = generic_list_size(pere->liste_fils);

    if (nbr_fils == 0 && pere->statut == EC){
        piece_t nextJ = next_J (pere->J);
        generation_fils(pere->plateau, nextJ, pere->liste_fils);        
    }

    /////////////////////////////// Descente dans l'arbre ////////////////////////////////

    int res = 0.0; // Par défaut
    nbr_fils = generic_list_size(pere->liste_fils);
    
    /********************************** Feuille *****************************************/

    if (nbr_fils+1 > pere->N){                 
        //fils générés mais pas tous explorés

        generic_list_elmt_t* elmt = generic_list_head(pere->liste_fils);
        for(; elmt != NULL; elmt = generic_list_next(elmt)){
            noeud_t* fils=(noeud_t*)generic_list_data(elmt);
            if (fils->N == 0){ // Premier non vu 
                
                // Evaluation du noeud par simulation
                int deep_max = 0;
                simulation(fils->plateau, fils->J, &deep_max, &res);
                
                //Mise à jour du fils qui a été exploré

                if (deep_max == 0){ 
                    if (res == 0){fils->statut = D;} //défaite de fils->J
                    else{fils->statut = V;} //victoire de fils->J
                }

                fils->N = 1;
                fils->n = res;
                if(pere->J == J1 || pere->J == J2){
                    res = 1 - res;
                }

                break; // On ne regarde pas les autres fils tout de suite. 
            }
        }


    /********************************* Noeud sans fils **********************************/

    }else if(nbr_fils == 0 && pere->statut != EC){ 
        // Noeud sans fils donc son statut est défini comme V ou D, partie finie
        if (pere->statut == V){res = 1;} 
        else{res = 0;}


    /********************************* Noeud avec fils **********************************/

    }else{
        // Noeud dont tous les fils ont été explorés au moins une fois
        noeud_t* max_elmt;
        float max_MCTS = 0.0;

        generic_list_elmt_t* elmt = generic_list_head(pere->liste_fils);
        for(; elmt != NULL; elmt = generic_list_next(elmt)){
            noeud_t* fils=(noeud_t*)generic_list_data(elmt);
            
            float MCTS = (fils->n) /(fils->N) + sqrt(2)*sqrt(logf(pere->N)/ (fils->N)); // Pas de Pb fils->N non nul. 
            if ( MCTS > max_MCTS){ 
                max_MCTS = MCTS;
                max_elmt = fils;
            }
        }
            res = exploration(max_elmt); //resultat pour le pere
        }

    //////////////////////////// Mise à jour du noeud courant ///////////////////////////
    
    pere->N += 1;
    pere->n += res;

    if (pere->J == B1 || pere->J == B2){ return 1-res;}
    return res;
}


int saisie_coup_IA(noeud_t* root, coup_t* coup){
    /*
    Attribue à coup le coup optimale après NB_DESC descentes dans l'arbre de racine root.
    Attribue à root le noeud associé au coup à joué (déplacement dans l'arbre)
    */

    for (int i = 0; i < NB_DESC; i++){
        exploration(root);
    }
    
    float ratio = -10.0;
    int code = 9999;
    if(root->liste_fils == NULL){
        printf(" STOP : root->liste_fils == NULL\n");
    }
    
    generic_list_elmt_t* elmt = generic_list_head(root->liste_fils);
    for(; elmt != NULL; elmt = generic_list_next(elmt)){
        
        noeud_t* nd = ((noeud_t*)generic_list_data(elmt));
        
        if( nd == NULL){
            printf("STOP :  nd == NULL\n ");
        }
        
        if( nd->N == 0){
            printf("DIV ZERO\n");
        }
        if(nd->n/ nd->N > ratio){
            ratio = nd->n / nd->N;
            code = nd->code_coup;
            *root = *nd;
        }
    }
    printf("Ratio : %f, %d\n", ratio, root->statut);
    int temp = code;
    coup->yf = temp%10;
    temp = temp/10;
    coup->xf = temp%10;
    temp = temp/10;
    coup->yi = temp%10;
    temp = temp/10;
    coup->xi = temp%10;

    return EXIT_SUCCESS;
}

int deplacement_arbre(noeud_t* root, coup_t* coup){
    int code = ((coup->xi*10 + coup->yi)*10 + coup->xf)*10 + coup->yf;
    
    if (generic_list_size(root->liste_fils) == 0){
        
        if(root->J == VIDE){
            printf("STOP ");
        }
        piece_t nextJ = next_J (root->J);
        if(nextJ == VIDE){
            printf("STOP ");
        }
        generation_fils(root->plateau, nextJ, root->liste_fils);

        
    }
    generic_list_elmt_t* elmt = generic_list_head(root->liste_fils);
    for(; elmt != NULL; elmt = generic_list_next(elmt)){
        noeud_t* fils = (noeud_t*)generic_list_data(elmt);
        
        if (fils->code_coup == code){
            
            *root = *fils;
            return EXIT_SUCCESS;
        }
    }
    
    return EXIT_FAIL;
}
