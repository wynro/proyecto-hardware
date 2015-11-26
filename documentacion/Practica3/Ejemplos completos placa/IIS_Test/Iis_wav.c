/*********************************************************************************************
* File£º	IIS.c
* Author:	embest	
* Desc£º	IIS sound circuit code
* History:	
*********************************************************************************************/

/*--- include files ---*/
#include "44b.h"
#include "44blib.h"
#include "option.h"
#include "def.h"
#include "iis.h"

#define TESTSIZE 700000
#define PLAY 0
#define RECORD 1
#define REC_LEN	0xF0000

/*--- global variables ---*/
int   iDMADone;
unsigned char *Buf,*_temp;
unsigned char *rec_buf;
volatile unsigned int size=0;
volatile unsigned int leng,ssize;
volatile char Rec_Done=0;
volatile char mute=1;

void Test_Iis(void);

/*--- function code---*/
/*********************************************************************************************
* name:		Test_Iis
* func:		Test IIS circuit
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void Test_Iis(void)
{
    IISInit();					// initialize IIS
    Uart_Printf(" press \"R\" to Record..., any key to play wav(t.wav)\n");
    if(Uart_Getch()=='R')
      Record_Iis();             // test record
    Playwave(5);				// play wave 5 times
    IISClose();					// close IIS
}

/*********************************************************************************************
* name:		IISInit
* func:		Initialize IIS circuit
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void IISInit(void)
{
	rPCONE = (rPCONE&0xffff)+(2<<16); 	// PE:CODECLK
	iDMADone = 0;
	/* initialize philips UDA1341 chip */
	Init1341(PLAY);	
}

/*********************************************************************************************
* name:		IISClose
* func:		Close IIS circuit
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void IISClose()
{
    rIISCON = 0x0;    			// IIS stop
    Cache_Flush();
	
	/* recover Port B and Port C state */    
    rPDATB = 0x7ff;				// P9-LED1 P10-LED2
	rPCONB = 0x1cf;
	rPDATC = 0xff00;
	rPCONC = 0x0ff0ffff;	
	rPUPC  = 0x30ff;
	
	rINTMSK |= BIT_GLOBAL;		// Mask interrupt
}

/*********************************************************************************************
* name:		Playwave
* func:		play wave data
* para:		
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void Playwave(int times)
{
	int  sound_len,i;
	unsigned short* pWavFile;
	
	/* enable interrupt */
	rINTMOD=0x0;
	rINTCON=0x1;
	/* execute command "download t.wav 0xc300000" before running */
	pWavFile=(unsigned short*)0xC300000;
    
	/* initialize philips UDA1341 chip */
	Init1341(PLAY);	
    /* set BDMA interrupt */
	pISR_BDMA0=(unsigned)BDMA0_Done;
    rINTMSK=~(BIT_GLOBAL|BIT_BDMA0);
    
 	for(i = times; i != 0; i--)
	{
		/* initialize variables */
		iDMADone = 0;
		sound_len = 475136;//512000;// 155956;
		
		rBDISRC0 = (1<<30)+(1<<28)+((int)(pWavFile));	// Half word,increment,Buf
	    rBDIDES0 = (1<<30)+(3<<28)+((int)rIISFIF);		// M2IO,Internel peripheral,IISFIF
	    /* IIS,Int,auto-reload,enable DMA */
	    rBDICNT0 = (1<<30)+(1<<26)+(3<<22)+(0<<21)+(1<<20)+sound_len;
	    rBDCON0  = 0x0<<2;		
	   
		/* IIS Initialize */
	    rIISCON  = 0x22;			// Tx DMA enable,Rx idle,prescaler enable
	    rIISMOD  = 0xC9;			// Master,Tx,L-ch=low,iis,16bit ch.,codeclk=256fs,lrck=32fs
	    rIISPSR  = 0x88;//0x22;			// Prescaler_A/B enable, value=3
	    rIISFCON = 0xF00;			// Tx/Rx DMA,Tx/Rx FIFO 
		
	    rIISCON |= 0x1;             // enable IIS   
	    while( iDMADone == 0);      // DMA end ?	    
	    rIISCON = 0x0;    			// IIS stop
	}
}

void Record_Iis(void)
{
    U32 i;
    
	/* enable interrupt */
	//rINTMOD=0x0;
	//rINTCON=0x1;

    Uart_Printf("  Record test using UDA1341...\n");
    rPCONE=(rPCONE&0xffff)+(2<<16);      // PE:CODECLK
    pISR_BDMA0=(unsigned)BDMA0_Rec_Done;
    rINTMSK=~(BIT_GLOBAL|BIT_BDMA0);

    rec_buf=(unsigned char *)0x0C400000; // for download
    Buf =rec_buf;
    for(i= ( U32 )rec_buf;i<( ( U32 )rec_buf+REC_LEN);i+=4)
    {	*((volatile unsigned int*)i)=0x0;    }

    Init1341(RECORD);
    /****** BDMA0 Initialize ******/
    rBDISRC0=(1<<30)+(3<<28)+((int)rIISFIF);	// Half word,inc,Buf
    rBDIDES0=(2<<30)+(1<<28)+((int)rec_buf);	// M2IO,fix,IISFIF
    rBDICNT0=(1<<30)+(1<<26)+(3<<22)+(1<<21)+(1<<20)+REC_LEN;
    rBDCON0 = 0x0<<2;
  
    /****** IIS Initialize ******/
    rIISCON=0x1a;	// Rx DMA enable,Rx idle,prescaler enable
    rIISMOD=0x49;	// Master,Tx,L-ch=low,iis,16bit ch.,codeclk=256fs,lrck=32fs
    rIISPSR=0x33;	// Prescaler_A/B enable, value=3
    rIISFCON=0x500;	// Tx/Rx DMA,Tx/Rx FIFO --> start piling....
    
    Uart_Printf(" Press any key to start record!\n");
    Uart_Getch();
    Uart_Printf(" Recording...\n");
    
    rIISCON |=0x1;  // --- Rx start
    
    Uart_Printf(" Press any key to stop record!!!\n");
    while(!Uart_GetKey());
    // while(!Rec_Done);
    rINTMSK |= BIT_BDMA0;
    Rec_Done=0;
    Delay(10);		// for end of H/W Rx
    rIISCON=0x0;	// IIS stop
    rBDICNT0=0x0;   // BDMA stop
    Uart_Printf(" End of Record!!!\n");
    Uart_Printf(" Press any key to play recorded data\n");
    Uart_Getch();
    
    size=*(Buf+0x2c) | *(Buf+0x2d)<<8 | *(Buf+0x2e)<<16 | *(Buf+0x2f)<<24;
    size=(size>>1)<<1;
    Uart_Printf(" sample size=0x%x\n",size/2);
    //size=REC_LEN*2;
    //Uart_Printf(" size=%d\n",size);
    Init1341(PLAY);

    rBDIDES0=(1<<30)+(3<<28)+((int)rIISFIF);	// M2IO,fix,IISFIF
    rBDISRC0=(1<<30)+(1<<28)+((int)rec_buf);	// Half word,inc,Buf
    rBDICNT0=(1<<30)+(1<<26)+(3<<22)+(1<<21)+(1<<20)+ REC_LEN;
    rBDCON0 = 0x0<<2;  
    pISR_BDMA0=(unsigned)BDMA0_Done;
    rINTMSK=~(BIT_GLOBAL|BIT_BDMA0);
    
    /****** IIS Initialize ******/
    rIISCON=0x26;	// Tx DMA enable,Rx idle,prescaler enable
    rIISMOD=0x89;	// Master,Tx,L-ch=low,iis,16bit ch.,codeclk=256fs,lrck=32fs
    rIISPSR=0x33;	// Prescaler_A/B enable, value=3
    rIISFCON=0xa00;	// Tx/Rx DMA,Tx/Rx FIFO --> start piling....
    Uart_Printf(" Press any key to exit!!!\n");
    
    rIISCON |=0x1;  // IIS Start
    while(!Uart_GetKey());
    rIISCON=0x0;    // IIS stop
    rBDICNT0=0x0;   // BDMA stop

    free(rec_buf);
    Cache_Flush();
    rNCACHBE0=0x0;
    size=0;
  
    rINTMSK=BIT_GLOBAL;
}

/*********************************************************************************************
* name:		BDMA0
* func:		BDMA0 interrupt handler
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void BDMA0_Rec_Done(void)
{
    rI_ISPC=BIT_BDMA0;	// clear pending bit
    WrUTXH0('#');
    
}

void BDMA0_Done(void)
{
    rI_ISPC=BIT_BDMA0;			// clear pending bit
    WrUTXH0('&');
    iDMADone = 1;
}

