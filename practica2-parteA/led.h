
#ifndef LED_H_
#define LED_H_

/*
 * todos los leds encendidos
 */
void leds_on();
/*
 * todos los leds apagados
 */
void leds_off();
/*
 * led 1 encendido
 */
void led1_on();
/*
 * led 1 apagado
 */
void led1_off();
/*
 * led 2 encendido
 */
void led2_on();
/*
 * led 2 apagado
 */
void led2_off();
/*
 * invierte el valor de los leds
 */
void leds_switch();
/*
 * control directo del LED
 */
void Led_Display(int LedStatus);

#endif LED_H_
