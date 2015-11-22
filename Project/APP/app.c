#include "includes.h"


/*
*********************************************************************************************************
*                                      Task Processes
*********************************************************************************************************
*/
void Task_Start(void *p_arg);
static void Task_Comm(void *p_arg);
static void Task_LED(void *p_arg);



static OS_STK task_led_stk[TASK_LED_STK_SIZE];
static OS_STK task_comm_stk[TASK_COMM_STK_SIZE];

void Task_Start(void *p_arg)
{
    (void)p_arg;   
    
    //OS_CPU_SysTickInit(); 
    
    // create task communication between the terminal
    OSTaskCreate(Task_Comm,(void *)0,  
                 &task_comm_stk[TASK_COMM_STK_SIZE-1], TASK_COMM_PRIO);
    
    
    // create task led
    OSTaskCreate(Task_LED,(void *)0,  
                 &task_led_stk[TASK_LED_STK_SIZE-1], TASK_LED_PRIO);
    
    while (1){   
      OSTimeDlyHMSM(0, 0,10,0);	// don't need to do anything    
    }
}


void Task_Comm(void *p_arg){
   
  (void)p_arg;
  
  while(1){
    printf("\r\n Hi Professor Tim Woo \r\n");
    OSTimeDlyHMSM(0, 0,1,0);
    printf("\r\n Hi Fox \r\n");
    OSTimeDlyHMSM(0, 0,1,0);
    printf("\r\n Hi Gary \r\n");
    OSTimeDlyHMSM(0, 0,1,0);   
  }
  
  
}


void Task_LED(void *p_arg){
  
  (void)p_arg;
  
  while(1){
    
      GPIOF->BSRR=GPIO_Pin_9;
      OSTimeDlyHMSM(0, 0,0,500);
      GPIOF->BRR=GPIO_Pin_9;  
      OSTimeDlyHMSM(0, 0,0,500);
  }
   
}

/*
*********************************************************************************************************
*********************************************************************************************************
*                                          uC/OS-II APP HOOKS
*********************************************************************************************************
*********************************************************************************************************
*/

#if (OS_APP_HOOKS_EN > 0)
/*
*********************************************************************************************************
*                                      TASK CREATION HOOK (APPLICATION)
*
* Description : This function is called when a task is created.
*
* Argument : ptcb   is a pointer to the task control block of the task being created.
*
* Note     : (1) Interrupts are disabled during this call.
*********************************************************************************************************
*/

void App_TaskCreateHook(OS_TCB* ptcb)
{
}

/*
*********************************************************************************************************
*                                    TASK DELETION HOOK (APPLICATION)
*
* Description : This function is called when a task is deleted.
*
* Argument : ptcb   is a pointer to the task control block of the task being deleted.
*
* Note     : (1) Interrupts are disabled during this call.
*********************************************************************************************************
*/

void App_TaskDelHook(OS_TCB* ptcb)
{
   (void) ptcb;
}

/*
*********************************************************************************************************
*                                      IDLE TASK HOOK (APPLICATION)
*
* Description : This function is called by OSTaskIdleHook(), which is called by the idle task.  This hook
*               has been added to allow you to do such things as STOP the CPU to conserve power.
*
* Argument : none.
*
* Note     : (1) Interrupts are enabled during this call.
*********************************************************************************************************
*/

#if OS_VERSION >= 251
void App_TaskIdleHook(void)
{
}
#endif

/*
*********************************************************************************************************
*                                        STATISTIC TASK HOOK (APPLICATION)
*
* Description : This function is called by OSTaskStatHook(), which is called every second by uC/OS-II's
*               statistics task.  This allows your application to add functionality to the statistics task.
*
* Argument : none.
*********************************************************************************************************
*/

void App_TaskStatHook(void)
{

}

/*
*********************************************************************************************************
*                                        TASK SWITCH HOOK (APPLICATION)
*
* Description : This function is called when a task switch is performed.  This allows you to perform other
*               operations during a context switch.
*
* Argument : none.
*
* Note     : 1 Interrupts are disabled during this call.
*
*            2  It is assumed that the global pointer 'OSTCBHighRdy' points to the TCB of the task that
*                   will be 'switched in' (i.e. the highest priority task) and, 'OSTCBCur' points to the
*                  task being switched out (i.e. the preempted task).
*********************************************************************************************************
*/

#if OS_TASK_SW_HOOK_EN > 0
void App_TaskSwHook(void)
{
}
#endif

/*
*********************************************************************************************************
*                                     OS_TCBInit() HOOK (APPLICATION)
*
* Description : This function is called by OSTCBInitHook(), which is called by OS_TCBInit() after setting
*               up most of the TCB.
*
* Argument : ptcb    is a pointer to the TCB of the task being created.
*
* Note     : (1) Interrupts may or may not be ENABLED during this call.
*********************************************************************************************************
*/

#if OS_VERSION >= 204
void App_TCBInitHook(OS_TCB* ptcb)
{						 
   (void) ptcb;
}
#endif

#endif