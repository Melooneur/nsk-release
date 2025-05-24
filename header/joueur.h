#pragma once
#include <asio.hpp>
#include "deck.h"
using asio::ip::tcp;


struct joueur
{
    int point;
    int pointgagne;

    bool humain; // Le joueur est-il humain ou non
    float comportementCPU; // -1 par défaut, sert à choisir le comportement de l'IA
    /*
    COMPORTEMENTS :
    < 1 - Prudent
    = 1 - Equilibré
    > 1 - Agressif
    */
   
    int parie; // Le gamble au debut de chaque manche
    int plit; // NB de tour gagnée dans la manche x

    carte carteActive; // Carte jouée pour la manche actuelle
    deck mainJoueur; // Main du joueur contenant toutes ses cartes

    // Réseau :

    std::shared_ptr<tcp::socket> socket = nullptr;  
    int id;                            
    bool isReady = false;

};

using table = joueur *;

void init_j(table t, int nb);
int diff(int a, int b);
void attribution_point(table t,int nbj,int manche);
void affiche_j(table t,int nbj);
void affiche_carteJ(joueur j, int manche);
carte supp_cc(deck &mainJ,int ind,int sous_manche);
void clear_j(table & t, int nb);
void delplit(table &t,int nbj);
int verifGagnant(table & t, int nbj, int indicePremier);
void poseCarte(int carteC, table& t, int joueur_actif);
bool estMeilleureCarte(carte a, carte b, int couleurPremier);

