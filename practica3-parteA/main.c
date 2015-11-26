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

#define SUDOKU_NUM_CUADS 9
#define SUDOKU_SQUARE_SIZE 18
#define SUDOKU_X0 30
#define SUDOKU_Y0 30

#define SUDOKU_FONT_HEIGHT 16
#define SUDOKU_FONT_LENGTH 8

#define ASCII_NUMBER_BASE 48

/*--- declaracion de funciones ---*/
void Main(void);

inline void putNumberInSquare(INT8 x, INT8 y, INT8 number) {
	Lcd_DisplayChar(
			SUDOKU_X0 + ((SUDOKU_SQUARE_SIZE - SUDOKU_FONT_LENGTH) / 2 + 1)
					+ ((SUDOKU_SQUARE_SIZE + 1) * x),
			SUDOKU_Y0 + ((SUDOKU_SQUARE_SIZE - SUDOKU_FONT_HEIGHT) + 1)
					+ ((SUDOKU_SQUARE_SIZE + 1) * y), BLACK,
			ASCII_NUMBER_BASE + number);
}

inline void remarkSquare(INT8 x, INT8 y) {
	Lcd_Draw_Box(SUDOKU_X0 + 1 + ((SUDOKU_SQUARE_SIZE + 1) * x),
			SUDOKU_Y0 + 1 + ((SUDOKU_SQUARE_SIZE + 1) * y),
			SUDOKU_X0 + SUDOKU_SQUARE_SIZE + ((SUDOKU_SQUARE_SIZE + 1) * x),
			SUDOKU_Y0 + SUDOKU_SQUARE_SIZE + ((SUDOKU_SQUARE_SIZE + 1) * y),
			BLACK);
}

inline void invertSquare(INT8 x, INT8 y) {
	ReverseSquare(SUDOKU_X0 + 1 + ((SUDOKU_SQUARE_SIZE + 1) * x),
			SUDOKU_Y0 + 1 + ((SUDOKU_SQUARE_SIZE + 1) * y),
			SUDOKU_X0 + SUDOKU_SQUARE_SIZE + ((SUDOKU_SQUARE_SIZE + 1) * x),
			SUDOKU_Y0 + SUDOKU_SQUARE_SIZE + ((SUDOKU_SQUARE_SIZE + 1) * y));
}


/*--- codigo de la funcion ---*/
void Main(void) {
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
//		Lcd_DspHz16(10,0,DARKGRAY,"”¢›ÌÃÿ»˝–« µ—È∆¿π¿∞Â");
#endif

	//Lcd_DspAscII8x16(10, 20, BLACK, "Codigo del puesto: ");
	int i;
//	for (i = 0; i < 10; ++i) {
//		Lcd_Draw_HLine(SUDOKU_X0, 100, SUDOKU_X0 + 11 * i, BLACK, 1);
//		Lcd_Draw_VLine(SUDOKU_Y0, 100, SUDOKU_Y0 + 11 * i, BLACK, 1);
//	}
//	Lcd_Draw_HLine(10, 100, 3, BLACK, 1);
//	Lcd_Draw_Box(10,30,50,40,BLACK);

//	Lcd_Draw_VLine(0,100,0,BLACK,1);
//	Lcd_Draw_VLine(0,100,11,BLACK,1);
//	Lcd_Draw_VLine(0,100,22,BLACK,1);
//	Lcd_Draw_VLine(0,100,33,BLACK,1);

// Guidelines
	Lcd_Draw_HLine_pointed(10, 300, 30, BLACK, 1);
	Lcd_Draw_VLine_pointed(25, 300, 15, BLACK, 1);

	// Dibujamos la cuadricula en sÌ
	for (i = 0; i < SUDOKU_NUM_CUADS + 1; ++i) {
		Lcd_Draw_VLine(SUDOKU_Y0,
				SUDOKU_Y0 + (SUDOKU_NUM_CUADS * (SUDOKU_SQUARE_SIZE + 1)),
				SUDOKU_X0 + (SUDOKU_SQUARE_SIZE + 1) * i, BLACK, 1);
		Lcd_Draw_HLine(SUDOKU_X0,
				SUDOKU_X0 + (SUDOKU_NUM_CUADS * (SUDOKU_SQUARE_SIZE + 1)),
				SUDOKU_Y0 + (SUDOKU_SQUARE_SIZE + 1) * i, BLACK, 1);
	}

	// Dibujamos los numeros del lateral
	for (i = 0; i < SUDOKU_NUM_CUADS; i++) {
		Lcd_DisplayChar(SUDOKU_X0 + 6 + ((SUDOKU_SQUARE_SIZE + 1) * i),
				SUDOKU_Y0 - (SUDOKU_SQUARE_SIZE + 1) + 3, BLACK,
				ASCII_NUMBER_BASE + i);
		Lcd_DisplayChar(SUDOKU_X0 + 6 - (SUDOKU_SQUARE_SIZE + 1),
				SUDOKU_Y0 + 3 + ((SUDOKU_SQUARE_SIZE + 1) * i), BLACK,
				ASCII_NUMBER_BASE + i);
	}

	putNumberInSquare(3, 3, 4);
	remarkSquare(0, 1);
	invertSquare(3, 3);
	ReverseLine(20, 40);
	ReverseSquare(0, 0, 40, 50);
	Lcd_Circle(200,200,25,BLACK);

	Lcd_Circle_Filled(200,100,25,BLACK);

	Lcd_DisplayChar_inverted(0, 0, BLACK, ASCII_NUMBER_BASE + 3);
//	Lcd_DisplayChar(SUDOKU_X0+6,SUDOKU_Y0+3,BLACK,ASCII_NUMBER_BASE+3);
//	Lcd_DisplayChar(SUDOKU_X0,SUDOKU_Y0+3,BLACK,ASCII_NUMBER_BASE+3);
//
//	Lcd_DisplayChar(32,32,BLACK,ASCII_NUMBER_BASE+5);
//
//	Lcd_Draw_Box(10, 40, 310, 230, 14);
//	Lcd_Draw_Box(20, 45, 300, 225, 13);
//	Lcd_Draw_Box(30, 50, 290, 220, 12);
//	Lcd_Draw_Box(40, 55, 280, 215, 11);
//	Lcd_Draw_Box(50, 60, 270, 210, 10);
//	Lcd_Draw_Box(60, 65, 260, 205, 9);
//	Lcd_Draw_Box(70, 70, 250, 200, 8);
//	Lcd_Draw_Box(80, 75, 240, 195, 7);
//	Lcd_Draw_Box(90, 80, 230, 190, 6);
//	Lcd_Draw_Box(100, 85, 220, 185, 5);
//	Lcd_Draw_Box(110, 90, 210, 180, 4);
//	Lcd_Draw_Box(120, 95, 200, 175, 3);
//	Lcd_Draw_Box(130, 100, 190, 170, 2);
	//BitmapView(125, 135, Stru_Bitmap_gbMouse);
	Lcd_Dma_Trans();

	DelayMs(100);
	Lcd_Circle_Filled(200,150,10,LIGHTGRAY);
	Lcd_Dma_Trans();
	while (1)
		;
}

