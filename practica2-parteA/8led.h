/*
 * Modulo        : 8led
 * Fichero       : 8led.h
 * Descripcion   : Modulo que gestiona el 7 segmentos
 * Mantenido por :
 * Estabilidad   : experimental
 *
 * Modulo que contiene un conjunto de funciones que permiten el gestionado del
 * 8 led en la placa s3c44b0x dada.
 */

#ifndef _8LED_H_
#define _8LED_H_

/**
 * Enumeracion de los valores a poner en la pantalla y su codigo
 */
#define size_8led 17
typedef enum {
	// Se podria tipar con un typedef, pero estaria el problema de extraer el tamano...
	D8Led_cero = ~0xED,
	D8Led_uno = ~0x60,
	D8Led_dos = ~0xCE,
	D8Led_tres = ~0xEA,
	D8Led_cuatro = ~0x63,
	D8Led_cinco = ~0xAB,
	D8Led_seis = ~0x2F,
	D8Led_siete = ~0xE0,
	D8Led_ocho = ~0xEF,
	D8Led_nueve = ~0xE3,
	D8Led_A = ~0xE7,
	D8Led_B = ~0x2F,
	D8Led_C = ~0x8D,
	D8Led_D = ~0x6E,
	D8Led_E = ~0x8F,
	D8Led_F = ~0x87,
	D8Led_blank = ~0x00
} D8Led_state;

/**
 * Inicializa todo el sistema del 8Led
 */
void D8Led_init(void);

/**
 * Cambia el valor de la pantalla al dado
 */
void D8Led_symbol(int value);

/**
 * Cambia el valor de la pantalla al dado, no actualiza el valor interno
 */
void D8Led_set(D8Led_state state);

/**
 * Devuelve el valor actual que aparece en la pantalla
 */
int D8Led_current_symbol(void);

/**
 * Pone el valor dado en la pantalla durante aproximadamente
 * ms milisegundos, a continuacion vuelve al valor anterior
 */
void D8Led_blink_symbol(int value, int ms);

#endif /* _8LED_H_ */
