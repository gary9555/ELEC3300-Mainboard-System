#ifndef _BSP_H_ 
#define _BSP_H_

#include "usart1.h"

void RCC_Init(void);
void SysTick_init(void);
void BSP_Init(void);
void GPIO_Configuration(void);
void RCC_Configuration(void);
void ADC_Configuration(void);
void USART1_Config(void);
void NVIC_Configuration(void);
int fputc(int ch, FILE *f);

#endif