/*--- include files ---*/
#include "44blib.h"
#include "44b.h"
#include "def.h"
#ifdef __cplusplus
extern "C" {
#endif

#define KEY_VALUE_MASK	0x0f
/*--- extern function ---*/
void KeyIsr(void) __attribute__ ((interrupt ("IRQ")));
void Eint4567Isr(void) __attribute__ ((interrupt ("IRQ")));
inline int key_read();

unsigned char IntNesting = 0;
unsigned char which_int  = 0;
/* keyboard control address */
volatile U8 *keyboard_base = (U8 *)0x06000000;

/*********************************************************************************************
* name:		init_Ext4567
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
	rINTMSK    = ~(BIT_GLOBAL|BIT_EINT1|BIT_EINT4567);
		
	/* set EINT interrupt handler */
    pISR_EINT4567 = (int)Eint4567Isr;
    pISR_EINT1    = (int)KeyIsr;
    
   /* PORT G */
    rPCONG  = 0xffff;                          // EINT7~0
	rPUPG   = 0x0;                             // pull up enable	    
	rEXTINT = rEXTINT|0x22220020;              // EINT1¡¢EINT4567 falling edge mode	
    rI_ISPC    |= (BIT_EINT1|BIT_EINT4567);
	rEXTINTPND = 0xf;                          // clear EXTINTPND reg
}

/*********************************************************************************************
* name:		Test_Eint
* func:	
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void Test_Eint(void)
{
    unsigned int save_G,save_PG;
 	int value = -1;
    init_Eint();
    which_int=0;
    
    Uart_Printf(" External Interrupt Test for Embest S3CEV40.\n");
    Uart_Printf("\n\nInterrupt Controler Configration concern with Registers:\n");
    Uart_Printf("         rINTCON = 0x%x\n",rINTCON);
    Uart_Printf("         rINTPND = 0x%x\n", rINTPND);   
    Uart_Printf("         rINTMSK = 0x%x\n", rINTPND);
    rINTCON=0x1;   /**** Vectored interrupt test(S3C44B0X). ****/
    Uart_Printf("\n ***** Vectored interrupt test(S3C44B0X). ***** \n");
   
    Uart_Printf("Please Select the trigger:\n"
	"  1> Falling trigger\n"
	"  2> Rising trigger\n"
	"  3> Both Edge trigger\n" 
	"  4> Low level trigger\n"
	"  5> High level trigger\n"
	"\n   any key to exit...\n");
    
    save_G=rPCONG;
    save_PG=rPUPG;
    rPCONG=0xffff;	//EINT7~0
    rPUPG=0x0;		//pull up enable	    

    switch(Uart_Getch())
    {
	case '1':
	    rEXTINT = 0x22222222; // Falling edge mode 
	    break;
	case '2':
	    rEXTINT = 0x44444444; // Rising edge mode
	    break;
	case '3':
	    rEXTINT = 0x77777777; // Both edge mode
	    break;
	case '4':
	    rEXTINT = 0x0;        // "0" level mode
	    break;
	case '5':
	    Uart_Printf("EINT1(connect to Keyboard) was pulled up. \n");//rEXTINT=0x11111111; //"1" level mode
	    //rEXTINT = 0x11111111;  // "1" level mode
	    rINTMSK    |= BIT_EINT4567;
	    while(!which_int);
	    break;
	default:
	    rPCONG=save_G;
	    rPUPG=save_PG;
	    return;
    }

    Uart_Printf("Press the EINT buttons(SB2/SB3)!!!");
    Uart_Printf("(push buttons may have glitch noise problem)\n");
    rINTMSK=~(BIT_GLOBAL|BIT_EINT1|BIT_EINT4567);  

    while(!which_int);

    switch(which_int)
    {
	case 1:
	    Uart_Printf("EINT4 had been occured...\n");
	    break;
	case 2:
	    Uart_Printf("EINT5 had been occured...\n");
	    break;
	case 4:			     // SB2 --- up 0-F
        which_int=0;
        IntNesting=1;    // Allow to Nesting.
	    Uart_Printf("EINT6 had been occured...\n");
        led1_on();
        //D8Led_Direction(0);
        led1_off();
	    break;
	case 8:	             // SB3 --- down F-0
        which_int=0;
        IntNesting=1;    // Allow to Nesting.
	    Uart_Printf("EINT7 had been occured...\n");
        led2_on();
        //D8Led_Direction(1);
        led2_off();
	    break;
	case 9:
	    Uart_Printf("EINT1 had been occured...(Look 8Led)\n");
            value = key_read();
            if(value > -1)
		Digit_Led_Symbol(value);
	default :
	    break;
    }

    IntNesting = 0;
    rINTMSK=BIT_GLOBAL;
    rPCONG=save_G;
    rPUPG=save_PG;
    which_int=0;
    
    Uart_Printf("\n         rINTCON = 0x%x\n",rINTCON);
    Uart_Printf("         rINTPND = 0x%x\n", rINTPND);
    rINTCON=0x5;
}

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
    
    if(IntNesting)
     {
     	IntNesting++;
        Uart_Printf("IntNesting = %d\n",IntNesting);//An Extern Intrrupt had been occur before dealing with one. 
     }
    which_int  = rEXTINTPND;
    rEXTINTPND = 0xf;				//clear EXTINTPND reg.		
    rI_ISPC   |= BIT_EINT4567;		//clear pending_bit
}

/*********************************************************************************************
* name:		
* func:	
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void KeyIsr(void)
{
    which_int  = 9;
    rI_ISPC    = BIT_EINT1;		//clear pending_bit
    Uart_Printf("Key press is:%d\n",(char)key_read());
}
/*********************************************************************************************
* name:		key_read
* func:		read key value
* para:		none
* ret:		key value, -1 -- error
* modify:
* comment:		
********************************************************************************************/
inline int key_read()
{
	int value;
	char temp;
	
	/* read line 1 */
	temp = *(keyboard_base+0xfd);
	/* not 0xF mean key down */
	if(( temp & KEY_VALUE_MASK) != KEY_VALUE_MASK)
	{
		if( (temp&0x1) == 0 )
			value = 3;
		else if( (temp&0x2) == 0 )
			value = 2;
		else if( (temp&0x4) == 0 )
			value = 1;
		else if( (temp&0x8) == 0 )
			value = 0;
		return value;
	}
	
	/* read line 2 */
	temp = *(keyboard_base+0xfb);
	/* not 0xF mean key down */
	if(( temp & KEY_VALUE_MASK) != KEY_VALUE_MASK)
	{
		if( (temp&0x1) == 0 )
			value = 7;
		else if( (temp&0x2) == 0 )
			value = 6;
		else if( (temp&0x4) == 0 )
			value = 5;
		else if( (temp&0x8) == 0 )
			value = 4;
		return value;
	}
	
	/* read line 3 */
	temp = *(keyboard_base+0xf7);
	/* not 0xF mean key down */
	if(( temp & KEY_VALUE_MASK) != KEY_VALUE_MASK)
	{
		if( (temp&0x1) == 0 )
			value = 0xb;
		else if( (temp&0x2) == 0 )
			value = 0xa;
		else if( (temp&0x4) == 0 )
			value = 9;
		else if( (temp&0x8) == 0 )
			value = 8;
		return value;
	}
	
	/* read line 4 */
	temp = *(keyboard_base+0xef);
	/* not 0xF mean key down */
	if(( temp & KEY_VALUE_MASK) != KEY_VALUE_MASK)
	{
		if( (temp&0x1) == 0 )
			value = 0xf;
		else if( (temp&0x2) == 0 )
			value = 0xe;
		else if( (temp&0x4) == 0 )
			value = 0xd;
		else if( (temp&0x8) == 0 )
			value = 0xc;
		return value;
	}
	
	return -1;
}

#ifdef __cplusplus
 }
#endif
