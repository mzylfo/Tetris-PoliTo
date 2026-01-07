/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               tetris.c
** Descriptions:            The GLCD application function
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-11-7
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             Matteo Zylfo
** Modified date:           21/12/2025
** Version:                 v2.0
** Descriptions:            basic program for LCD and Touch Panel teaching
**
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "LPC17xx.h"
#include "GLCD/GLCD.h"
#include "TouchPanel/TouchPanel.h"
#include "timer/timer.h"
#include "RIT/RIT.h"
#include "button_EXINT/button.h"
#include "joystick/joystick.h"
#include "led/led.h"

#include "tetris.h"

#include "stdio.h"

int score = 0; 
int top = 0; 
int lines = 0;


/* GAME STATUS 
	0 --> 	PAUSED
	1 --> 	RUNNING 
	-1 -->	GAME OVER
*/ 
int game_status = 0;

/* MATRIX GAME
	MATRIX 20x10 
	0 --> empty 
	1 --> cell occupied by a tetromino
	
	MATRIX 20x10 FOR COLORS
*/ 
int matrix_game[20][10];
uint16_t matrix_color[20][10]; 
uint16_t colors[7] = {Blue, Blue2, Red, Magenta, Green, Cyan, Yellow}; 

//VARIABILI PER LA GESTIONE DEL MOVIMENTO IN BASSO DEL TETROMINO
int current_type; 
int current_row; 
int current_col; 
int current_rot;
uint16_t current_color; 

int first_start = 1;

/* --- Aggiunte in tetris.c --- */
volatile uint8_t gravity_event = 0;
volatile uint8_t key1_event = 0;
volatile uint8_t key2_event = 0;
volatile uint8_t move_l_event = 0;
volatile uint8_t move_r_event = 0;
volatile uint8_t move_d_event = 0;
volatile uint8_t rotate_event = 0;

/* TETROMINOES DEFINITION 
a 4x4 matrix with: 
- i --> type of tetromino
- j --> rotation of the tetromino
*/

const int tetromino[7][4][4][4] =
{
/* ===================== I ===================== */
{
    {   // rot 0
        {1,1,1,1},
        {0,0,0,0},
        {0,0,0,0},
				{0,0,0,0}
    },
    {   // rot 1
        {0,0,1,0},
        {0,0,1,0},
        {0,0,1,0},
        {0,0,1,0}
    },
    {   // rot 2 (uguale a 0)
        {1,1,1,1},
        {0,0,0,0},
        {0,0,0,0},
				{0,0,0,0}
    },
    {   // rot 3 (uguale a 1)
        {0,0,1,0},
        {0,0,1,0},
        {0,0,1,0},
        {0,0,1,0}
    }
},

/* ===================== O ===================== */
{
    {
        {0,1,1,0},
        {0,1,1,0},
        {0,0,0,0},
        {0,0,0,0}
    },
    {
        {0,1,1,0},
        {0,1,1,0},
        {0,0,0,0},
        {0,0,0,0}
    },
    {
        {0,1,1,0},
        {0,1,1,0},
        {0,0,0,0},
        {0,0,0,0}
    },
    {
        {0,1,1,0},
        {0,1,1,0},
        {0,0,0,0},
        {0,0,0,0}
    }
},

/* ===================== T ===================== */
{
    {
        {0,1,0,0},
        {1,1,1,0},
        {0,0,0,0},
        {0,0,0,0}
    },
    {
        {0,1,0,0},
        {0,1,1,0},
        {0,1,0,0},
        {0,0,0,0}
    },
    {
        {1,1,1,0},
        {0,1,0,0},
        {0,0,0,0},
				{0,0,0,0}
    },
    {
        {0,1,0,0},
        {1,1,0,0},
        {0,1,0,0},
        {0,0,0,0}
    }
},

/* ===================== J ===================== */
{
    {
        {1,0,0,0},
        {1,1,1,0},
        {0,0,0,0},
        {0,0,0,0}
    },
    {
        {0,1,1,0},
        {0,1,0,0},
        {0,1,0,0},
        {0,0,0,0}
    },
    {
        {1,1,1,0},
        {0,0,1,0},
        {0,0,0,0},
        {0,0,0,0}
    },
    {
        {0,1,0,0},
        {0,1,0,0},
        {1,1,0,0},
        {0,0,0,0}
    }
},

/* ===================== L ===================== */
{
    {
        {0,0,1,0},
        {1,1,1,0},
        {0,0,0,0},
        {0,0,0,0}
    },
    {
        {0,1,0,0},
        {0,1,0,0},
        {0,1,1,0},
        {0,0,0,0}
    },
    {
        {0,0,0,0},
        {1,1,1,0},
        {1,0,0,0},
        {0,0,0,0}
    },
    {
        {1,1,0,0},
        {0,1,0,0},
        {0,1,0,0},
        {0,0,0,0}
    }
},

/* ===================== S ===================== */
{
    {
        {0,1,1,0},
        {1,1,0,0},
        {0,0,0,0},
        {0,0,0,0}
    },
    {
        {0,1,0,0},
        {0,1,1,0},
        {0,0,1,0},
        {0,0,0,0}
    },
    {
        {0,1,1,0},
        {1,1,0,0},
        {0,0,0,0},
        {0,0,0,0}
    },
    {
        {0,1,0,0},
        {0,1,1,0},
        {0,0,1,0},
        {0,0,0,0}
    }
},

/* ===================== Z ===================== */
{
    {
        {1,1,0,0},
        {0,1,1,0},
        {0,0,0,0},
        {0,0,0,0}
    },
    {
        {0,0,1,0},
        {0,1,1,0},
        {0,1,0,0},
        {0,0,0,0}
    },
    {
        {1,1,0,0},
        {0,1,1,0},
        {0,0,0,0},
        {0,0,0,0}
    },
    {
        {0,0,1,0},
        {0,1,1,0},
        {0,1,0,0},
        {0,0,0,0}
    }
}
};


void print_score(void){
	//Setting lines --> TOP 
	char buffer[20];
  sprintf(buffer, "%d", top);
	GUI_Text(170, 10, (uint8_t *)"TOP",White, Black);
	GUI_Text(170, 30, (uint8_t *)buffer,White, Black);

	//Setting lines --> SCORE 
  sprintf(buffer, "%d", score);
	GUI_Text(170, 90, (uint8_t *)"SCORE",White, Black);
	GUI_Text(170, 110, (uint8_t *)buffer,White, Black);
	
	//Setting lines --> DELETED 
  sprintf(buffer, "%d", lines);
	GUI_Text(170, 170, (uint8_t *)"LINES",White, Black);
	GUI_Text(170, 190, (uint8_t *)buffer,White, Black);
	
	GUI_Text(170, 250, (uint8_t *)"       ", Green, Black);
	
	//Setting lines --> GAME_STATUS
	GUI_Text(165, 250, (uint8_t *)"         ", Black, Black);
	if(game_status == 0)
		GUI_Text(170, 250, (uint8_t *)"PAUSED",Blue, Black);
	if(game_status == 1)
		GUI_Text(170, 250, (uint8_t *)"PLAYING", Green, Black);
	if(game_status == -1)
		GUI_Text(165, 250, (uint8_t *)"GAME OVER", Red, Black);
	
}

void print_game_grid(){
	
	int i = 0; 
	int j = 0; 
	
	for(i=1; i<20; i++){
		LCD_DrawLine( 10, (10+15*i), 160, (10+15*i), White );
	}
	
	for(j=1; j<10; j++){
		LCD_DrawLine( (10+15*j), 10, (10+15*j), 310, White );
	}
}

void draw_current_piece(void){
	int i,j,q; 
	
	for(i=0; i<4; i++){
		for(j=0; j<4; j++){
			if(tetromino[current_type][current_rot][i][j]){
				for (q = 0; q < 14; q++) {
                    LCD_DrawLine(11 + (15 * (current_col + j)), 11 + (15 * (current_row + i)) + q, 
                                 11 + (15 * (current_col + j)) + 13, 11 + (15 * (current_row + i)) + q, current_color);
                }
			}
		}
	}
}

void define_matrix_game(){
	int i=0; 
	int j=0; 
	
	for(i=0; i<20; i++){
		for(j=0; j<10; j++){
			/*matrix_game[i][j] = 0;
			matrix_color[i][j] = Black;*/
			
			if(matrix_game[i][j] == 1 ) {
				matrix_game[i][j] = 0;      
				matrix_color[i][j] = Black;   
			}
		}
	}
}

void draw_cells(void) {
    int i, j, q;
    
    // Concetto: Scansione invertita (Bottom-Up)
    // Partiamo dalla riga 19 (fondo) fino alla riga 0 (cima)
    for(i = 19; i >= 0; i--) {
        for(j = 0; j < 10; j++) {
            
            // Disegniamo solo se la cella è occupata (Spec. 1)
            // Ottimizzazione: saltiamo il disegno dei blocchi neri se non necessario
            if(matrix_game[i][j] == 1) {
                uint16_t color = matrix_color[i][j];
                
                // Disegno veloce del blocco 15x15 pixel (Spec. 1)
                for(q = 0; q < 14; q++) {
                    LCD_DrawLine(11 + (15 * j), 11 + (15 * i) + q, 
                                 11 + (15 * j) + 13, 11 + (15 * i) + q, color);
                }
            }
            else {
                // Se la cella è vuota, la coloriamo di nero (Black)
                // Questo assicura che le linee cancellate spariscano visivamente
                for(q = 0; q < 14; q++) {
                    LCD_DrawLine(11 + (15 * j), 11 + (15 * i) + q, 
                                 11 + (15 * j) + 13, 11 + (15 * i) + q, Black);
                }
            }
        }
    }
}

void game_over_brother(void){
	//funzione che viene chiamata quando abbiamo un game over
	game_status = -1; 
	
	if(score>top) 
		top = score;

	score=0;
	lines=0; 
	
	print_score();
	
	define_matrix_game();
}

void generate_tetromino(void) {
    current_type = rand() % 7;
    current_rot = rand() % 4;
    current_color = colors[rand() % 7];
    current_col = 3; // Centrato
    current_row = 0;

    if (!can_move(current_row, current_col, current_rot)) {
        /*game_status = -1; // GAME OVER
        if (score > top) top = score;
        print_score();*/ 
				game_over_brother(); 
    } else {
        // Disegniamo subito il pezzo nella riga 0
        draw_current_piece();
    }
}


void start_tetris(void){
	//Setting black screen 
	LCD_Clear(Black);
	
	/*
	GAME FIELD COORDINATES
	A --> x = 10, 	y = 10 
	B --> x = 160, 	y = 10
	C --> x = 10, 	y = 310
	D --> x = 160,	y = 310
	*/ 
	
	/*
	SIZE BLOCK 
	15 x 15 
	*/
	
	//GAME FIELD
	LCD_DrawLine( 10, 10, 160, 10, White );
	LCD_DrawLine( 10, 10, 10, 310, White );
	LCD_DrawLine( 10, 310, 160, 310, White );
	LCD_DrawLine( 160, 10, 160, 310, White );
	
	//define_matrix_game();
	//print_game_grid(); 
	
	print_score();
	
	//draw_cells();
}

void set_random_seed(int seed){
	srand(seed); 
}

void start_and_stop(void){
	if(game_status!=-1){
		if(game_status == 0){ //from pause to running
			game_status = 1;
			
			if(first_start){
				set_random_seed(LPC_TIM0->TC);
				generate_tetromino(); 
				first_start = 0; 
			}
		}
		else if(game_status == 1){ //from running to pause
			game_status = 0;
		}
	}
	else{
		//new game
		start_tetris();
		game_status = 1; 
	}
	
	print_score(); 
}

void place_piece(void) {
    int i, j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            if (tetromino[current_type][current_rot][i][j]) {
                int r = current_row + i;
                int c = current_col + j;
                if (r >= 0 && r < 20 && c >= 0 && c < 10) {
                    matrix_game[r][c] = 1;
                    matrix_color[r][c] = current_color;
                } else if (r < 0) {
                    game_status = -1; // Sconfitta sopra il bordo
                }
            }
        }
    }
    score += 10; // 10 punti per pezzo piazzato
    print_score();
}

void delete_current_piece(void){
	int i,j,q; 
	
	for(i=0; i<4; i++){
		for(j=0; j<4; j++){
			if(tetromino[current_type][current_rot][i][j]){
				for (q = 0; q < 14; q++) {
                    LCD_DrawLine(11 + (15 * (current_col + j)), 11 + (15 * (current_row + i)) + q, 
                                 11 + (15 * (current_col + j)) + 13, 11 + (15 * (current_row + i)) + q, Black);
                }
            }
        }
    }
}

void draw_partial_cells(int row){
	int i,j,q; 
	
	for(i = row; i >= 0; i--) {
        for(j = 0; j < 10; j++) {
						uint16_t color; 
						if(matrix_game[i][j]==1)
							color=matrix_color[i][j]; 
						else
							color=Black; 
            
            // Disegno chirurgico del blocco
            for(q = 0; q < 14; q++) {
                LCD_DrawLine(11 + (15 * j), 11 + (15 * i) + q, 
                             11 + (15 * j) + 13, 11 + (15 * i) + q, color);
            }
        }
    }
}

void check_clear_lines(void){
	int r, c, r_shift, lines_cleared = 0;
	int highest_row=19; 
	
	for(r=19; r>=0; r--){ //scansionamento dal basso verso l'alto
		int full = 1; 
		
		for(c=0; c<10; c++){
			if(matrix_game[r][c]== 0){ // la riga non è piena
				full=0; 
				break; 
			}
		}
		
		if(full){
			lines_cleared++; 
			
			if(r<highest_row)
				highest_row = r; 
			
			for(r_shift=r; r_shift>0; r_shift--){
						for(c=0; c<10; c++){
							matrix_game[r_shift][c] = matrix_game[r_shift-1][c]; 
							matrix_color[r_shift][c] = matrix_color[r_shift-1][c]; 
						}
			}
			
			//svuotiamo la riga in alto
			for(c=0; c<10; c++){
				matrix_game[0][c] = 0; 
				matrix_color[0][c] = Black; 
			}
			r++; 
		}
	}
	
	//aggiornamento del punteggio
	if(lines_cleared>0){
		lines += lines_cleared; 
		if(lines_cleared == 4){
			score += 600; 
		}
		else{
			score += (lines_cleared*100); 
		}
		print_score();
		
		draw_partial_cells(highest_row);
	}
}

void shift_matrix(void) {
    if (game_status == 1) {
        // Concetto: Controllo preventivo
        if (can_move(current_row + 1, current_col, current_rot)) {
            // Se può muoversi, cancelliamo la vecchia posizione e disegniamo la nuova
            delete_current_piece();
            current_row++;
            draw_current_piece();
        } else {
            // Se collide, il pezzo deve essere fissato
            place_piece();
            check_clear_lines();
            
            // Creiamo il nuovo pezzo
            generate_tetromino();
        }
    }
}

int can_move(int new_row, int new_col, int new_rot) {
    int i, j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            if (tetromino[current_type][new_rot][i][j]) {
                int target_row = new_row + i;
                int target_col = new_col + j;

                // Controllo bordi campo 20x10
                if (target_col < 0 || target_col >= 10 || target_row >= 20)
                    return 0;

                // Controllo collisione con blocchi fissi in matrix_game
                if (target_row >= 0 && matrix_game[target_row][target_col] == 1)
                    return 0;
            }
        }
    }
    return 1;
}

void hard_drop(void){
	if(game_status==1){ //controlliamo se siamo in stato PLAYING
		delete_current_piece(); 
		
		while(can_move(current_row+1, current_col, current_rot)){
			current_row++; //aumenta fino a quando non viene toccato il fondo
		}
		
		draw_current_piece(); //disegnamolo nella board
		place_piece(); 				//inseriamolo nella matrice del game
		check_clear_lines();
		generate_tetromino(); //ripartiamo subito con la generazione
	}
}

void move_left(void){
	if(game_status==1){
		if(can_move(current_row, current_col-1, current_rot)){
			delete_current_piece();
			current_col -= 1; 
			draw_current_piece();
		}
	}
}

void move_right(void){
	if(game_status == 1){
		if(can_move(current_row, current_col+1, current_rot)){
			delete_current_piece();
			current_col += 1; 
			draw_current_piece();
		}
	}
}

void rotate(void){
	if(game_status == 1){
		int next_rotation = (current_rot+1)%4; 
		
		if(can_move(current_row, current_col, next_rotation)){
			delete_current_piece();
			current_rot = next_rotation;
			draw_current_piece();
		}
	}
}

void Tetris_Process_Events(void) {
		static uint8_t last_d_state = 0; 
	
    if (key1_event) { 
        key1_event = 0; 
        start_and_stop(); 
    }
    if (move_l_event) { 
        move_l_event = 0; 
        move_left(); 
    }
    if (move_r_event) { 
        move_r_event = 0; 
        move_right(); 
    }
    if (rotate_event) { 
        rotate_event = 0; 
        rotate(); 
    }
    if (key2_event) { 
        key2_event = 0; 
        hard_drop(); 
    }
		
		if (move_d_event!=last_d_state) {
        // 1. Ferma e resetta il timer per applicare il nuovo MR0 istantaneamente
        LPC_TIM0->TCR = 0x2;
        
        if (move_d_event == 1) {
            // JOYSTICK PREMUTO: Velocità Soft Drop
            LPC_TIM0->MR0 = 0x7A120;
        } else {
            // JOYSTICK RILASCIATO: Torna a velocità 1 secondo
            LPC_TIM0->MR0 = 0xF4240;
        }
        
        // 2. Fai ripartire il timer
        LPC_TIM0->TCR = 0x1;
        
        // 3. Aggiorna lo stato statico per la prossima iterazione
				last_d_state = move_d_event; 
		}
    
    // Gestione della caduta effettiva
    if (gravity_event) { 
        gravity_event = 0; 
        shift_matrix();
    }
}

