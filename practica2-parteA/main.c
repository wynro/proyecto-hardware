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
#include "Button_rebound.h"
#include "sudoku_2015.h"
#include "Timer2.h"
#include "8led.h"
#include "arm_recalcular.h"

#define NUM_PRUEBAS 1000

// Funciones de sudoku
//extern int sudoku_recalcular_c_c(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS]);
//extern int sudoku_recalcular_c_a(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS]);
//extern int sudoku_recalcular_c_t(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS]);
//
//extern int sudoku_recalcular_a_c(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS]);
//extern int sudoku_recalcular_a_a(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS]);
//extern int sudoku_recalcular_a_t(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS]);

static CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS]__attribute__((align(4))) = {
	{0x9800,0x6800,0x0000,0x0000,0x0000,0x0000,0x7800,0x0000,0x8800,0,0,0,0,0,0,0},
	{0x8800,0x0000,0x0000,0x0000,0x0000,0x4800,0x3800,0x0000,0x0000,0,0,0,0,0,0,0},
	{0x1800,0x0000,0x0000,0x5800,0x0000,0x0000,0x0000,0x0000,0x0000,0,0,0,0,0,0,0},
	{0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x1800,0x7800,0x6800,0,0,0,0,0,0,0},
	{0x2800,0x0000,0x0000,0x0000,0x9800,0x3800,0x0000,0x0000,0x5800,0,0,0,0,0,0,0},
	{0x7800,0x0000,0x8800,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0,0,0,0,0,0,0},
	{0x0000,0x0000,0x7800,0x0000,0x3800,0x2800,0x0000,0x4800,0x0000,0,0,0,0,0,0,0},
	{0x3800,0x8800,0x2800,0x1800,0x0000,0x5800,0x6800,0x0000,0x0000,0,0,0,0,0,0,0},
	{0x0000,0x4800,0x1800,0x0000,0x0000,0x9800,0x5800,0x2800,0x0000,0,0,0,0,0,0,0}
};

void Main(void);

/*--- codigo de funciones ---*/
void Main(void) {

	/* Inicializa controladores */
	sys_init();        // Inicializacion de la placa, interrupciones y puertos
	//timer_init();	   // Inicializacion del temporizador

	D8Led_init(); // inicializamos el 8led

	//Eint4567_init();
	Button_init(0, 9); // inicializamos los pulsadores. Cada vez que se pulse se verá reflejado en el 8led
//	led1_on();

	// Medida de tiempos
	Timer2_Inicializar();

//	int i;
//	int tiempo_total_c_a = 0;
//	int tiempo_total_c_c = 0;
//	int tiempo_total_c_t = 0;
//	int tiempo_total_a_a = 0;
//	int tiempo_total_a_c = 0;
//	int tiempo_total_a_t = 0;
//
//	double media_c_a = 0;
//	double media_c_c = 0;
//	double media_c_t = 0;
//	double media_a_a = 0;
//	double media_a_c = 0;
//	double media_a_t = 0;
//
//	for (i = 0; i < NUM_PRUEBAS; ++i) {
//		Timer2_Empezar();
//		sudoku_recalcular_c_c(cuadricula);
//		tiempo_total_c_c += Timer2_Leer();
//	}
//	media_c_c = tiempo_total_c_c / NUM_PRUEBAS;
//
//	for (i = 0; i < NUM_PRUEBAS; ++i) {
//		Timer2_Empezar();
//		sudoku_recalcular_c_a(cuadricula);
//		tiempo_total_c_a += Timer2_Leer();
//	}
//	media_c_a = tiempo_total_c_a / NUM_PRUEBAS;
//
//	for (i = 0; i < NUM_PRUEBAS; ++i) {
//		Timer2_Empezar();
//		sudoku_recalcular_c_t(cuadricula);
//		tiempo_total_c_t += Timer2_Leer();
//	}
//	media_c_t = tiempo_total_c_t /NUM_PRUEBAS;
//
//	for (i = 0; i < NUM_PRUEBAS; ++i) {
//		Timer2_Empezar();
//		sudoku_recalcular_a_c(cuadricula);
//		tiempo_total_a_c += Timer2_Leer();
//	}
//	media_a_c = tiempo_total_a_c / NUM_PRUEBAS;
//
//	for (i = 0; i < NUM_PRUEBAS; ++i) {
//		Timer2_Empezar();
//		sudoku_recalcular_a_a(cuadricula);
//		tiempo_total_a_a += Timer2_Leer();
//	}
//	media_a_a = tiempo_total_a_a / NUM_PRUEBAS;
//
//	for (i = 0; i < NUM_PRUEBAS; ++i) {
//		Timer2_Empezar();
//		sudoku_recalcular_a_t(cuadricula);
//		tiempo_total_a_t += Timer2_Leer();
//	}
//	media_a_t = tiempo_total_a_t / NUM_PRUEBAS;
//
//	int y = 1;

	// Iniciamos el juego
	init_game(cuadricula);
	//sudoku_recalcular_c_a_2(cuadricula);

//	while (1) {
//		// HeartBeat
//		if (switch_leds == 1) {
//			leds_switch();
//			switch_leds = 0;
//		}
}

