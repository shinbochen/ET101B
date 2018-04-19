//////////////////////////////////////////////////////////////////////////
// file name: HW_SPIFLASH.h
// Date: 2009-12-28
// Author: wyb 
// AT45DB321D 芯片
#include "..\hdr\main.h"
#include "..\hdr\config.h"
#include "..\hdr\HW_SPI.h"
#include "..\hdr\HW_GPIO.h"
#include "..\hdr\APP_GSM.h"

#ifdef		EXTERNAL_FLASH

#ifdef  ATMEL_FLASH //ATMEL厂家芯片

#define     SPI_PORT_RESET		29
#define		SPI_PORT_WP			3

#define		SPI_PORT_CS			27

#define		SPI_SSEEL0			7


/*********************************************************************************************************
**函数名称：SPI0Init  
**函数功能：初始化SPI控制器  
**出口参数：无  
**说明：在SPI模式下，默认CPHA=0，CPOL=1,数据位数默认为8位  
**      通信速率限制在Fpclk/(254*256)~Fpclk/2   
**      如果通信格式参数错误，则默认为SPI格式。  
********************************************************************************************************/ 
char  SPI0Init( void ){
		
	GPIO_Enable(SPI_PORT_RESET);
	GPIO_SetDirection(SPI_PORT_RESET, 1 ); 
		
	GPIO_Enable(SPI_PORT_WP);
	GPIO_SetDirection(SPI_PORT_WP, 1 ); 
	 
	GPIO_Enable(SPI_PORT_CS);
	GPIO_SetDirection(SPI_PORT_CS, 1 );
	
	GPIO_Enable(SPI_SSEEL0);
	GPIO_SetDirection(SPI_SSEEL0, 1 );
		
	//GPIO_Set(SPI_PORT_RESET, 0 );	 
	//OSTimeDly(10);	
	GPIO_Set(SPI_PORT_RESET, 1 );	
	GPIO_Set(SPI_PORT_WP, 1 ); 
		
	GPIO_Set(SPI_PORT_CS, 1 );
	
	GPIO_Set(SPI_SSEEL0, 1 );
	 
	PINSEL0 = (PINSEL0 &(~(0xFF<<8)))|(0x55<<8);	
		
	// 时钟计数寄存器
	S0PCCR = 0x08;	
	S0PSR &= 0x00;
	// 控制寄存器初始化 主模式
	S0PCR = SPCR_CPHA|SPCR_CPOL|SPCR_MSTR;
	
	OSTimeDly(20);
	SPI0_wait_busy();	
	SPI0_Read_Reg(); 
	
	return TRUE;
}
/*****************************************************************************
** Function name:		SSP0Send
** AT45DBXX系列存储的SPI接口写一个字节函数
*****************************************************************************/
unsigned char SSP0_Write(unsigned char data)
{   
   S0PDR = data;
   while( 0==(S0PSR&0x80) );		// 等待SPIF置位，即等待数据发送完毕   
   return  (S0PDR);
}
/*****************************************************************
*通用延时程序
******************************************************************/
void delay_250ns(){

	unsigned char  nCnt;
		
	for(nCnt = 0;nCnt <250;){
		nCnt++;
	}
}
//********************************************************************************************************
//
void  Chip_Select(){
	
	GPIO_Set(SPI_PORT_CS, 0);
	delay_250ns();
}
//********************************************************************************************************
//
void  Chip_DeSelect(){

	GPIO_Set(SPI_PORT_CS, 1);
	delay_250ns();
}
//********************************************************************************************************
//	检查状态寄存器最高位是否为忙，并等待空闲
void SPI0_wait_busy(void){
	
	Chip_Select();	
	SSP0_Write(READ_STATE_REGISTER);	
	SSP0_Write(0x00);	
	SSP0_Write(0x00);
	SSP0_Write(0x00); 
	
	while(1){
		if( SSP0_Write(0x00) & 0x80 ){
			break;
		}		
	}	
	Chip_DeSelect();
} 
//********************************************************************************************************
//  擦除指定的主存储器页（地址范围0-8092）
void SPI0_Page_Earse(unsigned short nPage)
{		
	SPI0_wait_busy();
	
	Chip_Select();
	SSP0_Write(PAGE_ERASE);
	SSP0_Write((unsigned char)(nPage >> 6));
	SSP0_Write((unsigned char)(nPage << 2));
	SSP0_Write(0x00);	
	Chip_DeSelect();
}
//********************************************************************************************************
// 将保存在数组pSrc[]中的一页数据写入第二缓冲区后送入主存储区
//（先擦除后写入模式，页地址范围0-8092）
void SPI0_Write_Page( unsigned short nPage, unsigned short  nPos ,unsigned char	*pSrc, unsigned short nLen )
{
	unsigned short nCnt;
	
	SPI0_wait_busy();	
	
	Chip_Select();
	SSP0_Write(BUFFER_2_WRITE);
	SSP0_Write(0x00);
	SSP0_Write((unsigned char)(nPos>>8));	
	SSP0_Write((unsigned char)(nPos));
	for (nCnt = 0; nCnt < nLen; nCnt++){
		SSP0_Write(pSrc[nCnt]);	
	}	
	Chip_DeSelect();
	
	SPI0_wait_busy();
	
	Chip_Select();
	SSP0_Write(B2_TO_MM_PAGE_PROG_WITH_ERASE);		//CMD//将BUFF2的数据写入FLASH
	SSP0_Write((unsigned char)(nPage>>6));
	SSP0_Write((unsigned char)(nPage<<2));
	SSP0_Write(0x00);		
	Chip_DeSelect();
}

//********************************************************************************************************
// 将指定主存储器页的数据转入第一缓冲区后读出，保存在pDest[]数组中
//（页地址范围0-8092）
void SPI0_Read_Page(unsigned short nPage, unsigned short  nPos,unsigned char	*pDest, unsigned short nLen )
{
	unsigned short nCnt = 0;
	
	SPI0_wait_busy();	
	
	Chip_Select();
	SSP0_Write(MM_PAGE_TO_B1_XFER);	
	SSP0_Write((unsigned char)(nPage >> 6));
    SSP0_Write((unsigned char)(nPage << 2));   
    SSP0_Write(0x00);
    Chip_DeSelect();
    
    SPI0_wait_busy();
    	
	Chip_Select();
	SSP0_Write(BUFFER_1_READ); 
	SSP0_Write(0x00);
	SSP0_Write((unsigned char)(nPos >> 8));
	SSP0_Write((unsigned char)(nPos));
	SSP0_Write(0x00);
	// LPC2368与LPC2134 读取数据不同
	// LPC2368读取数据是要加上SSP0_Write(0x00) 不加会多读一个字节
	// LPC2134读取数据是要减去SSP0_Write(0x00) 要不加会少读一个字节	
	//SSP0_Write(0x00);
	for ( nCnt = 0; nCnt< nLen; nCnt++){	
		pDest[nCnt] = SSP0_Write(0x00);
	}
	Chip_DeSelect();
}
//********************************************************************************************************
//读取状态寄存器
//	bit7		bit6	bit5	bit4	bit3	bit2	bit1		bit0
// RDY/BUSY		COMP	1		1		0		1		PROTECT		PAGE SIZE	
unsigned char SPI0_Read_Reg(void)
{
	unsigned char  nState = 0;

	SPI0_wait_busy();
	
	Chip_Select();
	SSP0_Write(READ_STATE_REGISTER);
	SSP0_Write(0x00);
	SSP0_Write(0x00);                        
	SSP0_Write(0x00);   
	nState= SSP0_Write(0x00);
	Chip_DeSelect();
	
	return nState;
}
//********************************************************************************************************
//  读取设备ID
void 	SPI0_ReadDeviceID( unsigned char  *pDest ){
	
	unsigned char 	nCnt = 0;

	SPI0_wait_busy();
	
	Chip_Select();
	SSP0_Write(READ_DEVICE_ID);	
	//SSP0_Write(0x00);
	for( nCnt = 0; nCnt < 6; nCnt++ ){	
		pDest[nCnt+1]= SSP0_Write(0x00);	
	}	
	Chip_DeSelect();
}
//=================================================================================
//  把数据写入FALSH
void WriteDataToFlash2( unsigned int	nDest, unsigned char	*pSrc, unsigned int nDataLen ){
			
	unsigned char	nCnt = 0;	
	unsigned short	nWPageSize = 0;			//写多少页
	unsigned short	nWPageSizePos = 0;		//最后一页写多少字节
	unsigned int	nWBPage = 0;			//写的开始页
	unsigned int    nWBPagePos = 0;			//写的开始页位置
	unsigned char	*lpLargeBuf=NULL;
				
				
	lpLargeBuf = &lpLargeBuf4[500];
				
	nWBPage = nDest / PAGE_BLOCKSIZE;
	nWBPagePos = nDest % PAGE_BLOCKSIZE;
	
	// 大于当前页,接着写下一页
	if( (nWBPagePos + nDataLen) > PAGE_BLOCKSIZE ){
				
	//	nWPageSize += (nDataLen-nWBPagePos) / PAGE_BLOCKSIZE;
	//	nWPageSizePos = (nDataLen-nWBPagePos) % PAGE_BLOCKSIZE;			
	//	if ( nWPageSizePos > 0 ){
	//		nWPageSize += 1;
	//	}		
		
		nWPageSize += (nDataLen+nWBPagePos) / PAGE_BLOCKSIZE;
		nWPageSizePos = (nDataLen+nWBPagePos) % PAGE_BLOCKSIZE;	
			
		for( nCnt = 0; nCnt < nWPageSize; nCnt++ , nWBPage++){
			if( nCnt == 0 ){
				
				// 读取数据改写
				SPI0_Read_Page(  nWBPage, 0, lpLargeBuf, PAGE_BLOCKSIZE );				
				memcpy( &lpLargeBuf[nWBPagePos], pSrc, PAGE_BLOCKSIZE-nWBPagePos );				
				SPI0_Write_Page( nWBPage, 0, lpLargeBuf, PAGE_BLOCKSIZE);	
			}	
			else{				
				SPI0_Write_Page( nWBPage, 0, &pSrc[nWBPagePos+((nCnt-1)*PAGE_BLOCKSIZE)], PAGE_BLOCKSIZE );	
			}
		}
		// 写最后一页数据
		if( nWPageSizePos > 0 ){
		
			// 读取数据改写
			SPI0_Read_Page(  nWBPage, 0, lpLargeBuf, PAGE_BLOCKSIZE );							
			memcpy( &lpLargeBuf[0], &pSrc[nWBPagePos+((nCnt-1)*PAGE_BLOCKSIZE)], nWPageSizePos );
			
			SPI0_Write_Page( nWBPage, 0, lpLargeBuf, PAGE_BLOCKSIZE );	
		}
	}
	else{//只有一页数据
	
		SPI0_Read_Page(  nWBPage, 0, lpLargeBuf, PAGE_BLOCKSIZE );	
		memcpy( &lpLargeBuf[nWBPagePos], pSrc, nDataLen );		
		SPI0_Write_Page( nWBPage, 0, lpLargeBuf, PAGE_BLOCKSIZE);
	}	
}

//=================================================================================
//  把数据读取FALSH数据
void ReadDataFromFlash( unsigned char *pDest, unsigned int  nSrc, unsigned int nDataLen ){
	
	unsigned char	nCnt = 0;	
	unsigned short	nRPageSize = 0;			//写多少页
	unsigned short	nRPageSizePos = 0;		//最后一页写多少字节
	unsigned int	nRBPage = 0;			//读的开始页
	unsigned int    nRBPagePos = 0;   		//读的开始页位置
	
		
	nRBPage = nSrc / PAGE_BLOCKSIZE;
	nRBPagePos = nSrc % PAGE_BLOCKSIZE;
	
	if( (nRBPagePos + nDataLen) > PAGE_BLOCKSIZE ){
		
	//	nRPageSize += (nDataLen-nRBPagePos) / PAGE_BLOCKSIZE;		
	//	nRPageSizePos = (nDataLen-nRBPagePos) % PAGE_BLOCKSIZE;
	//	if( nRPageSizePos > 0 ){
	//		nRPageSize += 1;		
	//	}
	
		nRPageSize += (nDataLen+nRBPagePos) / PAGE_BLOCKSIZE;		
		nRPageSizePos = (nDataLen+nRBPagePos) % PAGE_BLOCKSIZE;
		
		for( nCnt = 0; nCnt < nRPageSize; nCnt++, nRBPage++ ){			
			if( nCnt == 0 ){
				SPI0_Read_Page( nRBPage, nRBPagePos, &pDest[0], PAGE_BLOCKSIZE-nRBPagePos );
			}	
			else{
				SPI0_Read_Page( nRBPage, 0, &pDest[nRBPagePos+((nCnt-1)*PAGE_BLOCKSIZE)], PAGE_BLOCKSIZE );
			}
		}
		// 读最后一页数据
		if( nRPageSizePos > 0 ){
			SPI0_Read_Page( nRBPage, 0, &pDest[nRBPagePos+((nCnt-1)*PAGE_BLOCKSIZE)], nRPageSizePos);
		}
	}
	else{	//读取数据
		SPI0_Read_Page( nRBPage, nRBPagePos, pDest, nDataLen);
	}
}
#endif //ATMEL_FLASH

//********************************************************************************************************
//
//
//
#ifdef  WINBAND_FLASH

#define     SPI_PORT_RESET		29
#define		SPI_PORT_WP			3

#define		SPI_PORT_CS			27

#define		SPI_SSEEL0			7


/*********************************************************************************************************
**函数名称：SPI0Init  
**函数功能：初始化SPI控制器  
**出口参数：无  
**说明：在SPI模式下，默认CPHA=0，CPOL=1,数据位数默认为8位  
**      通信速率限制在Fpclk/(254*256)~Fpclk/2   
**      如果通信格式参数错误，则默认为SPI格式。  
********************************************************************************************************/ 
char  SPI0Init( void ){
		
	GPIO_Enable(SPI_PORT_RESET);
	GPIO_SetDirection(SPI_PORT_RESET, 1 ); 
		
	GPIO_Enable(SPI_PORT_WP);
	GPIO_SetDirection(SPI_PORT_WP, 1 ); 
	 
	GPIO_Enable(SPI_PORT_CS);
	GPIO_SetDirection(SPI_PORT_CS, 1 );
	
	GPIO_Enable(SPI_SSEEL0);
	GPIO_SetDirection(SPI_SSEEL0, 1 );
		
	//GPIO_Set(SPI_PORT_RESET, 0 );	 
	//OSTimeDly(10);	
	GPIO_Set(SPI_PORT_RESET, 1 );	
	GPIO_Set(SPI_PORT_WP, 1 ); 
		
	GPIO_Set(SPI_PORT_CS, 1 );
	
	GPIO_Set(SPI_SSEEL0, 1 );
	 
	PINSEL0 = (PINSEL0 &(~(0xFF<<8)))|(0x55<<8);	
		
	// 时钟计数寄存器
	S0PCCR = 0x08;	
	S0PSR &= 0x00;
	// 控制寄存器初始化 主模式
	S0PCR = SPCR_CPHA|SPCR_CPOL|SPCR_MSTR;
	
	OSTimeDly(20);
	SPI0_wait_busy();
	
	return TRUE;
}
/*****************************************************************************
** Function name:		SSP0Send
** AT45DBXX系列存储的SPI接口写一个字节函数
*****************************************************************************/
unsigned char SSP0_Write(unsigned char data)
{   
   S0PDR = data;
   while( 0==(S0PSR&0x80) );		// 等待SPIF置位，即等待数据发送完毕   
   return  (S0PDR);
}
/*****************************************************************
*通用延时程序
******************************************************************/
void delay_250ns(){

	unsigned char  nCnt;
		
	for(nCnt = 0;nCnt <250;){
		nCnt++;
	}
}
//********************************************************************************************************
//
void  Chip_Select(){
	
	GPIO_Set(SPI_PORT_CS, 0);
	delay_250ns();
}
//********************************************************************************************************
//
void  Chip_DeSelect(){

	GPIO_Set(SPI_PORT_CS, 1);
	delay_250ns();
}
//********************************************************************************************************
//  读取设备ID
void    SPIO_ReadDeviceID( unsigned char  *pDest ){

	unsigned char 	nCnt = 0;

	Chip_Select();
	SSP0_Write(READ_DEVICE_ID);	
	SSP0_Write(0x00);
	SSP0_Write(0x00);
	SSP0_Write(0x00);
	SSP0_Write(0xFF);
	for( nCnt = 0; nCnt < 2; nCnt++ ){	
		pDest[nCnt]= SSP0_Write(0xFF);	
	}	
	Chip_DeSelect();
}
//********************************************************************************************************
//  读取状态寄存器
void    SPI0_wait_busy( void ){

	unsigned char  nData = 0;

	Chip_Select();
	
	SSP0_Write(READ_STATUS_REGISTER_1);	
	SSP0_Write(0xFF);
	while( 1 ){
		nData = SSP0_Write(0xFF);
		if( (nData & 0x01) == 0x00 ){
			break;
		}		
	}
	Chip_DeSelect();
}
//********************************************************************************************************
//  开启写功能
void   SPIO_EnableWrite( void ){

	unsigned char  nData = 0;
		
	Chip_Select();	
	SSP0_Write(ENABLE_WRITE_FUNCTION);
	Chip_DeSelect();

	Chip_Select();	
	SSP0_Write(READ_STATUS_REGISTER_1);	
	SSP0_Write(0xFF);
	while( 1 ){
		nData = SSP0_Write(0xFF);
		if( (nData & 0x02) == 0x02 ){
			break;
		}
	}
	Chip_DeSelect();
	
}
//********************************************************************************************************
// 将保存在数组pSrc[]中的一页数据写主存储区
//（地址范围0-4194304）
void SPI0_Write_Page( unsigned int nStartAddr, unsigned char	*pSrc, unsigned short nLen ){
	
	unsigned short 	nCnt = 0;
	
	SPI0_wait_busy();
	SPIO_EnableWrite();
	SPI0_wait_busy();		
	//写数据
	Chip_Select();
	SSP0_Write(WRITE_PAGE_DATA);
	SSP0_Write( (nStartAddr>>16)&0xFF );   //A23-A16
	SSP0_Write( (nStartAddr>>8)&0xFF );    //A15-A8
	SSP0_Write( nStartAddr&0xFF ); 		   //A7-A0	
	//SSP0_Write(0xCC);
	for( nCnt = 0; nCnt < nLen; nCnt++ ){
	
		SSP0_Write(pSrc[nCnt]);
	}		
	Chip_DeSelect();
	
	SPI0_wait_busy();
}
//********************************************************************************************************
// 将指定主存储器页的数据读出在pDest[]数组中
//（地址范围0-4194304）
void SPI0_Read_Page(unsigned char	*pDest, unsigned int nStartAddr, unsigned int nLen ){

	unsigned short nCnt = 0;
	
	SPI0_wait_busy();
	Chip_Select();
	SSP0_Write( READ_SPI_DATA );
	SSP0_Write( (nStartAddr>>16)&0xFF );   //A23-A16
	SSP0_Write( (nStartAddr>>8)&0xFF );    //A15-A8
	SSP0_Write( nStartAddr&0xFF ); 		   //A7-A0
	
	// LPC2368与LPC2134 读取数据不同
	// LPC2368读取数据是要加上SSP0_Write(0xFF) 不加会多读一个字节
	// LPC2134读取数据是要减去SSP0_Write(0x00) 要加会少读一个字节		
	// SSP0_Write(0xFF);
	
	for( nCnt = 0; nCnt < nLen; nCnt++ ){
	
		pDest[nCnt] = SSP0_Write(0xFF);
	}
	Chip_DeSelect();	
}
//********************************************************************************************************
// 扇区删除大小为4K
void  SPI0_Sector_Earse( unsigned int nStartAddr ){
	
	SPI0_wait_busy();
	SPIO_EnableWrite();
	SPI0_wait_busy();
	Chip_Select();
	SSP0_Write( SECTOR_ERASE_DATA );
	SSP0_Write( (nStartAddr>>16)&0xFF );   //A23-A16
	SSP0_Write( (nStartAddr>>8)&0xFF );    //A15-A8
	SSP0_Write( nStartAddr&0xFF ); 		   //A7-A0
	Chip_DeSelect();
	SPI0_wait_busy();
}
//********************************************************************************************************
// 整个FLASH清除 大小4M
void    SPI0_Chip_Earse( void ){
	
	SPI0_wait_busy();
	SPIO_EnableWrite();
	SPI0_wait_busy();
	Chip_Select();
	SSP0_Write( CHIP_ERASE_DATA );
	Chip_DeSelect();
	SPI0_wait_busy();
}
//=================================================================================
//  把数据读取FALSH数据
void ReadDataFromFlash( unsigned char *pDest, unsigned int  nSrc, unsigned int nDataLen ){

	SPI0_Read_Page( pDest, nSrc, nDataLen );
	return;
}
//////////////////////////////////////////////////////////////////////////////////////
// 注意：
// 一次最多允许写4096个字节
// 且地址不能同时跨越sector
void WriteDataToFlash2( unsigned int nDest, unsigned char *pSrc, unsigned int nDataLen ){

	unsigned int   nStartAddr = 0;
	unsigned int   nOffset = 0;
	unsigned int   nCnt = 0;
	unsigned char  *lpLargeBuf=NULL;


	nStartAddr = nDest/4096;
	nStartAddr *= 4096;
	nOffset = nDest%4096;


	lpLargeBuf = lpLargeBuf1;

	OS_ENTER_CRITICAL();
	SPI0_Read_Page( lpLargeBuf, nStartAddr, 4096 );

	SPI0_Sector_Earse( nStartAddr );
	for( nCnt = 0; nCnt < nDataLen; nCnt++ ){
	
		*(lpLargeBuf+nOffset+nCnt) = *(pSrc+nCnt); 
	}

	for( nCnt = 0; nCnt < 16; nCnt++ ){
	
		SPI0_Write_Page( nStartAddr+nCnt*256, lpLargeBuf+nCnt*256, 256 ); 
	}
	OS_EXIT_CRITICAL();
	return;
}

#endif //WINBAND_FLASH


#endif //EXTERNAL_FLASH







