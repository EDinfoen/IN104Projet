#include "generic_list.h"
#include <math.h>
#include "evaluation.h"

typedef generic_list_t noeud_list_t; 

typedef enum {V, D, EC} statut_t;
    /*V = Victoire assurée;
      D = Défaite assurée;
      EC = partie En Cours (fin de partie avec simulation aléatoire)
    */

typedef struct noeud_ {float n; int N; statut_t statut; int** plateau; piece_t J; int code_coup; noeud_list_t* liste_fils;} noeud_t;
// n : Nombre de victoires de J 
// N : nombre d'explorations 
// statut : état partie pour J
// J : joeur qui vient de jouer pour être dans la sitation actuelle
// plateau : plateau actuel
// code_coup : code pour passer du plateau père au plateau actuel
// liste_fils : liste générique de noeud_t



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
        for(; elmt != NULL; elmt = generic_list_next(elmt)){
            noeud_t* fils=(noeud_t*)generic_list_data(elmt);
            if (fils->N == 0){ // Premier non vu 
                
                int deep_max;
                if (simulation(fils->plateau, fils->J, &deep_max, &res) == 0){
                    return EXIT_FAIL;
                }
                fils->statut=EC;
                if (res == 0){ 
                    //défaite de fils->J
                    if (deep_max==0){fils->statut = D;}
                }else{
                    //victoire
                    if (deep_max == 0){fils->statut = V;}
                }
                //mise à jour du fils qui a été exploré
                fils->N = 1;
                fils->n = res;
                break; // On ne regarde pas les autres fils tout de suite. 
            }
        }
    }else if(nbr_fils == 0){
        // Noeud sans fils donc son statut est défini comme V ou D, partie finie
        if (pere->statut==V){ res = 1;
        }else{ res = 0;}
    }else{
        // Noeud dont tous les fils ont été explorés au moins une fois
        noeud_t* max_elmt;
        float max_MCTS = 0;
        generic_list_elmt_t* elmt = generic_list_head(pere->liste_fils);
        for(; elmt != NULL; elmt = generic_list_next(elmt)){
            noeud_t* fils=(noeud_t*)generic_list_data(elmt);
            float MCTS = (fils->n) /(fils->N) + sqrt(2)*sqrt(logf((pere->N)/ (fils->N))); // Pas de Pb fils->N non nul. 
            if ( MCTS > max_MCTS){ 
                max_MCTS = MCTS;
                max_elmt = fils;
            }
            res = exploration(max_elmt);//resultat pour le pere
        }
    }

// mise à jour du noeud courant
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

    for (int i = 0; i < 500; i++){
        exploration(root);
    }
    printf("Nbr fils = %d\n",generic_list_size(root->liste_fils));
    printf("exploré\n");
    print_noeud_list(root->liste_fils);
    float ratio = 0.0;
    int code = 9999;

    generic_list_elmt_t* elmt = generic_list_head(root->liste_fils);
    for(; elmt != NULL; elmt = generic_list_next(elmt)){
        noeud_t* nd = ((noeud_t*)generic_list_data(elmt));
        if(nd->n/ nd->N > ratio){
            ratio = nd->n / nd->N; //////////////////// DIV euclid
            code = nd->code_coup;
            root = nd;
        }
    }
    
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
    printf("%d,%d,%d,%d\n",coup->xi,coup->yi,coup->xf,coup->yf);
    generic_list_elmt_t* elmt = generic_list_head(root->liste_fils);
        for(; elmt != NULL; elmt = generic_list_next(elmt)){
            noeud_t* fils=(noeud_t*)generic_list_data(elmt);
            if (fils->code_coup==code){
                root=fils;
                return EXIT_SUCCESS;
            }
        }
return EXIT_FAIL;
}
