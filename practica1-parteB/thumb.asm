/**
 * Fichero que guarda la funcion sudoku_candidatos_thumb y
 * sudoku_candidatos_thumb_prologo, que analizan una casilla
 * de nuestro sudoku.
 *
 * Autor: Guillermo Robles Gonzalez (604409), Sergio Martin Segura (622612)
 */

.global sudoku_candidatos_thumb_prologo

######################################################################################################
# Funcion prologo de candidatos_thumb, dado que no se conoce forma de llamar a
# una función como thumb desde C.
# Params:
# 1: direccion inicial de la tabla de sudoku
# 2: coordenada fila de la celda a analizar
# 3: coordenada columna de la celda a analizar
sudoku_candidatos_thumb_prologo:
	mov ip, sp
	STMDB   sp!, {r5,fp,ip,lr,pc} // Guardamos los parametros a tocar
	sub fp, ip, #4

	LDR r5, =sudoku_candidatos_thumb
	add r5, r5, #1

	mov lr, pc // Guardamos el lr
	add lr, lr, #4 // Le sumamos para obtener la dirección de vuelta correcta
	bx	r5 // Saltamos a la rutina

	LDMDB   fp, {r5,fp,sp,pc} // Recargamos los registros almacenados al inicio

######################################################################################################
# Función Thumb: Modifica las pistas de la tabla de sudoku pasada en la
# Params:
# 1: direccion inicial de la tabla de sudoku
# 2: coordenada fila de la celda a analizar
# 3: coordenada columna de la celda a analizar
######################################################################################################
.global sudoku_candidatos_thumb
.thumb // Indicacion the set de instrucciones THUMB
sudoku_candidatos_thumb:
	//Prologo, guardamos los registros que usamos
	PUSH {r4-r7} // Almacenamos los registros que podemos modificar
	mov r4, r8
	mov r5, r9
	PUSH {r4-r5}
	mov r9, r0 // Guardamos la direccion inicial de la cuadricula en los registros HI para tener libre el r0 para luego

	mov r3, r9 // Movemos la direccion inicial para poder modificarla
	lsl r1, #5
	add r3, r3, r1 // Le sumamos el offset de fila
	lsr r1, #5
	lsl r2, #1
	add r3, r3, r2// Le sumamos el offset de columna
	lsr r2, #1
	ldrh r4, [r3] // Cargamos la celda a modificar (Guardamos la direccion para poder moverla despues a su sitio)
	mov r5, r4 // Lo movemos
	lsr r5, #12 // Cortamos el numero
	cmp r5, #0 // Si la celda visitada NO es cero, es pista inicial y no hay que tocarla
	bne sudoku_candidatos_thumb_fin // Saltamos al final para no tocar la celda
	// Recordar que, en el algoritmo elegido, primero marcamos todos los numeros como posibles, y luego vamos eliminando
	// posibilidades conforme las encontremos
	// Ponemos todas las pistas a 1 realizando un OR de la casilla con la constante 0x01FF
	mov r4, #15
	mov r5, #16
	lsl r5, #4
	orr r4, r5
	mov r5, #240
	orr r4, r5

	// Recorrido por fila-columna
	# r5: iterador de fila-columna
	mov r5, #0 // Indice de recorrido
sudoku_candidatos_thumb_bucle_fila_columna:
	mov r6, r9 // Movemos la direccion inicial para no modificarla
	lsl r1, #5
	add r6, r6, r1 // Le sumamos el offset de la fila inicial
	lsr r1, #5
	lsl r5, #1
	add r6, r6, r5 // Le sumamos el offset de la columna contador
	lsr r5, #1
	ldrh r6, [r6] // Cargamos el valor a leer
	lsr r6, #12 // Separamos el numero de la celda
	cmp r6, #0 // Si el numero es cero lo ignoramos, no nos vale como pista
	beq sudoku_candidatos_thumb_bucle_fila_columna_si_fila_cero
	sub r6, r6, #1 // Le restamos 1 para que sea indice valido

	# r7: auxiliar. Mascara
	mov r7, #1 // Ponemos el 1 de la futura mascara
	lsl r7, r6 // Lo desplazamos a su lugar
	bic r4, r7 // Aplicamos la mascara a nuestra celda

sudoku_candidatos_thumb_bucle_fila_columna_si_fila_cero:
	mov r6, r9 // Movemos la direccion inicial para no modificarl
	lsl r5, #5
	add r6, r6, r5 // Le sumamos el offset de la fila inicial (offset_fila = fila * 2^5)
	lsr r5, #5
	lsl r2, #1
	add r6, r6, r2 // Le sumamos el offset de la columna contador (offset_columna = contador_columna * 2^1)
	lsr r2, #1
	ldrh r6, [r6] // Cargamos el valor a leer
	lsr r6, #12 // Separamos el numero
	cmp r6, #0 // En caso de que sea cero...
	beq sudoku_candidatos_thumb_bucle_fila_columna_si_columna_cero
	sub r6, r6, #1 // Le restamos 1 para que sea indice valido
	mov r7, #1 // Ponemos el 1 de la futura mascara
	lsl r7, r6 // Lo desplazamos a su lugar
	bic r4, r7

sudoku_candidatos_thumb_bucle_fila_columna_si_columna_cero:
	add r5, r5, #1
	cmp r5, #9
	bne sudoku_candidatos_thumb_bucle_fila_columna
	// Recorremos region
	// caalculamos la fila inicial de la region

	mov r5, #6 // Inicialmente asumimos que estamos en la region 2 (columna inicial 6)
	cmp r1, #6 // Si la resta fila - 6 ...
	bpl sudoku_candidatos_thumb_bucle_fila_columna_region_fila_no_3
	mov r5, #3 // Es negativa, estamos en las region 0 o 1, asumimos 1
sudoku_candidatos_thumb_bucle_fila_columna_region_fila_no_3:
	cmp r1, #3 // Si la resta fila - 3 ...
	bpl sudoku_candidatos_thumb_bucle_fila_columna_region_fila_no_2
	mov r5, #0 // Es negativa, estamos en la region 0
sudoku_candidatos_thumb_bucle_fila_columna_region_fila_no_2:
	mov r1, r5 // Sustituimos la columna guardada porque ya no la necesitamos, y así ahorramos un registro

	// Calculamos la columna inicial de la region, con el mismo algortimo que la fila
	mov r5, #6
	cmp r2, #6
	bpl sudoku_candidatos_thumb_bucle_fila_columna_region_columna_no_3
	mov r5, #3
sudoku_candidatos_thumb_bucle_fila_columna_region_columna_no_3:
	cmp r2, #3
	bpl sudoku_candidatos_thumb_bucle_fila_columna_region_columna_no_2
	mov r5, #0
sudoku_candidatos_thumb_bucle_fila_columna_region_columna_no_2:
	mov r2, r5

	mov r8, r3
	mov r0, #0 // Inicializamos contador de fila

sudoku_candidatos_thumb_tag_fila:

	mov r3, #0 // Inicializamos contador de columna

sudoku_candidatos_thumb_tag_columna:

	//

	mov r5, r9 // Movemos a r5 el valor de la direccion inicial de la cuadricula

	mov r7, r1 // Almacenamos en r7 la suma del contador de fila y la fila inicial de la region
	add r7, r0
	lsl r7, #5
	add r5, r5, r7 // Le sumamos a la dirección inicial el offset de fila
	mov r7, r2 // Almacenamos en r7 la suma del contador de la columna y la columna inicial de la region
	add r7, r3
	lsl r7, #1
	add r5, r5, r7 // Le sumamos a la dirección inicial el offset de columna
	ldrh r5, [r5] // Cargamos el la celda a comprobar
	lsr r5, #12 // Extraemos el numero
	cmp r5, #0 // Si el numero es 0...

	beq sudoku_candidatos_thumb_tag_columna_cero // ...lo ignoramos
	sub r5, r5, #1 // Le restamos 1 para que sea indice valido de bits
	mov r6, #1 // Ponemos el 1 de la futura mascara
	lsl r6, r5 // Movemos el 1 a la posición adecuada
	bic r4, r4, r6 // Bic nos permite elimina el bit marcado (la pista dada)


sudoku_candidatos_thumb_tag_columna_cero:
	add r3, r3, #1 // Incrementamos el contador de columna
	cmp r3, #3 // Si es 3 fin
	bne sudoku_candidatos_thumb_tag_columna

	add r0, r0, #1
	cmp r0, #3
	bne sudoku_candidatos_thumb_tag_fila

	mov r3, r8
	strh r4, [r3] // Devolvemos la celda modificada a memoria
sudoku_candidatos_thumb_fin:
	lsr r4, #12
	mov r0, r4 // Colocamos el valor de la celda como respuesta, si la celda vale 0 esta
				// vacía, y devolvemos falso, si no devolvemos cualquier valor que
				// contenga que se interpretará como cierto

	POP {r4-r5}
	mov r8, r4
	mov r9, r5
	POP {r4-r7} // Recargamos los registros almacenados al inicio
	BX      lr // Retornamos al lugar de invocación, en modo ARM
#################################################################################################################
