/** @file sudoku_2015.h
 *  @brief Modulo que contiene funciones relacionadas con el analisis
 *  de sudokus
 * 
 * @author Guillermo Robles Gonzalez
 */

#ifndef SUDOKU_2015_H_
#define SUDOKU_2015_H_
#include <inttypes.h>

#define CELDA uint16_t
/**
 * @brief Informacion de la cuadricula
 */
enum {
	// Tamaños de la cuadricula
	// Se utilizan 16 columnas para facilitar la visualización
	NUM_FILAS = 9,
	NUM_COLUMNAS = 16,
	TAM_REGION = 3,
	NUM_REGION = 3
};

/**
 * Inicializa el juego
 */
void init_game(void);

/**
 * Funcion que modifica los posibles candidatos de una celda,
 * eliminando una pista dada
 *
 * @param[in]	valor		Pista que se eliminara
 * @param[in]	cuadricula	Cuadricula a modificar
 * @param[in]	fila		Coordenada fila de la casilla a modificar
 * @param[in]	columna		Coordenada columna de la casilla a modificar
 */
void celda_cambiar_candidatos(uint8_t valor,
		CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS], uint8_t fila,
		uint8_t columna);

/**
 * Funcion que dado una casilla en una cuadricula, actualiza sus
 * candidatos
 *
 * @param[in]	cuadricula	Cuadricula a modificar
 * @param[in]	fila		Coordenada fila de la casilla a modificar
 * @param[in]	columna		Coordenada columna de la casilla a modificar
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
 * @param[out]	cuadricula	Cuadricula a modificar
 *
 * @return	Numero de casillas vacias, o -1 en caso de que haya errores
 */
int sudoku_recalcular(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS]);

/**
 * Devuelve un numero mayor de 0 si y solo si la celda esta marcada
 * como error, devuelve 0 en caso contrario
 *
 * @param[in]	celda Celda a comprobar
 *
 * @return 0 en caso de que la celda sea correcta
 */
int celda_es_error(CELDA celda);

/**
 * Devuelve un numero mayor que cero en caso de que la celda sea pista inicial,
 * devuelve 0 en caso contrario
 *
 * @param[in] celda Celda a comprobar
 *
 * @return 0 si y solo si la celda no es pista
 */
int celda_es_pista(CELDA celda);

/**
 * Devuelve un numero mayor que cero en caso de que la celda sea pista
 * inicial, devuelve 0 en caso contrario
 *
 * @param[in] celda Celda a comprobar
 *
 * @return 0 si y solo si la celda no es pista
 */
int celda_es_candidato(CELDA celda, uint8_t valor);

/**
 * Actualiza el valor de la celda dada al valor dado
 *
 * @param[out] celda Celda a cambiar
 * @param[in]  val	Nuevo valor
 */
void celda_poner_valor(CELDA *celdaptr, uint8_t val);

/**
 * Devuelve el numero en la celda dada (0 si es vacia)
 *
 * @param[out] celda Celda a consultar
 */
inline uint8_t celda_leer_valor(CELDA celda);

/**
 * Pone cualquier casilla que no sea pista a 0
 *
 * @param[out] cuadricuala Cuadricula a editar
 */
void sudoku_vacia_tabla(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS]);

#endif /* SUDOKU_2015_H_ */
