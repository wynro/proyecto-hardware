/****************************************************************************
¡¾File¡¿model.c
¡¾Description¡¿Code for USB of Embest S3CEV40 Evaluation Board
¡¾Version¡¿1.0
¡¾Author and Date¡¿
¡¾Modify¡¿
****************************************************************************/
#include <stdio.h>
#include <stdarg.h>
#include "44b.h"
#include "option.h"
#include "usblib.h"

#define U32 unsigned int
#define U16 unsigned short
#define S32 int
#define S16 short int
#define U8  unsigned char
#define	S8  char
#define TRUE  1
#define FALSE 0

extern int dtapid_TGL3PID;

U8  COMbuf[64];
U8  COMlen;

/* Flush and disable the USB TX3 **************************************/
#define FLUSHTX3 {write_usb(TXC3,FLUSH);}
/* enable TX3, using the appropriate DATA PID, but not toggling it ****/
#define TXEN3_PID_NO_TGL                                               \
	  { if(dtapid_TGL3PID) write_usb(TXC3,TX_TOGL+TX_LAST+TX_EN);  \
	    else write_usb(TXC3,TX_LAST+TX_EN);               /*DATA3*/\
	  }
/* enable TX3, using the appropriate DATA PID *************************/
#define TXEN3_PID                                                      \
	  { TXEN3_PID_NO_TGL;                                          \
	    dtapid_TGL3PID=!dtapid_TGL3PID;}

/****************************************************************************
¡¾Function Description¡¿Phase-locked loop setting, modify system primary frequency
Fout = (8 + M_DIV) * Fin / [ (2+P_DIV) * (2^S_DIV) ]
****************************************************************************/
void ChangePllValue(int mdiv,int pdiv,int sdiv)
{
    rPLLCON = (mdiv << 12) | (pdiv << 4) | sdiv;
}

/****************************************************************************
¡¾Function Description¡¿Universal delay function, delay "time" 100us
****************************************************************************/
static int delayLoopCount = 400;
void Delay(int time)
{
	int i,adjust=0;
	if(time==0)
	{
		time=200;
		adjust=1;
		delayLoopCount=400;
		rWTCON=((MCLK/1000000-1)<<8)|(2<<3);
		rWTDAT=0xffff;
		rWTCNT=0xffff;	 
		rWTCON=((MCLK/1000000-1)<<8)|(2<<3)|(1<<5);
	}
	for(;time>0;time--)
		for(i=0;i<delayLoopCount;i++);
	if(adjust==1)
	{
		rWTCON=((MCLK/1000000-1)<<8)|(2<<3);
		i=0xffff-rWTCNT;
		delayLoopCount=8000000/(i*64);
	}
}

/****************************************************************************
¡¾Function Description¡¿IO port function, direction setting
****************************************************************************/
void Port_Init(void)
{
	//CAUTION:Follow the configuration order for setting the ports. 
	// 1) setting value 
	// 2) setting control register 
	// 3) configure pull-up resistor.  

	//16bit data bus configuration  

	// PORT A GROUP
	// BIT 	9	8	7	6	5	4	3	2	1	0
	// 		A24	A23	A22	A21	A20	A19	A18	A17	A16	A0
	//		0	1	1	1	1	1	1	1	1	1
	rPCONA = 0x1ff;	

	// PORT B GROUP
	// BIT 	10		9		8		7		6		5		4		3		2		1		0
	//		/CS5	/CS4	/CS3	/CS2	/CS1	GPB5	GPB4	/SRAS	/SCAS	SCLK	SCKE
	//		EXT		NIC		USB		IDE		SMC		NC		NC		Sdram	Sdram	Sdram	Sdram
	//      1, 		1,   	1,   	1,    	1,    	0,       0,     1,    	1,    	1,   	1	
	rPDATB = 0x7ff;				// P9-LED1 P10-LED2
	rPCONB = 0x1cf;
    
	// PORT C GROUP
	// BUSWIDTH=16
	//  PC15	14		13		12		11		10		9		8
	//	I		I		RXD1	TXD1	I		I		I		I
	//	NC		NC		Uart1	Uart1	NC		NC		NC		NC
	//	00		00		11		11		00		00		00		00

	//  PC7		6		5		4		3		2		1		0
	//   I		I		I		I		I		I		I		I
	//   NC		NC		NC		NC		IISCLK	IISDI	IISDO	IISLRCK
	//   00		00		00		00		11		11		11		11
	rPDATC = 0xff00;
	//?????2004/2/20  rPCONC = 0x0ff0ffff;	
	rPCONC = 0x4ff0ffff;	
	rPUPC  = 0x30ff;	//PULL UP RESISTOR should be enabled to I/O

	// PORT D GROUP
	// PORT D GROUP(I/O OR LCD)
	// BIT7		6		5		4		3		2		1		0
	//	VF		VM		VLINE	VCLK	VD3		VD2		VD1		VD0
	//	00		00		00		00		00		00		00		00
	rPDATD= 0xff;
	rPCOND= 0xaaaa;	
	rPUPD = 0x0;
	// These pins must be set only after CPU's internal LCD controller is enable
	
	// PORT E GROUP 
	// Bit	8		7		6		5		4		3		2		1		0
	//  	CODECLK	LED4	LED5	LED6	LED7	BEEP	RXD0	TXD0	LcdDisp
	//  	10		01		01		01		01		01		10		10		01
	rPDATE	= 0x1ff;
	rPCONE	= 0x25529;	
	rPUPE	= 0x6;
	
	// PORT F GROUP
	// Bit8		7		6		5		 4		3		2		1		0
	// IISCLK	IISDI	IISDO	IISLRCK	Input	Input	Input	IICSDA	IICSCL
	// 100		100		100		100		00		00		00		10		10
	rPDATF = 0x0;
	rPCONF = 0x252a;
	rPUPF  = 0x0;

	// PORT G GROUP
	// BIT7		6		5		4		3		2		1		0
	// 	INT7	INT6	INT5	INT4	INT3	INT2	INT1	INT0
	//	S3		S4		S5		S6		NIC		EXT		IDE		USB
	//	11      11      11      11      11      11      11      11
	rPDATG = 0xff;
	rPCONG = 0xffff;
	rPUPG  = 0x0;		//should be enabled  
	rSPUCR = 0x7;  		//D15-D0 pull-up disable

	/* define non-Cache area */
	rNCACHBE0=((Non_Cache_End>>12)<<16)|(Non_Cache_Start>>12);
	rNCACHBE1=((Non_Cache_End>>12)<<16)|(Non_Cache_Start>>12);
	/* all external hardware interruptions are low-lever triggering */
	rEXTINT=0x0;
}

/****************************************************************************
¡¾Function Description¡¿asynchronous serial port0 initializing
****************************************************************************/
void Uart0_Init(int mclk,int baud)
{
    if(mclk == 0)
	mclk = MCLK;

    rUFCON0 = 0x0;     //prohibition of using FIFO
    rUMCON0 = 0x0;     //prohibition of using FIFO
    rULCON0 = 0x3;     //without parity check normally, one stop bit, eight data bit
    rUCON0 = 0x245;    
   
    rUBRDIV0 = ( (int)(mclk/16./baud + 0.5) -1 );
    Delay(1);		//delay several 100us
}

/****************************************************************************
¡¾Function Description¡¿asynchronous serial port1 initializing
****************************************************************************/
void Uart1_Init(int mclk,int baud)
{
    if(mclk == 0)
	mclk = MCLK;

    rUFCON1 = 0x0;     //prohibition of using FIFO
    rUMCON1 = 0x0;     //prohibition of using FIFO
    rULCON1 = 0x3;     //without parity check normally, one stop bit, eight data bit    rUCON1 = 0x245;
   
    rUBRDIV1 = ( (int)(mclk/16./baud + 0.5) -1 );
    Delay(1);		//delay several 100us
}

/****************************************************************************
¡¾Function Description¡¿Get a character from serial port, from UART0 if ch is 0 and from UART1 if ch is 1.
****************************************************************************/
char Uart_Getch(U8 ch)
{
    if(ch == 0)
    {	    
		while(!(rUTRSTAT0 & 0x1));		//wait until receive a data
		return rURXH0;
    }
    else
    {
		while(!(rUTRSTAT1 & 0x1));		//wait until receive a data
		return	rURXH1;
    }
}

/****************************************************************************
¡¾Function Description¡¿Check if the serial port has received data and return. Get from UART0 if ch is 0 and from UART1 if ch is 1.
****************************************************************************/
char Uart_GetKey(U8 ch)
{
    if(ch == 0)
    {	    
		if(rUTRSTAT0 & 0x1)		//If received a character,return to the character.
    	    return rURXH0;
		else	return 0;		//If did not receive a character,return to 0.
    }
    else
    {
		if(rUTRSTAT1 & 0x1)		//If received a character,return to the character.
		    return rURXH1;
		else	return 0;		//If did not receive a character,return to 0.
    }
 }


/****************************************************************************
¡¾Function Description¡¿Send a data to serial port. Send to UART0 if ch is 0 and send to UART1 if ch is 1.
****************************************************************************/
void Uart_SendByte(U8 ch,int data)
{
	if(ch == 0)
	{
		if(data == '\n')
		{
			while(!(rUTRSTAT0 & 0x2));
			
			//change delay time1--10
			Delay(10);		//because hyperterminal is very slow, a slim delay exsits.
			WrUTXH0('\r');
		}
		while(!(rUTRSTAT0 & 0x2));		//wait until THR be blank
		//change delay time1--10
		Delay(10);
		rUTXH0 = data;
   	}
	else
	{
		if(data == '\n')
		{
			while(!(rUTRSTAT1 & 0x2));
			//change delay time1--10
			Delay(10);		//because hyperterminal is very slow, a slim delay exsits.
			rUTXH1 = '\r';
		}
		while(!(rUTRSTAT1 & 0x2));		//wait until THR be blank
		//change delay time1--10
		Delay(10);
		rUTXH1 = data;
    }	
}		

/****************************************************************************
¡¾Function Description¡¿Send a character string to serial port. Send to UART0 if ch is 0 and send to UART1 if ch is 1.
****************************************************************************/
void Uart_SendString(U8 ch,char *pt)
{
    while(*pt)		Uart_SendByte(ch,*pt++);
}

/****************************************************************************
¡¾Function Description¡¿Send a character string to serial port using standard format "printf"
Send to UART0 if ch is 0 and send to UART1 if ch is 1.(If not using vsprintf, code will be reduces much!)
****************************************************************************/
void Uart_Printf(U8 ch,char *fmt,...)
{
    va_list ap;
    char string[256];

    va_start(ap,fmt);
    vsprintf(string,fmt,ap);
    Uart_SendString(ch,string);
    va_end(ap);
}

/****************************************************************************
¡¾Function Description¡¿System Main Function
****************************************************************************/
void Main(void)
{
	rINTCON=0x00000000;
	rINTMSK=0x07ffffff;
	rINTMOD=0x00000000;		//IRQ mode


	Port_Init();			//IO port function, direction setting
//	ChangePllValue(56,2,1);	
	Uart0_Init(0,115200);		//asynchronous serial port0 initializing, set baud rate as 115200 
	Uart_Printf(0,"\n¡¾SAMSUNG S3C44B0X  EVB¡¿");
	Uart_Printf(0,"\n¡¾Flash£º2Mbyte SST39VF160¡¿");
	Uart_Printf(0,"\n¡¾SDRAM£º8Mbyte HY57V641620HG¡¿");

	USB_Init();		// initialize USB device

	while(1)
	{
		int count;
		char ch[100];

		USB_Purge(2);
		count = USB_Read(2, ch, 100);
		USB_Write(2, ch, count);
	}
}
//***************************************************************************
