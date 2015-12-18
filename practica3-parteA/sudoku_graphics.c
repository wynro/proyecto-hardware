/*
 * sudoku_graphics.c
 *
 *  Created on: 26/11/2015
 *      Author: a604409
 */

#include "sudoku_2015.h"
#include "sudoku_graphics.h"
#include "44blib.h"
#include "44b.h"
#include "lcd.h"
#include "title-bitmap.h"
#include "aperture-logo-bitmap.h"
#include "still-alive-lyrics.h"
#include "Bmp.h"

#define celdaPos0x(x) (SUDOKU_X0 + 1 + ((SUDOKU_SQUARE_SIZE + 1) * (x)))
#define celdaPos0y(y) (SUDOKU_Y0 + 1 + ((SUDOKU_SQUARE_SIZE + 1) * (y)))

#define DRAW_MESSAGE

char *time_message = "Tiempos:";
char *time_playing_short = "Jugando:";
char *time_calculating_short = "Calc (ms):";
char *time_playing_message = "Tiempo total de juego: ";
char *time_calculating_message = "Tiempo total de calculo(ms): ";
char *mensaje_aperture_1 = "Resolviste el sudoku con exito!";
char *mensaje_aperture_2 = "Boton derecho para recibir tu premio";
char *mensaje_fracaso_1 = "No resolviste el sudoku :(";
char *mensaje_fracaso_2 = "Pulsa boton derecho para reiniciar.";
char *mensaje_seleccionando = "Seleccione:";
char *mensaje_fila = "Fila";
char *mensaje_columna = "Columna";
char *mensaje_valor = "Valor";
char *title_message = "Boton derecho para seleccionar!";
char *title_message_selection = "Boton izquierdo para cambiar de opcion!";
char *message_A = "Introduzca fila A para finalizar";
#define INSTRUCTION_LINES 11

// Estas instrucciones han sido cedidas por Miguel Jorge Galindo e Iñigo Gascon
char *instructions[INSTRUCTION_LINES] = { "INSTRUCCIONES DE JUEGO: ",
		"Cuando comience el juego podra elegir",
		"la fila que quiere modificar con el", "boton izquierdo y confirmar la",
		"eleccion con el boton derecho.", "A continuacion podra elegir la",
		"columna de la misma manera.", "Por ultimo debera seleccionar",
		"el valor deseado.", "", "Pulse boton derecho para volver" };
//
//char *instructions[INSTRUCTION_LINES] = { "INSTRUCCIONES",
//		"El boton derecho actua como start y", "enter.",
//		"El boton izquierdo permite cambiar la", "seleccion actual.", "",
//		"Introduzca fila A para salir" };
#define GAME_TYPES_SIZE 3
char *game_types_message[GAME_TYPES_SIZE] = { "1: Cuadricula normal",
		"2: Cuadricula terminada", "3: Instrucciones" };

void reverse(char str[], int length) {
	int start = 0;
	int end = length - 1;
	while (start < end) {
		GUISWAP(*(str + start), *(str + end));
		start++;
		end--;
	}
}

char* itoa(int num, char* str, int base) {
	int i = 0;
	int isNegative = 0;
	if (num == 0) {
		str[i++] = '0';
		str[i] = '\0';
		return str;
	}
	if (num < 0 && base == 10) {
		isNegative = 1;
		num = -num;
	}
	while (num != 0) {
		int rem = num % base;
		str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
		num = num / base;
	}
	if (isNegative) {
		str[i++] = '-';
	}
	str[i] = '\0';
	reverse(str, i);

	return str;
}

inline void sudoku_graphics_put_number_in_square(INT8 x, INT8 y, INT8 number,
		INT8U ucColor) {
	Lcd_DisplayChar(
			SUDOKU_X0 + ((SUDOKU_SQUARE_SIZE - SUDOKU_FONT_LENGTH) / 2 + 2)
					+ ((SUDOKU_SQUARE_SIZE + 1) * x),
			SUDOKU_Y0 + ((SUDOKU_SQUARE_SIZE - SUDOKU_FONT_HEIGHT) / 2 + 1)
					+ ((SUDOKU_SQUARE_SIZE + 1) * y), ucColor,
			ASCII_NUMBER_BASE + number);
}

inline void sudoku_graphics_remark_square(INT8 x, INT8 y) {
	Lcd_Draw_Box_inverted(celdaPos0y(y), celdaPos0x(x),
			celdaPos0y(y) + SUDOKU_SQUARE_SIZE - 1,
			celdaPos0x(x) + SUDOKU_SQUARE_SIZE - 1, BLACK);
}

inline void sudoku_graphics_invert_square(INT8 x, INT8 y) {
	ReverseSquare(celdaPos0x(x), celdaPos0y(y),
			celdaPos0x(x) + SUDOKU_SQUARE_SIZE - 1,
			celdaPos0y(y) + SUDOKU_SQUARE_SIZE - 1);
}

inline void sudoku_graphics_mark_error(INT8 x, INT8 y, INT8 error) {
	switch (error) {
	case 1:
		Lcd_Draw_Filled_Box(celdaPos0x(x) + 1, celdaPos0y(y) + 1,
				celdaPos0x(x) + SUDOKU_SQUARE_SIZE / 3 - 1,
				celdaPos0y(y) + SUDOKU_SQUARE_SIZE / 3 - 1, BLACK);
		break;
	case 2:
		Lcd_Draw_Filled_Box(celdaPos0x(x) + (SUDOKU_SQUARE_SIZE / 3) + 1,
				celdaPos0y(y) + 1,
				celdaPos0x(x) + 2 * (SUDOKU_SQUARE_SIZE / 3) - 1,
				celdaPos0y(y) + SUDOKU_SQUARE_SIZE / 3 - 1, BLACK);
		break;
	case 3:
		Lcd_Draw_Filled_Box(celdaPos0x(x) + 2 * (SUDOKU_SQUARE_SIZE / 3) + 1,
				celdaPos0y(y) + 1, celdaPos0x(x) + SUDOKU_SQUARE_SIZE - 1,
				celdaPos0y(y) + SUDOKU_SQUARE_SIZE / 3 - 1, BLACK);
		break;
	case 4:
		Lcd_Draw_Filled_Box(celdaPos0x(x) + 1,
				celdaPos0y(y) + SUDOKU_SQUARE_SIZE / 3 + 1,
				celdaPos0x(x) + SUDOKU_SQUARE_SIZE / 3 - 1,
				celdaPos0y(y) + 2 * (SUDOKU_SQUARE_SIZE / 3) - 1, BLACK);
		break;
	case 5:
		Lcd_Draw_Filled_Box(celdaPos0x(x) + (SUDOKU_SQUARE_SIZE / 3) + 1,
				celdaPos0y(y) + SUDOKU_SQUARE_SIZE / 3 + 1,
				celdaPos0x(x) + 2 * (SUDOKU_SQUARE_SIZE / 3) - 1,
				celdaPos0y(y) + 2 * (SUDOKU_SQUARE_SIZE / 3) - 1, BLACK);
		break;
	case 6:
		Lcd_Draw_Filled_Box(celdaPos0x(x) + 2 * (SUDOKU_SQUARE_SIZE / 3) + 1,
				celdaPos0y(y) + SUDOKU_SQUARE_SIZE / 3 + 1,
				celdaPos0x(x) + SUDOKU_SQUARE_SIZE - 1,
				celdaPos0y(y) + 2 * (SUDOKU_SQUARE_SIZE / 3) - 1, BLACK);
		break;
	case 7:
		Lcd_Draw_Filled_Box(celdaPos0x(x) + 1,
				celdaPos0y(y) + 2 * (SUDOKU_SQUARE_SIZE / 3) + 1,
				celdaPos0x(x) + SUDOKU_SQUARE_SIZE / 3 - 1,
				celdaPos0y(y) + SUDOKU_SQUARE_SIZE - 1, BLACK);
		break;
	case 8:
		Lcd_Draw_Filled_Box(celdaPos0x(x) + (SUDOKU_SQUARE_SIZE / 3) + 1,
				celdaPos0y(y) + 2 * (SUDOKU_SQUARE_SIZE / 3) + 1,
				celdaPos0x(x) + 2 * (SUDOKU_SQUARE_SIZE / 3) - 1,
				celdaPos0y(y) + SUDOKU_SQUARE_SIZE - 1, BLACK);
		break;
	case 9:
		Lcd_Draw_Filled_Box(celdaPos0x(x) + 2 * (SUDOKU_SQUARE_SIZE / 3) + 1,
				celdaPos0y(y) + 2 * (SUDOKU_SQUARE_SIZE / 3) + 1,
				celdaPos0x(x) + SUDOKU_SQUARE_SIZE - 1,
				celdaPos0y(y) + SUDOKU_SQUARE_SIZE - 1, BLACK);
		break;
	default:
		break;
	}
}

inline void sudoku_graphics_mark_error_in_square(INT8 x, INT8 y, INT8 error) {
	switch (error) {
	case 1:
		ReverseSquare(celdaPos0y(y) + 2, celdaPos0x(x) + 2,
				celdaPos0y(y) + SUDOKU_SQUARE_SIZE / 3 - 3,
				celdaPos0x(x) + SUDOKU_SQUARE_SIZE / 3 - 3);
		break;
	case 2:
		ReverseSquare(celdaPos0y(y) + SUDOKU_SQUARE_SIZE / 3 + 2,
				celdaPos0x(x) + 2,
				celdaPos0y(y) + 2 * (SUDOKU_SQUARE_SIZE / 3) - 3,
				celdaPos0x(x) + SUDOKU_SQUARE_SIZE / 3 - 3);
		break;
	case 3:
		ReverseSquare(celdaPos0y(y) + 2 * (SUDOKU_SQUARE_SIZE / 3) + 2,
				celdaPos0x(x) + 2, celdaPos0y(y) + SUDOKU_SQUARE_SIZE - 3,
				celdaPos0x(x) + SUDOKU_SQUARE_SIZE / 3 - 3);
		break;
	case 4:

		ReverseSquare(celdaPos0y(y) + 2,
				celdaPos0x(x) + (SUDOKU_SQUARE_SIZE / 3) + 2,
				celdaPos0y(y) + SUDOKU_SQUARE_SIZE / 3 - 3,
				celdaPos0x(x) + 2 * (SUDOKU_SQUARE_SIZE / 3) - 3);
		break;
	case 5:
		ReverseSquare(celdaPos0y(y) + SUDOKU_SQUARE_SIZE / 3 + 2,
				celdaPos0x(x) + (SUDOKU_SQUARE_SIZE / 3) + 2,
				celdaPos0y(y) + 2 * (SUDOKU_SQUARE_SIZE / 3) - 3,
				celdaPos0x(x) + 2 * (SUDOKU_SQUARE_SIZE / 3) - 3);
		break;
	case 6:
		ReverseSquare(celdaPos0y(y) + 2 * (SUDOKU_SQUARE_SIZE / 3) + 2,
				celdaPos0x(x) + (SUDOKU_SQUARE_SIZE / 3) + 2,
				celdaPos0y(y) + SUDOKU_SQUARE_SIZE - 3,
				celdaPos0x(x) + 2 * (SUDOKU_SQUARE_SIZE / 3) - 3);
		break;
	case 7:
		ReverseSquare(celdaPos0y(y) + 2,
				celdaPos0x(x) + 2 * (SUDOKU_SQUARE_SIZE / 3) + 2,
				celdaPos0y(y) + SUDOKU_SQUARE_SIZE / 3 - 3,
				celdaPos0x(x) + SUDOKU_SQUARE_SIZE - 3);
		break;
	case 8:

		ReverseSquare(celdaPos0y(y) + SUDOKU_SQUARE_SIZE / 3 + 2,
				celdaPos0x(x) + 2 * (SUDOKU_SQUARE_SIZE / 3) + 2,
				celdaPos0y(y) + 2 * (SUDOKU_SQUARE_SIZE / 3) - 3,
				celdaPos0x(x) + SUDOKU_SQUARE_SIZE - 3);
		break;
	case 9:
		ReverseSquare(celdaPos0y(y) + 2 * (SUDOKU_SQUARE_SIZE / 3) + 2,
				celdaPos0x(x) + 2 * (SUDOKU_SQUARE_SIZE / 3) + 2,
				celdaPos0y(y) + SUDOKU_SQUARE_SIZE - 3,
				celdaPos0x(x) + SUDOKU_SQUARE_SIZE - 3);
		break;
	default:
		break;
	}
}

void sudoku_graphics_fill_from_data(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS]) {
	int i, j, k;
	for (i = 0; i < NUM_FILAS; i++) {
		for (j = 0; j < NUM_FILAS; ++j) {
			uint8_t valor = celda_leer_valor(cuadricula[j][i]);
			if (valor == 0) {
				// Hemos de rellenar todos los candidatos
				for (k = 1; k <= 9; k++) {
					if (celda_es_candidato(cuadricula[j][i], k)) {
						sudoku_graphics_mark_error(i, j, k);
					}
				}
			} else {
				// Simplemente ponemos el numero en el cuadrado, y si es error lo marcamos
				sudoku_graphics_put_number_in_square(i, j, valor,
						celda_es_pista(cuadricula[j][i]) ? BLACK : 0xd);
				if (celda_es_error(cuadricula[j][i])) {
					sudoku_graphics_invert_square(i, j);
				}
			}

		}
	}
}

void sudoku_graphics_update_lcd() {
	Lcd_Dma_Trans();
}

void sudoku_graphics_draw_base() {
	int i;
	for (i = 0; i < SUDOKU_NUM_CUADS + 1; ++i) {
		Lcd_Draw_VLine(SUDOKU_Y0 + 1,
				SUDOKU_Y0 + (SUDOKU_NUM_CUADS * (SUDOKU_SQUARE_SIZE + 1)) - 1,
				SUDOKU_X0 + (SUDOKU_SQUARE_SIZE + 1) * i, DARKGRAY, 1);
		Lcd_Draw_HLine(SUDOKU_X0 + 1,
				SUDOKU_X0 + (SUDOKU_NUM_CUADS * (SUDOKU_SQUARE_SIZE + 1)) - 1,
				SUDOKU_Y0 + (SUDOKU_SQUARE_SIZE + 1) * i, DARKGRAY, 1);
		if (i < SUDOKU_NUM_CUADS) {
			Lcd_DisplayChar(
					SUDOKU_X0
							+ ((SUDOKU_SQUARE_SIZE - SUDOKU_FONT_LENGTH) / 2 + 1)
							+ ((SUDOKU_SQUARE_SIZE + 1) * -1),
					SUDOKU_Y0
							+ ((SUDOKU_SQUARE_SIZE - SUDOKU_FONT_HEIGHT) / 2 + 1)
							+ ((SUDOKU_SQUARE_SIZE + 1) * i), BLACK,
					ASCII_NUMBER_BASE + i + 1);
			Lcd_DisplayChar(
					SUDOKU_X0
							+ ((SUDOKU_SQUARE_SIZE - SUDOKU_FONT_LENGTH) / 2 + 1)
							+ ((SUDOKU_SQUARE_SIZE + 1) * i),
					SUDOKU_Y0
							+ ((SUDOKU_SQUARE_SIZE - SUDOKU_FONT_HEIGHT) / 2 + 1)
							+ ((SUDOKU_SQUARE_SIZE + 1) * -1), BLACK,
					ASCII_NUMBER_BASE + i + 1);
		}
	}
	// Tiene el problema de que aumenta el tiempo de renderizado, pero se evitan algunos bugs gráficos
	for (i = 0; i < SUDOKU_NUM_CUADS + 1; ++i) {
		if (i % 3 == 0) {
			uint8_t uColor = i % 3 != 0 ? DARKGRAY : BLACK;
			Lcd_Draw_VLine(SUDOKU_Y0,
					SUDOKU_Y0 + (SUDOKU_NUM_CUADS * (SUDOKU_SQUARE_SIZE + 1)),
					SUDOKU_X0 + (SUDOKU_SQUARE_SIZE + 1) * i, uColor, 1);
			Lcd_Draw_HLine(SUDOKU_X0,
					SUDOKU_X0 + (SUDOKU_NUM_CUADS * (SUDOKU_SQUARE_SIZE + 1)),
					SUDOKU_Y0 + (SUDOKU_SQUARE_SIZE + 1) * i, uColor, 1);
		}
	}

#ifdef DRAW_MESSAGE
	Lcd_DspAscII8x16(SUDOKU_FONT_LENGTH,
			SCR_YSIZE - SUDOKU_FONT_HEIGHT - SUDOKU_FONT_HEIGHT / 2, BLACK,
			message_A);
#endif
}

void toComplexNotation(int seconds, char* dst) {
	int minutes = seconds / 60;
	seconds %= 60;
	char S_minutes[4];
	char S_seconds[3];
	itoa(seconds, S_seconds, 10);
	if (minutes != 0) {
		itoa(minutes, S_minutes, 10);
	} else {
		S_minutes[0] = ' ';
		S_minutes[1] = ' ';
		S_minutes[2] = '0';
	}

	if (S_minutes[1] == '\0') {
		dst[0] = ' ';
		dst[1] = ' ';
		dst[2] = S_minutes[0];
	} else if (S_minutes[2] == '\0') {
		dst[0] = ' ';
		dst[1] = S_minutes[0];
		dst[2] = S_minutes[1];
	} else {
		dst[0] = S_minutes[0];
		dst[1] = S_minutes[1];
		dst[2] = S_minutes[2];
	}
	dst[3] = ':';
	if (S_seconds[1] == '\0') {
		dst[4] = '0';
		dst[5] = S_seconds[0];
		dst[6] = '\0';
	} else {
		dst[4] = S_seconds[0];
		dst[5] = S_seconds[1];
		dst[6] = '\0';
	}
}

int strlen(char* s) {
	int i = 0;
	while (*(s + i) != '\0') {
		i++;
	}
	return i;
}
void sudoku_graphics_draw_time(int time_playing_s, int time_calculating_ms) {
	char play_time[8];
	char calc_time[8];

	itoa(time_calculating_ms, calc_time, 10);
	toComplexNotation(time_playing_s, play_time);
	// Mensaje de tiempos
	Lcd_DspAscII8x16((SUDOKU_NUM_CUADS + 2) * SUDOKU_SQUARE_SIZE,
			SUDOKU_FONT_HEIGHT / 2, BLACK, time_message);
	// Mensaje de Tiempo jugado
	Lcd_DspAscII8x16((SUDOKU_NUM_CUADS + 2) * SUDOKU_SQUARE_SIZE,
			SUDOKU_FONT_HEIGHT / 2 + SUDOKU_FONT_HEIGHT, BLACK,
			time_playing_short);
	// Tiempo de juego
	Lcd_DspAscII8x16((SUDOKU_NUM_CUADS + 2) * SUDOKU_SQUARE_SIZE,
			(SUDOKU_FONT_HEIGHT / 2) + 2 * SUDOKU_FONT_HEIGHT, BLACK,
			play_time);

	// Mensaje de tiempo de calculo
	Lcd_DspAscII8x16((SUDOKU_NUM_CUADS + 2) * SUDOKU_SQUARE_SIZE,
			(SUDOKU_FONT_HEIGHT / 2) + 3 * SUDOKU_FONT_HEIGHT, BLACK,
			time_calculating_short);

	// Tiempo de calculo
	Lcd_DspAscII8x16((SUDOKU_NUM_CUADS + 2) * SUDOKU_SQUARE_SIZE,
			(SUDOKU_FONT_HEIGHT / 2) + 4 * SUDOKU_FONT_HEIGHT, BLACK,
			calc_time);
}

void sudoku_graphics_print_instructions() {
	int i;
	for (i = 0; i < INSTRUCTION_LINES; ++i) {
		Lcd_DspAscII8x16(20, 16 + 16 * i, BLACK, instructions[i]);
	}
}

void sudoku_graphics_print_title_screen(int lineSelected) {
	int i;
	BitmapViewHorizontallyCentered(
			(SCR_YSIZE / 2) - SUDOKU_FONT_HEIGHT * 2
					- Stru_Bitmap_title.usHeight, Stru_Bitmap_title);

	// Line selection
	int maxLength = 0;
	for (i = 0; i < GAME_TYPES_SIZE; ++i) {
		// Comprobamos si toca actualizar la linea mayor
		if (i == lineSelected - 1) {
			Lcd_DspAscII8x16HorizontallyCentered_inverted(
					(SCR_YSIZE / 2) - SUDOKU_FONT_HEIGHT
							+ SUDOKU_FONT_HEIGHT * i, BLACK,
					game_types_message[i]);
		} else {
			Lcd_DspAscII8x16HorizontallyCentered(
					(SCR_YSIZE / 2) - SUDOKU_FONT_HEIGHT
							+ SUDOKU_FONT_HEIGHT * i, BLACK,
					game_types_message[i]);
		}

	}
	Lcd_DspAscII8x16HorizontallyCentered(
			(SCR_YSIZE / 2) + SUDOKU_FONT_HEIGHT * 4, BLACK,
			(unsigned char *) title_message_selection);
	Lcd_DspAscII8x16HorizontallyCentered(
			(SCR_YSIZE / 2) + SUDOKU_FONT_HEIGHT * 5, BLACK,
			(unsigned char *) title_message);
}

void sudoku_graphics_print_final_screen(int tiempo_juego_s,
		int tiempo_calculos_ms, int errores) {
	char time_playing[7];
	char time_calculating[7];

	toComplexNotation(tiempo_juego_s, time_playing);
	itoa(tiempo_calculos_ms, time_calculating, 10);
	// 3 posibles pantallas
	// Titulo
	BitmapViewHorizontallyCentered(20, Stru_Bitmap_title);

	// Tiempo de juego
	int time_playing_x = ((SCR_XSIZE - strlen(time_playing_message) * 8
			- strlen(time_playing) * 8)) / 2;
	Lcd_DspAscII8x16(time_playing_x, (SCR_YSIZE / 2) - SUDOKU_FONT_HEIGHT * 2,
			BLACK, (unsigned char *) time_playing_message);
	Lcd_DspAscII8x16(time_playing_x + strlen(time_playing_message) * 8,
			(SCR_YSIZE / 2) - 32, BLACK, (unsigned char *) time_playing);

	// Tiempo de calculo
	int time_calculating_x = ((SCR_XSIZE - strlen(time_calculating_message) * 8
			- strlen(time_calculating) * 8)) / 2;
	Lcd_DspAscII8x16(time_calculating_x, (SCR_YSIZE / 2) - SUDOKU_FONT_HEIGHT,
			BLACK, (unsigned char *) time_calculating_message);
	Lcd_DspAscII8x16(time_calculating_x + strlen(time_calculating_message) * 8,
			(SCR_YSIZE / 2) - 16, BLACK, (unsigned char *) time_calculating);

	switch (errores) {
	case 0:
		// Terminado con éxito, aperture entra en escena
		Lcd_DspAscII8x16HorizontallyCentered(
				(SCR_YSIZE / 2) + SUDOKU_FONT_HEIGHT * 3, BLACK,
				(unsigned char *) mensaje_aperture_1);
		Lcd_DspAscII8x16HorizontallyCentered(
				(SCR_YSIZE / 2) + SUDOKU_FONT_HEIGHT * 4, BLACK,
				(unsigned char *) mensaje_aperture_2);
		break;
	default:
		// Con errores/Sin terminar
		Lcd_DspAscII8x16HorizontallyCentered(
				(SCR_YSIZE / 2) + SUDOKU_FONT_HEIGHT * 3, BLACK,
				(unsigned char *) mensaje_fracaso_1);
		Lcd_DspAscII8x16HorizontallyCentered(
				(SCR_YSIZE / 2) + SUDOKU_FONT_HEIGHT * 4, BLACK,
				(unsigned char *) mensaje_fracaso_2);
		break;
	}
}

void sudoku_graphics_print_still_alive(int lineNumber) {
	BitmapViewHorizontallyCentered(16, Stru_Bitmap_aperture);
	int i;
	for (i = 0; i < 6; ++i) {
		Lcd_DspAscII8x16HorizontallyCentered(
				Stru_Bitmap_aperture.usHeight + 16 * 2 + 16 * i, BLACK,
				still_alive[lineNumber + i]);
	}

}

void sudoku_graphics_draw_state(int state, int number) {
	char valor[2];
	if (number > 9) {
		valor[0] = 'A';
		valor[1] = '\0';
	} else {
		itoa(number, valor, 10);
	}
	Lcd_DspAscII8x16((SUDOKU_NUM_CUADS + 2) * SUDOKU_SQUARE_SIZE,
			(SUDOKU_FONT_HEIGHT / 2) + (6 * SUDOKU_FONT_HEIGHT), BLACK,
			mensaje_seleccionando);
	switch (state) {
	case 0:
		// Esperando fila
		Lcd_DspAscII8x16((SUDOKU_NUM_CUADS + 2) * SUDOKU_SQUARE_SIZE,
				(SUDOKU_FONT_HEIGHT / 2) + (7 * SUDOKU_FONT_HEIGHT), BLACK,
				mensaje_fila);
		Lcd_DspAscII8x16(
				(SUDOKU_NUM_CUADS + 2) * SUDOKU_SQUARE_SIZE
						+ (strlen(mensaje_fila) + 1) * 8,
				(SUDOKU_FONT_HEIGHT / 2) + (7 * SUDOKU_FONT_HEIGHT), BLACK,
				valor);
		break;
	case 1:
		// Esperando columna
		Lcd_DspAscII8x16((SUDOKU_NUM_CUADS + 2) * SUDOKU_SQUARE_SIZE,
				(SUDOKU_FONT_HEIGHT / 2) + (7 * SUDOKU_FONT_HEIGHT), BLACK,
				mensaje_columna);
		Lcd_DspAscII8x16(
				(SUDOKU_NUM_CUADS + 2) * SUDOKU_SQUARE_SIZE
						+ (strlen(mensaje_columna) + 1) * 8,
				(SUDOKU_FONT_HEIGHT / 2) + (7 * SUDOKU_FONT_HEIGHT), BLACK,
				valor);
		break;
	case 2:
		// Esperando valor
		Lcd_DspAscII8x16((SUDOKU_NUM_CUADS + 2) * SUDOKU_SQUARE_SIZE,
				(SUDOKU_FONT_HEIGHT / 2) + (7 * SUDOKU_FONT_HEIGHT), BLACK,
				mensaje_valor);
		Lcd_DspAscII8x16(
				(SUDOKU_NUM_CUADS + 2) * SUDOKU_SQUARE_SIZE
						+ (strlen(mensaje_valor) + 1) * 8,
				(SUDOKU_FONT_HEIGHT / 2) + (7 * SUDOKU_FONT_HEIGHT), BLACK,
				valor);
		break;
	default:
		break;
	}
}
