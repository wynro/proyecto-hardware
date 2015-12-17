/*********************************************************************************************
 * Fichero:		timer2.c
 * Autor:
 * Descrip:		funciones de control del timer2 del s3c44b0x
 * Version:
 *********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "44b.h"
#include "44blib.h"
#include <stdint.h>
#include "Timer2.h"
/*--- variables---*/
uint32_t timer2_num_ms = 0;
uint32_t timer2_num_s = 0;

/*--- declaracion de funciones ---*/
void Timer2_ISR(void) __attribute__((interrupt("IRQ")));

/*--- codigo de las funciones ---*/
void Timer2_ISR(void) {
	timer2_num_ms = timer2_num_ms + 1;
	uint32_t aux = timer2_num_ms * (65535 / 32) + ((65535 - rTCNTO2 )/32);
	if (aux >= 1000000) {
		timer2_num_s++;
		timer2_num_ms = 0;
	}
	/* borrar bit en I_ISPC para desactivar la solicitud de interrupción*/
	rI_ISPC |= BIT_TIMER2; // BIT_TIMER2 está definido en 44b.h y pone un uno en el que correponde al Timer2
}

void Timer2_Inicializar(void) {
	/* Configuraion controlador de interrupciones */
	rINTMSK &= ~BIT_TIMER2; // Emascara todas las lineas excepto Timer2)

	/* Establece la rutina de servicio para TIMER2 */pISR_TIMER2 =
			(unsigned) Timer2_ISR;

	/* Configura el Timer2 */rTCFG0 = (rTCFG0 & 0xFFFF00FF); // ajusta el preescalado a 0 para que sea la maxima frecuencia
	rTCFG1 = (rTCFG1 & 0xFFFFF0FF); // selecciona la entrada del mux que proporciona el reloj. La 00 corresponde a un divisor de 1/2.
	rTCNTB2 = 0xFFFF; // valor inicial de cuenta (la cuenta es descendente) se mide lo maximo posible
	rTCMPB2 = 0; // valor de comparación
}

void Timer2_Empezar(void) {
	timer2_num_ms = 0;
	/* establecer update=manual (bit 13) + inverter=off */rTCON = (rTCON
			& 0xFFFF0FFF) | 0x2000;
	/*update manual a 0 de nuevo*/rTCON = (rTCON & 0xFFFF0FFF);
	/* iniciar timer (bit 12) con auto-reload (bit 15)*/
	rTCON = rTCON | 0x9000;
}

uint32_t Timer2_Leer(void) {
	//Devuelve el tiempo en microsegundos
	//TCNTO permite ver el valor de la cuenta sin modificar el registro
	uint32_t aux = (timer2_num_s * 1000000)
			+ (timer2_num_ms * (65535 / 32) + ((65535 - rTCNTO2 )/32));
	return aux;
}

void Timer2_Reiniciar(void) {
	timer2_num_ms = 0;
	timer2_num_s = 0;
}

