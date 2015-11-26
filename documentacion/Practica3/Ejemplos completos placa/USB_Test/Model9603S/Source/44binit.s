;****************************************************************************
;����  ��  ��  �ơ�44binit.s
;����  ��  ��  ����C����ʼ���룬�������ô洢���ռ䡢��ʼ���жϺͶ�ջ��
;����  ��  ��  ����2.0
;���޸��˼��޸����ڡ���ϣ��//2003��1��20��20:43
;****************************************************************************
 
    GET include\option.a
    GET include\memcfg.a

;****************************************************************************
;�洢���ռ�
;GCS6 64M 16bit(8MB) DRAM/SDRAM(0xc000000-0xc7fffff)
;APP    RAM=0xc000000~0xc7effff 
;44BMON RAM=0xc7f0000-0xc7fffff
;STACK	   =0xc7ffa00		   

;****************************************************************************
;�жϿ���Ԥ����
INTPND	    EQU	0x01e00004
INTMOD	    EQU	0x01e00008
INTMSK	    EQU	0x01e0000c
I_ISPR	    EQU	0x01e00020
I_CMST	    EQU	0x01e0001c

;****************************************************************************
;���Ź���ʱ��Ԥ����
WTCON	    EQU	0x01d30000

;****************************************************************************
;ϵͳʱ��Ԥ����
PLLCON	    EQU	0x01d80000
CLKCON	    EQU	0x01d80004
LOCKTIME    EQU	0x01d8000c
	
;****************************************************************************
;�洢������Ԥ����
REFRESH	    EQU 0x01c80024

;****************************************************************************
;BDMAĿ�ļĴ���
BDIDES0     EQU 0x1f80008
BDIDES1     EQU 0x1f80028

;****************************************************************************
;Ԥ���峣����������
USERMODE    EQU	0x10
FIQMODE	    EQU	0x11
IRQMODE	    EQU	0x12
SVCMODE	    EQU	0x13
ABORTMODE   EQU	0x17
UNDEFMODE   EQU	0x1b
MODEMASK    EQU	0x1f
NOINT	    EQU	0xc0

;****************************************************************************
;����Ƿ�ʹ��tasm.exe���б���
    GBLL    THUMBCODE
    [ {CONFIG} = 16	
THUMBCODE SETL	{TRUE}
    CODE32
    |   
THUMBCODE SETL	{FALSE}
    ]

    [ THUMBCODE
    CODE32   ;for start-up code for Thumb mode
    ]

    MACRO
$HandlerLabel HANDLER $HandleLabel

;****************************************************************************
$HandlerLabel
    sub	    sp,sp,#4	    ;decrement sp(to store jump address)
    stmfd   sp!,{r0}	    ;PUSH the work register to stack(lr does't push because it return to original address)
    ldr	    r0,=$HandleLabel;load the address of HandleXXX to r0
    ldr	    r0,[r0]	    ;load the contents(service routine start address) of HandleXXX
    str	    r0,[sp,#4]	    ;store the contents(ISR) of HandleXXX to stack
    ldmfd   sp!,{r0,pc}	    ;POP the work register and pc(jump to ISR)
    MEND

    IMPORT	|Image$$RO$$Limit|  ; End of ROM code (=start of ROM data)
    IMPORT	|Image$$RW$$Base|   ; Base of RAM to initialise
    IMPORT	|Image$$ZI$$Base|   ; Base and limit of area
    IMPORT	|Image$$ZI$$Limit|  ; to zero initialise

    IMPORT  Main    ; The main entry of mon program 

    AREA    Init,CODE,READONLY

;****************************************************************************
    ENTRY 		;���������
    b ResetHandler  ;for debug
    b HandlerUndef  ;handlerUndef
    b HandlerSWI    ;SWI interrupt handler
    b HandlerPabort ;handlerPAbort
    b HandlerDabort ;handlerDAbort
    b .		    ;handlerReserved
    b HandlerIRQ
    b HandlerFIQ
	;***IMPORTANT NOTE***
	;If the H/W vectored interrutp mode is enabled, The above two instructions should
	;be changed like below, to work-around with H/W bug of S3C44B0X interrupt controller. 
	; b HandlerIRQ  ->  subs pc,lr,#4
	; b HandlerIRQ  ->  subs pc,lr,#4

;****************************************************************************
VECTOR_BRANCH
    ldr pc,=HandlerEINT0    ;mGA    H/W interrupt vector table
    ldr pc,=HandlerEINT1    ;	
    ldr pc,=HandlerEINT2    ;
    ldr pc,=HandlerEINT3    ;
    ldr pc,=HandlerEINT4567 ;
    ldr pc,=HandlerTICK	    ;mGA
    b .
    b .
    ldr pc,=HandlerZDMA0    ;mGB
    ldr pc,=HandlerZDMA1    ;
    ldr pc,=HandlerBDMA0    ;
    ldr pc,=HandlerBDMA1    ;
    ldr pc,=HandlerWDT	    ;
    ldr pc,=HandlerUERR01   ;mGB
    b .
    b .
    ldr pc,=HandlerTIMER0   ;mGC
    ldr pc,=HandlerTIMER1   ;
    ldr pc,=HandlerTIMER2   ;
    ldr pc,=HandlerTIMER3   ;
    ldr pc,=HandlerTIMER4   ;
    ldr pc,=HandlerTIMER5   ;mGC
    b .
    b .
    ldr pc,=HandlerURXD0    ;mGD
    ldr pc,=HandlerURXD1    ;
    ldr pc,=HandlerIIC	    ;
    ldr pc,=HandlerSIO	    ;
    ldr pc,=HandlerUTXD0    ;
    ldr pc,=HandlerUTXD1    ;mGD
    b .
    b .
    ldr pc,=HandlerRTC	    ;mGKA
    b .						;
    b .						;
    b .						;
    b .						;
    b .						;mGKA
    b .
    b .
    ldr pc,=HandlerADC	    ;mGKB
    b .						;
    b .						;
    b .						;
    b .						;
    b .						;mGKB
    b .
    b .
;0xe0=EnterPWDN
    ldr pc,=EnterPWDN

    LTORG	

;****************************************************************************
HandlerFIQ	HANDLER HandleFIQ
HandlerIRQ	HANDLER HandleIRQ
HandlerUndef	HANDLER HandleUndef
HandlerSWI	HANDLER HandleSWI
HandlerDabort	HANDLER HandleDabort
HandlerPabort	HANDLER HandlePabort

HandlerADC	HANDLER HandleADC
HandlerRTC	HANDLER HandleRTC
HandlerUTXD1	HANDLER HandleUTXD1
HandlerUTXD0	HANDLER HandleUTXD0
HandlerSIO	HANDLER HandleSIO
HandlerIIC	HANDLER HandleIIC
HandlerURXD1	HANDLER HandleURXD1
HandlerURXD0	HANDLER HandleURXD0
HandlerTIMER5	HANDLER HandleTIMER5
HandlerTIMER4	HANDLER HandleTIMER4
HandlerTIMER3	HANDLER HandleTIMER3
HandlerTIMER2	HANDLER HandleTIMER2
HandlerTIMER1	HANDLER HandleTIMER1
HandlerTIMER0	HANDLER HandleTIMER0
HandlerUERR01	HANDLER HandleUERR01
HandlerWDT	HANDLER HandleWDT
HandlerBDMA1	HANDLER HandleBDMA1
HandlerBDMA0	HANDLER HandleBDMA0
HandlerZDMA1	HANDLER HandleZDMA1
HandlerZDMA0	HANDLER HandleZDMA0
HandlerTICK	HANDLER HandleTICK
HandlerEINT4567	HANDLER HandleEINT4567
HandlerEINT3	HANDLER HandleEINT3
HandlerEINT2	HANDLER HandleEINT2
HandlerEINT1	HANDLER HandleEINT1
HandlerEINT0	HANDLER HandleEINT0


;****************************************************************************
;�������������е�һ�����������������ж�
IsrIRQ	;using I_ISPR register.
    sub	    sp,sp,#4       ;reserved for PC
    stmfd   sp!,{r8-r9}   

	;IMPORTANT CAUTION
	;if I_ISPC isn't used properly, I_ISPR can be 0 in this routine.

    ldr	    r9,=I_ISPR
    ldr	    r9,[r9]

	cmp		r9, #0x0	;If the IDLE mode work-around is used,
						;r9 may be 0 sometimes.
	beq		%F2

    mov	    r8,#0x0
0
    movs    r9,r9,lsr #1
    bcs	    %F1
    add	    r8,r8,#4
    b	    %B0

1
    ldr	    r9,=HandleADC
    add	    r9,r9,r8
    ldr	    r9,[r9]
    str	    r9,[sp,#8]
    ldmfd   sp!,{r8-r9,pc}

2
	ldmfd	sp!,{r8-r9}
	add		sp,sp,#4
	subs	pc,lr,#4

;****************************************************************************
;��ʼ������ʼ
ResetHandler
    ldr	    r0,=WTCON	    ;��ֹ���Ź�
    ldr	    r1,=0x0 		
    str	    r1,[r0]

    ldr	    r0,=INTMSK
    ldr	    r1,=0x07ffffff  ;��ֹ�����ж�
    str	    r1,[r0]

	;�趨ʱ�ӿ��ƼĴ���
    ldr	r0,=LOCKTIME
    ldr	r1,=0xfff
    str	r1,[r0]

    [ PLLONSTART
	ldr	r0,=PLLCON			;���໷��Ƶ�趨
    ldr	r1,=((M_DIV<<12)+(P_DIV<<4)+S_DIV)		;�趨ϵͳ��ʱ��Ƶ��
	str	r1,[r0]
    ]

    ldr	    r0,=CLKCON		 
    ldr	    r1,=0x7ff8	    ;���й��ܵ�Ԫ��ʱ��ʹ��
    str	    r1,[r0]

;****************************************************************************
;ΪBDMA�ı�BDMACON�ĸ�λֵ
    ldr     r0,=BDIDES0       
    ldr     r1,=0x40000000   ;BDIDESn reset value should be 0x40000000	 
    str     r1,[r0]

    ldr     r0,=BDIDES1      
    ldr     r1,=0x40000000   ;BDIDESn reset value should be 0x40000000	 
    str     r1,[r0]

;****************************************************************************
;�趨�洢�����ƼĴ���
    ldr	    r0,=SMRDATA
    ldmia   r0,{r1-r13}
    ldr	    r0,=0x01c80000  ;BWSCON Address
    stmia   r0,{r1-r13}

;****************************************************************************
;��ʼ����ջ
    ldr	    sp, =SVCStack
    bl	    InitStacks

;****************************************************************************
;�����жϴ���
    ldr	    r0,=HandleIRQ		;This routine is needed
    ldr	    r1,=IsrIRQ			;if there isn't 'subs pc,lr,#4' at 0x18, 0x1c
    str	    r1,[r0]

;****************************************************************************
;Copy and paste RW data/zero initialized data

    LDR	    r0, =|Image$$RO$$Limit|	; Get pointer to ROM data
    LDR	    r1, =|Image$$RW$$Base|	; and RAM copy
    LDR	    r3, =|Image$$ZI$$Base|	
	;Zero init base => top of initialised data
			
    CMP	    r0, r1	    ; Check that they are different
    BEQ	    %F1
0		
    CMP	    r1, r3	    ; Copy init data
    LDRCC   r2, [r0], #4    ;--> LDRCC r2, [r0] + ADD r0, r0, #4		 
    STRCC   r2, [r1], #4    ;--> STRCC r2, [r1] + ADD r1, r1, #4
    BCC	    %B0
1		
    LDR	    r1, =|Image$$ZI$$Limit| ; Top of zero init segment
    MOV	    r2, #0
2		
    CMP	    r3, r1	    ; Zero init
    STRCC   r2, [r3], #4
    BCC	    %B2

    [ :LNOT:THUMBCODE
	BL	Main	    ;�ӻ�����C���Դ���ռ䣬��Ҫʹ��main()
	B	.						
    ]

    [ THUMBCODE		    ;for start-up code for Thumb mode
	orr     lr,pc,#1
	bx      lr
	CODE16
	bl      Main	    ;�ӻ�����C���Դ���ռ䣬��Ҫʹ��main()
	b       .
	CODE32
    ]

;****************************************************
;������������ʼ����ջ
;****************************************************
InitStacks
	;Don't use DRAM,such as stmfd,ldmfd......
	;SVCstack is initialized before
	;Under toolkit ver 2.50, 'msr cpsr,r1' can be used instead of 'msr cpsr_cxsf,r1'

    mrs	    r0,cpsr
    bic	    r0,r0,#MODEMASK
    orr	    r1,r0,#UNDEFMODE|NOINT
    msr	    cpsr_cxsf,r1		;UndefMode
    ldr	    sp,=UndefStack
	
    orr	    r1,r0,#ABORTMODE|NOINT
    msr	    cpsr_cxsf,r1 	    	;AbortMode
    ldr	    sp,=AbortStack

    orr	    r1,r0,#IRQMODE|NOINT
    msr	    cpsr_cxsf,r1 	    	;IRQMode
    ldr	    sp,=IRQStack
	
    orr	    r1,r0,#FIQMODE|NOINT
    msr	    cpsr_cxsf,r1 	    	;FIQMode
    ldr	    sp,=FIQStack

    bic	    r0,r0,#MODEMASK|NOINT
    orr	    r1,r0,#SVCMODE
    msr	    cpsr_cxsf,r1 	    	;SVCMode
    ldr	    sp,=SVCStack

	;USER mode is not initialized.
    mov	    pc,lr ;The LR register may be not valid for the mode changes.

;****************************************************
;�����������������ģʽ
;****************************************************
;void EnterPWDN(int CLKCON);
EnterPWDN
    mov	    r2,r0               ;r0=CLKCON
    ldr	    r0,=REFRESH		
    ldr	    r3,[r0]
    mov	    r1, r3
    orr	    r1, r1, #0x400000   ;self-refresh enable
    str	    r1, [r0]

    nop     ;Wait until self-refresh is issued. May not be needed.
    nop     ;If the other bus master holds the bus, ...
    nop	    ; mov r0, r0
    nop
    nop
    nop
    nop

;enter POWERDN mode
    ldr	    r0,=CLKCON
    str	    r2,[r0]

;wait until enter SL_IDLE,STOP mode and until wake-up
    ldr	    r0,=0x10
0   subs    r0,r0,#1
    bne	    %B0

;exit from DRAM/SDRAM self refresh mode.
    ldr	    r0,=REFRESH
    str	    r3,[r0]
    mov	    pc,lr

    LTORG

SMRDATA DATA
;*****************************************************************
;�洢��������ó����ŵ�����,����Ĳ����������Ż���
;*****************************************************************

;*** memory access cycle parameter strategy ***
; 1) Even FP-DRAM, EDO setting has more late fetch point by half-clock
; 2) The memory settings,here, are made the safe parameters even at 66Mhz.
; 3) FP-DRAM Parameters:tRCD=3 for tRAC, tcas=2 for pad delay, tcp=2 for bus load.
; 4) DRAM refresh rate is for 40Mhz. 

;bank0	16bit BOOT ROM
;bank1	8bit NandFlash
;bank2	16bit IDE
;bank3	8bit UDB
;bank4	rtl8019
;bank5	ext
;bank6	16bit SDRAM
;bank7	16bit SDRAM
    [ BUSWIDTH=16
	   DCD 0x11110001	;Bank0=16bit BootRom(AT29C010A*2) :0x0
    | ;BUSWIDTH=32
	DCD 0x22222220	;Bank0=OM[1:0], Bank1~Bank7=32bit
    ]
	DCD ((B0_Tacs<<13)+(B0_Tcos<<11)+(B0_Tacc<<8)+(B0_Tcoh<<6)+(B0_Tah<<4)+(B0_Tacp<<2)+(B0_PMC))	;GCS0
	DCD ((B1_Tacs<<13)+(B1_Tcos<<11)+(B1_Tacc<<8)+(B1_Tcoh<<6)+(B1_Tah<<4)+(B1_Tacp<<2)+(B1_PMC))	;GCS1 
	DCD ((B2_Tacs<<13)+(B2_Tcos<<11)+(B2_Tacc<<8)+(B2_Tcoh<<6)+(B2_Tah<<4)+(B2_Tacp<<2)+(B2_PMC))	;GCS2
	DCD ((B3_Tacs<<13)+(B3_Tcos<<11)+(B3_Tacc<<8)+(B3_Tcoh<<6)+(B3_Tah<<4)+(B3_Tacp<<2)+(B3_PMC))	;GCS3
	DCD ((B4_Tacs<<13)+(B4_Tcos<<11)+(B4_Tacc<<8)+(B4_Tcoh<<6)+(B4_Tah<<4)+(B4_Tacp<<2)+(B4_PMC))	;GCS4
	DCD ((B5_Tacs<<13)+(B5_Tcos<<11)+(B5_Tacc<<8)+(B5_Tcoh<<6)+(B5_Tah<<4)+(B5_Tacp<<2)+(B5_PMC))	;GCS5
	[ BDRAMTYPE="DRAM" 
	    DCD ((B6_MT<<15)+(B6_Trcd<<4)+(B6_Tcas<<3)+(B6_Tcp<<2)+(B6_CAN))	;GCS6 check the MT value in parameter.a
	    DCD ((B7_MT<<15)+(B7_Trcd<<4)+(B7_Tcas<<3)+(B7_Tcp<<2)+(B7_CAN))	;GCS7
	| ;"SDRAM"
		DCD ((B6_MT<<15)+(B6_Trcd<<2)+(B6_SCAN))	;GCS6
		DCD ((B7_MT<<15)+(B7_Trcd<<2)+(B7_SCAN))	;GCS7
	]
	DCD ((REFEN<<23)+(TREFMD<<22)+(Trp<<20)+(Trc<<18)+(Tchr<<16)+REFCNT)	;REFRESH RFEN=1, TREFMD=0, trp=3clk, trc=5clk, tchr=3clk,count=1019
	DCD 0x10			;SCLK power down mode, BANKSIZE 32M/32M
	DCD 0x20			;MRSR6 CL=2clk
	DCD 0x20			;MRSR7

	ALIGN


	AREA RamData, DATA, READWRITE

	^	(_ISR_STARTADDRESS-0x500)
				
UserStack	#	256	;c1(c7)ffa00
SVCStack	#	256	;c1(c7)ffb00
UndefStack	#	256	;c1(c7)ffc00
AbortStack	#	256	;c1(c7)ffd00
IRQStack	#	256	;c1(c7)ffe00
FIQStack	#	0	;c1(c7)fff00


		^	_ISR_STARTADDRESS
HandleReset	#	4
HandleUndef	#	4
HandleSWI	#	4
HandlePabort	#	4
HandleDabort	#	4
HandleReserved	#	4
HandleIRQ	#	4
HandleFIQ	#	4

;Don't use the label 'IntVectorTable',
;because armasm.exe cann't recognize this label correctly.
;the value is different with an address you think it may be.
;IntVectorTable
HandleADC	#	4
HandleRTC	#	4
HandleUTXD1	#	4
HandleUTXD0	#	4
HandleSIO	#	4
HandleIIC	#	4
HandleURXD1	#	4
HandleURXD0	#	4
HandleTIMER5	#	4
HandleTIMER4	#	4
HandleTIMER3	#	4
HandleTIMER2	#	4
HandleTIMER1	#	4
HandleTIMER0	#	4
HandleUERR01	#	4
HandleWDT	#	4
HandleBDMA1	#	4
HandleBDMA0	#	4
HandleZDMA1	#	4
HandleZDMA0	#	4
HandleTICK	#	4
HandleEINT4567	#	4
HandleEINT3	#	4
HandleEINT2	#	4
HandleEINT1	#	4
HandleEINT0	#	4   ;0xc1(c7)fff84

		END
;****************************************************************************
