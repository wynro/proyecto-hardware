/*
 * Button.h
 *
 *  Created on: 12/11/2015
 *      Author: a604409
 */

#ifndef BUTTON_H_
#define BUTTON_H_
/**
 * Respuesta a la IRQ
 */
void Button_ISR(void) __attribute__((interrupt("IRQ")));

/**
 * Accion asociada al temporizador
 */
void Button_Timer_ISR(void) __attribute__((interrupt("IRQ")));

/**
 * Valor actual de la cuenta interna
 */
int Button_valor_actual(void);

/**
 * Informa de si se ha de avanzar
 */
int next(void);

/**
 * Iniciar el sistema de botones
 */
void Button_init(int min, int max);

/**
 * Accion asociada a la pulsacion del boton de codigo n
 */
void action(int n);

#endif /* BUTTON_H_ */
