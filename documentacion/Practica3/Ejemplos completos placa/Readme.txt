<<<<<<<<<<<<<<< DESCRIPTION OF FILES and DIRECTORiES  >>>>>>>>>>>>>>>>>>>>>

Embest S3CEV40 board test program, all projects are made in Embest IDE for ARM :

	001_asm1         	a single test program of basic
	002_asm2         	a single test program of basic
	003_ThumbCode    	a single test program of basic
	004_ARMMode      	a single test program of basic
	005_c1           	a single test program of basic
	006_c2           	a single test program of basic
	007_explasm      	a single test program of basic
	008_interwork    	a single test program of basic
	8LED_Test        	8-segment test program
	ALL_Test         	The board test program
	ARMcode_Test     	ARM code test program
	ARMMode_Test     	ARM Mode test program
	basic.ews/.pjf/opt     	The basic for ARM development project
	Clock_Test       	The PLL test program
	common           	common files
	DHCP_Test        	Enternet (DHCP) test program
	DMA_Test         	DMA translation test program
	EmbestS3CEV40.cfg	The cfg file for Embest online Flash Programmer for ARM
	Flash_Test       	Program into Flash test program
	IIC_Test         	IIC module test program
	IIS_Test         	IIS module test program
	image            	The image files
	INT_Test         	Extend Interrupt test program
	Keyboard_Test    	4x4 Keyboard test program
	LCD_Test         	LCD usage test program
	LED_Test         	IO usage test program
	Memory_Test      	Memory operation test program
	Network          	The Enternet lib files
	Power_Test       	The CPU work mode test program
	Readme.txt       	This file
	RTC_Test         	RTC module test program
	TFTP_Test        	Enternet (TFTP) test program
	TFTPDown.exe     	The application software for TFTP_Test
	ThumbCode_Test   	ARM Thumb code test program
	Timer_Test       	Timer module test program
	TouchScreen_Test 	Tough screen input test program
	UART_Test        	UART moduletest program
	uCOS-II          	uCOS-II for Embest S3CEV40 test program
	USB_Test         	The USB translation test program

NOTES:
1. Embest S3CEV40 default "Remote Divce" setting as:
      PowerICE ARM7   ---  Embest PowerICE for ARM

2. Please copy the whole files to directory \EmbestIDE\Examples\Samsung\S3CEV40\;
   If you only copy single test program directory to EmbestIDE\Examples\Samsung\MyDir.
   Make sure to copy "common" file in "MyDir" direcroty together). 

3. If you want to copy the test program to another directory, please modify:
   replace ..\..\..\..\ with the installation directory such as C:\EmbestIDE

Program image into FLASH, please reference Notes file (common\ProgramFlash.txt).

!IMPORTANT!  The board only can be debug the interrupt ONLY cerrect running!
  Please program the image\Boardinit.bin* into Flash first when NO code in FLASH.
* Boardinit.bin(just a startup of the CPU, source code is 44binit.s) 
