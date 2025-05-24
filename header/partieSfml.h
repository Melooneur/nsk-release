#pragma once
#include <SFML/Graphics.hpp>
#include "deck.h"
#include "joueur.h"

using namespace sf;

void intro(RenderWindow& window);
void title(RenderWindow& window);
void SelectMenu(RenderWindow& window,int& buttonSel,int& nbj,int& nbot,bool line);
void execute(RenderWindow& window,int& buttonSel,bool &line,bool &jouer);
void menu(RenderWindow& window,int &nbj,int& nbot,bool &line, bool &jouer, bool &fin);
void afficheC(RenderWindow &window,int nbCarte,joueur j);
int gambleSfml(RenderWindow &window,int manche,int numJ,joueur j);
void SfmlCarteJ(RenderWindow &window,int manche,int numJ,int &ajouer,table t,int nbj,int cptr);
void afficheTable(RenderWindow &window,int numJ,table t,int nbj,int cptr);
void afficheJG(RenderWindow &window,int numJ,int manche);
void affichePoint(RenderWindow &window,int nbj,table t);
int maxP(table t,int nbj);
bool tigresse(RenderWindow &window);
bool menuFinOffline(RenderWindow &window,int nbj,table t);
void afficheFond(RenderWindow &window);