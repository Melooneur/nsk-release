#include <asio.hpp>
#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <cstdlib>
#include <ctime>
#include "../../header/joueur.h"
#include "../../header/deck.h"
#include "../../header/jcpu.h"
#include "../../header/partieSfml.h"
using namespace std;
using asio::ip::tcp;

void sendInt(std::shared_ptr<tcp::socket>& socket, int entier) 
{
    if (socket)
    {
        asio::error_code ec;
        asio::write(*socket, asio::buffer(&entier, sizeof(entier)), ec);

        if (ec) 
        {
            std::cerr << "[Erreur] Envoi de l'entier : " << ec.message() << '\n';
        }
    }
}

void sendPlayerNumber(int nbj, table & dest)
{
    int taille = sizeof(int);
    std::vector<char> buffer(sizeof(taille) + taille);
    std::memcpy(buffer.data(), &taille, sizeof(taille));
    std::memcpy(buffer.data() + sizeof(taille), &nbj, sizeof(nbj));

    for (int i = 0; i < nbj; i++)
    {
        if (!dest[i].humain) continue;

        std::shared_ptr<tcp::socket> socket = dest[i].socket;
        asio::error_code ec;

        if (!socket || !socket->is_open())
        {
            std::cerr << "Socket invalide ou fermée pour un joueur\n";
            continue;
        }

        asio::write(*socket, asio::buffer(buffer), ec);

        if (ec)
        {
            std::cerr << "Erreur envoi manche au joueur : " << ec.message() << '\n';
        }
    }
}

void sendDebutTour(joueur plr, int i)
{
    if (!plr.humain) return;

    std::shared_ptr<tcp::socket> socket = plr.socket;

    if (!socket || !socket->is_open())
    {
        std::cerr << "Socket invalide ou fermée pour un joueur\n";
        return;
    }

    asio::error_code ec;
    asio::write(*socket, asio::buffer(&i, sizeof(i)), ec);

    if (ec)
    {
        std::cerr << "Erreur envoi manche au joueur : " << ec.message() << '\n';
    }

    std::cout << "Début du tour envoyé : " << i << "\n";
}

void sendPlayerData(joueur & j, joueur & dest) 
{
    if (dest.humain)
    {
        std::shared_ptr<tcp::socket> socket = dest.socket;
        
        if (!socket || !socket->is_open()) 
        {
            std::cerr << "Socket invalide ou fermée\n";
            return;
        }

        int taille = sizeof(int) * 6 + sizeof(bool);
        std::vector<char> buffer(sizeof(taille) + taille);
        std::memcpy(buffer.data(), &taille, sizeof(taille));

        size_t offset = sizeof(taille);

        std::memcpy(buffer.data() + offset, &j.point, sizeof(j.point)); offset += sizeof(j.point);
        std::memcpy(buffer.data() + offset, &j.parie, sizeof(j.parie)); offset += sizeof(j.parie);
        std::memcpy(buffer.data() + offset, &j.pointgagne, sizeof(j.pointgagne)); offset += sizeof(j.pointgagne);
        std::memcpy(buffer.data() + offset, &j.plit, sizeof(j.plit)); offset += sizeof(j.plit);
        std::memcpy(buffer.data() + offset, &j.carteActive.valeur, sizeof(j.carteActive.valeur)); offset += sizeof(j.carteActive.valeur);
        std::memcpy(buffer.data() + offset, &j.carteActive.couleur, sizeof(j.carteActive.couleur)); offset += sizeof(j.carteActive.couleur);
        std::memcpy(buffer.data() + offset, &j.carteActive.tigressePirate, sizeof(j.carteActive.tigressePirate));

        asio::error_code ec;
        asio::write(*socket, asio::buffer(buffer), ec);

        if (ec) 
        {
            std::cerr << "Erreur lors de l'envoi simple : " << ec.message() << '\n';
        }
    }
}

void sendDeck(joueur & plr, joueur & dest, int manche, int i) 
{
    if (dest.humain)
    {
        deck d = plr.mainJoueur;
        int tailleDeck = manche-i;
        std::shared_ptr<tcp::socket> socket = dest.socket;

        if (!socket || !socket->is_open()) 
        {
            std::cerr << "Socket invalide ou fermée\n";
            return;
        }

        uint32_t taille = sizeof(int) + tailleDeck * (sizeof(int) * 2 + sizeof(bool));
        std::vector<char> buffer(sizeof(taille) + taille);
        std::memcpy(buffer.data(), &taille, sizeof(taille));

        size_t offset = sizeof(taille);
        std::memcpy(buffer.data() + offset, &tailleDeck, sizeof(int));
        offset += sizeof(int);

        for (int i = 0; i < tailleDeck; ++i) 
        {
            std::memcpy(buffer.data() + offset, &d[i].valeur, sizeof(int)); offset += sizeof(int);
            std::memcpy(buffer.data() + offset, &d[i].couleur, sizeof(int)); offset += sizeof(int);
            std::memcpy(buffer.data() + offset, &d[i].tigressePirate, sizeof(bool)); offset += sizeof(bool);
        }

        asio::error_code ec;
        asio::write(*socket, asio::buffer(buffer), ec);

        if (ec) 
        {
            std::cerr << "Erreur lors de l'envoi du deck : " << ec.message() << '\n';
        }
    }
}

void sendToPlayer(joueur & plr, joueur & dest, int manche, int i)
{
    if (dest.humain)
    {
        sendPlayerData(plr, dest);
        sendDeck(plr, dest, manche, i);
    }
}

void broadcast(table & players, int manche, int i, int nbj) 
{
    for (int j = 0; j < nbj; j++) 
    {
        if (players[j].humain)
        {
            std::cout << "[INFO] Envoi au joueur " << j+1 << std::endl;
            sendToPlayer(players[j], players[j], manche, i);
        } 
    }
}

void broadcastAll(table & players, int manche, int i, int nbj) 
{
    for (int j = 0; j < nbj; j++) 
    {
        for (int k = 0; k < nbj; k++)
        {
            if (players[k].humain)
            {
                std::cout << "[INFO] Envoi au joueur " << k+1 << " des données de " << j+1 << std::endl;
                sendToPlayer(players[j], players[k], manche, i);
            }  
        }
    }
}

void sendWholeTable(table &players, int manche, int i, int nbj, int targetPlayerIndex)
{
    if (!players[targetPlayerIndex].humain) return;

    for (int sender = 0; sender < nbj; sender++)
    {
        std::cout << "[INFO] Envoi des données du joueur " << sender + 1
                  << " au joueur " << targetPlayerIndex + 1 << std::endl;
        sendToPlayer(players[sender], players[targetPlayerIndex], manche, i);
    }
}

void recevoirDonneesJoueurDepuisClient(joueur& j, std::shared_ptr<tcp::socket> socket)
{
    if (!socket || !socket->is_open())
    {
        std::cerr << "Socket invalide ou fermée pour réception\n";
        return;
    }

    asio::error_code ec;

    // Lire la taille d'abord
    int taille = 0;
    asio::read(*socket, asio::buffer(&taille, sizeof(taille)), ec);
    if (ec)
    {
        std::cerr << "Erreur lors de la lecture de la taille : " << ec.message() << '\n';
        return;
    }

    // Lire les données
    std::vector<char> buffer(taille);
    asio::read(*socket, asio::buffer(buffer), ec);
    if (ec)
    {
        std::cerr << "Erreur lors de la lecture des données joueur : " << ec.message() << '\n';
        return;
    }

    size_t offset = 0;

    std::memcpy(&j.point, buffer.data() + offset, sizeof(j.point));
    offset += sizeof(j.point);

    std::memcpy(&j.parie, buffer.data() + offset, sizeof(j.parie));
    offset += sizeof(j.parie);

    std::memcpy(&j.plit, buffer.data() + offset, sizeof(j.plit));
    offset += sizeof(j.plit);

    std::memcpy(&j.carteActive.valeur, buffer.data() + offset, sizeof(j.carteActive.valeur));
    offset += sizeof(j.carteActive.valeur);

    std::memcpy(&j.carteActive.couleur, buffer.data() + offset, sizeof(j.carteActive.couleur));
    offset += sizeof(j.carteActive.couleur);

    std::memcpy(&j.carteActive.tigressePirate, buffer.data() + offset, sizeof(j.carteActive.tigressePirate));
}

int receivePlayerNumber(joueur &plr)
{
    std::shared_ptr<tcp::socket> socket = plr.socket;
    asio::error_code ec;

    // Lire la taille (uint32_t)
    uint32_t taille = 0;
    asio::read(*socket, asio::buffer(&taille, sizeof(taille)), ec);
    if (ec)
    {
        std::cerr << "Erreur lecture taille (Indice carte) : " << ec.message() << '\n';
        return -1; // erreur
    }

    if (taille != sizeof(int))
    {
        std::cerr << "Taille inattendue reçue : " << taille << '\n';
        return -1;
    }

    // Lire la donnée (int)
    int indiceC = 0;
    asio::read(*socket, asio::buffer(&indiceC, sizeof(indiceC)), ec);
    if (ec)
    {
        std::cerr << "Erreur lecture indice carte : " << ec.message() << '\n';
        return -1;
    }

    std::cout << "Indice carte recu cote serveur : " << indiceC << std::endl;

    return indiceC;
}


bool receiveBool(joueur &plr)
{
    // Reçoit le booléen pour la Tigresse
    std::shared_ptr<tcp::socket> socket = plr.socket;
    asio::error_code ec;

    // Lire la taille (uint32_t)
    uint32_t taille = 0;
    asio::read(*socket, asio::buffer(&taille, sizeof(taille)), ec);
    if (ec)
    {
        std::cerr << "Erreur lecture taille (Bool) : " << ec.message() << '\n';
        return false; // erreur
    }

    if (taille != sizeof(bool))
    {
        std::cerr << "Taille inattendue reçue : " << taille << '\n';
        return false;
    }

    // Lire la donnée (int)
    bool recu;
    asio::read(*socket, asio::buffer(&recu, sizeof(recu)), ec);
    if (ec)
    {
        std::cerr << "Erreur lecture bool : " << ec.message() << '\n';
        return false;
    }

    std::cout << "Bool recu cote serveur : " << recu << std::endl;

    return recu;
}



// ========================================================================================================================================================================= //

void connexionClients(asio::io_context& io_context, table &t, int nbj)
{
    // Connecte tous les clients au serveur et met leur socket dans leur joueur
    // Les CPU sont exempt de ça puisque pas encore compté dans nbj

    tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 25565));  // On se connecte en tcp aux clients, l'ip est celle locale de la machine, le port est 25565

    // On accepte nbj connexions
    for (int i = 0; i < nbj; ++i)
    {
        cout << "[INFO] En attente de connexion pour le joueur " << i + 1 << " ..." << endl;
        shared_ptr<tcp::socket> sock = make_shared<tcp::socket>(io_context); 
        acceptor.accept(*sock); // Une fois la connexion établie
        cout << "[INFO] Joueur " << i + 1 << " connecte !" << endl;
        t[i].socket = sock; // On stocke le socket dans la struct joueur
    } 
}

void gambleOnline(table& t, int nbj, int manche)
{ 
    for(int i = 0; i < nbj; i++)
    {
        if(t[i].humain)
        {
            
            // On envoie la struct et la pseudo table à tous les joueurs avec leurs cartes
            sendToPlayer(t[i], t[i], manche, 0);
            recevoirDonneesJoueurDepuisClient(t[i], t[i].socket);

            cout << "Joueur " << i+1 << " a parie : " << t[i].parie;
        }
        else
        {
            gambleCPU(t, nbj, manche, i);
        }
    }   
}

void piocheOnline(joueur &j, int manche, deck &d, int &taille, int num) // -> Version unique
{
    j.mainJoueur = new carte[manche]; 

    for (int i = 0; i < manche; i++)
    {
        j.mainJoueur[i] = d[0]; 
        cout << "Vous avez pioche la carte : " << carteToString(j.mainJoueur[i]);
        supp_d0(d,taille); // On supprime la carte piochee du deck
    }
}

void joueOnline(deck &d, int &taille, table &t, int nbj, int manche) // Joue la manche
{
    int carteC = -1;
    int num = (manche - 1) % nbj; // C'est au joueur i de jouer -> à chaque manche le 1er joueur change
    int cptr;
    
    for (int i = 0; i < manche; i++) // Nombre de tours par manche
    {
        cptr=0; // Nombre de joueurs qui ont joue
        
        for (int k = 0; k < nbj; k++)
            t[k].carteActive = creerCarte(-1,-1);
        
        while(cptr<nbj) // Tant que tous les joueurs n'ont pas joué
        {
            
            int joueur_actif=(num+cptr) % nbj; // Joueur actif
            sendWholeTable(t, manche, i, nbj, joueur_actif); 

            if(t[joueur_actif].humain)
            {
                
                // Envoie au joueur actif -> c'est à toi de jouer
                sendDebutTour(t[joueur_actif], i);
                
                carteC = receivePlayerNumber(t[joueur_actif]);
                // Attend la réponse du joueur en question
                // -> réponse : joue la carte et envoie à tout le monde la carte active modifiée
                // Passe au joueur suivant

                // ajouter l'option tigresse !!!!!! -> oui je sais
                
                if (t[joueur_actif].mainJoueur[carteC].couleur == 7)//l'option tigresse en legendeee
                    {
                        bool tigressePirate = receiveBool(t[joueur_actif]);
                        cout <<"tigressePirate" << tigressePirate<< endl;
                        t[joueur_actif].mainJoueur[carteC].tigressePirate = tigressePirate;
                        cout<<"dedans";
                    }
                

                poseCarte(carteC, t, joueur_actif);

                        
                carte defausse = supp_cc(t[joueur_actif].mainJoueur, carteC, manche-i); // On supprime la carte choisie de la main du joueur et on va la remettre dans le deck à la fin
                if (defausse.couleur != -1) ajouteFin(d,taille,defausse);

                cptr++;
                cout << "Carte posee : "; afficheCarte(t[joueur_actif].carteActive); cout << endl;

            }
            else
            {
                carteC = joueCPU(d, joueur_actif, taille, t, manche, i, nbj);
                carte defausse = supp_cc(t[joueur_actif].mainJoueur, carteC, manche-i); // On supprime la carte choisie de la main du joueur et on va la remettre dans le deck à la fin
                if (defausse.couleur != -1) ajouteFin(d,taille,defausse);
                cptr++;
            }
        }
        num = verifGagnant(t,nbj,num); // Le gagnant a +1 plit
        for(int j = 0; j < nbj; j++)
        {
            sendInt(t[j].socket,num);
        }
        for(int j = 0; j < nbj; j++)
        {
            sendWholeTable(t, manche, i, nbj, j);
        }
    }
}

void attributionPointOnline(table t,int nbj,int manche,int joueur_actif)
{
    // Attribue les points aux joueurs à la fin de la manche

    for (int i=0;i<nbj;++i)
    {
        if (t[i].parie==0) //si parie 0
        {
            if (t[i].plit==0)
            {
                t[i].point+=10*manche; //gagne
            }
            else
            {              
                t[i].point-=10*manche; //perdu
            }
        }
        else //sinon
        {
            if (t[i].plit==t[i].parie)
            { 
                t[i].point+=t[i].parie*20;                  //gagne
            }
            else
            {
                t[i].point-=diff(t[i].parie,t[i].plit)*20;  //perdu
            }
        }

        sendWholeTable(t,manche, 0, nbj, i); 
    }

}     

void mancheOnline(table &t, int manche, int nbj, deck &d, int& taille)
{
    int i = 0; // Compteur pour que le distributeur change à chaque manche 
    melangeDeck(d, taille); // On melange le deck avant de commencer la manche

    while (i < nbj)
    {
        int num = (i + manche - 1) % nbj; // C'est au joueur i de piocher -> à chaque manche le 1er piocheur change
        piocheOnline(t[num], manche, d, taille, num);
        
        i++;
    }

    cout << "ça a pioche";
    for(int j = 0; j < nbj; j++)
    {
        if(t[j].humain) sendWholeTable(t, manche, 0, nbj, j);
    }
    
    cout << "ça a envoye";

    gambleOnline(t, nbj, manche); // On demande à chaque joueur combien de points il parie pour la manche

    joueOnline(d, taille, t, nbj, manche); // joue la manche
    attribution_point(t,nbj,manche); // donne les points

    for(int j = 0; j < nbj; j++)
    {
        if(t[j].humain) sendWholeTable(t, manche, 0, nbj, j);
    }

    delplit(t,nbj);
}

void joue(int nbj,int nbo) 
{
    srand(time(NULL));

    int manche = 1; //n° de manche 1-10
    int taille_d=70; // taille du deck
     
    deck d;
    d = new carte[70];
    remplirDeck(d);


    table t;
    t = new joueur[nbj+nbo];
    init_j(t,nbj);
    if (nbo>0) init_cpu(t,nbo,nbj);

    asio::io_context io_context;
    connexionClients(io_context, t, nbj);

    nbj = nbj + nbo; // Après avoir initialisé les ordis séparéments des joueurs on fusionne le nombre total de joueurs
    sendPlayerNumber(nbj, t);
    
    for(int i = 0; i < nbj; i++) sendInt(t[i].socket, i);

    while (manche<=10)
    {
        mancheOnline(t,manche,nbj,d,taille_d); // Debut manche
        manche++;
        if (manche<10)
        {
            cout << " === Manche suivante === " <<endl<<"====================================================================================================================================================="<<endl;
        }

    }
}

int main()
{
    int nbj = -1; //nbr de joueurs humains
    int nbo = -1; //nbr de joueurs ordis
    cout << "Combien de joueurs souhaitez-vous dans la partie ? (2,3,4,5,6) : ";
    cin >> nbj;
    
    while(nbj < 2 || nbj > 6)
    {
        cout << "Erreur -> inferieur a 6 ou superieur a 2 joueurs" << endl;
        cout << "Combien de joueurs souhaitez-vous dans la partie ? (2,3,4,5,6) : ";
        cin >> nbj;
    }

    if (6-nbj > 0) 
    {
        while(nbo < 0 || nbo > 6-nbj+1)
        {
            cout << "Combien d'ordis souhaitez-vous dans la partie ? (" << 6-nbj << " au maximum) : ";
            cin >> nbo;
        }
    }

    for (int i = 0; i < 10; i++)
    {
        cout<<"partie "<<i+1<<endl;
        joue(nbj,nbo);
    }
    
}