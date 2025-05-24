#pragma once
#include <SFML/Graphics.hpp>

struct pseudoJoueur 
{
    int id;
    
    int point;
    int parie;
    int plit;
    int pointgagne;
    carte carteActive;
    
    int tailleMain;
    deck mainJoueur = nullptr;
};

using pseudoTable = pseudoJoueur *;

void connectToServer(sf::RenderWindow& window);