/** @file sudoku_collection_san.h
 *  @brief Fichero de recursos que contiene cuadriculas
 * 
 * Modulo que contiene cuadriculas en formato comprimido, ademas de
 * funciones de descompresion
 * 
 * @author Guillermo Robles Gonzalez
 */

#pragma once

#ifndef SUDOKU_COLLECTION_SAN_H_
#define SUDOKU_COLLECTION_SAN_H_
#include "sudoku_2015.h"
#define NUM_CUADRICULAS 10
#define COMPRESSED_SIZE 41

/**
 * @brief Coleccion de cuadriculas comprimidas
 *
 * Coleccion de cuadriculas en formato compreso
 * 
 */
extern char cuadriculas[NUM_CUADRICULAS][COMPRESSED_SIZE];

/**
 * @brief Cuadricula especial casi resuelta
 *
 * Cuadricula especial en la cual solo resta introducir un valor
 */
extern char cuadriculaCasiResuelta[COMPRESSED_SIZE];

/**
 * Funcion que convierte del formato comprimido al formato normal de
 * uso
 *
 * @param[in] compressed Cuadricula compresa
 * @param[out] destiny Zona de memoria en la que se descomprimira la
 *                     cuadricula
 */
void sudoku_collection_descomprime(char *compressed,
		CELDA destiny[NUM_FILAS][NUM_COLUMNAS]);

#endif /* SUDOKU_COLLECTION_SAN_H_ */
