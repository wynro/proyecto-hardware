#include "44b.h"
#include "44blib.h"
#include "timer.h"

volatile int isWdtInt;
void Wdt_Int(void) __attribute__ ((interrupt ("IRQ")));

/****************************************
 *	Watch-dog timer test		*
 ****************************************/
void Test_WDTimer(void)
{
    Uart_Printf("<---- WatchDog Timer Test ---->\n");

    rINTMSK=~(BIT_GLOBAL|BIT_WDT);
    pISR_WDT=(unsigned)Wdt_Int;
    isWdtInt=0;
	
    rWTCON=((MCLK/1000000-1)<<8)|(3<<3)|(1<<2);  //  t_watchdog = 1/66/128, interrupt enable
    rWTDAT=8448/4;
    rWTCNT=8448/4;
    rWTCON=rWTCON|(1<<5);   // 1/40/128,interrupt 

    while(isWdtInt!=10);

    rWTCON=((MCLK/1000000-1)<<8)|(3<<3)|(1);  //   1/66/128, reset enable
    Uart_Printf("\nI will restart after 2 sec!!!\n");
    rWTCNT=8448*2;
    rWTCON=rWTCON|(1<<5);   // 1/40/128,interrupt 
    while(1);
    rINTMSK|=BIT_GLOBAL;
}

void Wdt_Int(void)
{
    rI_ISPC=BIT_WDT;
    Uart_Printf("%d ",++isWdtInt);
}
