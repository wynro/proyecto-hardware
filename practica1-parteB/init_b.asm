.text
#        ENTRY                  /*  mark the first instruction to call */
.global start
start:
.arm /*indicates that we are using the ARM instruction set */
#------standard initial code
# --- Setup interrupt / exception vectors
      B       Reset_Handler // Jump to the handler of the reset event (In our case the sudoku program)
/* In this version we do not use the following handlers */
#######################################################################################################
#-----------Undefined_Handler:
#      B       Undefined_Handler
#----------SWI_Handler:
#      B       SWI_Handler
#----------Prefetch_Handler:
#      B       Prefetch_Handler
#----------Abort_Handler:
#      B       Abort_Handler
#         NOP      /* Reserved vector */
#----------IRQ_Handler:
#      B       IRQ_Handler
#----------FIQ_Handler:
#      B       FIQ_Handler
#######################################################################################################
# Reset Handler:
# the processor starts executing this code after system reset
#######################################################################################################
Reset_Handler:
#
        MOV     sp, #0x4000      /*  set up stack pointer (r13) */
#
#  USING A .C FUNCTION
#
# FUNCTION CALL the parameters are stored in r0 and r1
# If there are 4 or less parameters when calling a C function the compiler assumes that they have
# been stored in r0-r3. If there are more parameters you have to store them in the data stack using the stack pointer
# function __c_copy is in copy.c
#        LDR     r0, =cuadricula  /*  puntero a la cuadricula */
#
#.extern     sudoku9x9
#        ldr         r5, = sudoku9x9 // Load the function direction
#        mov         lr, pc // Save the direction to return 
#        bx          r5 // Jump to the function


# USING A ARM FUNCTION
# Parameters in r0-r1
		LDR		r0, =cuadricula // Cargamos llos argumentos de la funcion
		ldr 	r5, =sudoku_recalcular_arm // Cargamos los parametros de la función
		mov		lr, pc // Guardamos la dirección de retorno
		bx		r5 // Saltamos a la función
stop:
        B       stop        /*  end of program */

#######################################################################################################
# Función ARM: Modifica las pistas de la tabla de sudoku pasada
# Params:
# r0: direccion inicial de la tabla de sudoku
#######################################################################################################
.arm
sudoku_recalcular_arm:
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
	bl	sudoku_candidatos_arm // Saltamos a la rutina

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
	B       lr // Retornamos al lugar de invocación, en modo ARM


#######################################################################################################
# Función ARM: Dada una cuadricula y una celda, busca en la fila, en la columna y en la region de dicha
# celda y ajusta las pistas de la celda en consonancia
# Params:
# r0: direccion inicial de la tabla de sudoku
# r1: coordenada fila
# r2: coordenada columna
#######################################################################################################
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


	
	// Incrementamos el contador


	mov r9, #0 // Inicializamos contador de fila

tag_fila:

	mov r10, #0 // Inicializamos contador de columna

tag_columna:

	// 

	mov r5, r0
 // Movemos a r5 el valor de la direccion inicial de la cuadricula
	add r7, r1, r9 // Almacenamos en r7 la suma del contador de fila y la fila inicial de la region
	add r5, r5, r7, LSL #5 // Le susmamos a la dirección inicial el offset de fila
	add r7, r2, r10 // Almacenamos en r7 lasuma del contador de la columna y la columna inicial de la region
	add r5, r5, r7, LSL #1 // Le sumamos a la dirección inicial el offset de columna
	ldrh r5, [r5] // Cargamos el la celda a comprobar
	lsr r5, #12 // Extraemos el numero
	cmp r5, #0 // Si el numero es 0...
	beq tag_columna_cero // ...lo ignoramos
	sub r5, r5, #1 // Le restamos 1 para que sea indice valido de bits
	mov r6, #1 // Ponemos el 1 de la futura mascara
	lsl r6, r5 // Movemos el 1 a la posición adecuada
	bic r4, r4, r6 // Bic nos permite elimina el bit marcado (la pista dada)
	

tag_columna_cero:
	add r10, r10, #1
	cmp r10, #3
	bne tag_columna

	add r9, r9, #1

	cmp r9, #3

	bne tag_fila

	strh r4, [r3] // Devolvemos la celda modificada a memoria
sudoku_candidatos_arm_fin:
	mov r0, r4, LSR #12 // Colocamos el valor de la celda como respuesta, si la celda vale 0 esta
				// vacía, y devolvemos falso, si no devolvemos cualquier valor que
				// contenga que se interpretará como cierto
	
	LDMFD   sp!, {r4-r11,lr} // Recargamos los registros almacenados al inicio
	B       lr // Retornamos al lugar de invocación
#################################################################################################################


#######################################################################################################
# Función Thumb: Modifica las pistas de la tabla de sudoku pasada
# Params:
# 1: direccion inicial de la tabla de sudoku
#######################################################################################################
//.thumb // Indicacion the set de instrucciones THUMB
//sudoku_candidatos_thumb:
//	STMFD   sp!, {r4-r7,lr} // Almacenamos los registros que podemos modificar
//
//	LDMFD   sp!, {r4-r7,lr} // Recargamos los registros almacenados al inicio
//	B       lr // Retornamos al lugar de invocación, en modo ARM
#################################################################################################################




.data
.ltorg /*guarantees the alignment*/
.align 5

# huecos para cuadrar
cuadricula:
     /* 9 filas de 16 entradas para facilitar la visualizacion y 16 bits por celda */
    .hword   0x9800,0x6800,0x0000,0x0000,0x0000,0x0000,0x7800,0x0000,0x8800,0,0,0,0,0,0,0
    .hword   0x8800,0x0000,0x0000,0x0000,0x0000,0x4800,0x3800,0x0000,0x0000,0,0,0,0,0,0,0
    .hword   0x1800,0x0000,0x0000,0x5800,0x0000,0x0000,0x0000,0x0000,0x0000,0,0,0,0,0,0,0
    .hword   0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x1800,0x7800,0x6800,0,0,0,0,0,0,0
    .hword   0x2800,0x0000,0x0000,0x0000,0x9800,0x3800,0x0000,0x0000,0x5800,0,0,0,0,0,0,0
    .hword   0x7800,0x0000,0x8800,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0,0,0,0,0,0,0
    .hword   0x0000,0x0000,0x7800,0x0000,0x3800,0x2800,0x0000,0x4800,0x0000,0,0,0,0,0,0,0
    .hword   0x3800,0x8800,0x2800,0x1800,0x0000,0x5800,0x6800,0x0000,0x0000,0,0,0,0,0,0,0
    .hword   0x0000,0x4800,0x1800,0x0000,0x0000,0x9800,0x5800,0x2800,0x0000,0,0,0,0,0,0,0

.end
#        END
