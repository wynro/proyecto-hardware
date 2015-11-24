/*
 * Button.h
 *
 *  Created on: 12/11/2015
 *      Author: a604409
 */

#ifndef BUTTON_H_
#define BUTTON_H_

/**
 * Valor actual de la cuenta interna
 */
int Button_valor_actual(void);

/**
 * Informa de si se ha de avanzar
 */
int Button_next(void);

/**
 * Iniciar el sistema de botones
 */
void Button_init(int min, int max);

/**
 * Accion asociada a la pulsacion del boton de codigo n
 */
void action(int n);

/**
 * Ajustamos el valor de la cuenta interna del boton, actualizando el display
 */
void Button_set_valor_actual(int);

/**
 * Bajamos el flag interno de next
 */
void Button_low_next(void);

#endif /* BUTTON_H_ */
