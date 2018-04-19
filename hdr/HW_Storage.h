//////////////////////////////////////////////////////////////////////////////
// Flash 存储数据程序
// Filename:HW_storage.h
  
#include "..\hdr\main.h"

#ifndef HW_STORAGE_20081027_H
#define HW_STORAGE_20081027_H

/*
index area
-------------------------------------------------------------------------
|len |free_len|write_index| read_index| i1 | i2 | i3 | i4 | i5	| i6 |...|
-------------------------------------------------------------------------


data area
-----------------------------------------------------------------
|       |       |       |       |       |       |       |       |
----------------------------------------------------------------- 
|       |       |       |       |       |       |       |       |
----------------------------------------------------------------- 
|       |       |       |       |       |       |       |       |
----------------------------------------------------------------- 
|       |       |       |       |       |       |       |       |
----------------------------------------------------------------- 
*/
#if	(EXTERNAL_FLASH)

#ifndef		BLOCKSIZE
#define		BLOCKSIZE										512	
#endif

#define	GPSDATASIZE											32		
//#define	MAX_GPS											65456		//(2094592/32)
#define	MAX_GPS												65408		//1FF000 2093056/32=

#define	GPSINDEX_AREA_SIZE									0x800					

// 0x00 -0x7FF
//#define	HW_FLASH_SETUP_START		0x00
// 0x800- 0x9FF
#define	HW_FLASH_GPSINDEXAREA_START		0x000800
// 0xA00-0x1FFFFF = 1FF600 = 2094592
#define	HW_FLASH_GPSDATAAREA_START		HW_FLASH_GPSINDEXAREA_START+GPSINDEX_AREA_SIZE


typedef	struct _STGPSINDEX{
	unsigned short		m_nGPSLen;			// GPS总数目
	unsigned short		m_nFreeGPSLen;		// 空闲块数目	
	unsigned short		m_nWriteIndex;		// 写指针
	unsigned short		m_nReadIndex;		// 读指针
}STGPSINDEX,*PSTGPSINDEX;


///////////////////////////////////////////////////////////////////
// function declare
// Initall Index Area
void	InitGPSStorageIndex(void );
//////////////////////////////////////////////////////////////////////////////
// 存入数据
// 成功返回1
// 失败返回-1
int	PushGPSData( unsigned char *pData, int nSize );
//////////////////////////////////////////////////////////////////////////////
// 取出数据
// nAmount: 可以同时取多个数据
// 成功返回得到的数据长度
// 失败返回0
int	PopGPSData( unsigned char *pData, int	nAmount );
// 读索引
void ReadGPSIndex( PSTGPSINDEX pStGPSIndex );

#endif //EXTERNAL_FLASH

#endif
