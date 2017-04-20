
#include "stm32f0xx.h"
#include "soc.h"

double g_slAS = 100;//当前容量
unsigned int uiCapType = 100;   //电池额定容量100AH
TSOCPARA g_SOCPara;

void CalSoc(double iSLA)
{
	double ulTemp = 0;
	unsigned char ucSOC;    //剩余电量 1%/位  
	
	if( iSLA < 0 )  //电流为负，蓄电池放电
	{
		ulTemp = (-1) * iSLA;
		ulTemp *= 0.1;  //0.1s
		ulTemp /= 3600;//As => Ah
		g_slAS -= ulTemp;//累计上电后 放电 容量
	}
	
	else
	{
		ulTemp *= 0.1;  //0.1s
		ulTemp /= 3600;//As => Ah
		g_slAS += ulTemp;//累计上电 充电 容量
	}

	//当前容量  < 0
	if( g_slAS < 0 )  
	{
		g_slAS = 0;
	}

	if( g_slAS >= uiCapType)   //当前容量大于额定容量
	{
		g_slAS = uiCapType;//当前容量 = 额定容量
	}

	ucSOC = 100*g_slAS/uiCapType;
	
	g_SOCPara.ulAs = g_slAS;
	g_SOCPara.ucSOC = ucSOC;
}
