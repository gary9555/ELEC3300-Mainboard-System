#include "includes.h"

void Task_lcd(void *p_arg)
{
    (void)p_arg;   
    
    char hex[]="0123456789ABCDEF";
    int  cel, sw, a0, a1, a2, a3=0;
    float num, mv;
	
    while (1){
      
      //  OSTimeDlyHMSM(0, 0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);	//?иои║б└б┴ииии?100ms
      ADC_SoftwareStartConvCmd(ADC1, ENABLE);
      uint16_t value=ADC_GetConversionValue(ADC1);
      
      if((value>246) & (sw!=0))
      {
        GPIOC->BRR = GPIO_Pin_0;
        sw=0;
      }
      else if((value<82) & (sw=0))
      {
        GPIOC->BSRR = GPIO_Pin_0;
        sw=1;
      }
        
      LCD_DrawChar(0x2, 16, 'T');
      LCD_DrawChar(0x2, 16+8, 'E');
      LCD_DrawChar(0x2, 16+16, 'P');
      LCD_DrawChar(0x2, 16+24, ' ');
      LCD_DrawChar(0x2, 16+32, ':');
      
      num= (float)value;
      mv= (num)*(5000.0/4096.0);
      
      cel= (int)(mv);
      
      printf("\r\nCurrent Temperature: %.1f degrees celcius\r\n", mv/10.0);
     
      a3=cel%10;
      cel=cel/10;
      a2=cel%10;
      cel=cel/10;
      a1=cel%10;
      cel=cel/10;
      a0=cel%10;
    
       /*
      a3=value%10;
      value=value/10;
      a2=value%10;
      value=value/10;
      a1=value%10;
      value=value/10;
      a0=value%10;
        */
      LCD_DrawChar(0x2, 16+40, hex[a1]);
      LCD_DrawChar(0x2, 16+48, hex[a2]);
      LCD_DrawChar(0x2, 16+56, '.');
      LCD_DrawChar(0x2, 16+64, hex[a3]);
      LCD_DrawChar(0x2, 16+72, '^');
      LCD_DrawChar(0x2, 16+80, 'C');
  
          
      OSTimeDlyHMSM(0, 0,1,0);
      
      printf("\r\n Hi Professor Tim Woo \r\n");
      OSTimeDlyHMSM(0, 0,1,0);
      printf("\r\n Hi Fox \r\n");
      OSTimeDlyHMSM(0, 0,1,0);
      printf("\r\n Hi Gary \r\n");
      OSTimeDlyHMSM(0, 0,1,0);
       
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