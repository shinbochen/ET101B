///////////////////////////////////////////////////////////////////////////////
// file name	: hw_gpio.c
// function		: GPIO����
// Remark			: 20090204 optimize by shinbo
///////////////////////////////////////////////////////////////////////////////
// include file declare

#include "..\hdr\config.h"
#include "..\hdr\HW_GPIO.h"

///////////////////////////////////////////////////////////////////////////////
// function ʹ��IO��
// ����:
// nPortNo:	IO�ڱ�� p0.1Ϊ1,p1.1Ϊ101,p2.1Ϊ201
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
// function IO��������
// ����: 
// nPortNo:	IO�ڱ�� p0.1Ϊ1,p1.1Ϊ101,p2.1Ϊ201
// bDirFlag:	���� 0,���� 1���

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
// 	function ��ȡ��ƽ״̬
// 	����:
//	nPortNo:	IO�ڱ�� p0.1Ϊ1,p1.1Ϊ101,p2.1Ϊ201
//  ����ֵ:
//	�͵�ƽ���� 0
//	�ߵ�ƽ���� 1

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
// function ���øߵ͵�ƽ���
// ����:
//	nPortNo:		IO�ڱ�� p0.1Ϊ1,p1.1Ϊ101,p2.1Ϊ201
//	nLevel:	0Ϊ�͵�ƽ; 1Ϊ�ߵ�ƽ

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
