#pragma once
#include "joueur.h"
#include <SFML/Graphics.hpp>
void gamble(table &t, int nbj, int manche,sf::RenderWindow &window);
void poseCarte(int carteC, table& t, int joueur_actif);
void mancheJ(table &t, int manche, int nbj, deck &d, int &taille,sf::RenderWindow &window);

int verifGagnant(table & t, int nbj, int indicePremier); 
int gagnantFin(table t, int nbj);
void delplit(table &t,int nbj);
void supp_d0(deck &d, int &taille);
void ajouteFin(deck &d, int &taille, carte nouvelleCarte);