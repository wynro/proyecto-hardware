/*********************************************************************************************
* File£º	main.c
* Author:	embest
* Desc£º	c main entry
* History:	
*********************************************************************************************/

/*--- include files ---*/
#include "44blib.h"
#include "44b.h"
#include "rtc.h"
#include "bmp.h"

extern STRU_BITMAP Stru_Bitmap_gbMouse;
	char yn;
	int input_char=-1;

/*--- function declare ---*/
void Main(void);
void user_input_action(int value);
char User_Getch(void);
void FuncTest_view(int stdio);
void LCD_menu(void);


char * info[][2]={
        " "," Embest S3C44B0X Evaluation Board(S3CEV40)",
		" "," *====  Embest Info&Tech Co.,LTD. ====*",
		" "," Please select test item:",
		" "," 1: 8LED test  2: LCD test    3: Keyboard test",
		" "," 4: Sound test 5: Timer test  6: Ethernet DHCP test",
		" "," 7: Flash test 8: IIC test    9: Ethernet TFTP test",
		" "," 0: TouchScreen test",
		" "," >",
		" ","Note: Please connect UART0(115.2K,8,N,1) to PC COMx",
                0,0};
/*--- function code ---*/
/*********************************************************************************************
* name:		main
* func:		c code entry
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void Main(void)
{
    int RTC_OK,i=0;

    sys_init();        /* Initial 44B0X's Interrupt,Port and UART */
    //_Link();           /* Print Misc info */
	//RTC_OK = check_RTC();
	//if(RTC_OK) Display_Rtc();
	
	/******************/
	/* user interface */
	/******************/
    Lcd_Init();		
	Lcd_Clr(); // clear screen 
	Lcd_Active_Clr();
	Lcd_Dma_Trans();
	BitmapView(180,20,Stru_Bitmap_gbMouse);
		while(1)
		 {  //display menu
			Lcd_DspAscII6x8(10,i*20+50,BLACK,info[i][1]);
			i++;
			if((int)(info[i][0])==0)	break;
		 }
	Lcd_Dma_Trans();
	
	//LCD_menu();
	FuncTest_view(UART);
	init_keyboard();
	
	for(;;)
	 {
		/* get user input */
		input_char = User_Getch();
		user_input_action(input_char);
		Delay(100);
	 }
}

/*********************************************************************************************
* name:		user_input_actionin
* func:		action depend on user input value
* para:		value -- user input value
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void user_input_action(int value)
{
	if(!((value < 0x30)|(value > 0x39)))   Uart_Printf("%x",value-0x30);
	switch(value)
	{
	case '0':
		TS_Test();
		break;	
	case '1':
		Digit_Led_Test();
		break;	
	case '2':
		Uart_Printf("\nLook at LCD ...\n");
		Lcd_Test();
		break;	
	case '3':
		Test_Keyboard();
		break;	
	case '4':
		Test_Iis();
		break;	
	case '5':
		Test_Timer();
		break;	
	case '6':
		Dhcp_Test();
		break;	
	case '7':
		Test_Flash();
		break;	
	case '8':
		Test_Iic();
		break;
	case '9':
		Tftp_Test();
		break;
	case 'A':
	case 'a':
		break;
	case 'B':
	case 'b':
		break;
	case 'T':
	case 't':
		USE_RTC();
	default:
		break;
	}
}

char whichUart = 0;
char User_Getch(void)
{
    int i;
    char fg = 0;
    
    LED8ADDR = 0;
    
    if(whichUart==0)
    {	    
		while(!(rUTRSTAT0 & 0x1)) //Receive data read
		 {
		   if (fg)
		       { led1_off(); led2_on();}
		   else
		       { led1_on(); led2_off();}
		         
		   i++;		        
		   if (i > 10000)
		       { i = 0; fg = ~fg;}

		   if(input_char > -1)
			 {
			   user_input_action(input_char += 0x30);
			/* printf interface */
			   FuncTest_view(UART);
			/*
			   FuncTest_view(LCD);
			   Lcd_DspAscII6x8(20,120+50,BLACK,&input_char); // 
			   Lcd_Dma_Trans();
			*/   
			   input_char = -1;
			 }
			 rINTMSK = ~(BIT_GLOBAL|BIT_EINT1);
		         
		 }
		return RdURXH0();
    }
    else
    {
		while(!(rUTRSTAT1 & 0x1)); //Receive data ready
		return	rURXH1;
    }
}

void FuncTest_view(int stdio)
{
	int i;
	if (stdio == LCD )
	 {
		while(1)
		 {  //display menu
			Lcd_DspAscII6x8(10,i*20+50,BLACK,info[i][1]);
			i++;
			if((int)(info[i][0])==0)	break;
		 }
	 }
	if (stdio == UART )
	 {
		/* printf interface */
		Uart_Printf("\n Embest S3C44B0X Evaluation Board(S3CEV40)\n");
		Uart_Printf("  *==================================*\n");
		Uart_Printf("  *=    Embest Info&Tech Co.,LTD.   =*\n");
		Uart_Printf("  *==================================*\n");
		Uart_Printf("  *==========  R&D CENTER  ==========*\n");
		Uart_Printf("  *=======  86-755-25631365  ========*\n");
		Uart_Printf("  *===== support@embedinfo.com ======*\n");
		Uart_Printf("  *===       Version 2.1          ===*\n");
		Uart_Printf(" Please select test item:\n");
		Uart_Printf("  1: 8LED test\t 2: LCD test\t 3: Keyboard test\n");
		Uart_Printf("  4: Sound test\t 5: Timer test \t 6: Ethernet DHCP test\n");
		Uart_Printf("  7: Flash test\t 8: IIC test \t 9: Ethernet TFTP test\n");
		Uart_Printf("  0: TouchScreen test\n");
		Uart_Printf("  >");
	 }
}

void LCD_menu(void)
{
	BitmapView(180,20,Stru_Bitmap_gbMouse);
	FuncTest_view(LCD);
	Lcd_Dma_Trans();
}

