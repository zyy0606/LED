#include "stm32f0xx.h"
#include "usart.h"
#include "stdio.h"
#include <string.h>
//*******************************************************************************
// ��������    : void usart1_conf(void)
// ��������    : ��ʼ������1.
// ����        : ��
// ���        : None
// ����        : None
//******************************************************************************
void usart1_conf(void)
{
  USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);  // ʹ��PB����ʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);// ʹ�ܴ���1����ʱ�� 
  
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_0);// ����PB6ΪAF_1���� 
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_0);// ����PB7ΪAF_1���� 
  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  //ʹ�����蹦��
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
   
  USART_InitStructure.USART_BaudRate = 115200;  //��������Ϊ9600
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;// 8λ���ݴ���
  USART_InitStructure.USART_StopBits = USART_StopBits_1;// 1��ֹͣλ 
  USART_InitStructure.USART_Parity = USART_Parity_No;   // ��У��λ
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//����������
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART1, &USART_InitStructure);
  
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;  //����1�ж�����
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0;    //�ж����ȼ�0
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;    //�ж�ʹ��
  NVIC_Init(&NVIC_InitStructure);
  
  USART_Cmd(USART1, ENABLE);   //ʹ�ܴ���1
}

//*******************************************************************************
// ��������    : void Usart1_Sends(void)
// ��������    : �������ݴ�
// ����        : data[]
// ���        : None
// ����        : None
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
// ��������    : void usart2_conf(void)
// ��������    : ��ʼ������1.
// ����        : ��
// ���        : None
// ����        : None
//******************************************************************************
void usart2_conf(void)
{
  USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);  // ʹ��PA����ʱ��
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);// ʹ�ܴ���1����ʱ�� 
  
	/*--------------485���ƶ˳�ʼ��------PA1----------*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   //��Ϊ���
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;   //�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//50MHzIO��
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_1);// ����PA2ΪAF_1���� 
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_1);// ����PA3ΪAF_1���� 
  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  //ʹ�����蹦��
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
   
  USART_InitStructure.USART_BaudRate = 57600;  //��������Ϊ115200
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;// 8λ���ݴ���
  USART_InitStructure.USART_StopBits = USART_StopBits_1;// 1��ֹͣλ 
  USART_InitStructure.USART_Parity = USART_Parity_No;   // ��У��λ
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//����������
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART2, &USART_InitStructure);
  
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;  //����2�ж�����
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0;    //�ж����ȼ�0
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;    //�ж�ʹ��
  NVIC_Init(&NVIC_InitStructure);
  
  USART_Cmd(USART2, ENABLE);   //ʹ�ܴ���2
}

//*******************************************************************************
// ��������    : void Usart2_Sends(char *str)
// ��������    : ����2��������
// ����        : ��
// ���        : None
// ����        : None
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
// ��������    : int fputc(int ch,FILE *f)
// ��������    : �ض���printf��USART2
// ����        : ch, *f
// ���        : None
// ����        : ch
//******************************************************************************
int fputc(int ch,FILE *f)
{
  USART_SendData(USART2,(unsigned char)ch);//ch�͸�USART2
	while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
  return(ch);
}

//*******************************************************************************
// ��������    : int USART2_Receive(const char *str)
// ��������    : �Ƚ�USART2���յ��Ķ��ź�ָ����Ϣ
// ����        : *str
// ���        : None
// ����        : 0��1
//******************************************************************************
int USART2_Receive(const char *str)
{
	extern char USART2_Rx_Buffer[];
	
	if(strcmp(USART2_Rx_Buffer,str)==0)  return 1;
	else return 0;
}
