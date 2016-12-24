#ifndef AFFICHAGE_H
#define AFFICHAGE_H

#include "TsudokuData.h"
#include "pileSudoku.h"

void assembleParagrapheEnColonne(char *output, char *p1, char *p2, int espacement);
void construireGrille(char *output, const char *grilleData, const char* grilleDataBase, int tailleGrille, int unicode);
void afficheAide();
void afficheListeGrille(int tailleGrille, int solution, int unicode);


#endif // AFFICHAGE_H
