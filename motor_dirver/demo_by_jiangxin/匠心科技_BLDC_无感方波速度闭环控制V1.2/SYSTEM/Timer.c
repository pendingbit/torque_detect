//############################################################
// Created on: 2019��5��18��
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//��Ȩ���У�����ؾ�
//STM32������ƿ�����
//���ĿƼ�
//��ַ: https://shop298362997.taobao.com/
//������ƴ������䣺JXKJ_2007@163.com
//############################################################
#include "Timer.h"

void SysTickConfig(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
  if (SysTick_Config(SystemCoreClock /100))   
  {	
    while (1);
  }
  NVIC_SetPriority(SysTick_IRQn, 0x0);
}


