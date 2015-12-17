/** @file Timer2.h
 *  @brief Modulo que gestiona un contador
 * 
 * Modulo que gestiona un timer de precision milisegundos, usa el
 * timer 2
 * 
 * @author Guillermo Robles Gonzalez
 */

#ifndef TIMER2_H_
#define TIMER2_H_
#include "44b.h"

/**
 * Funcion que inicializa el contador
 */
void Timer2_Inicializar(void);

/**
 * Funcion que comienza la cuenta.
 */
void Timer2_Empezar(void);

/**
 * Funcion que resetea el contador interno a 0
 */
void Timer2_Reiniciar(void);

/**
 * Devuelve el contador interno en microsegundos
 *
 * @return Tiempo en microsegundos desde el ultimo reinicio del contador
 */
uint32_t Timer2_Leer(void);
#endif /* TIMER2_H_ */
