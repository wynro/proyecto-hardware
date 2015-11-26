/*
*********************************************************************************************************
*                                               uC/OS-II
*                                        The Real-Time Kernel
*
*                         (c) Copyright 1992-2002, Jean J. Labrosse, Weston, FL
*                                          All Rights Reserved
*
* File : OS_CPU_C.C
* By   : Jean J. Labrosse
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                               ARM Port
*
*                 Target           : ARM (Includes ARM7, ARM9)
*                 Ported by        : Michael Anburaj
*                 URL              : http://geocities.com/michaelanburaj/    Email : michaelanburaj@hotmail.com
*
*********************************************************************************************************
*/

#define  OS_CPU_GLOBALS
#include "includes.h"
#include "44blib.h"
#include "44b.h"
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
* Note(s)    : Interrupts are enabled when your task starts executing. Also that the tasks run in SVC
*              mode.
*********************************************************************************************************
*/

OS_STK *OSTaskStkInit (void (*task)(void *pd), void *pdata, OS_STK *ptos, INT16U opt)
{
    OS_STK *stk;


    opt    = opt;                           /* 'opt' is not used, prevent warning                      */
    stk    = ptos;                          /* Load stack pointer                                      */
    *(stk) = (OS_STK)task;                  /* Entry Point                                             */
    *(--stk) = (INT32U)0;                   /* lr                                                      */
    *(--stk) = (INT32U)0;                   /* r12                                                     */
    *(--stk) = (INT32U)0;                   /* r11                                                     */
    *(--stk) = (INT32U)0;                   /* r10                                                     */
    *(--stk) = (INT32U)0;                   /* r9                                                      */
    *(--stk) = (INT32U)0;                   /* r8                                                      */
    *(--stk) = (INT32U)0;                   /* r7                                                      */
    *(--stk) = (INT32U)0;                   /* r6                                                      */
    *(--stk) = (INT32U)0;                   /* r5                                                      */
    *(--stk) = (INT32U)0;                   /* r4                                                      */
    *(--stk) = (INT32U)0;                   /* r3                                                      */
    *(--stk) = (INT32U)0;                   /* r2                                                      */
    *(--stk) = (INT32U)0;                   /* r1                                                      */
    *(--stk) = (INT32U)pdata;               /* r0 : argument                                           */
    *(--stk) = (INT32U)(SVC32MODE|0x0);       /* PSR                                                     */
    *(--stk) = (INT32U)(SVC32MODE|0x0);       /* SPSR                                                    */

        return (stk);
}

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
#if (OS_CPU_HOOKS_EN > 0) && (OS_TASK_SW_HOOK_EN > 0)
void  OSTaskSwHook (void)
{
}
#endif

/*
*********************************************************************************************************
*                                           OS_TCBInit() HOOK
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


/*
*********************************************************************************************************
*                                               TICK HOOK
*
* Description: This function is called every tick.
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts may or may not be ENABLED during this call.
*********************************************************************************************************
*/
#if (OS_CPU_HOOKS_EN > 0) && (OS_TIME_TICK_HOOK_EN > 0)
void  OSTimeTickHook (void)
{
}
#endif


INT32U        OSIntCtxSwFlag = 0;           /* Used to flag a context switch                           */

/* Initialize an ARM Target board */
void ARMTargetInit(void)
{
    sys_init();        /* Initial 44B0X's Interrupt,Port and UART */

	/* ---------------- print To UART0 ---------------- */
	uHALr_printf("\n Embest S3C44B0X Evaluation Board(S3CEV40)\n");
	uHALr_printf("  *==================================*\n");
	uHALr_printf("  *=    Embest Info&Tech Co.,LTD.   =*\n");
	uHALr_printf("  *==================================*\n");
	uHALr_printf("  *==========  R&D CENTER  ==========*\n");
	uHALr_printf("  *=======  86-755-25631365  ========*\n");
    uHALr_printf("*===== support@embedinfo.com ======*\n"); 
	uHALr_printf(" uCOS-II Running... \n") ;

    /* ---------------- interrupts DEMO handler initialization ---------------- */
    uHALr_InitInterrupts();

    uHALr_InitTimers();
}                               /* targetInit */

//timer interrupt handler routine
extern void TickHandler(void);
extern void Handle_Eint1(void);
extern void Handle_Eint4567(void);

/* start the ARM target running */
void ARMTargetStart(void)
{

    /* request the system timer */
    if (uHALr_RequestSystemTimer((void *) OSTimeTick,
                                 (const unsigned char *)"uCOS-II") <= 0)
        uHALr_printf("Timer/IRQ busy\n");
// New Way        
    //RegisterIrqHandler(HandlerTIMER0, (void *) TickHandler);
    //RegisterIrqHandler(HandlerEINT1, (void *) Handle_Eint1);
    //RegisterIrqHandler(HandlerEINT4567, (void *) Handle_Eint4567);
    
    //SysENInterrupt(BIT_TIMER0|BIT_EINT4567);//|BIT_EINT1

// Old Method    
    /* request the system timer */
    if (uHALr_RequestSystemTimer((void *) OSTimeTick,
                                 (const unsigned char *)"uCOS-II") <= 0)
        uHALr_printf("Timer/IRQ busy\n");
    /* Start system timer & enable the interrupt. */
    uHALr_InstallSystemTimer();
    
    SysENInterrupt(BIT_TIMER0|BIT_EINT4567);
}

void UserIRQ_Eint4567(void)
{
    OSSemPend(UART_sem, 0, 0);
    Uart_Printf(" SB2 or SB3 had pressed...\r");
    OSSemPost(UART_sem);
	Eint4567Isr();
}
void UserIRQ_Eint1(void)
{
	KeyboardInt();
}

void Timer0Interrupt()
{
	rI_ISPC=BIT_TIMER0;                  // Clear pending bit -- Watchdog timer
	OSTimeTick();
}

