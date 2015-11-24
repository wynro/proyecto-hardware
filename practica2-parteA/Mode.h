/*
 * Mode.h
 *
 *  Created on: 24/11/2015
 *      Author: a604409
 */

#ifndef MODE_H_
#define MODE_H_

/**
 * Devuelve los ultimos 4 bits del modo actual como entero
 */
int getMode(void);

/**
 * Lanza la excepcion de software de id 2
 */
void doSWI2(void);
#endif /* MODE_H_ */
