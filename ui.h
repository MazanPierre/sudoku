#ifndef UI_H
#define UI_H

#include "TsudokuData.h"

typedef struct _Tui Tui;
typedef struct _Tjeu Tjeu;

extern Tui *ui_new(int indexGrille, int chargerGrille);
extern void ui_free(Tui *ui);
extern void ui_afficherGrille(Tui *ui, Tjeu *jeu);

#endif