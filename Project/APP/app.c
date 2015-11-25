#include "includes.h"

#define FP

#define LOG_OFF   0
#define LOG_IN    1
#define SUSPEND   2
/*
*********************************************************************************************************
*                                      Global variables
*********************************************************************************************************
*/
u8 mRsLength;
u8 mRsBuf[50];   // serial receiver buffer for communication with terminal
u8 cmdLength;
u8 cmdReadyFlag;     // set to 1 when command is ready to be processed by manager task


/*
*********************************************************************************************************
*                                      Static variables
*********************************************************************************************************
*/
static u8 fpOperation;  // operation for the fingerprint module
static u8 sysState;    //state of the system
static u8 attempts;    // number of incorrect attempts    


/*
*********************************************************************************************************
*                                      Task Processes
*********************************************************************************************************
*/
void Task_Start(void *p_arg);
static void Task_Manager(void *p_arg);    // in charge of the states and commands 
static void Task_Comm(void *p_arg);
static void Task_LED(void *p_arg);        // breathing light
static void Task_FP(void *p_arg);         // fingerprint


static OS_STK task_led_stk[TASK_LED_STK_SIZE];
static OS_STK task_comm_stk[TASK_COMM_STK_SIZE];
static OS_STK task_fp_stk[TASK_FP_STK_SIZE];
static OS_STK task_manager_stk[TASK_MANAGER_STK_SIZE];

// door open
// air conditioner

void Task_Start(void *p_arg){
  
    (void)p_arg;   
    
    //OS_CPU_SysTickInit(); 
    
    // create the manager for the whole system, it is a state machine
    OSTaskCreate(Task_Manager,(void *)0,  
                 &task_manager_stk[TASK_MANAGER_STK_SIZE-1], TASK_MANAGER_PRIO);
    
    // create task communication between the terminal
    OSTaskCreate(Task_Comm,(void *)0,  
                 &task_comm_stk[TASK_COMM_STK_SIZE-1], TASK_COMM_PRIO);
       
    // create task led
    OSTaskCreate(Task_LED,(void *)0,  
                 &task_led_stk[TASK_LED_STK_SIZE-1], TASK_LED_PRIO);
    
    // create task fingerprint
    OSTaskCreate(Task_FP,(void *)0,  
                 &task_fp_stk[TASK_FP_STK_SIZE-1], TASK_FP_PRIO);
 
    
    while (1){   
      OSTimeDlyHMSM(0, 0,10,0);	// don't need to do anything    
    }
}

// takes care of the logic and states of the entire system
void Task_Manager(void *p_arg){

  (void)p_arg;
  u8 i;
  while(1){
    if(cmdReadyFlag){  // if the uart message is ready for check
      switch(sysState){
        case LOG_OFF:
          if(!strncmp(mRsBuf,"#LG#",4)){
            if(!strncmp(mRsBuf,"#LG#gary/123456",15)){
              sysState=LOG_IN;
              printf("Log in success!\r\n");
            }
            else{
              attempts++;  
              if(attempts==3){
                sysState=SUSPEND;
                printf("Suspended...\r\n");
              }
              else
                printf("Username or password incorrect!\r\n");
            }
          }
          else{
            printf("Please Log in first\r\n");
          }
          break;
        case LOG_IN:
          if(!strncmp(mRsBuf,"#LG#",4))
            printf("Already Logged in\r\n");
          break;
        case SUSPEND:
          break;
      }
      
        
      if(mRsBuf[0]=='1')
        fpOperation=1;
      else if(mRsBuf[0]=='2')
        fpOperation=2;
      else if(mRsBuf[0]=='3')
        fpOperation=3;
      else;
        //for(i=0;i<cmdLength+1;i++)
          //printf("%c",mRsBuf[i]);
          
      
      
      
      cmdReadyFlag=0;
    }
    OSTimeDlyHMSM(0, 0,0,100);
  }

}

// deal with RF communication
void Task_Comm(void *p_arg){
   
  (void)p_arg;
  
  while(1){
    /*
    printf("\r\n Hi Professor Tim Woo \r\n");
    OSTimeDlyHMSM(0, 0,1,0);
    printf("\r\n Hi Fox \r\n");
    OSTimeDlyHMSM(0, 0,1,0);
    printf("\r\n Hi Gary \r\n");
    OSTimeDlyHMSM(0, 0,1,0);   
    */
    OSTimeDlyHMSM(0, 0,10,0);
    
  }
}

u8 key_scan(void){
  
    if(KEY2||KEY4||JOY_SEL)
    {
        //delay();
        if(KEY2) return 1;
        if(KEY4) return 2;
        if(JOY_SEL) return 3;
    }
    return 0;
}

// deals with the fingerprint function
void Task_FP(void *p_arg){
  
    (void)p_arg;
#ifdef FP   
    int i = 5;
    
    SetcompareLevel(5);
    printf("Compare level is %d\r\n", GetcompareLevel());
    printf("Current timeout is %d\r\n",GetTimeOut());
    printf("User:%d\r\n",GetUserCount());
    
    while(1)
    {	
        switch(fpOperation)
        {
            case 1:
                printf("\r\nUser:%d\r\n",GetUserCount());
                switch(AddUser(i)){
                    case ACK_SUCCESS:
                        i++;
                        printf("\r\nFingerprint added successfully!\r\n");
                        LED5_ON;	
                        OSTimeDlyHMSM(0, 0,0,500);
                        LED5_OFF;
                        OSTimeDlyHMSM(0, 0,0,500);
                        LED5_ON;	
                        OSTimeDlyHMSM(0, 0,0,500);
                        LED5_OFF;
                        break;                 
                    case ACK_FAIL: 			
                        printf("\r\nOperation Failed.\r\n");
                        LED6_ON;	
                        OSTimeDlyHMSM(0, 0,0,500);
                        LED6_OFF;
                        OSTimeDlyHMSM(0, 0,0,500);
                        LED6_ON;	
                        OSTimeDlyHMSM(0, 0,0,500);
                        LED6_OFF;
                        break;
                    case ACK_FULL:			
                        printf("\r\nDatabase is full.\r\n");
                        LED7_ON;	
                        OSTimeDlyHMSM(0, 0,0,500);
                        LED7_OFF;
                        OSTimeDlyHMSM(0, 0,0,500);
                        LED7_ON;	
                        OSTimeDlyHMSM(0, 0,0,500);
                        LED7_OFF;
                        break;		
                }	
                break;
            case 2:
              switch(VerifyUser()){
                  case ACK_SUCCESS:	
                      printf("\r\nVerification successful!\r\n");
                      LED5_ON;	
                      OSTimeDlyHMSM(0, 0,0,500);
                      LED5_OFF;
                      OSTimeDlyHMSM(0, 0,0,500);
                      LED5_ON;	
                      OSTimeDlyHMSM(0, 0,0,500);
                      LED5_OFF;
                      break;
                  case ACK_NO_USER:
                      printf("\r\nNo such user!\r\n");
                      LED6_ON;	
                      OSTimeDlyHMSM(0, 0,0,500);
                      LED6_OFF;
                      OSTimeDlyHMSM(0, 0,0,500);
                      LED6_ON;	
                      OSTimeDlyHMSM(0, 0,0,500);
                      LED6_OFF;
                      break;
                  case ACK_TIMEOUT:	
                      printf("\r\nTimeout!\r\n");
                      LED7_ON;	
                      OSTimeDlyHMSM(0, 0,0,500);
                      LED7_OFF;
                      OSTimeDlyHMSM(0, 0,0,500);
                      LED7_ON;	
                      OSTimeDlyHMSM(0, 0,0,500);
                      LED7_OFF;
                      break;	
                  case ACK_GO_OUT:
                      printf("\r\nGO OUT\r\n");
                      break;
              };
              break;                                             
            case 3:
              ClearAllUser();
              printf("\r\nAll users have been cleared!\r\n");
              break;
        }
      fpOperation=0;  
      OSTimeDlyHMSM(0, 0,1,0);
    }  
#else 
    while(1){
      OSTimeDlyHMSM(0, 0,1,0);
    }
#endif
}

// a simple breath led
void Task_LED(void *p_arg){
  
  (void)p_arg;
  
  while(1){ 
      LED4_ON;
      OSTimeDlyHMSM(0, 0,0,500);
      LED4_OFF;  
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