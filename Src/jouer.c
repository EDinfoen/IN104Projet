#include "affichage.h"
#include "exploration.h"


int tour(int** plateau, coup_t* coup, piece_t J_act, piece_t p_mouv ){
    /*
    Exécute un tour : demande de coup au joueur J, modification de la position du pion, affichage du nouvel état du plateau.
    */
    saisie_coup(plateau, coup, J_act, p_mouv);
    mouvement(plateau, coup);
    affichage(plateau);

    return EXIT_SUCCESS;
}

int tour_IA(int** plateau, coup_t* coup, noeud_t* root ){
    /*
    Exécute un tour : calcul le coup optimal, modification de la position du pion, affichage du nouvel état du plateau.
    */
      
    saisie_coup_IA(root, coup);
    mouvement(plateau, coup);
    affichage(plateau);

    return EXIT_SUCCESS;
}

int jouer_pvp(){
    /*
    Implémente une partie de joueur contre joueur. La fonciton s'arrête quand l'un d'eux a gagné.
    */
    

    ////////////////////// PREPARATION PARTIE //////////////////////////


    /************** Initialisation du plateau et du coup **************/

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

    coup_t* coup = malloc(sizeof(coup_t));
    if (coup == NULL){
        free(coup);
        destroy(plateau);
        printf("Erreur init : allocation (coup)");
        return EXIT_FAIL;

    }

    // Initialisation du plateau
    init(plateau);



    ////////////////////////  PARTIE  ////////////////////////////////
    
    /************************ Début de la partie *********************/

    printf("------------------------------------------------\n \n \n");
    printf("Symboles :\n    Joueur 1 : x\n    Joueur 2 : +\n    Bobail : o\n\n\n");
    printf("------------------------------------------------\n\n");

    if(affichage(plateau) == EXIT_FAIL){
        destroy(plateau); 
        return EXIT_FAIL;
    }
 
    
    /************************** Premier tour *************************/

    tour(plateau, coup, J1, J1);
    
    piece_t J_act = J2;
    piece_t gagnant = VIDE; // Par défaut

 
    /************************** Autres tours *************************/

    do{
        tour(plateau, coup, J_act, BOBAIL); // Mouvement du Bobail
           
        if(fin(plateau, J_act, &gagnant)){
            break;
        }

        tour(plateau, coup, J_act, J_act); // Mouvement du pion
    
        J_act = 1 - J_act;

    }while(!fin(plateau, J_act, &gagnant));

    /************************** Fin de partie *************************/

    printf("Joueur %d a gagné !\n", gagnant + 1);
    


    ////////////////////////  LIBERATION MEMOIRE  ///////////////////////
    free(coup);
    destroy(plateau);

    return EXIT_SUCCESS;
}


int jouer_IA(){ 
    /*
    Implémente une partie d'un joueur contre une IA (Monte-Carlo). La fonciton s'arrête quand l'un d'eux a gagné.
    */

    ////////////////////// PREPARATION PARTIE //////////////////////////


    /************** Initialisation du plateau et du coup **************/
    
    // Allocation mémoire
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

    coup_t* coup = malloc(sizeof(coup_t));
    if (coup == NULL){
        free(coup);
        destroy(plateau);
        printf("Erreur init : allocation (coup)\n");
        return EXIT_FAIL;
    }

    piece_t gagnant = VIDE; // Par défaut

    // Initialisation du plateau 
    init(plateau);
    
    /******************** Initialisation de l'IA ********************/
    
    // Création de l'arbre 
    noeud_t* root=malloc(sizeof(noeud_t));
    if (root==NULL){
        printf("Erreur init: allocation root\n");
        return EXIT_FAIL;
    }
    init_noeud(root);
    copier_plt(plateau, root->plateau);
    root->J = B1;

    // Choix du joueur de l'IA
    printf("Voulez-vous jouer en premier ? (y ou n)");
    char ordre = ' ';
    char c = ' ';
    int verif = scanf("%c%c",&ordre, &c);
    if (verif != 2 || c != '\n'){
        printf("Erreur saisie ordre joueur\n");
        return EXIT_FAIL;
    }
    piece_t IA = VIDE; // J1 ou J2
    if( ordre == 'y'){
        IA = J2; 
    }else{
        IA = J1; 
    }
    



    ////////////////////////  PARTIE  ////////////////////////////////
    
    /************************ Début de la partie *********************/
    
    printf("------------------------------------------------\n \n \n");
    if(IA == J1){
        printf("Symboles :\n    IA : x\n    Vous : +\n    Bobail : o\n\n\n");
    }else{
        printf("Symboles :\n    Vous : x\n    IA : +\n    Bobail : o\n\n\n");
    }
    printf("------------------------------------------------\n\n");

    if(affichage(plateau) == EXIT_FAIL){
        destroy(plateau); 
        return EXIT_FAIL;
    }

    /************************** Premier tour *************************/

    piece_t J_act = VIDE; // J1 ou J2
    if(IA == J2){ // Le joueur commence
        //Lors du 1er tour, pas de mvt du bobail.        
        tour(plateau, coup, J1, J1);
        
        if (deplacement_arbre(root, coup) != EXIT_SUCCESS){
            printf("Erreur : deplacement racine\n");
            return EXIT_FAIL;
        }

    }else{ // L'IA commence
        tour_IA(plateau, coup, root); 
    }
    
    J_act = J1; 
    
    /************************** Autres tours *************************/
    
    do{ 
        J_act = 1 - J_act;

        if(J_act == IA){// Tour de l'IA
  
            tour_IA(plateau, coup, root); // Mouv du Bobail (contient le déplacement dans l'arbre)
            
            if(fin(plateau, J_act, &gagnant)){
                break;
            }

            tour_IA(plateau, coup, root); // Mouv du pion (contient le déplacement dans l'arbre)
            
        }else{// Tour du joueur
           
            tour(plateau, coup, J_act, BOBAIL); // Mouvement du Bobail 
            if (deplacement_arbre(root, coup) != EXIT_SUCCESS){
                printf("Erreur : deplacement racine\n");
                return EXIT_FAIL;
            }

            if(fin(plateau, J_act, &gagnant)){
                break;
            }

            tour(plateau, coup, J_act, J_act); // Mouvement du pion
            if (deplacement_arbre(root, coup) != EXIT_SUCCESS){
                printf("Erreur : deplacement racine\n");
                return EXIT_FAIL;
            }

        }
    }while(!fin(plateau, J_act, &gagnant));

    /************************** Fin de partie *************************/
    
    if(IA == gagnant){
        printf("Vous avez perdu !\n"); 
    }else{
        printf("Vous avez gagné !\n"); 
    }


    ////////////////////////  LIBERATION MEMOIRE  ///////////////////////
    
    free(coup);
    destroy(plateau);

    return EXIT_SUCCESS;
}