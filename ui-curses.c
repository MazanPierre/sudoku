#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

#include "TsudokuData.h"
#include "pileSudoku.h"
#include "pileCoup.h"
#include "ui.h"

#define KEY_ENTER 10

typedef struct _win_border_struct {
	chtype 	ls, rs, ts, bs, 
	 	tl, tr, bl, br;
}WIN_BORDER;

typedef struct _WIN_struct {

	int startx, starty;
	int height, width;
	WIN_BORDER border;
}WIN;


void init_win_params(WIN *p_win);
void print_win_params(WIN *p_win);
void create_box(WIN *win, bool flag);

struct _Tui {
  PileSudoku *pileSudoku;
  int indexGrille;
};

struct _Tjeu {
};

Tui *ui_new(int indexGrille)
{
  Tui *ui = malloc(sizeof(Tui));
  
  ui->pileSudoku = chargerSudokuDB("sudoku-db.dat");
  ui->indexGrille = indexGrille;
  
  return ui;
}

void ui_free(Tui *ui)
{
  dechargerSudoku(ui->pileSudoku);
  free(ui);
}

void draw_borders(WINDOW *screen) {
  int x, y, i;

  getmaxyx(screen, y, x);

  // 4 corners
  mvwprintw(screen, 0, 0, "+");
  mvwprintw(screen, y - 1, 0, "+");
  mvwprintw(screen, 0, x - 1, "+");
  mvwprintw(screen, y - 1, x - 1, "+");

  // sides
  for (i = 1; i < (y - 1); i++) {
    mvwprintw(screen, i, 0, "|");
    mvwprintw(screen, i, x - 1, "|");
  }

  // top and bottom
  for (i = 1; i < (x - 1); i++) {
    mvwprintw(screen, 0, i, "-");
    mvwprintw(screen, y - 1, i, "-");
  }
}

enum {MODE_JOUER, MODE_RETOUR, MODE_VOIR, MODE_SAUVEGARDER, MODE_CHARGER, NB_CASE_TAB};

void ui_afficherGrille(Tui *ui, Tjeu *jeu)
{
  TsudokuData *sudokuData = pileSud_get(ui->pileSudoku, ui->indexGrille);
  PileCoup *pileCoup = pileCoup_create();
  char screenOutput[2048] = "";
  char grilleData[81] = "";
  int tailleGrille = conversion_DBTypeToInt(sudokuData->type);
  int nbPropositionAutorise = (int)(compteurCaseVide(sudokuData->init, tailleGrille) * 2);
  int parent_x, parent_y, new_x, new_y;
  int menu_width = 30;
  int score_size = 3;
  int ch = 0;
  int startx = 0;
  int starty = 0;
  int modeMenu = 0;
  int modeMenuActif = 0;
  int modeMenuEtape = 0;
  char *entreeMenu[NB_CASE_TAB];
  int i = 0;
  int charX, charY;
  int startGX, startGY;
  int entreeX, entreeY, entreeV;
  int x, y, v;
  int erreurProposition = 0;
  int unicode = 0;
  int continuer = 1;
  char charCoche = ' ';
  
  strcpy(grilleData, sudokuData->init);
  
  entreeMenu[MODE_JOUER] = "Jouer un coup";
  entreeMenu[MODE_RETOUR] = "Coup précédent";
  entreeMenu[MODE_VOIR] = "Voir un coup";
  entreeMenu[MODE_SAUVEGARDER] = "Sauvegarder";
  entreeMenu[MODE_CHARGER] = "Charger";

  initscr();
  noecho();
  curs_set(FALSE);
  // set up initial windows
  getmaxyx(stdscr, parent_y, parent_x);
  cbreak();
  keypad(stdscr, TRUE);
  refresh();
  
  WINDOW *menu = newwin(parent_y - score_size, menu_width, 0, 0);
  WINDOW *field = newwin(parent_y - score_size, parent_x - menu_width, 0, menu_width);
  WINDOW *score = newwin(score_size, parent_x, parent_y - score_size, 0);
  
  //startx = (COLS - width) / 2;
  //starty = (LINES - height) / 2;

  draw_borders(menu);
  draw_borders(field);
  draw_borders(score);

  do {
    getmaxyx(stdscr, new_y, new_x);

    if (new_y != parent_y || new_x != parent_x) {
      parent_x = new_x;
      parent_y = new_y;

      wresize(menu, new_y - score_size, new_x);
      wresize(field, new_y - score_size, menu_width + new_x);
      wresize(score, score_size, new_x);
      mvwin(score, new_y - score_size, 0);

      wclear(stdscr);
      wclear(menu);
      wclear(field);
      wclear(score);

      draw_borders(menu);
      draw_borders(field);
      draw_borders(score);
    }
    
    // dessinner le menu
    for(i = 0; i < NB_CASE_TAB; i++)
    {
        if(modeMenu == i) {
            if(modeMenuActif)
                charCoche = 'X';
            else
                charCoche = '*';
        } else {
            charCoche = ' ';
        }
        
        mvwprintw(menu, i + 1, 3, "[%c] %s", charCoche, entreeMenu[i]);
    }
    
    construireGrille(screenOutput, grilleData, sudokuData->init, tailleGrille, unicode);
    
    // dessinner la grille
    i = 0;
    charX = 0;
    charY = 0;
    while(screenOutput[i] != '\0') {
        
        if(screenOutput[i] == '\n') {
            charX = 0;
            charY++;
        } else {
            mvwprintw(field, charY + 1, charX + 1, "%c", screenOutput[i]);
            charX++;
        }
        i++;
    }
    
    if(modeMenuActif)
    {
        if(modeMenu == MODE_JOUER) {
            mvwprintw(menu, NB_CASE_TAB + 3, 1, "X:[%c] Y:[%c] V:[%c]", entreeX, entreeY, entreeV);
        }
    }
    else
    {
        for(i = 1; i < menu_width - 1; i++)
            mvwprintw(menu, NB_CASE_TAB + 3, i, " ");
    }
    
    // dessinner les stats
    mvwprintw(score, 1, 1, "Score [0]");
    
    mvwprintw(menu, NB_CASE_TAB + 2, 1, "%d %d", ch, modeMenuEtape);
    
     // refresh each window
    wrefresh(menu);
    wrefresh(field);
    wrefresh(score);

    ch = getch();
    
    
    if(modeMenuActif) {
        
        if(modeMenu == MODE_JOUER) {
            if(modeMenuEtape == 0) {
                if(ch >= 'A' && ch <= 'A' + tailleGrille) {
                    entreeX = ch;
                    modeMenuEtape++;
                }
                if(ch >= 'a' && ch <= 'a' + tailleGrille) {
                    entreeX = ch - 'a' + 'A';
                    modeMenuEtape++;
                }
            }
            else if(modeMenuEtape == 1) {
                if(ch >= '1' && ch <= '1' + tailleGrille) {
                    entreeY = ch;
                    modeMenuEtape++;
                }
                if(ch >= 113 && ch <= 113 + tailleGrille) {
                    entreeY = ch - 113 + '1';
                    modeMenuEtape++;
                }
            }
            else if(modeMenuEtape == 2) {
                if(ch >= 'A' && ch <= 'A' + tailleGrille) {
                    entreeV = ch;
                    modeMenuActif = 0;
                }
                if(ch >= 'a' && ch <= 'a' + tailleGrille) {
                    entreeV = ch - 'a' + 'A';
                    modeMenuActif = 0;
                }
                if(!modeMenuActif) {
                    x = entreeX - 'A';
                    y = tailleGrille - (entreeY - '0');
                    v = entreeV;

                    if(!erreurProposition) {
                      if(sudokuData->init[x + y * tailleGrille] == '.') {
                        pileCoup_empile(pileCoup, x, y, grilleData[x + y * tailleGrille], v);
                        grilleData[x + y * tailleGrille] = v;
                      } else {
                        printf("/!\\ Vous ne pouvez pas modifier cette case!\n");
                        erreurProposition = 1;
                      }
                    }
                    modeMenuEtape++;
                }
                
            } 
        }
    } else {
        
        if(ch == KEY_UP) {
            modeMenu--;
            if(modeMenu < 0)
                modeMenu = NB_CASE_TAB - 1;
        }
        else if(ch == KEY_DOWN) {
            modeMenu++;
            if(modeMenu >= NB_CASE_TAB)
                modeMenu = 0;
        }
        else if(ch == KEY_ENTER) {
            if(modeMenu == MODE_JOUER) {
                modeMenuActif = 1;
                entreeX = ' ';
                entreeY = ' ';
                entreeV = ' ';
                modeMenuEtape = 0;
            } else if(modeMenu == MODE_RETOUR) {
                if(!pileCoup_vide(pileCoup)) {
                    ElementCoup elementCoup = pileCoup_depile(pileCoup);
                    grilleData[elementCoup.abscisse + elementCoup.ordonnee * tailleGrille] = elementCoup.valeurPrecedente;
                }
            }
        }
    }
    
   
    
  } while(continuer);

  endwin();
}


