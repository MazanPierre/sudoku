#ifndef TSUDOKUDATA_H
#define TSUDOKUDATA_H


#if !defined(DB_TypeTyp) && !defined(DB_TypeVal)
#   define DB_TypeTyp char *
#   define DB_TypeVal(v) #v
#elif defined(DB_TypeTyp) ^ defined(DB_TypeVal)
#   error DB_Type[Typ|Val] doivent etre définis ensemble
#endif

typedef struct _TsudokuData {
  DB_TypeTyp type;  // le type/taille du sudoku
  char* solu;  // le sudoku solutionné
  char* init;  // le sudoku initial
} TsudokuData;

#endif // TSUDOKUDATA_H
