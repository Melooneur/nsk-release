#pragma once
#include "joueur.h"

void init_cpu(table & t, int nb, int nbj);
void gambleCPU(table &t, int nbj, int manche, int i);
float puissanceDeck(deck mainJoueur, int nbc);
float puissanceTable(table t, int nbj);
int joueCPU(deck &d, int joueurActif, int &taille, table &t, int manche, int i, int nbj);
