/*********************************************************************************************
* File£º	IIC.c
* Author:	embest
* Desc£º	IIC source code
* History:	
*********************************************************************************************/

/*--- include files ---*/

#include "44b.h"
#include "44blib.h"
#include "def.h"
#include "iic.h"

/*--- global variables ---*/

int iGetACK;

/*--- function declare ---*/
void IicInt(void) __attribute__ ((interrupt ("IRQ")));
void Test_Iic(void);
void Wr24C040(U32 slvAddr,U32 addr,U8 data);
void Rd24C040(U32 slvAddr,U32 addr,U8 *data);

/*--- function code ---*/

/*********************************************************************************************
* name:		Test_Iic
* func:		test IIC
* para:		none
* ret:		none
* modify:
* comment:		
********************************************************************************************/
void Test_Iic(void)
{
    unsigned int i,j;
    static U8 data[16];

	iGetACK = 0;
    Uart_Printf("\nIIC Test using AT24C04 (U18)...\n");

    /* enable interrupt */
	rINTMOD=0x0;
	rINTCON=0x1;
	
	rINTMSK=~(BIT_GLOBAL|BIT_IIC);
    pISR_IIC=(unsigned)IicInt;

	/* S3C44B0X slave address */
    rIICADD=0x10;
    
	/*Enable ACK,interrupt, IICCLK=MCLK/16, Enable ACK//64Mhz/16/(15+1) = 257Khz */
    rIICCON=0xaf;
    
    /* enbale TX/RX */
    rIICSTAT=0x10;

    Uart_Printf("Write char 0-f into AT24C04\n");

	/* write 0-255 to 24C080 */
    for( i=0; i<16; i++ )
		Wr24C040(0xa0,(U8)i,i);
	
	/* clear array */
    for( i=0; i<16; i++)
		data[i]=0;

    Uart_Printf("Read 16 bytes from AT24C04\n");
    
    /* read 16 byte from 24C080 */
    for( i=0; i<16; i++ )
		Rd24C040(0xa0,(U8)i,&(data[i])); 

	/* printf read data */
    for( i=0; i<16; i++ )
    {
	    Uart_Printf("%2x ",data[i]);
    }
    Uart_Printf("\n");
}

/*********************************************************************************************
* name:		Wr24C040
* func:		write data to 24C080
* para:		slvAddr --- chip slave address
*			addr	--- data address
*			data    --- data value
* ret:		none
* modify:
* comment:		
********************************************************************************************/
void Wr24C040(U32 slvAddr,U32 addr,U8 data)
{
	iGetACK = 0;
    
    /* send control byte */
    rIICDS = slvAddr;		// 0xa0
    rIICSTAT=0xf0; 			// Master Tx,Start

    while(iGetACK == 0);	// wait ACK
    iGetACK = 0;
    
    /* send address */
    rIICDS = addr;
    rIICCON = 0xaf;   		// resumes IIC operation.
    
    while(iGetACK == 0);	// wait ACK
    iGetACK = 0;
    
    /* send data */
    rIICDS = data;
    rIICCON = 0xaf;   		// resumes IIC operation.
    
    while(iGetACK == 0);	// wait ACK
    iGetACK = 0;
    
    /* end send */
    rIICSTAT = 0xd0;		// stop Master Tx condition
    rIICCON = 0xaf;			// resumes IIC operation.
    DelayMs(5);				// wait until stop condtion is in effect.
}
	
/*********************************************************************************************
* name:		Rd24C080
* func:		read data from 24C080
* para:		slvAddr --- chip slave address
*			addr	--- data address
*			data    --- data pointer
* ret:		none
* modify:
* comment:		
********************************************************************************************/
void Rd24C040(U32 slvAddr,U32 addr,U8 *data)
{
	char recv_byte;
	
	iGetACK = 0;

    /* send control byte */
    rIICDS = slvAddr;		// 0xa0
    rIICSTAT=0xf0; 			// Master Tx,Start

    while(iGetACK == 0);	// wait ACK
    iGetACK = 0;

    /* send address */
    rIICDS = addr;
    rIICCON = 0xaf;   		// resumes IIC operation.
    
    while(iGetACK == 0);	// wait ACK
    iGetACK = 0;

    /* send control byte */
    rIICDS = slvAddr;		// 0xa0
    rIICSTAT=0xb0; 			// Master Rx,Start
	rIICCON=0xaf;  			// resumes IIC operation.   
	
    while(iGetACK == 0);	// wait ACK
    iGetACK = 0;
    
    /* get data */
    recv_byte = rIICDS;
    rIICCON = 0x2f;
    DelayMs(1);
    
    /* get data */
    recv_byte = rIICDS;
    
    /* end receive */
    rIICSTAT = 0x90;  		// stop Master Rx condition 
	rIICCON = 0xaf;   		// resumes IIC operation.
	DelayMs(5);				// wait until stop condtion is in effect.
	
    *data = recv_byte;
}

/*********************************************************************************************
* name:		IicInt
* func:		IIC interrupt handler
* para:		none
* ret:		none
* modify:
* comment:		
********************************************************************************************/
void IicInt(void)
{
    rI_ISPC=BIT_IIC;
	iGetACK = 1;
}
