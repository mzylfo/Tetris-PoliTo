
#include "LPC17xx.h"
#include "GLCD/GLCD.h"
#include "TouchPanel/TouchPanel.h"
#include "timer/timer.h"
#include "RIT/RIT.h"
#include "button_EXINT/button.h"
#include "joystick/joystick.h"
#include "led/led.h"

#include "tetris/tetris.h"

#define SIMULATOR 1

#ifdef SIMULATOR
extern uint8_t ScaleFlag; // <- ScaleFlag needs to visible in order for the emulator to find the symbol (can be placed also inside system_LPC17xx.h but since it is RO, it needs more work)
#endif


extern void name_function_assembler(int r0, int r1, int r2, int r3);

int main(void)
{
  SystemInit();  												/* System Initialization (i.e., PLL)  */
	BUTTON_init();												/* Inizializzazione Buttons 					*/
  LCD_Initialization();									/* Inizializzazione Display 					*/
	//TP_Init();														/* Inizializzazione TouchPanel 				*/
	joystick_init();											/* Inizializzazione Joystick 					*/
	LED_init();
	//TouchPanel_Calibrate();								/* Calibrazione touch display         */
	
	init_timer(0, 0, 0, 3, 0xF4240); 	// Timer0 inizializzazione con periodo 1 sec
	enable_timer(0);
	
	init_RIT(0xC350);  //inizializzazione del timer2 con periodo di 50mS --> 0xc350 = 50000
	enable_RIT(); 
	
	start_tetris();

	LPC_SC->PCON |= 0x1;									/* power-down	mode										*/
	LPC_SC->PCON &= ~(0x2);
	
  while (1)	
  {
		Tetris_Process_Events();
		__ASM("wfi");
  }
}
