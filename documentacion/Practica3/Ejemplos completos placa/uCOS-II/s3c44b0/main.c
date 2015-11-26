#include	"includes.h"               /* uC/OS interface */
#include	"Sems.h"               /* Semaphore */

//task stack size
#ifdef SEMIHOSTED
	#define	TASK_STACK_SIZE	(64+SEMIHOSTED_STACK_NEEDS)
#else
	#define		TASK_STACK_SIZE	10*1024
#endif

//Task definition

/* allocate memory for tasks' stacks */
#define STACKSIZE 128

/* Global Variable */
unsigned int Stack1[STACKSIZE];
unsigned int Stack2[STACKSIZE];
unsigned int Stack3[STACKSIZE];
unsigned int Stack4[STACKSIZE];
unsigned int StackMain[STACKSIZE];

void Task1(void *Id)
{
    /* print task's id */
    OSSemPend(UART_sem, 0, &err);
    uHALr_printf("  Task%c Called.\n", *(char *)Id);
    OSSemPost(UART_sem);
	while(1)
	{

		led1_on();
		led2_off();
		OSTimeDly(800);
		led1_off();
		led2_on();
		OSTimeDly(800);
	}
}

void Task4(void *Id)
{
	int i;
  INT32U NowTime;

    /* print task's id */
    OSSemPend(UART_sem, 0, &err);
    uHALr_printf("  Task%c Called.\n", *(char *)Id);
    OSSemPost(UART_sem);
	while(1)
	{
       	for(i=0; i<16; i++)
	  {
	    OSSemPend(UART_sem, 0, &err);
	    NowTime=OSTimeGet();  //get time
	    //uHALr_printf("Run Times at:%d\r", NowTime);
	    OSSemPost(UART_sem);
		OSTimeDly(180);
	  }
	}
}
void Task3 (void *Id)
{
    char *Msg;
	int i=0;
    /* print task's id */
    OSSemPend(UART_sem, 0, &err);
    uHALr_printf("  Task%c Called.\n", *(char *)Id);
    OSSemPost(UART_sem);
    while(1)
    {
 		  OSTimeDly(900);

          OSSemPend(UART_sem, 0, &err);
          EV40_rtc_Disp();
          OSSemPost(UART_sem);
	}
}

void Task2 (void *Id)
{
	int value;
    char *Msg;

    /* print task's id */
    OSSemPend(UART_sem, 0, &err);
    uHALr_printf("  Task%c Called.\n\n", *(char *)Id);
    OSSemPost(UART_sem);
    while(1)
    {
	  value = key_read();
	  // display in 8-segment LED
	  if(value > -1)
	    {
		  Digit_Led_Symbol(value);
 		  OSTimeDly(90);
 	    }
  	  OSTimeDly(90);
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

     uHALr_InitTimers();            // enable timer counter interrupt
                
     /*
     * create the tasks in uC/OS and assign decreasing
     * priority to them
     */
     OSTaskCreate(Task1, (void *)&Id1, &Stack1[STACKSIZE - 1], 2);
     OSTaskCreate(Task2, (void *)&Id2, &Stack2[STACKSIZE - 1], 3);
     OSTaskCreate(Task3, (void *)&Id3, &Stack3[STACKSIZE - 1], 4);
     OSTaskCreate(Task4, (void *)&Id4, &Stack4[STACKSIZE - 1], 5);
    
   	 ARMTargetStart();
     // Delete current task
     OSTaskDel(OS_PRIO_SELF);

}

void Main(void)//int argc, char **argv
{
	char Id0 = '4';
	ARMTargetInit();

	/* needed by uC/OS */
	OSInit();
	
	OSTimeSet(0);
	
	/* create the start task */
	OSTaskCreate(TaskStart,(void *)0, &StackMain[STACKSIZE - 1], 0);

	/* start the operating system */
	OSStart();
	
}                              
