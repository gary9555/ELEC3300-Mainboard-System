/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CURSOR_H
#define __CURSOR_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "lcd.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void twinkle(void);
void up(void);
void down(void);
void left(void);
void right(void);
void set(void);
void Delayms(u32 m);
void print_screen(void);

int detect_cursor(void);
int detect_number(void);
    

#endif /* __CURSOR_H */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/