
#include "stm32f0xx.h"
#include "adc.h"
/* Private variables ---------------------------------------------------------*/
#define N 24   //每通道采样24次
#define M 8   //共8通道
__IO uint16_t AD_Value[N][M];  //用于存放ADC转换结果
__IO uint16_t After_filter[M];  //用于存放滤波之后的结果
uint32_t value[8];

int i;
//***************************************************************************
// 函数名称    : void adc_conf(void)
// 功能描述    : ADC参数初始化定义函数.
// 输入        : None
// 输出        : None
// 返回        : None
//***************************************************************************
void adc_conf(void)
{
 // unsigned short adc_data;
  
  ADC_InitTypeDef     ADC_InitStructure;
  GPIO_InitTypeDef    GPIO_InitStructure;
	DMA_InitTypeDef     DMA_InitStructure;
   /* ADC1 Periph clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	/* DMA1 clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1 , ENABLE);
  
  /* DMA1 Channel1 Config */
  DMA_DeInit(DMA1_Channel1);
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&AD_Value[0][0];
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = N*M;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);
  
  /* DMA1 Channel1 enable */
  DMA_Cmd(DMA1_Channel1, ENABLE);
	
 /* Configure ADC Channel8 as analog input */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 |GPIO_Pin_2| GPIO_Pin_3 
	                              |GPIO_Pin_4 |GPIO_Pin_5 |GPIO_Pin_6 |GPIO_Pin_7 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//50MHzIO口
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
 /* ADCs DeInit */  
  ADC_DeInit(ADC1);
  
  /* Initialize ADC structure */
  ADC_StructInit(&ADC_InitStructure);
  
  /* Configure the ADC1 in continous mode withe a resolutuion equal to 12 bits  */
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; 
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Upward;//ADC_ScanDirection_Upward;
  ADC_Init(ADC1, &ADC_InitStructure); 
  
  /* Convert the ADC1 Channels with 239.5 Cycles as sampling time */ 
  ADC_ChannelConfig(ADC1, ADC_Channel_0 , ADC_SampleTime_239_5Cycles);//ADC_SampleTime_239_5Cycles);   
	ADC_ChannelConfig(ADC1, ADC_Channel_1 , ADC_SampleTime_239_5Cycles);
	ADC_ChannelConfig(ADC1, ADC_Channel_2 , ADC_SampleTime_239_5Cycles);
	ADC_ChannelConfig(ADC1, ADC_Channel_3 , ADC_SampleTime_239_5Cycles);
	ADC_ChannelConfig(ADC1, ADC_Channel_4 , ADC_SampleTime_239_5Cycles);
	ADC_ChannelConfig(ADC1, ADC_Channel_5 , ADC_SampleTime_239_5Cycles);
	ADC_ChannelConfig(ADC1, ADC_Channel_6 , ADC_SampleTime_239_5Cycles);
	ADC_ChannelConfig(ADC1, ADC_Channel_7 , ADC_SampleTime_239_5Cycles);

  /* ADC Calibration */
  ADC_GetCalibrationFactor(ADC1);
  ADC_DMACmd(ADC1, ENABLE);
  /* Enable ADCperipheral[PerIdx] */
  ADC_Cmd(ADC1, ENABLE);     
  
  /* Wait the ADCEN falg */
  while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADEN)); 
  
  /* ADC1 regular Software Start Conv */ 
  ADC_StartOfConversion(ADC1); 
//  
//   /* Test EOC flag */
//  while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
//    
//    /* Get ADC1 converted data */
//  adc_data =ADC_GetConversionValue(ADC1);
//  uart2_trx[uart2_rx++] = adc_data >> 8 ;
//  uart2_trx[uart2_rx++] = adc_data  ;
}

/*获取ADC的值*/
uint32_t GetVolt(uint16_t advalue)
{
	return (uint32_t)((advalue*3218)>>12);  //把采集到的数值转换成电压数据，放大1000倍
}

/*中值滤波并求平均值*/
void filter(void)
{
	int sum = 0;
	unsigned char count;
	uint16_t tmp;
	
	for(i=0;i<M;i++)
	{
		for(count=0;count<=N-3;count=count+3)
		{
			/*中值滤波*/
			if(AD_Value[count][i]<AD_Value[count+1][i])   
			{
				tmp=AD_Value[count][i];
				AD_Value[count][i]=AD_Value[count+1][i];
				AD_Value[count+1][i]=tmp;
			}
			if(AD_Value[count+1][i]<AD_Value[count+2][i])
			{
				tmp=AD_Value[count+1][i];
				AD_Value[count+1][i]=AD_Value[count+2][i];
				AD_Value[count+2][i]=tmp;
			}
			if(AD_Value[count][i]<AD_Value[count+1][i])   
			{
				tmp=AD_Value[count][i];
				AD_Value[count][i]=AD_Value[count+1][i];
				AD_Value[count+1][i]=tmp;
			}
			
			/*求平均值*/
			sum += AD_Value[count+1][i];
		}
		After_filter[i]=sum*3/N;
		sum=0;
	}
}

void Read_ADC(void)
{
	ADC_StartOfConversion(ADC1);   //启动ADC1转换
  filter();
	
  for(i=0;i<M;i++)
  {
    value[i]=GetVolt(After_filter[i]);   //结果放大1000倍
  }
}
