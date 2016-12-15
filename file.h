#ifndef FILE_H
#define FILE_H

#include "pileSudoku.h"

void creeSudokuDB(char *filepath);
PileSudoku *chargerSudokuDB(char *filepath);
void dechargerSudoku(PileSudoku *pileSudoku);

#endif
