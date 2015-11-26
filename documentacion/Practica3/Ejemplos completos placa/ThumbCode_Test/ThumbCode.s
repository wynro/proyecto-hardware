.global _start
.text
_start:
.arm              			            /*  Subsequent instructions are ARM */
header:
        ADR     r0, start + 1           /*  Processor starts in ARM state, */
        BX      r0                      /*  so small ARM code header used */
                                    	/*  to call Thumb main program. */
        NOP    
.thumb

start:
        MOV     r0, #10                 /*  Set up parameters */
        MOV     r1, #3
        BL      doadd                   /*  Call subroutine */

stop:
                B stop

doadd:
        ADD     r0, r0, r1              /*  Subroutine code */
        MOV     pc, lr                  /*  Return from subroutine. */

.end                                 	/*  Mark end of file */

