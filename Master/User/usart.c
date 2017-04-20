#include "stm32f0xx.h"
#include "usart.h"
#include "stdio.h"
#include <string.h>
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
  
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);  // 使能PB外设时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);// 使能串口1外设时钟 
  
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_0);// 配置PB6为AF_1外设 
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_0);// 配置PB7为AF_1外设 
  
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

//*******************************************************************************
// 函数名称    : void usart2_conf(void)
// 功能描述    : 初始化串口1.
// 输入        : 无
// 输出        : None
// 返回        : None
//******************************************************************************
void usart2_conf(void)
{
  USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);  // 使能PA外设时钟
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);// 使能串口1外设时钟 
  
	/*--------------485控制端初始化------PA1----------*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   //设为输出
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;   //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//50MHzIO口
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_1);// 配置PA2为AF_1外设 
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_1);// 配置PA3为AF_1外设 
  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  //使用外设功能
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
   
  USART_InitStructure.USART_BaudRate = 57600;  //串口速率为115200
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;// 8位数据传输
  USART_InitStructure.USART_StopBits = USART_StopBits_1;// 1个停止位 
  USART_InitStructure.USART_Parity = USART_Parity_No;   // 无校验位
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无流量控制
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART2, &USART_InitStructure);
  
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;  //串口2中断向量
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0;    //中断优先级0
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;    //中断使能
  NVIC_Init(&NVIC_InitStructure);
  
  USART_Cmd(USART2, ENABLE);   //使能串口2
}

//*******************************************************************************
// 函数名称    : void Usart2_Sends(char *str)
// 功能描述    : 串口2发送数据
// 输入        : 无
// 输出        : None
// 返回        : None
//******************************************************************************
void Usart2_Sends(char *str)
{
	while(*str!='\0')
	{
		USART_SendData(USART2, *str);
    while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
		str++;
	}
}

//*******************************************************************************
// 函数名称    : int fputc(int ch,FILE *f)
// 功能描述    : 重定向printf到USART2
// 输入        : ch, *f
// 输出        : None
// 返回        : ch
//******************************************************************************
int fputc(int ch,FILE *f)
{
  USART_SendData(USART2,(unsigned char)ch);//ch送给USART2
	while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
  return(ch);
}

//*******************************************************************************
// 函数名称    : int USART2_Receive(const char *str)
// 功能描述    : 比较USART2接收到的短信和指定信息
// 输入        : *str
// 输出        : None
// 返回        : 0或1
//******************************************************************************
int USART2_Receive(const char *str)
{
	extern char USART2_Rx_Buffer[];
	
	if(strcmp(USART2_Rx_Buffer,str)==0)  return 1;
	else return 0;
}
