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

/*--- declaracion de funciones ---*/
void Main(void);

typedef enum {
	title_screen, fin, esperando_fila, esperando_columna, esperando_valor
} Game_state;

/*--- codigo de la funcion ---*/
void Main(void) {
	int tiempo_juego = 0;
	int tiempo_calculos = 0;
	int tiempo_ini = 0;
	int tiempo_fin = 0;

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

	CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS] __attribute__((align(4))) = { {
			0x9800, 0x6800, 0x0000, 0x0000, 0x0000, 0x0000, 0x7800, 0x0000,
			0x8800, 0, 0, 0, 0, 0, 0, 0 }, { 0x8800, 0x0000, 0x0000, 0x0000,
			0x0000, 0x4800, 0x3800, 0x0000, 0x0000, 0, 0, 0, 0, 0, 0, 0 }, {
			0x1800, 0x0000, 0x0000, 0x5800, 0x0000, 0x0000, 0x0000, 0x0000,
			0x0000, 0, 0, 0, 0, 0, 0, 0 }, { 0x0000, 0x0000, 0x0000, 0x0000,
			0x0000, 0x0000, 0x1800, 0x7800, 0x6800, 0, 0, 0, 0, 0, 0, 0 }, {
			0x2800, 0x0000, 0x0000, 0x0000, 0x9800, 0x3800, 0x0000, 0x0000,
			0x5800, 0, 0, 0, 0, 0, 0, 0 }, { 0x7800, 0x0000, 0x8800, 0x0000,
			0x003B, 0x0000, 0x0000, 0x0000, 0x0000, 0, 0, 0, 0, 0, 0, 0 }, {
			0x0030, 0x0000, 0x7800, 0x0000, 0x3800, 0x2800, 0x0000, 0x4800,
			0x0000, 0, 0, 0, 0, 0, 0, 0 }, { 0x3800, 0x8800, 0x2800, 0x1800,
			0x0000, 0x5800, 0x6800, 0x0000, 0x0000, 0, 0, 0, 0, 0, 0, 0 }, {
			0x0000, 0x4800, 0x1800, 0x0000, 0x0000, 0x9800, 0x5800, 0x2800,
			0x0000, 0, 0, 0, 0, 0, 0, 0 } };

	Timer2_Inicializar();
	Button_init(1, 10);
	Timer2_Empezar();
	tiempo_ini = Timer2_Leer();
	sudoku_recalcular(cuadricula);
	tiempo_fin = Timer2_Leer();
	tiempo_calculos += (tiempo_fin - tiempo_ini);

	uint8_t fila = 1;
	uint8_t columna = 1;
	int valor = 0;
	Game_state estadoJuego = title_screen;

	while (1) {
		tiempo_juego = Timer2_Leer() / 1000000;
		Lcd_Clr();
		if (estadoJuego == title_screen) {
			sudoku_graphics_print_title_screen();
		} else {
			// Actualizamos la pantalla
			sudoku_graphics_draw_base();
			sudoku_graphics_fill_from_data(cuadricula);
			sudoku_graphics_draw_time(tiempo_juego, tiempo_calculos);
			if (fila < 10) {
				sudoku_graphics_remark_square(fila - 1, columna - 1);
				if (!es_pista(cuadricula[columna - 1][fila - 1])) {
					sudoku_graphics_mark_error_in_square(fila - 1, columna - 1,
							valor);
				}
			} else {
				// Tiene sentido que no se dibuje, dado que no seleccionas coordenadas v醠idas
			}
		}
		sudoku_graphics_update_lcd();

//		sudoku_graphics_update_lcd();
		switch (estadoJuego) {
		case title_screen:
			if (Button_next()) {
				Lcd_Clr();
				Button_low_next();
				sudoku_graphics_draw_base();
				sudoku_graphics_draw_time(0, 0);
				sudoku_graphics_fill_from_data(cuadricula);
				sudoku_graphics_update_lcd();
				fila = 1;
				columna = 1;
				valor = 0;
				Button_reconfigure_range(1, 10);
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
					estadoJuego = title_screen;
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
				if (!es_pista(cuadricula[columna - 1][fila - 1])) {
					celda_poner_valor(&(cuadricula[columna - 1][fila - 1]),
							valor);
					tiempo_ini = Timer2_Leer();
					sudoku_recalcular(cuadricula);
					tiempo_fin = Timer2_Leer();
					tiempo_calculos += (tiempo_fin - tiempo_ini);
				}
				fila = 1;
				columna = 1;
				valor = 0;
				Button_reconfigure_range(1, 10);
				estadoJuego = esperando_fila;
			}
			break;
		default:
			break;
		}
	}
}

