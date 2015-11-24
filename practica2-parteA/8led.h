/*
 * 8led.h
 *
 *  Created on: 24/11/2015
 *      Author: a604409
 */

#ifndef D8LED_H_
#define D8LED_H_


/*--- declaración de funciones ---*/
void D8Led_init(void);
void D8Led_symbol(int);
int D8Led_current_symbol(void);
void D8Led_blink_symbol(int,int);
#endif /* 8LED_H_ */
