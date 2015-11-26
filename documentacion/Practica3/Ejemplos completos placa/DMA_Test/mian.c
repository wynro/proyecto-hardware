/*********************************************************************************************
* File£º	main.c
* Author:	embest
* Desc£º	c main entry
* History:	
*********************************************************************************************/

/*--- include files ---*/
#include "44blib.h"
#include "44b.h"

char str_error[50] = "TERMINAL OVERFLOW : 256 character max !";

/*********************************************************************************************
* name:		main
* func:		c code entry
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
int Main(void)
{
	char input_char;				/* user input char 			*/
	
    sys_init();        /* Initial 44B0X's Interrupt,Port and UART */
    _Link();           /* Print Misc info */
	
	/* printf interface */
    while(1)
     {
	   Uart_Printf("\n > Embest S3CEV40 board. < ");
       Uart_Printf("\n <<DMA transmission>> Test. Y/y to continue,any key skip it.\n");
       input_char = Uart_Getch();
       if(input_char == 'Y' || input_char == 'y')
         {
            Test_Zdma0();
            Test_Zdma1();
         }
       if(input_char == 0x1b) break;// ESC·µ»Ø
     }
     
    return 1;
}
