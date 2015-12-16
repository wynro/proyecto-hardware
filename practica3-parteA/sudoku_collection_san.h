/*
 * sudoku_collection-san.h
 *
 *  Created on: 16/12/2015
 *      Author: a604409
 */
#pragma once

#ifndef SUDOKU_COLLECTION_SAN_H_
#define SUDOKU_COLLECTION_SAN_H_
#include "sudoku_2015.h"
#define NUM_CUADRICULAS 10
#define COMPRESSED_SIZE 41

/**
 * Coleccion de cuadriculas comprimidas
 */
extern char cuadriculas[NUM_CUADRICULAS][COMPRESSED_SIZE];

/**
 * Cuadricula especial en la cual solo resta introducir un valor
 */
extern char cuadriculaCasiResuelta[COMPRESSED_SIZE];

/**
 * Funcion que convierte del formato comprimido al formato normal de uso
 */
void sudoku_collection_descomprime(char *compressed,
		CELDA destiny[NUM_FILAS][NUM_COLUMNAS]);

#endif /* SUDOKU_COLLECTION_SAN_H_ */
