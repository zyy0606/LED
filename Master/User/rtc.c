
#include "stm32f0xx.h"

/**
  * @brief  Configures the RTC peripheral and select the clock source.
  * @param  None
  * @retval None
  */
void RTC_Config(void)
{
  RTC_InitTypeDef RTC_InitStructure;
  RTC_TimeTypeDef  RTC_TimeStruct;
  RTC_DateTypeDef RTC_DateStruct;
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);//ʹ��PWRʱ��

  PWR_BackupAccessCmd(ENABLE);  //PWRʹ��

  RCC_LSEConfig(RCC_LSE_ON);  //�ⲿ���پ���ʹ��

  while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET) //�ȴ��ⲿ���پ�������
  {
  }

  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);//ѡ���ⲿ���پ���ΪRTCʱ��

  RTC_InitStructure.RTC_AsynchPrediv = 0x7F;
  RTC_InitStructure.RTC_SynchPrediv  = 0xFF;
  RTC_InitStructure.RTC_HourFormat   = RTC_HourFormat_24;//24Сʱ��ʾ��ʽ
  RTC_Init(&RTC_InitStructure);
  
  //�趨ʱ��Ϊ��00��00��00��
  RTC_TimeStruct.RTC_H12     = RTC_H12_AM;
  RTC_TimeStruct.RTC_Hours   = 00;
  RTC_TimeStruct.RTC_Minutes = 00;
  RTC_TimeStruct.RTC_Seconds = 00;  
  RTC_SetTime(RTC_Format_BIN, &RTC_TimeStruct);
  
  //�趨ʱ��Ϊ��2016��4��19�ţ�����2
  RTC_DateStruct.RTC_Date = 01;
  RTC_DateStruct.RTC_Month = 06;
  RTC_DateStruct.RTC_WeekDay = 3;
  RTC_DateStruct.RTC_Year =   16;    
  RTC_SetDate(0, &RTC_DateStruct);
   
  RCC_RTCCLKCmd(ENABLE);//ʹ��RTC����
  
  RTC_WaitForSynchro();
}
