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
# Parameter in r0
		LDR		r0, =cuadriculaCorrecta // Cargamos los argumentos de la funcion
		ldr 	r5, =sudoku9x9 // Cargamos los parametros de la función
		mov		lr, pc // Guardamos la dirección de retorno
		bx		r5 // Saltamos a la función
stop:
        B       stop        /*  end of program */

.end
#        END
