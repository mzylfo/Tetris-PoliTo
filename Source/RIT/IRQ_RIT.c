#include "LPC17xx.h"
#include "RIT.h"
#include "../tetris/tetris.h"

/* Variabili per il debouncing (definite qui) */
volatile int down_0 = 0;
volatile int down_1 = 0;
volatile int down_2 = 0;

/* Flag degli eventi (dichiarati in tetris.c) */
extern volatile uint8_t gravity_event;
extern volatile uint8_t key1_event;
extern volatile uint8_t key2_event;
extern volatile uint8_t move_l_event;
extern volatile uint8_t move_r_event;
extern volatile uint8_t move_d_event;
extern volatile uint8_t rotate_event;

void RIT_IRQHandler (void)
{					
    static int j_left = 0; 
		static int j_right = 0;
		static int j_up = 0; 
		static int j_down = 0;
    
    /* --- JOYSTICK POLLING --- */
		// Down 
		if((LPC_GPIO1->FIOPIN & (1<<26)) == 0){ 
        j_down++;
			if(j_down  == 1) 
				move_d_event = 1; 
    } else{
			j_down = 0;
			move_d_event = 0;
		}
    // Left
    if((LPC_GPIO1->FIOPIN & (1<<27)) == 0){ 
        j_left++;
			if(j_left == 1) 
				move_l_event = 1; 
    } else j_left = 0;

    // Right
    if((LPC_GPIO1->FIOPIN & (1<<28)) == 0){ 
        j_right++; 
			if(j_right == 1)
				move_r_event = 1; 
    } else j_right = 0;

    // Up
    if((LPC_GPIO1->FIOPIN & (1<<29)) == 0){ 
        j_up++; 
			if(j_up == 1)
				rotate_event = 1; 
    } else j_up = 0;
		
		// Down 

    /* --- BUTTON MANAGEMENT (Debouncing) --- */
    
    // KEY1 (Start/Stop)
    if(down_1 != 0){
        down_1++;
        if((LPC_GPIO2->FIOPIN & (1<<11)) == 0){
            if(down_1 == 2)
							key1_event = 1;
        } else {
            down_1 = 0;
            LPC_PINCON->PINSEL4 = (LPC_PINCON->PINSEL4 & ~(3 << 22)) | (1 << 22); // Reset a EINT1
            LPC_SC->EXTINT |= (1 << 1); // Pulisce flag
            NVIC_ClearPendingIRQ(EINT1_IRQn);
            NVIC_EnableIRQ(EINT1_IRQn);
        }
    }

    // KEY2 (Hard Drop)
    if(down_2 != 0){
        down_2++;
        if((LPC_GPIO2->FIOPIN & (1<<12)) == 0){
            if(down_2 == 2)
							key2_event = 1;
        } else {
            down_2 = 0;
            LPC_PINCON->PINSEL4 = (LPC_PINCON->PINSEL4 & ~(3 << 24)) | (1 << 24); // Reset a EINT2
            LPC_SC->EXTINT |= (1 << 2);
            NVIC_ClearPendingIRQ(EINT2_IRQn);
            NVIC_EnableIRQ(EINT2_IRQn);
        }
    }

    LPC_RIT->RICTRL |= 0x1; // Clear interrupt flag
}