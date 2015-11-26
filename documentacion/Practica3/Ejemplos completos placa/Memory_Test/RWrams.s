    
    .global sRWramtest
    .equ RWBase, 0x0c030000
    /* Read and write initialized RAM */
sRWramtest:
	LDR     r2,=RWBase
	LDR     r3,=0x55AA55AA	
	STR     r3,[r2]

	LDR     r3,[r2]            /*//  Read by Word.*/
	ADD     r3,r3,#1
	STR     r3,[r2]            /*//  Write by Word.*/

	LDR     r2,=RWBase 	
	LDRH    r3,[r2]            /*//  Read by half Word.*/	
	ADD     r3,r3,#1
	STRH    r3,[r2],#2         /*//  Write by half Word.*/
	STRH    r3,[r2]
	    
	LDR     r2,=RWBase                       
	LDRB    r3,[r2]	           /*// Read by half Byte.*/
	LDRB    r3,=0xDD
	STRB    r3,[r2],#1         /*// Write by half Byte.*/
	LDRB    r3,=0xBB	
	STRB    r3,[r2],#1	
	LDRB    r3,=0x22	
	STRB    r3,[r2],#1 
	LDRB    r3,=0x11	
	STRB    r3,[r2]
	
    mov	    pc,lr 	/* The LR register may be not valid for the mode changes. */
    