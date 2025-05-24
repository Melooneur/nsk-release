#include <iostream>
#include "../header/joueur.h"
#include "../header/deck.h"
#include "../header/partie.h"
#include "../header/jcpu.h"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "../header/partieSfml.h"
#include "../header/network.h"
#include "../header/jeu.h"

using namespace std;

void jeu(bool &line,bool &fin) 
{   
    int nbj = 0; //nbr de joueurs humains
    int nbo = 0; //nbr de joueurs ordis
    int manche = 1; //n° de manche 1-10
    int taille_d=70; // taille du deck

    bool jouer=false; //demare la partie si true

    srand(time(NULL));


//graphic parts

    sf::RenderWindow window;
    window.create(sf::VideoMode({LARGEUR_FENETRE, HAUTEUR_FENETRE}),"Neo skull king");
    
    nbj=2;
    //nbo=4;
    //skip pour gagner du tps




    intro(window);
    window.clear();
    menu(window,nbj,nbo,line,jouer,fin);//sort quand "entrer" sur jouer ou quitter 
    
    
    intro(window); //chargement ^^
    //debut du jeu
    
    
    if (!line)
    {    
        bool rejoue = true;

        while(rejoue)
        {
            deck d;
            d = new carte[70];
            remplirDeck(d);


            table t;
            t = new joueur[nbj+nbo];
            init_j(t,nbj);
            init_cpu(t,nbo,nbj);

            nbj = nbj + nbo; // Une fois les initialisations séparées réalisées, on regroupe le nombre de joueurs en une variable

            
            while (manche<=10 && window.isOpen())
            {
                mancheJ(t,manche,nbj,d,taille_d,window); // Debut manche
                manche++;
                if (manche<10)
                {
                    cout << "manche suivante" <<endl<<"====================================================================================================================================================="<<endl;
                }
            }

            window.clear();
            rejoue = menuFinOffline(window, nbj, t);
        }

    }
    else if(line)
    {
        connectToServer(window);

    }


}


int main()
{
    bool line=false; //offline par default
    bool fin=false; //fin de la partie
    while(!fin)
        jeu(line,fin);
    
    return 0;
}