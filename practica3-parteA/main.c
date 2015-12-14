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

/*--- declaracion de funciones ---*/
void Main(void);

typedef enum {
	title_screen,
	final_screen,
	aperture,
	esperando_fila,
	esperando_columna,
	esperando_valor
} Game_state;
CELDA cuadriculaSeleccionada[NUM_FILAS][NUM_COLUMNAS];
CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS] = { { 0x9800, 0x6800, 0x0000, 0x0000,
		0x0000, 0x0000, 0x7800, 0x0000, 0x8800, 0, 0, 0, 0, 0, 0, 0 }, { 0x8800,
		0x0000, 0x0000, 0x0000, 0x0000, 0x4800, 0x3800, 0x0000, 0x0000, 0, 0, 0,
		0, 0, 0, 0 }, { 0x1800, 0x0000, 0x0000, 0x5800, 0x0000, 0x0000, 0x0000,
		0x0000, 0x0000, 0, 0, 0, 0, 0, 0, 0 }, { 0x0000, 0x0000, 0x0000, 0x0000,
		0x0000, 0x0000, 0x1800, 0x7800, 0x6800, 0, 0, 0, 0, 0, 0, 0 }, { 0x2800,
		0x0000, 0x0000, 0x0000, 0x9800, 0x3800, 0x0000, 0x0000, 0x5800, 0, 0, 0,
		0, 0, 0, 0 }, { 0x7800, 0x0000, 0x8800, 0x0000, 0x0000, 0x0000, 0x0000,
		0x0000, 0x0000, 0, 0, 0, 0, 0, 0, 0 }, { 0x0030, 0x0000, 0x7800, 0x0000,
		0x3800, 0x2800, 0x0000, 0x4800, 0x0000, 0, 0, 0, 0, 0, 0, 0 }, { 0x3800,
		0x8800, 0x2800, 0x1800, 0x0000, 0x5800, 0x6800, 0x0000, 0x0000, 0, 0, 0,
		0, 0, 0, 0 }, { 0x0000, 0x4800, 0x1800, 0x0000, 0x0000, 0x9800, 0x5800,
		0x2800, 0x0000, 0, 0, 0, 0, 0, 0, 0 } };
CELDA cuadriculaCasiResuelta[NUM_FILAS][NUM_COLUMNAS] = { { 0x0000, 0x6800,
		0x4800, 0x3800, 0x2800, 0x1800, 0x7800, 0x5800, 0x8800, 0, 0, 0, 0, 0,
		0, 0 }, { 0x8800, 0x7800, 0x5800, 0x9800, 0x6800, 0x4800, 0x3800,
		0x1800, 0x2800, 0, 0, 0, 0, 0, 0, 0 }, { 0x1800, 0x2800, 0x3800, 0x5800,
		0x8800, 0x7800, 0x9800, 0x6800, 0x4800, 0, 0, 0, 0, 0, 0, 0 }, { 0x4800,
		0x3800, 0x9800, 0x2800, 0x5800, 0x8800, 0x1800, 0x7800, 0x6800, 0, 0, 0,
		0, 0, 0, 0 }, { 0x2800, 0x1800, 0x6800, 0x7800, 0x9800, 0x3800, 0x4800,
		0x8800, 0x5800, 0, 0, 0, 0, 0, 0, 0 }, { 0x7800, 0x5800, 0x8800, 0x4800,
		0x1800, 0x6800, 0x2800, 0x3800, 0x9800, 0, 0, 0, 0, 0, 0, 0 }, { 0x5800,
		0x9800, 0x7800, 0x6800, 0x3800, 0x2800, 0x8800, 0x4800, 0x1800, 0, 0, 0,
		0, 0, 0, 0 }, { 0x3800, 0x8800, 0x2800, 0x1800, 0x4800, 0x5800, 0x6800,
		0x9800, 0x7800, 0, 0, 0, 0, 0, 0, 0 }, { 0x6800, 0x4800, 0x1800, 0x8800,
		0x7800, 0x9800, 0x5800, 0x2800, 0x3800, 0, 0, 0, 0, 0, 0, 0 } };

/*--- codigo de la funcion ---*/
void Main(void) {
	int tiempo_juego = 0;
	int tiempo_calculos = 0;
	int tiempo_ini = 0;
	int tiempo_fin = 0;
	int tiempo_juego_final = 0;
	int tiempo_final_no_leido = 1;
	int tiempo_base_aperture = 0;
	int tiempo_base_aperture_no_leido = 1;
	int iterador_aperture = -1;
	// Lo siento much pero no voy a poner 119 estados mas
	int last_time_aperture = -1;

	sys_init(); /* inicializacion de la placa, interrupciones, puertos y UART */

	/* initial LCD controller */
	Lcd_Init();
	/* clear screen */
	Lcd_Clr();
	Lcd_Active_Clr();

	/* draw rectangle pattern */
#ifdef Eng_v // english version
	Lcd_DspAscII8x16(10,0,DARKGRAY,"Embest S3CEV40 ");
#else
//		Lcd_DspHz16(10,0,DARKGRAY,"英蓓特三星实验评估板");
#endif

	Timer2_Inicializar();
	Button_init(1, 2);
	Timer2_Empezar();
	tiempo_ini = Timer2_Leer();
	int errores = sudoku_recalcular(cuadricula);
	tiempo_fin = Timer2_Leer();
	tiempo_calculos += (tiempo_fin - tiempo_ini);

	uint8_t fila = 1;
	uint8_t columna = 1;
	int valor = 0;

	Game_state estadoJuego = title_screen; // TODO: cambiar

	while (1) {
		tiempo_juego = Timer2_Leer() / 1000;
		// BLOQUE RENDERIZADO
		Lcd_Clr();
		if (estadoJuego == title_screen) {
			sudoku_graphics_print_title_screen();
		} else if (estadoJuego == final_screen) {
			// Esto es para poder mantener en funcionamiento el reloj, para la tarta
			if (tiempo_final_no_leido) {
				tiempo_final_no_leido = 0;
				tiempo_juego_final = tiempo_juego;
			}
			sudoku_graphics_print_final_screen(tiempo_juego_final,
					tiempo_calculos, errores);
		} else if (estadoJuego == aperture) {
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
		} else {
			// Actualizamos la pantalla de juego
			sudoku_graphics_draw_base();
			sudoku_graphics_fill_from_data(cuadriculaSeleccionada);
			sudoku_graphics_draw_time(tiempo_juego, tiempo_calculos);
			// Por el problemilla A
			if (fila < 10) {
				sudoku_graphics_remark_square(fila - 1, columna - 1);
				if (!es_pista(cuadriculaSeleccionada[columna - 1][fila - 1])) {
					sudoku_graphics_mark_error_in_square(fila - 1, columna - 1,
							valor);
				}
			} else {
				// Tiene sentido que no se dibuje, dado que no seleccionas coordenadas v醠idas
			}
		}
		sudoku_graphics_update_lcd();

		// BLOQUE CAMBIO DE ESTADO
		switch (estadoJuego) {
		case title_screen:
			if (Button_next()) {
				// Seleccionamos cuadricula
				if (Button_valor_actual() == 1) {
					memcpy(cuadriculaSeleccionada, cuadricula,
							sizeof(cuadriculaSeleccionada));
				} else {
					memcpy(cuadriculaSeleccionada, cuadriculaCasiResuelta,
							sizeof(cuadriculaSeleccionada));
				}
				// Reseteamos to-do
				sudoku_vacia_tabla(cuadriculaSeleccionada);
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
				Button_low_next();
				sudoku_graphics_draw_base();
				sudoku_graphics_draw_time(0, 0);
				sudoku_graphics_fill_from_data(cuadriculaSeleccionada);
				sudoku_graphics_update_lcd();

				// Nos vamos
				estadoJuego = esperando_fila;
				Timer2_Empezar();
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
				columna = Button_valor_actual();
				Button_reconfigure_range(0, 9);
				estadoJuego = esperando_valor;
			}
			break;
		case esperando_valor:
			valor = Button_valor_actual();
			if (Button_next()) {
				Button_low_next();
				valor = Button_valor_actual();

				// Update sudoku
				if (!es_pista(cuadriculaSeleccionada[columna - 1][fila - 1])) {
					celda_poner_valor(
							&(cuadriculaSeleccionada[columna - 1][fila - 1]),
							valor);
					tiempo_ini = Timer2_Leer();
					errores = sudoku_recalcular(cuadriculaSeleccionada);
					tiempo_fin = Timer2_Leer();
					tiempo_calculos += (tiempo_fin - tiempo_ini);
				}
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
					Button_init(1, 2);
					estadoJuego = aperture;
				} else {
					Button_init(1, 2);
					estadoJuego = title_screen;
				}
			}
			break;
		case aperture:
			if (Button_next() || iterador_aperture == 113) { //TODO: sincronizar esto con still-alive-lyrics.h
				Button_low_next();
				estadoJuego = title_screen;
			}
		default:
			break;
		}
	}
}

