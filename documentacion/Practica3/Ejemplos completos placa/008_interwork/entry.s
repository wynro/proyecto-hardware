/*********************************************************************************************
*File£º		entry.s
*Author£º		Gf.XU	2002.5.15
*Description£º		Cross-debugging example program for ARM and Thumb
*Composing£º		1) entry.s	Assembly language example for ARM and Thumb
*			2) thumb.c	C program of Thumb
*			3) arm.c	C program of ARM
*Function£º		
*Modify:		
*********************************************************************************************/

.equ	count, 20
.global Thumb_function					/* example function of Thumb, defined in thumb.c */

#Promgram entry, ARM assembly
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
	    mov	r0, #count

		mov	r1, #0
		mov	r2, #0
		mov	r3, #0
		mov	r4, #0
		mov	r5, #0
		mov	r6, #0
loop0:
		add	r1, r1, #1
		add	r2, r2, #1
		add	r3, r3, #1
		add	r4, r4, #1
		add	r5, r5, #1
		add	r6, r6, #1

		subs	r0, r0, #1
		bne	loop0

#jump to Thumb        
        ADR     R0, Thumb_Entry+1
        BX      R0

#thumb program entry
.thumb
Thumb_Entry:
		mov	r0, #count

		mov	r1, #0
		mov	r2, #0
		mov	r3, #0
		mov	r4, #0
		mov	r5, #0
		mov	r6, #0
		mov	r7, #0
loop1:
		add	r1, #1
		add	r2, #1
		add	r3, #1
		add	r4, #1
		add	r5, #1
		add	r6, #1
		add	r7, #1

		sub	r0, #1
		bne	loop1
		
		bl 	Thumb_function
		
		b	Thumb_Entry
.end
