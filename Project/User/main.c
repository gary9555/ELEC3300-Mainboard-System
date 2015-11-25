/* Includes ------------------------------------------------------------------*/
#include "includes.h"



static OS_STK startup_task_stk[STARTUP_TASK_STK_SIZE];
void Task_Start(void *p_arg);

int main(void){  
  
  BSP_Init();
  OSInit();
  OSTaskCreate(Task_Start,(void *)0, 
               &startup_task_stk[STARTUP_TASK_STK_SIZE-1], STARTUP_TASK_PRIO); 
  
  OSTimeSet(0);
  OSStart(); 
 
  return 0;

}
