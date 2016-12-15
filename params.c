#include <stdlib.h>
#include <string.h>

#include "params.h"

/*
Fonction : valueFromArg
Verifie si une option est pr�cis� au moins une fois
Si elle n'est pas pr�ciser alors la valeur par defaut pass� en param�tre est renvoy�
Si elle est pr�ciser plusieurs fois alors la fonction renvoie nil
*/

char *valueFromArg(int argv, char *argc[], char *param, char* def)
{
  int i = 0;
  char *result = def;
  char *paramWithSymbole = malloc(sizeof(char) * (strlen(param) + 1));
  paramWithSymbole[0] = '-';
  paramWithSymbole[1] = '\0';
  strcat(paramWithSymbole, param);

  for(i = 0; i < argv; i++) {
    if(strcmp(argc[i], paramWithSymbole) == 0)
    {
      if(result == def) {
        if(i + 1 < argv) {
          result = argc[i + 1];
        }
      } else {
        return NULL;
      }
    }
  }

  free(paramWithSymbole);
  return result;
}

/*
Fonction : valueExistInParam
Compte le nombre de fois qu'une option est pr�cis�
*/

int valueExistInParam(int argv, char *argc[], char *param)
{
  int i = 0;
  int result = 0;
  char *paramWithSymbole = malloc(sizeof(char) * (strlen(param) + 1));
  paramWithSymbole[0] = '-';
  paramWithSymbole[1] = '\0';
  strcat(paramWithSymbole, param);

  for(i = 0; i < argv; i++) {
    if(strcmp(argc[i], paramWithSymbole) == 0) {
      result++;
    }
  }

  free(paramWithSymbole);
  return result;
}
