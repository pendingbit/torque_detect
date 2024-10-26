//############################################################
// Created on: 2019��5��18��
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//��Ȩ���У�����ؾ�
//STM32������ƿ�����
//���ĿƼ�
//��ַ: https://shop298362997.taobao.com/
//������ƴ������䣺JXKJ_2007@163.com
//############################################################
#include "ADC_int.h"
#include "Hall.h"
#include "Control stage parameters.h"
#include "State_Machine.h"
#include "Hallless.h"
#include "Drive parameters.h"

uint32_t  ADC_ConvertedValue[5]={0,0,0,0,0};//��ʼ��AD����ֵ

extern volatile uint16_t ADC_DualConvertedValueTab[5];    //����ʱ����ȡֵ

extern    State       StateContr;
extern    ADCSamp     ADCSampPare;

#ifdef HALL
extern    Hall        Hall_Three;
#endif

#ifdef HALLLESS
extern    Hallless   Hallless_Three;
#endif
void Adc_Configuration(void)
{
	ADC_InitTypeDef       ADC_InitStructure;	
	DMA_InitTypeDef       DMA_InitStructure;
	GPIO_InitTypeDef      GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1 , ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);  
	
	GPIO_InitStructure.GPIO_Pin = PHASE_U_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(PHASE_U_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PHASE_V_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(PHASE_V_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PHASE_W_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(PHASE_W_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = BusVolt_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(BusVolt_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = POT_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(POT_GPIO_PORT, &GPIO_InitStructure);
	
	ADC_StructInit(&ADC_InitStructure);
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;  
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 5;     
	ADC_Init(ADC1, &ADC_InitStructure);

	ADC_RegularChannelConfig(ADC1, PHASE_U_CURR_CHANNEL, 1, ADC_SampleTime_7Cycles5);
	ADC_RegularChannelConfig(ADC1, PHASE_V_CURR_CHANNEL, 2, ADC_SampleTime_7Cycles5);
	ADC_RegularChannelConfig(ADC1, PHASE_W_CURR_CHANNEL, 3, ADC_SampleTime_7Cycles5);  
	ADC_RegularChannelConfig(ADC1, BusVolt_CHANNEL, 4, ADC_SampleTime_7Cycles5); 
	ADC_RegularChannelConfig(ADC1, POT_CHANNEL, 5, ADC_SampleTime_7Cycles5); 

	ADC_Cmd(ADC1, ENABLE);
	ADC_DMACmd(ADC1, ENABLE);
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1));
	ADC_SoftwareStartConvCmd(ADC1, ENABLE); 	
	
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (unsigned int) & (ADC1->DR);
	DMA_InitStructure.DMA_MemoryBaseAddr = (unsigned int)&ADC_DualConvertedValueTab;	 
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;

	DMA_InitStructure.DMA_BufferSize =5;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;

	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	
	DMA_Cmd(DMA1_Channel1, ENABLE);
}

void Offset_CurrentReading(void)  
{
	static uint16_t ADC_PhaseU_Curr[64];
	static uint16_t ADC_PhaseV_Curr[64];
	static uint16_t ADC_PhaseW_Curr[64];
	static uint8_t i=0;
   
  /* ADC Channel used for current reading are read  in order to get zero currents ADC values*/
  //64�β�����ƽ��ֵ��������������ʼУ׼   	
	ADC_PhaseU_Curr[i] = ADC_DualConvertedValueTab[0];
	ADC_PhaseV_Curr[i] = ADC_DualConvertedValueTab[1];  
	ADC_PhaseW_Curr[i] = ADC_DualConvertedValueTab[2];

	i++;
	if(i>=64)
	{
	i=0;
	}
 //�����������ĸ�ߵ����ĵ��跨��������,��Ҫ�ϵ��ȡ��ʼƫִ��ѹ
	if(StateContr.drive_car == 0) 
	{
		uint32_t sum_U=0;
		uint32_t sum_V=0;
		uint32_t sum_W=0;
		uint8_t i;
		for(i=0; i < 64; i++)
		{
			sum_U += ADC_PhaseU_Curr[i];
			sum_V += ADC_PhaseV_Curr[i];
			sum_W += ADC_PhaseW_Curr[i];
		}
	     ADCSampPare.OffsetPhaseU_Curr = sum_U /64 ;
	     ADCSampPare.OffsetPhaseV_Curr = sum_V /64 ;
	     ADCSampPare.OffsetPhaseW_Curr = sum_W /64 ;
	 }
}
#ifdef HALL
void HALL_ADC_Sample(void) // ����PWM�жϽ����󣬲���ʱ��Ϊ1us����,(7cycles)  ��֤��PWM�жϺ����������Ϊ��PWM�м�ɼ������
{ 
  ADCSampPare.PhaseU_Curr = ADC_DualConvertedValueTab[0] - ADCSampPare.OffsetPhaseU_Curr;	
	ADCSampPare.PhaseV_Curr = ADC_DualConvertedValueTab[1] - ADCSampPare.OffsetPhaseV_Curr;
	ADCSampPare.PhaseW_Curr = ADC_DualConvertedValueTab[2] - ADCSampPare.OffsetPhaseW_Curr;   
  ADCSampPare.BUS_Voltage = ADC_DualConvertedValueTab[3];	
	if(Hall_Three.Hall_State == 0x05)
	{
	ADCSampPare.BUS_Curr = ADCSampPare.PhaseV_Curr + ADCSampPare.PhaseW_Curr;
	}
	else if(Hall_Three.Hall_State == 0x01)
	{
	ADCSampPare.BUS_Curr = ADCSampPare.PhaseU_Curr + ADCSampPare.PhaseW_Curr;
	}
	else if(Hall_Three.Hall_State == 0x03)
	{
	ADCSampPare.BUS_Curr = ADCSampPare.PhaseU_Curr + ADCSampPare.PhaseW_Curr;
	}
	else if(Hall_Three.Hall_State == 0x02)
	{
	ADCSampPare.BUS_Curr = ADCSampPare.PhaseU_Curr + ADCSampPare.PhaseV_Curr;
	}
	else if(Hall_Three.Hall_State == 0x06)
	{
	ADCSampPare.BUS_Curr = ADCSampPare.PhaseU_Curr + ADCSampPare.PhaseV_Curr;
	}
	else if(Hall_Three.Hall_State == 0x04)
	{
	ADCSampPare.BUS_Curr = ADCSampPare.PhaseW_Curr + ADCSampPare.PhaseV_Curr;
	}
}
#endif

#ifdef HALLLESS
void HALLLESS_ADC_Sample(void) // ����PWM�жϽ����󣬲���ʱ��Ϊ1us����,(7cycles)  ��֤��PWM�жϺ����������Ϊ��PWM�м�ɼ������
{    
  ADCSampPare.PhaseU_Curr = ADC_DualConvertedValueTab[0] - ADCSampPare.OffsetPhaseU_Curr;	
	ADCSampPare.PhaseV_Curr = ADC_DualConvertedValueTab[1] - ADCSampPare.OffsetPhaseV_Curr;
	ADCSampPare.PhaseW_Curr = ADC_DualConvertedValueTab[2] - ADCSampPare.OffsetPhaseW_Curr;  
  ADCSampPare.BUS_Voltage = ADC_DualConvertedValueTab[3];	
	if(Hallless_Three.Hallless_State == 0x05)
	{
	ADCSampPare.BUS_Curr = ADCSampPare.PhaseV_Curr + ADCSampPare.PhaseW_Curr;
	}
	else if(Hallless_Three.Hallless_State == 0x01)
	{
	ADCSampPare.BUS_Curr = ADCSampPare.PhaseU_Curr + ADCSampPare.PhaseW_Curr;
	}
	else if(Hallless_Three.Hallless_State == 0x03)
	{
	ADCSampPare.BUS_Curr = ADCSampPare.PhaseU_Curr + ADCSampPare.PhaseW_Curr;
	}
	else if(Hallless_Three.Hallless_State == 0x02)
	{
	ADCSampPare.BUS_Curr = ADCSampPare.PhaseU_Curr + ADCSampPare.PhaseV_Curr;
	}
	else if(Hallless_Three.Hallless_State == 0x06)
	{
	ADCSampPare.BUS_Curr = ADCSampPare.PhaseU_Curr + ADCSampPare.PhaseV_Curr;
	}
	else if(Hallless_Three.Hallless_State == 0x04)
	{
	ADCSampPare.BUS_Curr = ADCSampPare.PhaseW_Curr + ADCSampPare.PhaseV_Curr;
	}
}
#endif
