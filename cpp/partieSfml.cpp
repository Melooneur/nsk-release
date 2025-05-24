#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include "../header/joueur.h"
#include "../header/jeu.h"
#include "../header/deck.h"

using namespace sf;


void afficheFond(RenderWindow &window)
{
    // Affiche le fond d'écran
    
    Texture texture;
    texture.loadFromFile("../assets/fond2.png");
    
    Sprite sprite(texture);
    sprite.setScale(1.0f,0.75); // Ajuste la taille du sprite pour remplir la fenêtre
    sprite.setPosition(0, 0);
    
    window.draw(sprite);
}



void intro(RenderWindow& window)
{
    // Joue l'animation du Skull King
    
    Image icon;
    icon.loadFromFile("../assets/skullking.png");

    Texture texture;
    texture.loadFromImage(icon);
    
    Sprite sprite(texture);
    float s=0;
    int mid_x,mid_y;
    mid_x=s+LARGEUR_FENETRE;
    mid_y=s+HAUTEUR_FENETRE;
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    while (s<2)
    {
        window.clear();
        sprite.setScale(s,s);
        mid_x=(LARGEUR_FENETRE-s*1000)/2;
        mid_y=(HAUTEUR_FENETRE-s*1000)/2;
        sprite.setPosition(mid_x,mid_y);
        window.draw(sprite);
        window.display();
        s+=0.01;
        sleep(milliseconds(60-s*100)); // toutes les 75ms
    }

}

void title(RenderWindow& window)
{
    // Affiche le titre
    
    Font font;
    font.loadFromFile("../assets/neonFont.ttf");
    Text titre1("Neo Skull King", font, 100);
    Text titre2("Neo Skull King", font, 100);

    titre1.setFillColor(sf::Color(0, 191, 255));
    titre2.setFillColor(sf::Color(255, 140, 0));

    sf::FloatRect Ttitre = titre1.getLocalBounds(); // donne le rectangle qui englobe le titre1
    float pos_x = (LARGEUR_FENETRE / 2) - (Ttitre.width / 2);
    float pos_y = 5;
    titre2.setPosition(pos_x+5,pos_y+5);
    titre1.setPosition(pos_x,pos_y);
    window.draw(titre2);
    window.draw(titre1);
}

void SelectMenu(RenderWindow& window,int& buttonSel,int& nbj,int& nbot,bool line)
{
    // Affiche les différents menus
    
    Font font,djv;
    font.loadFromFile("../assets/neonFont.ttf");
    djv.loadFromFile("../assets/DejaVuSerif.ttf");

    window.clear();


    if (buttonSel<=2)
    {

        Text play("Jouer",font,50);
        Text offline("offline",font,50);
        Text online("online",font,50);
        Text quit("Quitter",font,50);

        sf::FloatRect Tplay = play.getLocalBounds(); // donne le rectangle qui englobe "Jouer"
        sf::FloatRect Toffline = offline.getLocalBounds(); // donne le rectangle qui englobe "offline"
        sf::FloatRect Tonline = online.getLocalBounds(); // donne le rectangle qui englobe "online"
        sf::FloatRect Tquit = quit.getLocalBounds(); // donne le rectangle qui englobe "Jouer"

        float play_x = (LARGEUR_FENETRE / 2) - (Tplay.width / 2);
        float off_x = (LARGEUR_FENETRE / 2) - (Toffline.width / 2);
        float on_x = (LARGEUR_FENETRE / 2) - (Tonline.width / 2);
        float quit_x = (LARGEUR_FENETRE / 2) - (Tquit.width / 2);

        play.setPosition(play_x,300);
        offline.setPosition(off_x,400);
        online.setPosition(on_x,400);
        quit.setPosition(quit_x,500);

        if (buttonSel==0)
        {
            play.setFillColor(sf::Color(255, 140, 0));
            offline.setFillColor(sf::Color(0, 191, 255));
            online.setFillColor(sf::Color(0, 191, 255));
            quit.setFillColor(sf::Color(0, 191, 255));
        }
        else if (buttonSel==1)
        {
            play.setFillColor(sf::Color(0, 191, 255));
            offline.setFillColor(sf::Color(255, 140, 0));
            online.setFillColor(sf::Color(255, 140, 0));
            quit.setFillColor(sf::Color(0, 191, 255));
        }
        else if (buttonSel==2)
        {
            play.setFillColor(sf::Color(0, 191, 255));
            offline.setFillColor(sf::Color(0, 191, 255));
            online.setFillColor(sf::Color(0, 191, 255));
            quit.setFillColor(sf::Color(255, 140, 0));
        }

        window.draw(play);
        window.draw(quit);
        if (line) window.draw(online);
        else      window.draw(offline);
    }
    else if(buttonSel<10)
    {

        Text nbPlayer("Player :",font,50);
        Text joueur(std::to_string(nbj),djv,50);
        Text bot(std::to_string(nbot),djv,50);
        Text nbBot("Bot :",font,50);
        Text retour("Menu",font,60);
        Text Jouer("PLAY",font,60);

        nbPlayer.setPosition(150,250);
        joueur.setPosition(540,240);
        nbBot.setPosition(150,400);
        bot.setPosition(540,390);
        retour.setPosition(800,600);
        Jouer.setPosition(380,600);


        if (buttonSel==3)
        {
            //nb player
            nbPlayer.setFillColor(sf::Color(255, 140, 0));
            nbBot.setFillColor(sf::Color(0, 191, 255));
            retour.setFillColor(sf::Color(0, 191, 255));
            Jouer.setFillColor(sf::Color(0, 191, 255));

        }
        else if (buttonSel==4)
        {
            //nb Bots
            nbPlayer.setFillColor(sf::Color(0, 191, 255));
            nbBot.setFillColor(sf::Color(255, 140, 0));
            retour.setFillColor(sf::Color(0, 191, 255));
            Jouer.setFillColor(sf::Color(0, 191, 255));

        }
        else if (buttonSel==5)
        {
            //jouer
            nbPlayer.setFillColor(sf::Color(0, 191, 255));
            nbBot.setFillColor(sf::Color(0, 191, 255));
            retour.setFillColor(sf::Color(0, 191, 255));
            Jouer.setFillColor(sf::Color(255, 140, 0));


        }
        else if (buttonSel==6)
        {
            //menu
            nbPlayer.setFillColor(sf::Color(0, 191, 255));
            nbBot.setFillColor(sf::Color(0, 191, 255));
            retour.setFillColor(sf::Color(255, 140, 0));
            Jouer.setFillColor(sf::Color(0, 191, 255));
        }



        window.clear();



        window.draw(nbPlayer);
        window.draw(joueur);
        window.draw(nbBot);
        window.draw(bot);
        window.draw(Jouer);
        window.draw(retour);
        
    }
    else if(buttonSel>=10)
    {


        Text retour("Menu",font,60);
        sf::FloatRect Tretour = retour.getLocalBounds(); // donne le rectangle qui englobe "Menu"
        float retour_x = (LARGEUR_FENETRE / 2) - (Tretour.width / 2);

        retour.setPosition(retour_x,500);
        retour.setFillColor(sf::Color(255, 140, 0));
        window.draw(retour);


    }

    title(window); // remet le titre
    window.display();
}


void execute(RenderWindow& window,int& buttonSel,bool &line,bool &jouer, bool &fin)
{
    if (buttonSel>=5 or buttonSel<=2)
    {
        window.clear();
        afficheFond(window); //affiche le fond d'écran
        title(window);

        if (buttonSel==0 && !line) buttonSel=3; //menu jouer

        else if(buttonSel==0 && line) jouer=true; //demare la partie en ligne

        else if(buttonSel==1) line= !line; //online/offline

        else if(buttonSel==2)
        {
            window.close(); //quitter
            fin=true;
        }
        else if(buttonSel==6 or buttonSel==10) buttonSel=0; //retour au menu principale

        else if(buttonSel==5) jouer=true;

    }

}

void menu(RenderWindow& window,int &nbj,int& nbot,bool &line, bool &jouer,bool &fin)
{
    // Main menu
    // ButtonSel: De 0 à 2 -> Menu principale; de 3 à ... menu jouer;10 à .. menu parametre
    
    int buttonSel=0;
    window.clear();
    title(window); // Affiche le titre
    SelectMenu(window,buttonSel,nbj,nbot,line); // Affiche les textes du menu + le selectionner est mis en surbrillance

    sf::Event event;




    while(window.isOpen() && !jouer && !fin)
    {
        while(window.pollEvent(event) && !jouer && !fin)//tant qu'un evenement est produit
        {
            if (event.type == Event::KeyReleased)
            {
                if (event.key.code==Keyboard::Enter)
                {
                    execute(window,buttonSel,line,jouer,fin);
                }



                else if (buttonSel<=2)
                {
                    if (event.key.code==Keyboard::Up) // si la fleche du haut est presse
                    {
                        if (buttonSel==0) buttonSel=2; //pour revenir a la fin
                        else buttonSel--; //descend a la ligne d'en dessous
                    } 
                    
                    else if(event.key.code==Keyboard::Down)
                    {
                        if (buttonSel==2) buttonSel=0; //pour revenir au dessus
                        else buttonSel++; //monte a la ligne d'au dessus 
                    }
                    
                }
                else if(buttonSel<10)
                {
                    if (event.key.code==Keyboard::Up) // si la fleche du haut est presse
                    {
                        if (buttonSel==3) buttonSel=6; //pour revenir a la fin
                        else buttonSel--; //descend a la ligne d'en dessous
                    } 
                    
                    else if(event.key.code==Keyboard::Down)
                    {
                        if (buttonSel==6) buttonSel=3; //pour revenir au dessus
                        else buttonSel++; //monte a la ligne d'au dessus 
                    }

                    else if(buttonSel==3)
                    {
                        if (event.key.code==Keyboard::Right)
                        {
                            if (nbj<6)nbj++;
                            else nbj=0;
                        }
                        else if (event.key.code==Keyboard::Left)
                        {
                            if (nbj>0)nbj--;
                            else nbj=6;
                        }
                    }

                    else if(buttonSel==4)
                    {
                        if (event.key.code==Keyboard::Right)
                        {
                            if (nbot<6)nbot++;
                            else nbot=0;
                        }
                        else if (event.key.code==Keyboard::Left)
                        {
                            if (nbot>0)nbot--;
                            else nbot=6;
                        }
                    }

                }

                else if(buttonSel>10)
                {}
                
                SelectMenu(window,buttonSel,nbj,nbot,line);
                
            }
        }      
    }
    while (window.pollEvent(event)) {}
    //vide juste la file 
};









//=======================================================================================Pendant la partie

void afficheManche(RenderWindow &window,int manche)    //affiche "Manche ..."
{
    Font font,djv;
    font.loadFromFile("../assets/neonFont.ttf");
    djv.loadFromFile("../assets/DejaVuSerif.ttf");
    Text Manche("Manche ",font,50);
    Text num(std::to_string(manche),djv,50);
    num.setScale(1.1,1.1);

    Manche.setPosition(1050,10);
    num.setPosition(1350,0);

    window.draw(Manche);
    window.draw(num);
}

void interieurC(RenderWindow &window,carte card,int x,int y,float taille) //taille=0.5 ou 1
{
    Font djv;
    djv.loadFromFile("../assets/DejaVuSerif.ttf");

    Texture img;


    int couleur=card.couleur,nbr=card.valeur;
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
            //scale=0.5;
            
        }
        Sprite imgS(img);
        imgS.setOrigin(imgS.getLocalBounds().width/2,imgS.getLocalBounds().height/2); //centre l'image
        imgS.setScale(scale*taille,scale*taille);
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

        chiffre.setOutlineThickness(2); // Épaisseur du contour
        chiffre.setOutlineColor(sf::Color::White); // Couleur du contour
    
        sf::FloatRect Tchiffre = chiffre.getLocalBounds(); // donne le rectangle qui englobe le chiffre
        chiffre.setOrigin(Tchiffre.width/2,Tchiffre.height/2); //centre le texte
        chiffre.setScale(taille/taille,taille/taille);
        chiffre.setPosition(x,y-(Tchiffre.height/2)*taille); //positionne le texte
        window.draw(chiffre);
    }
}

void afficheC(RenderWindow &window,int nbCarte,joueur j)   
{
    // Affiche les cartes de la main

    Texture imgT;
    imgT.loadFromFile("../assets/carte.png");

    Sprite carte(imgT);

    float carteSize=0.5,cartePos ;
    if(nbCarte>5) carteSize=0.25;   //pour voir toutes les cartes

    carte.setScale(carteSize,carteSize);
    sf::Vector2u taille = carte.getTexture()->getSize();
    

    cartePos=(LARGEUR_FENETRE/2-(0.5*taille.x*nbCarte*carteSize));

    
    for (int i=0;i<nbCarte;i++)
    {
        int posx = cartePos + (taille.x * carteSize) / 2;
        int posy = 550 + (taille.y * carteSize) / 4;
        carte.setPosition(cartePos,550);
        window.draw(carte);
        interieurC(window,j.mainJoueur[i],posx,posy,carteSize*2); //affiche le chiffre de la carte
        cartePos+=taille.x*carteSize;
    }
}

void SfmlafficheJ(RenderWindow &window,int numJ)    //affiche "Joueur ..."
{
    Font font,djv;
    font.loadFromFile("../assets/neonFont.ttf");
    djv.loadFromFile("../assets/DejaVuSerif.ttf");
    Text joueur("Joueur :",font,50);
    Text num(std::to_string(numJ+1),djv,50);
    joueur.setPosition(10,10);
    num.setPosition(350,0);

    window.draw(joueur);
    window.draw(num);

}

void afficheJG(RenderWindow &window,int numJ,int manche)    //affiche "Joueur ... a gagné"
{
    Font font,djv;
    font.loadFromFile("../assets/neonFont.ttf");
    djv.loadFromFile("../assets/DejaVuSerif.ttf");
    Text joueur("Joueur      a gagne le tour ",font,50);
    Text num(std::to_string(numJ+1),djv,50);
    Text mancheT(std::to_string(manche),djv,50);

    sf::FloatRect bounds = joueur.getLocalBounds();
    joueur.setPosition((LARGEUR_FENETRE-bounds.width)/2,(HAUTEUR_FENETRE/4)*3);
    num.setScale(1.25,1.25);  
    mancheT.setScale(1.25,1.25);

    num.setPosition((LARGEUR_FENETRE-bounds.width)/2+325,(HAUTEUR_FENETRE/4)*3-10);
    mancheT.setPosition((LARGEUR_FENETRE-bounds.width)/2+bounds.width,(HAUTEUR_FENETRE/4)*3-10);
    
    afficheFond(window); //affiche le fond d'écran
    window.draw(joueur);
    window.draw(num);
    window.draw(mancheT);

}

void afficheG(RenderWindow &window,int nbParie,int manche,int numJ,joueur j)
{
    // Affiche le pari

    afficheManche(window,manche);
    afficheC(window,manche,j);
    SfmlafficheJ(window,numJ);
    Font djv,font;
    font.loadFromFile("../assets/neonFont.ttf");
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

int gambleSfml(RenderWindow &window,int manche,int numJ, joueur j)
{
    int nbParie = 0;   
    bool jouer=false;

    window.clear();
    afficheFond(window); //affiche le fond d'écran
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
        window.clear();
        afficheFond(window); //affiche le fond d'écran
        afficheG(window,nbParie,manche,numJ,j);

    }
    //sf::sleep(sf::milliseconds(2000));
    while (window.pollEvent(event)) {}
    //vide juste la file
    return nbParie;
}

void afficheTable(RenderWindow &window,int numJ,table t,int nbj,int cptr) //carte deja posé par autre joueurs
{
    if (cptr!=0)
    {
        Texture imgT;
        imgT.loadFromFile("../assets/carte.png");
        Sprite carte(imgT);

        float carteSize=0.25,cartePos;

        carte.setScale(carteSize,carteSize);
        sf::Vector2u taille = carte.getTexture()->getSize();
        if (cptr>nbj) cartePos=(LARGEUR_FENETRE/2-(0.5*taille.x*(nbj-1)*carteSize));
        else cartePos=(LARGEUR_FENETRE/2-(0.5*taille.x*(nbj)*carteSize));
        int premierJ=numJ-cptr;
        if (premierJ<=0) premierJ=nbj+premierJ;

        for (int i=0;i<cptr;i++)
        {
            int posx = static_cast<int>(cartePos + (taille.x * carteSize) / 2);
            int posy = 200 + (taille.y * carteSize) / 2;
            carte.setPosition(cartePos,200);
            window.draw(carte);
            interieurC(window,t[(premierJ+i)%nbj].carteActive,posx,posy,1);


            cartePos+=taille.x*carteSize;
        }
    }

}

void afficheCS(RenderWindow &window,int nbCarte,int numC,int numJ,table t,int joueur,int nbj,int cptr)    //selection de carte en plus comparer a l'autre
{   
    //numC -> carte selectionné

    window.clear();
    afficheFond(window); //affiche le fond d'écran

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
            posx = cartePos + (taille.x * carteSize) / 2;
            posy = 450 + (taille.y * carteSize) / 4;
            carte.setPosition(cartePos,450);
            window.draw(carte);
            interieurC(window,t[numJ].mainJoueur[i],posx,posy,carteSize*2);
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
            }}

            window.draw(carte);
            interieurC(window,t[numJ].mainJoueur[i],posx,posy,carteSize*2);
        
        
        cartePos+=taille.x*carteSize;

    }

    afficheManche(window,nbCarte);
    SfmlafficheJ(window,numJ);
    afficheTable(window,numJ,t,nbj,cptr);

    window.display();
}

void SfmlCarteJ(RenderWindow &window,int manche,int numJ,int &ajouer,table t,int nbj,int cptr)// pouvoir choisir la carte a jouer
{

    window.clear();
    afficheFond(window); //affiche le fond d'écran
    ajouer=0; //la carte selectionné
    //afficheCS(window,manche,ajouer,numJ);
    bool jouer=false;

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
        afficheCS(window,manche,ajouer,numJ,t,numJ,nbj,cptr);
    }
    while (window.pollEvent(event)) {}
}

int maxP(table t,int nbj)
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

void affichePoint(RenderWindow &window,int nbj,table t)
{
    Font font,djv;
    font.loadFromFile("../assets/neonFont.ttf");
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
    int pointM=maxP(t,nbj);



    while(cptr<=pointM)
    {    
        window.clear();
        afficheFond(window);
        for (int i=0;i<nbj;i++)
        {
            joueur.setPosition(310,400+i*100);
            num.setPosition(650,400+i*100);
            num.setString(std::to_string(i+1));

            
            window.draw(joueur);
            window.draw(num);
        }

        for(int i=0;i<nbj;i++)
        {
            int pointJ=t[i].point-t[i].pointgagne;

            if(t[i].pointgagne>0)
            { 
                if (t[i].pointgagne<=cptr)
                {
                    Text point(std::to_string(t[i].point), djv, 50);
                    point.setPosition(980,400+i*100);
                    window.draw(point);
                } 
                else
                {         
                    Text point(std::to_string(pointJ+cptr), djv, 50);
                    point.setPosition(980,400+i*100);
                    window.draw(point);
                }
            }
            else if (t[i].pointgagne<=0)
            {
                if (t[i].pointgagne>=-cptr)
                {
                    Text point(std::to_string(t[i].point), djv, 50);
                    point.setPosition(980,400+i*100);
                    window.draw(point);
                } 
                else
                {
                    Text point(std::to_string(pointJ-cptr), djv, 50);
                    point.setPosition(980,400+i*100);
                    window.draw(point);
                }
            }
        }
        window.display();
        if(cptr!=0) sleep(milliseconds(3000/pointM));
        cptr++;
        
    }

}


void choixTigresse(RenderWindow &window,bool tigresse)
{
    Font font;
    font.loadFromFile("../assets/neonFont.ttf");

    Text fuite("Fuite",font,50);
    Text pirate("Pirate",font,50);

    if (tigresse)
    {
        fuite.setFillColor(sf::Color(0, 191, 255));
        pirate.setFillColor(sf::Color(255, 140, 0));
    }
    else
    {
        fuite.setFillColor(sf::Color(255, 140, 0));
        pirate.setFillColor(sf::Color(0, 191, 255));
    }
    
    FloatRect bounds = fuite.getLocalBounds();
    FloatRect bounds2 = pirate.getLocalBounds();
    fuite.setPosition((LARGEUR_FENETRE-bounds.width)/4,HAUTEUR_FENETRE/3*2);
    pirate.setPosition((LARGEUR_FENETRE-bounds2.width)/4*3,HAUTEUR_FENETRE/3*2);

    window.draw(fuite);
    window.draw(pirate);



}



bool tigresse (RenderWindow &window)
{
    window.clear();
    afficheFond(window); //affiche le fond d'écran
    window.display();
    Font font;
    font.loadFromFile("../assets/neonFont.ttf");

    Image fuite;
    Image pirate;



    Text titre("Vous avez joue la carte tigresse",font,50);
    Text choix("Souhaitez vous jouez fuite ou pirate",font,40);
    bool tigresse=false,jouer=false;        //tigresse=true -> pirate , tigresse=false -> fuite

    FloatRect bounds = choix.getLocalBounds(); 
    

    titre.setPosition(10,00);
    choix.setPosition((LARGEUR_FENETRE-bounds.width)/2,100);
    sf::Event event;
    while (window.pollEvent(event)) {}
    //vide juste la file

    while(window.isOpen() && !jouer)
    {

        while(window.pollEvent(event))
        {
            if (event.type == Event::KeyReleased)
            {
                if (event.key.code==Keyboard::Enter)
                {
                    jouer=true;
                }    
                else if (event.key.code==Keyboard::Right)
                {
                    tigresse=true;
                    
                }
                else if (event.key.code==Keyboard::Left)
                {
                    tigresse=false;
                }
                window.clear();
                
            }
        }

        afficheFond(window);
        choixTigresse(window,tigresse);

        window.draw(titre);
        window.draw(choix);
        window.display();

    }
    return tigresse; 
}

void afficheFinOffline(RenderWindow &window,int nbj,table t,bool blue)
{
    Font font,djv;
    font.loadFromFile("../assets/neonFont.ttf");
    djv.loadFromFile("../assets/DejaVuSerif.ttf");


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

bool menuFinOffline(RenderWindow &window,int nbj,table t)
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
        if (i%2==0 ) blue=true;
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
        afficheFond(window);
        afficheFinOffline(window,nbj,t,blue); 
        window.display();
        if (!finie) sf::sleep(sf::milliseconds(500));
        i++;
    }



    return false;
}