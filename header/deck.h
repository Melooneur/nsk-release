#pragma once
using namespace std;

struct carte
{
    // Les valeurs vont de 1 à 14
    int valeur;
    /*
    Les couleurs, dans l'ordre d'importance
    Fuite -> 0
    
    Jaune -> 1
    Vert -> 2
    Bleu -> 3

    Noir -> 4

    Sirène -> 5
    Pirate -> 6
    Tigresse -> 7
    Skull King -> 8
    */
    int couleur;
    bool tigressePirate = false;
};

using deck = carte *;

carte creerCarte(int valeur, int couleur);
void remplirDeck(deck & d);
void melangeDeck(deck & d, int taille);
void afficheDeck(deck d, int taille);
void afficheCarte(carte c);
void clearDeck(deck & d);
string carteToString(carte c);
void supp_d0(deck &d, int &taille);
void ajouteFin(deck &d, int &taille, carte nouvelleCarte);
