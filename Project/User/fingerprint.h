#ifndef _FINGERPRINT_H
#define _FINGERPRINT_H

#include <stdio.h>
#include "stm32f10x.h"

#define FG_UART		USART2

#define TRUE  1
#define FALSE 0

// basic response
#define ACK_SUCCESS       0x00
#define ACK_FAIL          0x01
#define ACK_FULL          0x04
#define ACK_NO_USER	  0x05
#define ACK_TIMEOUT       0x08
#define ACK_GO_OUT	  0x0F

// user info 
#define ACK_ALL_USER      0x00
#define ACK_GUEST_USER 	  0x01
#define ACK_NORMAL_USER   0x02
#define ACK_MASTER_USER   0x03

#define USER_MAX_CNT	40  // set volume max = 1000

// commands
#define CMD_HEAD	0xF5
#define CMD_TAIL	0xF5
#define CMD_ADD_1       0x01
#define CMD_ADD_2       0x02
#define CMD_ADD_3	0x03
#define CMD_MATCH	0x0C
#define CMD_DEL		0x04
#define CMD_DEL_ALL	0x05
#define CMD_USER_CNT    0x09
#define CMD_COM_LEV	0x28
#define CMD_LP_MODE	0x2C
#define CMD_TIMEOUT	0x2E

#define CMD_FINGER_DETECTED 0x14

#define USER_KEY	GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_6)
#define PRESS_KEY	GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_8)
#define DEL_KEY		GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_3)

void TxByte(u8 temp);
u8 GetUserCount(void);
u8 GetcompareLevel(void);
u8 SetcompareLevel(u8 temp);	// compare level ranging 0-9, higher level stricter comparison, default is 5
u8 AddUser(u8 k);
void ClearAllUser(void);
u8 VerifyUser(void);
u8 GetTimeOut(void);

#endif // _FINGERPRINT_H

