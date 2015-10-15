#######################################################################################################
# Función ARM: Modifica las pistas de la tabla de sudoku pasada
# Params:
# r0: direccion inicial de la tabla de sudoku
#######################################################################################################
.global sudoku_recalcular_a_t
.arm
sudoku_recalcular_a_t:
	# r0: direccion de la cuadricula
	# r1-r3: NA
	# r4: contador de filas
	# r5: contador de columnas
	# r6: test
	STMFD   sp!, {r4-r11,lr} // Almacenamos los registros que podemos modificar
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

	ldr 	r8, =sudoku_candidatos_thumb // Cargamos los parametros de la función
	add		r8, r8, #1 // Le sumamos 1 para que haga exchange

	mov lr, pc // Guardamos el lr
	add lr, lr, #8 // Le sumamos para obtener la dirección de vuelta correcta
	bx	r8 // Saltamos a la rutina

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
	LDMFD   sp!, {r4-r11,lr} // Recargamos los registros almacenados al inicio
	BX		lr // Retornamos al lugar de invocación, en modo ARM
