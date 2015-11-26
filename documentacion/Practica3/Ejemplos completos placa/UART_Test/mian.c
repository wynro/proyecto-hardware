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
char str_send[17] = "Embest S3CEV40 >\0";
char str_error[50] = "TERMINAL OVERFLOW : 256 character max !";
char str[256];
char CR[1] = {0x0A};

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
    int i;
    char *pt_str = str;
	
	Port_Init();					/* Initial 44B0X's I/O port */
	rI_ISPC = 0xffffffff;			/* clear all interrupt pend	*/
	Uart_Init(0,115200);			/* Initial Serial port 1	*/
	
	/* printf interface */
	Uart_Printf("\n");
	Uart_Printf(str_send);
	/* get user input */
	Delay(500);
    //* Terminal handler
    while(1)
    {
      //Test_Uart0()       ;
      //Test_Uart0Fifo()   ;
      //Test_Uart1()       ;
      //Test_Uart1Fifo()   ;
      //Test_Uart1IrDA_Rx();
      //Test_Uart1IrDA_Tx();
      //Test_UartAFC_Rx()  ;
      //Test_UartAFC_Tx()  ;

        *pt_str = Uart_Getch();
		Uart_SendByte(*pt_str);
        if (*pt_str == 0x0D)
        {
	        if (pt_str != str)
	        {
	            //* Send str_send
	            Uart_SendByte(CR[0]);
	            //* Send received string
	            pt_str = str;
	            while (*pt_str != 0x0D)
	            {
	                Uart_SendByte(*pt_str);
	        		pt_str++;
	            }
	            pt_str = str;
	        }
            Uart_SendByte(CR[0]);
			Uart_Printf(str_send);
        }
            else
                pt_str++;
    }
}
