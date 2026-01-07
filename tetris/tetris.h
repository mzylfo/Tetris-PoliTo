#ifndef TETRIS_H
#define TETRIS_H

#include "LPC17xx.h"

/* User info */
extern volatile int top;
extern volatile int score;
extern volatile int lines;

/* Eventi del gioco (Flag) */
extern volatile uint8_t gravity_event;
extern volatile uint8_t key1_event;
extern volatile uint8_t key2_event;
extern volatile uint8_t move_l_event;
extern volatile uint8_t move_r_event;
extern volatile uint8_t move_d_event; 
extern volatile uint8_t rotate_event;

/* Functions */
void start_tetris(void);
void print_score(void);
void start_and_stop(void);
void shift_matrix(void);
void hard_drop(void); 
void move_right(void); 
void move_left(void);
void rotate(void);

#endif /* TETRIS_H */