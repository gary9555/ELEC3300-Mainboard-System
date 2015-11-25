/**
  ******************************************************************************
  * @file    TIM/Cascade_Synchro/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.1.2
  * @date    09/28/2009
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  */ 

/* Includes ------------------------------------------------------------------*/

#include "includes.h"


/** @addtogroup STM32F10x_StdPeriph_Examples
  * @{
  */

/** @addtogroup TIM_Cascade_Synchro
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{}

/**
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
 

void OSIntEnter(void); 	 
void OSTimeTick(void);	         
void OSIntExit(void);	        

void SysTick_Handler(void)
{
  OSIntEnter(); 	 //  used to count the interrupt nested layer number, number+1
  OSTimeTick();	         //  to record the time 
  OSIntExit();	         //  nested layer-1, activate the task commanding before exiting interrupt
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

extern u8 gRsLength;
extern u8 gRsBuf[9];  // serial receiver buffer for FP


extern u8 mRsLength;
extern u8 mRsBuf[50];   // serial receiver buffer for communication with terminal
static u8 startFlag=0;
static char c; 
extern u8 cmdLength;
extern u8 cmdReadyFlag;

void USART1_IRQHandler(void)
{  	
  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
  { 
    if(!startFlag){
       startFlag=1;
       mRsLength=0;
    }
    c = USART_ReceiveData(USART1);	
    if(c=='\n'){
       startFlag=0;
       cmdLength=mRsLength;
       cmdReadyFlag=1;
    }
    mRsBuf[mRsLength]=c;
    ++mRsLength;
  }
	
}

void USART2_IRQHandler(void)
{
  if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
  {	
    gRsBuf[gRsLength] = USART_ReceiveData(USART2);	
    printf("%c",gRsBuf[gRsLength]);
    ++gRsLength;
  }
}

#if 0
int check_count;
void EXTI0_IRQHandler(void)
{
  char hex[]="0123456789ABCDEF";
  // int cursor_pos = 0, detect= 0;
  int h1=0, h0=0, min0=0, min1=0;
   if(EXTI_GetITStatus(EXTI_Line0) != RESET)
  {

/*       WAKEUP KEY (i.e. PA.0) is pressed.                  */
   
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
    
    while (1)
    {
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
    }  
    
    /*
    LCD_DrawChar(5, 16, hex[h1]);
    LCD_DrawChar(5, 24, hex[h0]);
    LCD_DrawChar(5, 32, ':');
    LCD_DrawChar(5, 40, hex[min1]);
    LCD_DrawChar(5, 48, hex[min0]);
    */
    
    SysTick_Init();
    while(1)
    {
      SysTickDelay_us(1000000-8559);
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
    
    //  LCD_DrawChar(5, 88, 'G');  // represent it finished

   // Clear the Key Button EXTI line pending bit 
    EXTI_ClearITPendingBit(EXTI_Line0);
  }  
}
#endif
/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
