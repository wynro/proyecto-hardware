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
#include "button2.h"

/*--- variables globales ---*/
extern int switch_leds;
extern int on_led_0;
extern int on_led_1;
//extern int valor_pantalla;

/*--- funciones externas ---*/
extern void leds_switch();
extern void timer_init();
extern void D8Led_init();

extern void doSWI(uint32_t T);
/*--- declaracion de funciones ---*/
void Main(void);

/*--- codigo de funciones ---*/
void Main(void) {

	/* Inicializa controladores */
	sys_init();        // Inicializacion de la placa, interrupciones y puertos
	timer_init();	   // Inicializacion del temporizador

	D8Led_init(); // inicializamos el 8led

	//Eint4567_init();
	Button_init(); // inicializamos los pulsadores. Cada vez que se pulse se verá reflejado en el 8led

	/* Valor inicial de los leds */
//	Timer2_Inicializar();
//	Timer2_Empezar();
//	int res = Timer2_Leer();
	//excepcion_init();
	//trataExcepcion();
	// doSWI(1);
	// Delay(1000);
	while (1) {
		// Cambia los leds con cada interrupcion del temporizador
		if (switch_leds == 1) {
			leds_switch();
			switch_leds = 0;
		}

	}
}
