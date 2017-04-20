/**
  ******************************************************************************
  * @file    Project/STM32F0xx_StdPeriph_Templates/main.c 
  * @author  MCD Application Team
  * @version V1.5.0
  * @date    05-December-2014
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "rtc.h"
#include "usart.h"
#include "stm32f0xx_it.h"
#include "sun.h"
#include <stdio.h>
#include <string.h>
/** @addtogroup STM32F0xx_StdPeriph_Templates
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t SunriseHourToday,SunriseMinuteToday,SunsetHourToday,SunsetMinuteToday; 
uint8_t NiceWeather = 1;
uint8_t SystemOn = 0;
uint8_t Period = 0x00;
uint8_t SendOrder = 0;
uint8_t Handle_U2Data = 0;
uint8_t year,month,day,hour,minute,second;
RTC_TimeTypeDef RTC_Time;
RTC_DateTypeDef  RTC_Date;
RTC_TimeTypeDef  RTC_TimeSet;
RTC_DateTypeDef RTC_DateSet;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void delay(unsigned int ms);
void GetSunTime(void);
/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f0xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f0xx.c file
     */ 
      
  /* Add your application code here
     */
	//unsigned char broadcast[] = {0xde,0xdf,0xef,0xd9,0x01}; //广播模式AT指令
	extern char USART2_Rx_Buffer[256];
	
  usart1_conf();
	usart2_conf();
	RTC_Config();
  GPIO_ResetBits(GPIOA,1);//将485_DIR引脚置低，处于接收状态
	
  /* Infinite loop */
  while (1)
  {
		delay(1000);
		RTC_GetDate(0, &RTC_Date);
	  RTC_GetTime(0, &RTC_Time);
	  GetSunTime();
		
		if(SystemOn){
		  /*-------------日出~8:00AM----------------*/
	  	if(((RTC_Time.RTC_Hours > SunriseHourToday && RTC_Time.RTC_Hours < 8) 
		  	||(RTC_Time.RTC_Hours == SunriseHourToday && RTC_Time.RTC_Minutes >= SunriseMinuteToday)) && Period !=Sunrise)
	  	{
		    if(!NiceWeather){
		  	  USART_SendData(USART1, ChargeVallyTime);
	  	  }
	  	  else{
		      USART_SendData(USART1, Sunrise);
		    }
		  	Period = Sunrise;
	  	}
		
	  	/*--------------8:00AM~日落前三小时-----------------*/
	  	else if(RTC_Time.RTC_Hours >= 8 && RTC_Time.RTC_Hours < SunsetHourToday -3 && Period != EightAM)
	  	{
		    if(!NiceWeather){
  	  	  USART_SendData(USART1, EightAM);
		  	}
		  	Period = EightAM;
		  }
		
	  	/*-------------日落前三小时~日落-----------------*/
	  	else if(RTC_Time.RTC_Hours >= SunsetHourToday -3 && RTC_Time.RTC_Hours < SunsetHourToday 
	  		        && RTC_Time.RTC_Minutes >= SunsetMinuteToday && Period != Three_Before_Sunset)
	  	{
    		USART_SendData(USART1, Three_Before_Sunset);
	  		Period = Three_Before_Sunset;
	  	}
		
	  	/*----------------日落~7：00PM--------------------*/
	  	else if(RTC_Time.RTC_Hours >= SunsetHourToday && RTC_Time.RTC_Hours < 19 
		  	&& RTC_Time.RTC_Minutes >= SunsetMinuteToday && Period != Sunset)
		  {
		    USART_SendData(USART1, Sunset);
        Period = Sunset;
	  	}
	  	
	  	/*----------------下午7点~下午9点---------------------*/
	  	else if(RTC_Time.RTC_Hours >= 19 && RTC_Time.RTC_Hours < 21 && Period != SevenPM)
	  	{
    		USART_SendData(USART1, SevenPM);
	  		Period = SevenPM;
	  	}
	  	/*---------------下午9点~次日零点----------------*/
	  	else if(RTC_Time.RTC_Hours >= 21 && Period != NinePM)
	  	{
	  		USART_SendData(USART1, NinePM);
	  		Period = NinePM;
      }
		
  		/*---------------零点~日出----------------*/
  		else if((RTC_Time.RTC_Hours < SunriseHourToday ||(RTC_Time.RTC_Hours == SunriseHourToday 
  			&& RTC_Time.RTC_Minutes < SunriseMinuteToday)) && Period != ZeroAM)
	  	{
	  	  USART_SendData(USART1, ZeroAM);
        Period = ZeroAM;
	  	}
    }
		else
		{
			delay(500);
			if(SendOrder == LightOn){
				USART_SendData(USART1,LightOn);
				Period = LightOn;
				SendOrder = 0x00;
			}
			else if(SendOrder == LightOff){
				USART_SendData(USART1,LightOff);
				Period = LightOff;
				SendOrder = 0x00;
			}
		}
		
		if(Handle_U2Data == 1)
		{
					if(USART2_Receive("开灯")){
					USART_SendData(USART1, LightOn);
					Period = LightOn;
					SystemOn = 0;
					SendOrder = LightOn;
					GPIO_SetBits(GPIOA,GPIO_Pin_1);
					delay(1);
				  printf("已完成开灯 ");
					delay(1);
					GPIO_ResetBits(GPIOA,GPIO_Pin_1);
			  }
		    else if(USART2_Receive("关灯")){
				  USART_SendData(USART1, LightOff);
					Period = LightOff;
					SystemOn = 0;
					SendOrder = LightOff;
					GPIO_SetBits(GPIOA,GPIO_Pin_1);
				  printf("已完成关灯 ");
					GPIO_ResetBits(GPIOA,GPIO_Pin_1);
			  }
				else if(USART2_Receive("正常运行")){
					SystemOn = 1;
					GPIO_SetBits(GPIOA,GPIO_Pin_1);
				  printf("开始正常运行 ");
					GPIO_ResetBits(GPIOA,GPIO_Pin_1);
			  }
				else if(strncmp(USART2_Rx_Buffer,"时间",4)==0){
					year = (USART2_Rx_Buffer[6]-'0')*10+(USART2_Rx_Buffer[7]-'0');
					month = (USART2_Rx_Buffer[10]-'0')*10+(USART2_Rx_Buffer[11]-'0');
					day = (USART2_Rx_Buffer[14]-'0')*10+(USART2_Rx_Buffer[15]-'0');
					hour = (USART2_Rx_Buffer[18]-'0')*10+(USART2_Rx_Buffer[19]-'0');
					minute = (USART2_Rx_Buffer[22]-'0')*10+(USART2_Rx_Buffer[23]-'0');
					second = (USART2_Rx_Buffer[26]-'0')*10+(USART2_Rx_Buffer[27]-'0');
					
					//设定时间
					RTC_TimeSet.RTC_H12     = RTC_H12_AM;
					RTC_TimeSet.RTC_Hours   = hour;
					RTC_TimeSet.RTC_Minutes = minute;
					RTC_TimeSet.RTC_Seconds = second;  
					RTC_SetTime(RTC_Format_BIN, &RTC_TimeSet);
  
					//设定日期
					RTC_DateSet.RTC_Date = day;
					RTC_DateSet.RTC_Month = month;
					RTC_DateSet.RTC_Year = year;    
					RTC_SetDate(0, &RTC_DateSet);
					RTC_GetTime(0, &RTC_Time);
	        RTC_GetDate(0, &RTC_Date);
					RTC_SetTime(RTC_Format_BIN, &RTC_TimeSet);
					RTC_SetDate(0, &RTC_DateSet);
					RTC_GetTime(0, &RTC_Time);
	        RTC_GetDate(0, &RTC_Date);
					GPIO_SetBits(GPIOA,GPIO_Pin_1);
					printf("时间设置完成,现在时间是%d年%d月%d日%d点%d分%d秒。 ",
					        RTC_Date.RTC_Year,RTC_Date.RTC_Month,RTC_Date.RTC_Date,RTC_Time.RTC_Hours,RTC_Time.RTC_Minutes,RTC_Time.RTC_Seconds);
					GPIO_ResetBits(GPIOA,GPIO_Pin_1);
				}
				else if(strstr(USART2_Rx_Buffer,"天气")){
					if(strstr(USART2_Rx_Buffer,"阴")||strstr(USART2_Rx_Buffer,"雨")){
						NiceWeather = 0;
						GPIO_SetBits(GPIOA,GPIO_Pin_1);
				  printf("将在谷电时段对电池充电。 ");
					GPIO_ResetBits(GPIOA,GPIO_Pin_1);
					}
					else{
						NiceWeather = 1;
						GPIO_SetBits(GPIOA,GPIO_Pin_1);
					  printf("谷电时段不对电池充电。 ");
						GPIO_ResetBits(GPIOA,GPIO_Pin_1);
					}
				}
  		  memset(USART2_Rx_Buffer,0,256);
				Handle_U2Data = 0;
		}
  }
}

/**
  * @brief  Delay program.
  * @param  ms
  * @retval None
  */
void delay(unsigned int ms)
{
	unsigned int i,k;
	for(i=0;i<0x1f00;i++)
	{
		for(k=0;k<ms;k++);
	}
}

/**
  * @brief  Delay program.
  * @param  ms
  * @retval None
  */
void GetSunTime(void)
{
	SunriseHourToday = SunriseHour[RTC_Date.RTC_Month - 1][RTC_Date.RTC_Date - 1];
	SunriseMinuteToday = SunriseMinute[RTC_Date.RTC_Month - 1][RTC_Date.RTC_Date - 1];
	SunsetHourToday = SunsetHour[RTC_Date.RTC_Month - 1][RTC_Date.RTC_Date - 1];
	SunsetMinuteToday = SunsetMinute[RTC_Date.RTC_Month - 1][RTC_Date.RTC_Date - 1];
}
#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
