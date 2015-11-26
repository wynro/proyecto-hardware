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
extern void Dhcp_Test(void);

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
    sys_init();        /* Initial 44B0X's Interrupt,Port and UART */
    _Link();           /* Print Misc info */
	
	/******************/
	/* user interface */
	/******************/
	Uart_Printf("\n\rEmbest 44B0X Evaluation Board(S3CEV40)");
	Uart_Printf("\n\rEthernet DHCP client Test Example\n");
	Dhcp_Test();
	for( ; ; )
	{
		Delay(500);
	}
}

