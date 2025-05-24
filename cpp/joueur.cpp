#include <iostream>
#include "../header/joueur.h"
using namespace std;

void init_j(table t, int nb)
{
    // Initialise une table de nb joueurs
    for (int i=0;i<nb;++i)
    {
        t[i].point = 0;
        t[i].plit = 0;
        t[i].parie = 0;
        t[i].humain = true;
        t[i].comportementCPU = -1;
        t[i].carteActive = creerCarte(-1,-1);
    }

}

void clear_j(table & t, int nb)
{
    // Nettoie la main des joueurs présent autour de la table avant de la supprimer
    for(int i = 0; i < nb; i++)
    {
        delete[] t[i].mainJoueur;
        t = nullptr;
    }
    
    delete[] t;
    t = nullptr;
}

int diff(int a, int b)              
{
    // Calcule la valeur absolue de la difference
    int c=a-b;
    if (c<0)
        c=-c;
    return c;

}


bool estMeilleureCarte(carte a, carte b, int couleurPremier) 
{
    // True : a meilleure que b
    // False : b meilleure que a

    if (a.couleur == 0) return false; // Fuite perd toujours
    else if (b.couleur == 0) return true;

    // Skull King (8) > tout sauf sirène (5)
    else if (a.couleur == 8 && b.couleur != 5) return true;
    else if (b.couleur == 8 && a.couleur != 5) return false;

    // Pirate (6) > tout sauf Skull King
    else if (a.couleur == 6 && b.couleur < 6) return true;
    else if (b.couleur == 6 && a.couleur < 6) return false;


    // Sirène (5) > Skull King et les autres sauf pirate

    else if (a.couleur == 5 && b.couleur == 8) return true;
    else if (a.couleur == 5 && b.couleur < 5) return true;
    else if (b.couleur == 5 && a.couleur == 8) return false;
    else if (b.couleur < 5 && a.couleur == 5) return false;

    // Noir (4) > couleurs simples (1-3)
    else if (a.couleur==b.couleur && a.couleur == 4) return a.valeur > b.valeur;
    else if (a.couleur == 4 && b.couleur < 4) return true;
    else if (b.couleur == 4 && a.couleur < 4) return false;

    // Si mêmes couleurs simples (1-3), on compare la valeur en prenant en compte le fait qu'une carte d'une autre couleur que la première à une valeur inférieure par défaut
    else if (a.couleur < 4 && b.couleur < 4 && a.couleur == b.couleur)
        return a.valeur > b.valeur;
    else if (a.couleur < 4 && b.couleur < 4 && a.couleur != b.couleur && a.couleur != couleurPremier)
        return false;


    // Sinon, b garde l'avantage (dominante supposée)
    return false;
}

void poseCarte(int carteC, table& t, int joueur_actif)
{
    carte carteJouee = t[joueur_actif].mainJoueur[carteC]; // On met la carte choisie dans la carte active du joueur
    cout << "Joueur " << joueur_actif+1 << " a joue la carte : ";
    afficheCarte(carteJouee); // On affiche la carte jouée
    
    if(carteJouee.couleur == 7) // Pour la Tigresse, on remplace la carte par une carte fuite ou pirate en interne
    {
        if(carteJouee.tigressePirate)
        {
            carteJouee = creerCarte(0,6);
        }
        else
        {
            carteJouee = creerCarte(0,0);
        }
    }
    t[joueur_actif].carteActive = carteJouee;   
    afficheCarte(t[joueur_actif].carteActive); // On affiche la carte active du joueur
    cout << endl;
}

// Détermine le gagnant d'une manche
int verifGagnant(table & t, int nbj, int indicePremier) 
{
    int indiceGagnant = indicePremier; // Le gagnant est par défaut le premier
    int couleurPremier = t[indiceGagnant].carteActive.couleur; // On récupère la couleur de la première carte posée -> utile pour déterminer quelle couleur domine quand il n'y a pas de carte spéciales
    carte meilleureCarte = t[indiceGagnant].carteActive; // On récupère la meilleure carte, qui est pour l'instant celle du premier

    for (int offset = 1; offset < nbj; ++offset) 
    {
        int i = (indicePremier + offset) % nbj;
        carte c = t[i].carteActive;

        if (estMeilleureCarte(c, meilleureCarte, couleurPremier)) 
        {
            meilleureCarte = c;
            indiceGagnant = i;
        }
    }
    t[indiceGagnant].plit += 1;
    return indiceGagnant;
}

void attribution_point(table t,int nbj,int manche)         
{
    // Attribue les points aux joueurs à la fin de la manche
    for (int i=0;i<nbj;++i)
    {
        t[i].pointgagne = 0; // On remet à 0 les points gagnés
        if (t[i].parie==0) //si parie 0
        {
            if (t[i].plit==0)
            {
                t[i].point+=10*manche; //gagne
                t[i].pointgagne=10*manche;
                cout<< "Joueur " << i+1 << " a gagne "<<10*manche<<" points" << endl;
            }
            else
            {              
                t[i].point-=10*manche; //perdu
                t[i].pointgagne=-10*manche;
                cout<< "Joueur " << i+1 << " a perdu "<<10*manche<<" points" << endl;
            }
        }
        else //sinon
        {
            if (t[i].plit==t[i].parie)
            { 
                t[i].point+=t[i].parie*20;                  //gagne
                t[i].pointgagne=10*t[i].parie;
                cout<< "Joueur " << i+1 << " a gagne " << t[i].parie*20 << " points" << endl;
            }
            else
            {
                t[i].point-=diff(t[i].parie,t[i].plit)*20;  //perdu
                t[i].pointgagne=-diff(t[i].parie,t[i].plit)*20;
                cout<< "Joueur " << i+1 << " a perdu " << diff(t[i].parie,t[i].plit)*20 << " points" << endl;
            }
        }

    }

}
/*
carte supp_cc(deck &mainJ, int ind, int sous_manche) // Supprime la carte choisie de la main du joueur
{
    if (sous_manche != 0)
    {
        deck temp = new carte[sous_manche - 1];
        carte defausse = mainJ[ind];
        
        for (int i = 0; i < ind; i++)
            temp[i] = mainJ[i];

        for (int i = ind + 1; i < sous_manche; i++)
            temp[i - 1] = mainJ[i];
        delete[] mainJ;
        mainJ = new carte[sous_manche - 1];
        mainJ = temp; // Pour que le deck du joueur change
        return defausse;
    }
    return creerCarte(-1,-1);
}
*/

carte supp_cc(deck &mainJ, int ind, int sous_manche) // Supprime la carte choisie de la main du joueur
{
    if (sous_manche > 0 && ind >= 0 && ind < sous_manche)
    {
        carte defausse = mainJ[ind];
        deck temp = new carte[sous_manche - 1];

        for (int i = 0, j = 0; i < sous_manche; ++i)
        {
            if (i != ind)
            {
                temp[j++] = mainJ[i];
            }
        }

        delete[] mainJ;
        mainJ = temp;
        return defausse;
    }
    return creerCarte(-1, -1);
}

void delplit(table &t,int nbj) 
{
    // Nettoie la table de toutes ses variables
    for (int i = 0; i < nbj; i++)
    {
        t[i].plit=0;
        t[i].parie=0;
        delete[] t[i].mainJoueur;
        t[i].mainJoueur = nullptr;
    }
}