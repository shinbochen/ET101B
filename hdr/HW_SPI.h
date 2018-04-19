//////////////////////////////////////////////////////////////////////////
// file name: HW_SPIFLASH.h
// Date: 2009-12-28
// Author: wyb 

#include "..\hdr\main.h"

#ifndef _HW_SPIFLASH_H_
#define _HW_SPIFLASH_H_

#if	(EXTERNAL_FLASH)
 
#if (ATMEL_FLASH) //ATMEL����оƬ
 
#define		PAGE_BLOCKSIZE			   512	

// SPI_SPCR ���ƼĴ���
#define  SPCR_CPHA		0x08		//ʱ����λ����
#define	 SPCR_CPOL		0x10		//ʱ�Ӽ��Կ���
#define	 SPCR_MSTR		0x20		//��ģʽ���� 1:��ģʽ 0:��ģʽ
#define	 SPCR_LSBF		0x40		//�ֽ��ƶ��������
#define	 SPCR_SPIE		0x80		//SPI�ж�ʹ�� 1:SPIF��MODF��λ�����ж�  0:�жϽ�ֹ


// AT45DB321DоƬָ��
#define	BUFFER_1_WRITE 				  0x84					// д���һ������
#define BUFFER_2_WRITE 				  0x87					// д��ڶ�������
#define BUFFER_1_READ 				  0xD4					// ��ȡ��һ������
#define BUFFER_2_READ 				  0xD6					// ��ȡ�ڶ�������
#define B1_TO_MM_PAGE_PROG_WITH_ERASE 0x83					// ����һ������������д�����洢��������ģʽ��
#define B2_TO_MM_PAGE_PROG_WITH_ERASE 0x86					// ���ڶ�������������д�����洢��������ģʽ��
#define MM_PAGE_TO_B1_XFER 			  0x53					// �����洢����ָ��ҳ���ݼ��ص���һ������
#define MM_PAGE_TO_B2_XFER 			  0x55					// �����洢����ָ��ҳ���ݼ��ص��ڶ�������
#define PAGE_ERASE 					  0x81					// ҳɾ����ÿҳ512/528�ֽڣ�
#define SECTOR_ERASE 				  0x7C					// ����������ÿ����64KByte��
#define READ_STATE_REGISTER 		  0xD7					// ��ȡ״̬�Ĵ���
#define READ_DEVICE_ID				  0x9F					// �����̺��豸��


//  SPI�ڵĳ�ʼ��
char  SPI0Init( void );
// AT45DB321DоƬָ��
void SPI0_wait_busy(void);
void SPI0_Page_Earse(unsigned short nPage);
void SPI0_Write_Page( unsigned short nPage, unsigned short  nPos ,unsigned char	*pSrc, unsigned short nLen );
void SPI0_Read_Page(unsigned short nPage, unsigned short  nPos,unsigned char	*pDest, unsigned short nLen );
unsigned char SPI0_Read_Reg(void);
void 	SPI0_ReadDeviceID( unsigned char  *pDest );

//  ������д��FALSH
void WriteDataToFlash2( unsigned int	nDest, unsigned char	*pSrc, unsigned int nDataLen );
//  �����ݶ�ȡFALSH����
void ReadDataFromFlash( unsigned char *pDest, unsigned int  nSrc, unsigned int nDataLen );

#endif //ATMEL_FLASH


/*****************************************************************************
**
**
*****************************************************************************/
#if( WINBAND_FLASH )

// SPI_SPCR ���ƼĴ���
#define  SPCR_CPHA		0x08		//ʱ����λ����
#define	 SPCR_CPOL		0x10		//ʱ�Ӽ��Կ���
#define	 SPCR_MSTR		0x20		//��ģʽ���� 1:��ģʽ 0:��ģʽ
#define	 SPCR_LSBF		0x40		//�ֽ��ƶ��������
#define	 SPCR_SPIE		0x80		//SPI�ж�ʹ�� 1:SPIF��MODF��λ�����ж�  0:�жϽ�ֹ


#define   READ_DEVICE_ID				0x90   //��ȡ�����豸ID
#define	  READ_STATUS_REGISTER_1		0x05   //״̬�Ĵ���1	
#define	  READ_STATUS_REGISTER_2		0x35   //״̬�Ĵ���2
#define	  READ_SPI_DATA					0x03   //��ȡ�洢������


#define	  ENABLE_WRITE_FUNCTION			0x06   //����д����
#define   DISABLE_WRITE_FUNCTION		0x04   //�ر�д����

#define	  WRITE_PAGE_DATA				0x02   //дҳ����
#define	  SECTOR_ERASE_DATA				0x20   //����ɾ��ָ��,��СΪ4K
#define   CHIP_ERASE_DATA				0xC7   //����FLASH���

//  SPI�ڵĳ�ʼ��
char 	 SPI0Init( void );
// AT45DB321DоƬָ��
void SPI0_wait_busy(void);
//  ������д��FALSH
void	WriteDataToFlash2( unsigned int	nDest, unsigned char	*pSrc, unsigned int nDataLen );
//  �����ݶ�ȡFALSH����
void 	ReadDataFromFlash( unsigned char *pDest, unsigned int  nSrc, unsigned int nDataLen );


void    SPIO_ReadDeviceID( unsigned char  *pDest );
void    SPI0_wait_busy( void );
void    SPIO_EnableWrite( void );
void 	SPI0_Write_Page( unsigned int nStartAddr, unsigned char	*pSrc, unsigned short nLen );
void 	SPI0_Read_Page( unsigned char	*pDest,unsigned int nStartAddr, unsigned int nLen );
void  	SPI0_Sector_Earse( unsigned int nStartAddr );
void    SPI0_Chip_Earse( void );

#endif //WINBAND_FLASH

#endif //EXTERNAL_FLASH

#endif //_HW_SPIFLASH_H_
