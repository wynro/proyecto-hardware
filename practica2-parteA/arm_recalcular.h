/*
 * Modulo        : ARM_recalcular
 * Fichero       : arm_recalcular.h
 * Descripcion   : Modulo que contiene las funciones de recalculado de una
 * 				   casilla del sudoku
 * Mantenido por : guillerobles1995@gmail.com
 * Estabilidad   : experimental
 * Portabilidad  : ARMv7, Thumb
 *
 * Modulo que contiene un conjunto de funciones de bajo nivel que ayudan a
 * testear el modulo Exceptions
 */
#ifndef ARM_RECALCULAR_H_
#define ARM_RECALCULAR_H_

/**
 * Funcion que recalcula los posibles candidatos de una cuadricula,
 * utilizando una funcion ASM como hoja
 */
int sudoku_recalcular_a_a(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS]);

/**
 * Funcion que recalcula los posibles candidatos de una cuadricula,
 * utilizando una funcion C como hoja
 */
int sudoku_recalcular_a_c(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS]);

/**
 * Funcion que recalcula los posibles candidatos de una cuadricula,
 * utilizando una funcion Thumb como hoja
 */
int sudoku_recalcular_a_t(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS]);
#endif /* ARM_RECALCULAR_H_ */
