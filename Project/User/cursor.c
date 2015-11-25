/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "lcd.h"
#include "cursor.h"

/** @addtogroup STM32F10x_StdPeriph_Examples
  * @{
  */

/** @addtogroup EXTI_Example
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
  * @brief  This function twinkle the cursor.
  * @param  None
  * @retval None
  */
void twinkle(void)
{
  
}

/**
  * @brief  This function increase the value.
  * @param  None
  * @retval None
  */
void up(void)
{
  char hex[]="0123456789ABCDEF";
  int det=0, cursor_pos=0, rised=0, offset=0;
  det= detect_number();
  
  if (det == 9)
    rised = 0;
  else
    rised = det + 1;
  
  cursor_pos= detect_cursor();  
  if(cursor_pos==1 || cursor_pos==2)
    offset= 8+cursor_pos*8;
  else if (cursor_pos==3 || cursor_pos==4)
    offset= 16+cursor_pos*8;
  
  LCD_EraseChar(2, offset);
  LCD_DrawChar(2, offset, hex[rised]);
}

/**
  * @brief  This function decrease the value.
  * @param  None
  * @retval None
  */

void down(void)
{
  char hex[]="0123456789ABCDEF";
  int det=0, cursor_pos=0, decreased=0, offset=0;
  det= detect_number();
  
  if (det == 0)
    decreased = 9;
  else
    decreased = det - 1;
  
  cursor_pos= detect_cursor();  
  if(cursor_pos==1 || cursor_pos==2)
    offset= 8+cursor_pos*8;
  else if (cursor_pos==3 || cursor_pos==4)
    offset= 16+cursor_pos*8;
  
  LCD_EraseChar(2, offset);
  LCD_DrawChar(2, offset, hex[decreased]);
}


/**
  * @brief  This function erase the cursor.
  * @param  None
  * @retval None
  */
void set(void)
{
  int cursor_pos=0, offset=0, i=0, j=0;
  u8 *p=tempbuffer;
  
    cursor_pos= detect_cursor();  
  if(cursor_pos==1 || cursor_pos==2)
    offset= 8+cursor_pos*8;
  else if (cursor_pos==3 || cursor_pos==4)
    offset= 16+cursor_pos*8;
  
  LCD_EraseLine(33, offset, 33, offset+7);
  LCD_EraseLine(32, offset, 32, offset+7);
  
  
   LCD_Command = Set_Start_Line_X|0x0;
  
    // print out
    for (i=0; i<8; i++)
  {
    // for each page 
    LCD_Command = Set_Page_Addr_X|i; // page no.
    LCD_Command = Set_ColH_Addr_X|0x0; // fixed col first addr
    LCD_Command = Set_ColL_Addr_X|0x0;

    
    j=128;
    while (j--)
    {
      LCD_Data=*p++;
      delay();
    }
  }
  

}

/**
  * @brief  This function move the cursor left.
  * @param  None
  * @retval None
  */
void left(void)
{
  int cursor_pos= 0;
  cursor_pos= detect_cursor();
  if (cursor_pos == 1)
    ;
  else if (cursor_pos == 2)
  {
    LCD_EraseLine(33, 24, 33, 31);
    LCD_EraseLine(32, 24, 32, 31);
    LCD_DrawLine(33, 16, 33, 23);
    LCD_DrawLine(32, 16, 32, 23);
  }
  else if (cursor_pos == 3)
  {
    LCD_EraseLine(33, 40, 33, 47);
    LCD_EraseLine(32, 40, 32, 47);
    LCD_DrawLine(33, 24, 33, 31);
    LCD_DrawLine(32, 24, 32, 31);
  }
  else if (cursor_pos == 4)
  {
    LCD_EraseLine(33, 48, 33, 55);
    LCD_EraseLine(32, 48, 32, 55);
    LCD_DrawLine(33, 40, 33, 47);
    LCD_DrawLine(32, 40, 32, 47);
  }
}

/**
  * @brief  This function move the cursor right.
  * @param  None
  * @retval None
  */
void right(void)
{
  int cursor_pos= 0;
  cursor_pos= detect_cursor();
  if (cursor_pos == 1)
  {
    LCD_EraseLine(33, 16, 33, 23);
    LCD_EraseLine(32, 16, 32, 23);
    LCD_DrawLine(33, 24, 33, 31);
    LCD_DrawLine(32, 24, 32, 31);
  }
  else if (cursor_pos == 2)
  {
    LCD_EraseLine(33, 24, 33, 31);
    LCD_EraseLine(32, 24, 32, 31);
    LCD_DrawLine(33, 40, 33, 47);
    LCD_DrawLine(32, 40, 32, 47);
  }
  else if (cursor_pos == 3)
  {
    LCD_EraseLine(33, 40, 33, 47);
    LCD_EraseLine(32, 40, 32, 47);
    LCD_DrawLine(33, 48, 33, 55);
    LCD_DrawLine(32, 48, 32, 55);
  }
  else if (cursor_pos == 4)
    ;
}

/**
  * @brief  This function detect the cursor.
  * @param  None
  * @retval None
  */
int detect_cursor(void)
{
  if(tempbuffer[128*4+19] & 1)
    return 1;
  else if (tempbuffer[128*4+27] & 1)
    return 2;
  else if (tempbuffer[128*4+43] & 1)
    return 3;
  else if (tempbuffer[128*4+51] & 1)
    return 4;
  else
    return 0;
}

/**
  * @brief  This function detect the number.
  * @param  None
  * @retval None
  */
int detect_number(void)
{
  int cursor_pos= 0, offset= 0;
  cursor_pos= detect_cursor();
  if(cursor_pos==1 || cursor_pos==2)
    offset= 8+cursor_pos*8;
  else if (cursor_pos==3 || cursor_pos==4)
    offset= 16+cursor_pos*8;

  
  if ((tempbuffer[128*2+offset+1] == 0xE0) && (tempbuffer[128*3+offset+6] == 0x0F) && (tempbuffer[128*3+offset+1] == 0x0F))
    return 0;
  else if (tempbuffer[128*3+offset+1] == 0x20)
    return 1;
  else if (tempbuffer[128*3+offset+5] == 0x21)
    return 2;
  else if (tempbuffer[128*2+offset+5] == 0x48)
    return 3;
  else if (tempbuffer[128*3+offset+2] == 0x04)
    return 4;
  else if (tempbuffer[128*3+offset+1] == 0x19)
    return 5;
  else if (tempbuffer[128*2+offset+5] == 0x18)
    return 6;
  else if (tempbuffer[128*2+offset+4] == 0xC8)
    return 7;
  else if (tempbuffer[128*3+offset+4] == 0x21)
    return 8;
  else if (tempbuffer[128*3+offset+2] == 0x31)
    return 9;
  else 
    return 0;
  
}




void print_screen(void)
{
   u8 *p=tempbuffer;
   int i=0, j=0;
  
   LCD_Command = Set_Start_Line_X|0x0;   delay();
  
  
    for (i=0; i<8; i++)
  {
    // for each page 
    LCD_Command = Set_Page_Addr_X|i; // page no.
    LCD_Command = Set_ColH_Addr_X|0x0; // fixed col first addr
    LCD_Command = Set_ColL_Addr_X|0x0;

    
    j=128;
    while (j--)
    {
      LCD_Data=*p++;
      delay();
    }
  }
   
}
