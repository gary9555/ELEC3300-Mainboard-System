#ifndef _BSP_H_ 
#define _BSP_H_

#include "usart1.h"

void RCC_Init(void);
void SysTick_init(void);
void BSP_Init(void);
void GPIO_Configuration(void);
void RCC_Configuration(void);
void ADC_Configuration(void);
void USART_Config(u32 USART1_BaudRate,u32 USART2_BaudRate);
void NVIC_Configuration(void);
int fputc(int ch, FILE *f);

#endif