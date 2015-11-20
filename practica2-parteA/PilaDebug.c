/*
 * Stack.c
 *
 *  Created on: 05/11/2015
 *      Author: Guillermo Robles
 */

#include "44b.h"
#include "44blib.h"
#include <inttypes.h>
#include "Timer2.h"
// Number of events to record
#define SIZE 20

int *stack;
int pointer;

void debug_init();
void push_debug(int ID_Evento, int auxData);

void debug_init() {
	stack = (int *) 0xc7ff000;
	pointer = SIZE*3;
}

void push_debug(int ID_evento, int auxData) {
	if(pointer == 0) {
		pointer = SIZE*3;
	}
	stack[pointer--] = ID_evento;
	stack[pointer--] = auxData;
	stack[pointer--] = Timer2_Leer(); // Asumamos que el Timer2 esta inizializado...
}
