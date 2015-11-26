.equ	x, 45						/* define variable x and evaluate to 45 */
.equ	y, 64						/* define variable y and evaluate to 64 */
.equ	stack_top, 0x1000			/* define StackTop 0x1000*/
.global _start

.text

_start:                         	/* program startup symbol */
        MOV		sp, #stack_top
        MOV     r0, #x        		/* put the value of x to R0 */
        STR		r0, [sp]			/* save the value of R0 to stack */
        MOV     r0, #y        		/* put the value of y to R0 */
        LDR		r1, [sp]			/* get the value in stack to R1 */
		ADD		r0, r0, r1
		STR		r0, [sp]
stop:
        B     	stop              	/* program end and get into endless loop */
.end
