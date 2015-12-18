/*********************************************************************************************
 * Fichero:	main.c
 * Autor:
 * Descrip:	codigo de entrada de C
 * Version:	<P6-ARM>
 *********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "44blib.h"
#include "44b.h"
#include "lcd.h"
#include "sudoku_graphics.h"
#include "sudoku_2015.h"
#include "Button.h"
#include "Timer2.h"
#include "Bmp.h"
#include "string.h"
#include "still-alive-lyrics.h"
#include "sudoku_collection_san.h"

/*--- declaracion de funciones ---*/
void Main(void);


/*--- codigo de la funcion ---*/
void Main(void) {
	sys_init(); /* inicializacion de la placa, interrupciones, puertos y UART */

	/* initial LCD controller */
	Lcd_Init();
	/* clear screen */
	Lcd_Clr();
	Lcd_Active_Clr();

	init_game();

}

