/**
  ******************************************************************************
  * @file    Project/STM32F0xx_StdPeriph_Templates/main.h 
  * @author  MCD Application Team
  * @version V1.5.0
  * @date    05-December-2014
  * @brief   Header for main.c module
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
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#define Three_Before_Sunset 0x01
#define Sunset 0x02
#define SevenPM 0x03
#define NinePM 0x04
#define ChargeVallyTime 0x05
#define EightAM 0x06
#define ZeroAM 0x07
#define Sunrise 0x08
#define BadBattery 0x09
#define LightOn 0x0a
#define LightOff 0x0b

/* USART Transaction Type */
#define USART_TRANSACTIONTYPE_CMD        0x00
#define USART_TRANSACTIONTYPE_DATA       0x01
/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */



#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
