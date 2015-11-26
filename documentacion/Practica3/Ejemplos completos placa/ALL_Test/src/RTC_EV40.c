//****-------------------------------------------------------------------------
//*				Embest Info&Tech Co.,LTD. 
//*			Technology & Software Support  Team	                   -  Rijing  -
//****-------------------------------------------------------------------------
//+		Embest Info and Tech Co.,Ltd.
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
#include "44blib.h"
#include "44b.h"
#include "rtc.h"

int USE_RTC(void);
int check_RTC(void);

int check_RTC(void)
{
    int RTC_alr = 0;
/*   //check RTC code        
    char yn = 0x59;
   
    while((yn ==0x0d)|(yn ==0x59)|(yn ==0x79)|(RTC_alr ==0))
     {
       Uart_Printf("\n RTC Check(Y/N)? ");
       
       yn = Uart_Getch();
       if((yn == 0x4E)|(yn == 0x6E)|(yn == 0x59)|(yn == 0x79))  Uart_SendByte(yn);
       if((yn == 0x0d)|(yn == 0x59)|(yn == 0x79))
         {
           RTC_alr = Test_Rtc_Alarm();
           Display_Rtc();
         }
       else break;
       if (RTC_alr) break;
     }
*/
    return Test_Rtc_Alarm();
}
    
int USE_RTC(void)
{
	char yn,tmp,i,N09=1;
	char num0 = 0x30;//"0";
	char num9 = 0x39;//"9";
	char schar[] ={0,'-',' ',':'};
	char sDATE[12];//xxxx-xx-xx x
	char sTIME[8];//xx:xx:xx

	if(check_RTC())
     {      
	  Uart_Printf("\n RTC Working now. To set time(Y/N)? ");
	          
      yn = Uart_Getch();
      if((yn == 0x4E)|(yn == 0x6E)|(yn == 0x59)|(yn == 0x79))  Uart_SendByte(yn);
      if((yn == 0x0d)|(yn == 0x59)|(yn == 0x79))  //want to set time?
       {
///////////////////////////////////////////////////////////////////////////////////         
        do{
          Uart_Printf("\nCurrent day is (%04x,%02x,%02x, %s). To set day(yy-mm-dd w): "\
                       ,year,month,day,date[weekday]);
  
          Uart_GetString(sDATE);  
          if(sDATE[0] == 0x32) 
           {
            if((sDATE[4] == schar[1] )&(sDATE[7] == schar[1] )&(sDATE[10] == schar[2] ))
             {
               if((sDATE[11] >0)|(sDATE[11] <8))
                {
                  i=0; N09 = 0;
                  while(i<12)
                   {
                     if((i !=4)|(i !=7)|(i !=10))
                      {
                        if((sDATE[i] < num0 )&(sDATE[i] > num9))
                         { N09 = 1; 
                           break;  }
                      }
                     i++;
                   }
                  if(N09 == 0)
                    break;//all right
                }        // if date 1 - 7
              }         // if "-" or " "
           }           // if 32 (21th century)
          
          N09 = 1;
          Uart_Printf("\n Wrong value!!  To set again(Y/N)? ");        
          
          yn = Uart_Getch();  //want to set DATE again?
          if((yn == 0x4E)|(yn == 0x6E)|(yn == 0x59)|(yn == 0x79))  Uart_SendByte(yn);
         }while((yn == 0x0d)|(yn == 0x59)|(yn == 0x79));

        if(N09 ==0)
         {
          rRTCCON  = 0x01;	       // R/W enable, 1/32768, Normal(merge), No reset
          rBCDYEAR = ((sDATE[2]<<4)|0x0f)&(sDATE[3]|0xf0);//->syear;
          rBCDMON  = ((sDATE[5]<<4)|0x0f)&(sDATE[6]|0xf0);//->smonth;
          rBCDDAY  = ((sDATE[8]<<4)|0x0f)&(sDATE[9]|0xf0);//->sday;
          
          tmp      = ((sDATE[11]&0x0f)+1);
          if(tmp ==8) rBCDDATE = 1;// SUN:1 MON:2 TUE:3 WED:4 THU:5 FRI:6 SAT:7
          else        rBCDDATE = tmp;
          
          rRTCCON  = 0x00;	       // R/W disable
          
         }else Uart_Printf("\n\n Use Current DATE Settings.\n");

///////////////////////////////////////////////////////////////////////////////////   
        do{
          Uart_Printf("\nCurrent time is (%02x:%02x:%02x). To set time(hh:mm:ss): "\
                       ,hour,min,sec);
          Uart_GetString(sTIME);
          
            if((sTIME[2] == schar[3] )&(sTIME[5] == schar[3]))
             {
               i=0; N09 = 0;
               while(i<8)
                {
                 if((i !=2)|(i !=5))
                  {
                   if((sTIME[i] < num0 )&(sTIME[i] > num9))
                    { N09 = 1;
                      break;  }
                  }
                 i++;
                }
               if(N09 == 0)
		          {
		          tmp = ((sTIME[0]<<4)|0x0f)&(sTIME[1]|0xf0);
		          if((tmp >0)&(tmp <0x24))          
		           { 
		            sTIME[2] = tmp;//->shour;
		            
		            tmp  = ((sTIME[3]<<4)|0x0f)&(sTIME[4]|0xf0);          
		            if(tmp <=0x59)          
		             {
		               sTIME[5] = tmp;//->smin;
		               
		               tmp  = ((sTIME[6]<<4)|0x0f)&(sTIME[7]|0xf0);
		               if(tmp <=0x59) 
		                 break;//all right
		              }       //if min < 59
		            }        //if 0 < hour < 24
		          }         //if num 0-9
		                                             
             }
          N09 = 1;
          Uart_Printf("\n Wrong value!!  To set again(Y/N)? ");        
        
          yn = Uart_Getch();  //want to set Time again?
          if((yn == 0x4E)|(yn == 0x6E)|(yn == 0x59)|(yn == 0x79))  Uart_SendByte(yn);
         }while((yn == 0x0d)|(yn == 0x59)|(yn == 0x79));

        if(N09 ==0)
         {
          rRTCCON  = 0x01;	   // R/W enable, 1/32768, Normal(merge), No reset
          rBCDHOUR = sTIME[2]; //->shour;     
          rBCDMIN  = sTIME[5]; //->smin;
          rBCDSEC  = ((sTIME[6]<<4)|0x0f)&(sTIME[7]|0xf0); //->ssec;
          rRTCCON  = 0x00;	   // R/W disable
          
         }else Uart_Printf("\n\n Use Current TIME Settings.\n");
        
      }else{
      
          Uart_Printf("\n Use Current Settings...\n");
          return 1;
      } /* end if want to set? */
     }else{
     
          Uart_Printf("\n Please check RTC or maybe it's Wrong. \n");
          return 0;
      } /* end if(check_RTC) */
     
}
