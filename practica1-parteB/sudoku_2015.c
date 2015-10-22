#include <inttypes.h>
#include <string.h> // Para memcmp()
// Tamaños de la cuadricula
// Se utilizan 16 columnas para facilitar la visualización
enum {
	NUM_FILAS = 9, NUM_COLUMNAS = 16, TAM_REGION = 3, NUM_REGION = 3
};

enum {
	FALSE = 0, TRUE = 1
};

typedef uint16_t CELDA;
// La información de cada celda está codificada en 16 bits
// con el siguiente formato (empezando en el bit más significativo):
// 4 MSB VALOR
// 1 bit PISTA
#define CELDA_PISTA(c) (c & 0b0000100000000000)
// 1 bit ERROR
#define CELDA_ERROR(c) (c & 0b0000010000000000)
// 1 bit no usado
// 9 LSB CANDIDATOS

inline void celda_poner_valor(CELDA *celdaptr, uint8_t val) {
	*celdaptr = (*celdaptr & 0x0FFF) | ((val & 0x000F) << 12);
}
inline uint8_t celda_leer_valor(CELDA celda) {
	return celda >> 12;
}

// todo: nombre a celda_cambiar_candidatos
inline void celda_cambiar_pista(uint8_t valor_celda,
		CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS], uint8_t fila,
		uint8_t columna) {
	cuadricula[fila][columna] = cuadricula[fila][columna]
			& ~(1 << (valor_celda - 1));
}
// funcion a implementar en ARM
//extern int sudoku_recalcular_arm(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS]);

// funcion a implementar en Thumb
//extern int sudoku_candidatos_thumb(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS], uint8_t fila, uint8_t columna);

extern int sudoku_candidatos_arm(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS],
		uint8_t fila, uint8_t columna);
extern int sudoku_candidatos_thumb_prologo(
		CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS], uint8_t fila,
		uint8_t columna);

////////////////////////////////////////////////////////////////////////////////
// dada una determinada celda encuentra los posibles valores candidatos
// y guarda el mapa de bits en la celda
// retorna false si la celda esta vacia, true si contiene un valor
int sudoku_candidatos_c(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS], uint8_t fila,
		uint8_t columna) {
	//iniciar candidatos
	// inicialmente todos a 1 (todos los números son candidatos), despues los iremos quitando
	if (celda_leer_valor(cuadricula[fila][columna]) == 0) {
		cuadricula[fila][columna] = (cuadricula[fila][columna] | 0x01FF);
		//recorrer fila y columna recalculando candidatos
		// TODO: Mirar la diferencia entre for y while en altas prestaciones
		// TODO: mirar como antes se declaraban las variables al principio de bloque
		int i = 0;
		while (i < 9) {
			uint8_t valor_celda_fila = celda_leer_valor(cuadricula[i][columna]);
			uint8_t valor_celda_columna = celda_leer_valor(cuadricula[fila][i]);
			celda_cambiar_pista(valor_celda_fila, cuadricula, fila, columna);
			celda_cambiar_pista(valor_celda_columna, cuadricula, fila, columna);
			i += 1;
		}

		//recorrer region recalculando candidatos
		// TODO:
		// fila2 = (fila/3)*3
		uint8_t fila2 = fila - fila % TAM_REGION; // Obtenemos la fila inicial de la región
		uint8_t columna2 = columna - columna % TAM_REGION; // Obtenemos la fila inicial de la región
		// Recorrido por columnas
		// Posibilidad de quitar los *0xF, en esta situación es indistinto (asumiendo que almacenamos NUM_COLUMNAS*0xF),
		// dado que se realizan de todas formas 3 productos, en caso de regiones más grandes merece aún más la pena,
		// en caso de regiones más pequeñas es mejor quitarlo.
		int recorre_fila = fila2;
		while (recorre_fila < fila2 + TAM_REGION) {
			int recorre_columna = columna2;
			while (recorre_columna < columna2 + TAM_REGION) {
				uint8_t valor_celda = celda_leer_valor(
						cuadricula[recorre_fila][recorre_columna]);
				celda_cambiar_pista(valor_celda, cuadricula, fila, columna);
				recorre_columna += 1;
			}
			recorre_fila += 1;
		}
	}
	//retornar indicando si la celda tiene un valor o esta vacia
	if ((cuadricula[fila][columna] & 0xF000) != 0)
		return TRUE;
	return FALSE;

}

////////////////////////////////////////////////////////////////////////////////
// recalcula el tablero (9x9)
// retorna el numero de celdas vacias
int sudoku_recalcular_c_c(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS]) {
	int celdas_vacias = 0;
	// Para cada fila
	int fila = 0;
	while (fila < (NUM_REGION * TAM_REGION)) {
		int columna = 0;
		while (columna < (NUM_REGION * TAM_REGION)) {
			if (!sudoku_candidatos_c(cuadricula, fila, columna)) {
				// FALSE devuelto=>Celda vacía
				celdas_vacias++;
			}
			columna += 1;
		}
		fila += 1;
	}
	//retornar el numero de celdas vacias
	return celdas_vacias;
}

int sudoku_recalcular_c_a(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS]) {
	int celdas_vacias = 0;
	// Para cada fila
	int fila = 0;
	while (fila < (NUM_REGION * TAM_REGION)) {
		int columna = 0;
		while (columna < (NUM_REGION * TAM_REGION)) {
			if (!sudoku_candidatos_arm(cuadricula, fila, columna)) {
				// FALSE devuelto=>Celda vacía
				celdas_vacias++;
			}
			columna += 1;
		}
		fila += 1;
	}
	//retornar el numero de celdas vacias
	return celdas_vacias;
}

int sudoku_recalcular_c_t(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS]) {
	int celdas_vacias = 0;
	// Para cada fila
	int fila = 0;
	while (fila < (NUM_REGION * TAM_REGION)) {
		int columna = 0;
		while (columna < (NUM_REGION * TAM_REGION)) {
			if (!sudoku_candidatos_thumb_prologo(cuadricula, fila, columna)) {
				// FALSE devuelto=>Celda vacía
				celdas_vacias++;
			}
			columna += 1;
		}
		fila += 1;
	}
	//retornar el numero de celdas vacias
	return celdas_vacias;
}

// Declaracion de las 6 cuadriculas, una por combinacion de funciones
extern CELDA cuadricula_c_c[NUM_FILAS][NUM_COLUMNAS];
extern CELDA cuadricula_c_a[NUM_FILAS][NUM_COLUMNAS];
extern CELDA cuadricula_c_t[NUM_FILAS][NUM_COLUMNAS];
extern CELDA cuadricula_a_c[NUM_FILAS][NUM_COLUMNAS];
extern CELDA cuadricula_a_a[NUM_FILAS][NUM_COLUMNAS];
extern CELDA cuadricula_a_t[NUM_FILAS][NUM_COLUMNAS];

// Declaracion de las funciones de recorrido en asm
extern int sudoku_recalcular_a_c(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS]);
extern int sudoku_recalcular_a_a(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS]);
extern int sudoku_recalcular_a_t(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS]);

////////////////////////////////////////////////////////////////////////////////
// proceso principal del juego que recibe el tablero,
// y la señal de ready que indica que se han actualizado fila y columna

//
// Funcion que recibe
//
void sudoku9x9(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS], char *ready) {
	int c_c_bien;
	int c_a_bien;
	int c_t_bien;
	int a_c_bien;
	int a_a_bien;
	int a_t_bien;

	unsigned int celdas_vacias_c_c; //numero de celdas aun vacias
	unsigned int celdas_vacias_c_a; //numero de celdas aun vacias
	unsigned int celdas_vacias_c_t; //numero de celdas aun vacias
	unsigned int celdas_vacias_a_c; //numero de celdas aun vacias
	unsigned int celdas_vacias_a_a; //numero de celdas aun vacias
	unsigned int celdas_vacias_a_t; //numero de celdas aun vacias
	// C-C
	#define NUM_TEST 1
	int i = 0;
	int f = 0;
	for (i = 0; i < NUM_TEST; i++) {
		celdas_vacias_c_c = sudoku_recalcular_c_c(cuadricula_c_c);
	}
	c_c_bien = memcmp(cuadricula_c_c, cuadricula, sizeof(cuadricula_c_c));
	f = 2;
	for (i = 0; i < NUM_TEST; i++) {
		celdas_vacias_c_a = sudoku_recalcular_c_a(cuadricula_c_a);
	}
	// c_a_bien = memcmp(cuadricula_c_a, cuadricula, sizeof(cuadricula_c_a));
	f = 3;
	for (i = 0; i < NUM_TEST; i++) {
		celdas_vacias_c_t = sudoku_recalcular_c_t(cuadricula_c_t);
	}
	f = 4;
	// c_t_bien = memcmp(cuadricula_c_t, cuadricula, sizeof(cuadricula_c_t));

	for (i = 0; i < NUM_TEST; i++) {
		celdas_vacias_a_c = sudoku_recalcular_a_c(cuadricula_a_c);
	}
	//a_c_bien = memcmp(cuadricula_a_c, cuadricula, sizeof(cuadricula_a_c));
	f = 6;
	for (i = 0; i < NUM_TEST; i++) {
		celdas_vacias_a_a = sudoku_recalcular_a_a(cuadricula_a_a);
	}
	//a_a_bien = memcmp(cuadricula_a_a, cuadricula, sizeof(cuadricula_a_a));
	f = 7;
	for (i = 0; i < NUM_TEST; i++) {
		celdas_vacias_a_t = sudoku_recalcular_a_t(cuadricula_a_t);
	}
	//a_t_bien = memcmp(cuadricula_a_t, cuadricula, sizeof(cuadricula_a_t));
	f = 8;
//	unsigned int celdas_vacias; //numero de celdas aun vacias
//	celdas_vacias = sudoku_recalcular_c(cuadricula);
	// Mandamos la señal de ready
}

