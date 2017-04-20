#include "stm32f0xx.h"
#include "usart.h"
#include "main.h"

//*******************************************************************************
// 函数名称    : void usart1_conf(void)
// 功能描述    : 初始化串口1.
// 输入        : 无
// 输出        : None
// 返回        : None
//******************************************************************************
void usart1_conf(void)
{
  USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);  // 使能PA外设时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);// 使能串口1外设时钟 
  
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_0);// 配置PA9为AF_1外设 
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_0);// 配置PA10为AF_1外设 
  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  //使用外设功能
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
   
  USART_InitStructure.USART_BaudRate = 115200;  //串口速率为9600
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;// 8位数据传输
  USART_InitStructure.USART_StopBits = USART_StopBits_1;// 1个停止位 
  USART_InitStructure.USART_Parity = USART_Parity_No;   // 无校验位
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无流量控制
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART1, &USART_InitStructure);
  
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;  //串口1中断向量
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0;    //中断优先级0
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;    //中断使能
  NVIC_Init(&NVIC_InitStructure);
  
  USART_Cmd(USART1, ENABLE);   //使能串口1
}

//*******************************************************************************
// 函数名称    : void Usart1_Sends(void)
// 功能描述    : 发送数据串
// 输入        : data[]
// 输出        : None
// 返回        : None
//******************************************************************************
void Usart1_Sends(unsigned char data[],unsigned char num)
{
	unsigned char i=0;

	while(i< num)
	{
    USART_SendData(USART1, data[i]);
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
		
		i++;
	}
}


void Uart1_PutChar(unsigned char data)
{
  USART_SendData(USART1, data);
  while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}

