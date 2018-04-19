/*
*******************************************************************************
* Flash��������
*******************************************************************************
*/
#ifndef FLASH_H
#define FLASH_H

#define IAP_ENTER_ADR	0x7ffffff1		// IAP��ڵ�ַ

/*
********************************************************************************
* IAP������
********************************************************************************
*/
#define IAP_SELECTOR	50				// ѡ������
#define IAP_RAMTOFLASH	51				// ��������
#define IAP_ERASESECTOR	52				// ��������
#define IAP_BLANKCHK	53				// �������
#define IAP_READPARTID	54				// ������ID
#define IAP_BOOTCODEID	55				// �������汾��
#define IAP_COMPARE		56				// �Ƚ�

/*
********************************************************************************
* IAP����״̬��
********************************************************************************
*/
#define CMD_SUCCESS			0
#define INVALID_COMMAND		1
#define SRC_ADDR_ERROR		2
#define DST_ADDR_ERROR		3
#define SRC_ADDR_NOT_MAPPED	4
#define DST_ADDR_NOT_MAPPED	5
#define COUNT_ERROR			6
#define INVALID_SECTOR		7
#define SECTOR_NOT_BLANK	8
#define SECTOR_NOT_PREPARED_FOR_WRITE_OPERATION	9
#define COMPARE_ERROR		10
#define BUSY				11

/*
********************************************************************************
* ����CCLKֵ(k)
********************************************************************************
*/
#define IAP_FCCLK			11059

extern	char  g_RamBuffer[4096];

/*
********************************************************************************
* �ӿں���
********************************************************************************
*/
unsigned int SelSector(unsigned int p_SecBegin, unsigned int p_SecEnd);
unsigned int RamToFlash(unsigned int p_Dst, unsigned int p_Src, unsigned int p_Count);
unsigned int EraseSector(unsigned int p_SecBegin, unsigned int p_SecEnd);
unsigned int BlankChk(unsigned int p_SecBegin, unsigned int p_SecEnd);
unsigned int ReadParID(void);
unsigned int BootCodeID(void);
unsigned int Compare(unsigned int p_Dst, unsigned int p_Src, unsigned int p_Count);
unsigned int WriteDataToFlash(void * p_Dst1, void * p_Src1, unsigned int p_Count);

#endif
