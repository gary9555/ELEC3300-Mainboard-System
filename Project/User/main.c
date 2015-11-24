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

int main(void){  
  
  BSP_Init();
  OSInit();
  OSTaskCreate(Task_Start,(void *)0, 
               &startup_task_stk[STARTUP_TASK_STK_SIZE-1], STARTUP_TASK_PRIO); 
  
  OSTimeSet(0);
  OSStart(); 
  return 0;

}
