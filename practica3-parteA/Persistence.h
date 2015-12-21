/** @file Persistence.h
 *  @brief Modulo que permite una forma simple de persistencia
 *
 * Modulo que gestiona persistencia entre reseteos (Y perdidas de
 * corriente) mediante el almacenado de informacion en los registros
 * de alarma (rALM...), por lo que no se pueden usar a la vez que esta
 * libreria. El espacio que ocupa el entero y los caracteres es
 * compartido, de tal forma que los caracteres 2-5 representan los
 * bytes del entero, respectivamente, de mas significativo a menos
 * significativo.
 *
 * @author Guillermo Robles Gonzalez
 */
#ifndef PERSISTENCE_H_
#define PERSISTENCE_H_

#include "44b.h"

/**
 * Devuelve el entero almacenado
 */
int  Persistence_read_int(void);

/**
 * Devuelve el char almacenado
 *
 * @param[in] n Numero de char a leer, rango [0,5]
 */
char Persistence_read_char(int n);

/**
 * Guarda el entero pasado, sobreescribiendo el valor anterior
 *
 * @param[in] n Entero a guardar
 */
void Persistence_save_int(int n);

/**
 * Guarda el char dado en la posicion de memoria dada
 *
 * @param[in] n Posicion en la que se guardara, rango [0,5]
 * @param[in] c Char a guardar
 */
void Persistence_save_char(int n,char c);

#endif /* PERSISTENCE_H_ */
