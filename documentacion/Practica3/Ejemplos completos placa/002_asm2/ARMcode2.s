.global _start
.text
.equ	num, 2               	/* Number of entries in jump table */

_start:
        MOV     r0, #0  		/*  set up the three parameters */
        MOV     r1, #3
        MOV     r2, #2
        BL      arithfunc       /*  call the function */

stop:
        B		stop

arithfunc:                       /*  label the function */
        CMP     r0, #num        /*  Treat function code as unsigned integer */
        BHS     DoAdd           /*  If code is >=2 then do operation 0. */

        ADR     r3, JumpTable   /*  Load address of jump table */
        LDR     pc, [r3,r0,LSL#2]	/*  Jump to the appropriate routine */

JumpTable:
        .long     DoAdd
        .long     DoSub

DoAdd:
        ADD     r0, r1, r2      /*  Operation 0, >1 */
        MOV     pc, lr  		/*  Return */

DoSub:
        SUB     r0, r1, r2      /*  Operation 1 */
        MOV     pc,lr           /*  Return */

.end							/*  mark the end of this file */
