/****************************************Copyright (c)**************************************************
**描        述: lpc22xx（飞利浦的ARM）目标板特殊的代码头文件
**              每个工程应当具有这个文件的拷贝，用户根据程序的需要修改本文件
********************************************************************************************************/
#ifndef IN_TARGET 
#define IN_TARGET 

void Reset(void);
void InitHardware(void);
void SW_WatchdogEnable(void);
void SW_Watchdog(void);
#endif