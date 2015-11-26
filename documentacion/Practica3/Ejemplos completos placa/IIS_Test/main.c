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
extern void Test_Iis();

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
	char input_char;				/* user input char 			*/
	
    sys_init();        /* Initial 44B0X's Interrupt,Port and UART */
    _Link();           /* Print Misc info */
	
	/* printf interface */
    while(1)
     {
    /******************/
	/* user interface */
	/******************/
	   Uart_Printf("\n\r Embest 44B0X Evaluation Board(S3CEV40)");
	   Uart_Printf("\n\r CODEC: Philips UDA1341");
       Uart_Printf("\n <<IIS transmission>> Test. Y/y to continue,any key skip it.\n");
       input_char = Uart_Getch();
       if(input_char == 'Y' || input_char == 'y')
        Test_Iis();
        
       else if(input_char == 0x1b) break;// ESC·µ»Ø
     }
}

