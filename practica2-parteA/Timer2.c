/*
 * Timer2.c
 *
 *  Created on: 29/10/2015
 *      Author: Guillermo Robles
 */

#include "44b.h"
#include "44blib.h"
#include <inttypes.h>
#include "Timer2.h"

// Contador de ciclos completados
int timer2_num_int = 0;


/**
 * Configura el timer 2 para que trabaje a la
 * m�xima precisi�n posible. El reloj de la placa est� configurado a
 * 64MHz. Para aumentar el rango del contador, el temporizador generar�
 * una interrupci�n cada vez que haga una cuenta completa (queremos
 * que la cuenta sea lo mayor posible para no sobrecargar en exceso al
 * sistema con interrupciones). La biblioteca dispondr� de la variable
 * interna timer2_num_int, compartida entre la interrupci�n y el resto de
 * funciones, que llevar� la cuenta de los periodos completos del
 * temporizador. Al acabar la cuenta completa el temporizador se reinicia
 * al valor inicial y seguir� contando.
 */
void Timer2_Inicializar(void) {
	/* Configuracion controlador de interrupciones */
	rINTMOD = 0x0; // Configura las lineas como de tipo IRQ
	rINTCON = 0x1; // Habilita int. vectorizadas y la linea IRQ (FIQ no)
	rINTMSK = ~(BIT_GLOBAL | BIT_TIMER2);
	// Emascara todas las lineas excepto Timer2 y el bit global (bits 26 y 13,
	// BIT_GLOBAL y BIT_TIMER2 est�n definidos en 44b.h)

	// Establece la rutina de servicio para TIMER2
	pISR_TIMER2 = (unsigned) Timer2_ISR;

	// Configura el Timer2
	rTCFG0 = PREESCALADO << 8; // ajusta el preescalado (para el timer2)
	rTCFG1 = 0x00; // selecciona la entrada del mux que proporciona el reloj. La 00 corresponde a un divisor de 1/2.
	rTCNTB2 = ANCHO_CUENTA; // Registro de cuenta
	rTCMPB2 = ANCHO_CUENTA / 2; // Registro de comparacion
}

/**
 * Reinicia la cuenta de tiempo (contador y la variable), y comienza a medir.
 */
void Timer2_Empezar(void) {
	// Reiniciamos la cuenta
	rTCNTB2 = ANCHO_CUENTA;
	// Reiniciamos contador de ciclos
	timer2_num_int = 0;
	// establecer update=manual (bit 1) + inverter=on (�? ser� inverter off un cero en el bit 2 pone el inverter en off)*/
	rTCON = 0x2000;
	// iniciar timer (bit 0) con auto-reload (bit 3)
	rTCON = 0x9000;
}

/**
 * lee la cuenta actual del temporizador y el n�mero de interrupciones generadas,
 * y devuelve el tiempo transcurrido en microsegundos.
 */
uint32_t Timer2_Leer(void) {
	//int cuenta_actual = rTCNTO2; // Leemos el valor actual del contador
	//int int_generadas = timer2_num_int;
	return timer2_num_int * ANCHO_CUENTA + rTCNTO2 ;
}

/**
 * Funcion que se activa cuando llega una interrupcion del timer,
 */
void Timer2_ISR(void) {
	timer2_num_int++; // Incrementamos contador
	rI_ISPC |= BIT_TIMER2; // BIT_TIMER2 est� definido en 44b.h y pone un uno en el bit que correponde al Timer2
}
