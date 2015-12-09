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
#include "Bmp.h"

#define celdaPos0x(x) (SUDOKU_X0 + 1 + ((SUDOKU_SQUARE_SIZE + 1) * (x)))
#define celdaPos0y(y) (SUDOKU_Y0 + 1 + ((SUDOKU_SQUARE_SIZE + 1) * (y)))

#define DRAW_MESSAGE

#define MESSAGE "Introduzca fila A para finalizar"
//#define TIME_PLAYING "Jugando:"
//#define TIME_CALCULATING "Calculos:"
#define TITLE_MESSAGE "Pulse start para jugar!"

/* A utility function to reverse a string  */
void reverse(char str[], int length) {
	int start = 0;
	int end = length - 1;
	while (start < end) {
		GUISWAP(*(str + start), *(str + end));
		start++;
		end--;
	}
}

// Implementation of itoa()
char* itoa(int num, char* str, int base) {
	int i = 0;
	int isNegative = 0;

	/* Handle 0 explicitely, otherwise empty string is printed for 0 */
	if (num == 0) {
		str[i++] = '0';
		str[i] = '\0';
		return str;
	}

	// In standard itoa(), negative numbers are handled only with
	// base 10. Otherwise numbers are considered unsigned.
	if (num < 0 && base == 10) {
		isNegative = 1;
		num = -num;
	}

	// Process individual digits
	while (num != 0) {
		int rem = num % base;
		str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
		num = num / base;
	}

	// If number is negative, append '-'
	if (isNegative)
		str[i++] = '-';

	str[i] = '\0'; // Append string terminator

	// Reverse the string
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
	Lcd_Draw_Box_inverted(SUDOKU_X0 + 1 + ((SUDOKU_SQUARE_SIZE + 1) * x),
			SUDOKU_Y0 + 1 + ((SUDOKU_SQUARE_SIZE + 1) * y),
			SUDOKU_X0 + SUDOKU_SQUARE_SIZE + ((SUDOKU_SQUARE_SIZE + 1) * x),
			SUDOKU_Y0 + SUDOKU_SQUARE_SIZE + ((SUDOKU_SQUARE_SIZE + 1) * y),
			BLACK);
}

inline void sudoku_graphics_invert_square(INT8 x, INT8 y) {
	ReverseSquare(SUDOKU_X0 + 1 + ((SUDOKU_SQUARE_SIZE + 1) * x),
			SUDOKU_Y0 + 1 + ((SUDOKU_SQUARE_SIZE + 1) * y),
			SUDOKU_X0 + SUDOKU_SQUARE_SIZE + ((SUDOKU_SQUARE_SIZE + 1) * x),
			SUDOKU_Y0 + SUDOKU_SQUARE_SIZE + ((SUDOKU_SQUARE_SIZE + 1) * y));
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
		ReverseSquare(celdaPos0x(x) + 2, celdaPos0y(y) + 2,
				celdaPos0x(x) + SUDOKU_SQUARE_SIZE / 3 - 3,
				celdaPos0y(y) + SUDOKU_SQUARE_SIZE / 3 - 3);
		break;
	case 2:
		ReverseSquare(celdaPos0x(x) + (SUDOKU_SQUARE_SIZE / 3) + 2,
				celdaPos0y(y) + 2,
				celdaPos0x(x) + 2 * (SUDOKU_SQUARE_SIZE / 3) - 3,
				celdaPos0y(y) + SUDOKU_SQUARE_SIZE / 3 - 3);
		break;
	case 3:
		ReverseSquare(celdaPos0x(x) + 2 * (SUDOKU_SQUARE_SIZE / 3) + 2,
				celdaPos0y(y) + 2, celdaPos0x(x) + SUDOKU_SQUARE_SIZE - 3,
				celdaPos0y(y) + SUDOKU_SQUARE_SIZE / 3 - 3);
		break;
	case 4:
		ReverseSquare(celdaPos0x(x) + 2,
				celdaPos0y(y) + SUDOKU_SQUARE_SIZE / 3 + 2,
				celdaPos0x(x) + SUDOKU_SQUARE_SIZE / 3 - 3,
				celdaPos0y(y) + 2 * (SUDOKU_SQUARE_SIZE / 3) - 3);
		break;
	case 5:
		ReverseSquare(celdaPos0x(x) + (SUDOKU_SQUARE_SIZE / 3) + 2,
				celdaPos0y(y) + SUDOKU_SQUARE_SIZE / 3 + 2,
				celdaPos0x(x) + 2 * (SUDOKU_SQUARE_SIZE / 3) - 3,
				celdaPos0y(y) + 2 * (SUDOKU_SQUARE_SIZE / 3) - 3);
		break;
	case 6:
		ReverseSquare(celdaPos0x(x) + 2 * (SUDOKU_SQUARE_SIZE / 3) + 2,
				celdaPos0y(y) + SUDOKU_SQUARE_SIZE / 3 + 2,
				celdaPos0x(x) + SUDOKU_SQUARE_SIZE - 3,
				celdaPos0y(y) + 2 * (SUDOKU_SQUARE_SIZE / 3) - 3);
		break;
	case 7:
		ReverseSquare(celdaPos0x(x) + 2,
				celdaPos0y(y) + 2 * (SUDOKU_SQUARE_SIZE / 3) + 2,
				celdaPos0x(x) + SUDOKU_SQUARE_SIZE / 3 - 3,
				celdaPos0y(y) + SUDOKU_SQUARE_SIZE - 3);
		break;
	case 8:
		ReverseSquare(celdaPos0x(x) + (SUDOKU_SQUARE_SIZE / 3) + 2,
				celdaPos0y(y) + 2 * (SUDOKU_SQUARE_SIZE / 3) + 2,
				celdaPos0x(x) + 2 * (SUDOKU_SQUARE_SIZE / 3) - 3,
				celdaPos0y(y) + SUDOKU_SQUARE_SIZE - 3);
		break;
	case 9:
		ReverseSquare(celdaPos0x(x) + 2 * (SUDOKU_SQUARE_SIZE / 3) + 2,
				celdaPos0y(y) + 2 * (SUDOKU_SQUARE_SIZE / 3) + 2,
				celdaPos0x(x) + SUDOKU_SQUARE_SIZE - 3,
				celdaPos0y(y) + SUDOKU_SQUARE_SIZE - 3);
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
					if (es_candidato(cuadricula[j][i], k)) {
						sudoku_graphics_mark_error(i, j, k);
					}
				}
			} else {
				// Simplemente ponemos el numero en el cuadrado, y si es error lo marcamos
				sudoku_graphics_put_number_in_square(i, j, valor,
						es_pista(cuadricula[j][i]) ? BLACK : DARKGRAY);
				if (es_error(cuadricula[j][i])) {
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
	// Tiene el problema de que aumenta el tiempo de renderizado, pero se evitan algunos bugs gr�ficos
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
	char *message = MESSAGE;

	Lcd_DspAscII8x16(SUDOKU_FONT_LENGTH,
			SCR_YSIZE - SUDOKU_FONT_HEIGHT - SUDOKU_FONT_HEIGHT / 2, BLACK,
			message);
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
void sudoku_graphics_draw_time(int time_playing_s, int time_calculating_us) {
	char *s = "Jugando:";
	char *t = "Calculos:";
	char play_time[8];
	char calc_time[8];

	int time_calculating_ms = time_calculating_us / 1000;

	itoa(time_calculating_ms, calc_time, 10);
	toComplexNotation(time_playing_s, play_time);
	// Mensaje de Tiempo jugado
	Lcd_DspAscII8x16((SUDOKU_NUM_CUADS + 2) * SUDOKU_SQUARE_SIZE,
			SUDOKU_FONT_HEIGHT / 2, BLACK, s);
	// Mensaje de tiempo de calculo
	Lcd_DspAscII8x16((SUDOKU_NUM_CUADS + 2) * SUDOKU_SQUARE_SIZE,
			(SUDOKU_FONT_HEIGHT / 2) + SUDOKU_FONT_HEIGHT, BLACK, t);

	// Tiempo de juego
	Lcd_DspAscII8x16(
			(SUDOKU_NUM_CUADS + 2) * SUDOKU_SQUARE_SIZE + (strlen(s) - 1) * 8,
			(SUDOKU_FONT_HEIGHT / 2), BLACK, play_time);

	// Tiempo de calculo
	Lcd_DspAscII8x16(
			(SUDOKU_NUM_CUADS + 2) * SUDOKU_SQUARE_SIZE + (strlen(t) + 1) * 8,
			(SUDOKU_FONT_HEIGHT / 2) + SUDOKU_FONT_HEIGHT, BLACK, calc_time);
//	Lcd_DspAscII8x16(
//			(SUDOKU_NUM_CUADS + 2) * SUDOKU_SQUARE_SIZE
//					+ SUDOKU_FONT_LENGTH * 10,
//			SUDOKU_FONT_HEIGHT / 2, BLACK, play_time_s);

}

void sudoku_graphics_print_title_screen() {
	char *s = TITLE_MESSAGE;
	Lcd_DspAscII8x16(0, 0, BLACK, s);
	BitmapView(0, 0, Stru_Bitmap_title);
}