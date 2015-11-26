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

extern int check_count;
/*
*********************************************************************************************************
*                                      Static variables
*********************************************************************************************************
*/
static u8 fpOperation;  // operation for the fingerprint module
static u8 sysState;    //state of the system
static u8 attempts;    // number of incorrect attempts    
static u8 ac_on;        // boolean to control the temperature
static u8 curtain_open;  // boolean to control temperature
static u8 temperature;  // temperature value
static u8 doorStatus;
static u8 num_user;

/*
*********************************************************************************************************
*                                      Task Processes
*********************************************************************************************************
*/
void Task_Start(void *p_arg);
static void Task_Manager(void *p_arg);    // in charge of the states and commands 
static void Task_Update(void *p_arg);
//static void Task_Temp(void *p_arg);       // temperature management task
static void Task_FP(void *p_arg);         // fingerprint
static void Task_Curtain(void *p_arg);    // curtain with lcd ui control
static void Task_LED(void *p_arg);        // breathing light


// stack
static OS_STK task_led_stk[TASK_LED_STK_SIZE];
static OS_STK task_update_stk[TASK_UPDATE_STK_SIZE];
//static OS_STK task_temp_stk[TASK_TEMP_STK_SIZE];
static OS_STK task_fp_stk[TASK_FP_STK_SIZE];
static OS_STK task_curtain_stk[TASK_CURTAIN_STK_SIZE];
static OS_STK task_manager_stk[TASK_MANAGER_STK_SIZE];


// the initial task which starts every other task
void Task_Start(void *p_arg){
  
    (void)p_arg;   
    
    //OS_CPU_SysTickInit(); 
    
     //create the manager for the whole system, it is a state machine
    OSTaskCreate(Task_Manager,(void *)0,  
                 &task_manager_stk[TASK_MANAGER_STK_SIZE-1], TASK_MANAGER_PRIO);
    
    // create task communication between the terminal
    OSTaskCreate(Task_Update,(void *)0,  
                 &task_update_stk[TASK_UPDATE_STK_SIZE-1], TASK_UPDATE_PRIO);
    
    // create task temperature
    //OSTaskCreate(Task_Temp,(void *)0,  
      //           &task_temp_stk[TASK_TEMP_STK_SIZE-1], TASK_TEMP_PRIO);
    
    // create task fingerprint
    OSTaskCreate(Task_FP,(void *)0,  
                 &task_fp_stk[TASK_FP_STK_SIZE-1], TASK_FP_PRIO);
    
    // create task curtain
    OSTaskCreate(Task_Curtain,(void *)0,  
                 &task_curtain_stk[TASK_CURTAIN_STK_SIZE-1], TASK_CURTAIN_PRIO);
 
    // create task led
    OSTaskCreate(Task_LED,(void *)0,  
                 &task_led_stk[TASK_LED_STK_SIZE-1], TASK_LED_PRIO);
    
    
    
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
            if(!strncmp(mRsBuf+4,"gary/123456",11)){
              sysState=LOG_IN;
              printf("#LG#\r\n");
            }
            else{
              attempts++;  
              if(attempts==3){
              //  sysState=SUSPEND;
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
          
          if(!strncmp(mRsBuf,"#CM#",4)){
            if(cmdLength>5){
              switch(mRsBuf[4]){
                // air-con
                case 'A':   
                  if(mRsBuf[5]=='0')
                    ac_on=0;
                  else if(mRsBuf[5]=='1')
                    ac_on=1;
                  else
                    printf("cmd wrong ac\r\n");
                  break;
                // fingerprint
                case 'F':  // fingerprint
                  if(mRsBuf[5]=='1')
                    fpOperation = 1;
                  else if(mRsBuf[5]=='2')
                    fpOperation = 2;
                  else if(mRsBuf[5]=='3')
                    fpOperation = 3;
                  else
                    printf("cmd wrong fp\r\n");
                  break;
                // curtain
                case 'C':  
                  if(mRsBuf[5]=='0')
                    curtain_open=0;
                  else if(mRsBuf[5]=='1')
                    curtain_open=1;
                  else
                    printf("cmd wrong ac\r\n");
                  break;
                default:
                  printf("cmd wrong 2\r\n");
                  break;
              }
            }
          }
          else if(!strncmp(mRsBuf,"#LG#",4))
            printf("Already Logged in\r\n");
          else if(!strncmp(mRsBuf,"#LF#",4)){
            sysState=LOG_OFF;
            printf("#LF#\r\n");
          }
          else{printf("cmd wrong\r\n");}
            
                      
          break;
        case SUSPEND:
          break;
      }
       //for(i=0;i<cmdLength+1;i++)
          //printf("%c",mRsBuf[i]);
          
      
      
      
      cmdReadyFlag=0;
    }
    OSTimeDlyHMSM(0, 0,0,100);
  }

}

// deal with RF communication
void Task_Update(void *p_arg){
   
  (void)p_arg;
  
  while(1){    
    if(sysState==1){
      printf("#IF#T%d\r\n", temperature);
      if(!DOOR_OPEN){
        printf("#IF#D1\r\n");
      }
      else{
        printf("#IF#D0\r\n");
      } 
    }
    printf("#IF#S%d\r\n", sysState);
    OSTimeDlyHMSM(0, 0,2,0);         
  }
}

static void Task_Temp(void *p_arg){
  
  
   (void)p_arg;
   char hex[]="0123456789ABCDEF";
   unsigned short dex[]={0,1,2,3,4,5,6,7,8,9};
   unsigned short val=0, valh=0, vall= 0;
   int sw=0, a0=0, a1=0, a2=0, a3=0, celi=0;
   float cel=0.0, celr= 0.0;
  
   //ds18b20_start();
   
   while(1){
     
     // val = ds18b20_read();
      LCD_EraseScreen();
      LCD_DrawChar(0x2, 16, 'T');
      LCD_DrawChar(0x2, 16+8, 'E');
      LCD_DrawChar(0x2, 16+16, 'P');
      LCD_DrawChar(0x2, 16+24, ' ');
      LCD_DrawChar(0x2, 16+32, ':');
      
      valh= val<<1;
      valh= valh>>9;
      vall= val<<12;
      vall= vall>>12;
      vall= dex[vall];
      cel= valh + vall*0.1;  // real temperature
      celr= cel;
      cel= cel*10;
      celi= (int)cel;
      
      a0=celi%10;
      celi=celi/10;
      a1=celi%10;
      celi=celi/10;
      a2=celi%10;    
      
      /*
      a0=val%10;
      val=val/10;
      a1=val%10;
      val=val/10;
      a2=val%10;
      val=val/10;
      a3=val%10;
      */
      
      LCD_DrawChar(0x2, 16+40, hex[a2]);
      LCD_DrawChar(0x2, 16+48, hex[a1]);
      LCD_DrawChar(0x2, 16+56, '.');
      LCD_DrawChar(0x2, 16+64, hex[a0]);
      LCD_DrawChar(0x2, 16+72, ' ');
      LCD_DrawChar(0x2, 16+80, 'C');
    /*  
      if((celr>30.0) && (sw!=0))
      {
        LED7_OFF;
        sw=0;
      }
      else if((celr<29.0) && (sw==0))
      {
        LED7_ON;
        sw=1;
      }
      else;
      */
      OSTimeDlyHMSM(0, 0,0,200);
   }
  
  
}


// deals with the fingerprint function
void Task_FP(void *p_arg){
  
    (void)p_arg;
#ifdef FP   
    
    
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
                switch(AddUser(num_user+1)){
                    case ACK_SUCCESS:
                        num_user++;
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
                      // unlock , then lock the door after 5 seconds if it is closed
                      DOOR_UNLOCK;
                      OSTimeDlyHMSM(0, 0,5,0);
                      while(!DOOR_OPEN);
                      DOOR_LOCK;
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
              num_user=0;
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

extern TIM_OCInitTypeDef  TIM_OCInitStructure;

void Task_Curtain(void *p_arg){
  
  (void)p_arg;
  char hex[]="0123456789ABCDEF";
  // int cursor_pos = 0, detect= 0;
  int check_count = 0;
  int h1=0, h0=0, min0=0, min1=0;
  
  while(1){ 
   
    if (KEY1){
  
  LCD_DrawChar(2, 16, '0');
  LCD_DrawChar(2, 24, '0');
  LCD_DrawChar(2, 32, ':');
  LCD_DrawChar(2, 40, '0');
  LCD_DrawChar(2, 48, '5');
  LCD_DrawChar(2, 56, ' ');
  LCD_DrawLine(33, 16, 33, 23);
  LCD_DrawLine(32, 16, 32, 23);
  
  TIM_Cmd(TIM4, ENABLE);
  OSTimeDlyHMSM(0, 0,2,0);   
  TIM_Cmd(TIM4, DISABLE);
  
  
    
    
  
  /*       WAKEUP KEY (i.e. PA.0) is pressed.                  */
  
  while(1){
        if((~(GPIOB->IDR>>13))&0x01)  
        {
          while((~(GPIOB->IDR>>13))&0x01);
          left();
        }
        else if ((~(GPIOB->IDR>>12)&0x01))
        {
          while((~(GPIOB->IDR>>12))&0x01);
          right();
        }
        else if ((~(GPIOB->IDR>>14)&0x01))
        {
          while((~(GPIOB->IDR>>14))&0x01);
          up();
        }
        else if ((~(GPIOB->IDR>>15)&0x01))
        {
          while((~(GPIOB->IDR>>15))&0x01);
          down();
        }
        else if ((~(GPIOB->IDR>>11)&0x01))
        {
          while((~(GPIOB->IDR>>11))&0x01);
          set();
          LCD_DrawLine(33, 16, 33, 23);
          LCD_DrawLine(32, 16, 32, 23);
          h1= detect_number();
          right();
          h0= detect_number();
          right();
          min1= detect_number();
          right();
          min0= detect_number();
          set();
          break;
        }
        else{}
        OSTimeDlyHMSM(0, 0,0,500);
  }
        
      
      
      /*
      LCD_DrawChar(5, 16, hex[h1]);
      LCD_DrawChar(5, 24, hex[h0]);
      LCD_DrawChar(5, 32, ':');
      LCD_DrawChar(5, 40, hex[min1]);
      LCD_DrawChar(5, 48, hex[min0]);
      */
      
      //SysTick_Init();
      while(1)
      {
        OSTimeDlyHMSM(0, 0,1,0);   
        if(min0==0)
        {
          min0= 9;
          if(min1==0)
          {
           min1= 5; 
           if(h0==0)
           {
             h0= 9;
             if(h1==0)
             {
               // finish count
               check_count= 1;
               break;
             }
             else
               h1--;
           }
           else
             h0--;
          }
          else
            min1--;
        }
        else
          min0--;
        LCD_EraseChar(2, 48);
        LCD_DrawChar(2, 48, hex[min0]);
        LCD_EraseChar(2, 40);
        LCD_DrawChar(2, 40, hex[min1]);
        LCD_EraseChar(2, 24);
        LCD_DrawChar(2, 24, hex[h0]);
        LCD_EraseChar(2, 16);
        LCD_DrawChar(2, 16, hex[h1]);
      }    
      check_count= 1;
    }
    
      if(check_count){
        LCD_EraseChar(2, 48);
        LCD_EraseChar(2, 40);
        LCD_EraseChar(2, 32);
        LCD_EraseChar(2, 24);
        LCD_EraseChar(2, 16);
        print_screen();
        
        TIM_OCInitStructure.TIM_Pulse = 1; //<- TIMx_CCRx register
        TIM_OC1Init(TIM4, &TIM_OCInitStructure);
        TIM_Cmd(TIM4, ENABLE);
        OSTimeDlyHMSM(0, 0,2,0);
        TIM_Cmd(TIM4, DISABLE);
        
        TIM_OCInitStructure.TIM_Pulse = 3; //<- TIMx_CCRx register
        TIM_OC1Init(TIM4, &TIM_OCInitStructure);
        
        check_count= 0;
      }
      OSTimeDlyHMSM(0, 0,1,0);
  }
   
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