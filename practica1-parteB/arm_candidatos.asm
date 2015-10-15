#######################################################################################################
# Función ARM: Dada una cuadricula y una celda, busca en la fila, en la columna y en la region de dicha
# celda y ajusta las pistas de la celda en consonancia
# Params:
# r0: direccion inicial de la tabla de sudoku
# r1: coordenada fila
# r2: coordenada columna
#######################################################################################################
.global sudoku_candidatos_arm
.arm
sudoku_candidatos_arm:
	# r3: direccion de la celda siendo modificada
	# r4: valor de la celda modificada
	STMFD   sp!, {r4-r11,lr} // Guardamos los parametros a tocar
	// Calculamos la direccion de la celda siendo modificada
	mov r3, r0 // Movemos la direccion inicial para poder modificarla
	add r3, r3, r1, LSL #5 // Le sumamos el offset de fila
	add r3, r3, r2, LSL #1 // Le sumamos el offset de columna
	ldrh r4, [r3] // Cargamos la celda a modificar (Guardamos la direccion para poder moverla despues a su sit)
	mov r5, r4, LSR #12 // Lo movemos cortando el numero
	cmp r5, #0 // Si la celda visitada NO es cero, es pista inicial y no hay que tocarla
	bne sudoku_candidatos_arm_fin // Saltamos al final para no tocar la celda
	// Recordar que, en el algoritmo elegido, primero marcamos todos los numeros como posibles, y luego vamos eliminando
	// posibilidades conforme las encontremos
	// Ponemos todas las pistas a 1
	orr r4, r4, #496 // Dividimos la constante 0x1FF
	orr r4, r4, #15 // Para aplicarsela
	// Recorrido por fila-columna
	# r5: iterador de fila-columna
	mov r5, #0 // Indice de recorrido
sudoku_candidatos_arm_bucle_fila_columna:
	mov r6, r0 // Movemos la direccion inicial para no modificarla
	add r6, r6, r1, LSL #5 // Le sumamos el offset de la fila inicial
	add r6, r6, r5, LSL #1 // Le sumamos el offset de la columna contador
	ldrh r6, [r6] // Cargamos el valor a leer
	lsr r6, #12 // Separamos el numero de la celda
	cmp r6, #0 // Si el numero es cero lo ignoramos, no nos vale como pista
	beq sudoku_candidatos_arm_bucle_fila_columna_si_fila_cero
	sub r6, r6, #1 // Le restamos 1 para que sea indice valido

	# r7: auxiliar. Mascara
	mov r7, #1 // Ponemos el 1 de la futura mascara
	mvn r7, r7, LSL r6 // Movemos el 1 a la posición adecuada y lo negamos para convertirlo en una mascara
	and r4, r4, r7 // Aplicamos la mascara a nuestra celda

sudoku_candidatos_arm_bucle_fila_columna_si_fila_cero:
	mov r6, r0 // Movemos la direccion inicial para no modificarla
	add r6, r6, r5, LSL #5 // Le sumamos el offset de la fila inicial (offset_fila = fila * 2^5)
	add r6, r6, r2, LSL #1 // Le sumamos el offset de la columna contador (offset_columna = contador_columna * 2^1)
	ldrh r6, [r6] // Cargamos el valor a leer
	lsr r6, #12 // Separamos el numero
	cmp r6, #0 // En caso de que sea cero...
	beq sudoku_candidatos_arm_bucle_fila_columna_si_columna_cero
	sub r6, r6, #1 // Le restamos 1 para que sea indice valido
	mov r7, #1 // Ponemos el 1 de la futura mascara
	mvn r7, r7, LSL r6 // Movemos el 1 a su posicion y lo negamos para convertirlo en una mascara
	and r4, r4, r7 // Aplicamos la mascara a nuestra celda

sudoku_candidatos_arm_bucle_fila_columna_si_columna_cero:
	add r5, r5, #1
	cmp r5, #9
	bne sudoku_candidatos_arm_bucle_fila_columna
	// Recorremos region
	// caalculamos la fila inicial de la region
	mov r5, #6 // Inicialmente asumimos que estamos en la region 2 (columna inicial 6)
	cmp r1, #6 // Si la resta fila - 6 ...
	movmi r5, #3 // Es negativa, estamos en las region 0 o 1, asumimos 1
	cmp r1, #3 // Si la resta fila - 3 ...
	movmi r5, #0 // Es negativa, estamos en la region 0
	mov r1, r5 // Sustituimos la columna guardada porque ya no la necesitamos, y así ahorramos un registro
	// Calculamos la columna inicial de la region, con el mismo algortimo que la fila
	mov r5, #6
	cmp r2, #6
	movmi r5, #3
	cmp r2, #3
	movmi r5, #0
	mov r2, r5
	mov r8, #0 // Inicializamos contador de fila

sudoku_candidatos_arm_tag_fila:

	mov r9, #0 // Inicializamos contador de columna

sudoku_candidatos_arm_tag_columna:

	//

	mov r5, r0 // Movemos a r5 el valor de la direccion inicial de la cuadricula
	add r7, r1, r8 // Almacenamos en r7 la suma del contador de fila y la fila inicial de la region
	add r5, r5, r7, LSL #5 // Le sumamos a la dirección inicial el offset de fila
	add r7, r2, r9 // Almacenamos en r7 la suma del contador de la columna y la columna inicial de la region
	add r5, r5, r7, LSL #1 // Le sumamos a la dirección inicial el offset de columna
	ldrh r5, [r5] // Cargamos el la celda a comprobar
	lsr r5, #12 // Extraemos el numero
	cmp r5, #0 // Si el numero es 0...

	beq sudoku_candidatos_arm_tag_columna_cero // ...lo ignoramos
	sub r5, r5, #1 // Le restamos 1 para que sea indice valido de bits
	mov r6, #1 // Ponemos el 1 de la futura mascara
	lsl r6, r5 // Movemos el 1 a la posición adecuada
	bic r4, r4, r6 // Bic nos permite elimina el bit marcado (la pista dada)


sudoku_candidatos_arm_tag_columna_cero:
	add r9, r9, #1 // Incrementamos el contador de columna
	cmp r9, #3 // Si es 3 fin
	bne sudoku_candidatos_arm_tag_columna

	add r8, r8, #1

	cmp r8, #3

	bne sudoku_candidatos_arm_tag_fila

	strh r4, [r3] // Devolvemos la celda modificada a memoria
sudoku_candidatos_arm_fin:
	mov r0, r4, LSR #12 // Colocamos el valor de la celda como respuesta, si la celda vale 0 esta
				// vacía, y devolvemos falso, si no devolvemos cualquier valor que
				// contenga que se interpretará como cierto

	LDMFD   sp!, {r4-r11,lr} // Recargamos los registros almacenados al inicio
	BX      lr // Retornamos al lugar de invocación
#################################################################################################################
