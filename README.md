
# Jeu du Bobail

## Aperçu 
Le but du projet est d'implémenter le jeu de Bobail en version joueur contre joueur puis contre une IA reposant sur la recherche Monte Carlo. 

![game_box](https://github.com/user-attachments/assets/f35618ce-83c2-4034-bbb5-5d75bd64ae13)

## Règles

- Le Bobail se joue sur un plateau de 
5x5 cases. Chaque joueur dispose de 5 pions. Un dernier pion est 
neutre, le BOBAIL, et est installé au centre du plateau.

-  Pour gagner il faut amener le BOBAIL dans une position particulière:
     1.  Soit le bobail est positionné sur 
une des 5 cases de départ des 
pions.
     2. Soit en bloquant le BOBAIL (de 
sorte que l’adversaire ne puisse 
pas le bouger).

 - Les règles de déplacement des pions sont les suivantes:
     1. Le BOBAIL ne se déplace que 
d'une seule case, dans 
n'importe quelle direction (y 
compris les diagonales)

    2. Les pions se déplacent 
également dans toutes les 
directions, mais jusqu’au bout 
de la rangée, ou jusqu’à un 
obstacle.

- Les joueurs jouent à tour de rôle. À son tour, le joueur déplace obligatoirement deux 
pions, d’abord le BOBAIL, puis un de ses 5 pions, suivant le principe 
de déplacement du jeu.
Exception : celui qui entame la partie n’avance que l’un de ses pions 
(le mouvement du BOBAIL est sauté).

## Prérequis
C

## Utilisation
Saisissez les commandes suivantes:

- cd Src
- make
- ./projet.x 


## Fonctionnalité 
- Affichage dans le terminal
  
  ![Capture d'écran 2025-05-28 215457](https://github.com/user-attachments/assets/e711b9ba-2958-4565-8724-4ba21834b599)
  
- Mode joueur contre joueur : entrer 1  lorsque la demande apparaît au départ dans le terminal.
- Mode solo contre une IA : entrer 0 lorsque la demande apparaît au départ dans le terminal. L'IA codée se base sur le principe du Monte-Carlo, en effectuant un nbr NBR_DESCENTES, hyperparamètre valant 50 000  par défaut et correspondant au nombre d'explorations. Un noeud de l'arbre représente un état du jeu possible. Les fils d'un noeud sont tous les coups que le joueur suivant peut effectuer à partir du plateau du  noeud actuel. Lors de l'exploration, c'est celui qui aura le meilleur MCTS qui sera exploré en priorité, à défaut le 1er des noeuds de la liste sera sélectionné. Pour estimer le nombre de victoires d'une feuille, une fin de partie aléatoire à partir du plateau de cette feuille est réalisée, à moins qu'elle ne représente déjà un plateau de victoire ou de défaite assurée. Après la phase de rétropropagation, l'IA choisira le noeud ayant le plus grand ratio victoire sur nombre d'explorations comme coup à jouer.
- Pour donner un coup pour :
     -  le BOBAIL il suffit d'entrer la case sur laquelle le joueur souhaite le positionner sous la forme 'A0',
     -  un PION, il faut entrer la case de départ du pion puis sa case d'arrivée séparée d'un espace, soit 'A0 B1'.
  Dans tous les cas si le coup n'est pas valide (dépassement du plateau ou mauvais choix de pion), il sera automatiquement demandé au joueur d'entrer un nouveau coup pour la même pièce.






