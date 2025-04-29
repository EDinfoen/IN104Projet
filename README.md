#IN104Projet

  Le but du projet est d'implanter le jeu de Bobail en version joueur contre joueur puis une de coder une IA. Dans l'idéal nous aimerions créer une IA qui s'appuie sur l'algorithme de Monte-Carlo.

  Dans un 1er temps, l'implémentation de la version joueur contre joueur a été codée. Pour l'utiliser il faut se placer dans le dossier Projet_o en tapant dans le terminal : cd ./Projet_o. Ensuite, le Makefile doit être exécuté en entrant la commande : make all. Ainsi tous les exécutables .o correspondant aux fichiers annexe, jouer et affichage sont créés ainsi que les exécutables projet.o et projet.x. Enfin pour commencer à jouer, l'instruction ./projet.x doit être saisie dans le terminal. Le jeu démarre et le plateau est affiché.
  Il faut ensuite suivre les instructions qui s'affichent afin de déplacer les pions. Le jeu s'arrête automatiquement quand l'un des joueurs a gagné.

  Dans un 2nd temps, le code pour l'IA a été commencé. Il se situe dans le fichier IA.c qui pour le moment ne contient qu'une fonction renvoyant un coup aléatoire possible pour un joueur donné. Le main a permis de vérifier la validité de la fonction à partir du plateau initial. L'éxécutable est créé en entrant la commandant : make IA.x dans le terminal en étant placé danns le dossier Projet_o.
