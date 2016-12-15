#include <stdlib.h>

#include "pileSudoku.h"

PileSudoku *pileSud_create() {
  PileSudoku *listSud = malloc(sizeof(PileSudoku));

  listSud->elem = NULL;

  return listSud;
}

TsudokuData *pileSud_get(PileSudoku *pileSudoku, int index)
{
  ElementSudoku *elem = pileSudoku->elem;
  int size = pileSud_count(pileSudoku);
  int inverseIndex = size - index - 1;
  int i = 0;

  while(i < inverseIndex)
  {
    if(elem == NULL) {
      return NULL;
    }

    elem = elem->next;
    i++;
  }

  if(elem != NULL)
    return elem->sud;

  return NULL;
}

void pileSud_add(PileSudoku *pileSudoku, TsudokuData *sudokuData) {
  ElementSudoku *newSudoku = malloc(sizeof(ElementSudoku));
  newSudoku->next = pileSudoku->elem;
  newSudoku->sud = sudokuData;

  pileSudoku->elem = newSudoku;
}

void pileSud_reset(PileSudoku *pileSudoku) {
  ElementSudoku *elem = pileSudoku->elem;
  ElementSudoku *tmp = NULL;

  while(elem != NULL) {
    tmp = elem->next;
    free(elem);
    elem = tmp;
  }

  free(pileSudoku);
}

void pileSud_free(PileSudoku *pileSudoku) {
  pileSud_reset(pileSudoku);
  free(pileSudoku);
}

int pileSud_count(PileSudoku *pileSudoku) {
  ElementSudoku *elem = pileSudoku->elem;
  int result = 0;

  while(elem != NULL) {
    elem = elem->next;
    result++;
  }

  return result;
}

PileSudoku *pileSud_getListSudFromType(PileSudoku *pileSudoku, DB_TypeTyp type) {
  PileSudoku *result = pileSud_create();

  ElementSudoku *elem = pileSudoku->elem;

  while(elem != NULL) {
    if(*(elem->sud->type) == *type) {
      pileSud_add(result, elem->sud);
    }

    elem = elem->next;
  }

  return result;
}
