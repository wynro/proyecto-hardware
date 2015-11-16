/*
 * Button.c
 *
 *  Created on: 12/11/2015
 *      Author: a604409
 */

/*--- ficheros de cabecera ---*/
#include "44blib.h"
#include "44b.h"
#include "def.h"
#include "Button.h"

#define ANCHO_CUENTA (10*1000) // ms
#define TRP 15 // * 10 ms
#define TRD 20 // * 10 ms
#define WAIT_BETWEEN_INCREMENTS 30 // Bloques de 10 ms
#define ESPERA (10*1000) // ms
#define PREESCALADO 32 // Na na na na na na na na na na na na na na na na... BATMAN! <DEV>
unsigned int cuenta = 0;
unsigned int num_espera = 0;
unsigned int pressed;

typedef enum {
	reposo, // Estado en el cual esperamos a que llege una interrupcion de boton
	espera_trp, // Estado en el cual esperamos a que pase el tiempo de seguridad trp
	desactivado, // Estado en el cual ignoramos cualquier interrupcion excepto el reloj
	espera_trd // Estado en el cual esperamos a que pase el tiempo de seguridad trd
} Button_state;

/*--- variables globales ---*/ // Mala idea... <DEV> <--Esto sera un reto para commenTrimmer
/* count la utilizamos para sacar un número por el 8led.
 Cuando se pulsa un botón sumamos y con el otro restamos. ¡A veces (siempre) hay rebotes!  <DEV>*/

/**
 * Almacena el valor actual de la pantalla
 */
unsigned int actual = 1;
unsigned int _next = 0;
unsigned int _min = 0;
unsigned int _max = 16;

Button_state estado = reposo;
/*--- declaracion de funciones ---*/
extern void D8Led_symbol(int value); // declaramos la función que escribe en el 8led

/*--- codigo de funciones ---*/
/**
 * Funciones de control del temporizador
 */
inline void Timer4_init() {
	rINTMOD = 0x0; // Configura las lineas como de tipo IRQ
	rINTCON = 0x1; // Habilita int. vectorizadas y la linea IRQ (FIQ no)
	rINTMSK &= ~BIT_TIMER4;
	// Establece la rutina de servicio para TIMER4
	pISR_TIMER4 = (unsigned) Button_Timer_ISR;
	// Configura el Timer4 a frecuencia de microsegundo
	// TODO: Comprobar que esto no afecta a los otros timers (Lo hace), ya no :)
	rTCFG0 &= 0xFF00FFFF;
	rTCFG0 |= PREESCALADO << 16; // ajusta el preescalado (para el timer4 y timer5)
	rTCFG1 &= 0xFFF0FFFF;
	rTCFG1 |= 0b0000 << 16; // selecciona la entrada del mux que proporciona el reloj. La 00 corresponde a un divisor de 1/2.
}

inline void Timer4_set_time(int t) {
	// Estableceremos a onda cuadrada, es decir mismo tiempo de 0 que de 1
	rTCNTB4 = t; // Registro de cuenta
	rTCMPB4 = t / 2; // Registro de comparacion
}

inline void Timer4_start() {
	// establecer update=manual (bit 1) + inverter=off
	//rTCON = 0b0010 << 20; <DEV>
	rTCON &= 0xFFEFFFFF;
	rTCON |= 0b0010 << 20;
	// iniciar timer (bit 0) sin auto-reload (bit 3)
	//rTCON = 0b0001 << 20; <DEV>
	rTCON &= 0xFF0FFFFF;
	rTCON |= 0b0001 << 20;
}

inline void Timer4_start_reload() {
	// establecer update=manual (bit 1) + inverter=off
	//rTCON = 0b0010 << 20; <DEV>
	rTCON &= 0xFFEFFFFF;
	rTCON |= 0b0010 << 20;
	// iniciar timer (bit 0) con auto-reload (bit 3)
	//rTCON = 0b0001 << 20; <DEV>
	rTCON &= 0xFF0FFFFF;
	rTCON |= 0b1001 << 20;
}

inline void Timer4_set_and_start(int t) {
	Timer4_set_time(t);
	Timer4_start();
}

inline void Timer4_set_and_start_reload(int t) {
	Timer4_set_time(t);
	Timer4_start_reload();
}

void Button_init(int min, int max) {
	/* Configuracion del controlador de interrupciones. Estos registros están definidos en 44b.h */
	rI_ISPC = 0x3ffffff;	// Borra INTPND escribiendo 1s en I_ISPC
	rEXTINTPND = 0xf;    // Borra EXTINTPND escribiendo 1s en el propio registro
	rINTMOD = 0x0;		// Configura las linas como de tipo IRQ
	rINTCON = 0x1;	    // Habilita int. vectorizadas y la linea IRQ (FIQ no)
	//NOTE: 1 enmascara
	rINTMSK &= (~BIT_EINT4567); // Desenmascara nuestro bit de interrupcion

	// Establece la rutina de servicio para el(los) boton(es)

	pISR_EINT4567 = (int) Button_ISR;

	// Configuracion del puerto G
	rPCONG = 0xffff; // Establece la funcion de los pines (EINT0-7)
	rPUPG = 0x0;                  // Habilita el "pull up" del puerto
	rEXTINT = rEXTINT | 0x22222222; // Configura las lineas de int. como de flanco de bajada

	// Por precaucion, se vuelven a borrar los bits de INTPND y EXTINTPND
	rI_ISPC |= (BIT_EINT4567);
	rEXTINTPND = 0xf;

	_min = min;
	_max = max;
	// Iniciar timer
	Timer4_init();
	D8Led_symbol(min);
}

void Button_ISR(void) {
	// Estado reposo
	// Desactivar interrupciones
	rINTMSK |= BIT_EINT4567;	// Desactivar interrupciones de botones
	// Finalizar ISR
	//rEXTINTPND = 0xf;			// borra los bits en EXTINTPND
	rI_ISPC |= BIT_EINT4567;	// borra el bit pendiente en INTPND
	// Reiniciamos la cuenta
	Timer4_set_and_start_reload(ESPERA);

	// Identificar la interrupcion (hay dos pulsadores)
	pressed = rEXTINTPND;
	action(pressed);
	num_espera = 0;
	estado = espera_trp;
}

void action(int n) {
	//D8Led_symbol(16);
	switch (n) {
	case 4:
		actual = (actual + 1) % (_max - _min + 1);
		D8Led_symbol(actual + _min);
		break;
	case 8:
		_next = 1;
		break;
	case 12:
		//Ambos botones pulsados
		break;
	default:
		break;
	}
}

void Button_Timer_ISR() {
	switch (estado) {
	case reposo:
		break;
	case espera_trp:
		if (num_espera == TRP) {
			estado = desactivado;
			cuenta = 0;
		} else {
			num_espera++;
		}
		break;
	case desactivado:
		// Si esta activado algun boton, no pasamos de estado
		if ((rPDATG & 0xC0) != 0xC0) {
			estado = desactivado;
			cuenta++;
			if (cuenta == WAIT_BETWEEN_INCREMENTS) {
				action(pressed);
				cuenta = 0;
			}
		} else {
			// Pasamos a la espera del final
			num_espera = 0;
			estado = espera_trd;
		}
		break;
	case espera_trd:
		if (num_espera == TRD) {
			//rEXTINTPND = 0xf;			// borra los bits en EXTINTPND
			//rI_ISPC |= BIT_EINT4567;	// borra el bit pendiente en INTPND
			rEXTINTPND = 0xf;			// borra los bits en EXTINTPND
			rI_ISPC |= BIT_EINT4567;	// borra el bit pendiente en INTPND
			rINTMSK &= ~BIT_EINT4567;	// Reactivamos interrupciones
			estado = reposo;			// Volvemos a reposo
		} else {
			num_espera++;
		}
		break;
	default:
		break;
	}
	rI_ISPC |= BIT_TIMER4; // Bajamos la interrupcion
}

int Button_valor_actual(void) {
	return actual + _min;
}

int next(void) {
	return _next;
}
