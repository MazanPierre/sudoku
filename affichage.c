#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "file.h"
#include "conversion.h"

#include "TsudokuData.h"
#include "pileSudoku.h"

/*
Fonction : assembleParagrapheEnColonne
Assemble deux textes, l'un à coté de l'autre
Permet à deux grille de sudoku de se situer sur les mêmes lignes
*/

void assembleParagrapheEnColonne(char *output, char *p1, char *p2, int espacement)
{
  int maxLargeur = 0;
  int largeurActu = 0;
  int nbCharP1 = strlen(p1);
  int nbCharP2 = strlen(p2);
  int i = 0;
  int endLine = 0;
  int offsetP1 = 0;
  int offsetP2 = 0;
  int offsetOutput = 0;

  while(i < nbCharP1)
  {
    if(p1[i] == '\n')
    {
      if(maxLargeur < largeurActu) {
        maxLargeur = largeurActu;
      }

      largeurActu = 0;
    }
    else
    {
      largeurActu++;
    }

    i++;
  }

  while(offsetP1 < nbCharP1)
  {
    endLine = 0;

    for(i = 0; i < maxLargeur; i++) {

      if(endLine) {
        //output[offsetOutput] = '-';
        //offsetOutput++;
      } else {
        if(p1[offsetP1] == '\n') {
          endLine = 1;
        } else {
          output[offsetOutput] = p1[offsetP1];
          offsetOutput++;
          offsetP1++;
        }
      }
    }

    offsetP1++;

    for(i = 0; i < espacement; i++) {
      output[offsetOutput] = ' ';
      offsetOutput++;
    }

    while(offsetP2 < nbCharP2 && p2[offsetP2] != '\n')
    {
      output[offsetOutput] = p2[offsetP2];

      offsetOutput++;
      offsetP2++;
    }

    output[offsetOutput] = '\n';
    offsetOutput++;
    offsetP2++;
  }

  output[offsetOutput] = '\0';
}

/*
Fonction : construireGrille
construit la grille dans une variable de sortie pour pouvoir assemblé les grilles en colonne, ultérieurement
*/
void construireGrille(char *output, const char *grilleData, const char* grilleDataBase, int tailleGrille, int unicode)
{
  int cubeLargeur = sqrt(tailleGrille);
  int cubeHauteur = tailleGrille / cubeLargeur;
  int x = 0;
  int y = 0;

  output[0] = '\0';

  for(x = 0; x < tailleGrille; x++)
  {
    // Affiche bordure horizontal (haut, interne, droite)
    strcat(output, "   ");
    for(y = 0; y < tailleGrille; y++)
    {
      if(unicode) {
        if(x == 0 && y == 0) // haut && droit
          strcat(output, "╔═══");
        else if(x == 0 && y % cubeHauteur == 0) // haut
          strcat(output, "╦═══");
        else if(x % cubeLargeur == 0 && y == 0) // droit
          strcat(output, "╠═══");
        else if(x % cubeLargeur == 0 && y % cubeHauteur == 0) // croisement double ligne
          strcat(output, "╬═══");
        else if(x % cubeLargeur == 0) // croisement ligne
          strcat(output, "════");
        else if(y % cubeHauteur == 0) // interne && croisement vertical
          strcat(output, "║───");
        else                // normal
          strcat(output, "┼───");
      } else {
        if(x == 0 && y == 0) // haut && droit
          strcat(output, " ___");
        else if(x == 0 && y % cubeHauteur == 0) // haut
          strcat(output, "____");
        else if(x % cubeLargeur == 0 && y == 0) // droit
          strcat(output, "|___");
        else if(x % cubeLargeur == 0 && y % cubeHauteur == 0) // croisement double ligne
          strcat(output, "|___");
        else if(x % cubeLargeur == 0) // croisement ligne
          strcat(output, "____");
        else if(y % cubeHauteur == 0) // interne && croisement vertical
          strcat(output, "|...");
        else                // normal
          strcat(output, "....");
      }
    }

    // Affiche bordure vertical (gauche)
    if(unicode) {
      if(x == 0)
        strcat(output, "╗");
      else if(x % cubeLargeur == 0)
        strcat(output, "╣");
      else
        strcat(output, "║");
    } else {
      if(x == 0)
        strcat(output, " ");
      else if(x % cubeLargeur == 0)
        strcat(output, "|");
      else
        strcat(output, "|");
    }

    // Affiche le numero de la ligne
    sprintf(output, "%s\n %c ", output, '0' + tailleGrille - x);

    for(y = 0; y < tailleGrille; y++)
    {
      // Affiche bordure vertical (droite, interne)
      if(unicode) {
        if(y % cubeHauteur == 0)
          strcat(output, "║");
        else
          strcat(output, "│");
      } else {
        if(y % cubeHauteur == 0)
          strcat(output, "|");
        else
          strcat(output, ":");
      }

      // Affiche numero dans la case
      if(grilleDataBase[x * tailleGrille + y] == '.') {
        if(grilleData[x * tailleGrille + y] == '.')
          sprintf(output, "%s   ", output);
        else
          sprintf(output, "%s %c ", output, grilleData[x * tailleGrille + y]);
      } else {
        sprintf(output, "%s(%c)", output, grilleData[x * tailleGrille + y]);
      }
    }
    // Affiche bordure vertical (gauche)
    if(unicode) {
      strcat(output, "║\n");
    } else {
      strcat(output, "|\n");
    }
  }


  // Affiche la bar en bas de la grille
  if(unicode)
    strcat(output, "   ╚═══");
  else
    strcat(output, "   |___");

  for(y = 1; y < tailleGrille; y++)
  {
    if(unicode) {
      if(y % cubeHauteur == 0)
        strcat(output, "╩═══");
      else
        strcat(output, "════");
    } else {
      if(y % cubeHauteur == 0)
        strcat(output, "|___");
      else
        strcat(output, "____");
    }
  }

  if(unicode) {
    strcat(output, "╝\n   ");
  } else {
    strcat(output, "|\n   ");
  }

  for(y = 0; y < tailleGrille; y++)
  {
    sprintf(output, "%s  %c ", output, y + 'A');
  }
  strcat(output, " \n");
}

/*
Fonction : Affiche titre
Affiche le titre dans un cadre formatté
*/

void afficherTitre(const char *titre) {
  int i = 0;
  int tailleCadre = 100;
  int tailleTitre = strlen(titre);
  int nbEspace = tailleCadre / 2 - tailleTitre / 2;

  if(tailleTitre % 2)
    tailleCadre++;

  printf("\n\n\n+");
  for(i = 0; i < tailleCadre; i++) {
    printf("-");
  }
  printf("+\n|");

  for(i = 0; i < nbEspace; i++) {
    printf(" ");
  }
  printf(titre);
  for(i = 0; i < nbEspace; i++) {
    printf(" ");
  }
  printf("|\n+");

  for(i = 0; i < tailleCadre; i++) {
    printf("-");
  }
  printf("+\n\n");
}

/*
Fonction : afficheAide
Affiche une aide écrite des options que l'utilisateurs peut utiliser
*/

void afficheAide() {
  afficherTitre("Options du Sudoku");
  printf("\t-h Affiche sur le flux standard de sortie une aide et s'arrete.\n\n");
  printf("\t-g Ecrit sur le flux standard de sortie les grilles disponibles et se termine.\n\n");
  printf("\t-s Ecrit sur le flux standard de sortie les grilles remplies disponibles et se termine.\n\n");
  printf("\t-t <taille de grille>\n");
  printf("\t\t Choix de taille de la grille : 9x9, 8x8, 6x6, 4x4, rand\n");
  printf("\t\t Lors d'une partie, la valeur \"rand\" permet de choisir une grille aux hasard\n");
  printf("\t\t Lorsque l'option -s ou -g est utilisee, alors toutes les grilles s'affichent\n");
  printf("\t\t Si l'argument est omis alors la grille par defaut est 9x9\n\n");
  printf("\t-u Permet un affichage des grilles de sudoku plus esthetique.\n");
  printf("\t\t Attention : Si cette option est ajoutee alors les grilles de Sudoku utiliseront des\n");
  printf("\t\t caractere unicode. Ce type de caractere n'est pas supporter par tout les terminaux.\n\n");
  printf("\n\n");

}


/*
Fonction : afficheListeGrille
Affiche les grilles avec des cases vide ou compléte selon l'option -g ou -s
*/

void afficheListeGrille(int tailleGrille, int solution, int unicode)
{
  PileSudoku *pileSudoku = NULL;
  PileSudoku *pileToDisplay = NULL;
  TsudokuData *sudokuData = NULL;
  int nbSudokuToDisplay = 0;
  int indexSud = 0;
  int tailleSudoku = 0;
  int tailleSudokuPre = 0;
  char screenTestOut[4096] = "";
  char screenTestP1[4096] = "";
  char screenTestP2[4096] = "";
  char titre[64];
  DB_TypeTyp type = 0;

  pileSudoku = chargerSudokuDB("sudoku-db.dat");
  type = conversion_IntToDBType(tailleGrille);

  if(type == 0)
    pileToDisplay = pileSudoku;
  else
    pileToDisplay = pileSud_getListSudFromType(pileSudoku, type);

  nbSudokuToDisplay = pileSud_count(pileToDisplay);

  for(indexSud = 0; indexSud < nbSudokuToDisplay; indexSud++) {
    sudokuData = pileSud_get(pileToDisplay, indexSud);
    tailleSudoku = conversion_DBTypeToInt(sudokuData->type);

    if(tailleSudokuPre != tailleSudoku) {
      printf(screenTestOut);
      strcpy(screenTestOut, "");
      strcpy(screenTestP1, "");
      strcpy(screenTestP2, "");

      sprintf(titre, "Grille de taille %d", tailleSudoku);
      afficherTitre(titre);
    }
    else
    {
      strcpy(screenTestP2, screenTestOut);
    }

    if(solution) {
      construireGrille(screenTestP1, sudokuData->solu, sudokuData->init, tailleSudoku, unicode);
    } else {
      construireGrille(screenTestP1, sudokuData->init, sudokuData->init, tailleSudoku, unicode);
    }

    assembleParagrapheEnColonne(screenTestOut, screenTestP1, screenTestP2, 5);

    tailleSudokuPre = tailleSudoku;
  }

  printf(screenTestOut);
  printf("\n");

  if(pileToDisplay != pileSudoku)
    pileSud_free(pileToDisplay);

  dechargerSudoku(pileSudoku);
}
