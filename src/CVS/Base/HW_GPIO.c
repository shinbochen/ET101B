///////////////////////////////////////////////////////////////////////////////
// file name	: hw_gpio.c
// function		: GPIO驱动
// Remark			: 20090204 optimize by shinbo
///////////////////////////////////////////////////////////////////////////////
// include file declare

#include "..\hdr\config.h"
#include "..\hdr\HW_GPIO.h"

///////////////////////////////////////////////////////////////////////////////
// function 使能IO口
// 参数:
// nPortNo:	IO口编号 p0.1为1,p1.1为101,p2.1为201
// PINSELX      
// bit0:bit1    0:0(GPIOPx.0)
// Bit2:bit3    0:0(GPIOPx.1)
void GPIO_Enable(unsigned char nPortNo){	
	
	if (nPortNo < 16)
	{
		PINSEL0 = PINSEL0 & ((0x3 << (nPortNo * 2)) ^ 0xffffffff);
	}
	else if(nPortNo < 32)
	{
		PINSEL1 = PINSEL1 & ((0x3 << ((nPortNo - 16) * 2)) ^ 0xffffffff);
	}
	else if ((nPortNo >= 116) && (nPortNo <= 125))
	{
		PINSEL2 = PINSEL2 & ((0x3 << ((nPortNo - 100) * 2)) ^ 0xffffffff);
	}
	return;
}

///////////////////////////////////////////////////////////////////////////////
// function IO方向设置
// 参数: 
// nPortNo:	IO口编号 p0.1为1,p1.1为101,p2.1为201
// bDirFlag:	方向 0,输入 1输出

void GPIO_SetDirection(unsigned char nPortNo, unsigned char bDirFlag){
	
	if (nPortNo < 100)
	{
		if (bDirFlag)
			IO0DIR = IO0DIR | (0x1 << nPortNo);
		else
			IO0DIR = IO0DIR & ((0x1 << nPortNo) ^ 0xffffffff);
	}
	else if ((nPortNo >= 116) && (nPortNo <= 125))
	{
		if (bDirFlag)
			IO1DIR = IO1DIR | (0x1 << (nPortNo - 100));
		else
			IO1DIR = IO1DIR & ((0x1 << (nPortNo - 100)) ^ 0xffffffff);
	}		
	return;
}

///////////////////////////////////////////////////////////////////////////////
// 	function 获取电平状态
// 	参数:
//	nPortNo:	IO口编号 p0.1为1,p1.1为101,p2.1为201
//  返回值:
//	低电平返回 0
//	高电平返回 1

unsigned char GPIO_Get(unsigned char nPortNo)
{
	unsigned char nReturn = 0;
	
	if (nPortNo < 100)
	{
		if ((IO0PIN & (0x1 << nPortNo)) != 0)
			nReturn = 1;
	}
	else if ((nPortNo >= 116) && (nPortNo <= 125))
	{
		if ((IO1PIN & (0x1 << (nPortNo - 100))) != 0)
			nReturn = 1;
	}
	return nReturn;
}

///////////////////////////////////////////////////////////////////////////////
// function 设置高低电平输出
// 参数:
//	nPortNo:		IO口编号 p0.1为1,p1.1为101,p2.1为201
//	nLevel:	0为低电平; 1为高电平

void GPIO_Set(unsigned char nPortNo, unsigned char nLevel ){
		
	if (nPortNo < 100)
	{
		if (nLevel)
			IO0SET = 0x1 << nPortNo;
		else
			IO0CLR = 0x1 << nPortNo;
	}
	else if ((nPortNo >= 116) && (nPortNo <= 125))
	{
		if (nLevel)
			IO1SET = 0x1 << (nPortNo - 100);
		else
			IO1CLR = 0x1 << (nPortNo - 100);
	}
	return;
}
