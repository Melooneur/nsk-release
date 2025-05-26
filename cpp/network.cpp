#include <asio.hpp>
#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <SFML/Graphics.hpp>
#include "../header/SfmlServ.h"
#include "../header/partieSfml.h"
#include "../header/deck.h"
#include "../header/network.h"
#include "../header/joueur.h"

using asio::ip::tcp;
using namespace std;

int manche = 1;
int nbj = 1;
int idClient = 0;

void viderDeck(pseudoJoueur& joueur)
{
    if (joueur.mainJoueur != nullptr)
    {
        delete[] joueur.mainJoueur;
        joueur.mainJoueur = nullptr;
        joueur.tailleMain = 0;
    }
}

int compterCartesActives(pseudoTable t, int nbj) 
{
    int compteur = 0;
    for (int i = 0; i < nbj; i++) 
    {
        if (t[i].carteActive.valeur != -1 && t[i].carteActive.couleur != -1) 
        {
            compteur++;
        }
    }
    return compteur;
}

int recevoirInt(tcp::socket& socket) 
{
    int recu;
    asio::error_code ec;
    asio::read(socket, asio::buffer(&recu, sizeof(recu)), ec);

    if (ec) {
        std::cerr << "[Erreur] Réception int : " << ec.message() << '\n';
        return false;
    }
    
    return recu;
}

void recevoirNombreJoueurs(tcp::socket& socket, int& nbJoueurs)
{
    asio::error_code ec;

    // Lire la taille du message (doit être sizeof(int))
    uint32_t taille = 0;
    asio::read(socket, asio::buffer(&taille, sizeof(taille)), ec);
    if (ec) 
    {
        std::cerr << "Erreur lecture taille (nbJoueurs) : " << ec.message() << '\n';
    }

    if (taille != sizeof(int)) 
    {
        std::cerr << "Taille inattendue pour le nombre de joueurs : " << taille << '\n';
    }

    // Lire la valeur nbJoueurs
    asio::read(socket, asio::buffer(&nbJoueurs, sizeof(nbJoueurs)), ec);
    if (ec) 
    {
        std::cerr << "Erreur lecture valeur nbJoueurs : " << ec.message() << '\n';
    }

    std::cout << "Nombre de joueurs reçu : " << nbJoueurs << '\n';
}

int recevoirDebutTour(tcp::socket& socket)
{
    int tour;
    asio::error_code ec;

    asio::read(socket, asio::buffer(&tour, sizeof(int)), ec);

    if (ec) {
        std::cerr << "[Client] Erreur lors de la réception du début de tour : " << ec.message() << "\n";
        return -1;
    }

    return tour;
}

bool recevoirJoueur(tcp::socket& socket, pseudoJoueur& info) 
{
    asio::error_code ec;

    uint32_t taille = 0;
    asio::read(socket, asio::buffer(&taille, sizeof(taille)), ec);
    if (ec) {
        std::cerr << "Erreur lecture taille : " << ec.message() << '\n';
        return false;
    }

    std::vector<char> buffer(taille);
    asio::read(socket, asio::buffer(buffer), ec);
    if (ec) {
        std::cerr << "Erreur lecture données : " << ec.message() << '\n';
        return false;
    }

    size_t offset = 0;

    std::memcpy(&info.point, buffer.data() + offset, sizeof(info.point));
    offset += sizeof(info.point);

    std::memcpy(&info.parie, buffer.data() + offset, sizeof(info.parie));
    offset += sizeof(info.parie);
    
    std::memcpy(&info.pointgagne, buffer.data() + offset, sizeof(info.pointgagne));
    offset += sizeof(info.pointgagne);

    std::memcpy(&info.plit, buffer.data() + offset, sizeof(info.plit));
    offset += sizeof(info.plit);

    std::memcpy(&info.carteActive.valeur, buffer.data() + offset, sizeof(info.carteActive.valeur));
    offset += sizeof(info.carteActive.valeur);

    std::memcpy(&info.carteActive.couleur, buffer.data() + offset, sizeof(info.carteActive.couleur));
    offset += sizeof(info.carteActive.couleur);

    std::memcpy(&info.carteActive.tigressePirate, buffer.data() + offset, sizeof(info.carteActive.tigressePirate));

    return true;
}

bool recevoirDeck(tcp::socket& socket, pseudoJoueur& p) 
{
    viderDeck(p);
    asio::error_code ec;

    uint32_t taille = 0;
    asio::read(socket, asio::buffer(&taille, sizeof(taille)), ec);
    if (ec) 
    {
        std::cerr << "Erreur lecture taille : " << ec.message() << '\n';
        return false;
    }

    std::vector<char> buffer(taille);
    asio::read(socket, asio::buffer(buffer), ec);
    if (ec) 
    {
        std::cerr << "Erreur lecture données : " << ec.message() << '\n';
        return false;
    }

    size_t offset = 0;

    // Lire nombre de cartes
    std::memcpy(&p.tailleMain, buffer.data() + offset, sizeof(int));
    offset += sizeof(int);

    // Allouer dynamiquement le deck
    if (p.mainJoueur != nullptr) 
    {
        delete[] p.mainJoueur;
    }
    p.mainJoueur = new carte[p.tailleMain];

    // Lire les cartes une par une
    for (int i = 0; i < p.tailleMain; ++i) 
    {
        std::memcpy(&p.mainJoueur[i].valeur, buffer.data() + offset, sizeof(int));
        offset += sizeof(int);

        std::memcpy(&p.mainJoueur[i].couleur, buffer.data() + offset, sizeof(int));
        offset += sizeof(int);

        std::memcpy(&p.mainJoueur[i].tigressePirate, buffer.data() + offset, sizeof(bool));
        offset += sizeof(bool);
    }

    std::cout << "[DEBUG] Taille du message reçu (deck) : " << taille << " octets" << std::endl;

    return true;
}

void envoyerDonneesJoueurAuServeur(tcp::socket& socket, pseudoJoueur& j)
{
    // Envoie l'ensemble des données du pseudo Joueur au serveur
    if (!socket.is_open()) 
    {
        std::cerr << "Socket vers le serveur fermée\n";
        return;
    }

    // Données à envoyer : int point, int parie, int plit, carte (valeur, couleur, tigressePirate)
    int taille = sizeof(int) * 5 + sizeof(bool); // total des données utiles
    std::vector<char> buffer(sizeof(taille) + taille); // taille (4) + contenu

    // Écrire la taille
    std::memcpy(buffer.data(), &taille, sizeof(taille));

    // Écrire les champs
    size_t offset = sizeof(taille);

    std::memcpy(buffer.data() + offset, &j.point, sizeof(j.point));
    offset += sizeof(j.point);

    std::memcpy(buffer.data() + offset, &j.parie, sizeof(j.parie));
    offset += sizeof(j.parie);

    std::memcpy(buffer.data() + offset, &j.plit, sizeof(j.plit));
    offset += sizeof(j.plit);

    std::memcpy(buffer.data() + offset, &j.carteActive.valeur, sizeof(j.carteActive.valeur));
    offset += sizeof(j.carteActive.valeur);

    std::memcpy(buffer.data() + offset, &j.carteActive.couleur, sizeof(j.carteActive.couleur));
    offset += sizeof(j.carteActive.couleur);

    std::memcpy(buffer.data() + offset, &j.carteActive.tigressePirate, sizeof(j.carteActive.tigressePirate));

    // Envoi
    asio::error_code ec;
    asio::write(socket, asio::buffer(buffer), ec);

    if (ec) 
    {
        std::cerr << "Erreur envoi données joueur au serveur : " << ec.message() << '\n';
    }
    else
    {
        std::cout << "Données joueur envoyées au serveur.\n";
    }
}

void sendPlayerNumberToServer(int ind, tcp::socket& socket)
{
    // Taille = taille de l'int (ind)
    uint32_t taille = sizeof(int);

    std::vector<char> buffer(sizeof(taille) + taille);

    // Copier la taille au début
    std::memcpy(buffer.data(), &taille, sizeof(taille));

    // Copier la donnée (ind) juste après la taille
    std::memcpy(buffer.data() + sizeof(taille), &ind, sizeof(ind));

    asio::error_code ec;
    asio::write(socket, asio::buffer(buffer), ec);

    if (ec)
    {
        std::cerr << "Erreur envoi nombre joueurs au serveur : " << ec.message() << '\n';
    }
    else
    {
        std::cout << "Indice de la carte envoyée : " << ind+1 << std::endl;
    }
}

//Envoie boule au serveur
void sendBool(bool envoi, tcp::socket& socket)
{
    // Taille = taille de l'int (ind)
    uint32_t taille = sizeof(bool);

    std::vector<char> buffer(sizeof(taille) + taille);

    // Copier la taille au début
    std::memcpy(buffer.data(), &taille, sizeof(taille));

    // Copier la donnée (envoi) juste après la taille
    std::memcpy(buffer.data() + sizeof(taille), &envoi, sizeof(envoi));

    asio::error_code ec;
    asio::write(socket, asio::buffer(buffer), ec);

    if (ec)
    {
        std::cerr << "Erreur envoi bool : " << ec.message() << '\n';
    }
}




int attendreDebutTour(tcp::socket& socket)
{
    int t = -1;

    // Tant qu'on ne reçoit pas une manche valide, on ne continue pas
    while (t == -1)
    {
        t = recevoirDebutTour(socket);  // bloquant par nature
        if (t == -1)
        {
            std::cerr << "[Client] Attente du début de tour...\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(100)); // éviter de spammer si jamais il y a une micro erreur réseau
        }
    }

    return t;
}

string fetchIP()
{
    // On importe le fichier config.txt pour verifier l'IP qui y est rentree
    string chemin = "../config.txt";
    ifstream config(chemin);

    if (config.is_open())
    {
        string ligne;
        getline(config, ligne);
        return ligne;
    }
    else
    {
        return "90.105.238.16";
    }
}





void connectToServer(sf::RenderWindow& window)
{
    bool rejoue = true;
    // On connecte le client au serveur avec l'IP fournie dans le fichier config.txt
    while(rejoue)
    {        
        window.clear();
        window.display();
        
        asio::io_context io_context;
        tcp::resolver resolver(io_context);
        auto endpoints = resolver.resolve(fetchIP(), "25565");
    
        tcp::socket socket(io_context);
        asio::connect(socket, endpoints);
        
        cout << "Connecté au serveureuhhh.\n";
        recevoirNombreJoueurs(socket, nbj);
        pseudoTable tableJ = new pseudoJoueur[nbj];
        idClient = recevoirInt(socket);

        bool fin = false;
        while (manche<=10)
        {
            for(int k = 0; k < nbj; k++)
                {
                    recevoirJoueur(socket, tableJ[k]);
                    recevoirDeck(socket, tableJ[k]);
                }
            
            if (recevoirJoueur(socket, tableJ[idClient]))
            {               
                fin = false;
                recevoirDeck(socket, tableJ[idClient]);
                manche = tableJ[idClient].tailleMain;
                cout << "Manche : " << manche << endl;
                cout << "Nombre de joueurs : " << nbj << endl;
                tableJ[idClient].parie = gambleServ(window, tableJ[idClient].tailleMain, tableJ[idClient].mainJoueur);
                envoyerDonneesJoueurAuServeur(socket, tableJ[idClient]);
                window.clear();
                afficheFond(window);
                window.display();
                
                int tour = 0;
                int ind;
                
                while (tour+1 < manche or(manche==1 and !fin))
                {                   
                    cout<<manche;
                    
                    for (int j = 0; j < nbj; j++) 
                    {
                        recevoirJoueur(socket, tableJ[j]); // On met à jour les mêmes objets : tableJ[k]
                        recevoirDeck(socket, tableJ[j]);
                    }
                    int compteCarte = compterCartesActives(tableJ, nbj);
                    


                    window.clear();

                    tour = attendreDebutTour(socket);

                    ind = joueCarte(window, manche-tour, tableJ[idClient].mainJoueur, tableJ, compteCarte,nbj);



                    sendPlayerNumberToServer(ind, socket); // Renvoie ind

                    //LA DONC
                    if (tableJ[idClient].mainJoueur[ind].couleur == 7)  
                    {

                        bool tigre=tigresse(window);
                        sendBool(tigre, socket);
                    }
                    window.clear();
                    afficheFond(window);
                    afficheTable(window, compterCartesActives(tableJ,nbj), tableJ, nbj);
                    window.display();


                    int numJ = recevoirInt(socket);
                    cout << "Gagnant :" << numJ;

                    for (int j = 0; j < nbj; j++) 
                    {
                        recevoirJoueur(socket, tableJ[j]); // On met à jour les mêmes objets : tableJ[k]
                        recevoirDeck(socket, tableJ[j]);
                    }
                        


                    window.clear();
                    afficheJG(window, numJ, tour+1);
                    afficheTable(window, compterCartesActives(tableJ,nbj), tableJ, nbj);
                    window.display();
                    sf::sleep(sf::seconds(2)); //5sec pour voir les cartes
                   

                    cout << "Tour :" << tour+1;
                    fin=true;
                }

                cout << "Fini";



                for (int j = 0; j < nbj; j++) 
                {
                    recevoirJoueur(socket, tableJ[j]); // On met à jour les mêmes objets : tableJ[k]
                    recevoirDeck(socket, tableJ[j]);
                }

                    window.clear();
                    afficheFond(window);
                    affichePointServ(window, nbj, tableJ);
                    window.display();
                    sf::sleep(sf::seconds(2)); // 2sec pour voir les points

                
            }
            manche++;
        }
        rejoue=menuFin(window,nbj,tableJ);

        
        

    }/*
    catch (exception& e) 
    {
        cerr << "Client error: " << e.what() << "\n";
    }*/
}