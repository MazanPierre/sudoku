#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ui.h"
#include "TsudokuData.h"
#include "pileCoup.h"
#include "pileSudoku.h"
#include "affichage.h"

struct _Tui {
  PileSudoku *pileSudoku;
  int indexGrille;
};

struct _Tjeu {
};

Tui *ui_new(int indexGrille)
{
  Tui *ui = malloc(sizeof(Tui));
  
  ui->pileSudoku = chargerSudokuDB("sudoku-db.dat");
  ui->indexGrille = indexGrille;
  
  return ui;
}

void ui_free(Tui *ui)
{
  dechargerSudoku(ui->pileSudoku);
  free(ui);
}

void ui_afficherGrille(Tui *ui, Tjeu *jeu)
{
    TsudokuData *sudokuData = pileSud_get(ui->pileSudoku, ui->indexGrille);
	char screenOutput[2048] = "";
	char titre[92] = "";
	char grilleData[81] = "";
	char lettreX[16] = "A";
	char lettreY[2] = "1";
	char lettreV[2] = "A";
	int x = 0;
	int y = 0;
	int v = 0;
    int i = 0;
    
    PileCoup *pileCoup = pileCoup_create();
    int tailleGrille = conversion_DBTypeToInt(sudokuData->type);
    int nbPropositionJoue = 0;
    int nbPropositionAutorise = (int)(compteurCaseVide(sudokuData->init, tailleGrille) * 2);
    int unicode = 0;
    int erreurProposition = 0;
    int nbErreur = 0;
    int continuer = 1;


    strcpy(grilleData, sudokuData->init);

    while(continuer) {
        screenOutput[0] = '\0';

        sprintf(titre, "Sudoku de taille %d - Coups : %d / %d", tailleGrille, nbPropositionJoue, nbPropositionAutorise);
        afficherTitre(titre);
        construireGrille(screenOutput, grilleData, sudokuData->init, tailleGrille, unicode);
        printf(screenOutput);

        printf("\n");
        printf("X est une lettre comprise entre A et %c (abscisse)\n", 'A' + tailleGrille - 1);
        printf("Y un chiffre compris entre 1 et %c (ordonnee)\n", '0' + tailleGrille);
        printf("V une lettre compris entre A et %c (valeur)\n\n", tailleGrille + 'A' - 1);

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
              grilleData[elementCoup.abscisse + elementCoup.ordonnee * tailleGrille] = elementCoup.valeurPrecedente;
            }
          }

          /// SAUVEGARDE
          else if(strcmp(lettreX, "!s") == 0){
            sauvegarder(pileCoup, nbPropositionJoue, tailleGrille, ui->indexGrille);
            printf("\n------------------------    SAUVEGARDE REUSSIE    ------------------------");
          }

          /// CHARGEMENT
          else if(strcmp(lettreX, "!c") == 0){
            pileCoup_reset(pileCoup);
            charger(pileCoup, &nbPropositionJoue, &tailleGrille, &(ui->indexGrille));
            nbPropositionJoue--;

            sudokuData = pileSud_get(ui->pileSudoku, ui->indexGrille);
            
            strcpy(grilleData, sudokuData->init);

            ///Mise à jour de la grille avec les données de la pile chargée
            ElementCoup elementTemp;
            int nbCaseHistorique = pileCoup_count(pileCoup);

            for(i = nbCaseHistorique - 1; i >= 0; i--) {
                elementTemp = pileCoup_get(pileCoup, i);

                grilleData[elementTemp.abscisse + elementTemp.ordonnee * tailleGrille] = elementTemp.valeurSuivante;
            }

            printf("----------------     CHARGEMENT REUSSI     ----------------");

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
              if(sudokuData->init[x + y * tailleGrille] == '.') {
                pileCoup_empile(pileCoup, x, y, grilleData[x + y * tailleGrille], v);
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

