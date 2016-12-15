#include <stdio.h>
#include "ui.h"
#include "TsudokuData.h"
#include "pileCoup.h"

struct _Tui {

};

struct _Tjeu {
  
};

Tui *ui_new()
{
	return NULL;
}

void ui_free(Tui *ui)
{

}

void ui_afficherGrille(Tui *ui, const Tjeu *jeu)
{
  PileCoup *pileCoup = pileCoup_create();
  TsudokuData sudokuData;
	char screenOutput[2048] = "";
	char titre[92] = "";
	char grilleData[81] = "";
	char lettreX[2] = "A";
	char lettreY[2] = "1";
	char lettreV[2] = "A";
	int x = 0;
	int y = 0;
	int v = 0;

  int tailleGrille = 9;
  int nbPropositionJoue = 0;
  int nbPropositionAutorise = (int)(compteurCaseVide(sudokuData.init, tailleGrille) * 2);
  int unicode = 0;
  int erreurProposition = 0;
  int nbErreur = 0;
  int continuer = 1;


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