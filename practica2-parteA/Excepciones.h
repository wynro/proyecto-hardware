/*
 * Exceptions.h
 *
 *  Created on: 12/11/2015
 *      Author: a604409
 */

#ifndef EXCEPTIONS_H_
#define EXCEPTIONS_H_

// Funcion que cambia los punteros a handlers de excepciones a nuestra funcion
void excepcion_init(void);
void trataExcepcion(void);
void push_debug(int ID_evento, int auxData);

#endif /* EXCEPTIONS_H_ */
