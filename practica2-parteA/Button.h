/*
 * Button.h
 *
 *  Created on: 12/11/2015
 *      Author: a604409
 */

#ifndef BUTTON_ALT_H_
#define BUTTON_ALT_H_
/**
 * Respuesta a la IRQ
 */
void Button_ISR(void) __attribute__((interrupt("IRQ")));

/**
 * Iniciar el sistema de botones
 */
void Button_init(void);

/**
 * Accion asociada a la pulsacion del boton de codigo n
 */
void action(int n);


/**
 * Accion asociada al temporizador
 */
void Button_Timer_ISR();

#endif /* BUTTON_H_ */
