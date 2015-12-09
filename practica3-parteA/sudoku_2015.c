/*
 * sudoku_2015.c
 *
 *  Created on: 24/11/2015
 *      Author: a604409
 */

#include "8led.h"
#include "Button.h"
#include "sudoku_2015.h"
#include <inttypes.h>

#define BLINK_SYMBOL_TIME 500

typedef enum {
	esperando_fila, esperando_columna, esperando_valor
} Game_state;

inline void celda_poner_valor(CELDA *celdaptr, uint8_t val) {
	*celdaptr = ((*celdaptr) & 0x0FFF) | ((val & 0x000F) << 12);
}

inline int es_error(CELDA celda) {
	return celda & 0b0000001000000000;
}

inline int es_pista(CELDA celda) {
	return celda & 0x0800;
}

inline int es_candidato(CELDA celda, uint8_t valor) {
	return celda & (1 << (valor - 1));
}

inline uint8_t celda_leer_valor(CELDA celda) {
	return celda >> 12;
}

inline void celda_cambiar_candidatos(uint8_t valor,
		CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS], uint8_t fila,
		uint8_t columna) {
	cuadricula[fila][columna] = cuadricula[fila][columna] & ~(1 << (valor - 1));
}

void init_game(void) {
	Game_state estadoJuego = esperando_fila;
	int fila = 0;
	int columna = 0;
	int valor = 0;
	D8Led_blink_symbol(15, BLINK_SYMBOL_TIME);
	while (1) {
		if (Button_next()) {
			Button_low_next();
			switch (estadoJuego) {
			case esperando_fila:
				fila = Button_valor_actual();
				if (fila != 0) {
					D8Led_blink_symbol(12, BLINK_SYMBOL_TIME); // TODO: pantalla hardcoded, cambiar
					Button_set_valor_actual(0);
					estadoJuego = esperando_columna;
				} else {
					D8Led_blink_symbol(15, BLINK_SYMBOL_TIME); // TODO: pantalla hardcoded, cambiar
				}
				break;
			case esperando_columna:
				columna = Button_valor_actual();
				if (columna != 0) {
					D8Led_blink_symbol(13, BLINK_SYMBOL_TIME);
					Button_set_valor_actual(0);
					estadoJuego = esperando_valor;
				} else {
					D8Led_blink_symbol(12, 500); // TODO: pantalla hardcoded, cambiar
				}
				break;
			case esperando_valor:
				valor = Button_valor_actual();
				D8Led_blink_symbol(15, BLINK_SYMBOL_TIME);
				Button_set_valor_actual(0);
				estadoJuego = esperando_fila;
				break;
			default:
				break;
			}
		}
	}
}

int sudoku_recalcular(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS]) {
	int celdas_vacias = 0;
	int errores = 0;
	int fila = 0;
	while (fila < (NUM_REGION * TAM_REGION)) {
		int columna = 0;
		while (columna < (NUM_REGION * TAM_REGION)) {
			switch (sudoku_candidatos_arm(cuadricula, fila, columna)) {
			case -1:
				errores = 1;
				break;
			case 0:
				celdas_vacias++;
				break;
			case 1:
				break;
			default:
				break;
			}
			columna += 1;
		}
		fila += 1;
	}
	//retornar el numero de celdas vacias, siendo negativo si hay errores
	return errores ? -celdas_vacias : celdas_vacias;
}

