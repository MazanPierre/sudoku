typedef struct _Tui Tui;
typedef struct _Tjeu Tjeu;

extern Tui *ui_new();
extern void ui_free(Tui *ui);
extern void ui_afficherGrille(Tui *ui);