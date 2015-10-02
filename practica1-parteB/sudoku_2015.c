#include <inttypes.h>

// Tamaños de la cuadricula
// Se utilizan 16 columnas para facilitar la visualización
enum {NUM_FILAS = 9, NUM_COLUMNAS = 16};

enum {FALSE = 0, TRUE = 1};

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

// funcion a implementar en ARM
extern int sudoku_recalcular_arm(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS]);

// funcion a implementar en Thumb
extern int sudoku_recalcular_thumb(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS]);   



inline uint8_t next_line(int ) {

}

inline uint8_t next_column(int ) {

}

////////////////////////////////////////////////////////////////////////////////
// dada una determinada celda encuentra los posibles valores candidatos
// y guarda el mapa de bits en la celda
// retorna false si la celda esta vacia, true si contiene un valor
int sudoku_candidatos_c(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS],
                        uint8_t fila, uint8_t columna) {
    //iniciar candidatos
	*cuadricula[fila][columna] = (*cuadricula[fila][columna] | 0x01FF);
    //recorrer fila recalculando candidatos
	for(int i=(fila & 0xFFF0); i<(fila & 0xFFF0)+18; i+=2) {
		if(celda_leer_valor(*i)!=0) {
			*cuadricula[fila][columna] = *cuadricula[fila][columna] & ~(1<<(celda_leer_valor(*i)-1));
		}
	}
    //recorer columna recalculando candidatos
	for(int i=0; i<NUM_FILAS; i+=NUM_COLUMNAS*2) {
		if(celda_leer_valor(*i)!=0) {
			*cuadricula[fila][columna] = *cuadricula[fila][columna] & ~(1<<(celda_leer_valor(*i)-1));
		}
	}
    //recorrer region recalculando candidatos
    //retornar indicando si la celda tiene un valor o esta vacia
    if ((cuadricula[fila][columna] & 0xF000) != 0) return TRUE;
    return FALSE;
}

////////////////////////////////////////////////////////////////////////////////
// recalcula todo el tablero (9x9)
// retorna el numero de celdas vacias
int sudoku_recalcular_c(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS]) {
    //para cada fila
      //para cada columna
        //determinar candidatos
        //actualizar contador de celdas vacias
    //retornar el numero de celdas vacias
    return 1;
}

////////////////////////////////////////////////////////////////////////////////
// proceso principal del juego que recibe el tablero,
// y la señal de ready que indica que se han actualizado fila y columna
void sudoku9x9(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS], char *ready) {
    unsigned int celdas_vacias;     //numero de celdas aun vacias                       

    celdas_vacias = sudoku_recalcular_c(cuadricula);
}

