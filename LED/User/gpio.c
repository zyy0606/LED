//****************************************Copyright (c)**************************************************
//                       �� �� OpenM0V �� �� �� �� ��                            
//
// �ⲿ�ֳ������IAR EWARM6.50���뻷��
// 
// оƬ�ͺ�          : STM32F051R8T6
// ����Ƶ��          : 8.000000 MHz
//
//--------------�ļ���Ϣ--------------------------------------------------------------------------------
//��   ��   ��: gpio.c
//��   ��   ��:����Ǯ
//����޸�����: 2013��11��01��
//��        ��: ��ʼ��LED�Ͱ�������ӦIO�ڣ�����ɨ����򣻰����������
//              
//��      �̣�

//********************************************************************************************************

#include "stm32f0xx.h"
#include "gpio.h"
#include "adc.h"

unsigned char key_bit;

//*******************************************************************************
// ��������    : void gpio_conf(void)
// ��������    : IO�ڲ�����ʼ�����庯��.����PB0��1��3��4Ϊ���
//
// ����        : None
// ���        : None
// ����        : None
//******************************************************************************
void gpio_conf(void)
{
  GPIO_InitTypeDef        GPIO_InitStructure;
   /* GPIOB ����ʱ��ʹ�� */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

  /* ����PB3-7����Ϊ��� */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_3 | 
                                GPIO_Pin_4   ;      //�������0,1,3,4
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   //��Ϊ���
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;   //�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//50MHzIO��
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; //������ 
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}
