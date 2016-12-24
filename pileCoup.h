#ifndef PILECOUP_H
#define PILECOUP_H

/** Historique pour stocker les coup précédents
*   Liste sous forme de structure récursive **/
typedef struct _ElementCoup{
     char abscisse;
     char ordonnee;
     char valeurPrecedente;
     char valeurSuivante;

     struct _ElementCoup *coupPrecedent;
} ElementCoup;

typedef struct _PileCoup{
     struct _ElementCoup *dernierCoup;
} PileCoup;

PileCoup *pileCoup_create();
void pileCoup_empile(PileCoup *pileCoup, char abscisse, char ordonnee, char valeurPrecedente, char valeurSuivante);
void pileCoup_free(PileCoup *pileCoup);
ElementCoup pileCoup_depile(PileCoup *pileCoup);
ElementCoup pileCoup_get(PileCoup *pileCoup, int index);
int pileCoup_count(PileCoup *pileCoup);
int pileCoup_vide(PileCoup *pileCoup);
void pileCoup_reset(PileCoup *pileCoup);
int sauvegarder(PileCoup *pileCoup, int nbCoups, int tailleGrille, int indexGrille);
int charger(PileCoup *pileCoup, int *nbCoups, int *tailleGrille, int *indexGrille);

#endif // PILECOUP_H
