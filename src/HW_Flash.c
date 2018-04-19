/*
*******************************************************************************
* Flash��������
*******************************************************************************
*/
#include "..\hdr\config.h"
#include "..\hdr\HW_Flash.h"

/*
*******************************************************************************
* ����ָ��
*******************************************************************************
*/
void (* IAP_Entry)(unsigned int param_tab[], unsigned int result_tab[]);

unsigned int paramin[8];					// IAP��ڲ���������
unsigned int paramout[8];					// IAP���ڲ���������

char  g_RamBuffer[4096];
// zhwei begin
static unsigned int s_uiReturn = 0;
static unsigned int s_uiMul = 0; // ������
static unsigned int s_uiRes = 0; // ����
static unsigned int s_Dst = 0;
static unsigned int s_Src = 0;
static unsigned int s_uiAddrBigin = 0;	// ������ʼ��ַ
// zhwei end
/*
*******************************************************************************
* ����:����ѡ��
* ����:
*	p_SecBegin:	��ʼ����
*	p_SecEnd:	��������
* ����ֵ:
*	IAP״̬��
*******************************************************************************
*/
unsigned int SelSector(unsigned int p_SecBegin, unsigned int p_SecEnd)
{
	paramin[0] = IAP_SELECTOR;
	paramin[1] = p_SecBegin;
	paramin[2] = p_SecEnd;

	OS_ENTER_CRITICAL();
	(* IAP_Entry)(paramin, paramout);
	OS_EXIT_CRITICAL();
	
	return paramout[0];
}

/*
*******************************************************************************
* ����:����RAM���ݵ�FLASH��
* ����:
*	p_Dst:		Ŀ���ַ(512�ı���)
*	p_Src:		Դ��ַ(�ֶ���)
*	p_Count:	����ֱ�Ӹ���(512/1024/4096)
* ����ֵ:
*	IAP״̬��
*******************************************************************************
*/
unsigned int RamToFlash(unsigned int p_Dst, unsigned int p_Src, unsigned int p_Count)
{
	paramin[0] = IAP_RAMTOFLASH;
	paramin[1] = p_Dst;
	paramin[2] = p_Src;
	paramin[3] = p_Count;
	paramin[4] = IAP_FCCLK;	

	OS_ENTER_CRITICAL();
	(* IAP_Entry)(paramin, paramout);
	OS_EXIT_CRITICAL();
	
	return paramout[0];
}

/*
*******************************************************************************
* ����:��������
* ����:
*	p_SecBegin:	��ʼ����
*	p_SecEnd:	��������
* ����ֵ:
*	IAP״̬��
*******************************************************************************
*/
unsigned int EraseSector(unsigned int p_SecBegin, unsigned int p_SecEnd)
{
	paramin[0] = IAP_ERASESECTOR;
	paramin[1] = p_SecBegin;
	paramin[2] = p_SecEnd;
	paramin[3] = IAP_FCCLK;	

	OS_ENTER_CRITICAL();
	(* IAP_Entry)(paramin, paramout);
	OS_EXIT_CRITICAL();
	
	return paramout[0];
}

/*
*******************************************************************************
* ����:�������
* ����:
*	p_SecBegin:	��ʼ����
*	p_SecEnd:	��������
* ����ֵ:
*	IAP״̬��
*******************************************************************************
*/
unsigned int BlankChk(unsigned int p_SecBegin, unsigned int p_SecEnd)
{
	paramin[0] = IAP_BLANKCHK;
	paramin[1] = p_SecBegin;
	paramin[2] = p_SecEnd;

	OS_ENTER_CRITICAL();
	(* IAP_Entry)(paramin, paramout);
	OS_EXIT_CRITICAL();
	
	return paramout[0];
}

/*
*******************************************************************************
* ����:��ȡ����ID
* ����ֵ:
*	IAP״̬��
*******************************************************************************
*/
unsigned int ReadParID(void)
{
	paramin[0] = IAP_READPARTID;

	OS_ENTER_CRITICAL();
	(* IAP_Entry)(paramin, paramout);
	OS_EXIT_CRITICAL();
	
	return paramout[0];
}

/*
*******************************************************************************
* ����:��ȡBoot����汾��
* ����ֵ:
*	IAP״̬��
*******************************************************************************
*/
unsigned int BootCodeID(void)
{
	paramin[0] = IAP_BOOTCODEID;

	OS_ENTER_CRITICAL();
	(* IAP_Entry)(paramin, paramout);
	OS_EXIT_CRITICAL();
	
	return paramout[0];
}

/*
*******************************************************************************
* ����:У������
* ����:
*	p_Dst:		Ŀ���ַ(�ֶ���)
*	p_Src:		Դ��ַ(�ֶ���)
*	p_Count:	У���ֽ���(4�ı���)
* ����ֵ:
*	IAP״̬��
*******************************************************************************
*/
unsigned int Compare(unsigned int p_Dst, unsigned int p_Src, unsigned int p_Count)
{
	paramin[0] = IAP_COMPARE;
	paramin[1] = p_Dst;
	paramin[2] = p_Src;
	paramin[3] = p_Count;

	OS_ENTER_CRITICAL();
	(* IAP_Entry)(paramin, paramout);
	OS_EXIT_CRITICAL();
	
	return paramout[0];
}

/*
*******************************************************************************
* ����:������д��FLASH
* ����:
*	p_Dst:		Ŀ���ַ
*	p_Src:		Դ��ַ
*	p_Count:	Ҫд����ֽ���(С��512�ֽ�)
* ����ֵ:
*	�ɹ����� 0, ʧ�ܷ��� ������
*******************************************************************************
*/
// zhwei begin
unsigned int WriteDataToFlash(void * p_Dst1, void * p_Src1, unsigned int p_Count)
{
	s_uiReturn = 0;
	
	s_Dst = (unsigned int)p_Dst1;
	s_Src = (unsigned int)p_Src1;

	if (p_Count == 0)
		return s_uiReturn;
	
	IAP_Entry = (void( * )())IAP_ENTER_ADR;

	if (s_Dst <= 0x0fff)
	{
		s_uiMul = 0;
		s_uiRes = s_Dst - 0;
		s_uiAddrBigin = 0;
	}
	else if ((s_Dst >= 0x1000) && (s_Dst <= 0x1fff))
	{
		s_uiMul = 1;
		s_uiRes = s_Dst - 0x1000;
		s_uiAddrBigin = 0x1000;
	}
	else if ((s_Dst >= 0x2000) && (s_Dst <= 0x2fff))
	{
		s_uiMul = 2;
		s_uiRes = s_Dst - 0x2000;
		s_uiAddrBigin = 0x2000;
	}
	else if ((s_Dst >= 0x3000) && (s_Dst <= 0x3fff))
	{
		s_uiMul = 3;
		s_uiRes = s_Dst - 0x3000;
		s_uiAddrBigin = 0x3000;
	}
	else if ((s_Dst >= 0x4000) && (s_Dst <= 0x4fff))
	{
		s_uiMul = 4;
		s_uiRes = s_Dst - 0x4000;
		s_uiAddrBigin = 0x4000;
	}
	else if ((s_Dst >= 0x5000) && (s_Dst <= 0x5fff)) 
	{
		s_uiMul = 5;
		s_uiRes = s_Dst - 0x5000;
		s_uiAddrBigin = 0x5000; 
	}
	else if ((s_Dst >= 0x6000) && (s_Dst <= 0x6fff))
	{
		s_uiMul = 6;
		s_uiRes = s_Dst - 0x6000;
		s_uiAddrBigin = 0x6000;
	}
	else if ((s_Dst >= 0x7000) && (s_Dst <= 0x7fff))
	{
		s_uiMul = 7;
		s_uiRes = s_Dst - 0x7000;
		s_uiAddrBigin = 0x7000;
	}
	else if ((s_Dst >= 0x8000) && (s_Dst <= 0xffff))
	{
		s_uiMul = 8;
		s_uiRes = s_Dst - 0x8000;
		s_uiAddrBigin = 0x8000;
	}		
	else if ((s_Dst >= 0x10000) && (s_Dst <= 0x17fff))
	{
		s_uiMul = 9;
		s_uiRes = s_Dst - 0x10000;
		s_uiAddrBigin = 0x10000;
	}
	else if ((s_Dst >= 0x18000) && (s_Dst <= 0x1ffff))
	{
		s_uiMul = 10;
		s_uiRes = s_Dst - 0x18000;
		s_uiAddrBigin = 0x18000;
	}	
	else
	{
		return (unsigned int)1;
	}
//	s_uiMul = s_Dst / 8192;
//	s_uiRes = s_Dst % 8192;

	while (1)
	{
																	// Ĭ����һ��������
		memcpy((void *)g_RamBuffer, (void *)(s_uiAddrBigin), 4096);
		memcpy((void *)(g_RamBuffer + s_uiRes), (void *)s_Src, p_Count);

		s_uiReturn = SelSector(s_uiMul, s_uiMul);
		if (s_uiReturn != CMD_SUCCESS)
			break;
		s_uiReturn = EraseSector(s_uiMul, s_uiMul);
		if (s_uiReturn != CMD_SUCCESS)
			break;
		s_uiReturn = SelSector(s_uiMul, s_uiMul);
		if (s_uiReturn != CMD_SUCCESS)
			break;
		s_uiReturn = RamToFlash(s_uiAddrBigin, (unsigned int)(&g_RamBuffer[0]), 4096);
		if (s_uiReturn != CMD_SUCCESS)
			break;
		s_uiReturn = Compare(s_uiAddrBigin, (unsigned int)(&g_RamBuffer[0]), 4096);
		if (s_uiReturn != CMD_SUCCESS)
			break;
		break;
	}

	return s_uiReturn;
}
// zhwei end
