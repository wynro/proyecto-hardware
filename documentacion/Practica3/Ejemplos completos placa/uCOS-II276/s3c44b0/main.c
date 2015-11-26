#include	"includes.h"               /* uC/OS interface */
#include	"Sems.h"               /* Semaphore */

typedef struct{
int hms;
int secs;
int mins;
}watchTime;

static watchTime ETime;
int msCntr = 0;



#define CLEAR 0
#define COUNT 1
#define STOP 2

int TimeMode = CLEAR;

//task stack size
#ifdef SEMIHOSTED
	#define	TASK_STACK_SIZE	(64+SEMIHOSTED_STACK_NEEDS)
#else
	#define		TASK_STACK_SIZE	10*1024
#endif

//Task definition

/* allocate memory for tasks' stacks */
#define STACKSIZE 1024

/* Global Variable */
unsigned int Stack1[STACKSIZE];
unsigned int Stack2[STACKSIZE];
unsigned int Stack3[STACKSIZE];
unsigned int Stack4[STACKSIZE];
unsigned int StackMain[STACKSIZE];

void Task4(void *Id)
{
    OSSemPend(UART_sem, 0, &err);
    uHALr_printf("  Task%c Called.\n\n", *(char *)Id);
    Uart_Printf(" %02d:%02d:%01d\r",ETime.mins,ETime.secs,ETime.hms);
    OSSemPost(UART_sem);
    
  
	while(1)
	{
	OSSemPend(SwFlag, 0, &err);
	if(TimeMode == CLEAR)
	{
		msCntr = 0;
		TimeMode = COUNT;
	}
	else if(TimeMode == COUNT)
	{
		TimeMode = STOP;
		led1_off();
		led2_off();
	}
	else
	{
		TimeMode = CLEAR;
		led1_on();
		led2_on();
		ETime.hms = 0;
 		ETime.secs = 0;
 		ETime.mins = 0;
 		OSSemPend(UART_sem, 0, &err);
        Uart_Printf(" %02d:%02d:%01d\r",ETime.mins,ETime.secs,ETime.hms);
        OSSemPost(UART_sem);
     }  

	OSTimeDly(40);	
						  
	}
}

void Task1(void *Id)
{

ETime.hms = 0;
ETime.secs = 0;
ETime.mins = 0;


OSSemPend(UART_sem, 0, &err);
uHALr_printf("  Task%c Called.\n", *(char *)Id);
OSSemPost(UART_sem);

while(1)
{
        	
	OSTimeDly(1);
	if(TimeMode == COUNT)
	{
		msCntr++;
		if(msCntr == 100)
		{
			ETime.hms++;
			msCntr = 0;
			if(ETime.hms == 10)
			{
				ETime.secs++;
				ETime.hms = 0;
				
				if(ETime.secs%2 == 0)
				{
				led1_off();
				led2_on();
				}
				else
				{
				led1_on();
				led2_off();
				}
				
				
						
				if(ETime.secs == 60)
				{
					ETime.mins++;
					ETime.secs = 0;
					if(ETime.mins == 100)
					{
						ETime.mins =0;
					}
				}
				
				
			}
			
			OSSemPend(UART_sem, 0, &err);
          	Uart_Printf(" %02d:%02d:%01d\r",ETime.mins,ETime.secs,ETime.hms);
          	OSSemPost(UART_sem);
			
		}
		
	}
	else
	{
		OSTimeDly(20);
	}
	
          
}

	
}



void Task3 (void *Id)
{
	
	char *LCD_char = "00:00:0";
	
	OSSemPend(UART_sem, 0, &err);
    uHALr_printf("  Task%c Called.\n", *(char *)Id);
    OSSemPost(UART_sem);
    
    Lcd_Display(LCD_char);
    
    while(1)
    {
 		OSTimeDly(40);
 		  
 	
		LCD_char[0] =  (ETime.mins/10) + 48;
	    LCD_char[1] =  (ETime.mins%10) + 48;
	    
	    LCD_char[3] =  (ETime.secs/10) + 48;
	    LCD_char[4] =  (ETime.secs%10) + 48;
	    
	    LCD_char[6] =  ETime.hms + 48;
	    

        LCD_view(125,125,0xFF,LCD_char);
        //Lcd_Display(LCD_char);
	}
}

void Task2 (void *Id)
{
	int value;
    char *Msg;

    /* print task's id */
    OSSemPend(UART_sem, 0, &err);
    uHALr_printf("  Task%c Called.\n", *(char *)Id);
    OSSemPost(UART_sem);
    while(1)
    {
	  value = key_read();
	  // display in 8-segment LED
	  if(value > -1)
	    {
		  Digit_Led_Symbol(value);
		  
		  if(value == 0)
		  {
		  	OSSemPost(SwFlag);
		  }
		  	 
 		  OSTimeDly(300);
 	    }
  	  OSTimeDly(50);
	}
}

void TaskStart (void *i)
{

     char Id1 = '1';
     char Id2 = '2';
     char Id3 = '3';
     char Id4 = '4';
     
    /* 
     * create the first Semaphore in the pipeline with 1 
     * to get the task started.
     */
     UART_sem = OSSemCreate(1);
     SwFlag = OSSemCreate(0);

     uHALr_InitTimers();            // enable timer counter interrupt
                
     /*
     * create the tasks in uC/OS and assign decreasing
     * priority to them
     */
     OSTaskCreate(Task1, (void *)&Id1, &Stack1[STACKSIZE - 1], 2);
     OSTaskCreate(Task2, (void *)&Id2, &Stack2[STACKSIZE - 1], 6);
     OSTaskCreate(Task3, (void *)&Id3, &Stack3[STACKSIZE - 1], 8);
     OSTaskCreate(Task4, (void *)&Id4, &Stack4[STACKSIZE - 1], 10);
    
   	 ARMTargetStart();
     // Delete current task
     OSTaskDel(OS_PRIO_SELF);

}

void Main(void)//int argc, char **argv
{
	char Id0 = '0';
	ARMTargetInit();
	
	Lcd_Init();

	/* needed by uC/OS */
	OSInit();
	
	//Lcd_Display_0();
	
    uHALr_printf("*===       OS Version:%d       ===*\n\r",OSVersion()); 
	OSTimeSet(0);
	
	/* create the start task */
	OSTaskCreate(TaskStart,(void *)&Id0, &StackMain[STACKSIZE - 1], 0);

	/* start the operating system */
	OSStart();
	
}                              
