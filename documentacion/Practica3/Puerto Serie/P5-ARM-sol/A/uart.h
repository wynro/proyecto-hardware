/*********************************************************************************************
* Fichero:	uart.h
* Autor:	
* Descrip:	cabecera de las funciones de UART
* Version: <P5-ARM-A>	
*********************************************************************************************/

#ifndef __UART_H__
#define __UART_H__

#ifdef __cplusplus
extern "C" {
#endif

/*--- definic�n de constantes ---*/
#define LF_char 0x0A	/* Caracter de salto de l�nea */
#define CR_char 0x0D    /* Caracter de retorno de carro */

/*--- prototipos de funciones ---*/
void Uart_Config(void);
void Uart_TxEmpty(void);
char Uart_Getch(void);
void Uart_SendByte(int data);
void Uart_SendString(char *pt);
void Uart_Printf(char *fmt,...);

#ifdef __cplusplus
}
#endif

#endif /* __UART_H__ */