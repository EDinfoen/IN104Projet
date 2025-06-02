
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
  
- Mode joueur contre joueur
- Mode solo contre une IA






