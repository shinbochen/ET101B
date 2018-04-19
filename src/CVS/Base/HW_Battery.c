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
			
	PINSEL1 = (PINSEL1 & 0xfdffffff) | 0x01000000;	//P0.28

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

/*
////////////////////////////////////////////////////////////////////
// 后备电池电压检测	
void BackupBatterInit(void) { 

	// bit 27:bit26 = 0:1
	// set p0.29 as AD0.2 input
	PINSEL1 &= (~BIT27);
	PINSEL1 |= BIT26;
	
	ADCR0 = (1 << 2)  |		
			((Fpclk / 1000000 - 1) << 8) |
			(0 << 16) |
			(0 << 17) |
			(1 << 21) |
			(0 << 22) |
			(1 << 24) |
			(0 << 27);
}
*/