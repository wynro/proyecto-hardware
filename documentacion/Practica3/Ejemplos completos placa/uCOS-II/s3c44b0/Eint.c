/*--- include files ---*/
#include "44blib.h"
#include "44b.h"
#include "def.h"
#ifdef __cplusplus
extern "C" {
#endif
void Eint4567Isr(void);

/*********************************************************************************************
* name:		init_Eint
* func:	
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void init_Eint(void)
{
	/* enable interrupt */
    rI_ISPC    = 0x3ffffff;
	rEXTINTPND = 0xf;                          // clear EXTINTPND reg
	rINTMOD    = 0x0;
	rINTCON    = 0x1;
	rINTMSK    = ~(BIT_GLOBAL|BIT_EINT4567);
		
	/* set EINT interrupt handler */
    pISR_EINT4567 = (int)Eint4567Isr;
    
   /* PORT G */
    rPCONG  = 0xffff;                          // EINT7~0
	rPUPG   = 0x0;                             // pull up enable	    
	rEXTINT = rEXTINT|0x22220020;              // EINT1¡¢EINT4567 falling edge mode	
    rI_ISPC    |= BIT_EINT4567;
	rEXTINTPND = 0xf;                          // clear EXTINTPND reg
}
#if 0
/*********************************************************************************************
* name:		Eint4567Isr
* func:	
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void Eint4567Isr(void)
{
    
    Uart_Printf("Key pressed...\n");
    //which_int  = rEXTINTPND;
    rEXTINTPND = 0xf;				//clear EXTINTPND reg.		
    rI_ISPC   |= BIT_EINT4567;		//clear pending_bit
}
#endif

#ifdef __cplusplus
 }
#endif
