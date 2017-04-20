
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
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);//使能PWR时钟

  PWR_BackupAccessCmd(ENABLE);  //PWR使能

  RCC_LSEConfig(RCC_LSE_ON);  //外部低速晶振使能

  while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET) //等待外部低速晶振起振
  {
  }

  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);//选择外部低速晶振为RTC时钟

  RTC_InitStructure.RTC_AsynchPrediv = 0x7F;
  RTC_InitStructure.RTC_SynchPrediv  = 0xFF;
  RTC_InitStructure.RTC_HourFormat   = RTC_HourFormat_24;//24小时显示格式
  RTC_Init(&RTC_InitStructure);
  
  //设定时间为：00点00分00秒
  RTC_TimeStruct.RTC_H12     = RTC_H12_AM;
  RTC_TimeStruct.RTC_Hours   = 00;
  RTC_TimeStruct.RTC_Minutes = 00;
  RTC_TimeStruct.RTC_Seconds = 00;  
  RTC_SetTime(RTC_Format_BIN, &RTC_TimeStruct);
  
  //设定时间为：2016年4月19号，星期2
  RTC_DateStruct.RTC_Date = 01;
  RTC_DateStruct.RTC_Month = 06;
  RTC_DateStruct.RTC_WeekDay = 3;
  RTC_DateStruct.RTC_Year =   16;    
  RTC_SetDate(0, &RTC_DateStruct);
   
  RCC_RTCCLKCmd(ENABLE);//使能RTC计数
  
  RTC_WaitForSynchro();
}
