//############################################################
// Created on: 2019��5��18��
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//��Ȩ���У�����ؾ�
//STM32������ƿ�����
//���ĿƼ�
//��ַ: https://shop298362997.taobao.com/
//������ƴ������䣺JXKJ_2007@163.com
//############################################################
#include "PI_Cale.h"
#include "Drive parameters.h"

extern    PIDREG_T       pi_spd;
extern    PIDREG_T       pi_ICurr;

void PID_init(void)
{
  pi_spd.Kp = Speed_Kp;    
  pi_spd.Ki = Speed_Ki;  
	pi_spd.Kc = Speed_Kc;
  pi_spd.OutMax =4000;
#ifdef HALLLESS	
  pi_spd.OutMin =500;
#endif	
#ifdef HALL
  pi_spd.OutMin =500;
#endif	
	pi_spd.Ref = 0;

  pi_ICurr.Kp = Current_Kp;    
  pi_ICurr.Ki = Current_Ki;  
	pi_ICurr.Kc = Current_Kc;
  pi_ICurr.OutMax =4000; 
  pi_ICurr.OutMin =500;
	pi_ICurr.Ref = 0;	
	
 }


