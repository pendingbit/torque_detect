//############################################################
// Created on: 2019��5��18��
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//��Ȩ���У�����ؾ�
//STM32������ƿ�����
//���ĿƼ�
//��ַ: https://shop298362997.taobao.com/
//������ƴ������䣺JXKJ_2007@163.com
//############################################################
#ifndef _STATE_MACHINE_H
#define _STATE_MACHINE_H

#include "stm32f10x.h"
 
typedef struct {
	    uint8_t    Control_Mode;   // ����ģʽ  
	    uint8_t    TripFlagDMC;    // ���� ������־
	    uint8_t    drive_car;      // ��ʼ�������״̬
	    uint8_t    olddrive_car;   // ��ʷ��ʼ�������״̬
	    uint8_t    clear_PWMtripz; // ���������־
	    uint8_t    Run_mode;       // ����ת����״̬
	    uint16_t   Qiehuan_count;  // �л�״̬�ļ���ֵ
	    uint8_t    Start_order;    // ����PWM���������
	    uint16_t   Duty;           // �л�״̬�ļ���ֵ
	    uint16_t   Speed_Count;    // �ٶȻ���ʱ
	    uint16_t   Current_Count;  // ��������ʱ	
			uint16_t   Aim_Speed;
			uint32_t   Aim_Duty;
      uint32_t	 INVERSION;
	    uint32_t	 Temp;
	   	 }State; 

#define  State_DEFAULTS  {0,0,0,0,0,0,0,1,0,0,0,0,0,0,0} // ��ʼ������

void Key_Scanning(void);
void Key_Gpio(void);
void OLED_Display(void);
void RUN(void);
void STOP(void);
void UP(void);
void DOWN(void);
void DIR(void);
#endif
