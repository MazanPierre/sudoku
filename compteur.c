#include "TsudokuData.h"

/*
Fonction : compteurCaseVide
Compte le nombre de case incompléte dans une grille de sudoku
*/

int compteurCaseVide(const char *grilleData, int tailleGrille)
{
  int i = 0;
  int nbCaseTotal = tailleGrille * tailleGrille;
  int compteurVide = 0;

  for(i = 0; i < nbCaseTotal; i++)
  {
    if(grilleData[i] == '.') {
      compteurVide++;
    }
  }

  return compteurVide;
}

/*
Fonction : nombreErreur
Compte le nombre d'erreur d'un sudoku rempli par rapport à sa solution
*/

int nombreErreur(TsudokuData sudokuData, char *grilleData, int tailleGrille)
{
  int nbCase = tailleGrille * tailleGrille;
  int compteur = 0;
  int i = 0;

  for(i = 0; i < nbCase; i++) {

    if(sudokuData.solu[i] != grilleData[i]) {
      compteur++;
    }
  }

  return compteur;
}
