/*
 * sudoku_graphics.h
 *
 *  Created on: 26/11/2015
 *      Author: a604409
 */

#ifndef SUDOKU_GRAPHICS_H_
#define SUDOKU_GRAPHICS_H_

#include "sudoku_2015.h"

#define SUDOKU_NUM_CUADS 9
#define SUDOKU_SQUARE_SIZE 18
#define SUDOKU_X0 20
#define SUDOKU_Y0 20

#define SUDOKU_FONT_HEIGHT 16
#define SUDOKU_FONT_LENGTH 8

#define ASCII_NUMBER_BASE 48

void sudoku_graphics_draw_base();
void sudoku_graphics_fill_from_data(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS]);
void sudoku_graphics_update_lcd();
void sudoku_graphics_print_final_screen(int tiempo_juego_ms,
		int tiempo_calculos_ms, int errores);

#endif /* SUDOKU_GRAPHICS_H_ */
