#ifndef __TETRIS_H
#define __TETRIS_H

#include <LPC17xx.h>

/* ============================================================
 *  COSTANTI DI GIOCO
 * ============================================================ */
#define ROWS   20   /* righe della board                       */
#define COLS   10   /* colonne della board                     */

/* ============================================================
 *  VARIABILI GLOBALI (definite in tetris.c)
 * ============================================================ */

/* Punteggio */
extern int score;          /* punteggio corrente                */
extern int top;            /* record (high score)               */
extern int lines;          /* totale linee eliminate            */

/* Stato della partita:
 *    0 --> PAUSED
 *    1 --> RUNNING
 *   -1 --> GAME OVER                                            */
extern int game_status;

/* Board di gioco 20x10:
 *    matrix_game  -> 0 cella vuota, 1 cella occupata
 *    matrix_color -> colore della cella occupata                */
extern int      matrix_game[ROWS][COLS];
extern uint16_t matrix_color[ROWS][COLS];
extern uint16_t colors[7];

/* Stato del tetromino corrente */
extern int      current_type;
extern int      current_row;
extern int      current_col;
extern int      current_rot;
extern uint16_t current_color;

extern int first_start;

/* Definizione dei 7 tetromini (tipo x rotazione x 4 x 4) */
extern const int tetromino[7][4][4][4];

/* ============================================================
 *  FLAG DEGLI EVENTI (settati dalle ISR, gestiti nel main loop)
 * ============================================================ */
extern volatile uint8_t gravity_event;
extern volatile uint8_t key1_event;
extern volatile uint8_t key2_event;
extern volatile uint8_t move_l_event;
extern volatile uint8_t move_r_event;
extern volatile uint8_t move_d_event;
extern volatile uint8_t rotate_event;

/* ============================================================
 *  PROTOTIPI DELLE FUNZIONI
 * ============================================================ */

/* --- Rendering / GUI --- */
void print_score(void);
void print_game_grid(void);
void draw_current_piece(void);
void delete_current_piece(void);
void draw_cells(void);
void draw_partial_cells(int row);

/* --- Gestione della board --- */
void define_matrix_game(void);
void place_piece(void);
void check_clear_lines(void);

/* --- Logica del pezzo corrente --- */
int  can_move(int new_row, int new_col, int new_rot);
void generate_tetromino(void);
void shift_matrix(void);

/* --- Comandi di gioco --- */
void move_left(void);
void move_right(void);
void rotate(void);
void hard_drop(void);

/* --- Controllo della partita --- */
void start_tetris(void);
void start_and_stop(void);
void game_over_brother(void);
void set_random_seed(int seed);

/* --- Main loop --- */
void Tetris_Process_Events(void);

#endif /* __TETRIS_H */
