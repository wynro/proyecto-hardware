/*
 * sudoku_graphics.h
 *
 *  Created on: 26/11/2015
 *      Author: a604409
 */

#ifndef SUDOKU_GRAPHICS_H_
#define SUDOKU_GRAPHICS_H_

#include "sudoku_2015.h"
#include "44blib.h"
#include "44b.h"
#include "def.h"

#define SUDOKU_NUM_CUADS 9
#define SUDOKU_SQUARE_SIZE 18
#define SUDOKU_X0 20
#define SUDOKU_Y0 20

#define SUDOKU_FONT_HEIGHT 16
#define SUDOKU_FONT_LENGTH 8

#define ASCII_NUMBER_BASE 48

/**
 * Dibuja la base del sudoku (cuadricula, numeracion de la misma, frase de información)
 */
void sudoku_graphics_draw_base();
/**
 * Rellena el sudoku con la informacion de la cuadricula dada
 *
 * @param[in] cuadricula Cuadricula de la que se cojera la informacion
 */
void sudoku_graphics_fill_from_data(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS]);
/**
 * Actualiza el LCD con la informacion del sudoku
 */
void sudoku_graphics_update_lcd();
/**
 * Imprime la pantalla final, si el numero de errores es 0, se imprimira
 * la pantalla de exito, en caso contrario se imprimira un mensaje de fracaso.
 *
 * @param[in] tiempo_juego_ms Tiempo que ha durado el juego, en ms
 * @param[in] tiempo_calculos_ms Tiempo que han durado los calculos, en ms
 * @param[in] errores Numero de errores actual
 *
 */
void sudoku_graphics_print_final_screen(int tiempo_juego_ms,
		int tiempo_calculos_ms, int errores);

/**
 * Rellena una casilla con el numero dado
 *
 * @param[in] x Posicion x de la casilla
 * @param[in] y Posicion y de la casilla
 * @param[in] number Numero con el cual rellenarlo
 * @param[in] ucColor Color de letra
 */
inline void sudoku_graphics_put_number_in_square(INT8 x, INT8 y, INT8 number,
		INT8U ucColor);

/**
 * Remarca una casilla de forma visible al usuario
 *
 * @param[in] x Posicion x de la casilla
 * @param[in] y Posicion y de la casilla
 */
inline void sudoku_graphics_remark_square(INT8 x, INT8 y);

/**
 * Marca el error dado en la casilla dada
 *
 * @param[in] x Posicion x de la casilla
 * @param[in] y Posicion y de la casilla
 * @param[in] error Error a marcar (intervalo 1-9)
 */
inline void sudoku_graphics_mark_error(INT8 x, INT8 y, INT8 error);

/**
 * Remarca una marca de error en una casilla
 *
 * @param[in] x Posicion x de la casilla
 * @param[in] y Posicion y de la casilla
 * @param[in] error Error a remarcar
 */
void sudoku_graphics_remark_error_in_square(INT8 x, INT8 y, INT8 error);

/**
 * Imprime la pantalla de titulo
 */
void sudoku_graphics_print_title_screen();

/**
 * Imprime 6 lineas de Still Alive a partir de la linea dada
 *
 * @param[in] lineNumber Linea a partir de la cual comenzar a imprimir
 */
void sudoku_graphics_print_still_alive(int lineNumber);

#endif /* SUDOKU_GRAPHICS_H_ */
