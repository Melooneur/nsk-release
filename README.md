# Compilation

## Bibliothèques

Il faut les bibliothèques :
- `SFML` en version 2.6.5
- `Asio` en version 3.0.0

Les deux doivent être situées à la racine du disque et nommés respectivement **SFML-2.6** et **asio**.

## Windows

Le jeu est déjà compilé pour Windows en 64 bits, cependant des problèmes à l'exécution peuvent survenir si jamais les dll ne sont pas présentes, pour régler le problème
il suffit d'installer `minGW` et les dll seront installées avec.

## Linux

Pour compiler le jeu sous linux, il faudra utiliser le **CMakeListsLinux.txt** et de build le projet, après ça, deux exécutables seront compilés et le jeu sera jouable !

# Installation

## Exécution

Pour lancer en solo :
- Exécuter `NeoSkullKing.exe`
- Sélectionner `JOUER`
- Saisir le nombre de joueurs et de robots dans la partie
- Jouer !

Pour lancer en multijoueur :
Le multijoueur sur Neo Skull King se fait principalement en LAN (Local Access Network) soit sur le réseau local. Néanmoins, il est possible d'y jouer en ligne
en ouvrant le port '25565' de votre routeur, mais la manipulation ne sera pas détaillée ici.
Il faut tout d'abord s'assurer que tous les joueurs soient connectés au même réseau internet.
Ensuite le joueur qui souhaite héberger la partie doit exécuter `NeoSkullKing-Serveur.exe` et saisir le nombre de joueurs et d'ordis dans la partie.
Après cela, chaque joueur n'a plus qu'a ouvrir son client, changer l'option `Offline` en `Online` puis `JOUER` !

NB : L'IP par défaut est 127.0.0.1 soit l'hôte local, cependant, en cas de jeu en ligne, chaque client doit changer l'IP présente dans le fichier `config.txt`
et y mettre celle du joueur hôte.


Pour arreter de jouer en plein milieu de partie:
si vous n'avez pas l'invite de commande de NeoSkullKing,il suffit d'entrer cette commande :
`pkill -f NeoSkullKing`
