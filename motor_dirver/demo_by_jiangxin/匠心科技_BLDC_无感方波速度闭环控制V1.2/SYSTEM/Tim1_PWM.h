//############################################################
// Created on: 2019��5��18��
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//��Ȩ���У�����ؾ�
//STM32������ƿ�����
//���ĿƼ�
//��ַ: https://shop298362997.taobao.com/
//������ƴ������䣺JXKJ_2007@163.com
//############################################################
#ifndef _Tim1_PWM_H
#define _Tim1_PWM_H 
#include "stm32f10x.h"

void Tim1_PWM_Init(void);
void TIM1_Gpio(void);
void Start_Motor(void);
void Stop_Motor(void);
void MOS_Q15PWM(void) ;    // �л�Q15MOS����
void MOS_Q16PWM(void) ;    // �л�Q16MOS����
void MOS_Q26PWM(void) ;    // �л�Q26MOS����
void MOS_Q24PWM(void) ;    // �л�Q24MOS����
void MOS_Q34PWM(void) ;    // �л�Q34MOS����
void MOS_Q35PWM(void) ;    // �л�Q35MOS����

#endif
