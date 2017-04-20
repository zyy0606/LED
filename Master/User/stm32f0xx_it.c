/**
  ******************************************************************************
  * @file    GPIO/IOToggle/stm32f0xx_it.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    18-May-2012
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
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
#include "stm32f0xx_it.h"
#include "main.h"
#include "usart.h"
#include <stdio.h>
#include <string.h>
/** @addtogroup STM32F0xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup IOToggle
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
unsigned int ReceiveID = 0;
char USART2_Rx_Buffer[256];
uint8_t Rx_flg,Rx_Lenght = 0;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M0 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F0xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f0xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/******************************************************************************/
/*            STM32F0xx Peripherals Interrupt Handlers                        */
/******************************************************************************/
/**
* @brief  This function handles USART1 interrupt request.
* @param  None
* @retval None
*/
void USART1_IRQHandler(void)
{
	unsigned int RxData;
	uint16_t DeviceID;
	
  if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
  {
		if(!ReceiveID){
      DeviceID = USART_ReceiveData(USART1);
			ReceiveID = 1;
    }
		else{
			RxData = USART_ReceiveData(USART1);
			ReceiveID = 0;
			if(RxData == BadBattery){
				printf("AT+SENDSMS=15267018683,\"Battery No.%d is in bad condition, please fix it soon!\"\x1A",DeviceID);
			}
		}
	}
}

/**
* @brief  This function handles USART2 interrupt request.
* @param  None
* @retval None
*/
void USART2_IRQHandler(void)
{
	uint8_t tmp;
	extern uint8_t NiceWeather;
	extern uint8_t SystemOn;
	extern uint8_t Period;
	extern uint8_t SendOrder;
  extern uint8_t Handle_U2Data;
	
	if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
  {
		tmp = USART_ReceiveData(USART2);

		if(Rx_flg==1){
		  if(tmp==0x23){                     //收到符号#为发送完毕标志，编码为0x23
			  Rx_flg=0;
			  Rx_Lenght=0;
				Handle_U2Data = 1;
		  }
		  else{
			  USART2_Rx_Buffer[Rx_Lenght++] = tmp;
		  }
	  }
			
		if(tmp==0x2A){      //收到符号*为发送开始标志，编码为0x2A
			Rx_flg=1;
		}
	}
	
	USART_ClearITPendingBit(USART2, USART_IT_RXNE); //清中断标志
}

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
