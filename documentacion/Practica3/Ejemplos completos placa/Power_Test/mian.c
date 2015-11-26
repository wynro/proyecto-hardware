/*********************************************************************************************
* File£º	main.c
* Author:	embest
* Desc£º	c main entry
* History:	
*********************************************************************************************/

/*--- include files ---*/
#include "44blib.h"
#include "44b.h"

/*--- function code ---*/
void KeyIsr(void) __attribute__ ((interrupt ("IRQ")));

/*********************************************************************************************
* name:		main
* func:		c code entry
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void Main(void)
{
	char input_char;				/* user input char 			*/
	
    sys_init();        /* Initial 44B0X's Interrupt,Port and UART */
    _Link();           /* Print Misc info */
	
	/* printf interface */
	Uart_Printf("\n Embest S3CEV40 Power Mode test.\n");
	Delay(500);

    while(1)
    {
    /* PORT G */
    rPCONG     = 0xffff;                          // EINT7~0
	rPUPG      = 0x0;                             // pull up enable	    
	rEXTINT    = rEXTINT|0x22220020;              // EINT1¡¢EINT4567 falling edge mode	
    rI_ISPC   |= (BIT_EINT1|BIT_EINT4567);
	rEXTINTPND = 0xf;                          // clear EXTINTPND reg
    pISR_EINT1 = (int)KeyIsr;

    Uart_Printf("\n <<Normal mode>> Test. Y/y to continue,any key skip it.\n");
    input_char = Uart_Getch();
    if(input_char == 'Y' || input_char == 'y')
       Test_HoldMode();//Normal mode
       
    Uart_Printf("\n <<Slow mode>> Test. Y/y to continue,any key skip it.\n");
    input_char = Uart_Getch();
    if(input_char == 'Y' || input_char == 'y')
       Test_SlowMode();
       
    Uart_Printf("\n <<Stop mode>> Test. Y/y to continue,any key skip it.\n");
    input_char = Uart_Getch();
    if(input_char == 'Y' || input_char == 'y')
       Test_StopMode();
       
    Uart_Printf("\n <<Idle mode>> Test. Y/y to continue,any key skip it.\n");
    input_char = Uart_Getch();
    if(input_char == 'Y' || input_char == 'y')
       Test_IdleMode();  
       
    Uart_Printf("\n <<SLidle mode>> Test. Y/y to continue,any key skip it.\n");
    input_char = Uart_Getch();
    if(input_char == 'Y' || input_char == 'y')
       Test_SLIdleMode();
    }
}

/*********************************************************************************************
* name:		
* func:	
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void KeyIsr(void)
{
    rI_ISPC    = BIT_EINT1;		//clear pending_bit
    Uart_Printf("Keyboard interrupt has occured.\n");
}
