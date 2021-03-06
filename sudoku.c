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

/**

Fonction : main
Traite chaque option et redirige l'utilisateur vers la bonne fonction

**/

int main(int argv, char *argc[])
{
  char *paramT = valueFromArg(argv, argc, "t", "9x9");
  int paramH = valueExistInParam(argv, argc, "h");
  int paramG = valueExistInParam(argv, argc, "g");
  int paramS = valueExistInParam(argv, argc, "s");
  int paramU = valueExistInParam(argv, argc, "u");
  int paramR = valueExistInParam(argv, argc, "r");
  int tailleGrille = 9;
  int indexGrille = 0;
  int chargerGrille = 0;
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
        
        if(paramR > 0)
        {
            chargerGrille = 1;
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
            
            Tui *ui = ui_new(indexGrille, chargerGrille);
            Tjeu *jeu = NULL;
            ui_afficherGrille(ui, jeu);
            ui_free(ui);

        }
      }
    }
  }

  return 0;
}
