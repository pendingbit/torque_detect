//############################################################
// Created on: 2019��5��18��
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//��Ȩ���У�����ؾ�
//STM32������ƿ�����
//���ĿƼ�
//��ַ: https://shop298362997.taobao.com/
//������ƴ������䣺JXKJ_2007@163.com
//############################################################
#include "Hall.h"
#include "State_Machine.h"
#include "Tim1_PWM.h"
#include "Control stage parameters.h"
#include "Drive parameters.h"
#include "PMSM motor parameters.h"
#include "PI_Cale.h"

#ifdef HALL

extern    Hall       Hall_Three;
extern    State      StateContr;

void ThreeHallPara_init(void)
{
   Hall_Three.Poles = MOTOR_POLES;  
	 Hall_Three.speed_coeff = 18000 * 60 / Hall_Three.Poles / 2 ;  
}

unsigned char UHALL_Edge( unsigned char val)
{
	static  unsigned char oldval=0;
	
	if(oldval != val)
	{
		oldval = val;
		if(val == 0) return 0;  
		else return 1;			
	}
	return 2;  
}

void Hall_SW(void)  //����������
{
	if (HALL_U_STATUS==1)
		{		
		Hall_Three.HallUVW[0]=1;
		}
	else
		{
		Hall_Three.HallUVW[0]=0; 
		}
	if (HALL_V_STATUS==1)
		{			 
		Hall_Three.HallUVW[1]=1;
		}
	else
		{
		Hall_Three.HallUVW[1]=0; 
		}
	if (HALL_W_STATUS==1) 
		{
		Hall_Three.HallUVW[2]=1;
		}
	else
		{
		Hall_Three.HallUVW[2]=0;  
		}
		
	Hall_Three.Filter_Edge = UHALL_Edge(Hall_Three.HallUVW[0]);
		{
			if(Hall_Three.Filter_Edge == 1)
			{
				FirstOrder_LPF_Cacl(Hall_Three.Filter_Count,Hall_Three.Filter_CountF,0.1379);
				Hall_Three.Speed_RPM = Hall_Three.speed_coeff/Hall_Three.Filter_CountF;
				FirstOrder_LPF_Cacl(Hall_Three.Speed_RPM,Hall_Three.Speed_RPMF,0.2379);
				Hall_Three.Filter_Count = 0;
			}
			if(Hall_Three.Filter_Edge == 0)
			{
				Hall_Three.Filter_Count = 0;
			}
			if(Hall_Three.Filter_Edge == 2)
			{
				Hall_Three.Filter_Count++;
			}
		}
			
	Hall_Three.Hall_State = Hall_Three.HallUVW[0] +(Hall_Three.HallUVW[1]<<1) +(Hall_Three.HallUVW[2]<<2);
	if(!StateContr.INVERSION)Hall_Three.Hall_State=7-Hall_Three.Hall_State; 
	if(Hall_Three.Hall_State!=Hall_Three.OldHall_State)
	{
#ifdef H_PWM_L_PWM
	switch(Hall_Three.Hall_State)
	{
		case  0x5:     
		{
		MOS_Q15PWM();//A+B-
		}
		break;
		
		case  0x1:    
		{
		MOS_Q16PWM();//A+C-
		}
		break;
		
		case  0x3:    
		{
		MOS_Q26PWM();//B+C-
		}
		break;
		
		case  0x2:  
		{
		MOS_Q24PWM();//B+A-
		}
		break;
		
		case  0x6:   
		{
		MOS_Q34PWM();//C+A-
		}
		break;

		case  0x4:
		{
		MOS_Q35PWM();//C+B-
		}
		break;
	
		default:
		{
		Stop_Motor();
		Hall_Three.Speed_RPM=0;
		}
		break;
	}
#endif

#ifdef H_PWM_L_ON
	switch(Hall_Three.Hall_State)
	{
		case  0x5:     
		{
		MOS_Q15PWM();//A+B-
		}
		break;
		
		case  0x1:    
		{
		MOS_Q16PWM();//A+C-
		}
		break;
		
		case  0x3:    
		{
		MOS_Q26PWM();//B+C-
		}
		break;
		
		case  0x2:  
		{
		MOS_Q24PWM();//B+A-
		}
		break;
		
		case  0x6:   
		{
		MOS_Q34PWM();//C+A-
		}
		break;

		case  0x4:
		{
		MOS_Q35PWM();//C+B-
		}
		break;
	
		default:
		{
		Stop_Motor();
		Hall_Three.Speed_RPM=0;
		}
		break;
	}
#endif
}	
	if(Hall_Three.OldHall_State==Hall_Three.Hall_State)
	{		
#ifdef H_PWM_L_PWM
	switch(Hall_Three.Hall_State)
	{
		case  0x5:     
		{
		MOS_Q15PWM();//A+B-
		}
		break;
		
		case  0x1:    
		{
		MOS_Q16PWM();//A+C-
		}
		break;
		
		case  0x3:    
		{
		MOS_Q26PWM();//B+C-
		}
		break;
		
		case  0x2:  
		{
		MOS_Q24PWM();//B+A-
		}
		break;
		
		case  0x6:   
		{
		MOS_Q34PWM();//C+A-
		}
		break;

		case  0x4:
		{
		MOS_Q35PWM();//C+B-
		}
		break;
	
		default:
		{
		Stop_Motor();
		Hall_Three.Speed_RPM=0;
		}
		break;
	}
#endif

#ifdef H_PWM_L_ON
	switch(Hall_Three.Hall_State)
	{
		case  0x5:     
		{
		MOS_Q15PWM();//A+B-
		}
		break;
		
		case  0x1:    
		{
		MOS_Q16PWM();//A+C-
		}
		break;
		
		case  0x3:    
		{
		MOS_Q26PWM();//B+C-
		}
		break;
		
		case  0x2:  
		{
		MOS_Q24PWM();//B+A-
		}
		break;
		
		case  0x6:   
		{
		MOS_Q34PWM();//C+A-
		}
		break;

		case  0x4:
		{
		MOS_Q35PWM();//C+B-
		}
		break;
	
		default:
		{
		Stop_Motor();
		Hall_Three.Speed_RPM=0;
		}
		break;
	}
#endif
		}
	Hall_Three.OldHall_State=Hall_Three.Hall_State ;
}
#endif

