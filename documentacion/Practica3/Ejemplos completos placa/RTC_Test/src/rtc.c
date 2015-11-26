#include "..\..\common\44blib.h"
#include "..\..\common\44b.h"
#include "..\..\common\rtc.h"

char *date[8] = {"","SUN","MON","TUE","WED","THU","FRI","SAT"};
volatile unsigned int sec_tick;
volatile int          isRtcInt;
   
int Test_Rtc_Alarm(void)  
{
    int i;
    Rtc_Init();
    rRTCCON = 0x01;   // R/W enable, 1/32768, Normal(merge), No reset
    rALMYEAR =TESTYEAR2 ;
    rALMMON  =TESTMONTH2;
    rALMDAY  =TESTDAY2  ;
    rALMHOUR =TESTHOUR2 ;
    rALMMIN  =TESTMIN2  ;
    rALMSEC  =TESTSEC2+2; 
    rRTCCON = 0x0;    // R/W disable(for power consumption), 1/32768, Normal(merge), No reset
 
    Uart_Printf(" Set Alarm Time at 20%02x-%02x-%02x ",TESTYEAR2,TESTMONTH2,TESTDAY2);
    Uart_Printf("%02x:%02x:%02x\n\r",TESTHOUR2,TESTMIN2,TESTSEC2+2);
    
    isRtcInt=0;
    rRTCALM=0x7f;    
    pISR_RTC=(unsigned int)Rtc_Int;
    rINTMSK=~(BIT_GLOBAL|BIT_RTC);

    DelayMs(1200);    // wait
    
    rINTMSK |= BIT_RTC;    
    return isRtcInt;
        
}

void Test_Rtc_Tick(void)
{
    pISR_TICK=(unsigned)Rtc_Tick;
    rINTMSK=~(BIT_GLOBAL|BIT_TICK); 
        
    sec_tick=1;
    rTICINT = 127+(1<<7);	//START
}

void Rtc_Init(void)
{
    rRTCCON  = 0x01;	// R/W enable, 1/32768, Normal(merge), No reset
    rBCDYEAR = TESTYEAR;
    rBCDMON  = TESTMONTH;
    rBCDDAY  = TESTDAY;	// SUN:1 MON:2 TUE:3 WED:4 THU:5 FRI:6 SAT:7
    rBCDDATE = TESTDATE;
    rBCDHOUR = TESTHOUR;
    rBCDMIN  = TESTMIN;
    rBCDSEC  = TESTSEC;    
    rRTCCON  = 0x0;	// R/W disable, 1/32768, Normal(merge), No reset
    
    Uart_Printf("\n Set Default Time at 20%02x-%02x-%02x %s",TESTYEAR,TESTMONTH,TESTDAY,date[TESTDATE]);
    Uart_Printf(" %02x:%02x:%02x\n\r",TESTHOUR,TESTMIN,TESTSEC);

}

void Read_Rtc(void)
{
    //Uart_Printf("This test should be excuted once RTC test(Alarm) for RTC initialization\n");
    rRTCCON = 0x01;    // R/W enable, 1/32768, Normal(merge), No reset
    while(1)
    {
	if(rBCDYEAR == 0x99) 
	    year = 0x1999;
	else 
	    year = 0x2000 + rBCDYEAR;
	    month=rBCDMON;
	    day=rBCDDAY;
	    weekday=rBCDDATE;
	    hour=rBCDHOUR;
	    min=rBCDMIN;
	    sec=rBCDSEC;
	if(sec!=0)
	    break;
    }	 
    rRTCCON = 0x0;    // R/W disable(for power consumption), 1/32768, Normal(merge), No reset
}

void Display_Rtc(void)
{
    Read_Rtc();
    Uart_Printf(" Current Time is %02x-%02x-%02x %s",year,month,day,date[weekday]);
    Uart_Printf(" %02x:%02x:%02x\r",hour,min,sec);
}

void Rtc_Int(void)
{
    rI_ISPC=BIT_RTC;   
    //-------------//other inst. is needed only when cache=on & wrbuf=on & BSFRD=0
    Uart_Printf(" ... RTC Alarm Interrupt O.K. ...\n");
    isRtcInt = 1;
}

void Rtc_Tick(void)
{
    rI_ISPC=BIT_TICK; 
    Uart_Printf("\b\b\b\b\b\b\b%03d sec",sec_tick++);
}

