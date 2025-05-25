#include <SFML/Graphics.hpp>
#include <SFML/System.hpp> //pour les poses
#include <iostream>
#include "../header/joueur.h"
#include "../header/deck.h"
#include "../header/partieSfml.h"
#include "../header/jeu.h"
#include "../header/network.h"

using namespace sf;
using namespace std;

void afficheM(RenderWindow &window,int manche)    //affiche "Manche ..."
{
    Font font,djv;
    font.loadFromFile("../assets/NeoNeon.ttf");
    djv.loadFromFile("../assets/DejaVuSerif.ttf");
    Text Manche("Manche ",font,50);
    Text num(std::to_string(manche),djv,50);
    num.setScale(1.1,1.1);

    Manche.setPosition(LARGEUR_FENETRE/3*4,10);
    num.setPosition(LARGEUR_FENETRE/3*4 + Manche.getLocalBounds().width,0);

    window.draw(Manche);
    window.draw(num);
}

// affichage des cartes =========================================================================================================================


//Interieur de la carte
void intCarte(RenderWindow &window,carte card,int x,int y,float taille) //taille=0.5 ou 1
{
    Font djv;
    djv.loadFromFile("../assets/DejaVuSerif.ttf");

    Texture img;

    int couleur = card.couleur, nbr = card.valeur;
    float scale=0.1;
    
    if(couleur>4 or couleur==0)//pas de chiffre
    {
        if(couleur==5) //sirene
        {
            img.loadFromFile("../assets/sirene.png");
            
        } 
        else if(couleur==6) //Pirate
        {
            img.loadFromFile("../assets/pirate.png");
            
        }
        else if(couleur==7) // tigresse
        {
            img.loadFromFile("../assets/tigresse.png");
            //scale=0.5;
        }
        else if(couleur==8) //Skull King
        {
            img.loadFromFile("../assets/skullking.png");
            
        }
        else if(couleur==0) //fuite
        {
            img.loadFromFile("../assets/fuite.png");
            
            
        }
        else {
            Text erreur("Erreur",djv,50);
            erreur.setFillColor(sf::Color(255,0,0));
            erreur.setPosition(x,y);
            window.draw(erreur);
        }

        Sprite imgS(img);
        imgS.setOrigin(imgS.getLocalBounds().width/2,imgS.getLocalBounds().height/2); //centre l'image
        imgS.setScale(scale,scale);
        imgS.setPosition(x,y);
        window.draw(imgS);

    }
    else //avec chiffre
    {
        Text chiffre(std::to_string(nbr),djv,50);

        if (couleur==4)//noir
        {
            chiffre.setFillColor(sf::Color(0,0,0));
        }
        else if(couleur==3)//bleu
        {
            chiffre.setFillColor(sf::Color(0,0,255));
        }
        else if(couleur==2)//vert
        {
            chiffre.setFillColor(sf::Color(0,255,0));
        }
        else if(couleur==1)//jaune
        {
            chiffre.setFillColor(sf::Color(255,255,0));
        }
        else {
            Text erreur("Erreur",djv,50);
            erreur.setFillColor(sf::Color(255,0,0));
            erreur.setPosition(x,y);
            window.draw(erreur);
        }

        chiffre.setOutlineThickness(2); // Épaisseur du contour
        chiffre.setOutlineColor(sf::Color::White); // Couleur du contour
        
        
        sf::FloatRect Tchiffre = chiffre.getLocalBounds(); // donne le rectangle qui englobe le chiffre
        chiffre.setOrigin(Tchiffre.width/2,Tchiffre.height/2); //centre le texte
        chiffre.setScale(taille/taille,taille/taille);
        chiffre.setPosition(x,y-(Tchiffre.height/2)*taille); //positionne le texte
        window.draw(chiffre);
    }

}




// affichage de cartes
void affCarte(RenderWindow &window,int nbCarte,int numC,deck d)    //selection de carte en plus comparer a l'autre
{   
    //numC -> carte selectionné



    Texture imgT;
    imgT.loadFromFile("../assets/carte.png");

    Sprite carte(imgT);

    float carteSize=0.5,cartePos ;
    if(nbCarte>5) carteSize=0.25;   //pour voir toutes les cartes

    carte.setScale(carteSize,carteSize);
    sf::Vector2u taille = carte.getTexture()->getSize();
    

    cartePos=(LARGEUR_FENETRE/2-(0.5*taille.x*nbCarte*carteSize));

    int posx,posy;

    for (int i=0;i<nbCarte;i++)
    {
        if(i==numC)  
        {
            // Positionner le contenu au milieu de la carte sélectionnée
            posx = cartePos + (taille.x * carteSize) / 2;
            posy = 450 + (taille.y * carteSize) / 4;
            carte.setPosition(cartePos,450);
            window.draw(carte);
            intCarte(window,d[i],posx,posy,carteSize*2);
        }
        else 
        {
            if (nbCarte>5)
            // Positionner le contenu au milieu de la carte non sélectionnée
            {
                posx = cartePos + (taille.x * carteSize) / 2;
                posy = 600 + (taille.y * carteSize) / 3;
                carte.setPosition(cartePos,600);
            }
            else 
            {
                posx = cartePos + (taille.x * carteSize) / 2;
                posy = 550 + (taille.y * carteSize) / 4;
                carte.setPosition(cartePos,550);
            }
            
            window.draw(carte);
            intCarte(window,d[i],posx,posy,carteSize*2);
        
        }

        cartePos+=taille.x*carteSize;

    }


 
}

//affiche la table de jeu
void afficheTable(RenderWindow &window, int nbcarte, pseudoTable t,int nbj) // Affiche les cartes déjà posées par les autres joueurs
{   

    if (nbj <= 0) return;

    Texture imgT;
    if (!imgT.loadFromFile("../assets/carte.png")) {
        cerr << "Erreur de chargement de l'image de carte." << endl;
        return;
    }
    Sprite carte(imgT);

    float carteSize = 0.25f;
    carte.setScale(carteSize, carteSize);
    sf::Vector2u taille = carte.getTexture()->getSize();

    // Calcul de la position de départ pour centrer les cartes
    float cartePos;
    if (nbcarte < 5)
        cartePos = (LARGEUR_FENETRE / 2.0f) - (taille.x * carteSize * (nbcarte ) / 2.0f);
    else
        cartePos = (LARGEUR_FENETRE / 2.0f) - (taille.x * carteSize * nbcarte / 2.0f);

    for (int i = 0; i < nbj; ++i) {
        // Vérifie que la carte est valide
        if (t[i].carteActive.couleur >= 0 && t[i].carteActive.couleur <= 8 && t[i].carteActive.valeur != -1) {
            int cartePosX = static_cast<int>(cartePos + (taille.x * carteSize) / 2);
            int cartePosY = 200 + (taille.y * carteSize) / 2;
            carte.setPosition(cartePos, 200);
            window.draw(carte);
            intCarte(window, t[i].carteActive, cartePosX,cartePosY, carteSize);
            cartePos += taille.x * carteSize;
        }

    }
}


/* Parie ========================================================================================================== */

void afficheGamble(RenderWindow &window,int nbParie,int manche,deck d)    //affiche "Gamble: x"
{
    window.clear();

    afficheFond(window); //affiche le fond d'écran
    afficheM(window,manche);          //affiche "Manche ../assets."
    affCarte(window,manche,-1,d);    //affiche les cartes du joueur


    Font djv,font;
    font.loadFromFile("../assets/NeoNeon.ttf");
    djv.loadFromFile("../assets/DejaVuSerif.ttf");

    Text gambling("Pari",font,50);
    sf::FloatRect bounds = gambling.getLocalBounds();
    gambling.setPosition((LARGEUR_FENETRE-bounds.width)/2,HAUTEUR_FENETRE/4);



    Text gamble(std::to_string(nbParie),djv,50);
    bounds = gamble.getLocalBounds();
    gamble.setPosition((LARGEUR_FENETRE-bounds.width)/2,HAUTEUR_FENETRE/3);


    window.draw(gambling);
    window.draw(gamble);
    window.display();

}



int gambleServ(RenderWindow &window,int manche,deck& d)
{    
    int nbParie=0;   
    bool jouer=false;
    window.clear();

    sf::Event event;

    while (window.pollEvent(event)) {}

    while(window.isOpen() && !jouer)
    {
        while(window.pollEvent(event) && !jouer)
        {
            if (event.type == Event::KeyReleased)
            {
                if (event.key.code==Keyboard::Enter)
                {
                    jouer=true;

                }

                else if (event.key.code==Keyboard::Right)
                {
                    if (nbParie<manche)nbParie++;
                    else nbParie=0;

                }
                else if (event.key.code==Keyboard::Left)
                {
                    if (nbParie>0)nbParie--;
                    else nbParie=manche;

                } 
                else if (event.type == sf::Event::Closed)
                    window.close();
                //std::cout<<"je suis dedans"; 
                
            }
        }
        sf::sleep(sf::milliseconds(10));
        
        afficheGamble(window,nbParie,manche,d);       
    }

    while (window.pollEvent(event)) {}
    //vide juste la file
    return nbParie; //retourne le gamble
}



// jouer la carte =========================================================================================================================

int joueCarte(RenderWindow &window,int manche,deck d,pseudoTable t,int carteTable,int nbj)
{

    int ajouer=0; //la carte selectionné

    bool jouer=false;

    sf::Event event;
    while (window.pollEvent(event)) {}//vide le flux d'evenement

    while(window.isOpen() && !jouer)
    {
        while(window.pollEvent(event) && !jouer)
        {
            if (event.type == Event::KeyReleased)
            {
                if (event.key.code==Keyboard::Enter)
                {
                    jouer=true;
                }
                else if (event.type == sf::Event::Closed)
                    window.close();
                if(manche>1){
                    if (event.key.code==Keyboard::Right)
                    {
                        if (ajouer<manche-1)ajouer++;
                        else ajouer=0;

                    }
                    else if (event.key.code==Keyboard::Left)
                    {
                        if (ajouer>0)ajouer--;
                        else ajouer=manche-1;

                    } 
                }
                else if (manche==1) ajouer=0;

            }


        }
        window.clear();

        afficheFond(window); //affiche le fond d'écran
        affCarte(window,manche,ajouer,d); //affiche les cartes du joueur + carte selectionnée
        afficheTable(window,carteTable,t,nbj); //affiche les cartes deja posées
        window.display();

    }
    while (window.pollEvent(event)) {}//vide le flux d'evenement
    return ajouer; //carte selectionnée
}

int maxPServ(pseudoTable t,int nbj)
{
    int max=0;
    int min=1000;
    for (int i=0;i<nbj;i++)
    {
        if (t[i].point>max) max=t[i].point;
        if (t[i].point<min) min=t[i].point;
    }
    if (max<-min) return -min;
    return max;
}


void affichePointServ(RenderWindow &window,int nbj,pseudoTable t)
{
    Font font,djv;
    font.loadFromFile("../assets/NeoNeon.ttf");
    djv.loadFromFile("../assets/DejaVuSerif.ttf");
    Text joueur("Joueur    :",font,50);
    Text num(std::to_string(1),djv,50);
    joueur.setPosition(310,400);
    num.setPosition(650,400);

    window.draw(joueur);
    window.draw(num);

    for (int i=0;i<nbj;i++)
    {
        Text point(std::to_string(t[i].point-t[i].pointgagne),djv,50);
        point.setPosition(1000,400+i*100);
        window.draw(point);
    }
    int cptr=0;
    int pointM=maxPServ(t,nbj);

    joueur.setScale(0.75,0.75);
    num.setScale(0.75,0.75);


     while(cptr<=pointM)
    {    
        window.clear();
        afficheFond(window);
        int x=20,y=20;
        for (int i=0;i<nbj;i++)
        {
            joueur.setPosition(x,y);
            num.setPosition(x+joueur.getLocalBounds().width/3*2-num.getLocalBounds().width,y);
            num.setString(std::to_string(i+1));
            if (i%2)
            {
                x=20;
                y+=100;
            }
            else 
            {
                x=LARGEUR_FENETRE/2;

            }

            
            window.draw(joueur);
            window.draw(num);
        }

        y=20;
        x=20+joueur.getLocalBounds().width;
        for(int i=0;i<nbj;i++)
        {
            Text point("0",djv,50);
            int pointJ=t[i].point-t[i].pointgagne;

            if(t[i].pointgagne>0)
            { 
                if (t[i].pointgagne<=cptr)
                {
                    point.setString(std::to_string(t[i].point));
                } 
                else
                {         
                    point.setString(std::to_string(pointJ+cptr));
                    
                }
            }
            else if (t[i].pointgagne<=0)
            {
                if (t[i].pointgagne>=-cptr)
                {
                    point.setString(std::to_string(t[i].point));
                    
                } 
                else
                {
                    point.setString(std::to_string(pointJ-cptr));
                    
                }
            }
            point.setPosition(x,y);
            window.draw(point);
            if(i%2!=0)
            {
                x=20+joueur.getLocalBounds().width;
                y+=100;
            }
            else 
            {
                x=LARGEUR_FENETRE/2 + joueur.getLocalBounds().width;
                
            }

        }

        window.display();
        if(cptr!=0) sleep(milliseconds(3000/pointM));
        cptr++;
        
    }

}

void afficheFin(RenderWindow &window,int nbj,pseudoTable t,bool blue)
{
    Font font,djv;
    font.loadFromFile("../NeoNeon.ttf");
    djv.loadFromFile("../DejaVuSerif.ttf");


    Text fin("Fin de la partie",font,50);
    Text joueur("Joueur    :",font,50);
    Text num(std::to_string(1),djv,50);
    Text menu("Appuyer sur Entrer pour revenir au menu",djv,50);
    joueur.setPosition(310,400);
    num.setPosition(650,400);
    FloatRect bounds = fin.getLocalBounds();
    fin.setPosition((LARGEUR_FENETRE-bounds.width)/2,HAUTEUR_FENETRE/4);
    FloatRect bounds2 = menu.getLocalBounds();
    menu.setPosition((LARGEUR_FENETRE-bounds2.width)/2,600);



    if (blue)
    {
        fin.setFillColor(sf::Color(255, 140, 0));
    }
    else
    {
        fin.setFillColor(sf::Color(0, 191, 255));
    }

    window.draw(menu);
    window.draw(fin);

    for (int i=0;i<nbj;i++)
    {
            joueur.setPosition(310,400+i*100);
            num.setPosition(650,400+i*100);
            num.setString(std::to_string(i+1));

            
            window.draw(joueur);
            window.draw(num);
    }


    for (int i=0;i<nbj;i++)
    {
        Text point(std::to_string(t[i].point),djv,50);
        point.setPosition(1000,400+i*100);
        window.draw(point);
    }
}

bool menuFin(RenderWindow &window,int nbj,pseudoTable t)
{
    window.clear();
    
    window.display();
    
    bool finie=false;
    bool blue=false;
    sf::Event event;
    int i=0;
    while (window.pollEvent(event)) {}
    while(window.isOpen() && !finie)
    {
        if (i%2==0) blue=true;
        else blue=false;

        while(window.pollEvent(event) && !finie)
        {
            if (event.type == Event::KeyReleased)
            {
                if (event.key.code==Keyboard::Enter)
                {
                    finie=true;
                }
                else if (event.type == sf::Event::Closed)
                    window.close();
            }
        }


        sf::sleep(sf::milliseconds(10));
        window.clear();
        afficheFin(window,nbj,t,blue); 
        window.display();
        if (!finie) sf::sleep(sf::milliseconds(500));
        i++;
    }

    return false;
}