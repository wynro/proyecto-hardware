/**
 * Module       : Timer2
 * Timestamp    : <2015-11-02 - 18:29>
 * Description  : Modulo que gestiona un contador
 * Copyright    : (c) Guillermo Robles, 2015
 * Maintainer   : 604409@unizar.es
 * Stability    : experimental
 * Notes        : Modulo que gestiona un timer de precision
 *                milisegundos, usa el timer 2
 */
#ifndef TIMER2_H_
#define TIMER2_H_
#include "44b.h"

/**
 * Funcion que inicializa el contador
 */
void Timer2_Inicializar(void);
/**
 * Funcion que comienza la cuenta. Resetea el contador interno a 0
 */
void Timer2_Empezar(void);
/**
 * Devuelve el contador interno en milisegundos
 *
 * @return Tiempo en milisegundos desde el ultimo reinicio del contador
 */
int Timer2_Leer(void);
#endif /* TIMER2_H_ */
