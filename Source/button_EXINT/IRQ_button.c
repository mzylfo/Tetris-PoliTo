#include "button.h"
#include "LPC17xx.h"
#include "../RIT/RIT.h"
#include "../tetris/tetris.h"

extern int down_0;
extern int down_1;
extern int down_2;

void EINT0_IRQHandler (void)	  	/* INT0														 */
{		
	NVIC_DisableIRQ(EINT0_IRQn);		/* disable Button interrupts			 */
	LPC_PINCON->PINSEL4    &= ~(1 << 20);     /* GPIO pin selection */
	down_0=1; //debouncing
	LPC_SC->EXTINT &= (1 << 0);     /* clear pending interrupt         */
	//reset_RIT();
}


void EINT1_IRQHandler (void)	  	/* KEY1														 */
{
	NVIC_DisableIRQ(EINT1_IRQn);		/* disable Button interrupts			 */
	LPC_PINCON->PINSEL4    &= ~(1 << 22);     /* GPIO pin selection */
	down_1=1; //debouncing
	LPC_SC->EXTINT &= (1 << 1);     /* clear pending interrupt         */
	
	//start_and_stop(); 
	//reset_RIT();
}

void EINT2_IRQHandler (void)	  	/* KEY2														 */
{
	NVIC_DisableIRQ(EINT2_IRQn);		/* disable Button interrupts			 */
	LPC_PINCON->PINSEL4    &= ~(1 << 24);     /* GPIO pin selection */
	down_2=1; //debouncing
  LPC_SC->EXTINT &= (1 << 2);     /* clear pending interrupt         */
	
	//hard_drop();
	//reset_RIT();
}


