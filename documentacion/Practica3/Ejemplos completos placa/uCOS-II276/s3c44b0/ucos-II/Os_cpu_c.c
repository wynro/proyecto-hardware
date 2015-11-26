/*
*********************************************************************************************************
*                                               uC/OS-II
*                                         The Real-Time Kernel
*
*                        (c) Copyright 1992-1998, Jean J. Labrosse, Plantation, FL
*                                          All Rights Reserved
*                        (c) Copyright ARM Limited 1999.  All rights reserved.
*
*                                          ARM Specific code
*
*
* File : OS_CPU_C.C
*
* Modification : 21/08/00  EL
*********************************************************************************************************
*/

#define  OS_CPU_GLOBALS
#include "includes.h"
#include "../uhal/44b.h"

/*
*********************************************************************************************************
*                                       OS INITIALIZATION HOOK
*                                            (BEGINNING)
*
* Description: This function is called by OSInit() at the beginning of OSInit().
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts should be disabled during this call.
*********************************************************************************************************
*/
#if OS_CPU_HOOKS_EN > 0 && OS_VERSION > 203
void  OSInitHookBegin (void)
{
}
#endif

/*
*********************************************************************************************************
*                                       OS INITIALIZATION HOOK
*                                               (END)
*
* Description: This function is called by OSInit() at the end of OSInit().
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts should be disabled during this call.
*********************************************************************************************************
*/
#if OS_CPU_HOOKS_EN > 0 && OS_VERSION > 203
void  OSInitHookEnd (void)
{
}
#endif

/*$PAGE*/
/*
*********************************************************************************************************
*                                          TASK CREATION HOOK
*
* Description: This function is called when a task is created.
*
* Arguments  : ptcb   is a pointer to the task control block of the task being created.
*
* Note(s)    : 1) Interrupts are disabled during this call.
*********************************************************************************************************
*/
#if OS_CPU_HOOKS_EN > 0 
void  OSTaskCreateHook (OS_TCB *ptcb)
{
    ptcb = ptcb;                       /* Prevent compiler warning                                     */
}
#endif


/*
*********************************************************************************************************
*                                           TASK DELETION HOOK
*
* Description: This function is called when a task is deleted.
*
* Arguments  : ptcb   is a pointer to the task control block of the task being deleted.
*
* Note(s)    : 1) Interrupts are disabled during this call.
*********************************************************************************************************
*/
#if OS_CPU_HOOKS_EN > 0 
void  OSTaskDelHook (OS_TCB *ptcb)
{
    ptcb = ptcb;                       /* Prevent compiler warning                                     */
}
#endif

/*
*********************************************************************************************************
*                                             IDLE TASK HOOK
*
* Description: This function is called by the idle task.  This hook has been added to allow you to do  
*              such things as STOP the CPU to conserve power.
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts are enabled during this call.
*********************************************************************************************************
*/
#if OS_CPU_HOOKS_EN > 0 && OS_VERSION >= 251
void  OSTaskIdleHook (void)
{
}
#endif

/*
*********************************************************************************************************
*                                           STATISTIC TASK HOOK
*
* Description: This function is called every second by uC/OS-II's statistics task.  This allows your 
*              application to add functionality to the statistics task.
*
* Arguments  : none
*********************************************************************************************************
*/

#if OS_CPU_HOOKS_EN > 0 
void  OSTaskStatHook (void)
{
}
#endif

/*$PAGE*/
/*
*********************************************************************************************************
*                                        INITIALIZE A TASK'S STACK
*
* Description: This function is called by either OSTaskCreate() or OSTaskCreateExt() to initialize the
*              stack frame of the task being created.  This function is highly processor specific.
*
* Arguments  : task          is a pointer to the task code
*
*              pdata         is a pointer to a user supplied data area that will be passed to the task
*                            when the task first executes.
*
*              ptos          is a pointer to the top of stack.  It is assumed that 'ptos' points to
*                            a 'free' entry on the task stack.  If OS_STK_GROWTH is set to 1 then
*                            'ptos' will contain the HIGHEST valid address of the stack.  Similarly, if
*                            OS_STK_GROWTH is set to 0, the 'ptos' will contains the LOWEST valid address
*                            of the stack.
*
*              opt           specifies options that can be used to alter the behavior of OSTaskStkInit().
*                            (see uCOS_II.H for OS_TASK_OPT_???).
*
* Returns    : Always returns the location of the new top-of-stack' once the processor registers have
*              been placed on the stack in the proper order.
*
* Note(s)    : Interrupts are enabled when your task starts executing. You can change this by setting the
*              PSW to 0x0002 instead.  In this case, interrupts would be disabled upon task startup.  The
*              application code would be responsible for enabling interrupts at the beginning of the task
*              code.  You will need to modify OSTaskIdle() and OSTaskStat() so that they enable
*              interrupts.  Failure to do this will make your system crash!
*********************************************************************************************************
*/

#if OS_VERSION >= 251
OS_STK  *OSTaskStkInit (void (*task)(void *pd), void *pdata, OS_STK *ptos, INT16U opt)
#else
void *OSTaskStkInit (void (*task)(void *pd), void *pdata, void *ptos, INT16U opt)
#endif
{
    unsigned int *stk ;

    opt    = opt;                           /* 'opt' is not used, prevent warning                      */
    stk    = (unsigned int *)ptos;          /* Load stack pointer                                      */

    /* build a context for the new task */
    *--stk = (unsigned int) task;       /* pc */
    *--stk = (unsigned int) task;       /* lr */

    *--stk = 0;                         /* r12 */
    *--stk = 0;                         /* r11 */
    *--stk = 0;                         /* r10 */
    *--stk = 0;                         /* r9 */
    *--stk = 0;                         /* r8 */
    *--stk = 0;                         /* r7 */
    *--stk = 0;                         /* r6 */
    *--stk = 0;                         /* r5 */
    *--stk = 0;                         /* r4 */
    *--stk = 0;                         /* r3 */
    *--stk = 0;                         /* r2 */
    *--stk = 0;                         /* r1 */
    *--stk = (unsigned int) pdata;      /* r0 */
    *--stk = (SVC32MODE|0x0);			/* cpsr  IRQ, FIQ disable*/
    *--stk = (SVC32MODE|0x0);			/* spsr  IRQ, FIQ disable */


    return ((void *)stk);
}

/*
*********************************************************************************************************
*                                           OSTCBInit() HOOK
*
* Description: This function is called by OS_TCBInit() after setting up most of the TCB.
*
* Arguments  : ptcb    is a pointer to the TCB of the task being created.
*
* Note(s)    : 1) Interrupts may or may not be ENABLED during this call.
*********************************************************************************************************
*/
#if OS_CPU_HOOKS_EN > 0 && OS_VERSION > 203
void  OSTCBInitHook (OS_TCB *ptcb)
{
    ptcb = ptcb;                                           /* Prevent Compiler warning                 */
}
#endif

/*$PAGE*/
#if OS_CPU_HOOKS_EN

/*
*********************************************************************************************************
*                                           TASK SWITCH HOOK
*
* Description: This function is called when a task switch is performed.  This allows you to perform other
*              operations during a context switch.
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts are disabled during this call.
*              2) It is assumed that the global pointer 'OSTCBHighRdy' points to the TCB of the task that
*                 will be 'switched in' (i.e. the highest priority task) and, 'OSTCBCur' points to the
*                 task being switched out (i.e. the preempted task).
*********************************************************************************************************
*/
void OSTaskSwHook (void)
{
}

/*
*********************************************************************************************************
*                                               TICK HOOK
*
* Description: This function is called every tick.  It is currently unused, therefore commented out
*               in OS_CORE
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts may or may not be ENABLED during this call.
*********************************************************************************************************
*/
void OSTimeTickHook (void)
{
}

#endif

/* Initialize an ARM Target board */
void ARMTargetInit(void)
{
	rSYSCFG=CACHECFG;				// Using 8KB Cache
	Uart_Select(0);	 				// Select UART0
    sys_init();        /* Initial 44B0X's Interrupt,Port and UART */

    /* ---- Tell the world who we are ---------------------------------- */
	// print To UART0
	uHALr_printf("\n\n*=======================================*\n"); 
    uHALr_printf("*=  ENGG*4420 Realtime Systems Design  =*\n"); 
    uHALr_printf("*========= School of Engineering =======*\n"); 
    uHALr_printf("*========= University of Guelph ========*\n"); 
 	uHALr_printf("*=======================================*\n"); 
    uHALr_printf("*======= LAB2: Start-stop Watch ========*\n"); 
    uHALr_printf("*========       July, 2005       =======*\n");
    uHALr_printf("*=======================================*\n\n\n");
    uHALr_printf("uCOS-II is starting.... \n") ;
    /* ---- disable the MMU        ------------------------------------- */
    //uHALr_ResetMMU();

    /* ---- disable interrupts (IRQs) --------------------------------- */
    ARMDisableInt();

    /* Define pre & post-process routines for Interrupt */
    uHALir_DefineIRQ((void *)IrqStart, (void *)IrqFinish, (void *) 0);
    uHALr_InitInterrupts();

    uHALr_InitTimers();
}                               /* targetInit */

/* start the ARM target running */
void ARMTargetStart(void)
{

    /* request the system timer */
    if (uHALr_RequestSystemTimer((void *) OSTimeTick,
                                 (const unsigned char *)"uCOS-II") <= 0)
        uHALr_printf("Timer/IRQ busy\n");
    /* Start system timer & enable the interrupt. */
    uHALr_InstallSystemTimer();
    
    SysENInterrupt(BIT_TIMER0|BIT_EINT4567);

}

extern void IRQContextSwap(void);   /* post DispatchIRQ processing (the _real_ one) */

/* just 'notice' that we need to change context [called in OSIntExit]*/
int need_to_swap_context = 0 ;
void OSIntCtxSw(void) 
{
    need_to_swap_context = 1;
}

/* This is what uCOS does at the start of an IRQ */
void IrqStart(void)
{
    /* increment nesting counter */
    OSIntNesting++;
}

/* This is what uCOS does at the end of an IRQ */
void *IrqFinish(void)
{
	OSIntExit() ;
	
	if (need_to_swap_context) 
	{
		need_to_swap_context = 0 ;
		return ((void *)IRQContextSwap);
	} 
	else 
	{
		return (void *)0;
	}
}
void UserIRQ0(void)
{
	SysDISInterrupt(BIT_GLOBAL);    // avoid interrupt nesting
    rI_ISPC    = BIT_EINT4567;		//clear pending_bit
    rEXTINTPND = 0xf;				//clear EXTINTPND reg.
    		
	//OSIntEnter() ;
	uHALr_printf(" \r");
	//Eint4567Isr();
	SysENInterrupt(BIT_GLOBAL);
	//OSIntExit() ;
	
}
