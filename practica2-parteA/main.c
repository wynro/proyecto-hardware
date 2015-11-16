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

/*--- variables globales ---*/
extern int switch_leds;
//extern int valor_pantalla;

/*--- funciones externas ---*/
extern void leds_switch();
extern void timer_init();
extern void D8Led_init();

extern void doSWI(uint32_t T);
/*--- declaracion de funciones ---*/

typedef enum {
	esperando_fila, esperando_columna, esperando_valor
} Game_state;

void Main(void);
void led1_on(void);

/*--- codigo de funciones ---*/
void Main(void) {

	/* Inicializa controladores */
	sys_init();        // Inicializacion de la placa, interrupciones y puertos
	timer_init();	   // Inicializacion del temporizador

	D8Led_init(); // inicializamos el 8led

	//Eint4567_init();
	Button_init(1,9); // inicializamos los pulsadores. Cada vez que se pulse se verá reflejado en el 8led

	/* Valor inicial de los leds */
//	Timer2_Inicializar();
//	Timer2_Empezar();
//	int res = Timer2_Leer();
	//excepcion_init();
	//trataExcepcion();
	// doSWI(1);
	// Delay(1000);
	led1_on();
	Game_state estado = esperando_fila;
	while (1) {
		int num_fila = 0;
		int num_columna = 0;
		int valor = 0;
		// Cambia los leds con cada interrupcion del temporizador
		// HeartBeat
		if (switch_leds == 1) {
			leds_switch();
			switch_leds = 0;
		}
//
//		switch (estado) {
//		case esperando_fila:
//			if(next && )
//			break;
//		case esperando_columna:
//
//			break;
//		case esperando_valor:
//
//			break;
//		default:
//			break;
//		}
//		Delay(2);
//		D8Led_symbol(0);
//		Delay(2);
//		D8Led_symbol(num_fila);
//		Delay(2);
//		D8Led_symbol(num_columna);
//		Delay(2);
//		D8Led_symbol(valor);

	}
}
