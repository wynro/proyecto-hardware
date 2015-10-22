/**
 * Fichero que guarda la funcion arm_recorre_a_a, que recorre el sudoku usando
 * una funcion ARM y analizando las casillas con una funcion C
 *
 * Autor: Guillermo Robles Gonzalez (604409), Sergio Martin Segura (622612)
 */

#######################################################################################################
# Función ARM: Modifica las pistas de la tabla de sudoku pasada
# Params:
# r0: direccion inicial de la tabla de sudoku
#######################################################################################################
.global sudoku_recalcular_a_c
.arm
sudoku_recalcular_a_c:
	# r0: direccion de la cuadricula
	# r1-r3: NA
	# r4: contador de filas
	# r5: contador de columnas
	# r6: test
	mov ip, sp	// Para evitar el problema de apilar sp mientras lo cambiamos
	STMDB   sp!, {r4-r10,fp,ip,lr,pc} // Guardamos los parametros a tocar
	sub fp, ip, #4 // Colocamos el fp apuntando a su lugar
	# r4: contador de filas
	# r5: contador de columnas
	# r6: Almacen de la direccion inical
	# r7: Contador de celdas vacias
	MOV     r6, r0 // Guardamos la direccion inicial
	MOV 	r7, #0 // Inicializamos el contador de celdas vacias
	MOV 	r4, #0 // Inicializamos contador de filas
sudoku_recalcular_arm_filas:

	MOV		r5,	#0	// Incializamos el contador de columnas
sudoku_recalcular_arm_columnas:

	// Montamos la llamada

	MOV     r0, r6 // Cargamos el parametro direccion inicial
	MOV 	r1, r4 // Cargamos el parametro fila
	MOV 	r2, r5 // Cargamos el parametro columna

	bl sudoku_candidatos_c

	// Tras volver, procesamos la respuesta
	cmp     r0, #0 // Comprobamos si ha devuelto falso
	addeq   r7, r7, #1 // Incrementamos el contador de celdas vacias si la llamada ha devuelto falso

	ADD	r5, r5, #1 // Incrementamos el contador de columnas
	CMP 	r5, #9 // Comprobamos si hemos llegado al limite
	BLT	sudoku_recalcular_arm_columnas // En caso de no haber llegado, volvemos al principio del bucle

	ADD	r4, r4, #1 // Incrementamos el contador de filas
	CMP 	r4, #9 // Comprobamos si hemos llegado al limite
	BLT	sudoku_recalcular_arm_filas // En caso de no haber llegado, volvemos al principio del bucle


	mov     r0, r7 // Movemos el numero de celdas contadas para devolverlo como respuesta
	LDMDB   fp, {r4-r10,fp,sp,pc} // Recargamos los registros almacenados al inicio
