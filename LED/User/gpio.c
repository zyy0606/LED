//****************************************Copyright (c)**************************************************
//                       基 于 OpenM0V 试 验 板 程 序                            
//
// 这部分程序基于IAR EWARM6.50编译环境
// 
// 芯片型号          : STM32F051R8T6
// 晶振频率          : 8.000000 MHz
//
//--------------文件信息--------------------------------------------------------------------------------
//文   件   名: gpio.c
//创   建   人:喻金钱
//最后修改日期: 2013年11月01日
//描        述: 初始化LED和按键所对应IO口；按键扫描程序；按键处理程序
//              
//店      铺：

//********************************************************************************************************

#include "stm32f0xx.h"
#include "gpio.h"
#include "adc.h"

unsigned char key_bit;

//*******************************************************************************
// 函数名称    : void gpio_conf(void)
// 功能描述    : IO口参数初始化定义函数.定义PB0、1、3、4为输出
//
// 输入        : None
// 输出        : None
// 返回        : None
//******************************************************************************
void gpio_conf(void)
{
  GPIO_InitTypeDef        GPIO_InitStructure;
   /* GPIOB 外设时钟使能 */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

  /* 配置PB3-7外设为输出 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_3 | 
                                GPIO_Pin_4   ;      //外设口线0,1,3,4
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   //设为输出
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;   //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//50MHzIO口
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; //无上拉 
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}
