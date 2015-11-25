/******************** (C) COPYRIGHT 2012 WildFire Team ***************************
 * �ļ���  ��SysTick.c
 * ����    ��SysTick ϵͳ�δ�ʱ��10us�жϺ�����,�ж�ʱ����������ã�
 *           ���õ��� 1us 10us 1ms �жϡ�         
 * ʵ��ƽ̨��Ұ��STM32������
 * Ӳ�����ӣ�-----------------
 *          |                 |
 *          |      ��         |
 *          |                 |
 *           -----------------
 * ��汾  ��ST3.5.0
 * ����    ��wildfire team 
 * ��̳    ��http://www.amobbs.com/forum-1008-1.html
 * �Ա�    ��http://firestm32.taobao.com
**********************************************************************************/
#include "SysTick.h"

static __IO u32 TimingDelay;

/*
 * ��������SysTick_Init
 * ����  ������ϵͳ�δ�ʱ�� SysTick
 * ����  ����
 * ���  ����
 * ����  ���ⲿ���� 
 */
void SysTick_Init(void)
{
    SysTick->CTRL &= 0xfffffffb ; //bit2��0,Ҳ��������ѡ���ⲿʱ��  
}


/*
 * ��������Delay_us
 * ����  ��us��ʱ����,10usΪһ����λ
 * ����  ��- nTime
 * ���  ����
 * ����  ��Delay_us( 1 ) ��ʵ�ֵ���ʱΪ 1 * 10us = 10us
 *       ���ⲿ���� 
 */

void SysTickDelay_us(__IO u32 nTime)
{ 
    u32 temp=0;  
    SysTick->LOAD = nTime*9;  //��ʱ10us�Ļ�����  10*9=90,װ��load�Ĵ�����  
    SysTick->VAL=0x00;//��������0,��Ϊcurrrent�ֶα��ֶ�����ʱ,load���Զ���װ��VAL��  
    SysTick->CTRL = 0x01;//����ʹ�쳣��Ч,Ҳ���Ǽ�����������0ʱ�������쳣֪ͨ  
    do  
    {  
       temp = SysTick->CTRL;  //ʱ�䵽��֮��,��λ����Ӳ����1,������ѯ���Զ���0  
    }  
    while(temp & 0x01 && !(temp &(1<<16))); //��ѯ  
    SysTick->CTRL = 0x00;  //�رռ�����  
    SysTick->VAL = 0x00;   //���val
}


/*
 * ��������TimingDelay_Decrement
 * ����  ����ȡ���ĳ���
 * ����  ����
 * ���  ����
 * ����  ���� SysTick �жϺ��� SysTick_Handler()����
 */  
void TimingDelay_Decrement(void)
{
	if (TimingDelay != 0x00)
	{ 
	TimingDelay--;
	}
}
/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/
