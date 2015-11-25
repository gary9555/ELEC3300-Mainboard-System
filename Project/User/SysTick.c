/******************** (C) COPYRIGHT 2012 WildFire Team ***************************
 * 文件名  ：SysTick.c
 * 描述    ：SysTick 系统滴答时钟10us中断函数库,中断时间可自由配置，
 *           常用的有 1us 10us 1ms 中断。         
 * 实验平台：野火STM32开发板
 * 硬件连接：-----------------
 *          |                 |
 *          |      无         |
 *          |                 |
 *           -----------------
 * 库版本  ：ST3.5.0
 * 作者    ：wildfire team 
 * 论坛    ：http://www.amobbs.com/forum-1008-1.html
 * 淘宝    ：http://firestm32.taobao.com
**********************************************************************************/
#include "SysTick.h"

static __IO u32 TimingDelay;

/*
 * 函数名：SysTick_Init
 * 描述  ：启动系统滴答定时器 SysTick
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用 
 */
void SysTick_Init(void)
{
    SysTick->CTRL &= 0xfffffffb ; //bit2清0,也就是配置选择外部时钟  
}


/*
 * 函数名：Delay_us
 * 描述  ：us延时程序,10us为一个单位
 * 输入  ：- nTime
 * 输出  ：无
 * 调用  ：Delay_us( 1 ) 则实现的延时为 1 * 10us = 10us
 *       ：外部调用 
 */

void SysTickDelay_us(__IO u32 nTime)
{ 
    u32 temp=0;  
    SysTick->LOAD = nTime*9;  //延时10us的话就是  10*9=90,装到load寄存器中  
    SysTick->VAL=0x00;//计数器清0,因为currrent字段被手动清零时,load将自动重装到VAL中  
    SysTick->CTRL = 0x01;//配置使异常生效,也就是计数器倒数到0时将发出异常通知  
    do  
    {  
       temp = SysTick->CTRL;  //时间到了之后,该位将被硬件置1,但被查询后自动清0  
    }  
    while(temp & 0x01 && !(temp &(1<<16))); //查询  
    SysTick->CTRL = 0x00;  //关闭计数器  
    SysTick->VAL = 0x00;   //清空val
}


/*
 * 函数名：TimingDelay_Decrement
 * 描述  ：获取节拍程序
 * 输入  ：无
 * 输出  ：无
 * 调用  ：在 SysTick 中断函数 SysTick_Handler()调用
 */  
void TimingDelay_Decrement(void)
{
	if (TimingDelay != 0x00)
	{ 
	TimingDelay--;
	}
}
/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/
