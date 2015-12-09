/*
 * Timer2.h
 *
 *  Created on: 12/11/2015
 *      Author: a604409
 */

#ifndef TIMER2_H_
#define TIMER2_H_
#include "44b.h"

// Declaracion de funciones
void Timer2_Inicializar(void);
void Timer2_Empezar(void);
int Timer2_Leer();
void Timer2_ISR(void) __attribute__((interrupt("IRQ")));

#endif /* TIMER2_H_ */
