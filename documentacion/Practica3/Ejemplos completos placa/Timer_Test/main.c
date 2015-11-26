/*********************************************************************************************
* File£º	main.c
* Author:	embest
* Desc£º	c main entry
* History:	
*********************************************************************************************/

/*--- include files ---*/
#include "44blib.h"
#include "44b.h"

/*--- function declare ---*/
void main(void);

/*--- extern function ---*/
extern void Test_Timer();
extern void Test_WDTimer();

/*--- function code ---*/
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
    volatile char input_char;
    
    sys_init();        /* Initial 44B0X's Interrupt,Port and UART */
    _Link();           /* Print Misc info */
	
	/******************/
	/* user interface */
	/******************/
	Uart_Printf("\n\rEmbest 44B0X Evaluation Board(S3CEV40)");
	Uart_Printf("\n\rTimer Test Example\n");
    
	do{
      Uart_Printf("\n <<Timer0>> Test. Y/y to continue,any key skip it.\n");
      input_char = Uart_Getch();
      if(input_char == 'Y' || input_char == 'y')
         Test_Timer();
         
      Uart_Printf("\n <<Watchdog Timer>> Test. Y/y to continue,any key skip it.\n");
      input_char = Uart_Getch();
      if(input_char == 'Y' || input_char == 'y')
         Test_WDTimer();
    	
	}while(Uart_Getch());
}
