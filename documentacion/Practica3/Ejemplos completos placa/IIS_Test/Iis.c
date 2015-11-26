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

/*--- function decleare ---*/

void Playwave();
void Test_Iis(void);
void IISInit(void);
void IISClose(void);
void _WrL3Addr(U8 data);
void _WrL3Data(U8 data,int halt);
void Init1341();
void BDMA0_Done(void) __attribute__ ((interrupt ("IRQ")));

/*--- macro define ---*/

#define L3D (0x200)
#define L3M (0x10)
#define L3C (0x20)

#define TESTSIZE 700000
#define PLAY 0
#define RECORD 1
#define REC_LEN	0xF0000

/*--- global variables ---*/

int   iDMADone;

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
    Playwave();					// play wave
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
		
	/* initialize philips 1341 chip */
	Init1341();
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
* name:		Init1341
* func:		Init philips 1341 chip
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void Init1341()
{
    /* Port Initialize */
    rPCONA = 0x1ff;				// PA9(out):L3D
    rPCONB = 0x7CF;				// PG5:L3M, PG4:L3C
    rPDATB = L3M|L3C;			// L3M=H(start condition),L3C=H(start condition)					

    /* L3 Interface */
    _WrL3Addr(0x14+2);			// status (000101xx+10)
#ifdef FS441KHZ
    _WrL3Data(0x60,0); 			// 0,1,10,000,0 reset,256fs,no DCfilter,iis
#else
    _WrL3Data(0x40,0); 			// 0,1,00,000,0 reset,512fs,no DCfilter,iis
#endif

    _WrL3Addr(0x14+2); 			// status (000101xx+10)
#ifdef FS441KHZ
    _WrL3Data(0x20,0); 			// 0,0,10,000,0 no reset,256fs,no DCfilter,iis
#else
    _WrL3Data(0x00,0); 			// 0,0,00,000,0 no reset,512fs,no DCfilter,iis
#endif
    
    _WrL3Addr(0x14+2); 			// status (000101xx+10)
    _WrL3Data(0x81,0);			// 1,0,0,0,0,0,11 OGS=0,IGS=0,ADC_NI,DAC_NI,sngl speed,AonDon
    _WrL3Addr(0x14+0); 			// DATA0 (000101xx+00)
    _WrL3Data(0x0A,0); 
}

/*********************************************************************************************
* name:		_WrL3Addr
* func:		write control data address to 1341 through L3-interface
* para:		data -- control data address
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void _WrL3Addr(U8 data)
{	
    U32 vPdata = 0x0;			// L3D=L	
    U32 vPdatb = 0x0;			// L3M=L(in address mode)/L3C=L
    S32 i,j;

    rPDATB = vPdatb;			// L3M=L
    rPDATB |= L3C;				// L3C=H

    for( j=0; j<4; j++ )		// tsu(L3) > 190ns
    	;

    //PA9:L3D PG6:L3M PG7:L3C
    for( i=0; i<8; i++ )		
    {
		if( data&0x1 )			// if data bit is 'H'
		{
		    rPDATB = vPdatb;	// L3C=L
		    rPDATA = L3D;		// L3D=H		    
		    for( j=0; j<4; j++ )// tcy(L3) > 500ns
		    	;	
		    rPDATB = L3C;		// L3C=H
		    rPDATA = L3D;		// L3D=H
		    for( j=0; j<4; j++ )// tcy(L3) > 500ns
		    	;
		}
		else					// if data bit is 'L'
		{
		    rPDATB = vPdatb;	// L3C=L
		    rPDATA = vPdata;	// L3D=L
		    for( j=0; j<4; j++ )// tcy(L3) > 500ns
		    	;
		    rPDATB = L3C;		// L3C=H
		    rPDATA = vPdata;	// L3D=L
		    for( j=0; j<4; j++ )// tcy(L3) > 500ns
		    	;
		}
		data >>= 1;
    }
    rPDATG = L3C|L3M;			// L3M=H,L3C=H
}

/*********************************************************************************************
* name:		_WrL3Data
* func:		write control data to 1341 through L3-interface
* para:		data -- control data
*			halt -- halt operate
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void _WrL3Data(U8 data,int halt)
{
    U32 vPdata = 0x0;   		// L3D=L
    U32 vPdatb = 0x0;			// L3M/L3C=L
    S32 i,j;
    
    if(halt)
    {
        rPDATB = L3C;	    	// L3C=H(while tstp, L3 interface halt condition)
        for( j=0; j<4; j++ )   	// tstp(L3) > 190ns
        	;
    }
    rPDATB = L3C|L3M;	    	// L3M=H(in data transfer mode)	
    for( j=0; j<4; j++ )		// tsu(L3)D > 190ns
    	;	    	

    // PA9:L3MODE PG6:L3DATA PG7:L3CLOCK
    for( i=0; i<8; i++ )
    {
        if( data&0x1 )			// if data bit is 'H'
        {
	    	rPDATB = L3M;		// L3C=L
            rPDATA = L3D;		// L3D=H
            for( j=0; j<4; j++ )// tcy(L3) > 500ns
		    	;
            rPDATB = L3C|L3M;	// L3C=H,L3D=H
	    	rPDATA = L3D;
            for( j=0; j<4; j++ )// tcy(L3) > 500ns
		    	;
        }
        else					// if data bit is 'L'
        {
            rPDATB = L3M;		// L3C=L
	    	rPDATA = vPdata;	// L3D=L
            for( j=0; j<4; j++ )// tcy(L3) > 500ns
		    	;
            rPDATB = L3C|L3M;	// L3C=H
	    	rPDATA = vPdata;	// L3D=L
            for( j=0; j<4; j++ )// tcy(L3) > 500ns
		    	;
        }
        data >>= 1;
    }
    rPDATB = L3C|L3M;			// L3M=H,L3C=H
}

/*********************************************************************************************
* name:		Playwave
* func:		play wave data
* para:		
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void Playwave()
{
	int  sound_len,i;
	char test_byte;
	
	/* enable interrupt */
	rINTMOD=0x0;
	rINTCON=0x1;
    
    /* set BDMA interrupt */
	pISR_BDMA0=(unsigned)BDMA0_Done;
    rINTMSK=~(BIT_GLOBAL|BIT_BDMA0);
    
    /* write 0-256 to IISFIF */
	for( i = 0 ; i < 256 ; i++ )
	{
		/* initialize variables */
		iDMADone = 0;
		test_byte = i;
		sound_len = 0x130;
		
		rBDISRC0 = (1<<30)+(3<<28)+((int)(&test_byte));	// Half word,increment,Buf
	    rBDIDES0 = (1<<30)+(3<<28)+((int)rIISFIF);		// M2IO,Internel peripheral,IISFIF
	    /* IIS,Int,auto-reload,enable DMA */
	    rBDICNT0 = (1<<30)+(1<<26)+(3<<22)+(0<<21)+(1<<20)+sound_len;
	    rBDCON0  = 0x0<<2;
		
		/* IIS Initialize */
	    rIISCON  = 0x22;			// Tx DMA enable,Rx idle,prescaler enable
	    rIISMOD  = 0xC9;			// Master,Tx,L-ch=low,iis,16bit ch.,codeclk=256fs,lrck=32fs
	    rIISPSR  = 0x88;			// Prescaler_A/B enable, value=3
	    rIISFCON = 0xF00;			// Tx/Rx DMA,Tx/Rx FIFO 
	    
	    /* enable IIS */
	    rIISCON |= 0x1;
	    
	    /* DMA end */
	    while( iDMADone == 0);
	    
	    rIISCON = 0x0;    			// IIS stop
	}
}

/*********************************************************************************************
* name:		BDMA0_Done
* func:		BDMA0 interrupt handler
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void BDMA0_Done(void)
{
    rI_ISPC=BIT_BDMA0;			// clear pending bit
    iDMADone = 1;
}