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

#into System mode  --- to access All the registers r0 - r14
		MRS R0,CPSR
		BIC R0,R0,#0x1f
		ORR R0,R0,#0x1f
		MSR CPSR,R0
  # write the r0_sys and r14_sys
		MOV R0, #1	          @--- test breakpoint 1
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
	/* SWI should be use when user mode to Special mode*/
        
#into FIQ mode  --- to access the registers r0 - r7 and r8_fiq - r14_fiq
		MRS R0,CPSR
		BIC R0,R0,#0x1F
		ORR R0,R0,#0x11
		MSR CPSR,R0
  # just write the r8_fiq - r14_fiq 
		MOV R8, #16	         @--- test breakpoint 2 
		MOV R9, #17	
		MOV R10, #18	
		MOV R11, #19	
		MOV R12, #20	
		MOV R13, #21	
		MOV R14, #22
#into SVC mode  --- to access the registers r0 - r7 and r8_svc - r14_svc
		MRS R0,CPSR
		BIC R0,R0,#0x1F
		ORR R0,R0,#0x13
		MSR CPSR,R0
  # just write the r13_svc and r14_svc   
		MOV R13, #23         @--- test breakpoint 3
		MOV R14, #24
#into Abort mode  --- to access the registers r0 - r7 and r8_abt - r14_abt
		MRS R0,CPSR
		BIC R0,R0,#0x1F
		ORR R0,R0,#0x17
		MSR CPSR,R0
  # just write the r13_abt and r14_abt   
		MOV R13, #25         @--- test breakpoint 4
		MOV R14, #26
#into IRQ mode  --- to access the registers r0 - r7 and r8_irq - r14_irq
		MRS R0,CPSR
		BIC R0,R0,#0x1F
		ORR R0,R0,#0x12
		MSR CPSR,R0
  # just write the r13_irq and r14_irq   
		MOV R13, #27         @--- test breakpoint 5
		MOV R14, #28
#into UNDEF mode  --- to access the registers r0 - r7 and r8_und - r14_und
		MRS R0,CPSR
		BIC R0,R0,#0x1F
		ORR R0,R0,#0x1b
		MSR CPSR,R0
  # just write the r13_und and r14_und   
		MOV R13, #29         @--- test breakpoint 6
		MOV R14, #30

		
		B	Reset_Handler

.end

