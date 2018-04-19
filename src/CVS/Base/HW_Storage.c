//////////////////////////////////////////////////////////////////////////////
// Flash 存储数据程序
// Filename:HW_storage.c
 
#include "..\hdr\HW_Storage.h" 
#include "..\hdr\HW_SPI.h" 
#include <cstring>

#ifdef		EXTERNAL_FLASH

//////////////////////////////////////////////////////////////////////////////
// 读索引
void ReadGPSIndex( PSTGPSINDEX pStGPSIndex ){
		ReadDataFromFlash( (unsigned char*)pStGPSIndex, HW_FLASH_GPSINDEXAREA_START,  sizeof(STGPSINDEX) );
}
//////////////////////////////////////////////////////////////////////////////
// 写索引
void WriteGPSIndex( PSTGPSINDEX pStGPSIndex ){
		WriteDataToFlash2( HW_FLASH_GPSINDEXAREA_START, (unsigned char*)pStGPSIndex, sizeof(STGPSINDEX) );	
}
//////////////////////////////////////////////////////////////////////////////
//
unsigned int GetGPSDataAddr( unsigned int nIndex ){
	
	unsigned int		lAddr = HW_FLASH_GPSDATAAREA_START;
		
	lAddr += nIndex*(unsigned int)GPSDATASIZE;	
	return lAddr;	
}
//////////////////////////////////////////////////////////////////////////////
// Initall Index Area
void	InitGPSStorageIndex(void){
	
	
	STGPSINDEX				stGPSIndex;
	
	stGPSIndex.m_nGPSLen = 0;
	stGPSIndex.m_nFreeGPSLen = MAX_GPS;
	stGPSIndex.m_nWriteIndex = 0;
	stGPSIndex.m_nReadIndex = 0;
	
	WriteGPSIndex( &stGPSIndex );
		
	/*
	unsigned short		nData[BLOCKSIZE/2];
	unsigned short		nGPSIndex;
	unsigned short		nSize;
	unsigned short		nBlockCnt;
	STGPSINDEX				stGPSIndex;
	
	
	
	nGPSIndex = 0;
	nBlockCnt = 0;
	
	for( nBlockCnt = 0; nBlockCnt < GPSINDEX_AREA_SIZE/BLOCKSIZE; nBlockCnt++ ){
		if( nBlockCnt == 0 ){
			stGPSIndex.m_nGPSLen = 0;
			stGPSIndex.m_nFreeGPSLen = MAX_GPS;
			stGPSIndex.m_nWriteIndex = 0;
			stGPSIndex.m_nReadIndex = 0;
			
			nSize = sizeof( STGPSINDEX );
			memcpy( (char*)nData, (char*)&stGPSIndex, nSize );
			
			nSize /= 2;
			
			for( nCnt = nSize; nCnt < BLOCKSIZE/2; nCnt++ ){
				nData[nCnt] = nGPSIndex++;
			}
		}
		else{
			for( nCnt = 0; nCnt < BLOCKSIZE/2; nCnt++ ){
				nData[nCnt] = nGPSIndex++;
			}
		}
		WriteDataToFlash2( HW_FLASH_GPSINDEXAREA_START+nBlockCnt*BLOCKSIZE, (char*)nData, BLOCKSIZE );
	}*/
	return;
}


//////////////////////////////////////////////////////////////////////////////
// 存入数据
// 成功返回1
// 失败返回-1
int	PushGPSData( unsigned char *pData, int nSize ){	
	
//	int						nLen;
//	int						nIndex;
	STGPSINDEX		stGPSIndex;
	
	
	ReadGPSIndex( &stGPSIndex );
		
	if( nSize > GPSDATASIZE ){
		nSize = GPSDATASIZE;
	}
	
	if( stGPSIndex.m_nGPSLen >= MAX_GPS ||
			stGPSIndex.m_nFreeGPSLen  == 0	 ){
		return -1;
	}
	
	stGPSIndex.m_nGPSLen++;
	stGPSIndex.m_nFreeGPSLen--;
		
	WriteDataToFlash2( /*(unsigned char*)*/GetGPSDataAddr(stGPSIndex.m_nWriteIndex), pData, (unsigned int)nSize );
	stGPSIndex.m_nWriteIndex++;
	if( stGPSIndex.m_nWriteIndex >= MAX_GPS ){
		stGPSIndex.m_nWriteIndex = 0;
	}	
	WriteGPSIndex( &stGPSIndex );
	return 1;
}

//////////////////////////////////////////////////////////////////////////////
// 取出数据
// nAmount: 可以同时取多个数据
// 成功返回得到的数据长度
// 失败返回0
int	PopGPSData( unsigned char *pData, int	nAmount ){	
	
	int						nCnt;
//	int						nIndex;
	STGPSINDEX		stGPSIndex;
	
	
	ReadGPSIndex( &stGPSIndex );
	if( nAmount > stGPSIndex.m_nGPSLen ){
		nAmount = stGPSIndex.m_nGPSLen;
	}
	
	if( nAmount == 0 ){
		return 0;
	}	

	for( nCnt = 0; nCnt < nAmount; nCnt++ ){
		
		ReadDataFromFlash( (unsigned char*)(pData+nCnt*GPSDATASIZE), GetGPSDataAddr(stGPSIndex.m_nReadIndex), GPSDATASIZE );
		
		stGPSIndex.m_nGPSLen--;
		stGPSIndex.m_nFreeGPSLen++;
		
		stGPSIndex.m_nReadIndex++;
		if( stGPSIndex.m_nReadIndex >= MAX_GPS ){
			stGPSIndex.m_nReadIndex = 0;
		}			
	}
	WriteGPSIndex( &stGPSIndex );
	return nAmount*GPSDATASIZE;
}

#endif //EXTERNAL_FLASH
