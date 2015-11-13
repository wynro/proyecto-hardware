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

#define ANCHO_CUENTA 10 // ms
#define TRP 100 // ms
#define TRD 100 // ms
#define ESPERA 10 // ms
#define PREESCALADO 32

enum {
	reposo, espera_trp, desactivado_1, desactivado_2, espera_trd
};
/*--- variables globales ---*/
/* count la utilizamos para sacar un n�mero por el 8led.
 Cuando se pulsa un bot�n sumamos y con el otro restamos. �A veces (siempre) hay rebotes! */
unsigned int count = 0;
unsigned int estado = reposo;

/*--- declaracion de funciones ---*/
extern void D8Led_symbol(int value); // declaramos la funci�n que escribe en el 8led

/*--- codigo de funciones ---*/
void Button_init(void) {
	/* Configuracion del controlador de interrupciones. Estos registros est�n definidos en 44b.h */
	rI_ISPC = 0x3ffffff;	// Borra INTPND escribiendo 1s en I_ISPC
	rEXTINTPND = 0xf;    // Borra EXTINTPND escribiendo 1s en el propio registro
	rINTMOD = 0x0;		// Configura las linas como de tipo IRQ
	rINTCON = 0x1;	    // Habilita int. vectorizadas y la linea IRQ (FIQ no)
	//NOTE: 1 enmascara
	rINTMSK &= ~BIT_EINT4567; // Enmascara todas las lineas excepto eint4567, el bit global y el timer0

	/* Establece la rutina de servicio para Eint4567 */

	pISR_EINT4567 = (int) Button_ISR;

	// Configuracion del puerto G
	rPCONG = 0xffff; // Establece la funcion de los pines (EINT0-7)
	rPUPG = 0x0;                  // Habilita el "pull up" del puerto
	rEXTINT = rEXTINT | 0x22222222; // Configura las lineas de int. como de flanco de bajada

	// Por precaucion, se vuelven a borrar los bits de INTPND y EXTINTPND
	rI_ISPC |= (BIT_EINT4567);
	rEXTINTPND = 0xf;

	// Iniciar timer
	// Configuracion controlador de interrupciones
	rINTMOD = 0x0; // Configura las lineas como de tipo IRQ
	rINTCON = 0x1; // Habilita int. vectorizadas y la linea IRQ (FIQ no)
	rINTMSK &= ~BIT_TIMER4;
	// Emascara todas las lineas excepto Timer4 y el bit global (bits 26 y 13,
	// BIT_GLOBAL y BIT_TIMER2 est�n definidos en 44b.h)

	// Establece la rutina de servicio para TIMER4
	pISR_TIMER4 = (unsigned) Button_Timer_ISR;

	// Configura el Timer4
	rTCFG0 = PREESCALADO << 16; // ajusta el preescalado (para el timer4 y timer5)
	rTCFG1 = 0x00; // selecciona la entrada del mux que proporciona el reloj. La 00 corresponde a un divisor de 1/2.
	rTCNTB4 = TRP * ANCHO_CUENTA; // Registro de cuenta
	rTCMPB4 = (TRP * ANCHO_CUENTA) / 2; // Registro de comparacion
	D8Led_symbol(count);
}

void Button_ISR(void) {
	//Estado reposo
	// Desactivar interrupciones y bajar flag de interrupcion
	rINTMSK |= BIT_EINT4567; // Desactivar interrupciones
	// Finalizar ISR
	rEXTINTPND = 0xf;		// borra los bits en EXTINTPND
	rI_ISPC |= BIT_EINT4567;		// borra el bit pendiente en INTPND

	//Activamos timer4 para poder esperar el tiempo

	// Reiniciamos la cuenta
	rTCNTB4 = TRP; // Registro de cuenta
	rTCMPB4 = TRP / 2; // Registro de comparacion
	// establecer update=manual (bit 1) + inverter=on (�? ser� inverter off un cero en el bit 2 pone el inverter en off)*/
	rTCON = 0b0010 << 20;
	// iniciar timer (bit 0) sin auto-reload (bit 3)
	rTCON = 0b0001 << 20;

	/* Identificar la interrupcion (hay dos pulsadores)*/
	int which_int = rEXTINTPND;
	action(which_int);
	estado = espera_trp;
}

void action(int n) {
	D8Led_symbol(16);
	switch (n) {
	case 4:
		count = (count + 1) % 16;
		D8Led_symbol(count);
		break;
	case 8:
		count = (count - 1) % 16;
		D8Led_symbol(count);
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
		estado = desactivado_1;
		//TODO: Programar timer para 10 ms
		rTCNTB4 = ESPERA; // Registro de cuenta
		rTCMPB4 = ESPERA / 2; // Registro de comparacion
		rTCON = 0b0010 << 20; // establecer update=manual (bit 1) + inverter=off
		rTCON = 0b0101 << 20; // iniciar timer (bit 0) con auto-reload (bit 3)
		break;
	case desactivado_1:
		// Si esta activado algun boton, no pasamos de estado
		if ((rPDATG & 0x4) || (rPDATG & 0x8)) {
			estado = desactivado_1;
		} else {
			// Pasamos a la espera del final
			rTCNTB4 = TRD; // Registro de cuenta
			rTCMPB4 = TRD / 2; // Registro de comparacion
			rTCON = 0b0010 << 20; // establecer update=manual (bit 1) + inverter=off
			rTCON = 0b0001 << 20; // iniciar timer (bit 0) sin auto-reload (bit 3)
			estado = espera_trd;
		}
		break;
	case espera_trd:
		rINTMSK &= ~BIT_EINT4567; // Reactivamos interrupciones
		estado = reposo; // Volvemos a reposo
		break;
	default:
		break;
	}
}
