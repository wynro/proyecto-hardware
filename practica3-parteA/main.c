/*********************************************************************************************
 * Fichero:	main.c
 * Autor:
 * Descrip:	codigo de entrada de C
 * Version:	<P6-ARM>
 *********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "44blib.h"
#include "44b.h"
#include "lcd.h"
#include "sudoku_graphics.h"
#include "sudoku_2015.h"
#include "Button.h"
#include "Timer2.h"
#include "Bmp.h"
#include "string.h"
#include "still-alive-lyrics.h"
#include "sudoku_collection_san.h"

/*--- declaracion de funciones ---*/
void Main(void);

typedef enum {
	title_screen,
	final_screen,
	aperture,
	instructions,
	esperando_fila,
	esperando_columna,
	esperando_valor
} Game_state;
CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS];

/*--- codigo de la funcion ---*/
void Main(void) {
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
	// Lo siento mucho pero no voy a poner 119 estados mas
	int last_time_aperture = -1;

	sys_init(); /* inicializacion de la placa, interrupciones, puertos y UART */

	/* initial LCD controller */
	Lcd_Init();
	/* clear screen */
	Lcd_Clr();
	Lcd_Active_Clr();

	Timer2_Inicializar();
	Button_init(1, 3);
	Timer2_Reiniciar();
	Timer2_Empezar();

	uint8_t fila = 1;
	uint8_t columna = 1;
	int valor = 0;

	Game_state estadoJuego = title_screen;

	while (1) {
		tiempo_juego = Timer2_Leer() / 1000000;
		// BLOQUE RENDERIZADO
		Lcd_Clr();
		if (estadoJuego == title_screen) {
			sudoku_graphics_print_title_screen(Button_valor_actual());
		} else if (estadoJuego == final_screen) {
			// Esto es para poder mantener en funcionamiento el reloj, para la tarta
			if (tiempo_final_no_leido) {
				tiempo_final_no_leido = 0;
				tiempo_juego_final = tiempo_juego;
			}
			sudoku_graphics_print_final_screen(tiempo_juego_final,
					tiempo_calculos, errores);
		} else if (estadoJuego == aperture) {
			// Cores and turrets of every stage,
			// Wouldn't you like to escape this cage?
			// Come to us and you'll be sure
			// You're not safe in Aperture

			// La gestion de animaciones no es simple
			if (tiempo_base_aperture_no_leido) {
				tiempo_base_aperture_no_leido = 0;
				tiempo_base_aperture = tiempo_juego;
			}
			if (((tiempo_juego - tiempo_base_aperture) % 3 == 0)
					&& tiempo_juego != last_time_aperture) {
				iterador_aperture += 1;
				last_time_aperture = tiempo_juego;
			}
			sudoku_graphics_print_still_alive(iterador_aperture);
		} else if (estadoJuego == instructions) {
			sudoku_graphics_print_instructions();
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
				sudoku_graphics_draw_state(2, Button_valor_actual());
				break;
			default:
				break;
			}
			// Por el problemilla A
			if (fila < 10) {
				sudoku_graphics_remark_square(fila - 1, columna - 1);
				if (!celda_es_pista(cuadricula[columna - 1][fila - 1])) {
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
				} else {
					estadoJuego = instructions;
					break;
				}
				tiempo_calculos = 0;
				tiempo_ini_calculo = Timer2_Leer() / 1000;
				errores = sudoku_recalcular(cuadricula);
				tiempo_fin_calculo = Timer2_Leer() / 1000;
				tiempo_calculos += (tiempo_fin_calculo - tiempo_ini_calculo);
				// Reseteamos to-do

				fila = 1;
				columna = 1;
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
				Lcd_Clr();

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
				if (!celda_es_pista(cuadricula[columna - 1][fila - 1])) {
					columna = Button_valor_actual();
					Button_reconfigure_range(0, 9);
					estadoJuego = esperando_valor;
				} else {
					fila = 1;
					columna = 1;
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

				celda_poner_valor(
						&(cuadricula[columna - 1][fila - 1]),
						valor);
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
					Button_reconfigure_range(1, 3);
					estadoJuego = title_screen;
				}
			}
			break;
		case aperture:
			if (Button_next() || iterador_aperture == (STILL_ALIVE_SIZE - 6)) {
				Button_low_next();
				Button_reconfigure_range(1, 3);
				estadoJuego = title_screen;
			}
			break;
		case instructions:
			if (Button_next()) {
				Button_low_next();
				Button_reconfigure_range(1, 3);
				estadoJuego = title_screen;
			}
			break;
		default:
			break;
		}
	}
}

