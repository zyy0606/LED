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
#include "stm32f0xx.h"
#include "gpio.h"
#include "adc.h"
#include "pwm.h"
#include "soc.h"
#include "usart.h"
/** @addtogroup STM32F0xx_StdPeriph_Templates
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
RTC_TimeTypeDef RTC_Time;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint16_t DeviceID = 0x06;
extern uint32_t value[8];
extern unsigned short uart1_rx;
volatile unsigned char CalSOCTimesOut;
int FixTimesOut=0;
unsigned short UpperComputer=0x00,temp_SLA;
unsigned char HealthJudge, WriteVBeginDone, WriteVEndDone, charge_done_flag, additional_charge_flag, PWM_flag=0, BatteryIsHealth=1;
double ISLA,ILED,VDC,VSLA,VSOLAR,VSLA, vol_ChargeLimit, VBegin, VEnd, QBegin, QEnd;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void Day_Mode(void);
void Night_Mode(void);
void CalUIData(void);
void delay(unsigned int ms);
void Judge_Health(void);

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
  SystemInit(); //����ϵͳ��ʱ��
	adc_conf();
	pwm_conf();
	usart1_conf();
	gpio_conf();
	
	  if (SysTick_Config(SystemCoreClock / 1000))  //������Ķ�ʱ��ÿ1ms�ж�һ��
  { 
    /* Capture error */ 
    while (1);
  }
	
	Read_ADC();
  CalUIData();
	
  /* Infinite loop */
  while (1){
		if(uart1_rx != 0x00){  //�Ƿ���յ���λ������
			UpperComputer = uart1_rx;
			uart1_rx = 0x00;
		}
		
		if(CalSOCTimesOut == 0){  //0.1s ��ʱ����SOC����
			CalSoc(ISLA);
			CalSOCTimesOut = 100;    //�趨����ֵ
		}
		
		if(UpperComputer){
	    if(( UpperComputer == Sunrise || UpperComputer == Three_Before_Sunset || UpperComputer == ChargeVallyTime || UpperComputer == EightAM)){ //&& VSOLAR > 14){
			  Day_Mode();  //�������ģʽ�ӳ���
			  if(WriteVBeginDone)  WriteVBeginDone = 0;
		  }
		
		  else if(UpperComputer == LightOn){
			  /* K1��K4�ϣ�AC�ŵ� */
        GPIOB->BRR = 0x000a;
        GPIOB->BSRR = 0x0011;
		  }
		  else if(UpperComputer == LightOff){
			  /* K1��K2��K3��K4�� */
        GPIOB->BRR = 0x001b;
		  }
		  else{
			  Night_Mode();  //�����ҹģʽ�ӳ���
		
			  if(! WriteVBeginDone){
				  VBegin = VSLA;       //�ŵ�ǰ��ص�ѹ
				  QBegin = g_SOCPara.ulAs;  //�ŵ�ǰ��ص���
				  WriteVBeginDone = 1;
				  WriteVEndDone = 0;
			  }
		  }
	  } 
		
		Read_ADC();
    CalUIData();
	}
}

//*****************************************************************************
// ��������    : void Day_Mode(void)
// ��������    : ����ģʽ�ӳ��򣬿��ƹ�������س��
// ����        : None
// ���        : None
// ����        : None
//****************************************************************************
void Day_Mode(void)
{
	if(UpperComputer == Sunrise || UpperComputer == EightAM)
  {
	  if(charge_done_flag == 0){
	    /* K2��,��������س��,�ص� */
	  	GPIOB->BRR = 0x0019;
      GPIOB->BSRR = 0x0002;
  	}
		
  	temp_SLA = ((value[5] - value[6])/0.01/1000-100)/0.385;  //��ѹת�¶ȹ�ʽ T=(R-100)/0.385
		
		if(temp_SLA >= 5 && temp_SLA <= 35){
			vol_ChargeLimit = 14.57 - 0.018*(temp_SLA - 5); 
		}
		else if(temp_SLA < 5){
			vol_ChargeLimit = 14.57 + 0.3*(5 - temp_SLA);
		}
		else{
			vol_ChargeLimit = 14.03 - 0.3*(temp_SLA - 35);
		}
  	
    if(VSOLAR >= vol_ChargeLimit){  //�Ƚϳ���ѹ�����Ƶ�ѹ
			if(BatteryIsHealth == 1){
	      GPIOB->BRR = 0x0002;  //��K2
	      charge_done_flag = 1;
			}
			else{
				if(!FixTimesOut){
					FixTimesOut = 1800000;
				}
			}
  	}
	}
	
	else if(UpperComputer == ChargeVallyTime)
	{
		if(charge_done_flag == 0){
	    /* K1��,AC�����س�磬�ص� */
	  	GPIOB->BRR = 0x001a;
      GPIOB->BSRR = 0x0001;
  	}
		
  	temp_SLA = ((value[5] - value[6])/0.01/1000-100)/0.385;  //��ѹת�¶ȹ�ʽ T=(R-100)/0.385
		
		if(temp_SLA >= 5 && temp_SLA <= 35){
			vol_ChargeLimit = 14.57 - 0.018*(temp_SLA - 5); 
		}
		else if(temp_SLA < 5){
			vol_ChargeLimit = 14.57 + 0.3*(5 - temp_SLA);
		}
		else{
			vol_ChargeLimit = 14.03 - 0.3*(temp_SLA - 35);
		}
  	 
    if(VSOLAR >= vol_ChargeLimit){  //�Ƚϳ���ѹ�����Ƶ�ѹ
	    GPIOB->BRR = 0x0001;  //��K1
	    charge_done_flag = 1;
  	}
	}
	
	else //����ǰ��Сʱ~��������
	{
		if(g_SOCPara.ucSOC <60 && additional_charge_flag == 0){
			/*K1�ϣ�AC�����س��*/
			GPIOB->BRR = 0x001a;
      GPIOB->BSRR = 0x0001;
			additional_charge_flag = 1;
		}
		
		else if(g_SOCPara.ucSOC >= 100 && additional_charge_flag == 1){
			GPIOB->BRR = 0x0001; //�����ɣ���K1
			additional_charge_flag = 0;
		}
	}
}

//*****************************************************************************
// ��������    : void Night_Mode(void)
// ��������    : ҹ��ģʽ�ӳ��򣬿������طŵ�
// ����        : None
// ���        : None
// ����        : None
//****************************************************************************
void Night_Mode(void)
{
	if(VDC == 0)   //�޽�����ʱȫ���빦������
	{
	  if(PWM_flag == 0){
 			/* TIM1 counter enable */
      TIM_Cmd(TIM1, ENABLE);

      /* TIM1 Main Output Enable */
      TIM_CtrlPWMOutputs(TIM1, ENABLE);
				
			PWM_flag = 1;
		}
			
		/* K3��K4�ϣ���طŵ� */
    GPIOB->BRR = 0x0003;
    GPIOB->BSRR = 0x0018;
	}
		
	/*�н����������ʱ�����ģʽ*/
	else  
	{		
	  /*����~19�㡢21��~24�㣬����ȫ���ʷŵ�*/
		if((UpperComputer == Sunset ||UpperComputer == NinePM) && VSLA >= 11){
			if(PWM_flag){
			  /* TIM1 Main Output Disable */
        TIM_CtrlPWMOutputs(TIM1, DISABLE);
			  TIM_Cmd(TIM1, DISABLE);
					
				PWM_flag = 0;
			}
				
			/* K1��K3��K4�ϣ�AC����طŵ� */
      GPIOB->BRR = 0x0002;
      GPIOB->BSRR = 0x0019;
		}
			
		/*19��~21�㣬ȫ��ȫ����*/
		else if( UpperComputer == SevenPM )  {
			if(PWM_flag)
			{
				/* TIM1 Main Output Disable */
        TIM_CtrlPWMOutputs(TIM1, DISABLE);
				TIM_Cmd(TIM1, DISABLE);
					
				PWM_flag = 0;
			}

			/* K3��K4�ϣ���طŵ� */
      GPIOB->BRR = 0x0003;
      GPIOB->BSRR = 0x0018;
		}
			
		/*24��~�ճ���������ȫ���빦�ʷŵ�*/
		else
		{
	  	if(! WriteVEndDone){
				VEnd = VSLA;          //�ŵ���ص�ѹ
				QEnd = g_SOCPara.ulAs;  //�ŵ���ص���
				WriteVEndDone = 1;
			}
				
			/* K1��K3��K4�ϣ�AC����طŵ� */
      GPIOB->BRR = 0x0002;
      GPIOB->BSRR = 0x0019;
		  /* K1��K4�ϣ�AC�ŵ� 
      GPIOB->BRR = 0x000a;
      GPIOB->BSRR = 0x0011;*/
			
			delay(1000);
      if(PWM_flag == 0)
			{
 			  /* TIM1 counter enable */
        TIM_Cmd(TIM1, ENABLE);

        /* TIM1 Main Output Enable */
        TIM_CtrlPWMOutputs(TIM1, ENABLE);
		 		
		  	PWM_flag = 1;
	  	}
							
			if((!HealthJudge) && (UpperComputer==ZeroAM)){					
				Judge_Health();
			  HealthJudge = 1;
			}
		}
	}
}

/**
  * @brief  Calculate voltage and current data.
  * @param  None
  * @retval None
  */
void CalUIData(void)
{
		ISLA = ((double)value[0]/1000-(double)value[7]/1000)/0.004; 
		ILED = ((double)value[1]/1000-(double)value[7]/1000)/0.004; 
		VDC = 0.9904*(200+30)*(double)value[2]/1000/30;    //0.9904Ϊ��������
		VSLA = 1.0068*(200+51)*(double)value[3]/1000/51;         //1.0068Ϊ��������
		VSOLAR = 0.9897*(200+39)*(double)value[4]/1000/39;   //0.9897Ϊ��������
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void Time_Decrement(void)
{
  if (CalSOCTimesOut){ 
    CalSOCTimesOut--;
  }
	
	if(FixTimesOut){
		FixTimesOut --;
		if(!FixTimesOut) BatteryIsHealth =1;
	}
}

/**
  * @brief  To determine whether the battery is healthy.
  * @param  None
  * @retval None
  */
void Judge_Health(void)
{
	unsigned int SOC[34]={100,98,95,92,89,86,83,80,77,74,71,68,65,62,59,56,53,50,47,44,41,38,35,32,29,26,23,20,17,14,11,8,5,0};
	double V[34]={12.800,12.521,12.496,12.471,12.444,12.412,12.380,12.352,12.315,12.278,12.241,12.199,12.157,12.115,12.074,12.030,11.985,
	              11.941,11.894,11.847,11.800,11.752,11.700,11.648,11.594,11.533,11.472,11.423,11.335,11.248,11.160,11.041,10.910,10.500};
	int SOC_Begin,SOC_End;
	int i,index;
	double Q_act;   //��õ�ʵ������
			
	for(i=0;i<34;i++){
		if(VBegin <= V[i]){
			index = i;
		}
	}
	SOC_Begin = SOC[index]-((SOC[index]-SOC[index+1])*(V[index]-VBegin)/(V[index]-V[index+1]));   //���Բ�ֵ��ŵ�ǰSOC
	
	for(i=0;i<34;i++){
	  if(VEnd <= V[i]){
			index = i;
		}
	}
	SOC_End = SOC[index]-((SOC[index]-SOC[index+1])*(V[index]-VEnd)/(V[index]-V[index+1]));   //���Բ�ֵ��ŵ��SOC
	
	Q_act = (QBegin-QEnd)*100/(SOC_Begin - SOC_End);
	
	if(Q_act < 80){     //���ʵ������С��80AH����ز�����
		if(Q_act < 60){
			USART_SendData(USART1, DeviceID);
			delay(2000);
			USART_SendData(USART1, BadBattery);
		}
		else BatteryIsHealth = 0;
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
	for(i=0;i<0x1F40;i++)
	{
		for(k=0;k<ms;k++);
	}
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
