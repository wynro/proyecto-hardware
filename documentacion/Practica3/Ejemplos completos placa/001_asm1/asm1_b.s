.equ	x, 45						/* define variable x and evaluate to 45*/
.equ	y, 64						/* define variable y and evaluate to 64*/
.equ	z, 87						/* define variable z and evaluate to 87*/
.equ	stack_top, 0x1000			/* define StackTop 0x1000*/
.global _start

.text

_start:                         	/* program startup symbol*/
        MOV     r0, #x        		/* put the value of x to R0 */
        MOV		r0, r0, lsl #8		/* multiply value of R0 by 2^8 */
        MOV     r1, #y        		/* put value of y to R1 */
        ADD		r2, r0, r1, lsr #1	/* put value of (R1/2+r0) to R2 */
		MOV		sp, #0x1000
		STR		r2, [sp]

        MOV     r0, #z        		/* put the value of z to R0  */
        AND		r0, r0, #0xFF		/* get the low 8-bit of R0 */
        MOV     r1, #y        		/* put value of y to R1 */
        ADD		r2, r0, r1, lsr #1	/* put value of (R1/2+r0) to R2 */
		
		LDR		r0, [sp]			/* put the value of y to R1 */
		MOV		r1, #0x01
		ORR		r0, r0, r1
        MOV     r1, R2        		/* put the value of y to R1 */
        ADD		r2, r0, r1, lsr #1	/* put value of (R1/2+r0) to R2  */

stop:
        B     	stop              	/*program end and get into endless loop */
.end

