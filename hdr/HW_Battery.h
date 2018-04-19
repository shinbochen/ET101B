/*
******************************************************************************************
* 电源管理
******************************************************************************************
*/

#include "..\hdr\main.h"

#ifndef BATTERY_H
#define BATTERY_H

#if (HW_ET601)
// MMA_XOUT: Xout  
// MMA_YOUT: Yout 
// MMA_ZOUT:Zout
enum{
	MMA_XOUT = 0x00,
	MMA_YOUT,
	MMA_ZOUT	
};

void  		    MMA_ADCInit( void );
unsigned int	MMA_GetVoltage( unsigned char  nOut );
#endif //HW_ET601

void 					MainBatterInit(void);		
//void 					BackupBatterInit(void);			
unsigned int 			GetBatteryVoltage(void);		
#endif

