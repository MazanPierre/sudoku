#include <stdlib.h>
#include <stdio.h>

#include "pileSudoku.h"
#include "file.h"

void creeSudokuDB(char *filepath) {

  char *sudokuDB =
    "4x4 cbED EdcB DCBe bEDC\n"
    "4x4 bCde EdBC cBED DECb\n"
    "6x6 GfCEDb DBegCF eDGBfC bCFDGe CedFBG FgBceD\n"
    "6x6 fDeCBG gBCEdf bcGDFE DEFgCB CGbFEd eFdBgc\n"
    "8x8 CgCdeIHb eIBHfdGc FCHgIEBD DbEIGhCf BEicHFDG GhDFBCie IDGeCbFH HfcbDgEi\n"
    "8x8 IFHDbgcE gBECDFhi CHbFiDEg EIDGhcBf HcfiEBgD BDGeFhIC DGibCEfh fEcHGIdB\n"
    "9x9 hcdIEGbfj gBFdJcEHi iEJHbFGDc DhIgfeJcB BJecIhdGF CfGjdbIeH fGBEhJCId eIHbCdFJg jdcFGIhbe\n"
    "9x9 DChbfIjEG GBjedHFIC fEIJGcHbd BIgfHeDcj edCGiJBfh jhFdCbeGI hgEcBDIJf IFBHjgcDE CJdIefgHB";

  FILE* fichier = NULL;
  fichier = fopen(filepath, "w");

  fputs(sudokuDB, fichier);

  fclose(fichier);
}

PileSudoku *chargerSudokuDB(char *filepath) {

  FILE* fichier = NULL;
  PileSudoku *pileSudoku = pileSud_create();
  TsudokuData *sudokuActu = NULL;
  char chaine[64] = "";
  int tailleSud = 0;
  int indexSudoku = 0;
  int indexLigne = 0;
  int indexChar = 0;

  fichier = fopen(filepath, "r");

  if (fichier == NULL) {
    creeSudokuDB(filepath);
    fichier = fopen(filepath, "r");
  }

  if (fichier == NULL) {
    return NULL;
  } else {

    fscanf(fichier, "%s", chaine);

    while(feof(fichier) == 0){

      sudokuActu = malloc(sizeof(TsudokuData));
      pileSud_add(pileSudoku, sudokuActu);

      if(strcmp("4x4", chaine) == 0) {
        tailleSud = 4;
        sudokuActu->type = DB_TypeVal(4x4);
      } else if(strcmp("6x6", chaine) == 0) {
        tailleSud = 6;
        sudokuActu->type = DB_TypeVal(6x6);
      } else if(strcmp("8x8", chaine) == 0) {
        tailleSud = 8;
        sudokuActu->type = DB_TypeVal(8x8);
      } else if(strcmp("9x9", chaine) == 0) {
        tailleSud = 9;
        sudokuActu->type = DB_TypeVal(9x9);
      }

      sudokuActu->solu = malloc(sizeof(char) * tailleSud * tailleSud);
      sudokuActu->init = malloc(sizeof(char) * tailleSud * tailleSud);

      for(indexLigne = 0; indexLigne < tailleSud; indexLigne++) {
        fscanf(fichier, "%s", chaine);

        for(indexChar = 0; indexChar < tailleSud; indexChar++) {
          if(chaine[indexChar] >= 'a' && chaine[indexChar] <= 'z')
          {
            sudokuActu->solu[indexLigne * tailleSud + indexChar] = chaine[indexChar] - 'a' + 'A';
            sudokuActu->init[indexLigne * tailleSud + indexChar] = chaine[indexChar] - 'a' + 'A';
          }
          else
          {
            sudokuActu->solu[indexLigne * tailleSud + indexChar] = chaine[indexChar];
            sudokuActu->init[indexLigne * tailleSud + indexChar] = '.';
          }
        }
      }

      fscanf(fichier, "%s", chaine);
      indexSudoku++;
    }

    fclose(fichier);
  }

  return pileSudoku;
}

void dechargerSudoku(PileSudoku *pileSudoku)
{
  TsudokuData *sudokuData;
  int size = pileSud_count(pileSudoku);
  int i;

  for(i = 0; i < size; i++) {
    sudokuData = pileSud_get(pileSudoku, i);
    free(sudokuData);
  }

  pileSud_free(pileSudoku);
}
