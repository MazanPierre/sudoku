#ifndef PILESUDOKU_H
#define PILESUDOKU_H

#include "TsudokuData.h"


typedef struct _ElementSudoku
{
  TsudokuData *sud;
  struct _ElementSudoku *next;
} ElementSudoku;

typedef struct _PileSudoku
{
  struct _ElementSudoku *elem;
} PileSudoku;

PileSudoku *pileSud_create();
TsudokuData *pileSud_get(PileSudoku *pileSudoku, int index);
void pileSud_add(PileSudoku *pileSudoku, TsudokuData *sudokuData);
void pileSud_reset(PileSudoku *pileSudoku);
void pileSud_free(PileSudoku *pileSudoku);
int pileSud_count(PileSudoku *pileSudoku);
PileSudoku *pileSud_getListSudFromType(PileSudoku *pileSudoku, DB_TypeTyp type);

#endif
