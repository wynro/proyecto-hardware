/* For the fault of GNU GCC for ARM compiler to compile the interrupt function as:
    void user_handle(void) __attribute__ ((interrupt ("IRQ"))); // NOT RECOMMEND
    void user_handle(void); // RECOMMEND
#------------------------------------------------------------------------------
#- Entry function:	void	irqEntry(void)
#- Extern function:	void	isrHandle(void)
#------------------------------------------------------------------------------
#		.INCLUDE	"include.x"
*/
		.macro IRQHandle isrHandle:
		stmdb	sp!, {r0-r11, ip, lr}	/* save r0-r11, ip, lr */
		ldr		r0, =\isrHandle
		mov		lr, pc
		bx		r0						/* jump to user_handle(void) */
		ldmia	sp!, {r0-r11, ip, lr}	/* restore r0, ip, lr */
		subs	pc, r14, #4				/* return from interrupt */
		.endm

/* Examples showed as following:
@ void TSInt (void); ---> The interrupt handle function write as C program, it is user application func.
@ isrEINT2           ---> It is the interrupt vactor entry function to jump here, call by INTERRUPT  VECTOR TABLE.
@ TSP_INT            ---> modify Project ->Settings >Assembler >General category >Predefines: set as
								TSP_INT=1 
						  OR '#define TSP_INT' or '.EQU TSP_INT' in source file (except this file).
       .extern TSInt
       .global isrEINT2	
isrEINT2:
.ifdef TSP_INT
       IRQHandle TSInt
.endif
*/
@--------------------------------------------
       .extern UserIRQ0
       .global UserIRQ
UserIRQ: 
.ifdef USER_INT
       IRQHandle UserIRQ0
.endif

