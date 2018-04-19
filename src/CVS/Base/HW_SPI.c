//////////////////////////////////////////////////////////////////////////
// file name: HW_SPIFLASH.h
// Date: 2009-12-28
// Author: wyb 
// AT45DB321D оƬ
#include "..\hdr\main.h"
#include "..\hdr\config.h"
#include "..\hdr\HW_SPI.h"
#include "..\hdr\HW_GPIO.h"
#include "..\hdr\APP_GSM.h"

#ifdef		EXTERNAL_FLASH

#ifdef  ATMEL_FLASH //ATMEL����оƬ

#define     SPI_PORT_RESET		29
#define		SPI_PORT_WP			3

#define		SPI_PORT_CS			27

#define		SPI_SSEEL0			7


/*********************************************************************************************************
**�������ƣ�SPI0Init  
**�������ܣ���ʼ��SPI������  
**���ڲ�������  
**˵������SPIģʽ�£�Ĭ��CPHA=0��CPOL=1,����λ��Ĭ��Ϊ8λ  
**      ͨ������������Fpclk/(254*256)~Fpclk/2   
**      ���ͨ�Ÿ�ʽ����������Ĭ��ΪSPI��ʽ��  
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
		
	// ʱ�Ӽ����Ĵ���
	S0PCCR = 0x08;	
	S0PSR &= 0x00;
	// ���ƼĴ�����ʼ�� ��ģʽ
	S0PCR = SPCR_CPHA|SPCR_CPOL|SPCR_MSTR;
	
	OSTimeDly(20);
	SPI0_wait_busy();	
	SPI0_Read_Reg(); 
	
	return TRUE;
}
/*****************************************************************************
** Function name:		SSP0Send
** AT45DBXXϵ�д洢��SPI�ӿ�дһ���ֽں���
*****************************************************************************/
unsigned char SSP0_Write(unsigned char data)
{   
   S0PDR = data;
   while( 0==(S0PSR&0x80) );		// �ȴ�SPIF��λ�����ȴ����ݷ������   
   return  (S0PDR);
}
/*****************************************************************
*ͨ����ʱ����
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
//	���״̬�Ĵ������λ�Ƿ�Ϊæ�����ȴ�����
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
//  ����ָ�������洢��ҳ����ַ��Χ0-8092��
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
// ������������pSrc[]�е�һҳ����д��ڶ����������������洢��
//���Ȳ�����д��ģʽ��ҳ��ַ��Χ0-8092��
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
	SSP0_Write(B2_TO_MM_PAGE_PROG_WITH_ERASE);		//CMD//��BUFF2������д��FLASH
	SSP0_Write((unsigned char)(nPage>>6));
	SSP0_Write((unsigned char)(nPage<<2));
	SSP0_Write(0x00);		
	Chip_DeSelect();
}

//********************************************************************************************************
// ��ָ�����洢��ҳ������ת���һ�������������������pDest[]������
//��ҳ��ַ��Χ0-8092��
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
	// LPC2368��LPC2134 ��ȡ���ݲ�ͬ
	// LPC2368��ȡ������Ҫ����SSP0_Write(0x00) ���ӻ���һ���ֽ�
	// LPC2134��ȡ������Ҫ��ȥSSP0_Write(0x00) Ҫ���ӻ��ٶ�һ���ֽ�	
	//SSP0_Write(0x00);
	for ( nCnt = 0; nCnt< nLen; nCnt++){	
		pDest[nCnt] = SSP0_Write(0x00);
	}
	Chip_DeSelect();
}
//********************************************************************************************************
//��ȡ״̬�Ĵ���
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
//  ��ȡ�豸ID
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
//  ������д��FALSH
void WriteDataToFlash2( unsigned int	nDest, unsigned char	*pSrc, unsigned int nDataLen ){
			
	unsigned char	nCnt = 0;	
	unsigned short	nWPageSize = 0;			//д����ҳ
	unsigned short	nWPageSizePos = 0;		//���һҳд�����ֽ�
	unsigned int	nWBPage = 0;			//д�Ŀ�ʼҳ
	unsigned int    nWBPagePos = 0;			//д�Ŀ�ʼҳλ��
	unsigned char	*lpLargeBuf=NULL;
				
				
	lpLargeBuf = &lpLargeBuf4[500];
				
	nWBPage = nDest / PAGE_BLOCKSIZE;
	nWBPagePos = nDest % PAGE_BLOCKSIZE;
	
	// ���ڵ�ǰҳ,����д��һҳ
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
				
				// ��ȡ���ݸ�д
				SPI0_Read_Page(  nWBPage, 0, lpLargeBuf, PAGE_BLOCKSIZE );				
				memcpy( &lpLargeBuf[nWBPagePos], pSrc, PAGE_BLOCKSIZE-nWBPagePos );				
				SPI0_Write_Page( nWBPage, 0, lpLargeBuf, PAGE_BLOCKSIZE);	
			}	
			else{				
				SPI0_Write_Page( nWBPage, 0, &pSrc[nWBPagePos+((nCnt-1)*PAGE_BLOCKSIZE)], PAGE_BLOCKSIZE );	
			}
		}
		// д���һҳ����
		if( nWPageSizePos > 0 ){
		
			// ��ȡ���ݸ�д
			SPI0_Read_Page(  nWBPage, 0, lpLargeBuf, PAGE_BLOCKSIZE );							
			memcpy( &lpLargeBuf[0], &pSrc[nWBPagePos+((nCnt-1)*PAGE_BLOCKSIZE)], nWPageSizePos );
			
			SPI0_Write_Page( nWBPage, 0, lpLargeBuf, PAGE_BLOCKSIZE );	
		}
	}
	else{//ֻ��һҳ����
	
		SPI0_Read_Page(  nWBPage, 0, lpLargeBuf, PAGE_BLOCKSIZE );	
		memcpy( &lpLargeBuf[nWBPagePos], pSrc, nDataLen );		
		SPI0_Write_Page( nWBPage, 0, lpLargeBuf, PAGE_BLOCKSIZE);
	}	
}

//=================================================================================
//  �����ݶ�ȡFALSH����
void ReadDataFromFlash( unsigned char *pDest, unsigned int  nSrc, unsigned int nDataLen ){
	
	unsigned char	nCnt = 0;	
	unsigned short	nRPageSize = 0;			//д����ҳ
	unsigned short	nRPageSizePos = 0;		//���һҳд�����ֽ�
	unsigned int	nRBPage = 0;			//���Ŀ�ʼҳ
	unsigned int    nRBPagePos = 0;   		//���Ŀ�ʼҳλ��
	
		
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
		// �����һҳ����
		if( nRPageSizePos > 0 ){
			SPI0_Read_Page( nRBPage, 0, &pDest[nRBPagePos+((nCnt-1)*PAGE_BLOCKSIZE)], nRPageSizePos);
		}
	}
	else{	//��ȡ����
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
**�������ƣ�SPI0Init  
**�������ܣ���ʼ��SPI������  
**���ڲ�������  
**˵������SPIģʽ�£�Ĭ��CPHA=0��CPOL=1,����λ��Ĭ��Ϊ8λ  
**      ͨ������������Fpclk/(254*256)~Fpclk/2   
**      ���ͨ�Ÿ�ʽ����������Ĭ��ΪSPI��ʽ��  
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
		
	// ʱ�Ӽ����Ĵ���
	S0PCCR = 0x08;	
	S0PSR &= 0x00;
	// ���ƼĴ�����ʼ�� ��ģʽ
	S0PCR = SPCR_CPHA|SPCR_CPOL|SPCR_MSTR;
	
	OSTimeDly(20);
	SPI0_wait_busy();
	
	return TRUE;
}
/*****************************************************************************
** Function name:		SSP0Send
** AT45DBXXϵ�д洢��SPI�ӿ�дһ���ֽں���
*****************************************************************************/
unsigned char SSP0_Write(unsigned char data)
{   
   S0PDR = data;
   while( 0==(S0PSR&0x80) );		// �ȴ�SPIF��λ�����ȴ����ݷ������   
   return  (S0PDR);
}
/*****************************************************************
*ͨ����ʱ����
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
//  ��ȡ�豸ID
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
//  ��ȡ״̬�Ĵ���
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
//  ����д����
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
// ������������pSrc[]�е�һҳ����д���洢��
//����ַ��Χ0-4194304��
void SPI0_Write_Page( unsigned int nStartAddr, unsigned char	*pSrc, unsigned short nLen ){
	
	unsigned short 	nCnt = 0;
	
	SPI0_wait_busy();
	SPIO_EnableWrite();
	SPI0_wait_busy();		
	//д����
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
// ��ָ�����洢��ҳ�����ݶ�����pDest[]������
//����ַ��Χ0-4194304��
void SPI0_Read_Page(unsigned char	*pDest, unsigned int nStartAddr, unsigned int nLen ){

	unsigned short nCnt = 0;
	
	SPI0_wait_busy();
	Chip_Select();
	SSP0_Write( READ_SPI_DATA );
	SSP0_Write( (nStartAddr>>16)&0xFF );   //A23-A16
	SSP0_Write( (nStartAddr>>8)&0xFF );    //A15-A8
	SSP0_Write( nStartAddr&0xFF ); 		   //A7-A0
	
	// LPC2368��LPC2134 ��ȡ���ݲ�ͬ
	// LPC2368��ȡ������Ҫ����SSP0_Write(0xFF) ���ӻ���һ���ֽ�
	// LPC2134��ȡ������Ҫ��ȥSSP0_Write(0x00) Ҫ�ӻ��ٶ�һ���ֽ�		
	// SSP0_Write(0xFF);
	
	for( nCnt = 0; nCnt < nLen; nCnt++ ){
	
		pDest[nCnt] = SSP0_Write(0xFF);
	}
	Chip_DeSelect();	
}
//********************************************************************************************************
// ����ɾ����СΪ4K
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
// ����FLASH��� ��С4M
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
//  �����ݶ�ȡFALSH����
void ReadDataFromFlash( unsigned char *pDest, unsigned int  nSrc, unsigned int nDataLen ){

	SPI0_Read_Page( pDest, nSrc, nDataLen );
	return;
}
//////////////////////////////////////////////////////////////////////////////////////
// ע�⣺
// һ���������д4096���ֽ�
// �ҵ�ַ����ͬʱ��Խsector
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







