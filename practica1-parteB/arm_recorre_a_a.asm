#######################################################################################################
# Función ARM: Modifica las pistas de la tabla de sudoku pasada, usando como
# analista de celdas una subrutina en ARM
# Params:
# r0: direccion inicial de la tabla de sudoku
# Return:
# r0:
#######################################################################################################
.global sudoku_recalcular_a_a
.arm
sudoku_recalcular_a_a:
	# r0: direccion de la cuadricula
	# r1-r3: NA
	# r4: contador de filas
	# r5: contador de columnas
	# r6: test
	mov ip, sp
	STMDB   sp!, {r4-r10,fp,ip,lr,pc} // Guardamos los parametros a tocar
	sub fp, ip, #4
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
	bl sudoku_candidatos_arm

	// Tras volver, procesamos la respuesta
	cmp     r0, #0 // Comprobamos si ha devuelto falso
	addeq   r7, r7, #1 // Incrementamos el contador de celdas vacias si la llamada ha devuelto falso

	add		r5, r5, #1 // Incrementamos el contador de columnas
	cmp 	r5, #9 // Comprobamos si hemos llegado al limite
	blt		sudoku_recalcular_arm_columnas // En caso de no haber llegado, volvemos al principio del bucle

	add		r4, r4, #1 // Incrementamos el contador de filas
	cmp 	r4, #9 // Comprobamos si hemos llegado al limite
	blt		sudoku_recalcular_arm_filas // En caso de no haber llegado, volvemos al principio del bucle

	mov     r0, r7 // Movemos el numero de celdas contadas para devolverlo como respuesta


	LDMDB   fp, {r4-r10,fp,sp,pc} // Recargamos los registros almacenados al inicio
