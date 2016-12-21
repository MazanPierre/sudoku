#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "affichage.h"
#include "file.h"
#include "pileCoup.h"
#include "conversion.h"
#include "params.h"
#include "compteur.h"
#include "ui.h"

/*
Fonction : jouerGrille
Affiche le sudoku et demande les cases à remplir et verifie si le joueurs à gagner ou perdu
*/

void jouerGrille(TsudokuData sudokuData, int tailleGrille, int unicode)
{
  PileCoup *pileCoup = pileCoup_create();
  char screenOutput[2048] = "";
  char titre[92] = "";
  char grilleData[81] = "";
  char lettreX[2] = "A";
  char lettreY[2] = "1";
  char lettreV[2] = "A";
  int x = 0;
  int y = 0;
  int v = 0;
  int continuer = 1;
  int erreurProposition = 0;
  int nbPropositionAutorise = (int)(compteurCaseVide(sudokuData.init, tailleGrille) * 2);
  int nbPropositionJoue = 0;
  int nbErreur = 0;

  strcpy(grilleData, sudokuData.init);

  while(continuer) {
    screenOutput[0] = '\0';

    sprintf(titre, "Sudoku de taille %d - Coups : %d / %d", tailleGrille, nbPropositionJoue, nbPropositionAutorise);
    afficherTitre(titre);
    construireGrille(screenOutput, grilleData, sudokuData.init, tailleGrille, unicode);
    printf(screenOutput);

    printf("\n");
    printf("X est une lettre de A : %c\n", 'A' + tailleGrille - 1);
    printf("Y un chiffre de 1 : %c\n", '0' + tailleGrille);
    printf("V une lettre de A : %c\n\n", tailleGrille + 'A' - 1);

    do {

      erreurProposition = 0;

      printf("Entrez une lettre au format \"X Y Z\" : ");
      scanf("%s", lettreX);

      if(lettreX[0] == '<')
      {
        if(pileCoup_vide(pileCoup))
        {
          printf("Vous n'avez pas jouer de precedent coup");
        }
        else
        {
          ElementCoup elementCoup = pileCoup_depile(pileCoup);
          grilleData[elementCoup.abscisse + elementCoup.ordonnee * tailleGrille] = elementCoup.valeur;
        }
      }
      else
      {
        scanf("%s %s", lettreY, lettreV);

        x = lettreX[0] - 'A';
        y = tailleGrille - (lettreY[0] - '0');
        v = lettreV[0];

        if(x >= tailleGrille || x < 0) {
          erreurProposition = 1;
          printf("X est invalide!\n");
        }
        if(y >= tailleGrille || y < 0) {
          erreurProposition = 1;
          printf("Y est invalide!\n");
        }
        if(v > 'Z' || v < 'A') {
          erreurProposition = 1;
          printf("V est invalide!\n");
        }


        if(!erreurProposition) {
          if(sudokuData.init[x + y * tailleGrille] == '.') {
            pileCoup_empile(pileCoup, x, y, grilleData[x + y * tailleGrille]);
            grilleData[x + y * tailleGrille] = v;
          } else {
            printf("/!\\ Vous ne pouvez pas modifier cette case!\n");
            erreurProposition = 1;
          }
        }
      }

    } while(erreurProposition);

    printf("\n\n");

    nbPropositionJoue++;

    if(compteurCaseVide(grilleData, tailleGrille) == 0) {
      afficherTitre("Fin de la partie");
      printf("Vous avez remplie toutes la grille\n");
      nbErreur = nombreErreur(sudokuData, grilleData, tailleGrille);

      if(nbErreur > 0)
        printf("Vous avez fait %d erreur(s)\n", nbErreur);
      else
        printf("Vous avez reussi a terminer sans erreur\n");

      continuer = 0;
    }
    else if(nbPropositionJoue >= nbPropositionAutorise)
    {
      afficherTitre("Fin de la partie");
      printf("Vous avez depasse le nombre de proposition maximal autorise.\n");
      continuer = 0;
    }

  }

  printf("\n\n");
}




/*

Fonction : main
Traite chaque option et redirige l'utilisateur vers la bonne fonction

*/

int main(int argv, char *argc[])
{
  PileSudoku *pileSudoku = NULL;
  TsudokuData *sudokuData = NULL;
  char *paramT = valueFromArg(argv, argc, "t", "9x9");
  int paramH = valueExistInParam(argv, argc, "h");
  int paramG = valueExistInParam(argv, argc, "g");
  int paramS = valueExistInParam(argv, argc, "s");
  int paramU = valueExistInParam(argv, argc, "u");
  int tailleGrille = 9;
  int indexGrille = 0;
  int unicode = 0;
  int i;

  srand(time(0));

  if(paramH > 0)
  {
    afficheAide();
  }
  else
  {
    if(paramT == NULL)
    {
      printf("Erreur: Vous ne devez preciser qu'une et une seul taille de grille!\n\n");
    }
    else
    {
      if(strcmp(paramT, "9x9") == 0) tailleGrille = 9;
      else if(strcmp(paramT, "8x8") == 0) tailleGrille = 8;
      else if(strcmp(paramT, "6x6") == 0) tailleGrille = 6;
      else if(strcmp(paramT, "4x4") == 0) tailleGrille = 4;
      else if(strcmp(paramT, "rand") == 0) tailleGrille = -1;
      else tailleGrille = 0;

      if(tailleGrille == 0)
      {
        printf("Taille de grille invalide\n\n");
      }
      else
      {
        if(paramU > 0)
        {
          unicode = 1;
        }

        if(paramG > 0)
        {
          afficheListeGrille(tailleGrille, 0, unicode);
        }
        else if(paramS > 0)
        {
          afficheListeGrille(tailleGrille, 1, unicode);
        }
        else
        {
          pileSudoku = chargerSudokuDB("sudoku-db.dat");

          if(pileSudoku == NULL)
          {
            printf("Erreur lors de la lecture du fichier");
          }
          else
          {
            if(tailleGrille == 9) {
              indexGrille = 6 + rand() % 2;
            } else if(tailleGrille == 8) {
              indexGrille = 4 + rand() % 2;
            } else if(tailleGrille == 6) {
              indexGrille = 2 + rand() % 2;
            } else if(tailleGrille == 4) {
              indexGrille = 0 + rand() % 2;
            } else if(tailleGrille == -1){
              indexGrille = rand() % 8;
            }

            sudokuData = pileSud_get(pileSudoku, indexGrille);

            jouerGrille(*sudokuData, tailleGrille, unicode);

            dechargerSudoku(pileSudoku);

          }
        }
      }
    }
  }

  return 0;
}
