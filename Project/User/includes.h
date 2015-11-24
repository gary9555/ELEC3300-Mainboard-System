#ifndef _INCLUDES_H_
#define _INCLUDES_H_



#define LED4_ON	    GPIO_SetBits(GPIOF, GPIO_Pin_9)
#define LED4_OFF    GPIO_ResetBits(GPIOF, GPIO_Pin_9)

#define LED5_ON	    GPIO_SetBits(GPIOF, GPIO_Pin_8)
#define LED5_OFF    GPIO_ResetBits(GPIOF, GPIO_Pin_8)

#define LED6_ON	    GPIO_SetBits(GPIOF, GPIO_Pin_7)
#define LED6_OFF    GPIO_ResetBits(GPIOF, GPIO_Pin_7)

#define LED7_ON	    GPIO_SetBits(GPIOF, GPIO_Pin_6)
#define LED7_OFF    GPIO_ResetBits(GPIOF, GPIO_Pin_6) 

#define KEY1        GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)
#define KEY2        !GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13)
#define KEY4        !GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10)

#define JOY_SEL     !GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11)
#define JOY_RIGHT   !GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)
#define JOY_LEFT    !GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13)
#define JOY_UP      !GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14)
#define JOY_DOWN    !GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15)

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"

#include "ucos_ii.h"

#include "bsp.h"
#include "app.h"

#include "lcd.h"
#include "stm32f10x.h"
#include "stm32f10x_conf.h"
//#include "usart1.h"
#include "lcd.h"
#include "misc.h"
#include "stm32f10x_usart.h"
#include "fingerprint.h"


#endif  // _INCLUDES_H_