/* Includes ------------------------------------------------------------------*/
#include "includes.h"

#if 0
char hex[]="0123456789ABCDEF";
int cel, sw, a0, a1, a2, a3=0;
float num, mv;

/* Private typedef -----------------------------------------------------------*/

TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;

/* Private define ------------------------------------------------------------*/
#define ADC1_DR_Address    ((uint32_t)0x4001244C)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
u8 *adcstr = ADCString;
ADC_InitTypeDef ADC_InitStructure;
DMA_InitTypeDef DMA_InitStructure;
__IO uint16_t ADCConvertedValue;
ErrorStatus HSEStartUpStatus;

/* Private function prototypes -----------------------------------------------*/
void RCC_Configuration(void);
void GPIO_Configuration(void);

/* Private functions ---------------------------------------------------------*/
uint32_t dummy;

#endif 

static OS_STK startup_task_stk[STARTUP_TASK_STK_SIZE];
void Task_Start(void *p_arg);

int main(void)
{  
  BSP_Init();
  OSInit();
  OSTaskCreate(Task_Start,(void *)0, 
               &startup_task_stk[STARTUP_TASK_STK_SIZE-1], STARTUP_TASK_PRIO); 
  
  OSTimeSet(0);
  OSStart(); 
  return 0;
  
#if 0
  /* Setup the microcontroller system. Initialize the Embedded Flash Interface,  
     initialize the PLL and update the SystemFrequency variable. */
  SystemInit();
  
  /* Configure all unused GPIO port pins in Analog Input mode (floating input
     trigger OFF), this will reduce the power consumption and increase the device
     immunity against EMI/EMC *************************************************/
  
  /* Enable FSMC, GPIOA, GPIOD, GPIOE, GPIOF, GPIOG and AFIO clocks */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |
                         RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD |
                         RCC_APB2Periph_GPIOE |RCC_APB2Periph_GPIOF | 
                         RCC_APB2Periph_GPIOG, ENABLE);  

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_Init(GPIOF, &GPIO_InitStructure);
  GPIO_Init(GPIOG, &GPIO_InitStructure);  

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |
                         RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD |
                         RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOF | 
                         RCC_APB2Periph_GPIOG, DISABLE);  

  STM3210E_LCD_Init(); 

  /* System clocks configuration ---------------------------------------------*/
  RCC_Configuration();
  /* GPIO configuration ------------------------------------------------------*/
  GPIO_Configuration();
  
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 1;
  ADC_Init(ADC1, &ADC_InitStructure);
  
  /* ADC1 regular channel14 configuration */ 
  ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 1, ADC_SampleTime_55Cycles5);
  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);
  /* Enable ADC1 reset calibaration register */   
  ADC_ResetCalibration(ADC1);
  /* Check the end of ADC1 reset calibration register */
  while(ADC_GetResetCalibrationStatus(ADC1));
  /* Start ADC1 calibaration */
  ADC_StartCalibration(ADC1);
  /* Check the end of ADC1 calibration */
  while(ADC_GetCalibrationStatus(ADC1));
  
  USART_Config(57600,19200);
  NVIC_Configuration();
 // printf("\r\n Hi Professor Tim Woo \r\n");
  //Delayms(429496729);
  //printf("\r\n Hi Fox \r\n");
  //USART1_printf(USART1, "\r\n This is a USART1_printf demo \r\n");
  //USART1_printf(USART1, "\r\n ("__DATE__ " -" __TIME__ ") \r\n");
 
 
  while (1)
  {
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
    
    printf("\r\n Current Temperature: %.1f degrees celcius\r\n", cel/10.0);
   
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

	
    Delayms(200);
    
    printf("\r\n Hi Professor Tim Woo \r\n");
    Delayms(200);
    printf("\r\n Hi Fox \r\n");
    Delayms(200);
    printf("\r\n Hi Gary \r\n");
    Delayms(200);
  } 
   
#endif
}
