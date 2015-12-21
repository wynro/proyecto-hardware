/*
 * @file title-bitmap.h
 * @brief Archivo de recursos con la imagen del titulo
 */
#ifndef TITLE_BITMAP_H_
#define TITLE_BITMAP_H_

#include "lcd.h"
#include "bmp.h"

#define TITLE_BITMAP_HEIGHT 47
#define TITLE_BITMAP_WIDTH 195

// Eclipse crashea si intento indentarlo, y no pienso hacerlo a mano
const INT8U titleBitmap[TITLE_BITMAP_HEIGHT * TITLE_BITMAP_WIDTH];
STRU_BITMAP Stru_Bitmap_title;

// Dibujo robado desvergonzadamente de http://sudoku.taskfour.de/ (Maravillosa página en Aleman? sobre sudokus)
#endif /* TITLE_BITMAP_H_ */
