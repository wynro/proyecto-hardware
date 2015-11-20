/*
 * Modulo        : 8led
 * Fichero       : 8led.c
 * Referirse a 8led.h para mas informacion
 */

#include "44b.h"
#include "44blib.h"
#include "8led.h"

/* tabla de segmentos */
int Symbol[size_8led] = {
		D8Led_cero,
		D8Led_uno,
		D8Led_dos,
		D8Led_tres,
		D8Led_cuatro,
		D8Led_cinco,
		D8Led_seis,
		D8Led_siete,
		D8Led_ocho,
		D8Led_nueve,
		D8Led_A,
		D8Led_B,
		D8Led_C,
		D8Led_D,
		D8Led_E,
		D8Led_F,
		D8Led_blank };
int valor_actual;


/*--- código de las funciones ---*/
void D8Led_init(void) {
	/* Estado inicial del display con todos los segmentos iluminados
	 (buscar en los ficheros de cabera la direccion implicada) */
	LED8ADDR = D8Led_cero;
	valor_actual = 0;
}

void D8Led_symbol(int value) {
	/* muestra el Symbol[value] en el display (analogo al caso anterior) */
	if ((value >= 0) && (value < size_8led)) {
		LED8ADDR = Symbol[value];
		valor_actual = value;
	}
}

void D8Led_set(D8Led_state state) {
	LED8ADDR = state;
}

int D8Led_current_symbol(void) {
	return valor_actual;
}

void D8Led_blink_symbol(int value, int ms) {
	// Vale, esto no es exacto, pero es suficiente para este caso
	int temp_value = valor_actual;
	D8Led_symbol(value);
	DelayMs(ms);
	D8Led_symbol(temp_value);
}
