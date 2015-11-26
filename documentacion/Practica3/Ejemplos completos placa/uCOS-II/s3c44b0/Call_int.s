#------------------------------------------------------------------------------
#- entry function:	void	user_irq(void)
#- exit function:	void	user_handle(void)
#------------------------------------------------------------------------------
#		.INCLUDE	"include.x"
			.EXTERN  UserIRQ0
            .GLOBAL  UserIRQ

UserIRQ:
            stmdb	sp!, {r0-r11, ip, lr}	/*save r0-r11, ip, lr */
            ldr		r0, =UserIRQ0
            mov		lr, pc
            bx		r0						/* invoke user interrupt handler */
            ldmia	sp!, {r0-r11, ip, lr}	/* resume r0, ip, lr */
			subs	pc, r14, #4				/* return from interruption */
			
