#include  <math.h>
#include "..\common\44b.h"
#include "..\common\44blib.h"
#include "..\common\def.h"
#include "..\common\power.h"
#include "..\common\rtc.h"
#include "..\common\LCDlib.h"

#define FIN 8000000
#define CLKVAL_MONO 		(13)  //60Mhz, CLKVAL=19 ->78.6Hz

void idlePort(void);
void stopPort(void);
void SLIdleMode(void); //320x240
void Test_SLIdleMode20(void);
void LcdInit_4Gray320x240(void);
void Display_4Gray320x240(void);
void Key_EINT1(void);
char INT_set();

void ChangeMemCon(unsigned *pMemCfg){;}

/* ----------------------------------------------------------------------- */
/********************** *   Slow mode test   * **********************/
char slw_exit=0;
void Test_SlowMode(void)
{
    int i;
    unsigned int save_MCON[9];
    unsigned int MemCon[9]={0,0,0,0,0,0,0x18000,0x18000,0x800000+2033};
    
    pISR_EINT4567=(U32)SLWEINT4567Int;
    rINTMSK =~(BIT_GLOBAL|BIT_RTC|BIT_EINT4567);
    
    for(i=0;i<9;i++)
	save_MCON[i]=((unsigned int *)0x01c80004)[i];//  *(unsigned int *)(0x01c80004+4*i);
    	
    Uart_Printf("\n<---- POWER DOWN MODE TEST ---->\n");
    Uart_Printf("Entering SLOW mode.\n");
    Uart_Printf("  But, The frequency,in 1Mhz, is about %dms.\n",MCLK*20/1000000);
    Uart_Printf("  Press SB2 or SB3 to exit SLOW mode\n");
    Uart_TxEmpty(0);          //To avoid being crushed the character

    rLOCKTIME=0x640;	      //count=t_lock*Fin=800 (t_lock=200us, Fin=8MHz)
    rCLKSLOW=2|(1<<4)|(1<<5); //PLL off,SLOW mode,SLVAL=2 Fout=Fin/(2*SLVAL)=1MHz
    ChangeMemCon(MemCon);     //change memory control register(60MHz-->1MHz)

    while(!slw_exit)          //now!  slow mode:1Mhz, PLL off
     {
	   Delay(100);
	   Delay(100);
     }

    rINTMSK |=BIT_RTC;
    for(i=0;i<9;i++)
	MemCon[i]=save_MCON[i];

    ChangeMemCon(MemCon);     //change memory control register(1MHz-->66MHz)
    rCLKSLOW=2|(1<<4)|(0<<5); //PLL on

    for(i=0;i<2048;i++);	  //wait during PLL lock-time
    rCLKSLOW=2; 		      //exit SLOW mode(disable slow_bit)
    slw_exit=0;
}
/* ----------------------------------------------------------------------- */
/********************** *   Stop mode test   * **********************/
char wake_int=0;
void Test_StopMode(void)
{
    unsigned int i, j, portStatus[20];
    unsigned int *memdata,*temp;
    
    memdata = (unsigned int *)malloc(0x100000);
    temp=memdata;

    for(i=0;i<0x40000;i++)
	*(temp+i)=i;

    rLOCKTIME=0xfff;

    for(i=0;i<20;i++)
	portStatus[i]=*( (volatile U32 *)(0x1d20000+(i<<2)) );

    Uart_Printf("\n<---- STOP Mode Test ---->\n");
    Uart_Printf("After 5 seconds, S3C44B0X will wake up");
    Uart_Printf("by RTC alarm interrupt.\n");
    Uart_Printf("S3C44B0X will also wake up by keyboard.\n\n");
    Uart_TxEmpty(0);	//Wait until UART0 Tx buffer empty.
    
    Rtc_Init();
    rALMYEAR=TESTYEAR2 ;
    rALMMON =TESTMONTH2;
    rALMDAY =TESTDAY2  ;
    rALMHOUR=TESTHOUR2 ;
    rALMMIN =TESTMIN2  ;
    rALMSEC =TESTSEC2+4; 	    
    rRTCALM=0x7f;   //Start RTC alarm

    pISR_RTC=(U32)STAlarmInt;
    pISR_EINT1=(U32)Key_EINT1;    
    pISR_EINT4567=(U32)STEINT4567Int;    
    rINTMSK=BIT_GLOBAL;//Before entering Stop mode interrupts must be masked!!

    stopPort();
    EnterPWDN(0x1);//Entering Power down mode.
    rCLKCON=0x7ff8; //Thaw mode -> normal mode.
    
    rINTMSK=~(BIT_GLOBAL|BIT_RTC|BIT_EINT4567|BIT_EINT1);

    for(i=0;i<20;i++)// Retrun original port configurations 
        *( (volatile U32 *)(0x1d20000+(i<<2)) )=portStatus[i];

    rRTCCON = 0x01; //R/W enable to write rRTCALM
    rRTCALM = 0x0;  //Stop RTC alram
    rRTCCON = 0x0;  //R/W disable :Stop RTC
    rINTMSK =BIT_GLOBAL;//diable all interrupt.

    switch(wake_int)
    {
	case 1:
	    Uart_Printf("S3C44B0X is waked by Alarm\n");
	    break;
	case 2:
	    Uart_Printf("S3C44B0X is waked by EINT4567\n");
	    break;
	default :
	    Uart_Printf("Check int!!!\n");
	    break;
    }
    
    Uart_Printf("Return to Normal Mode.\n");   
    Uart_Printf("Self-refresh data verifing...\n");
    for(i=0;i<0x40000;i++)
    {
	j=*(temp+i);
	if(j!=i)
	{
	    Uart_Printf("Memory test fail after Self-refresh\n"); 
	    break;
	}
    }
    if(j+1==i)
	Uart_Printf("Stop mode and self-refresh test O.K.\n");

    free(memdata);
    wake_int=0;

}
/* ----------------------------------------------------------------------- */
/********************** *   IDLE mode test   * **********************/
void Test_IdleMode(void)
{
    int i;
    int extintMode;
    unsigned int portStatus[20];

// Save the port configurations
    for(i=0;i<20;i++)
	portStatus[i]=*( (volatile U32 *)(0x1d20000+(i<<2)) );

    Uart_Printf("\n<---- IDLE Mode Test ---->\n");
    Uart_Printf("Check the current consumption. Push the buttons to exit IDLE mode.\n");
    Uart_Printf("After 10 seconds, S3C44B0X will wake up by RTC alarm interrupt.\n");
    Uart_TxEmpty(0);    //Wait until UART0 Tx buffer empty.
    INT_set();
    
    Rtc_Init();
 
    rRTCCON  = 0x01;    // R/W enable, 1/32768, Normal(merge), No reset
    rALMYEAR = TESTYEAR2 ;
    rALMMON  = TESTMONTH2;
    rALMDAY  = TESTDAY2  ;
    rALMHOUR = TESTHOUR2 ;
    rALMMIN  = TESTMIN2  ;
    rALMSEC  = TESTSEC2+9;
    rRTCCON  = 0x0;  //R/W disable :Stop RTC
    rRTCALM  = 0x7f;
    rRTCALM  = 0x0;
    rI_ISPC  = BIT_EINT4567|BIT_RTC;	//to clear the previous pending status.
//    Uart_Printf("rINTPND=%x\n",rINTPND);
    pISR_RTC   = (U32)AlarmInt;
    pISR_EINT1 = (U32)Key_EINT1;    
    pISR_EINT4567 = (U32)EINT4567Int;
    rINTMSK=~(BIT_GLOBAL|BIT_EINT4567|BIT_RTC|BIT_EINT1);
    
    for(i=0;i<5;i++);	//wait until the pended interrupt is executed.
    idlePort();
    rCLKCON=0x7ff8|0x4;  //enter IDLE mode.
    for(i=0;i<20;i++);   //wait until S3C44B0X enters IDLE mode.
                         // wait EINT[7:0] interrupt or alarm wake-up

    rCLKCON=0x7ff8;      //turn-off IDLE bit on rCLKCON to synchronize rCLKCON with the real mode.
    for(i=0;i<20;i++)    // Retrun original port configurations 
        *( (volatile U32 *)(0x1d20000+(i<<2)) )=portStatus[i];

    Uart_Printf("Return to Normal Mode.\n");
    rINTMSK=BIT_GLOBAL;
}
#if 0
void Test_IdleMode(void)
{
    unsigned int portStatus[20];
    int i;
    int extintMode;

// Save the port configurations
    for(i=0;i<20;i++)
	portStatus[i]=*( (volatile U32 *)(0x1d20000+(i<<2)) );

    Uart_Printf("[IDLE Mode Test]\n");
    Uart_Printf("Check the current cunsumption. Push the buttons to exit IDLE mode.\n");
    Uart_Printf("After 10 seconds, KS32C41100 will wake up by RTC alarm interrupt.\n");
    Uart_Printf("KS32C41100 will also wake up by keyboard.\n");
    Uart_Printf("Select the external interrupt type. Press the number!!!\n");
    extintMode=Uart_Getch();

    Uart_TxEmpty(0);    //Wait until UART0 Tx buffer empty.

    Rtc_Init();
 
    rRTCCON = 0x01;    // R/W enable, 1/32768, Normal(merge), No reset

    rALMYEAR=TESTYEAR2 ;
    rALMMON =TESTMONTH2;
    rALMDAY =TESTDAY2  ;
    rALMHOUR=TESTHOUR2 ;
    rALMMIN =TESTMIN2  ;
    rALMSEC =TESTSEC2+9;

    //rRTCALM=0x7f;
    rRTCALM=0x0;
    rI_ISPC=BIT_EINT1|BIT_RTC;	//to clear the previous pending status.
//    Uart_Printf("rINTPND=%x\n",rINTPND);
     pISR_EINT1=(U32)Key_EINT1;
    pISR_RTC=(U32)AlarmInt;
    rINTMSK=~(BIT_GLOBAL|BIT_EINT1|BIT_RTC);
    for(i=0;i<2;i++);	//wait until the pended interrupt is executed.

    idlePort();
    rCLKCON=0x7ff8|0x4;  //enter IDLE mode.
    for(i=0;i<10;i++);   //wait until KS32C41100 enters IDLE mode.
    // wait EINT[7:0] interrupt or alarm wake-up

    rCLKCON=0x7ff8;      
    //turn-off IDLE bit on rCLKCON to synchronize rCLKCON with the real mode.

    for(i=0;i<20;i++)// Retrun original port configurations 
        *( (volatile U32 *)(0x1d20000+(i<<2)) )=portStatus[i];

    Uart_Printf("Return to Normal Mode.\n");

    rINTMSK=BIT_GLOBAL;
}
#endif

/********************* * SL_IDLE mode test * *********************/
#define MVAL_USED	(0)
#define MVAL 		(13)
#define L248 		(8)
#define CLKVAL_SL	(0x38)	// 60Mhz, fr=100Hz (CLKVAL=38.6)
#define M5D(n) ((n) & 0x1fffff)

int debug,t0cnt,t1cnt;
unsigned int (*frameBuffer1)[SCR_XSIZE/32];
unsigned int (*_frameBuffer4)[SCR_XSIZE/16];

void Test_SLIdleMode20(void)
{
    int i;
    LcdInit_4Gray320x240();
    Display_4Gray320x240();
    Delay(1000); //wait more than 1 frame time.     
    for(i=0;i<20;i++)SLIdleMode();
}
void Test_SLIdleMode(void)
{
    LcdInit_4Gray320x240();
    Display_4Gray320x240();
    Delay(1000); //wait more than 1 frame time.     
    SLIdleMode();
}
void SLIdleMode(void) //320x240
{
    int i;
    int saveDramcon;
    debug=0;
    Uart_Printf("\n<---- SL_IDLE MODE TEST for 320x240 ---->\n");
    Uart_Printf("After 10 seconds, S3C44B0X will wake up by RTC alarm interrupt.\n");
    Uart_Printf("S3C44B0X can wake up by keyboard\n");
    Uart_TxEmpty(0);

    Rtc_Init();
    rI_ISPC=BIT_EINT4567|BIT_RTC;	//to clear the previous pending status.
    pISR_RTC=(U32)STAlarmInt;
    pISR_EINT1=(U32)Key_EINT1;
    rINTMSK=~(BIT_GLOBAL|BIT_EINT1);
    rRTCCON = 0x01;    // R/W enable, 1/32768, Normal(merge), No reset

    rALMYEAR=TESTYEAR2 ;
    rALMMON =TESTMONTH2;
    rALMDAY =TESTDAY2  ;
    rALMHOUR=TESTHOUR2 ;
    rALMMIN =TESTMIN2  ;
    rALMSEC =TESTSEC2+9; 
 
    rRTCALM=0x7f;   //To test alarm wake-up.
    rADCCON|=0x20;  //ADC power down mode

    while((rLCDCON1>>22)!=9);   
    while((rLCDCON1>>22)!=8);   
    // To enter self-refresh mode, the VCLK has to be 'L' after 4n-th line is displayed completely,
    // So,the self-refresh command has to be issued at 4n th line.
    // VCLK will be 'L' from 4n+1th line.
    //for(i=0;i<10;i++); 
    rLCDCON3=0x1;  //Enter self-refresh mode.
        
    while((rLCDCON1>>22)!=0); 
    //Because the SLOW mode is used, the LCDCON1,2 should be changed.
    rLCDCON1=(0)|(1<<5)|(MVAL_USED<<7)|(0x0<<8)|(0x0<<10)|(1/*CLKVAL*/<<12);
   // disable,4B_SNGL_SCAN,WDLY=2clk,WLH=2clk
    rLCDCON2=(239+L248)|(15/*39*/<<10)|(1<<21);  //LINEBLANK=1
    rCLKSLOW=2|(1<<4)|(1<<5); //SLOWVAL=2,Fout=Fin/(2x2),PLL off. 
    //DRAM refresh may not be done because of slow MCLK. but,the abnormal period is very short.
    rLCDCON1=(1)|(1<<5)|(MVAL_USED<<7)|(0x0<<8)|(0x0<<10)|(1<<12);

    saveDramcon=rREFRESH; 
    rREFRESH=(2017)|(0<<22)|(1<<23); 
    //15,6us@2Mhz, tchr,trc,trp=min, refresh enable. 
    // DRAM refresh may not be done every 15.6uS 
    // because of slow MCLK(1Mhz) and long memory access cycle. 
           
    EnterPWDN(0x46);  //enters SL_IDLE mode. rCLKCON=0x46   
                      //DRAM/SDRAM self-refresh is executed in EnterPWDN()
    //Any special registers setting will not accepted because CPU is not normal mode.
    
    rCLKCON=0x7ff8;  
    rCLKSLOW=2|(1<<4)|(0<<5); //SLOWVAL=2,Fout=Fin/(2x2),PLL on.   
    for(i=0;i<2048;i++);      //Wait PLL lock time.
    
    while((rLCDCON1>>22)!=1); 
    while((rLCDCON1>>22)!=0);     
    //Because the SLOW mode is exited, the LCDCON1,2 should be changed.
    rLCDCON1=(0)|(1<<5)|(MVAL_USED<<7)|(0x3<<8)|(0x3<<10)|(CLKVAL_MONO<<12);
	    // disable,4B_SNGL_SCAN,WDLY=8clk,WLH=8clk,
    rLCDCON2=(239+L248)|(39<<10)|(10<<21); //HOZVAL=39,LINEBLANK=1
    rCLKSLOW=2;      //SLOWVAL=2,Fout=Fpllo,PLL on.   
    rLCDCON3=0x0; //LCD self refresh mode off.
    rLCDCON1=(1)|(1<<5)|(MVAL_USED<<7)|(0x3<<8)|(0x3<<10)|(CLKVAL_MONO<<12);
    rREFRESH=saveDramcon;    

    while((rLCDCON1>>22)!=1); 
    while((rLCDCON1>>22)!=0); //Display 1 more frame before exiting SL_IDLE mode. 
 
    rADCCON&=~(0x20);       
    Uart_Printf("debug=%d\n",debug);
    Uart_Printf("I have exited LCD SELFREF mode.\n");
}
/* ----------------------------------------------------------------------- */
/********************** *   IDLE mode test2   * **********************/
void Test_IdleModeHard(void)
{   
    int i,j;
    
    Uart_Printf("\n<---- IDLE Mode Test with Timer0,1 10000times ---->\n");
    Uart_Printf("S3C44B0X will also wake up by keyboard.\n");
    Uart_TxEmpty(0);    //Wait until UART0 Tx buffer empty.
    
    pISR_TIMER0=(U32)Timer0Int; 
    pISR_TIMER1=(U32)Timer1Int;
    pISR_EINT4567=(U32)EINT4567Int;
    
    rTCFG0=0x00010110;	//PRESC01,23,45= 1
    rTCFG1=0x00000000; 	//TIMER0,1,2,3,4,5= 1/2    
    rTCNTB0=65535;	
    rTCNTB1=2570;
    //rTCON=0xa0a;	//T1=MU,ITV,T0=MU,ITV
    //rTCON=0x909;	//Start T0,T1.
    rTCON=0x00a;
    rTCON=0x009;

    rINTMSK=~(BIT_GLOBAL|BIT_EINT4567|BIT_TIMER0|BIT_TIMER1|BIT_EINT1);

    //The two timer will test the IDLE mode hard.
     for(i=0;i<10000;i++)
    {
    	rCLKCON=0x7ff8|0x4; //enter IDLE mode.
    	for(j=0;j<10;j++); //wait until S3C44B0X enters IDLE mode.    	
    	rCLKCON=0x7ff8;   //wake up from normal mode    
    	Uart_Printf(".");//turn-off IDLE bit on rCLKCON to synchronize rCLKCON with the real mode.
    }
    rTCON=0x0;         //timer off
    rINTMSK|=~BIT_GLOBAL;
}
/* ----------------------------------------------------------------------- */
/********************** *   Normal mode test   * **********************/
void Test_HoldMode(void)
{
    Uart_Printf("<---- HOLD Mode(Normal mode with some stopped blocks) ---->\n");
    Uart_Printf("IIS,IIC,ADC,RTC,UART1,SIO,ZDMA,Timer,LCD are stopped step by step.\n");
    Uart_Printf("Check the current cunsumption. Type any key to exit.\n");	
    //Except GPIO,BDMA,UART0
    Uart_Printf("IIS off.\n");
    rCLKCON=0x3ff8;
    Uart_Getch();
    Uart_Printf("IIC off.\n");
    rCLKCON=0x1ff8;
    Uart_Getch();
    Uart_Printf("ADC off.\n");
    rADCCON|=0x20;
    rCLKCON=0x0ff8;
    Uart_Getch();
    Uart_Printf("RTC_control off.\n");
    rCLKCON=0x07f8;
    Uart_Getch();
    Uart_Printf("UART1 off.\n");
    rCLKCON=0x05f8;
    Uart_Getch();
    Uart_Printf("SIO off.\n");
    rCLKCON=0x05d8;
    Uart_Getch();
    Uart_Printf("ZDMA01 off.\n");
    rCLKCON=0x05c8;
    Uart_Getch();
    Uart_Printf("TIMER0123 off.\n");
    rCLKCON=0x05c0;
    Uart_Getch();
    Uart_Printf("LCD off.\n");
    rCLKCON=0x0580;
    Uart_Getch();

    Uart_Printf("Return to Normal Mode.\n");	    
    rCLKCON=0x7ff8; //IIS,IIC,ADC,RTC,UART,BRDMA,SIO,ZDMA,Timer,LCD	       
}

/* ----------------------------------------------------------------------- */
void Rtc_Init(void)
{
    rRTCCON = 0x01;      // R/W enable, 1/32768, Normal(merge), No reset

    rBCDYEAR = TESTYEAR;
    rBCDMON  = TESTMONTH;
    rBCDDAY  = TESTDAY;	 // SUN:1 MON:2 TUE:3 WED:4 THU:5 FRI:6 SAT:7
    rBCDDATE = TESTDATE;
    rBCDHOUR = TESTHOUR;
    rBCDMIN  = TESTMIN;
    rBCDSEC  = TESTSEC;
    
    rRTCCON = 0x0;      // R/W disable, 1/32768, Normal(merge), No reset

}
void LcdInit_4Gray320x240(void)
{
	if((U32)frameBuffer1==0)
	{
	    frameBuffer1=(unsigned int (*)[SCR_XSIZE/32])malloc(ARRAY_SIZE_MONO); 
	}
	rLCDCON1=(0)|(1<<5)|(MVAL_USED<<7)|(0x3<<8)|(0x3<<10)|(CLKVAL_MONO<<12);
	    // disable,4B_SNGL_SCAN,WDLY=8clk,WLH=8clk,
	rLCDCON2=(LINEVAL)|(HOZVAL<<10)|(10<<21);  
	    //LINEBLANK=10 (without any calculation) 
	rLCDSADDR1= (0x0<<27) | ( ((U32)frameBuffer1>>22)<<21 ) | M5D((U32)frameBuffer1>>1);
	    // monochrome, LCDBANK, LCDBASEU
	rLCDSADDR2= M5D( (((U32)frameBuffer1+(SCR_XSIZE*LCD_YSIZE/8))>>1) ) | (MVAL<<21);
	rLCDSADDR3= (LCD_XSIZE/16) | ( ((SCR_XSIZE-LCD_XSIZE)/16)<<9 );
	rLCDCON1=(1)|(1<<5)|(MVAL_USED<<7)|(0x3<<8)|(0x3<<10)|(CLKVAL_MONO<<12);
	    // enable,4B_SNGL_SCAN,WDLY=8clk,WLH=8clk,
}
void Display_4Gray320x240(void)
{
    int i,j;

    for(j=0;j<100;j++)
        for(i=2;i<10;i++)
        {
            _frameBuffer4[j][i]=0x55aa55aa;
        }

    for(j=0;j<100;j++)
    {
        _frameBuffer4[j][9]=0x5555ffff;
    }

    for(j=100;j<240;j++)
        for(i=2;i<10;i++)
            _frameBuffer4[j][i]=0x0;    
      for(i=2;i<10;i++)
        _frameBuffer4[100][i]=0xffffffff;
    for(i=2;i<10;i++)
        _frameBuffer4[0][i]=0xffffffff;
}

void idlePort(void)
{
// The I/O ports have to be configured 
// properly to reduce STOP mode current.
// pullup + output=high
    rPCONA=0x3ff;    //ROM addr:16,17,18,19 SDRAM bank addr:21,22

    rPCONB=0x1ff;	    //SDRAM:SCKE,SCLK,nSCAS,nSRAS

    rPDATC=0x0;
    rPCONC=0x10000000;	//all output
    rPUPC=0xc000;

    rPCOND=0xaaaa;	
    rPUPD=0xff;

    rPCONE=0x28;	
    rPUPE=0x106;

    rPDATF=0x0;
    rPCONF=0x2400;	//all output
    rPUPF=0x1e3;

    rPCONG=0xd30;	//all output
    rPUPG=0x24;

    rSPUCR=0x7;		
    rEXTINT=0x22222222;	//falling edge
}

void EINT4567Int(void)
{
    rI_ISPC=BIT_EINT4567;
    Uart_Printf("EINT4567 ISR is occurred for wake-up from IDLE mode.\n");
    rEXTINTPND = 0xf;				//clear EXTINTPND reg.		
}

void Timer0Int(void)
{
    int i;
    //for(i=0;i<10000;i++);
    rI_ISPC=BIT_TIMER0;
    for(i=0;i<100;i++); //
    t0cnt++;
}

void Timer1Int(void)
{
    rI_ISPC=BIT_TIMER1;
    t1cnt++;
}

void AlarmInt(void)
{
    rI_ISPC=BIT_RTC;
    Uart_Printf("ALARM ISR is occurred for wake-up from IDLE mode.\n");
}

void SlAlarmInt(void)
{
    //If you have to use the internal peripherals, 
    //you must configure CLKCON,CLKSLOW,PLL   

    rI_ISPC=BIT_RTC;
    debug=2;
}

void SLWEINT4567Int(void)
{
    rI_ISPC=BIT_EINT4567;     //clear pending_bit
    slw_exit=1;
    rEXTINTPND = 0xf;         //clear EXTINTPND reg.		
}

void STEINT4567Int(void)
{
    rI_ISPC=BIT_EINT4567;	//clear pending_bit
    wake_int=2;
    rEXTINTPND = 0xf;				//clear EXTINTPND reg.		
}
void Key_EINT1(void)
{
    rI_ISPC=BIT_EINT1;	//clear pending_bit
    wake_int=3;
}

void STAlarmInt(void)
{
    rI_ISPC=BIT_RTC;   
    //rI_ISPC; //is needed only when cache=on & wrbuf=on & BSFRD=0
    wake_int=1;
}

void stopPort(void)
{
// The I/O ports have to be configured 
// properly to reduce STOP mode current.
// pullup + output=high
    rPCONA=0x3ff;    //ROM addr:16,17,18,19 SDRAM bank addr:21,22

    rPCONB=0x7ff;	    //SDRAM:SCKE,SCLK,nSCAS,nSRAS

    rPDATC=0x0;
    rPCONC=0x10000000;	//all output
    rPUPC=0xc000;

    rPDATD=0x0;
    rPCOND=0x5555;	//all output

    rPDATE=0x0;
    rPCONE=0x404;	//all output
    rPUPE=0x106;

    rPDATF=0x0;
    rPCONF=0x00;	//all output
    rPUPF=0x1e3;

    rPDATG=0x0;
    rPCONG=0x55aa;	//all output
    rPUPG=0x0;

    rSPUCR=0x0;		
    rEXTINT=0x22222222;	//falling edge

    rRTCCON=0x0;    //RTC R/W disable for power consumption
    rADCCON |=0x20; //ADC sleep mode
}

char INT_set()
{
    char user_sel=0;
    unsigned int save_G,save_PG;
    Uart_Printf("Select the external interrupt type. Press the number!!!\n");
    Uart_Printf("Please Select the trigger:\n"
	"  1> Falling trigger\n"
	"  2> Rising trigger\n"
	"  3> Both Edge trigger\n" 
	"  4> Low level trigger\n"
	"  5> High level trigger\n"
	"\n   any key to exit...\n");
    user_sel=Uart_Getch();
    Uart_SendByte(user_sel);
    save_G=rPCONG;
    save_PG=rPUPG;
    rPCONG=0xffff;	//EINT7~0
    rPUPG=0x0;		//pull up enable	    

    switch(user_sel)
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
	    //rINTMSK    |= BIT_EINT4567;
	    break;
	default:
	    rPCONG=save_G;
	    rPUPG=save_PG;
    }

}
/* ----------------------------------------------------------------------- */
