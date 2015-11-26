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
	    NOP						/* Reserved vector */
IRQ_Handler:
	    B       IRQ_Handler
FIQ_Handler:
	    B       FIQ_Handler

SWI_Handler:
		mov pc, lr

Reset_Handler:

#into System mode
		MRS R0,CPSR
		BIC R0,R0,#0x1F
		ORR R0,R0,#0x1F
		MSR CPSR,R0
		MOV R0, #1	
		MOV R1, #2	
		MOV R2, #3	
		MOV R3, #4	
		MOV R4, #5	
		MOV R5, #6	
		MOV R6, #7	
		MOV R7, #8	
		MOV R8, #9	
		MOV R9, #10	
		MOV R10, #11	
		MOV R11, #12
		MOV R12, #13	
		MOV R13, #14	
		MOV R14, #15

#into FIQ mode
		MRS R0,CPSR
		BIC R0,R0,#0x1F
		ORR R0,R0,#0x11
		MSR CPSR,R0
		MOV R8, #16	
		MOV R9, #17	
		MOV R10, #18	
		MOV R11, #19	
		MOV R12, #20	
		MOV R13, #21	
		MOV R14, #22
#into SVC mode
		MRS R0,CPSR
		BIC R0,R0,#0x1F
		ORR R0,R0,#0x13
		MSR CPSR,R0
		MOV R13, #23	
		MOV R14, #24
#into Abort mode
		MRS R0,CPSR
		BIC R0,R0,#0x1F
		ORR R0,R0,#0x17
		MSR CPSR,R0
		MOV R13, #25	
		MOV R14, #26
#into IRQ mode
		MRS R0,CPSR
		BIC R0,R0,#0x1F
		ORR R0,R0,#0x12
		MSR CPSR,R0
		MOV R13, #27	
		MOV R14, #28
#into UNDEF mode
		MRS R0,CPSR
		BIC R0,R0,#0x1F
		ORR R0,R0,#0x1b
		MSR CPSR,R0
		MOV R13, #29	
		MOV R14, #30

		B	Reset_Handler

.end

