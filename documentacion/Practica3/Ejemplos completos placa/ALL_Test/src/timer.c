/*********************************************************************************************
* File£º	timer.c
* Author:	embest
* Desc£º	Timer source code
* History:	
*********************************************************************************************/

/*--- include files ---*/

#include "44b.h"
#include "44blib.h"
#include "timer.h"

/*--- global variables ---*/

/*--- function declare ---*/

void timer_Int(void) __attribute__ ((interrupt ("IRQ")));
void Test_Timer(void);
void timer_init(void);
void timer_close(void);

/*--- function code ---*/

/*********************************************************************************************
* name:		Test_Timer
* func:		test PWM Timer0
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void Test_Timer(void)
{
    Uart_Printf("\nTimer Start, press any key to exit...\n");
    // init timer0
	timer_init();
	// get input
	Uart_Getch();
	// close timer
	timer_close();
	Uart_Printf("\n");
}

/*********************************************************************************************
* name:		timer_Int
* func:		Timer Interrupt handler
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void timer_Int(void)
{
	//clear pending_bit
    rI_ISPC=BIT_TIMER0;
    Uart_Printf("*");
}

/*********************************************************************************************
* name:		timer_init
* func:		initialize PWM Timer0
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void timer_init(void)
{
	/* enable interrupt */
	rINTMOD=0x0;
	rINTCON=0x1;
	
	/* set interrupt handler */
	rINTMSK=~(BIT_GLOBAL|BIT_TIMER0);
    pISR_TIMER0=(unsigned)timer_Int;
	
	/* initialize PWM timer0 register */
    rTCFG0 = 255;
    rTCFG1 = 0x1;
    rTCNTB0 = 655352;
    rTCMPB0 = 128002;
    /* update TCNO0 */
	rTCON = 0x6;
	/* enable timer */
    rTCON  = 0x19;
}

/*********************************************************************************************
* name:		timer_close
* func:		close PWM Timer0
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void timer_close(void)
{
	pISR_TIMER0 = NULL;
	/* Mask interrupt */
	rINTMSK = rINTMSK | BIT_TIMER0;
	/* close Timer0 */
	rTCON = 0x0;
}


