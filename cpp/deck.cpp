#include <iostream>
#include <cstdlib>
#include <ctime>
#include "../header/deck.h"
using namespace std;

carte creerCarte(int valeur, int couleur)
{
    carte c;
    c.valeur = valeur;
    c.couleur = couleur;
    c.tigressePirate = false;
    return c;
}

void remplirDeck(deck & d)
{
    /*
    Un deck comporte 70 cartes
   
    5 - Fuites
    
    14 - Jaune, Vert, Bleu, Noir
    
    2 - Sirènes
    5 - Pirates
    1 - La Tigresse
    1 - Skull king
    */
    
    int indice = 0;
    
    // Fuite
    for(int i = 0; i < 5; i++)
    {
        d[indice] = creerCarte(0,0);
        indice++;
    }
    
    // Jaune
    for(int i = 0; i < 14; i++)
    {
        d[indice] = creerCarte(i+1,1);
        indice++;
    }

    // Vert
    for(int i = 0; i < 14; i++)
    {
        d[indice] = creerCarte(i+1,2);
        indice++;
    }
    
    // Bleu
    for(int i = 0; i < 14; i++)
    {
        d[indice] = creerCarte(i+1,3);
        indice++;
    }

    // Noir
    for(int i = 0; i < 14; i++)
    {
        d[indice] = creerCarte(i+1,4);
        indice++;
    }

    // Sirènes
    for(int i = 0; i < 2; i++)
    {
        d[indice] = creerCarte(0,5);
        indice++;
    }

    // Pirates
    for(int i = 0; i < 5; i++)
    {
        d[indice] = creerCarte(0,6);            // a changer en 0,6 ============================================================
        indice++;
    }

    // Tigresse
    d[indice] = creerCarte(0,7);
    indice++;
    
    // Skull King
    d[indice] = creerCarte(0,8);
    indice++;
}

void melangeDeck(deck & d, int taille)
{
    // Mélange le deck aléatoirement
    
    for(int i = taille-1; i >= 0; i--)
    {
        int j = rand()% (i+1);
        carte temp = d[i];
        d[i] = d[j];
        d[j] = temp;
    }
}

void supp_d0(deck &d, int &taille) // supprimer indice 0
{
    deck temp = new carte[taille - 1];
    for (int i = 1; i < taille; i++)
        temp[i - 1] = d[i];

    delete[] d;
    taille--;

    d = temp; // pour que le d change    
}

void afficheDeck(deck d, int taille)
{
    // Affiche le deck
    cout << "Deck : " << endl;
    for(int i = 0; i < taille; i++)
    {
        afficheCarte(d[i]);
    }
}

void ajouteFin(deck &d, int &taille, carte nouvelleCarte)
{
    // Ajoute une carte en fin de deck
    deck temp = new carte[taille + 1];
    for (int i = 0; i < taille; i++)
        temp[i] = d[i];
    temp[taille] = nouvelleCarte;
    delete[] d;
    taille++;
    d = temp;
}

void clearDeck(deck & d)
{
    delete[] d;
    d = nullptr;
}

void afficheCarte(carte c)
{
    // Affiche la carte
    cout << carteToString(c);
}

string carteToString(carte c)
{
    // Crée un string avec la valeur de la carte
    string n = "Carte ";
    switch (c.couleur)
    {
        case 0: return n + "fuite \n";
        case 1: return n + "jaune " + to_string(c.valeur) + "\n";
        case 2: return n + "verte " + to_string(c.valeur) + "\n";
        case 3: return n + "bleue " + to_string(c.valeur) + "\n";
        case 4: return n + "noire " + to_string(c.valeur) + "\n";
        case 5: return n + "sirene \n";
        case 6: return n + "pirate \n";
        case 7: return n + "Tigresse \n";
        case 8: return n + "Skull King \n";
        default: return n + "inconnue \n";
    }
}
