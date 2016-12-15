#include <stdlib.h>

#include "pileCoup.h"

PileCoup *pileCoup_create()
{
  PileCoup *pileCoup = malloc(sizeof(PileCoup));

  pileCoup->dernierCoup = NULL;

  return pileCoup;
}

/// Ajoute un Elément à la fin de la liste
void pileCoup_empile(PileCoup *pileCoup, char abscisse, char ordonnee, char valeur)
{
  ElementCoup *temp = malloc(sizeof(ElementCoup));

  temp->abscisse = abscisse;
  temp->ordonnee = ordonnee;
  temp->valeur = valeur;
  temp->coupPrecedent = pileCoup->dernierCoup;

  pileCoup->dernierCoup = temp;
}

void pileCoup_free(PileCoup *pileCoup)
{
  ElementCoup *elem = pileCoup->dernierCoup;
  ElementCoup *tmp = NULL;

  while(elem != NULL) {
    tmp = elem->coupPrecedent;
    free(elem);
    elem = tmp;
  }
}

ElementCoup pileCoup_depile(PileCoup *pileCoup)
{
  ElementCoup *avantDernier = pileCoup->dernierCoup->coupPrecedent;
  ElementCoup dernierCoup;

  dernierCoup.abscisse = pileCoup->dernierCoup->abscisse;
  dernierCoup.ordonnee = pileCoup->dernierCoup->ordonnee;
  dernierCoup.valeur = pileCoup->dernierCoup->valeur;
  dernierCoup.coupPrecedent = NULL;

  free(pileCoup->dernierCoup);
  pileCoup->dernierCoup = avantDernier;

  return dernierCoup;
}

int pileCoup_count(PileCoup *pileCoup)
{
  ElementCoup *elem = pileCoup->dernierCoup;
  int count = 0;

  while(elem != NULL) {
    elem = elem->coupPrecedent;
    count++;
  }

  return count;
}

int pileCoup_vide(PileCoup *pileCoup)
{
  if(pileCoup->dernierCoup == NULL)
    return 1;

  return 0;
}
