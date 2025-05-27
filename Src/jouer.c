#include "affichage.h"
#include "exploration.h"


int tour(int** plateau, coup_t* coup, piece_t J ){
    /*
    Exécute un tour : demande de coup, modification de la position du pion, affichage du nouvel état du plateau.
    */
    saisie_coup(plateau, coup, J);
    mouvement(plateau, coup);
    affichage(plateau);

    return EXIT_SUCCESS;
}

int tour_IA(int** plateau, coup_t* coup, noeud_t* root ){
    /*
    Exécute un tour : calcul le coup optimal, modification de la position du pion, affichage du nouvel état du plateau.
    */
        //printf("DEBUT COUP IA\n");
    saisie_coup_IA(root, coup);
    printf("coup saisi\n");
    printf("%d,%d,%d,%d\n",coup->xi,coup->yi,coup->xf,coup->yf);
    mouvement(plateau, coup);
    printf("mouv\n");
    affichage(plateau);
        //printf("FIN COUP IA\n");
    return EXIT_SUCCESS;
}

int jouer_pvp(){
    /*
    Implémente une partie de joueur contre joueur. La fonciton s'arrête quand l'un d'eux a gagné.
    */
    
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

    printf("Symboles :\nJoueur J1 : x\nJoueur J2 : +\nBobail : o\n");

    if(affichage(plateau) == EXIT_FAIL){
        destroy(plateau); 
        return EXIT_FAIL;
    }
    
    // Début de la partie

    coup_t* coup = malloc(sizeof(coup_t));
    if (coup == NULL){
        free(coup);
        destroy(plateau);
        printf("Erreur init : allocation (coup)");
        return EXIT_FAIL;

    }
    
    //Lors du 1er tour, pas de mvt du bobail.
    tour(plateau, coup, J1);
    
    piece_t J_act = J2;
    piece_t gagnant = VIDE; // Par défaut
    //bool fini = false;

    do{
        tour(plateau, coup, BOBAIL); // Mouvement du Bobail
           
        if(fin(plateau, J_act, &gagnant)){
            //gagnant = J_act;
            break;
        }

        tour(plateau, coup, J_act); // Mouvement du pion
    
        J_act = 1 - J_act;

    }while(!fin(plateau, J_act, &gagnant));

    // Fin de partie

    printf("Joueur %d a gagné !\n", gagnant + 1);
    
    // Mémoire libérée
    free(coup);
    destroy(plateau);

    return EXIT_SUCCESS;
}

int jouer_IA(){ 
    /*
    Implémente une partie de joueur contre IA. La fonciton s'arrête quand l'un d'eux a gagné.
    */
    
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
    //init_test(plateau);///////////////////////////////////////////

    printf("Symboles :\nJoueur J1 : x\nIA : +\nBobail : o\n");

    if(affichage(plateau) == EXIT_FAIL){
        destroy(plateau); 
        return EXIT_FAIL;
    }

    // Initialisation de l'IA

    noeud_t* root=malloc(sizeof(noeud_t));
    if (root==NULL){
        printf("Erreur init: allocation root\n");
        return EXIT_FAIL;
    }
    init_noeud(root);
    copier_plt(plateau, root->plateau);
    root->J = B1;
    //root->J = J1;
    piece_t IA = J2; // Joueur joué par l'IA
    
    // Début de la partie
    
    coup_t* coup = malloc(sizeof(coup_t));
    if (coup == NULL){
        free(coup);
        destroy(plateau);
        printf("Erreur init : allocation (coup)");
        return EXIT_FAIL;
    }
    
    //Lors du 1er tour, pas de mvt du bobail.
    
     ///////////////////////////////////////////
    tour(plateau, coup, J1);

    //A MODIFIER :INITIALISER
    //////////////////////////
    // Initialisation de l'IA

   /* noeud_t* root=malloc(sizeof(noeud_t)); 
    if (root==NULL){
        printf("Erreur init: allocation root\n");
        return EXIT_FAIL;
    }
    init_noeud(root);
    printf("init root");
    copier_plt(plateau, root->plateau);
    printf("copié");
    root->J = J1;
    piece_t IA = J2; // Joueur joué par l'IA

    print_noeud(root);*/
    if (deplacement_arbre(root, coup)!=EXIT_SUCCESS){
        printf("Erreur dep arbre");
        return EXIT_FAIL;
    }
    printf("déplacé");
    //print_noeud(root);

    
    
    piece_t J_act = IA;
    piece_t gagnant = VIDE; // Par défaut


    do{
        if( J_act == IA){// Tour de l'IA
  
            //print_noeud(root);
            tour_IA(plateau, coup, root); // Mouv du Bobail (contient le déplacement dans l'arbre)
            affichage(plateau);
            if(fin(plateau, J_act, &gagnant)){
                break;
            }

            tour_IA(plateau, coup, root); // Mouv du pion (contient le déplacement dans l'arbre)
            affichage(plateau);
            //printf("PIA\n");
            J_act = 1 - J_act;
        }else{// Tour du joueur
            ///////////////////////////////////////////////////////// Ajouter les mouv dans l'arbre

            tour(plateau, coup, BOBAIL); // Mouvement du Bobail 
            if (deplacement_arbre(root, coup)!=EXIT_SUCCESS){
                printf("Erreur dep arbre");
                return EXIT_FAIL;
            }
            printf("déplacé");
            if(fin(plateau, J_act, &gagnant)){
                break;
            }

            tour(plateau, coup, J_act); // Mouvement du pion
            if (deplacement_arbre(root, coup)!=EXIT_SUCCESS){
                printf("Erreur dep arbre");
                return EXIT_FAIL;
            }
            printf("déplacé");
            J_act = 1 - J_act;
        }
    }while(!fin(plateau, J_act, &gagnant));

    // Fin de partie

    printf("Joueur %d a gagné !\n", gagnant + 1);
    
    // Mémoire libérée
    free(coup);
    destroy(plateau);

    return EXIT_SUCCESS;
}