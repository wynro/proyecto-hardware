/*
 * Modulo        : Modo
 * Fichero       : Modo.h
 * Descripcion   : Modulo que agrupa funciones de bajo nivel relacionadas con
 *                 las excepciones
 * Mantenido por : guillerobles1995@gmail.com
 * Estabilidad   : experimental
 *
 * Modulo que contiene un conjunto de funciones de bajo nivel que ayudan a
 * testear el modulo Exceptions
 */

#ifndef MODE_H_
#define MODE_H_

/**
 * Devuelve el modo de ejecucion actual, eliminando el primer bit
 */
int getMode(void);

/*
 * Lanza la interrupcion por software de codigo 2
 */
void doSWI2(void);

#endif /* MODE_H_ */
