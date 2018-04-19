////////////////////////////////////////////////////////////////////
// file name: HW_Battery.c
// ��Դ����
////////////////////////////////////////////////////////////////////
// include file declare
#include 	"..\hdr\LPC2294.h"
#include	"..\hdr\config.h" 
#include	"..\hdr\HW_Battery.h"
#include	"..\hdr\HW_GPIO.h"

////////////////////////////////////////////////////////////////////
// ��Դ��ѹ����ʼ��
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
// ��Դ��ѹ���
// ���� ��λ ����
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
// �󱸵�ص�ѹ���	
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