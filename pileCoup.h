#ifndef PILECOUP_H
#define PILECOUP_H

/** Historique pour stocker les coup pr�c�dents
*   Liste sous forme de structure r�cursive **/
typedef struct _ElementCoup{
     char abscisse;
     char ordonnee;
     char valeur;

     struct _ElementCoup *coupPrecedent;
} ElementCoup;

typedef struct _PileCoup{
     struct _ElementCoup *dernierCoup;
} PileCoup;

PileCoup *pileCoup_create();
void pileCoup_empile(PileCoup *pileCoup, char abscisse, char ordonnee, char coupJouee);
void pileCoup_free(PileCoup *pileCoup);
ElementCoup pileCoup_depile(PileCoup *pileCoup);
int pileCoup_count(PileCoup *pileCoup);
int pileCoup_vide(PileCoup *pileCoup);

#endif // PILECOUP_H
