/*********************************************************************************************
* Fichero:	main.c
* Autor:	
* Descrip:	codigo de entrada de C
* Version:	<P6-ARM>	
*********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "44blib.h"
#include "44b.h"

/*--- declaracion de funciones ---*/
void main(void);

/*--- funciones externas ---*/
extern void Lcd_Test();

/*--- codigo de la funcion ---*/
void Main(void)
{
    sys_init();        /* inicializacion de la placa, interrupciones, puertos y UART */
	
	Lcd_Test();        /* inicializacion LCD y visualizacion texto y rectangulos     */
	while(1);
}

