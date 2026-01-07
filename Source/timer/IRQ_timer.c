/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "LPC17xx.h"
#include "timer.h"
#include "../led/led.h"
#include "../tetris/tetris.h"

extern volatile uint8_t gravity_event;

/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
extern unsigned char led_value;					/* defined in funct_led								*/
void TIMER0_IRQHandler (void)
{

	/* Match register 0 interrupt service routine */
	if (LPC_TIM0->IR & 01)
	{
		gravity_event = 1; 
		LPC_TIM0->IR = 1;			/* clear interrupt flag */
	}

  return;
}

/******************************************************************************
** Function name:		Timer2_IRQHandler
**
** Descriptions:		Timer/Counter 2 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER2_IRQHandler (void)
{ /*
	//VARIABILI PER LA GESTIONE DEL DEBOUNCING DEL JOYSTICK 
	static int j_down=0; 
	static int j_up=0;
	static int j_left=0; 
	static int j_right=0; 
	
	//SELEZIONE JOYSTICK BASSO
  if((LPC_GPIO1->FIOPIN & (1<<26)) == 0){
		j_down++; 
		
		if(j_down == 1){
			LPC_TIM0->MR0 = 0x7A120; //Se clicco verso il basso aumento la velocità 
			LPC_TIM0->TCR = 2; //reset immediato
			LPC_TIM0->TCR = 1; 
		}
	}
	else{
		//se non è cliccato ripristiniamo la velocità standard
		
		if(j_down != 0){
			LPC_TIM0->MR0 = 0xF4240; //Se clicco verso il basso aumento la velocità 
			LPC_TIM0->TCR = 2; //reset immediato
			LPC_TIM0->TCR = 1; 
		}
		
		j_down = 0; 
	}
	
	//SELEZIONE JOYSTICK SINISTRA
	if((LPC_GPIO1->FIOPIN & (1<<27)) == 0){
		j_left++; 
		
		if(j_left == 1){
			move_left(); 
		}
	}
	else{
		j_left = 0; 
	}
	
	//SELEZIONE JOYSTICK DESTRA
	if((LPC_GPIO1->FIOPIN & (1<<28)) == 0){
		j_right++; 
		
		if(j_right == 1){
			move_right(); 
		}
	}
	else{
		j_right = 0; 
	}
	
	//SELEZIONE JOYSTICK SOPRA
	if((LPC_GPIO1->FIOPIN &(1<<29)) == 0){
		j_up++; 
		
		if(j_up == 1){
			rotate(); 
		}
	}
	else{
		j_up = 0; 
	}
	
	
	//GESTIONE DEL DEBOUDING DEI BOTTONI
	if(down_0!=0){
		down_0++; 
		
		if((LPC_GPIO2->FIOPIN & (1<<10)) == 0){
			if(down_0==2){
				//azione reset
			}
		}
		else{
			down_0 = 0; NVIC_EnableIRQ(EINT0_IRQn);
			LPC_PINCON->PINSEL4 |= (1 << 20);
		}
	}
	
	if(down_1!=0){
		down_1++; 
		
		if((LPC_GPIO2->FIOPIN & (1<<11)) == 0){
			if(down_1==2){
				start_and_stop();
			}
		}
		else{
			down_1 = 0; NVIC_EnableIRQ(EINT1_IRQn);
			LPC_PINCON->PINSEL4 |= (1 << 22);
		}
	}
	
	if(down_2!=0){
		down_2++; 
		
		if((LPC_GPIO2->FIOPIN & (1<<12)) == 0){
			if(down_2==2){
				hard_drop();
			}
		}
		else{
			down_2 = 0; NVIC_EnableIRQ(EINT2_IRQn);
			LPC_PINCON->PINSEL4 |= (1 << 24);
		}
	}
	*/
	LPC_TIM2->IR = 0xFF; //RESETTING TIMER
	
  return;
}

/******************************************************************************
** Function name:		Timer3_IRQHandler
**
** Descriptions:		Timer/Counter 3 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER3_IRQHandler (void)
{
  /* Match register 0 interrupt service routine */
	if (LPC_TIM3->IR & 01)
	{
		
		LPC_TIM3->IR = 1;			/* clear interrupt flag */
	}
		/* Match register 1 interrupt service routine */
	  /* it should be possible to access to both interrupt requests in the same procedure*/
	if(LPC_TIM3->IR & 02)
  {
	
		LPC_TIM3->IR =  2 ;			/* clear interrupt flag */	
	}
	/* Match register 2 interrupt service routine */
  /* it should be possible to access to both interrupt requests in the same procedure*/
	if(LPC_TIM3->IR & 4)
  {
		
		LPC_TIM3->IR =  4 ;			/* clear interrupt flag */	
	}
		/* Match register 3 interrupt service routine */
  	/* it should be possible to access to both interrupt requests in the same procedure*/
	if(LPC_TIM3->IR & 8)
  {
	 
		LPC_TIM3->IR =  8 ;			/* clear interrupt flag */	
	}
  return;
}



/******************************************************************************
**                            End Of File
******************************************************************************/
