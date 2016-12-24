all: sudoku-txt sudoku-curses

sudoku-txt: TsudokuData.o params.o file.o pileCoup.o pileSudoku.o conversion.o affichage.o ui-txt.o compteur.o sudoku.o
	gcc -o sudoku-txt pileCoup.o compteur.o affichage.o file.o conversion.o pileSudoku.o params.o sudoku.o ui-txt.o -lm

sudoku-curses: TsudokuData.o params.o file.o pileCoup.o pileSudoku.o conversion.o affichage.o ui-curses.o compteur.o sudoku.o
	gcc -o sudoku-curses pileCoup.o compteur.o affichage.o file.o conversion.o pileSudoku.o params.o sudoku.o ui-curses.o -lm -lncurses

affichage.o: affichage.c affichage.h
	gcc -o affichage.o -c affichage.c -lm -O

compteur.o: compteur.c compteur.h
	gcc -o compteur.o -c compteur.c -lm -O

conversion.o: conversion.c conversion.h 
	gcc -o conversion.o -c conversion.c -lm -O

file.o: file.c file.h
	gcc -o file.o -c file.c -lm -O

params.o: params.c params.h
	gcc -o params.o -c params.c -lm -O

pileCoup.o: pileCoup.c pileCoup.h
	gcc -o pileCoup.o -c pileCoup.c -lm -O

pileSudoku.o: pileSudoku.c pileSudoku.h
	gcc -o pileSudoku.o -c pileSudoku.c -lm -O

TsudokuData.o: TsudokuData.c TsudokuData.h
	gcc -o TsudokuData.o -c pileSudoku.c -lm -O

sudoku.o: sudoku.c affichage.h compteur.h conversion.h file.h params.h pileCoup.h pileSudoku.h TsudokuData.h
	gcc -o sudoku.o -c sudoku.c -O

ui-txt: ui-txt.c ui.h TsudokuData.h
	gcc -c ui-txt.c

ui-curses: ui-curses.c ui.h TsudokuData.h
	gcc -c ui-curses.c -lncurses

clean:
	rm -f *.o core

mrproper: clean
	rm -f sudoku
