//////////////////////////////////////////////////////////////////////////
// file name: HW_SPIFLASH.h
// Date: 2009-12-28
// Author: wyb 

#include "..\hdr\main.h"

#ifndef _HW_SPIFLASH_H_
#define _HW_SPIFLASH_H_

#if	(EXTERNAL_FLASH)
 
#if (ATMEL_FLASH) //ATMEL厂家芯片
 
#define		PAGE_BLOCKSIZE			   512	

// SPI_SPCR 控制寄存器
#define  SPCR_CPHA		0x08		//时钟相位控制
#define	 SPCR_CPOL		0x10		//时钟极性控制
#define	 SPCR_MSTR		0x20		//主模式控制 1:主模式 0:从模式
#define	 SPCR_LSBF		0x40		//字节移动方向控制
#define	 SPCR_SPIE		0x80		//SPI中断使能 1:SPIF或MODF置位产生中断  0:中断禁止


// AT45DB321D芯片指令
#define	BUFFER_1_WRITE 				  0x84					// 写入第一缓冲区
#define BUFFER_2_WRITE 				  0x87					// 写入第二缓冲区
#define BUFFER_1_READ 				  0xD4					// 读取第一缓冲区
#define BUFFER_2_READ 				  0xD6					// 读取第二缓冲区
#define B1_TO_MM_PAGE_PROG_WITH_ERASE 0x83					// 将第一缓冲区的数据写入主存储器（擦除模式）
#define B2_TO_MM_PAGE_PROG_WITH_ERASE 0x86					// 将第二缓冲区的数据写入主存储器（擦除模式）
#define MM_PAGE_TO_B1_XFER 			  0x53					// 将主存储器的指定页数据加载到第一缓冲区
#define MM_PAGE_TO_B2_XFER 			  0x55					// 将主存储器的指定页数据加载到第二缓冲区
#define PAGE_ERASE 					  0x81					// 页删除（每页512/528字节）
#define SECTOR_ERASE 				  0x7C					// 扇区擦除（每扇区64KByte）
#define READ_STATE_REGISTER 		  0xD7					// 读取状态寄存器
#define READ_DEVICE_ID				  0x9F					// 制造商和设备读


//  SPI口的初始化
char  SPI0Init( void );
// AT45DB321D芯片指令
void SPI0_wait_busy(void);
void SPI0_Page_Earse(unsigned short nPage);
void SPI0_Write_Page( unsigned short nPage, unsigned short  nPos ,unsigned char	*pSrc, unsigned short nLen );
void SPI0_Read_Page(unsigned short nPage, unsigned short  nPos,unsigned char	*pDest, unsigned short nLen );
unsigned char SPI0_Read_Reg(void);
void 	SPI0_ReadDeviceID( unsigned char  *pDest );

//  把数据写入FALSH
void WriteDataToFlash2( unsigned int	nDest, unsigned char	*pSrc, unsigned int nDataLen );
//  把数据读取FALSH数据
void ReadDataFromFlash( unsigned char *pDest, unsigned int  nSrc, unsigned int nDataLen );

#endif //ATMEL_FLASH


/*****************************************************************************
**
**
*****************************************************************************/
#if( WINBAND_FLASH )

// SPI_SPCR 控制寄存器
#define  SPCR_CPHA		0x08		//时钟相位控制
#define	 SPCR_CPOL		0x10		//时钟极性控制
#define	 SPCR_MSTR		0x20		//主模式控制 1:主模式 0:从模式
#define	 SPCR_LSBF		0x40		//字节移动方向控制
#define	 SPCR_SPIE		0x80		//SPI中断使能 1:SPIF或MODF置位产生中断  0:中断禁止


#define   READ_DEVICE_ID				0x90   //读取厂家设备ID
#define	  READ_STATUS_REGISTER_1		0x05   //状态寄存器1	
#define	  READ_STATUS_REGISTER_2		0x35   //状态寄存器2
#define	  READ_SPI_DATA					0x03   //读取存储器数据


#define	  ENABLE_WRITE_FUNCTION			0x06   //开启写功能
#define   DISABLE_WRITE_FUNCTION		0x04   //关闭写功能

#define	  WRITE_PAGE_DATA				0x02   //写页数据
#define	  SECTOR_ERASE_DATA				0x20   //扇区删除指令,大小为4K
#define   CHIP_ERASE_DATA				0xC7   //整个FLASH清除

//  SPI口的初始化
char 	 SPI0Init( void );
// AT45DB321D芯片指令
void SPI0_wait_busy(void);
//  把数据写入FALSH
void	WriteDataToFlash2( unsigned int	nDest, unsigned char	*pSrc, unsigned int nDataLen );
//  把数据读取FALSH数据
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
