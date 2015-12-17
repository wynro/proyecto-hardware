/*********************************************************************************************
 * File£º	main.c
 * Author:	embest
 * Desc£º	c main entry
 * History:
 *********************************************************************************************/

/*--- include files ---*/
#include "44blib.h"
#include "44b.h"
#include "iis.h"

/*--- function declare ---*/
void Main(void);

/*--- extern function ---*/

/*--- function code ---*/
/*********************************************************************************************
 * name:		main
 * func:		c code entry
 * para:		none
 * ret:		none
 * modify:
 * comment:
 *********************************************************************************************/
void Main(void) {
	// RESTORE ES EL COMANDO DE GDB
	sys_init(); /* Initial 44B0X's Interrupt,Port and UART */

	int i = 1;
	i++;

	/* printf interface */
	while (1) {
		/******************/
		/* user interface */
		/******************/
		i++;
//		Record_Iis();// test record
		IISInit(); // initialize IIS
		Playwave(1); // play wave 5 times
		IISClose();	 // close IIS
	}
}

