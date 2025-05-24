#include <iostream>
#include <cmath>
#include "../header/joueur.h"
#include "../header/deck.h"
#include "../header/partie.h"
using namespace std;

void init_cpu(table & t, int nb, int nbj)
{
    // Initialise les cpu après les nbj joueurs
    for(int i = nbj; i < nb+nbj; i++)
    {
        t[i].point = 0;
        t[i].plit = 0;
        t[i].parie = 0;
        t[i].humain = false;
        t[i].comportementCPU = (rand()) / (static_cast<float>(RAND_MAX/2)); // Donne un comportement aléatoire compris entre 0.0 et 2.0
    }
}

float puissanceTable(table t, int nbj)
{
    // Calcule la puissance des cartes présente sur la table
    // -> la puissance est une moyenne (pondérée par la valeur s'il y a) des cartes déjà jouées

    float puiss = 0;
    int c = 0;

    for(int i = 0; i < nbj; i++)
    {
        if(t[i].carteActive.valeur != -1) // Ne prend que les cartes des joueurs ayant déjà joué
        {
            if(t[i].carteActive.couleur > 0 && t[i].carteActive.couleur < 4) // Si la carte est d'une couleur "commune"
            {
                puiss += 1 * (1 + (t[i].carteActive.valeur / 100.0));
            }
            else if(t[i].carteActive.couleur > 3 && t[i].carteActive.couleur < 5) // Si la carte est noire
            {
                puiss += 4 * (1 + (t[i].carteActive.valeur / 100.0));
            }
            else // Si la carte est spéciale (ie fuite, pirate, etc)
            {
                puiss += t[i].carteActive.couleur;
            }
            c++; // On incrémente le nombre de cartes ajoutées à notre moyenne
        }
    }

    if(c != 0) return (puiss / c); // Renvoie la puissance calculée précedemment
    return 0;
}

float puissanceDeck(deck mainJoueur, int nbc)
{
    // Calcule la puissance des cartes présente dans le deck
    // -> la puissance est une moyenne (pondérée par la valeur s'il y a) des cartes possédées

    float puiss = 0;
    int c = 0;

    for(int i = 0; i < nbc; i++)
    {
        if(mainJoueur[i].couleur > 0 && mainJoueur[i].couleur < 4) // Si la carte est d'une couleur "commune"
        {
            puiss += 1 * (1 + (mainJoueur[i].valeur / 100.0));
        }
        else if(mainJoueur[i].couleur > 3 && mainJoueur[i].couleur < 5) // Si la carte est noire
        {
            puiss += 4 * (1 + (mainJoueur[i].valeur / 100.0));
        }
        else // Si la carte est spéciale (ie fuite, pirate, etc)
        {
            puiss += mainJoueur[i].couleur;
        }
        c++; // On incrémente le nombre de cartes ajoutées à notre moyenne
        
    }

    if(c != 0) return (puiss / c); // Renvoie la puissance calculée précedemment
    return 0;
}


void gambleCPU(table &t, int nbj, int manche, int i)
{
    // Fait parier les CPU
    
    /*
    COMPORTEMENTS :
    < 1 - Prudent
    = 1 - Equilibré
    > 1 - Agressif
    */
    float comportement = t[i].comportementCPU;
    float puissance = puissanceDeck(t[i].mainJoueur, manche);
    
    
    puissance = clamp(puissance / 8.0, 0.0, 1.0); // On ramène la puissance dans une fourchette comprise entre 0 et 1
    int pari = round(puissance * manche * comportement); // On arrondit le nombre en int et on calcule à peu près le nombre de victoires espérées en prenant en compte l'agressivité du CPU

    t[i].parie = clamp(pari, 0, manche);
}

carte meilleureCarteJoue(table t, int nbj, int manche)
{
    // Récupère la meilleure carte actuellement jouée sur la table

    carte meilleureDuDeck = t[0].carteActive;
    //int indiceMeilleure = 0;

    for(int k = 1; k < nbj; k++)
    {
        if(t[k].carteActive.valeur != -1) // Ne prend que les cartes des joueurs ayant déjà joué
        {
            if(estMeilleureCarte(t[k].carteActive, meilleureDuDeck, -1)) 
            {
                meilleureDuDeck = t[k].carteActive;
                //indiceMeilleure = k;
            }
        }
    }

    return meilleureDuDeck;
}

int joueMoinsBonne(table t, int jActif, int manche, int i)
{
    carte moinsBonne = t[jActif].mainJoueur[0];
    int indiceM = 0;
    cout << "Je rentre dans joueMoinsBonne";
    for(int k = 1; k < manche-i; k++) // On récupère la moins bonne carte du deck de l'IA
    {
        if(!estMeilleureCarte(t[jActif].mainJoueur[k], moinsBonne, -1)) 
        {
            moinsBonne = t[jActif].mainJoueur[k];
            indiceM = k;
        }
    }

    cout << "Je sors de joueMoinsBonne";
    poseCarte(indiceM, t, jActif);
    return indiceM;
}

int joueCPU(deck &d, int joueurActif, int &taille, table &t, int manche, int i, int nbj)
{
    // Fait jouer les CPU en fonction de ce qu'ils doivent gagner et des cartes possédées

    if(manche-i == 1) // Il ne reste plus qu'une carte, dans ce cas l'IA la joue sans réfléchir
    {
        poseCarte(0, t, joueurActif);
        return 0;
    }

    joueur jActif = t[joueurActif];
    float comportement = t[joueurActif].comportementCPU;
    /*
    COMPORTEMENTS :
    < 1 - Prudent
    = 1 - Equilibré
    > 1 - Agressif
    */
    float prob = ((jActif.parie-jActif.plit) / float(manche - i + 1)); // Calcule la probabilité que l'IA doive jouer ce tour-ci
    cout << "prob :" << prob;
    float puissT = puissanceTable(t,nbj);
    cout << "puissT :" << puissT;
    float puissD = puissanceDeck(jActif.mainJoueur, manche-i);
    cout << "puissD :" << puissD;

    if(puissD > puissT)
    {
        // La table n'est pas trop forte par rapport au deck du CPU
        cout << "je rentre dans la boucle";
        if(prob > rand()%2) // Joue seulement si prob est > 1 ou sinon de manière aléatoire
        {
            carte meilleureDuDeck = jActif.mainJoueur[0];
            int indiceMeilleure = 0;
            cout << "indiceMeilleure :" << indiceMeilleure;

            for(int k = 1; k < manche-i; k++) // On récupère la meilleure carte du deck de l'IA
            {
                if(estMeilleureCarte(jActif.mainJoueur[k], meilleureDuDeck, -1)) 
                {
                    meilleureDuDeck = jActif.mainJoueur[k];
                    indiceMeilleure = k;
                    cout << "indiceMeilleure :" << indiceMeilleure;
                }
            }

            carte meilleureTable = meilleureCarteJoue(t,nbj,manche);

            if(estMeilleureCarte(meilleureDuDeck, meilleureTable, -1)) // Si jamais le joueur possède la meilleure carte
            {
                poseCarte(indiceMeilleure, t, joueurActif);
                return indiceMeilleure;
            }
            else // Sinon, joue une carte poubelle
            {
                return joueMoinsBonne(t, joueurActif, manche, i);
            }
        }
        else
        {
            int carteC = rand() % (manche - i);
            while(carteC < 1 || carteC > manche-i) carteC = (rand()% manche - i) + 1;
            carteC--;
            poseCarte(carteC, t, joueurActif);
            return carteC;
        }

    }
    else
    {
        return joueMoinsBonne(t, joueurActif, manche, i);
    }
}