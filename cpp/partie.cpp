#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "../header/joueur.h"
#include "../header/deck.h"
#include "../header/jcpu.h"
#include "../header/partieSfml.h"

using namespace std;

void gamble(table &t, int nbj, int manche,sf::RenderWindow &window)
{
    // Saisit le nombre de victoires pariées par le joueur i
    
    for(int i = 0; i < nbj; i++)
    {
        // Pour chaque joueur, affiche les possibilités de pari en fonction de la manche
        if(t[i].humain)
        {
            int nbParie = -1;
    
            while(nbParie < 0 || nbParie > manche)
            {
                afficheC(window,manche,t[i]);
                nbParie = gambleSfml(window,manche,i,t[i]);

            }
            t[i].parie = nbParie; // Une fois une valeur correcte obtenue, on l'insère dnas la variable parie du joueur i puis on passe au suivant
            cout<<nbParie;
        }
        else
        {
            gambleCPU(t, nbj, manche, i);
        }    
    }  
}


void joue(deck &d, int &taille, table &t, int nbj, int manche,sf::RenderWindow &window) // Joue la manche
{
    int carteC = 0;
    int num = (manche - 1) % nbj; // C'est au joueur i de jouer -> à chaque manche le 1er joueur change
    int cptr;
    for (int i = 0; i < manche; i++) // Nombre de tours par manche
    {
        cout << "Tour " << i+1 << endl << endl;
        cptr=0; // Nombre de joueurs qui ont joué
        while(cptr<nbj) // Tant que tous les joueurs n'ont pas joué
        {
            int joueur_actif=(num+cptr) % nbj; // Joueur actif
            
            if(t[joueur_actif].humain)
            {

                cout<<carteC;
                SfmlCarteJ(window,manche-i,joueur_actif,carteC,t,nbj,cptr);
                carteC++;
    
                // ajouter l'option tigresse !!!!!! -> oui je sais=======
                if(carteC < 1 || carteC > manche-i) // Si la carte choisie n'est pas dans la main du joueur
                    cout << "Carte non valide, veuillez recommencer" << endl;
                else 
                {
                    carteC--; // pour l'indice
                    if(t[joueur_actif].mainJoueur[carteC].couleur == 7) // On laisse le choix au joueur si la carte est une Tigresse
                    {
                        bool tigF = tigresse(window);
                        t[joueur_actif].mainJoueur[carteC].tigressePirate = tigF;
                    }
    
                    
                    poseCarte(carteC, t, joueur_actif);


                poseCarte(carteC, t, joueur_actif);

                
                carte defausse = supp_cc(t[joueur_actif].mainJoueur, carteC, manche-i); // On supprime la carte choisie de la main du joueur et on va la remettre dans le deck à la fin
                if (defausse.couleur != -1) ajouteFin(d,taille,defausse);

                cptr++;
                
                }
            }
            else
            {


                carteC = joueCPU(d, joueur_actif, taille, t, manche, i, nbj);

                carte defausse = supp_cc(t[joueur_actif].mainJoueur, carteC, manche-i); // On supprime la carte choisie de la main du CPU et on va la remettre dans le deck à la fin
                if (defausse.couleur != -1) ajouteFin(d,taille,defausse);
        
                cptr++;
            }

        }/*
        cout <<"Tour "<< i+1 << " Fini" << endl;
        
        cout << "Joueur " << num+1 <<" remporte le tour " << i+1 << endl << endl;
        cout << "=====================================================================================================================================================" << endl;
        */
        num = verifGagnant(t,nbj,num); // Le gagnant a +1 plit

        window.clear();
        afficheTable(window,num,t,nbj,cptr); // Affiche la table de jeu
        afficheJG(window,num,i+1); // Affiche le gagnant du tour
        window.display();
        sf::sleep(sf::seconds(1)); //5sec pour voir les cartes
        
        
    
    }
}

void pioche(joueur &j, int manche, deck &d, int &taille, int num) // num est le Xe-1 joueur a piocher

{
    j.mainJoueur = new carte[manche]; 
   
    for (int i = 0; i < manche; i++)
    {
        j.mainJoueur[i] = d[0]; 
        afficheCarte(j.mainJoueur[i]);
        //afficheC(window,manche);
        supp_d0(d,taille); // On supprime la carte piochée du deck
    }
}

void mancheJ(table &t, int manche, int nbj, deck &d, int &taille,sf::RenderWindow &window)
{
    int i = 0; // Compteur pour que le distributeur change à chaque manche 
    melangeDeck(d, taille); // On mélange le deck avant de commencer la manche

    cout << "=== Manche " << manche << " ===" << endl << endl;
    while (i < nbj)
    {
        int num = (i + manche - 1) % nbj; // C'est au joueur i de piocher -> à chaque manche le 1er piocheur change

        pioche(t[num], manche, d, taille, num);

        cout << endl;
        i++;
    }
    gamble(t, nbj, manche,window); // On demande à chaque joueur combien de points il parie pour la manche
    for (int j = 0; j < nbj; j++)
    {
        cout << "Joueur " << j+1 << " a parie " << t[j].parie << endl;
    }

    joue(d, taille, t, nbj, manche, window); // joue la manche tour par tour

    attribution_point(t,nbj,manche); // donne les points
    
    window.clear();
    affichePoint(window,nbj,t);
    window.display();
    sf::sleep(sf::seconds(1));
    
    
    cout <<endl;
    delplit(t,nbj);
}

int gagnantFin(table t, int nbj) // renvoi l'indice du gagnant
{
    int max = t[0].point; //point du 1er joueur defini comme max
    int ind = 0; // l'indice
    for (int i=1;i<nbj;i++)
    {
        if (t[i].point>max) 
        {
            max = t[i].point;
            ind = i;
        }

    }
    return ind;
}