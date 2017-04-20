
#include "stm32f0xx.h"
#include "soc.h"

double g_slAS = 100;//��ǰ����
unsigned int uiCapType = 100;   //��ض����100AH
TSOCPARA g_SOCPara;

void CalSoc(double iSLA)
{
	double ulTemp = 0;
	unsigned char ucSOC;    //ʣ����� 1%/λ  
	
	if( iSLA < 0 )  //����Ϊ�������طŵ�
	{
		ulTemp = (-1) * iSLA;
		ulTemp *= 0.1;  //0.1s
		ulTemp /= 3600;//As => Ah
		g_slAS -= ulTemp;//�ۼ��ϵ�� �ŵ� ����
	}
	
	else
	{
		ulTemp *= 0.1;  //0.1s
		ulTemp /= 3600;//As => Ah
		g_slAS += ulTemp;//�ۼ��ϵ� ��� ����
	}

	//��ǰ����  < 0
	if( g_slAS < 0 )  
	{
		g_slAS = 0;
	}

	if( g_slAS >= uiCapType)   //��ǰ�������ڶ����
	{
		g_slAS = uiCapType;//��ǰ���� = �����
	}

	ucSOC = 100*g_slAS/uiCapType;
	
	g_SOCPara.ulAs = g_slAS;
	g_SOCPara.ucSOC = ucSOC;
}
