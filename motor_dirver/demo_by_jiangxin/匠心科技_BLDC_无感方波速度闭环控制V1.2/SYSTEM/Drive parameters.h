//############################################################
// Created on: 2019��5��18��
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//��Ȩ���У�����ؾ�
//STM32������ƿ�����
//���ĿƼ�
//��ַ: https://shop298362997.taobao.com/
//������ƴ������䣺JXKJ_2007@163.com
//############################################################
#ifndef __DRIVE_PARAMETERS_H
#define __DRIVE_PARAMETERS_H

#define TEST_MANUELL                    0

//������ʽ
//#define  HALL

//�޻�����ʽ
#define  HALLLESS

//1:������2���ٶȻ���3�ٶȻ�����������
#define  LOOP                           2 

//PWMƵ�ʣ���λKHZ;
#define  PWM_FREQ                      ((u16) 18) 

//����Ŀ��ת�أ������������� 
#define  Torque                         0x00FF

//Hallless �˲�����
#define  FILTER_LONG                    0xFFFF 

//�ٶ�PID
#define  Speed_Kp                       0.05 
#define  Speed_Ki                       0.01 
#define  Speed_Kc                       0.01 

//����PID
#define  Current_Kp                     2 
#define  Current_Ki                     0.2
#define  Current_Kc                     0.01 

//H_PWM_L_PWM����ģʽ
//#define  H_PWM_L_PWM

//H_PWM_L_ON����ģʽ
#define  H_PWM_L_ON

#endif
