/*
 * Timer2.c
 *
 *  Created on: 29/10/2015
 *      Author: a622612
 */

#include "44b.h"
#include "44blib.h"
#include <inttypes.h>

int timer2_num_int = 0;

void Timer2_Inicializar(void);
void Timer2_Empezar(void);
uint32_t Timer2_Leer(void);
void Timer2_ISR(void __attribute__((interrupt("IRQ"))));

/**
 * Configura el timer 2 para que trabaje a la
 * máxima precisión posible. El reloj de la placa está configurado a
 * 64MHz. Para aumentar el rango del contador, el temporizador generará
 * una interrupción cada vez que haga una cuenta completa (queremos
 * que la cuenta sea lo mayor posible para no sobrecargar en exceso al
 * sistema con interrupciones). La biblioteca dispondrá de la variable
 * interna timer2_num_int, compartida entre la interrupción y el resto de
 * funciones, que llevará la cuenta de los periodos completos del
 * temporizador. Al acabar la cuenta completa el temporizador se reinicia
 * al valor inicial y seguirá contando.
 */
void Timer2_Inicializar(void) {
	/* Configuracion controlador de interrupciones */
	rINTMOD = 0x0; // Configura las linas como de tipo IRQ
	rINTCON = 0x1; // Habilita int. vectorizadas y la linea IRQ (FIQ no)
	rINTMSK = ~(BIT_GLOBAL | BIT_TIMER2);
	// Emascara todas las lineas excepto Timer0 y el bit global (bits 26 y 13,
	// BIT_GLOBAL y BIT_TIMER0 están definidos en 44b.h)

	// Establece la rutina de servicio para TIMER2
	pISR_TIMER2 = (unsigned) Timer2_ISR;

	// Configura el Timer2
	rTCFG0 = 65280; // ajusta el preescalado
	rTCFG1 = 0x700; // selecciona la entrada del mux que proporciona el reloj. La 00 corresponde a un divisor de 1/2.
	rTCNTB2 = 7812; // Registro de cuenta
	rTCMPB2 = 0; // Registro de comparacion
	// establecer update=manual (bit 1) + inverter=on (¿? será inverter off un cero en el bit 2 pone el inverter en off)

}

/**
 * Reinicia la cuenta de tiempo (contador y la variable), y comienza a medir.
 */
void Timer2_Empezar(void) {
	// Reiniciamos la cuenta
	rTCNTB2 = 32000;
	rTCON = 0x2;
	// iniciar timer (bit 0) con auto-reload (bit 3)
	rTCON = 0x09;
	// Reiniciamos contador de ciclos
	timer2_num_int = 0;
}

#define ANCHO_CUENTA 0	// TODO: Poner
#define ANCHO_INTERRUPCION 0 // TODO: Poner
/**
 * lee la cuenta actual del temporizador y el número de interrupciones generadas,
 * y devuelve el tiempo transcurrido en microsegundos.
 */
uint32_t Timer2_Leer(void) {
	//int cuenta_actual = rTCNTO2; // Leemos el valor actual del contador
	//int int_generadas = timer2_num_int;
	return rTCNTO2 * ANCHO_CUENTA + timer2_num_int * ANCHO_INTERRUPCION;
}

/**
 * Funcion que se activa cuando llega una interrupcion del timer,
 */
void Timer2_ISR(void) {
	timer2_num_int++; // Incrementamos contador
	rI_ISPC |= BIT_TIMER2; // BIT_TIMER2 está definido en 44b.h y pone un uno en el bit que correponde al Timer2
}
