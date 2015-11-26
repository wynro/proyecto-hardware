# *******************************************************
# * NAME    : 44BINIT.S									*
# * Version : 10.April.2000								*
# * Description:										*
# *	C start up codes									*
# *	Configure memory, Initialize ISR ,stacks			*
# *	Initialize C-variables								*
# *	Fill zeros into zero-initialized C-variables		*
# *******************************************************
#program entry£¬arm assembly
#.arm
.global _start
.text
_start:
# --- Setup interrupt / exception vectors
	    B       Reset_Handler
Undefined_Handler:
	    B       Undefined_Handler
SWI_Handler:
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
	    
Reset_Handler:
		LDR	sp, =0x00002000

#------------------------------------------------------------------------------
#- Branch on C code Main function (with interworking)
#----------------------------------------------------
#- Branch must be performed by an interworking call as either an ARM or Thumb 
#- main C function must be supported. This makes the code not position-
#- independant. A Branch with link would generate errors 
#------------------------------------------------------------------------------
                .extern     __main

                ldr         r0, = __main
                mov         lr, pc
                bx          r0
#------------------------------------------------------------------------------
#- Loop for ever
#---------------
#- End of application. Normally, never occur.
#- Could jump on Software Reset ( B 0x0 ).
#------------------------------------------------------------------------------
End:
                b           End

    .end
