/*
 * Exceptions.c
 *
 *  Created on: 05/11/2015
 *      Author: Guillermo Robles
 */

#include "44b.h"
#include "44blib.h"
#include <inttypes.h>
#include "Excepciones.h"
#include "8led.h"
#include "Modo.h"

/**
 *
 */
void excepcion_init(void) {
	/**
	 * Marcamos trataExcepcion como la que tratará todas las excepciones existentes
	 */
	pISR_UNDEF = (unsigned) trataExcepcion;
	pISR_SWI = (unsigned) trataExcepcion;
	pISR_PABORT = (unsigned) trataExcepcion;
	pISR_DABORT = (unsigned) trataExcepcion;
}

void trataExcepcion(void) {
	int codigo_error;
//Algoritmo de tratamiento:
// Imprimir codigo de error en 8led
	D8Led_init(); // Nos arriesgamos a reinizializar la pantalla...

	codigo_error = getMode(); // Obtenemos el modo, que usaremos para determinar en que excepcion estamos
	int a = 1;
	//push_debug(codigo_error, 0); // Metemos la excepcion en la pila
	// Por ahora, colgamos el sistema con el codigo de error parpadeante
	while (1) {
		D8Led_symbol(codigo_error - 1);
		Delay(1000);
		D8Led_symbol(16);
		Delay(1000);
	}
}
