/**
 * Fichero que contiene las funciones C del sistema
 *
 * Autor: Guillermo Robles Gonzalez (604409), Sergio Martin Segura (622612)
 */

#include <inttypes.h>
#include <string.h> // Para memcmp()
#include "8led.h"
#include "sudoku_2015.h"
#include "Button_rebound.h"
#include "arm_recalcular.h"
#define NUM_TEST 1
#define BLINK_SYMBOL_TIME 500

// Numero de repeticiones que hara en las pruebas
// La información de cada celda está codificada en 16 bits
// con el siguiente formato (empezando en el bit más significativo):
// 4 MSB VALOR
// 1 bit PISTA
#define CELDA_PISTA(c) (c & 0b0000100000000000)
// 1 bit ERROR
#define CELDA_ERROR(c) (c & 0b0000010000000000)
#define CELDA_PONER_ERROR(c) (c |= 0b0000010000000000)
// 1 bit no usado
// 9 LSB CANDIDATOS

//CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS] __attribute__((align(4))) = {
//		{ 0x9800, 0x6800, 0x0000, 0x0006, 0x0000, 0x0000, 0x7800, 0x0011, 0x8800, 0, 0, 0, 0, 0, 0, 0 },
//		{ 0x8800, 0x0052, 0x0000, 0x0000, 0x0000, 0x4800, 0x3800, 0x0131, 0x0103, 0, 0, 0, 0, 0, 0, 0 },
//		{ 0x1800, 0x0046, 0x000C,
//		0x5800, 0x00E2, 0x00E0, 0x010A, 0x0120, 0x010A, 0, 0, 0, 0, 0, 0, 0 }, {
//		0x0018, 0x0114, 0x011C, 0x008A, 0x009A, 0x0080, 0x1800, 0x7800, 0x6800,
//		0, 0, 0, 0, 0, 0, 0 }, { 0x2800, 0x0001, 0x0028, 0x00E8, 0x9800, 0x3800,
//		0x0088, 0x0080, 0x5800, 0, 0, 0, 0, 0, 0, 0 }, { 0x7800, 0x0115, 0x8800,
//		0x002A, 0x003B, 0x0021, 0x010A, 0x0104, 0x010E, 0, 0, 0, 0, 0, 0, 0 }, {
//		0x0030, 0x0110, 0x7800, 0x00A0, 0x3800, 0x2800, 0x0180, 0x4800, 0x0101,
//		0, 0, 0, 0, 0, 0, 0 }, { 0x3800, 0x8800, 0x2800, 0x1800, 0x0048, 0x5800,
//		0x6800, 0x0100, 0x0140, 0, 0, 0, 0, 0, 0, 0 }, { 0x0020, 0x4800, 0x1800,
//		0x00E0, 0x00E0, 0x9800, 0x5800, 0x2800, 0x0044, 0, 0, 0, 0, 0, 0, 0 } };

/**
 * Funcion que actualiza el valor en la casilla pasada
 *
 * @param celdaptr	Puntero a la celda a modificar
 * @param val		Nuevo valor que se le pondra a la celda
 */
inline void celda_poner_valor(CELDA *celdaptr, uint8_t val) {
	*celdaptr = (*celdaptr & 0x0FFF) | ((val & 0x000F) << 12);
}

inline uint8_t celda_leer_valor(CELDA celda) {
	return celda >> 12;
}

inline void celda_cambiar_candidatos(uint8_t valor,
		CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS], uint8_t fila,
		uint8_t columna) {
	cuadricula[fila][columna] = cuadricula[fila][columna] & ~(1 << (valor - 1));
}
// funcion a implementar en ARM
//extern int sudoku_recalcular_arm(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS]);

// funcion a implementar en Thumb
//extern int sudoku_candidatos_thumb(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS], uint8_t fila, uint8_t columna);

extern int sudoku_candidatos_arm(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS],
		uint8_t fila, uint8_t columna);

extern int sudoku_candidatos_thumb_prologo(
		CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS], uint8_t fila,
		uint8_t columna);

int sudoku_candidatos_c(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS], uint8_t fila,
		uint8_t columna) {
	//iniciar candidatos
	// inicialmente todos a 1 (todos los números son candidatos), despues los iremos quitando
	if (celda_leer_valor(cuadricula[fila][columna]) == 0) {
		cuadricula[fila][columna] = (cuadricula[fila][columna] | 0x01FF);
		//recorrer fila y columna recalculando candidatos
		// TODO: Mirar la diferencia entre for y while en altas prestaciones
		// TODO: mirar como antes se declaraban las variables al principio de bloque
		int i = 0;
		while (i < 9) {
			uint8_t valor_celda_fila = celda_leer_valor(cuadricula[i][columna]);
			uint8_t valor_celda_columna = celda_leer_valor(cuadricula[fila][i]);
			celda_cambiar_candidatos(valor_celda_fila, cuadricula, fila,
					columna);
			celda_cambiar_candidatos(valor_celda_columna, cuadricula, fila,
					columna);
			i += 1;
		}

		//recorrer region recalculando candidatos
		// FIXME:
		// fila2 = (fila/3)*3
		uint8_t fila2 = fila - fila % TAM_REGION; // Obtenemos la fila inicial de la región
		uint8_t columna2 = columna - columna % TAM_REGION; // Obtenemos la fila inicial de la región
		// Recorrido por columnas
		// Posibilidad de quitar los *0xF, en esta situación es indistinto (asumiendo que almacenamos NUM_COLUMNAS*0xF),
		// dado que se realizan de todas formas 3 productos, en caso de regiones más grandes merece aún más la pena,
		// en caso de regiones más pequeñas es mejor quitarlo.
		int recorre_fila = fila2;
		while (recorre_fila < fila2 + TAM_REGION) {
			int recorre_columna = columna2;
			while (recorre_columna < columna2 + TAM_REGION) {
				uint8_t valor_celda = celda_leer_valor(
						cuadricula[recorre_fila][recorre_columna]);
				celda_cambiar_candidatos(valor_celda, cuadricula, fila,
						columna);
				recorre_columna += 1;
			}
			recorre_fila += 1;
		}
	}
	//retornar indicando si la celda tiene un valor o esta vacia
	if ((cuadricula[fila][columna] & 0xF000) != 0)
		return TRUE;
	return FALSE;

}

int sudoku_recalcular_c_c(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS]) {
	int celdas_vacias = 0;
	// Para cada fila
	int fila = 0;
	while (fila < (NUM_REGION * TAM_REGION)) {
		int columna = 0;
		while (columna < (NUM_REGION * TAM_REGION)) {
			if (!sudoku_candidatos_c(cuadricula, fila, columna)) {
				// FALSE devuelto=>Celda vacía
				celdas_vacias++;
			}
			columna += 1;
		}
		fila += 1;
	}
	//retornar el numero de celdas vacias
	return celdas_vacias;
}

int sudoku_recalcular_c_a(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS]) {
	int celdas_vacias = 0;
	// Para cada fila
	int fila = 0;
	while (fila < (NUM_REGION * TAM_REGION)) {
		int columna = 0;
		while (columna < (NUM_REGION * TAM_REGION)) {
			if (!sudoku_candidatos_arm(cuadricula, fila, columna)) {
				// FALSE devuelto=>Celda vacía
				celdas_vacias++;
			}
			columna += 1;
		}
		fila += 1;
	}
	//retornar el numero de celdas vacias
	return celdas_vacias;
}

int sudoku_recalcular_c_a_2(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS]) {
	int celdas_vacias = 0;
	int errores = 0;
	int fila = 0;
	while (fila < (NUM_REGION * TAM_REGION)) {
		int columna = 0;
		while (columna < (NUM_REGION * TAM_REGION)) {
			switch (sudoku_candidatos_arm_2(cuadricula, fila, columna)) {
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

int sudoku_recalcular_c_t(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS]) {
	int celdas_vacias = 0;
	// Para cada fila
	int fila = 0;
	while (fila < (NUM_REGION * TAM_REGION)) {
		int columna = 0;
		while (columna < (NUM_REGION * TAM_REGION)) {
			if (!sudoku_candidatos_thumb_prologo(cuadricula, fila, columna)) {
				// FALSE devuelto=>Celda vacía
				celdas_vacias++;
			}
			columna += 1;
		}
		fila += 1;
	}
	//retornar el numero de celdas vacias
	return celdas_vacias;
}

void sudoku9x9(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS], char *ready) {
	sudoku_recalcular_c_a(cuadricula);
	// unsigned int celdas_vacias; //numero de celdas aun vacias
	// celdas_vacias = sudoku_recalcular_c(cuadricula);
	// Mandamos la señal de ready
	*ready = 1;
}

void init_game(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS]) {
	// TODO: recalcular todos los candidatos
	char ready = 0;
	Game_state estado = esperando_fila;
	int fila = 0;
	int columna = 0;
	int valor = 0;
	sudoku_recalcular_c_a_2(cuadricula);
	D8Led_blink_symbol(15, BLINK_SYMBOL_TIME);
	int r45tgfde3 = 0;
	while (1) {
		if (Button_next()) {
			Button_low_next();
			switch (estado) {
			case esperando_fila:
				fila = Button_valor_actual();
				if (fila != 0) {
					D8Led_blink_symbol(12, BLINK_SYMBOL_TIME); // TODO: pantalla hardcoded, cambiar
					Button_set_valor_actual(0);
					// TODO: resetea cuenta
					estado = esperando_columna;
				} else {
					D8Led_blink_symbol(15, BLINK_SYMBOL_TIME); // TODO: pantalla hardcoded, cambiar
				}
				break;
			case esperando_columna:
				columna = Button_valor_actual();
				if (columna != 0) {
					D8Led_blink_symbol(13, BLINK_SYMBOL_TIME); // TODO: pantalla hardcoded, cambiar
					Button_set_valor_actual(0);
					estado = esperando_valor;
				} else {
					D8Led_blink_symbol(12, BLINK_SYMBOL_TIME); // TODO: pantalla hardcoded, cambiar
				}
				break;
			case esperando_valor:
				valor = Button_valor_actual();
				// Anadimos porcelana para introducir datos en el sudoku?? // !TODO: Check
//				celda_poner_valor(&cuadricula[fila][columna], valor); // Si es cero se pone lo mismo
//				sudoku_recalcular_c_a_2(cuadricula);
				CELDA *a = &cuadricula[0][0];
				celda_poner_valor(&cuadricula[fila-1][columna-1], valor);
				sudoku_recalcular_c_a_2(cuadricula);
				D8Led_blink_symbol(15, BLINK_SYMBOL_TIME);
				Button_set_valor_actual(0);
				estado = esperando_fila;
				break;
			default:
				break;
			}
		}
	}
}
