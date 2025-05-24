#pragma once
#include <SFML/Graphics.hpp>
#include "deck.h"
#include "network.h"

using namespace sf;

int gambleServ(RenderWindow &window,int manche,deck&  d);
int joueCarte(RenderWindow &window,int manche,deck d,pseudoTable t,int carteTable,int nbj);
void afficheTable(RenderWindow &window, int nbcarte, pseudoTable t,int nbj);
void affichePointServ(RenderWindow &window,int nbj,pseudoTable t);
bool menuFin(RenderWindow &window,int nbj,pseudoTable t);
void affCarte(RenderWindow &window,int nbCarte,int numC,deck d);