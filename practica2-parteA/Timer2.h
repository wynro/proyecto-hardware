/*
 * Timer2.h
 *
 *  Created on: 12/11/2015
 *      Author: a604409
 */

#ifndef TIMER2_H_
#define TIMER2_H_

#define ANCHO_CUENTA 1000
#define PREESCALADO 32

// Declaracion de funciones
void Timer2_Inicializar(void);
void Timer2_Empezar(void);
uint32_t Timer2_Leer(void);
void Timer2_ISR(void) __attribute__((interrupt("IRQ")));


#endif /* TIMER2_H_ */
