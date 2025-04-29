#include "annexe.h"
#include "affichage.h"


/*
Fichier qui contiendra le code pour l'IA.
Pour le moment, il n'y a qu'une fonction coup aléatoire qui donne un coup aléatoire pour un joueur donné.
Elle utilise les fonctions avancer et piece_mobile.
Le main a permis de tester coup_aleatoire pour le plateau initial.
*/
enum direction {N, NE, E, SE, S, SW, W, NW} ;

bool avancer( int xi, int yi, enum direction dir, int* xf, int* yf, int **plateau ){
    " Vérifie que le pion se trouvant en (xi,yi) 
    peut avancer d'une case dans la direction dir.
    Si oui, elle modifie les pointeurs xf et yf pour 
    qu'ils indiquent la case suivante."
    
    int x,y;
    switch (dir)
    {
    case N:
        x=xi; y=yi-1;
        break;
    case NE:
        x=xi+1; y=yi-1;
        break;
    case E : 
        x=xi+1; y=yi;
        break;    
    case SE:
        x=xi+1; y=yi+1;
        break;
    case S:
        x=xi; y=yi+1;
        break;
    case SW:
        x=xi-1;y=yi+1;
        break;
    case W:
        x=xi-1;y=yi;
        break;
    case NW:
        x=xi-1;y=yi-1;
        break;
    }

// Vérification que la nouvelle case est dans le plateau et inoccupée.

if( 0>x || x>(SIZE - 1) || y<0 || y>(SIZE - 1) ){return false;}
if (plateau[x][y]==VIDE ){ *xf=x;*yf=y;return true;}
return false;
}

int piece_mobile(int** plateau, int x, int y){
"Test si un le pion en (x,y) a au moins une direction dans
laquelle il peut avancer, c'est-à-dire qu'il y a une case vide
autour de lui."

//Parcours des 9 cases alentours du pion.
    for(int i=-1;i<2;i++){
        for(int j=-1;j<2;j++){
            if (0<x+i && x+i<(SIZE - 1) && y+j>0 &&y+j<(SIZE - 1)){
                if (plateau[x+i][y+j]==VIDE){return true;}
            }
        }
    }
return false;
}


coup_t* coup_aleatoire ( int** plateau, piece joueur){
"Renvoie un coup aléatoire possible pour joueur."
srand(time(NULL));
int xi,yi;
if (joueur==J1 || joueur==J2){
    //on récupère les positions de pions
    int pions[5];
    int nbr_pions=0;
    printf("position:\n");
    for (int i=0;i<5; i++){
        for( int j=0;j<5;j++){
            if (plateau[i][j]==joueur){
                pions[nbr_pions]=i*10+j;
                printf("%d,%d\n",i,j);
                nbr_pions++;
            }
            if(nbr_pions==4){break;} //tous les pions trouvés
        }
    }
    printf("trouve\n");
    
    //choix d'un pion pouvant bouge
    bool bloque=true;
    while(bloque){
    int k=rand()%(nbr_pions+1);
    printf("k=%d\n",k);
    xi=pions[k]/10;
    yi=pions[k]%10;
    printf("%d,%d",xi,yi);
        if(!piece_mobile(plateau, xi,yi)){
        pions[k]=pions[nbr_pions];
        nbr_pions--;
        printf("bloque\n");
        }else{bloque=false;}
        }
    printf("mobile");
}else{
    localisation_bobail(plateau,&xi,&yi);
    printf("bobail\n");
}

coup_t* coup=malloc(sizeof(coup_t));
coup->xi=xi;
coup->yi=yi;

//choix d'une direction
bool avance=false;
enum direction directions[8]={N, NE, E, SE, S, SW, W, NW};
int nbr_dir=7;
int xf,yf;
while(!avance){
    int k=rand()%(nbr_dir+1);
    printf("k2=%d\n",k);
    enum direction dir=directions[k];
    avance=avancer(xi,yi,dir,&xf,&yf,plateau);
    if(!avance){//direction bloquee il faut changer
        //on place la direction testée en bout de tableau pour ne plus la choisir
        directions[k]=directions[nbr_dir];
        nbr_dir--;
    }else if(joueur!=BOBAIL){//deplacement jusqu'au bout de la direction
        bool max=avancer(xf,yf,dir,&xf,&yf,plateau);
        while(max){//on peut peut-être encore avancer
            max=avancer(xf,yf,dir,&xf,&yf,plateau);
        }
    }
}
coup->xf=xf;
coup->yf=yf;
return coup;
}


////////////////////////////////////////////////////////////////////////
#define DEBUG

int tour(int** plateau,int J ){
    coup_t* coup=coup_aleatoire(plateau, J);
    if( coup== NULL || mouvement(plateau, coup) == EXIT_FAIL || affichage(plateau) == EXIT_FAIL){
        return EXIT_FAIL;
    }

    return EXIT_SUCCESS;
}


int main(){

    //////////////////////////////////////////////////////////////////////////////
    
    #ifndef DEBUG
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
    printf("deb\n");
    if(init(plateau) == EXIT_FAIL){
        return EXIT_FAIL;
    }
    #else
    int** plateau = NULL;
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


    #endif
    /////////////////////////////////////////////////////////
    if(affichage(plateau) == EXIT_FAIL){
        destroy(plateau); 
        return EXIT_FAIL;
    }



printf("Symboles :\nJoueur J1 : x\nJoueur J2 : +\nBobail : o\n");
int xf,yf;
printf("%d,",avancer( 0, 3, SE, &xf, &yf,plateau ));
printf("%d,%d\n",xf,yf);
printf("%d",piece_mobile(plateau,4,3));
coup_t* coup=coup_aleatoire(plateau, BOBAIL);

mouvement(plateau, coup);
 affichage(plateau);
 //   if(tour(plateau, J1) == EXIT_FAIL){
   //     return EXIT_FAIL;
    //}

    destroy(plateau);

    return EXIT_SUCCESS;
}