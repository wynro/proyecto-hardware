/**********************************************************************
 CAUTION: DMA operation is being done in cache-on state NOW.
	  So,some read value may be not same with real memory value
	  because of the cache. Users must fully consider the role of
	  cache after DMA has been being operated. It is the best way
	  using non-cacheable region in the memory area written by DMA.
 **********************************************************************/

#include <string.h>
#include "44b.h"
#include "44blib.h"
#include "def.h"

int Zdma0Int(int srcAddr,int dstAddr,int length,int dw);
int Zdma1Int(int srcAddr,int dstAddr,int length,int dw);
void Zdma0Done(void) __attribute__ ((interrupt ("IRQ")));
void Zdma1Done(void) __attribute__ ((interrupt ("IRQ")));

volatile int zdma0Done,zdma1Done;
int tLength=0x80000;

void Test_Zdma0(void)
{
    unsigned char *src, *dst;
    int i;
    unsigned int memSum;
    
    rINTMSK|=BIT_GLOBAL;
    pISR_ZDMA0=(int)Zdma0Done;

    Uart_Printf("\n  ZDMA0 Memory --> Memory Test.\n");

    src=(unsigned char *)malloc(tLength);
    dst=(unsigned char *)malloc(tLength);
	 
    rNCACHBE1=( ( (((unsigned)dst+0x100000)>>12) +1 )<<16 )|((unsigned)dst>>12);

    Uart_Printf("  dst=%x,src=%x\n",(int)dst,(int)src);

/* Copy by word */
    memSum=0;
    for(i=0;i<tLength;i++)
	*(src+i)=0x1;
    Zdma0Int((int)src,(int)dst,tLength,2); //word
    for(i=0;i<tLength;i++)
	memSum+=*(dst+i);
    Uart_Printf("  Sum of the Memory =%8x: <Copy by Word>",memSum);
    if(memSum==tLength)Uart_Printf("  O.K.\n");
    else Uart_Printf("  ERROR!!!\n");

/* Copy by half-word */
    memSum=0;
    for(i=0;i<tLength;i++)
	*(src+i)=2;
    Zdma0Int((int)src,(int)dst,tLength,1); //half-word
    for(i=0;i<tLength;i++)
	memSum+=*(dst+i);
    Uart_Printf("  Sum of the Memory =%8x: <Copy by half Word>",memSum);
    if(memSum==0x100000)Uart_Printf("  O.K.\n");
    else Uart_Printf("  ERROR!!!\n");

/* Copy by byte */
    memSum=0;
    for(i=0;i<tLength;i++)
	*(src+i)=3;
    Zdma0Int((int)src,(int)dst,tLength,0); //byte
    for(i=0;i<tLength;i++)
	memSum+=*(dst+i);
    Uart_Printf("  Sum of the Memory =%8x: <Copy by Byte>",memSum);
    if(memSum==0x180000)Uart_Printf("  O.K.\n");
    else Uart_Printf("  ERROR!!!\n");

    free(src);
    free(dst);
}

void Test_Zdma1(void)
{
    unsigned char *src, *dst;
    int i;
    unsigned int memSum;

    rINTMSK|=BIT_GLOBAL;
    pISR_ZDMA1=(int)Zdma1Done;

    Uart_Printf("\n  ZDMA1 Memory --> Memory Test.\n");

    dst=(unsigned char *)malloc(tLength);
    src=(unsigned char *)malloc(tLength);

    rNCACHBE1=( ( (((unsigned)dst+0x100000)>>12) +1 )<<16 )|((unsigned)dst>>12);

/* Copy by word */
    memSum=0;
    for(i=0;i<tLength;i++)
	*(src+i)=1;
    Zdma1Int((int)src,(int)dst,tLength,2); //word
    for(i=0;i<tLength;i++)
	memSum+=*(dst+i);
    Uart_Printf("  Sum of the Memory =%8x: <Copy by Word>",memSum);
    if(memSum==tLength)Uart_Printf("  O.K.\n");
    else Uart_Printf("  ERROR!!!\n");

/* Copy by half-word */
    memSum=0;
    for(i=0;i<tLength;i++)
	*(src+i)=2;
    Zdma1Int((int)src,(int)dst,tLength,1); //half-word
    for(i=0;i<tLength;i++)
	memSum+=*(dst+i);
    Uart_Printf("  Sum of the Memory =%8x: <Copy by half Word>",memSum);
    if(memSum==0x100000)Uart_Printf("  O.K.\n");
    else Uart_Printf("  ERROR!!!\n");

/* Copy by byte */
    memSum=0;
    for(i=0;i<tLength;i++)
	*(src+i)=3;
    Zdma1Int((int)src,(int)dst,tLength,0); //byte
    for(i=0;i<tLength;i++)
	memSum+=*(dst+i);
    Uart_Printf("  Sum of the Memory =%8x: <Copy by Byte>",memSum);
    if(memSum==0x180000)
	Uart_Printf("  O.K.\n");
    else
	Uart_Printf("  ERROR!!!\n");

    free(src);
    free(dst);
}

int Zdma0Int(int srcAddr,int dstAddr,int length,int dw)
//returns the checksum
{
    int time;
    zdma0Done=0;
    rINTMSK=~(BIT_GLOBAL|BIT_ZDMA0);
    rZDISRC0=srcAddr|(dw<<30)|(1<<28); // inc
    rZDIDES0=dstAddr|( 2<<30)|(1<<28); // inc
    rZDICNT0=length |( 2<<28)|(1<<26)|(3<<22)|(1<<20);
    //whole,unit transfer,int@TC,enable DMA
    rZDCON0=0x1; // start!!!  

    Timer_Start(3);//128us resolution	      
    while(zdma0Done==0);
    time=Timer_Stop();
    Uart_Printf("  ZDMA0 %8x->%8x,%x: time = %dms\n",srcAddr,dstAddr,length,time);//*128E-6);
    rINTMSK=BIT_GLOBAL;
    return time;
}

//returns the checksum
int Zdma1Int(int srcAddr,int dstAddr,int length,int dw) 
{
    int time;
    zdma1Done=0;
    rINTMSK=~(BIT_GLOBAL|BIT_ZDMA1);
    rZDISRC1=srcAddr|(dw<<30)|(1<<28);	  // inc
    rZDIDES1=dstAddr|( 2<<30)|(1<<28);	  // inc
    rZDICNT1=length |( 2<<28)|(1<<26)|(3<<22)|(1<<20);
    //whole,unit transfer,int@TC,enable DMA
    rZDCON1=0x1; // start!!!

    Timer_Start(3);//128us resolution 
    while(zdma1Done==0);
    time=Timer_Stop();
    Uart_Printf("  ZDMA1 %8x->%8x,%x: time = %dms\n",srcAddr,dstAddr,length,time);//*128E-6);
    rINTMSK=BIT_GLOBAL;
}

void Zdma0Done(void)
{
    rI_ISPC=BIT_ZDMA0;	//clear pending
    //rI_ISPC;		//is needed only when cache=on & wrbuf=on & BSFRD=0
    zdma0Done=1;
}



void Zdma1Done(void)
{
    rI_ISPC=BIT_ZDMA1;	//clear pending
    //rI_ISPC;		//is needed only when cache=on & wrbuf=on & BSFRD=0
    zdma1Done=1;
}




