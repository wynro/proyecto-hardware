/****************************************************************************
����  ��  ��  �ơ�model.c
����  ��  ��  ��������Ӣ����S3CEV40������USB�������
����  ��  ��  ����1.0
�������˼��������ڡ�
���޸��˼��޸����ڡ�
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
������˵�������໷���ã��޸�ϵͳ��Ƶ
Fout = (8 + M_DIV) * Fin / [ (2+P_DIV) * (2^S_DIV) ]
****************************************************************************/
void ChangePllValue(int mdiv,int pdiv,int sdiv)
{
    rPLLCON = (mdiv << 12) | (pdiv << 4) | sdiv;
}

/****************************************************************************
������˵����ͨ����ʱ��������ʱtime��100us
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
������˵����IO�˿ڹ��ܡ������趨
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

	/*�����Cache��*/
	rNCACHBE0=((Non_Cache_End>>12)<<16)|(Non_Cache_Start>>12);
	rNCACHBE1=((Non_Cache_End>>12)<<16)|(Non_Cache_Start>>12);
	/*���е��ⲿӲ���ж�Ϊ�͵�ƽ����*/
	rEXTINT=0x0;
}

/****************************************************************************
������˵�����첽���п�0��ʼ��
****************************************************************************/
void Uart0_Init(int mclk,int baud)
{
    if(mclk == 0)
	mclk = MCLK;

    rUFCON0 = 0x0;     //��ֹʹ��FIFO
    rUMCON0 = 0x0;     //��ֹʹ��FIFO
    rULCON0 = 0x3;     //��������żУ�飬һ��ֹͣλ��8������λ
    rUCON0 = 0x245;    
   
    rUBRDIV0 = ( (int)(mclk/16./baud + 0.5) -1 );
    Delay(1);		//��ʱ���ɸ�100us
}

/****************************************************************************
������˵�����첽���п�1��ʼ��
****************************************************************************/
void Uart1_Init(int mclk,int baud)
{
    if(mclk == 0)
	mclk = MCLK;

    rUFCON1 = 0x0;     //��ֹʹ��FIFO
    rUMCON1 = 0x0;     //��ֹʹ��FIFO
    rULCON1 = 0x3;     //��������żУ�飬һ��ֹͣλ��8������λ
    rUCON1 = 0x245;
   
    rUBRDIV1 = ( (int)(mclk/16./baud + 0.5) -1 );
    Delay(1);		//��ʱ���ɸ�100us
}

/****************************************************************************
������˵�����Ӵ��п���ȡһ���ַ���chΪ0��UART0��ȡ��chΪ1��UART1��ȡ
****************************************************************************/
char Uart_Getch(U8 ch)
{
    if(ch == 0)
    {	    
		while(!(rUTRSTAT0 & 0x1));		//�ȴ�ֱ�����ܵ�һ������
		return rURXH0;
    }
    else
    {
		while(!(rUTRSTAT1 & 0x1));		//�ȴ�ֱ�����ܵ�һ������
		return	rURXH1;
    }
}

/****************************************************************************
������˵������鴮�п��Ƿ��յ��ַ������أ�chΪ0��UART0��ȡ��chΪ1��UART1��ȡ
****************************************************************************/
char Uart_GetKey(U8 ch)
{
    if(ch == 0)
    {	    
		if(rUTRSTAT0 & 0x1)		//����յ��ַ��ͷ��ظ��ַ�
    	    return rURXH0;
		else	return 0;		//���û���յ��ַ��ͷ���0
    }
    else
    {
		if(rUTRSTAT1 & 0x1)		//����յ��ַ��ͷ��ظ��ַ�
		    return rURXH1;
		else	return 0;		//���û���յ��ַ��ͷ���0
    }
}

/****************************************************************************
������˵�������пڷ���һ�����ݣ�chΪ0��UART0���ͣ�chΪ1��UART1����
****************************************************************************/
void Uart_SendByte(U8 ch,int data)
{
	if(ch == 0)
	{
		if(data == '\n')
		{
			while(!(rUTRSTAT0 & 0x2));
			
			//�Ķ���ʱʱ��1--10
			Delay(10);		//���ڳ����ն˷�Ӧ��������һ��΢С�ӳ�
			WrUTXH0('\r');
		}
		while(!(rUTRSTAT0 & 0x2));		//�ȴ�֪��THR���
		//�Ķ���ʱʱ��1--10
		Delay(10);
		rUTXH0 = data;
   	}
	else
	{
		if(data == '\n')
		{
			while(!(rUTRSTAT1 & 0x2));
			//�Ķ���ʱʱ��1--10
			Delay(10);		//���ڳ����ն˷�Ӧ��������һ��΢С�ӳ�
			rUTXH1 = '\r';
		}
		while(!(rUTRSTAT1 & 0x2));		//�ȴ�֪��THR���
		//�Ķ���ʱʱ��1--10
		Delay(10);
		rUTXH1 = data;
    }	
}		

/****************************************************************************
������˵�������пڷ���һ���ַ�����chΪ0��UART0���ͣ�chΪ1��UART1����
****************************************************************************/
void Uart_SendString(U8 ch,char *pt)
{
    while(*pt)		Uart_SendByte(ch,*pt++);
}

/****************************************************************************
������˵�����Ա�׼��ʽprintf���пڷ���һ���ַ���
chΪ0��UART0���ͣ�chΪ1��UART1���͡�(�������vsprintf������С�ܶ࣡)
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
������˵����ϵͳ������
****************************************************************************/
void Main(void)
{
	rINTCON=0x00000000;
	rINTMSK=0x07ffffff;
	rINTMOD=0x00000000;		//IRQ��ʽ


	Port_Init();			//IO�˿ڹ��ܡ������趨
//	ChangePllValue(56,2,1);	
	Uart0_Init(0,115200);		//�첽���п�0��ʼ�������ò�����Ϊ115200 
	Uart_Printf(0,"\n��SAMSUNG S3C44B0X  �����塿");
	Uart_Printf(0,"\n��Flash��2M�ֽ� SST39VF160��");
	Uart_Printf(0,"\n��SDRAM��8M�ֽ� HY57V641620HG��");

	USB_Init();		// ��ʼ��USB�豸

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
