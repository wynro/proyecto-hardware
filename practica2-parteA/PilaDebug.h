/*
 * PilaDebug.h
 *
 *  Created on: 20/11/2015
 *      Author: wynro
 */

#ifndef PILADEBUG_H_
#define PILADEBUG_H_

#define SIZE 20

/**
 * Inicializa la pila de debug, requiere que se inicialice el Timer2 previamente
 */
void debug_init();

/**
 * Registra el evento pasado en la pila, ademas del tiempo dado por el Timer2
 */
void push_debug(int ID_Evento, int auxData);

#endif /* PILADEBUG_H_ */
