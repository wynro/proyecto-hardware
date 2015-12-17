/** @file 8led.h
 *  @brief Funciones de control del display 8-segmentos
 */

#ifndef D8LED_H_
#define D8LED_H_

/**
 * @brief Inicializacion del sistema del 8 segmentos
 * 
 * Inicializa el sistema del 8 segmentos, y pone el simbolo 0 en el
 * mismo
 */
void D8Led_init(void);

/**
 * @brief Cambia el valor en el 8 segmentos
 * 
 * Cambia el simbolo en el 8 segmentos al numero pasado, interpretado
 * en hexadecimal. En caso de que el valor no pueda ser mostrado con
 * un unico digito hexadecimal, no cambia el numero en el 8 segmentos.
 * 
 * @param[in] value Valor a poner en el 8 segmentos
 */
void D8Led_symbol(int value);

/**
 * @brief Devuelve el simbolo actualmente en el 8 segmentos
 *
 * Devuelve el valor en el 8 segmentos, interpretado como un numero
 * decimal en el rango [0,15]
 *
 * @return Simbolo actualmente en el 8 segmentos
 */
int D8Led_current_symbol(void);

/**
 * @brief Pone temporalmente un nuevo simbolo el 8 segmentos
 *
 * Cambia temporalmente el simbolo en el 8 segmentos al valor value
 * (este parametro ha de cumplir las mismas condiciones que para su
 * uso en la funcion D8Led_symbol). El nuevo simbolo permanecera en el
 * 8 segmentos durante los siguientes ms milisegundos. A continuacion
 * se volvera a poner el valor inicial. Es una funcion bloqueante.
 *
 * @param[in] value Valor a poner en el 8 segmentos
 * @param[in] ms Milisegundos que permanecera el simbolo en el 8 
 *               segmentos
 */
void D8Led_blink_symbol(int value, int ms);
#endif /* 8LED_H_ */
