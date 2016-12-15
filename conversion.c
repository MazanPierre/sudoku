#include "conversion.h"

/*
Fonction : conversion_DBTypeToInt
*/

int conversion_DBTypeToInt(DB_TypeTyp type) {
  int result = -1;

  if(*type == *(DB_TypeVal(4x4))) {
    result = 4;
  } else if(*type == *(DB_TypeVal(6x6))) {
    result = 6;
  } else if(*type == *(DB_TypeVal(8x8))) {
    result = 8;
  } else if(*type == *(DB_TypeVal(9x9))) {
    result = 9;
  }

  return result;
}

/*
Fonction : conversion_IntToDBType

*/

DB_TypeTyp conversion_IntToDBType(int taille) {
  DB_TypeTyp result = "";

  switch(taille) {
    case 4:
      result = DB_TypeVal(4x4);
      break;
    case 6:
      result = DB_TypeVal(6x6);
      break;
    case 8:
      result = DB_TypeVal(8x8);
      break;
    case 9:
      result = DB_TypeVal(9x9);
      break;
  }

  return result;
}
