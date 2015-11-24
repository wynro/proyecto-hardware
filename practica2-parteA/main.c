/*********************************************************************************************
 * Fichero:	main.c
 * Autor:
 * Descrip:	punto de entrada de C
 * Version:  <P4-ARM.timer-leds>
 *********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "44blib.h"
#include "44b.h"
#include "stdio.h"
#include <inttypes.h>
#include "Excepciones.h"
#include "Button.h"
#include "8led.h"
#include "sudoku_2015.h"


void Main(void);


void Main(void) {
	/* Inicializa controladores */
	sys_init();        // Inicializacion de la placa, interrupciones y puertos
	D8Led_init(); // inicializamos el 8led
	Button_init(0, 9); // inicializamos los pulsadores. Cada vez que se pulse se verá reflejado en el 8led

	init_game();
}
