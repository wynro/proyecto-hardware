.global _start
.text
_start:
# --- Setup interrupt / exception vectors
	    B       Reset_Handler
Undefined_Handler:
	    B       Undefined_Handler
	    B       SWI_Handler
Prefetch_Handler:
	    B       Prefetch_Handler
Abort_Handler:
	    B       Abort_Handler
	    NOP					/* Reserved vector */
IRQ_Handler:
	    B       IRQ_Handler
FIQ_Handler:
	    B       FIQ_Handler

SWI_Handler:
		mov pc, lr

Reset_Handler:

visitmem:
       ldr  r1,=0xf0000000
       ldr  r2,[r1]
#into Abort mode
		MRS R0,CPSR
		MOV R13, #1	
		MOV R14, #2
#into User mode
		MRS R0,CPSR
		BIC R0,R0,#0x1F
		ORR R0,R0,#0x10
		MSR CPSR,R0
		MOV R13, #3	
		MOV R14, #4
stop:
        B stop

.end

