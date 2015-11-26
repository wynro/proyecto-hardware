/*********************************************************************************************
* File��	main.c
* Author:	embest
* Desc��	c main entry
* History:	
*********************************************************************************************/

/*--- include files ---*/
#include "44blib.h"
#include "44b.h"

/*--- function declare ---*/
void main(void);
void init_Ext4567(void);

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
		
    while(1)
     {
	   Uart_Printf("\n > Embest S3CEV40 board. < ");
       Uart_Printf("\n <<Extenal Interrupt>> Test. Y/y to continue,any key skip it.\n");
       input_char = Uart_Getch();
       if(input_char == 'Y' || input_char == 'y')
         Test_Eint();
     }
}

