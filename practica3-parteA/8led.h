/*
 * 8led.h
 *
 *  Created on: 24/11/2015
 *      Author: a604409
 */

#ifndef D8LED_H_
#define D8LED_H_

/**
 * Inicializa el sistema del 7 segmentos, poniendo la pantalla a 0.
 */
void D8Led_init(void);

/**
 * Cambia el valor en la pantalla al indicado.
 *
 * @param[in] value Nuevo valor a introducir en la pantalla.
 */
void D8Led_symbol(int value);

/**
 * Devuelve el simbolo actual.
 *
 * @return Valor actual en la pantalla.
 */
int D8Led_current_symbol(void);

/**
 * Cambia temporalmente el simbolo en la pantalla.
 *
 * @param[in] value Valor que se pondra temporalmente en la pantalla.
 * @param[in} ms 	Milisegundos que permanecerá en la pantalla.
 */
void D8Led_blink_symbol(int value, int ms);
#endif /* 8LED_H_ */
