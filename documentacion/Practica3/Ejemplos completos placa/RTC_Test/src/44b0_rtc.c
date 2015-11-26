//****-------------------------------------------------------------------------
//*				Embest Info&Tech Co.,LTD. 
//*			Technology & Software Support  Team	                   -  Rijing  -
//****-------------------------------------------------------------------------
//+		深圳市英蓓特信息技术有限责任公司
//+		http://www.embedinfo.com http://www.embed.com.cn
//+		Tel: 86-755-25631365     Fax:86-755-25616057
//+		Email:support@embedinfo.com
//****-------------------------------------------------------------------------
//+	Create:		Rijing		Date: 2003.08.25
//+
//+	Modify:		(1)	Rijing		upDate:	2003.08.25
//+				()				upDate: 
//+				()				upDate: 
//+				()				upDate: 
//+
//****-------------------------------------------------------------------------
//+Demo:	
//+			
//****-------------------------------------------------------------------------
#include "..\inc\44blib.h"
#include "..\inc\44b.h"
#include "..\inc\cputest\rtc.h"
extern date[8];
extern void Rtc_Init(void);


int Main()
{

	Port_Init();
	rINTMOD=0x0;
	rINTCON=0x01;
	rINTMSK=~(BIT_GLOBAL | BIT_EINT4567); //use Extern 6 to set time
	                                      //use Extern 7 to set Alarm time
	rPCONG=0xffff;			// EINT7~0
	rPUPG=0x0;				// pull up enable	    
	Uart_Init(0,115200);
	Rtc_Init();
	
		Uart_Printf("\n*==================================*\n"); 
       	Uart_Printf("*=  深圳市英蓓特信息技术有限公司  =*\n"); 
       	Uart_Printf("*==================================*\n"); 
       	Uart_Printf("*========== 产品研发中心 ==========*\n"); 
 		Uart_Printf("*=======  86-755-25631365  ========*\n"); 
       	Uart_Printf("*===== support@embedinfo.com ======*\n"); 
      	Uart_Printf("*===                            ===*\n"); 

    while(1)
    {
    	Display_Rtc();
    }
		return(0);
}
