/*
 * sudoku_2015.h
 *
 *  Created on: 18/11/2015
 *      Author: wynro
 */

#ifndef SUDOKU_2015_H_
#define SUDOKU_2015_H_

#define CELDA_PISTA(c) (c & 0b0000100000000000)
#define CELDA_ERROR(c) (c & 0b0000010000000000)
#define CELDA_PONER_ERROR(c) (c |= 0b0000010000000000)

typedef uint16_t CELDA;
typedef enum {
	esperando_fila, esperando_columna, esperando_valor
} Game_state;
enum {
	// Tamaños de la cuadricula
	// Se utilizan 16 columnas para facilitar la visualización
	NUM_FILAS = 9,
	NUM_COLUMNAS = 16,
	TAM_REGION = 3,
	NUM_REGION = 3
};
enum {
	FALSE = 0, TRUE = 1
};

extern CELDA cuadricul[NUM_FILAS][NUM_COLUMNAS];

/**
 * Funcion que actualiza el valor en la casilla pasada
 *
 * @param celdaptr	Puntero a la celda a modificar
 * @param val		Nuevo valor que se le pondra a la celda
 */
inline void celda_poner_valor(CELDA *celdaptr, uint8_t val);

/**
 * Funcion que extraera el valor de una celda
 *
 * @param celda		Celda a leer
 * @return			Valor numerico de la celda (0 si es vacia)
 */
inline uint8_t celda_leer_valor(CELDA celda);

/**
 * Funcion que modifica los posibles candidatos de una celda, eliminando una
 * pista dada
 *
 * @param	valor		Pista que se eliminara
 * @param	cuadricula	Cuadricula a modificar
 * @param	fila		Coordenada fila de la casilla a modificar
 * @param	columna		Coordenada columna de la casilla a modificar
 */
inline void celda_cambiar_candidatos(uint8_t valor,
		CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS], uint8_t fila,
		uint8_t columna);

/**
 * Funcion que dado una casilla en una cuadricula, actualiza sus candidatos
 *
 * @param	cuadricula	Cuadricula a modificar
 * @param	fila		Coordenada fila de la casilla a modificar
 * @param	columna		Coordenada columna de la casilla a modificar
 *
 * @return	0 en caso de que la casilla este vacia, !=0 en caso contrario
 */
extern int sudoku_candidatos_arm(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS],
		uint8_t fila, uint8_t columna);

/**
 * Funcion que dado una casilla en una cuadricula, actualiza sus candidatos
 *
 * @param	cuadricula	Cuadricula a modificar
 * @param	fila		Coordenada fila de la casilla a modificar
 * @param	columna		Coordenada columna de la casilla a modificar
 *
 * @return	0 en caso de que la casilla este vacia, !=0 en caso contrario
 */
extern int sudoku_candidatos_thumb_prologo(
		CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS], uint8_t fila,
		uint8_t columna);

/**
 * Funcion que dado una casilla en una cuadricula, actualiza sus candidatos
 *
 * @param	cuadricula	Cuadricula a modificar
 * @param	fila		Coordenada fila de la casilla a modificar
 * @param	columna		Coordenada columna de la casilla a modificar
 *
 * @return	0 en caso de que la casilla este vacia, !=0 en caso contrario
 */
int sudoku_candidatos_c(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS], uint8_t fila,
		uint8_t columna);

/**
 * Funcion que dado una cuadricula, actualiza todos sus candidatos, usando
 * como hoja una funcion C
 * @param	cuadricula	Cuadricula a modificar
 *
 * @return	Numero de casillas vacias
 */
int sudoku_recalcular_c_c(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS]);

/**
 * Funcion que dado una cuadricula, actualiza todos sus candidatos, usando
 * como hoja una funcion ARM
 * @param	cuadricula	Cuadricula a modificar
 *
 * @return	Numero de casillas vacias
 */
int sudoku_recalcular_c_a(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS]);

/**
 * Funcion que dado una cuadricula, actualiza todos sus candidatos y
 * reajusta los errores como sea necesario, usando como hoja una
 * funcion ARM.
 *
 * @param	cuadricula	Cuadricula a modificar
 *
 * @return	Numero de casillas vacias, negativo en caso de que haya errores
 */
int sudoku_recalcular_c_a_2(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS]);

/**
 * Funcion que dado una cuadricula, actualiza todos sus candidatos, usando
 * como hoja una funcion THUMB
 * @param	cuadricula	Cuadricula a modificar
 *
 * @return	Numero de casillas vacias
 */
int sudoku_recalcular_c_t(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS]);

void sudoku9x9(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS], char *ready);

void init_game();

#endif /* SUDOKU_2015_H_ */
