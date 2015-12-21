/*
 * sudoku_2015.c
 *
 *  Created on: 24/11/2015
 *      Author: a604409
 */

#include "8led.h"
#include "Button.h"
#include "sudoku_2015.h"
#include "Timer2.h"
#include "sudoku_graphics.h"
#include "Persistence.h"
#include "sudoku_collection_san.h"
#include "still-alive-lyrics.h"
#include <inttypes.h>

#define BLINK_SYMBOL_TIME 500

typedef enum {
	title_screen,
	final_screen,
	aperture,
	instructions,
	record,
	esperando_fila,
	esperando_columna,
	esperando_valor
} Game_state;
CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS];

inline void celda_poner_valor(CELDA *celdaptr, uint8_t val) {
	*celdaptr = ((*celdaptr) & 0x0FFF) | ((val & 0x000F) << 12);
}

inline int celda_es_error(CELDA celda) {
	return celda & 0b0000001000000000;
}

inline int celda_es_pista(CELDA celda) {
	return celda & 0x0800;
}

inline int celda_es_candidato(CELDA celda, uint8_t valor) {
	return celda & (1 << (valor - 1));
}

inline uint8_t celda_leer_valor(CELDA celda) {
	return celda >> 12;
}

inline void celda_cambiar_candidatos(uint8_t valor,
		CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS], uint8_t fila,
		uint8_t columna) {
	cuadricula[fila][columna] = cuadricula[fila][columna] & ~(1 << (valor - 1));
}

void init_game(void) {
	int tiempo_juego = 0;
	int tiempo_calculos = 0;
	int tiempo_ini_calculo = 0;
	int tiempo_fin_calculo = 0;
	int tiempo_juego_final = 0;
	int tiempo_final_no_leido = 1;
	int tiempo_base_aperture = 0;
	int tiempo_base_aperture_no_leido = 1;
	int iterador_aperture = -1;
	int errores = 1;
	int last_record;
	// Lo siento mucho pero no voy a poner 119 estados mas
	int last_time_aperture = -1;

	Timer2_Inicializar();
	Button_init(1, 4);
	Timer2_Reiniciar();
	Timer2_Empezar();

	uint8_t columna = 1;
	uint8_t fila = 1;
	int valor = 0;

	Game_state estadoJuego = title_screen;

	while (1) {
		tiempo_juego = Timer2_Leer() / 1000000;
		// BLOQUE RENDERIZADO
		sudoku_graphics_clear_screen_buffer();
		if (estadoJuego == title_screen) {
			sudoku_graphics_print_title_screen(Button_valor_actual());
		} else if (estadoJuego == final_screen) {
			// Esto es para poder mantener en funcionamiento el reloj, para la tarta
			if (tiempo_final_no_leido) {
				tiempo_final_no_leido = 0;
				tiempo_juego_final = tiempo_juego;
				last_record = Persistence_read_int();
				if ((tiempo_juego_final < last_record) && (!errores)) {
					Persistence_save_int(tiempo_juego_final);
				}
			}
			sudoku_graphics_print_final_screen(tiempo_juego_final,
					tiempo_calculos, last_record, errores);
		} else if (estadoJuego == aperture) {
			// Cores and turrets of every stage,
			// Wouldn't you like to escape this cage?
			// Come to us and you'll be sure
			// You're not safe in Aperture

			// La gestion de animaciones no es complicada
			if (tiempo_base_aperture_no_leido) {
				tiempo_base_aperture_no_leido = 0;
				tiempo_base_aperture = tiempo_juego;
			}
			if (((tiempo_juego - tiempo_base_aperture)
					% (4 - Button_valor_actual()) == 0)
					&& tiempo_juego != last_time_aperture) {
				iterador_aperture += 1;
				last_time_aperture = tiempo_juego;
			}
			sudoku_graphics_print_still_alive(iterador_aperture);
		} else if (estadoJuego == instructions) {
			sudoku_graphics_print_instructions();
		} else if (estadoJuego == record) {
			sudoku_graphics_print_record(Persistence_read_int());
		} else {
			// Actualizamos la pantalla de juego
			sudoku_graphics_draw_base();
			sudoku_graphics_fill_from_data(cuadricula);
			sudoku_graphics_draw_time(tiempo_juego, tiempo_calculos);
			switch (estadoJuego) {
			case esperando_fila:
				sudoku_graphics_draw_state(0, fila);
				break;
			case esperando_columna:
				sudoku_graphics_draw_state(1, columna);
				break;
			case esperando_valor:
				sudoku_graphics_draw_state(2, valor);
				break;
			default:
				break;
			}
			// Por el problemilla A
			if (fila < 10) {
				sudoku_graphics_remark_square(fila - 1, columna - 1);
				if (!celda_es_pista(cuadricula[fila - 1][columna - 1])) {
					sudoku_graphics_mark_error_in_square(fila - 1, columna - 1,
							valor);
				}
			} else {
				// Tiene sentido que no se dibuje, dado que no seleccionas coordenadas válidas
			}
		}
		sudoku_graphics_update_lcd();

		// BLOQUE CAMBIO DE ESTADO
		switch (estadoJuego) {
		case title_screen:
			if (Button_next()) {
				Button_low_next();
				// Seleccionamos modo de juego (cuadricula/instrucciones)
				int valor_actual = Button_valor_actual();
				// Aun cuando sea menos eficiente,
				// esto permite reutilizar los
				// casilleros
				if (valor_actual == 1) {
					// Una cualquiera
					sudoku_collection_descomprime(
							cuadriculas[Timer2_Leer() % NUM_CUADRICULAS],
							cuadricula);
				} else if (valor_actual == 2) {
					// Cuadricula especial
					sudoku_collection_descomprime(cuadriculaCasiResuelta,
							cuadricula);
				} else if (valor_actual == 3) {
					// Records
					Button_reconfigure_range(1, 1);
					estadoJuego = record;
					break;
				} else if (valor_actual == 4) {
					Button_reconfigure_range(1, 1);
					estadoJuego = instructions;
					break;
				}
				tiempo_calculos = 0;
				tiempo_ini_calculo = Timer2_Leer() / 1000;
				errores = sudoku_recalcular(cuadricula);
				tiempo_fin_calculo = Timer2_Leer() / 1000;
				tiempo_calculos += (tiempo_fin_calculo - tiempo_ini_calculo);
				// Reseteamos to-do

				columna = 1;
				fila = 1;
				valor = 0;
				tiempo_juego = 0;
				tiempo_juego_final = 0;
				tiempo_final_no_leido = 1;
				tiempo_base_aperture = 0;
				tiempo_base_aperture_no_leido = 1;
				iterador_aperture = -1;
				last_time_aperture = -1;
				Button_reconfigure_range(1, 10);

				// Dibujamos la nueva tabla
				sudoku_graphics_clear_screen_buffer();

				sudoku_graphics_draw_base();
				sudoku_graphics_fill_from_data(cuadricula);
				sudoku_graphics_draw_time(0, 0);
				sudoku_graphics_draw_state(0, 0);
				sudoku_graphics_update_lcd();

				// Nos vamos
				estadoJuego = esperando_fila;
				Timer2_Reiniciar();
			}
			break;
		case esperando_fila:
			fila = Button_valor_actual();
			if (Button_next()) {
				Button_low_next();
				if (Button_valor_actual() != 10) {
					fila = Button_valor_actual();
					Button_reconfigure_range(1, 9);
					//Button_set_valor_actual(1);
					estadoJuego = esperando_columna;
				} else {
					// FIN
					fila = 1;
					columna = 1;
					valor = 0;
					Button_reconfigure_range(1, 1);
					estadoJuego = final_screen;
				}
			}
			break;
		case esperando_columna:
			columna = Button_valor_actual();
			if (Button_next()) {
				Button_low_next();
				if (!celda_es_pista(cuadricula[fila - 1][columna - 1])) {
					columna = Button_valor_actual();
					Button_reconfigure_range(0, 9);
					estadoJuego = esperando_valor;
				} else {
					columna = 1;
					fila = 1;
					valor = 0;
					Button_reconfigure_range(1, 10);
					estadoJuego = esperando_fila;
				}
			}
			break;
		case esperando_valor:
			valor = Button_valor_actual();
			if (Button_next()) {
				Button_low_next();
				valor = Button_valor_actual();

				// Update sudoku

				celda_poner_valor(&(cuadricula[fila - 1][columna - 1]), valor);
				tiempo_ini_calculo = Timer2_Leer() / 1000;
				errores = sudoku_recalcular(cuadricula);
				tiempo_fin_calculo = Timer2_Leer() / 1000;
				tiempo_calculos += (tiempo_fin_calculo - tiempo_ini_calculo);

				fila = 1;
				columna = 1;
				valor = 0;
				if (errores) {
					Button_reconfigure_range(1, 10);
					estadoJuego = esperando_fila;
				} else {
					Button_reconfigure_range(1, 1);
					estadoJuego = final_screen;
				}
			}
			break;
		case final_screen:
			if (Button_next()) {
				Button_low_next();
				if (!errores) {
					// Vamonos a aperture
					Button_reconfigure_range(1, 3);
					estadoJuego = aperture;
				} else {
					Button_reconfigure_range(1, 4);
					estadoJuego = title_screen;
				}
			}
			break;
		case aperture:
			if (Button_next() || iterador_aperture == (STILL_ALIVE_SIZE - 6)) {
				Button_low_next();
				Button_reconfigure_range(1, 4);
				estadoJuego = title_screen;
			}
			break;
		case instructions:
			if (Button_next()) {
				Button_low_next();
				Button_reconfigure_range(1, 4);
				estadoJuego = title_screen;
			}
			break;
		case record:
			if (Button_next()) {
				Button_low_next();
				Button_reconfigure_range(1, 4);
				estadoJuego = title_screen;
			}
			break;
		default:
			break;
		}
	}
}

int sudoku_recalcular(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS]) {
	int celdas_vacias = 0;
	int errores = 0;
	int fila = 0;
	while (fila < (NUM_REGION * TAM_REGION)) {
		int columna = 0;
		while (columna < (NUM_REGION * TAM_REGION)) {
			switch (sudoku_candidatos_arm(cuadricula, fila, columna)) {
			case -1:
				errores = 1;
				break;
			case 0:
				celdas_vacias++;
				break;
			case 1:
				break;
			default:
				break;
			}
			columna += 1;
		}
		fila += 1;
	}

//retornar el numero de celdas vacias, siendo negativo si hay errores
	return errores ? -1 : celdas_vacias;
}

void sudoku_empty_grid(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS]) {
	int celdas_vacias = 0;
	int errores = 0;
	int fila, columna;
	for (fila = 0; fila < (NUM_REGION * TAM_REGION); fila++) {
		for (columna = 0; fila < (NUM_REGION * TAM_REGION); columna++) {
			if (!celda_es_pista(cuadricula[fila][columna])) {
				celda_poner_valor(&cuadricula[fila][columna], 0);
			}
		}
	}
}

