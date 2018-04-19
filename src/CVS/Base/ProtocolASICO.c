///////////////////////////////////////////////////////////////////////////
//   file name: ProtocolData.h
// description: GSM/GPRS协议头文件定义
//      author:
//        date: 2009-06-01
//     reamark: 

#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>
#include  "..\hdr\includes.h"
#include  "..\hdr\ProtocolA.h"
#include  "..\hdr\ProtocolB.h"
#include  "..\hdr\ProtocolASICO.h"
#include  "..\hdr\APP_LoopDetect.h" 
#include  "..\hdr\APP_Terminal.h"
#include  "..\hdr\HW_GPIO.h"
#include  "..\hdr\APP_GSM.h"
#include  "..\hdr\APP_GTM900.h"
#include  "..\hdr\APP_BQM26.h"
#include  "..\hdr\APP_EM200.h"
#include  "..\hdr\APP_SIMCOM.h"
#include  "..\hdr\APP_GPS.h"
#include  "..\hdr\SMSProtocolA.h"
#include  "..\hdr\SMSProtocolB.h"
#include  "..\hdr\SMSProtocolC.h"


#ifdef	SW_GPRSPROTOCOL_ASICO

/////////////////////////////////////////////////////////////////////////////
// 组织DEUID数据 13480924411
void	PL_ComposeDEUID( unsigned char	*pDest ){
	
	unsigned char	nResult = 0;
	unsigned char	nCnt = 0;
	unsigned char	nTmp;
	unsigned char	nLen = 0;
	
	memset( pDest, 0xFF,  ASICO_DEUID_LEN);
	nLen = strlen((char*)g_stGSMSetup.m_strUID);
	if( nLen > 14){
		nLen = 14;
	}
	for( nCnt = 0; nCnt < nLen; nCnt++ ){

		nTmp = g_stGSMSetup.m_strUID[nCnt];
		if( nTmp >= '0' && nTmp <='9' ){
			nTmp -= '0';

			if( (nCnt%2) == 0 ){
				*(pDest+nResult) = nTmp<<4;	
				*(pDest+nResult) |= 0x0F;									
			}
			else{
				*(pDest+nResult) &= 0xF0;		
				*(pDest+nResult) |= nTmp;
				nResult++;
			}
		}
	}
}
/////////////////////////////////////////////////////////////////////////////
//  组织 表头(2B)|长度(2B)|DEUID(7B)|命令码(2B) 
void	PL_ComposeHeadData( unsigned char	*pDest, 
							unsigned char	nDataLen,
							unsigned short	nCmdCode ){
		
	pDest[WHERE_MESSAGE_HEAD1] = SYN_TEHEADERCODE1;
	pDest[WHERE_MESSAGE_HEAD2] = SYN_TEHEADERCODE2;
	pDest[WHERE_DATALEN_HIGHT] = 0x00;
	pDest[WHERE_DATALEN_LOW] = nDataLen;
	PL_ComposeDEUID(pDest+WHERE_MESSAGE_DEUID);	
	pDest[WHERE_CMD_HIGHT] = (nCmdCode >> 8) & 0xff;
	pDest[WHERE_CMD_LOW] = nCmdCode & 0xff;											
}
/////////////////////////////////////////////////////////////////////////////
//   更新状态信息
void			PL_UPDataStatus( void ){
	
	unsigned int  			nTmp = 0;
	
	// GPS信号
	if( (g_stGPSInfo.m_nGPSSignal >= 1) &&
	    (g_stGPSInfo.m_nGPSSignal <= 3) ){		//信号差
	    g_stDeviceState.m_nHWState |= HW_GPS_SIGNAL1;
	    g_stDeviceState.m_nHWState &= ~HW_GPS_SIGNAL2;
	}
	else if( (g_stGPSInfo.m_nGPSSignal >= 4) &&	    
	    	 (g_stGPSInfo.m_nGPSSignal <= 5) ){ //信号一般
	    g_stDeviceState.m_nHWState &= ~HW_GPS_SIGNAL1;	 
	    g_stDeviceState.m_nHWState |= HW_GPS_SIGNAL2;
	}
	else if ( g_stGPSInfo.m_nGPSSignal > 5 ){   //信号强
		g_stDeviceState.m_nHWState |= HW_GPS_SIGNAL1;
		g_stDeviceState.m_nHWState |= HW_GPS_SIGNAL2;
	}
	else{										//没有信号	
		g_stDeviceState.m_nHWState &= ~HW_GPS_SIGNAL1;
		g_stDeviceState.m_nHWState &= ~HW_GPS_SIGNAL2;
	}
	// GSM信号	
	if( (g_stGSMState.m_nGSMSignal >= 8) &&  
	    (g_stGSMState.m_nGSMSignal <= 15) ){	 //信号差
	    g_stDeviceState.m_nHWState |= HW_GSM_SIGNAL1;
	    g_stDeviceState.m_nHWState &= ~HW_GSM_SIGNAL2;
	}
	else if( (g_stGSMState.m_nGSMSignal >= 16) && 
	    (g_stGSMState.m_nGSMSignal <= 23) ){ 	//信号一般
	    g_stDeviceState.m_nHWState &= ~HW_GSM_SIGNAL1;
	    g_stDeviceState.m_nHWState |= HW_GSM_SIGNAL2; 	    
	}
	else if( (g_stGSMState.m_nGSMSignal >= 24) &&
	    	 (g_stGSMState.m_nGSMSignal <= 31) ){ //信号强
	    g_stDeviceState.m_nHWState |= HW_GSM_SIGNAL1;
	    g_stDeviceState.m_nHWState |= HW_GSM_SIGNAL2; 
	}
	else{										  //没有信号
		g_stDeviceState.m_nHWState &= ~HW_GSM_SIGNAL1;
	    g_stDeviceState.m_nHWState &= ~HW_GSM_SIGNAL2;
	}
	// 电源电压
	nTmp = g_stDeviceState.m_nBattery*170/20; 
	nTmp /= 1000; 
	nTmp &= 0x1F;
	nTmp = nTmp<<24;
	g_stDeviceState.m_nHWState &= ~0x1F000000;
	g_stDeviceState.m_nHWState |= nTmp;	
}
/////////////////////////////////////////////////////////////////////////////
//  组织GPS数据 表头(2B)|长度(2B)|DEUID(7B)|命令码(2B)|数据内容(XB)|校验码(2B)|结束码(2B)
//  数据内容: GPRMC|水平数=1(1B)|高度(0米)(1B)|状态(0)(1B)|0,0 (3B)
unsigned char 	PL_ComposeGPSData( unsigned char  *pDest ){

	unsigned char			nDataLen = 0;
	unsigned char			nGpsLen = 0;
		
		
	memset( pDest, 0x00, 128);	
	//  更新状态信息	
	PL_UPDataStatus();	
	// GPRS模式报警
	if( TE_HaveAlarm() ){ 		
		// 14 = 表头(2B)|长度(2B)|DEUID(7B)|命令码(2B)|报警状态(4)
		nDataLen = 13+4;		
		// 10 = |水平数=1(1B)|高度(0米)(1B)|状态(0xFFFF FFFF)(8)|0,0 (3B)
		nDataLen += 17;	
		// 校验码(2B)|结束码(2B)
		nDataLen += 4;				
		memcpy( &pDest[WHERE_MESSAGE_DATA], &g_stDeviceState.m_nALState, 4);
		// 组织GPS数据
		nGpsLen = GPS_GetGPRMCData( &pDest[WHERE_MESSAGE_DATA+4] );	
		nDataLen += nGpsLen;	
		// 组织表头	
		PL_ComposeHeadData(pDest, nDataLen, CC_TE_ALARM_CMD );			
		
		// 组织其它数据
		sprintf( (char*)pDest+17+nGpsLen, "|1|%d|%02X%02X%02X%02X|0,0", 0, 
																		g_stDeviceState.m_nHWState&0xFF,
																		(g_stDeviceState.m_nHWState>>8)&0xFF,
																		(g_stDeviceState.m_nHWState>>16)&0xFF,
																		(g_stDeviceState.m_nHWState>>24)&0xFF );	
																		
		
	}
	else{
		// 13 = 表头(2B)|长度(2B)|DEUID(7B)|命令码(2B)|
		nDataLen = 13;			
		// 10 = |水平数=1(1B)|高度(0米)(1B)|状态(0xFFFF FFFF)(8B)|0,0 (3B)
		nDataLen += 17;	
		// 校验码(2B)|结束码(2B)
		nDataLen += 4;		
		// 组织GPS数据
		nGpsLen = GPS_GetGPRMCData( &pDest[WHERE_MESSAGE_DATA] );	
		nDataLen += nGpsLen;		
		// 组织表头
		PL_ComposeHeadData(pDest, nDataLen, CC_TE_SINGLE_LOCATION_REPORT );			
		// 组织其它数据
		sprintf( (char*)pDest+13+nGpsLen, "|1|%d|%02X%02X%02X%02X|0,0", 0,
																		g_stDeviceState.m_nHWState&0xFF,
																		(g_stDeviceState.m_nHWState>>8)&0xFF,
																		(g_stDeviceState.m_nHWState>>16)&0xFF,
																		(g_stDeviceState.m_nHWState>>24)&0xFF );
	}	
	// 计算校验和 
	PL_CountVerifySum(pDest, nDataLen-4);
	// 结速码
	pDest[nDataLen-2] = SYN_R_ENDCODE;  
	pDest[nDataLen-1] = SYN_N_ENDCODE;
	
	return nDataLen;
}
/////////////////////////////////////////////////////////////////////////////
//	没有定位的时候回复
unsigned char	PL_ComposeNoValidData( unsigned char	*pDest ){

	unsigned char			nDataLen = 0;
	unsigned char			nGpsLen = 0;
	
	
	memset( pDest, 0x00, 128 );
	// 更新状态信息
	PL_UPDataStatus();
	// 13 = 表头(2B)|长度(2B)|DEUID(7B)|命令码(2B)|标识符(1Byte) 
	nDataLen = 14;	
	// 17 = |水平数=1(1B)|高度(0米)(1B)|状态(0xFFFF FFFF)(8B)|0,0 (3B)
	nDataLen += 17;	
	// 校验码(2B)|结束码(2B)
	nDataLen += 4;	
	// 组织GPS数据
	pDest[WHERE_MESSAGE_DATA] = 0x66;
	nGpsLen = GPS_GetGPRMCData( &pDest[WHERE_MESSAGE_DATA+1] );	
	nDataLen += nGpsLen;	
	// 组织表头
	PL_ComposeHeadData( pDest, nDataLen, CC_TE_SINGLE_LOCATION_REPORT );	
	
	// 计算校验和 
	PL_CountVerifySum(pDest, nDataLen-4);
	// 组织其它数据
	sprintf( (char*)pDest+14+nGpsLen, "|1|%d|%02X%02X%02X%02X|0,0", 0, 
																	g_stDeviceState.m_nHWState&0xFF,
																	(g_stDeviceState.m_nHWState>>8)&0xFF,
																	(g_stDeviceState.m_nHWState>>16)&0xFF,
																	(g_stDeviceState.m_nHWState>>24)&0xFF );
	// 结速码
	pDest[nDataLen-2] = SYN_R_ENDCODE;  
	pDest[nDataLen-1] = SYN_N_ENDCODE;	
	return nDataLen;
}
/////////////////////////////////////////////////////////////////////////////
// 织织握手信息	
unsigned char 	PL_ComposeHSData( unsigned char	*pDest ){

	unsigned char			nLen  = 17;
		
	PL_ComposeHeadData(pDest, nLen, CC_TE_HS_DATA);
	PL_CountVerifySum(pDest, nLen-4);
	//结速码
	pDest[nLen-2] = SYN_R_ENDCODE;  
	pDest[nLen-1] = SYN_N_ENDCODE;
	return nLen;
}
/////////////////////////////////////////////////////////////////////////////
//
unsigned char	PL_ComposeLogin( unsigned char  *pDest ){

	unsigned char			nLen  = 17;
	
	PL_ComposeHeadData(pDest, nLen, CC_TE_LOGIN);
	PL_CountVerifySum(pDest, nLen-4);	
	//结速码
	pDest[nLen-2] = SYN_R_ENDCODE;  
	pDest[nLen-1] = SYN_N_ENDCODE;
	return nLen;
}
/////////////////////////////////////////////////////////////////////////////
//	设置跟踪时间 
unsigned char  PL_ComposeTrackTime( unsigned char	*pDest,
								 	unsigned short	nCmdCode,
								 	unsigned char	nStuts ){
									
	unsigned char		  nLen;	
	if( nStuts == 0 ){
		nLen = 21;
		PL_ComposeHeadData(pDest, nLen, nCmdCode);
		pDest[WHERE_MESSAGE_DATA]   = 0x01;
		pDest[WHERE_MESSAGE_DATA+1]   = g_stGSMSetup.m_nCBMode;
		pDest[WHERE_MESSAGE_DATA+2] = (g_stGSMSetup.m_nCBSpace>>8)&0xFF;
		pDest[WHERE_MESSAGE_DATA+3] = g_stGSMSetup.m_nCBSpace&0xFF;
		PL_CountVerifySum(pDest, nLen-4);
	}
	else{
		nLen = 20;
		PL_ComposeHeadData(pDest, nLen, nCmdCode);
		pDest[WHERE_MESSAGE_DATA]   = g_stGSMSetup.m_nCBMode;
		pDest[WHERE_MESSAGE_DATA+1] = (g_stGSMSetup.m_nCBSpace>>8)&0xFF;
		pDest[WHERE_MESSAGE_DATA+2] = g_stGSMSetup.m_nCBSpace&0xFF;
		PL_CountVerifySum(pDest, nLen-4);
	}
	//结速码
	pDest[nLen-2] = SYN_R_ENDCODE;  
	pDest[nLen-1] = SYN_N_ENDCODE;
	
	return  nLen;									
}
//////////////////////////////////////////////////////////////////////////
//	设置命令应答数据
unsigned char	PL_ComposeACKData( unsigned char	*pDest,
								   unsigned char	nFlag,	
								   unsigned short	nCmdCode ){
	unsigned char		nLen = 18;
	
	PL_ComposeHeadData(pDest, nLen, nCmdCode);
	pDest[WHERE_MESSAGE_DATA]   = nFlag;
	PL_CountVerifySum(pDest, nLen-4);	
	//结速码
	pDest[nLen-2] = SYN_R_ENDCODE;  
	pDest[nLen-1] = SYN_N_ENDCODE;
	
	return nLen;								   
}
//////////////////////////////////////////////////////////////////////////
//  读报警功能状态
unsigned char	PL_ComposeReadAlarm( unsigned char	*pDest, 
									 unsigned short	nCmdCode ){
									 
	unsigned char		nLen = 21;
	
	PL_ComposeHeadData(pDest, nLen, nCmdCode);
	memcpy( &pDest[WHERE_MESSAGE_DATA], &g_stAlarmSetup.m_nALValid, 4);
	PL_CountVerifySum(pDest, nLen-4);	
	//结速码
	pDest[nLen-2] = SYN_R_ENDCODE;  
	pDest[nLen-1] = SYN_N_ENDCODE;
	
	return nLen;								 
}
//////////////////////////////////////////////////////////////////////////
//  读取授权号码  17+16+16;
unsigned char PL_ComposeReadNumber( unsigned char	*pDest, 
									unsigned char	nPhoneType,
									unsigned char	*pPhone ){
	unsigned char		nLen= 17+1+16; 
	
	PL_ComposeHeadData(pDest, nLen, CC_TE_READ_NUMBER);
	memset( &pDest[WHERE_MESSAGE_DATA], 0x00, 17);
	pDest[WHERE_MESSAGE_DATA] = nPhoneType;	
	memcpy( &pDest[WHERE_MESSAGE_DATA+1], pPhone, 16);
	PL_CountVerifySum(pDest, nLen-4);	
	//结速码
	pDest[nLen-2] = SYN_R_ENDCODE;  
	pDest[nLen-1] = SYN_N_ENDCODE;
	return nLen;								
}
//////////////////////////////////////////////////////////////////////////
//	BCD转换为十六进制
unsigned char BCDToHex( unsigned char	nData){

	return (((nData >> 4 ) * 16) + (nData & 0x0f));
}
//////////////////////////////////////////////////////////////////////////
//	字节型到十六进制
unsigned short ByteToInt(unsigned char* pSrc){  //16bit

	  return ((pSrc[WHERE_CMD_HIGHT] << 8) | pSrc[WHERE_CMD_LOW]);
}
//////////////////////////////////////////////////////////////////////////
//得出DEUID
void   PL_ParseDEUID( unsigned char	*pDest, unsigned char *pSrc ){
	
	unsigned char	nResult = 0;			
	unsigned char	nHexData = 0;	
	unsigned char	nCnt = 0;	
	
	for( nCnt = 0; nCnt < ASICO_DEUID_LEN; nCnt++){	
		
		nHexData = BCDToHex( pSrc[nCnt] ); 
		if( (nHexData&0xF0) == 0xF0){ //高字节
			break;
		}
		else{ 				
			pDest[nResult++] = (nHexData/16) + 0x30;
		}
		if( (nHexData&0x0f) == 0x0F){ //低字节
			break;
		}
		else{			
			pDest[nResult++] = (nHexData%16) + 0x30;
		}
	}
	pDest[nResult]= 0x00;
	return;
}
///////////////////////////////////////////////////////////////
// 	解释协议数据
unsigned char	PL_ParseData(unsigned char *pDest, unsigned char *pSrc, unsigned char nLen ){

	unsigned char	 	nResultLen = 0;
	unsigned char		nDataLen = 0;	
	unsigned int		nCmdCode = 0;
	unsigned char		nDEUID[DEUID_LENGTH+1];
			
	// 同步码		
	if( pSrc[WHERE_MESSAGE_HEAD1] != SYN_SVEREHEADERCODE1 ||
		pSrc[WHERE_MESSAGE_HEAD2] != SYN_SVEREHEADERCODE2 ){
		return 0;
	}
	// 数据长度	
	nDataLen  = ByteToInt( pSrc );
	// 收到DEUID数据
	PL_ParseDEUID(nDEUID, pSrc+WHERE_MESSAGE_DEUID );
	if( memcmp( g_stGSMSetup.m_strUID, nDEUID, strlen((char*)g_stGSMSetup.m_strUID)) != 0){
		return 0;
	}
	// 命令码
	nCmdCode = ByteToInt( pSrc );	
	if( nCmdCode == CC_TE_LOGIN_ACK ){
		if(pSrc[WHERE_MESSAGE_DATA] == CMD_SUCCEED ){
			TE_SetLoginSuccess( );	
			UDP_ClrHandShakeCnt( );
			TCP_ClrRCntionCnt();
			IP_ClrConnectFailedCnt( );
		}		
	}
	else if( nCmdCode == CC_TE_HS_DATA ){
		UDP_ClrHandShakeCnt( );
		TCP_ClrRCntionCnt();
		IP_ClrConnectFailedCnt( );
	}
	else{
		//nDataLen: 表头(2Byte)|长度(2Byte)|DEUID(7Byte)|命令码(2Byte)|数据内容(X Byte)|校检码(2Byte)|结束符(2Byte)|
		nResultLen = PL_ParseDelivery( pDest, pSrc, nDataLen-ASICO_DEUID_LEN-2-2-2-4, nCmdCode );
	}	
	return nResultLen;
}

////////////////////////////////////////////////////////////////////////////
//   发送终端数据信息  MT_DELIVERY
unsigned char	PL_ParseDelivery( unsigned char  *pDest, 
								  unsigned char  *pSrc,
								  unsigned char	 nDataLen,
								  unsigned int	 nCmdCode ){
		
	unsigned char			nLen  = 0;
	unsigned char			nFlag = 0;
	unsigned char			*pTmp = NULL;
	unsigned char 			nCaller[TELNUMBER_LENGTH+1];
	unsigned int			nTmp = 0;
	
	switch(nCmdCode){
	case	CC_TE_CALLONCE:		 			 //点名跟踪
	case    CC_TE_EXPAND_CALLONCE:			 //点名跟踪
		if( GPS_IsPosValid() ){
			nLen = PL_ComposeGPSData( pDest );
		}
		else{
			nLen = PL_ComposeNoValidData( pDest );
		}
		break;
	case	CC_TE_SETUP_TRACKTIME:			 //设置定位时间			
		nFlag = pSrc[WHERE_MESSAGE_DATA];		
		nTmp = (pSrc[WHERE_MESSAGE_DATA+1]<<8) & 0xFF00;
		nTmp += pSrc[WHERE_MESSAGE_DATA+2];
		if( nFlag == 0x00){
			g_stGSMSetup.m_nCBMode = CB_STOP;
			g_stGSMSetup.m_nCBSpace = nTmp;
		}
		else if(nFlag == 0x01){
			if( nTmp < 10){
				nTmp = 10;
			}
			g_stGSMSetup.m_nCBMode = CB_BYTIME;
			g_stGSMSetup.m_nCBSpace = nTmp;
		}
		else if( nFlag == 0x02 ){
			if( nTmp < 300){
				nTmp = 300;
			}
			g_stGSMSetup.m_nCBMode = CB_BYDISTANCE;
			g_stGSMSetup.m_nCBSpace = nTmp;
		}		
		nLen = PL_ComposeTrackTime( pDest, CC_TE_ACK_TRACKTIME, 0  );		
		TE_SetNeedWriteFlash( ); 
		
		break;
	case	CC_TE_SETUP_NUMBER:				 //设置授权号码
		nFlag = PLS_SetupGpsMasterNo( pSrc );	
		nLen = PL_ComposeACKData( pDest, nFlag, nCmdCode);			
		TE_SetNeedWriteFlash( );	
		break;
	
	case	CC_TE_SETUP_OVERSPEED:			 //设置超速报警
		nFlag = pSrc[WHERE_MESSAGE_DATA];
		if( nFlag == 0 ){
			AL_DisableOverSpeed( );	
		}
		else{
			AL_EnableOverSpeed( );
			g_stAlarmSetup.m_nOverspeed = nFlag*10;
		}
		nLen = PL_ComposeACKData( pDest, 1, nCmdCode);	
		TE_SetNeedWriteFlash( );
		break;
	case	CC_TE_SETUP_OUTGEO:				 //设置移位报警
		nFlag = PLS_SetupGpsOutGeo(pSrc);
		nLen = PL_ComposeACKData( pDest, nFlag, nCmdCode );
		TE_SetNeedWriteFlash( );
		break;
	case	CC_TE_SETUP_FENCE:				 //设置电子栅栏
	case    CC_TE_SETUP_FENCE2:
	/*	nFlag = PLS_SetupFence( pSrc, nCmdCode );
		if( nFlag ){
			if( nCmdCode == CC_TE_SETUP_FENCE ){
				
				g_stAlarmSetup.m_oStGFence[0].m_nFlag &= ~0x80;
				//AL_DisableLeaveGEOFence();				
			}
			else if( nCmdCode == CC_TE_SETUP_FENCE2 ){
			
				g_stAlarmSetup.m_oStGFence[0].m_nFlag |= 0x80;
				//AL_DisableEnterGEOFence();				
			}
		}
		else{
			//AL_DisableLeaveGEOFence();
			//AL_DisableEnterGEOFence();
			g_stAlarmSetup.m_oStGFence[0].m_nFlag = 0;
		}		
		nLen = PL_ComposeACKData( pDest, nFlag, nCmdCode );		
		TE_SetNeedWriteFlash( );
	*/
		break;	
	case	CC_TE_RESTORE_FACTORY:			 //恢愎出厂设置
		TE_SetNeedWriteFlash( );
		InitializeAlarmFlash();
		InitializeGSMFlash();
		nLen = PL_ComposeACKData( pDest, 1, nCmdCode );	
		TE_SetNeedWriteFlash();
		break;	
	case	CC_TE_READ_TRACKTIME:	 		 //读取定位时间
		nLen = PL_ComposeTrackTime( pDest, nCmdCode , 1);
		break;
	case	CC_TE_READ_NUMBER:				 //读取授权号码
		nLen = pSrc[WHERE_MESSAGE_DATA];
		if( nLen == 0 ){
			pTmp = g_stGSMSetup.m_strMaterNo;
		}
		else if( nLen == 1){
			pTmp = g_stGSMSetup.m_strAuthNo1;
		}
		else if(nLen == 2){
			pTmp = g_stGSMSetup.m_strAuthNo2;
		}
		else if(nLen == 3){
			pTmp = g_stGSMSetup.m_strAuthNo3;
		}
		nLen = PL_ComposeReadNumber( pDest, nLen, pTmp );	
		break;
	
	case	CC_TE_READ_OVERSPEED:			 //读取超速报警
		nLen = PL_ComposeACKData(pDest,  g_stAlarmSetup.m_nOverspeed/10, CC_TE_READ_OVERSPEED );
		break;
	case	CC_TE_READ_OUTGEO:				 //读取移位报警
		nTmp = 0;
		switch( g_stCustomSetup.m_nDistance ){
		case 0:
			nTmp = 0;
			break;
		case 100:
			nTmp = 3;
			break;
		case 200:
			nTmp = 4;
			break;
		case 300:
			nTmp = 5;
			break;
		case 500:
			nTmp = 6;
			break;
		case 1000:
			nTmp = 7;
			break;
		case 2000:
			nTmp = 8;
			break;
		}
		nLen = PL_ComposeACKData( pDest, nTmp&0xFF, nCmdCode );
		break;
	case	CC_TE_READ_FENCE:				 //读取电子栅栏
		nLen = PL_ComposeGPSData( pDest );
		break;	
	
	case	CC_TE_SINGLE_LOCATION_REPORT:	 //单一上传	
		nLen = PL_ComposeGPSData( pDest );	
		break;	
	case	CC_TE_LISTEN_CAR:					 //发送车载监听
		memcpy(nCaller, &pSrc[WHERE_MESSAGE_DATA], 16);
		if( strlen((char*)nCaller) > 3 ){			
			CMM_DialNumber( nCaller );
			#ifdef HW_ET201B
			HW_PORT_SPKMUTE_CLOE();				//静音开启 
			#endif
			#ifdef HW_ET201B_2
			HW_PORT_SPKMUTE_CLOE();				//静音开启 
			#endif
			#ifdef HW_ET201B_3
			HW_PORT_SPKMUTE_CLOE();				//静音开启 
			#endif
		}
		else{
			nLen = PL_ComposeACKData( pDest, 0, nCmdCode );
		}
		break;
	case	CC_TE_TWO_WAY_CALL:					 //设置双向通话
		memcpy(nCaller, &pSrc[WHERE_MESSAGE_DATA], 16);
		if( strlen((char*)nCaller) > 3 ){			
			CMM_DialNumber( nCaller );
			#ifdef HW_ET201B
			HW_PORT_SPKMUTE_OPEN();  			//静音关闭
			#endif
			#ifdef HW_ET201B_2
			HW_PORT_SPKMUTE_OPEN();  			//静音关闭
			#endif
			#ifdef HW_ET201B_3
			HW_PORT_SPKMUTE_OPEN();  			//静音关闭
			#endif
		}
		else{
			nLen = PL_ComposeACKData( pDest, 0, nCmdCode );
		}		
		break;
	case	CC_TE_FUEL_CONTROL:				 //设置油路控制
		if( pSrc[WHERE_MESSAGE_DATA] == 1){    //断油
			HW_CTRLOIL_CUTOFF();
			HW_SetOilState();	
			GSM_SetOilState();
		}
		else if(pSrc[WHERE_MESSAGE_DATA] == 0){ //恢复		
			HW_CTRLOIL_RESUME();
			HW_ClrOilState( );			
			GSM_ClrOilState();
		} 
		nLen = PL_ComposeACKData( pDest, 1, nCmdCode );
		break;
	case	CC_TE_CENTER_LOCK_CONTROL:		 	 //设置中控锁	
		if( pSrc[WHERE_MESSAGE_DATA] == 1){      //锁
			HW_ClrDoorState( );
			TE_SetNeedLockFlag();
		}
		else if(pSrc[WHERE_MESSAGE_DATA] == 0){  //解锁
			HW_SetDoorState( );
			TE_SetNeedUnLockFlag();
		}
		nLen = PL_ComposeACKData( pDest, 1, nCmdCode ); 
		break;	
	case CC_TE_ALARM_CMD:						 
		nLen = PL_ComposeGPSData( pDest );				
		break;
	case CC_TE_ALARM_SETUP:						 //报警状态字设置
		nLen = pSrc[WHERE_MESSAGE_DATA];
		memcpy( &nTmp, &pSrc[WHERE_MESSAGE_DATA+1], 4);	
		if( nLen == 0xFF ){	//关闭报警状态
			AL_CancelAlarm( nTmp );
			GPRS_ClrNumberSMSMode( nTmp );
		}
		else if( nLen == 0x00 ){
			TE_SetNeedWriteFlash();
			g_stAlarmSetup.m_nAlarmEnable = nTmp;			
		}				
		nLen = PL_ComposeACKData( pDest, 1, nCmdCode );
		break;
	case CC_TE_READ_ALARM_STATUS:				//读报警状态
		PL_ComposeReadAlarm( pDest, nCmdCode );
		break;
	case SETUP_ALARM_PARKING:					//设置停车报警
		nTmp = (pSrc[WHERE_MESSAGE_DATA]<<8) & 0xFF00;
		nTmp += pSrc[WHERE_MESSAGE_DATA+1];
		if( nTmp == 0 ){
			AL_DisableParking( );
		}
		else{
			g_stAlarmSetup.m_nParking = (short)nTmp;
			AL_EnableParking();
		}
		nLen = PL_ComposeACKData( pDest, 1, nCmdCode ); 
		TE_SetNeedWriteFlash( ); 
		break;
	case SETUP_ALARM_ANTISTEAL:				// 非法开门报警
		if ( pSrc[WHERE_MESSAGE_DATA] == 0 ){
			AL_DisableNotDoor( );
		}
		else{
			g_stAlarmSetup.m_nILLDoorOpen = pSrc[WHERE_MESSAGE_DATA];	
			AL_EnableNotDoor( );
		}
		nLen = PL_ComposeACKData( pDest, 1, nCmdCode ); 
		TE_SetNeedWriteFlash( ); 
		break;
	case SETUP_ALARM_TIREDNESS:				// 疲劳驾驶设置	
		if ( pSrc[WHERE_MESSAGE_DATA]  == 0 ){
			AL_DisableTireDrive();
		}
		else{
			g_stAlarmSetup.m_nTireDriver = pSrc[WHERE_MESSAGE_DATA] ;
			AL_EnableTireDrive();
		}
		nLen = PL_ComposeACKData( pDest, 1, nCmdCode ); 
		TE_SetNeedWriteFlash( ); 
		break;
	case CC_TE_MULTI_SETUP:				  //多路控制
		nFlag = PL_ComposeMultiSetup( pSrc );
		nLen = PL_ComposeACKData( pDest, nFlag, nCmdCode ); 
		break;
	}
	return nLen;
}
////////////////////////////////////////////////////////////////////////////////
//   多路控制
//   一共四字节
//  1 Byte: 控制油路  0: 恢复油路	1: 断油	  2: 不动作
//  2 Byte: 控制上锁  0: 开锁		1: 上锁   2: 不动作
unsigned char	PL_ComposeMultiSetup( unsigned char	*pSrc ){

	unsigned char			nResult = 1;
	
	// 油路控制
	switch( pSrc[WHERE_MESSAGE_DATA] ){
	case TE_CTRL_ENABLE:
        HW_ClrOilState( );
		HW_CTRLOIL_RESUME();
		GSM_ClrOilState();
		break;
	case TE_CTRL_DISABLE:
		HW_CTRLOIL_CUTOFF();
		HW_SetOilState();	
		GSM_SetOilState();
		break;	
	}
	// 锁控制
	switch( pSrc[WHERE_MESSAGE_DATA+1] ){
	case TE_CTRL_ENABLE:			//开锁
		HW_SetDoorState( );
		TE_SetNeedUnLockFlag();		//开锁		
		break;
	case TE_CTRL_DISABLE:			//锁
		HW_ClrDoorState( );
        TE_SetNeedLockFlag();		
		break;	
	}
	return nResult;
}
////////////////////////////////////////////////////////////////////////////////
//  设置号码
unsigned char	PLS_SetupGpsMasterNo( unsigned char	*pSrc ){

	unsigned 	char		nResult = 1;
	unsigned    char		nPNumber = 0;
	unsigned    char		nPhone[20];
	
	memset( nPhone, 0x00, 20);
	nPNumber = pSrc[WHERE_MESSAGE_DATA];
	memcpy( nPhone, &pSrc[WHERE_MESSAGE_DATA+1], 16);
	switch( nPNumber ){
	case 0:
		strncpy( (char*)g_stGSMSetup.m_strMaterNo, (char*)nPhone, TELNUMBER_LENGTH);
		break;
	case 1:
		strncpy( (char*)g_stGSMSetup.m_strAuthNo1, (char*)nPhone, TELNUMBER_LENGTH);
		break;
	case 2:
		strncpy( (char*)g_stGSMSetup.m_strAuthNo2, (char*)nPhone, TELNUMBER_LENGTH);
		break;
	case 3:
		strncpy( (char*)g_stGSMSetup.m_strAuthNo3, (char*)nPhone, TELNUMBER_LENGTH);
		break;
	default:
		nResult = 0;
		break;
	}
	return nResult;
}
///////////////////////////////////////////////////////////////////
//	设置区域移位报警
unsigned char	PLS_SetupGpsOutGeo( unsigned char	*pSrc ){

	unsigned  char		nResult = 1;
	unsigned  char		nFlag = 0;
	
	nFlag =	pSrc[WHERE_MESSAGE_DATA];
	switch( nFlag ){
	case 0:
		g_stCustomSetup.m_nDistance = 0;
		TE_DisableOutGeo();	
		break;
	case 1:
	case 2:
	case 3:
		g_stCustomSetup.m_nDistance = 100;
		break;
	case 4:
		g_stCustomSetup.m_nDistance = 200;
		break;
	case 5:
		g_stCustomSetup.m_nDistance = 300;
		break;
	case 6:
		g_stCustomSetup.m_nDistance = 500;
		break;
	case 7:
		g_stCustomSetup.m_nDistance = 1000; 
		break;
	case 8:
		g_stCustomSetup.m_nDistance = 2000;
		break;
	default:
		nResult = 0;
		break;
	}
	if( (nFlag > 0) && (nFlag < 9) ){
		if( GPS_IsPosValid() ){		//GPS定位有效			
			g_stCustomSetup.m_nCLng = GPS_GetLongitude();	//经度
			g_stCustomSetup.m_nCLat = GPS_GetLatitude();	//纬度			
			TE_EnableOutGeo();
		}		
		else{
			g_stCustomSetup.m_nDistance = 0;
			TE_DisableOutGeo();	
			nResult = 0;
		}	
	}
	return nResult;
}
/*
///////////////////////////////////////////////////////////////////
//	设置电子围栏报警
unsigned char	PLS_SetupFence( unsigned char	*pSrc,
								unsigned char	nCmdCode ){

	unsigned char	nResult = 1;
	unsigned char	*pch = NULL;
	unsigned char	*pch1 = NULL;
	
	
	pch = pSrc+WHERE_MESSAGE_DATA;	
	// 经度
	pch1 = (unsigned char*)strchr((char*)pch, ',');
	if( pch1 == NULL ){
	  return 0;
	}
	*pch1 = 0x00;
	g_stAlarmSetup.m_oStGFence[0].m_LLng = PLS_ExplainLongitudeData( pch );
	pch = pch1 + 1;
	if( (*pch == 'E') || (*pch == 'e') ){
		g_stAlarmSetup.m_oStGFence[0].m_LLng *= 1;
	}
	else{
		g_stAlarmSetup.m_oStGFence[0].m_LLng *= -1;
	}
	pch += 2;
	// 纬度
	pch1 = (unsigned char*)strchr((char*)pch, ',');
	if( pch1 == NULL ){
	  return 0;
	}
	*pch1 = 0x00;
	g_stAlarmSetup.m_oStGFence[0].m_LLat = PLS_ExplainLatitudeData(pch);
	pch = pch1 + 1;
	if( (*pch == 'N') || (*pch=='n') ){
		g_stAlarmSetup.m_oStGFence[0].m_LLat *= 1;
	}
	else{
		g_stAlarmSetup.m_oStGFence[0].m_LLat *= -1;
	}
	pch += 2;
	// 经度
	pch1 = (unsigned char*)strchr((char*)pch, ',');
	if( pch1 == NULL ){
		return 0;
	}
	*pch1 = 0x00;
	g_stAlarmSetup.m_oStGFence[0].m_RLng = PLS_ExplainLongitudeData(pch);
	pch = pch1 + 1;
	if( (*pch == 'E') || (*pch == 'e') ){
		g_stAlarmSetup.m_oStGFence[0].m_RLng *= 1;
	}
	else{
		g_stAlarmSetup.m_oStGFence[0].m_RLng *= -1;
	}
	pch += 2;
	// 纬度
	pch1 = (unsigned char*)strchr((char*)pch, ',');
	if( pch1 == NULL ){
	  return 0;
	}
	*pch1 = 0x00;
	g_stAlarmSetup.m_oStGFence[0].m_RLat = PLS_ExplainLatitudeData(pch);
	pch = pch1 + 1;
	if( (*pch == 'N') || (*pch=='n') ){
		g_stAlarmSetup.m_oStGFence[0].m_RLat *= 1;
	}
	else{
		g_stAlarmSetup.m_oStGFence[0].m_RLat *= -1;
	}	
	// 标志位
	g_stAlarmSetup.m_oStGFence[0].m_nFlag |= 0x21;
	return nResult;	
}
//========================================================================================
//解释纬度函数
//参数:
//      pData 输入参数
//========================================================================================
int 	PLS_ExplainLatitudeData(unsigned char *pData)			
{
	unsigned char  nLen = 0;	
	unsigned char  nDegree = 0;
	unsigned char  nMinute = 0;
	unsigned short nPrecision = 0;	
	int			   nResult = 0;
	char 		   *pch = NULL;
	char		   *pch1 = NULL;
		
	pch = (char *)pData;     		//获取数据
	pch1 = strchr(pch,'.');     	//取小数点前的数据
	if(pch1)
	{
		pch[pch1 - pch] = 0;
		nLen = strlen(pch);
		if (nLen == 1)	{
			nMinute = (*pch) - '0';	
		}
		else if (nLen == 2) {
			nMinute = ((*pch) - '0') * 10;	
			pch++;
			nMinute += (*pch) - '0';	
		}
		else if (nLen == 3) {
			nDegree = (*pch) - '0';
			pch++;
			nMinute = ((*pch) - '0') * 10;	
			pch++;
			nMinute += (*pch) - '0';	
		}
		else if (nLen == 4) {
			nDegree = ((*pch) - '0') * 10;
			pch++;
			nDegree += (*pch) - '0';
			pch++;
			nMinute = ((*pch) - '0') * 10;	
			pch++;
			nMinute += (*pch) - '0';
		}
		// 取小数位	
		pch = pch1 + 1;					
		nPrecision = atoi( (char*)pch );
		
		nResult = nDegree*3600000;
		nResult += nMinute * 60000;
		nResult += nPrecision * 6;
	}		
	return nResult;
}
//========================================================================================
// 解释经度函数
// 参数:
//      pData 输入参数24.1278 0001.23 
//========================================================================================
int  PLS_ExplainLongitudeData(unsigned char *pData)     
{
	
	unsigned char  nLen = 0;	
	unsigned char  nDegree = 0;
	unsigned char  nMinute = 0;
	unsigned short nPrecision = 0;	
	int			   nResult = 0;
	char 		   *pch = NULL;
	char		   *pch1 = NULL;
	
	
	pch = (char *)pData;     		//获取数据
	pch1 = strchr(pch,'.');     	//取小数点前的数据   
	if( pch1 ){			
		pch[pch1 - pch] = 0;
		nLen = strlen(pch);
		nDegree = 0;
		if (nLen == 1)		{
			nMinute = (*pch) - '0';	
		}
		else if (nLen == 2) {
			nMinute = ((*pch) - '0') * 10;
			pch++;
			nMinute += (*pch) - '0';	
		}
		else if (nLen == 3) {
			nDegree = (*pch) - '0';
			pch++;
			nMinute = ((*pch) - '0') * 10;
			pch++;
			nMinute += (*pch) - '0';	
		}
		else if (nLen == 4) {
			nDegree = ((*pch) - '0') * 10;
			pch++;
			nDegree += (*pch) - '0';
			pch++;
			nMinute = ((*pch) - '0') * 10;
			pch++;
			nMinute += (*pch) - '0';			
		}
		else if (nLen == 5)  {
			nDegree = ((*pch) - '0') * 100;
			pch++;
			nDegree += ((*pch) - '0') * 10;
			pch++;
			nDegree += (*pch) - '0';
			pch++;
			nMinute = ((*pch) - '0') * 10;	
			pch++;
			nMinute += (*pch) - '0';
		}
	}
	// 提取小数位
	pch = pch1 + 1;      
	nPrecision = atoi( (char*)pch );
	
	nResult = nDegree*3600000;
	nResult += nMinute * 60000;
	nResult += nPrecision * 6;
	
	return nResult;	
}
*/
#endif  //SW_GPRSPROTOCOL_ASICO


