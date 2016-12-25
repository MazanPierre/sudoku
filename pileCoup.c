#include <stdio.h>
#include <stdlib.h>

#include "pileCoup.h"

PileCoup *pileCoup_create()
{
  PileCoup *pileCoup = malloc(sizeof(PileCoup));

  pileCoup->dernierCoup = NULL;

  return pileCoup;
}

/// Ajoute un Elément à la fin de la liste
void pileCoup_empile(PileCoup *pileCoup, char abscisse, char ordonnee, char valeurPrecedente, char valeurSuivante)
{
  ElementCoup *temp = malloc(sizeof(ElementCoup));

  temp->abscisse = abscisse;
  temp->ordonnee = ordonnee;
  temp->valeurPrecedente = valeurPrecedente;
  temp->valeurSuivante = valeurSuivante;
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
  dernierCoup.valeurPrecedente = pileCoup->dernierCoup->valeurPrecedente;
  dernierCoup.valeurSuivante = pileCoup->dernierCoup->valeurSuivante;
  dernierCoup.coupPrecedent = NULL;

  free(pileCoup->dernierCoup);
  pileCoup->dernierCoup = avantDernier;

  return dernierCoup;
}

ElementCoup pileCoup_get(PileCoup *pileCoup, int index)
{
  ElementCoup *elem = pileCoup->dernierCoup;
  ElementCoup resultat;
  int indexInc = 0;

  for(indexInc = 0; indexInc < index; indexInc++)
  {
    elem = elem->coupPrecedent;
  }

  resultat.abscisse = elem->abscisse;
  resultat.ordonnee = elem->ordonnee;
  resultat.valeurPrecedente = elem->valeurPrecedente;
  resultat.valeurSuivante = elem->valeurSuivante;
  resultat.coupPrecedent = NULL;

  return resultat;
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

void pileCoup_reset(PileCoup *pileCoup)
{
    ElementCoup *elem = pileCoup->dernierCoup;
    ElementCoup *tmp = NULL;
    pileCoup->dernierCoup = NULL;

    while(elem != NULL) {
        tmp = elem->coupPrecedent;
        free(elem);
        elem = tmp;
    }
}

/** Cette procédure sauvegarde les données de la partie dans un fichier texte **/
/** Si le fichier n'existe pas, il est créé automatiquement **/
/** Prend en paramètre la pile qui contient l'historique des coups précédents (historique)
	ainsi que le nombre de coups restants **/
int pileCoup_sauvegarder(PileCoup *historique, int nbCoups, int tailleGrille, int indexGrille){

	FILE* fichier = NULL;
	ElementCoup element;
	int nbCaseHistorique = pileCoup_count(historique);
	int i = 0;

	fichier = fopen("./sauvegarde.txt", "w");
	if (fichier != NULL)
	{
		/// Enregistrement du nombre de coups utilisés
		/// Enregistrement de la taille de la grille
        /// Enregistrement de l'indexe pour le chargement de la grille
		fprintf(fichier, "%d %d %d \n", nbCoups, tailleGrille, indexGrille); 

		/// Enregistrement de la pile
		for(i = nbCaseHistorique - 1; i >= 0; i--) {
			element = pileCoup_get(historique, i);

			fprintf(fichier, "%d %d %c %c \n", element.abscisse, element.ordonnee, element.valeurPrecedente, element.valeurSuivante);
		}

        fclose(fichier);
        return 1;
	}
    
    return 0;
}

/** Cette procédure charge les données d'une partie sauvegardée depuis un fichier texte **/
/** Prend en paramètre la pile qui contient l'historique des coups précédents (historique)
	ainsi que le nombre de coups restants, ces variables contiendrons les nouvelles valeurs chargées **/
int pileCoup_charger(PileCoup *historique, int *nbCoups, int *tailleGrille, int *indexGrille) {

	FILE* fichier = NULL;
	ElementCoup element;

    fichier = fopen("sauvegarde.txt", "r");
 	int fin = 1;

    if (fichier != NULL)
    {
		/// Chargement du nombre de coups restants (1er élément dans le fichier de sauvegarde
        /// Chargement de la taille de la grille (2ème élément dans le fichier de sauvegarde
        /// Chargement de l'indexe du sudoku (3ème élément dans le fichier de sauvegarde
        fscanf(fichier, "%d %d %d \n", nbCoups, tailleGrille, indexGrille); 

        ///Boucle de lecture des caractères
        do
        {
			fscanf(fichier, "%d %d %c %c \n", &element.abscisse, &element.ordonnee, &element.valeurPrecedente, &element.valeurSuivante);
            
			/// Chargement d'un coup
			pileCoup_empile(historique, element.abscisse, element.ordonnee, element.valeurPrecedente, element.valeurSuivante);

			if(feof(fichier))
				fin = 0;
        } while (fin); /// Continue tant que fgetc n'a pas retourné EOF (fin de fichier)

        fclose(fichier);
        
        return 1;
    }
    
    return 0;
}
