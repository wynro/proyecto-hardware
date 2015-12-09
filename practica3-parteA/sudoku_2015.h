/*
 * sudoku_2015.h
 *
 *  Created on: 24/11/2015
 *      Author: a604409
 */

#ifndef SUDOKU_2015_H_
#define SUDOKU_2015_H_
#include <inttypes.h>

#define CELDA uint16_t
enum {
	// Tamaños de la cuadricula
	// Se utilizan 16 columnas para facilitar la visualización
	NUM_FILAS = 9,
	NUM_COLUMNAS = 16,
	TAM_REGION = 3,
	NUM_REGION = 3
};

void init_game(void);

/**
 * Funcion que modifica los posibles candidatos de una celda, eliminando una
 * pista dada
 *
 * @param	valor		Pista que se eliminara
 * @param	cuadricula	Cuadricula a modificar
 * @param	fila		Coordenada fila de la casilla a modificar
 * @param	columna		Coordenada columna de la casilla a modificar
 */
void celda_cambiar_candidatos(uint8_t valor,
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
int sudoku_candidatos_arm(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS],
		uint8_t fila, uint8_t columna);

/**
 * Funcion que dado una cuadricula, actualiza todos sus candidatos y
 * reajusta los errores como sea necesario, usando como hoja una
 * funcion ARM.
 *
 * @param	cuadricula	Cuadricula a modificar
 *
 * @return	Numero de casillas vacias, negativo en caso de que haya errores
 */
int sudoku_recalcular(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS]);

/**
 * Devuelve un numero mayor de 0 si y solo si la celda esta marcada como error, devuelve 0 en caso contrario
 *
 * @param 	celda Celda a comprobar
 *
 * @return 0 en caso de que la celda sea correcta
 */
int es_error(CELDA celda);

void celda_poner_valor(CELDA *celdaptr, uint8_t val);

#endif /* SUDOKU_2015_H_ */
