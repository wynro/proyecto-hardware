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

/*--- variables globales ---*/
extern int switch_leds;
extern int on_led_0;
extern int on_led_1;
//extern int valor_pantalla;

/*--- funciones externas ---*/
extern void leds_off();
extern void led1_on();
extern void leds_switch();
extern void timer_init();
extern void Eint4567_init();
extern void D8Led_init();
extern void D8Led_symbol();

/*--- declaracion de funciones ---*/
void Main(void);

/*--- codigo de funciones ---*/
void Main(void) {

	/* Inicializa controladores */
	sys_init();        // Inicializacion de la placa, interrupciones y puertos
	timer_init();	   // Inicializacion del temporizador

	Eint4567_init();// inicializamos los pulsadores. Cada vez que se pulse se verá reflejado en el 8led
	D8Led_init(); // inicializamos el 8led

	/* Valor inicial de los leds */
	//leds_off();
	//led1_on();
	int valor_pantalla = 0;
	while (1) {
		/* Cambia los leds con cada interrupcion del temporizador */
		if (switch_leds == 1) {
//			D8Led_symbol(valor_pantalla);
//			valor_pantalla = (valor_pantalla + 1) % 16;
//			if (on_led_0 == 1) {
//				led0_on();
//			} else {
//				led0_off();
//			}
//
//			if (on_led_1 == 1) {
//				led1_on();
//				on_led_1 = 0;
//			} else {
//				led1_off();
//			}

			leds_switch();
			switch_leds = 0;
		}
	}
}
