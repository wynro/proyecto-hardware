/*
 * Modulo        : ARM_candidatos
 * Fichero       : arm_candidatos.h
 * Descripcion   : Modulo que agrupa funciones de bajo nivel relacionadas con
 *                 las excepciones
 * Mantenido por : guillerobles1995@gmail.com
 * Estabilidad   : experimental
 *
 * Modulo que contiene un conjunto de funciones de bajo nivel que ayudan a
 * testear el modulo Exceptions
 */

#ifndef ARM_CANDIDATOS_H_
#define ARM_CANDIDATOS_H_

/**
 * Funcion que recalcula los candidatos de una tabla de sudoku dada
 */
int sudoku_candidatos_arm(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS], int fila,
		int columna);

/**
 * Funcion que recalcula los candidatos de una tabla de sudoku dada
 */
int sudoku_candidatos_thumb(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS], int fila,
		int columna);

/**
 * Funcion que recalcula todos los candidatos de una tabla de sudoku dada,
 * ademas de actualizar los bits de errores.
 */
sudoku_candidatos_arm_2(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS], int fila,
		int columna);

#endif /* ARM_CANDIDATOS_H_ */
