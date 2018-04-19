////////////////////////////////////////////////////////////////////
// file name: HW_Battery.c
// 电源管理
////////////////////////////////////////////////////////////////////
// include file declare
#include 	"..\hdr\LPC2294.h"
#include	"..\hdr\config.h" 
#include	"..\hdr\HW_Battery.h"
#include	"..\hdr\HW_GPIO.h"


////////////////////////////////////////////////////////////////////
// 电源电压检测初始化
void MainBatterInit(void){ 

	// bit 25:bit24 = 0:1
	// set p0.28 as AD0.1 input				
	PINSEL1 = (PINSEL1 & 0xfdffffff) | 0x01000000; //P0.28
	
	ADCR0 = (0x2 << 0)	|		// -------------
			((Fpclk / 1000000 - 1) << 8) |
			(0 << 16) |
			(0 << 17) |
			(1 << 21) |
			(0 << 22) |
			(1 << 24) |
			(0 << 27);
}
////////////////////////////////////////////////////////////////////
// 电源电压检测
// 返回 单位 毫伏
unsigned int GetBatteryVoltage(void)
{
	unsigned int iReturn = 0;
	
#if ( HW_ET601)
	ADCR0 = ( 1 << 1 )	|		// -------------
			((Fpclk / 1000000 - 1) << 8) |
			(0 << 16) |
			(0 << 17) |
			(1 << 21) |
			(0 << 22) |
			(1 << 24) |
			(0 << 27);
	OSTimeDlyHMSM(0,0,0,4);			
	iReturn = AD0GDR;
	ADCR0 = ADCR0 | (1 << 24);		
	while( (AD0GDR & BIT31) == 0);	
	
	iReturn = AD0GDR;
	iReturn >>= 6;
	iReturn &= 0x3FF;
	iReturn *= 3300;
	iReturn /= 1024;
			
	return iReturn;
#else
	iReturn = AD0GDR;
	ADCR0 = ADCR0 | (1 << 24);
	while( (AD0GDR & BIT31) == 0);
	
	
	iReturn = AD0GDR;
	iReturn >>= 6;
	iReturn &= 0x3FF;
	iReturn *= 3300;
	iReturn /= 1024;	
#endif 	
	return iReturn;
}

//********************************************************************
// ET601 加速感应器
//
//********************************************************************
#if (HW_ET601)
//////////////////////////////////////////////////////////////////////
//	
//	初始化ADC加速感应器
void  MMA_ADCInit( void ){
	
	//Xout P0.30 as AD0.3 input		29:28  0:1	
	//Yout P0.26 as AD0.5 input		21:20  0:1 	
	//Zout P0.25 as AD0.4 input		19:18  0:1	
	PINSEL1 = (PINSEL1 & 0xEFD7FFFF) | 0x10140000;
	
}
//////////////////////////////////////////////////////////////////////
//	电压
unsigned int	MMA_GetVoltage( unsigned char  nOut )
{
	unsigned char   nBit = 0;
	unsigned int	iReturn = 0;
	
	
	switch( nOut ){
	case MMA_XOUT:
		nBit = 0x03;
		break;
	case MMA_YOUT:
		nBit = 0x05;
		break;
	case MMA_ZOUT:
		nBit = 0x04;
		break;
	}	
	
	ADCR0 = ( 1 << nBit )	|		// -------------
			((Fpclk / 1000000 - 1) << 8) |
			(0 << 16) |
			(0 << 17) |
			(1 << 21) |
			(0 << 22) |
			(1 << 24) |
			(0 << 27);
	OSTimeDlyHMSM(0,0,0,4);
	iReturn = AD0GDR;
	ADCR0 = ADCR0 | (1 << 24);		
	while( (AD0GDR & BIT31) == 0);	
	
	iReturn = AD0GDR;
	iReturn >>= 6;
	iReturn &= 0x3FF;
	iReturn *= 3300;
	iReturn /= 1024;
			
	return iReturn;
}
#endif //HW_ET601

