/*--- include files ---*/
#include "44b.h"
#include "44blib.h"
#include "option.h"
#include "def.h"
#include "iis.h"

/*--- macro define ---*/
#define L3D (0x200)
#define L3M (0x10)
#define L3C (0x20)

void _WrL3Addr(U8 data);
void _WrL3Data(U8 data,int halt);
void Init1341(char mode);

/*********************************************************************************************
* name:		Init1341
* func:		Init philips 1341 chip
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void Init1341(char mode)
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
//record
    if(mode)
    {
	_WrL3Addr(0x14+2); //STATUS (000101xx+10)
	_WrL3Data(0xa2,0); //1,0,1,0,0,0,10	: OGS=0,IGS=1,ADC_NI,DAC_NI,sngl speed,AonDoff

	_WrL3Addr(0x14+0); //DATA0 (000101xx+00)
	_WrL3Data(0xc2,0); //11000,010	: DATA0, Extended addr(010) 
	_WrL3Data(0x4d,0); //010,011,01	: DATA0, MS=9dB, Ch1=on Ch2=off, 
    }
//record
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
