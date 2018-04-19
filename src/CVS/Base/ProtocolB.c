///////////////////////////////////////////////////////////////////////////
//   file name: ProtocolData.h
// description: GSM/GPRS协议头文件定义
//      author:
//        date: 2009-02-15
//     reamark: 
#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>
#include  "..\hdr\includes.h"
#include  "..\hdr\ProtocolA.h"
#include  "..\hdr\ProtocolB.h"
#include  "..\hdr\ProtocolC.h"
#include  "..\hdr\ProtocolASICO.h"
#include  "..\hdr\APP_LoopDetect.h" 
#include  "..\hdr\APP_Terminal.h"
#include  "..\hdr\HW_GPIO.h"
#include  "..\hdr\APP_GSM.h"
#include  "..\hdr\APP_GTM900.h"
#include  "..\hdr\APP_BQM26.h"
#include  "..\hdr\APP_EM200.h"
#include  "..\hdr\APP_MC323.h"
#include  "..\hdr\APP_SIMCOM.h"
#include  "..\hdr\APP_GPS.h"
#include  "..\hdr\SMSProtocolA.h"
#include  "..\hdr\SMSProtocolB.h"
#include  "..\hdr\SMSProtocolC.h"

#ifdef	SW_GPRSPROTOCOLB
static unsigned char		s_sequnce = 0;
/////////////////////////////////////////////////////////////////////////////
//  增加表头
void  PL_AddHead( unsigned char	*pBuf ){
	
	*pBuf++ = SYN_HEADERCODE1;
	*pBuf = SYN_HEADERCODE2;	
}
/////////////////////////////////////////////////////////////////////////////
//  增加GPS数据
void  PL_AddGPSData( unsigned char *pDataBuf ){
	
	int			nTmpData = 0;
		
	//时间
	pDataBuf[0] = g_stGPSInfo.m_nYear;
	pDataBuf[1] = g_stGPSInfo.m_nMonth;
	pDataBuf[2] = g_stGPSInfo.m_nDay;
	pDataBuf[3] = g_stGPSInfo.m_nHour;
	pDataBuf[4] = g_stGPSInfo.m_nMinute;
	pDataBuf[5] = g_stGPSInfo.m_nSecond;
	//定位标志
	pDataBuf[6] = g_stGPSInfo.m_bPosValid;
	//经度	
	nTmpData = GPS_GetLongitude();	
	memcpy( &pDataBuf[7], &nTmpData, 4);
	//纬度
	nTmpData = GPS_GetLatitude();	
	memcpy( &pDataBuf[11], &nTmpData, 4);
	//方向
	memcpy( &pDataBuf[15],  &g_stGPSInfo.m_nCurDirection, 2 );
	//速度	
	nTmpData = GPS_GetMilimetre();	
	pDataBuf[17] =  nTmpData & 0xff;	
	
}
/////////////////////////////////////////////////////////////////////////////
//  获取硬件状态/获取报警状态/运行时间/状态码
void  PL_AddDEState( unsigned char *pDataBuf ){

	unsigned int  	nTmp = 0;
		
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
	memcpy( &pDataBuf[0], &g_stDeviceState.m_nHWState, 4);
	memcpy( &pDataBuf[4], &g_stDeviceState.m_nALState, 4);
	// ACC运行时间分钟
	nTmp = g_stAlarmSetup.m_nDriveTime / 120;
	memcpy( &pDataBuf[8], &nTmp, 2);
	memcpy( &pDataBuf[10],&g_stDeviceState.m_nCodeState, 1);
}
/////////////////////////////////////////////////////////////////////////////
// 
void  PL_AddIpAddrN( unsigned char	*pDataBuf ){
	
	unsigned char	*pData  = NULL;
	unsigned char	*pData1 = NULL;
	
	pData = g_stGSMSetup.m_strServerIP;
	
	// 1
	pData1 = (unsigned char*)strchr((char*)pData, TEXT_POINT );
	if ( pData1 == NULL ){
		return;
	}
	pDataBuf[0] = (unsigned char)atoi( (char*)pData );
	pData = pData1 + 1;
	
	// 2	
	pData1 = (unsigned char*)strchr( (char*)pData, TEXT_POINT );
	if ( pData1 == NULL ){
		return ;
	}
	pDataBuf[1] = (unsigned char)atoi((char*)pData );
	pData = pData1 + 1;
	
	// 3
	pData1 = (unsigned char*)strchr( (char*)pData, TEXT_POINT );
	if ( pData1 == NULL ){
		return ;
	}	
	pDataBuf[2] = (unsigned char)atoi( (char*)pData );
	pData = pData1 + 1;
	// 4
	pDataBuf[3] = (unsigned char)atoi( (char*)pData );
}

/////////////////////////////////////////////////////////////////////////////
//  组织GPS数据
unsigned char 	 PL_ComposeGPSData( unsigned char  *pDest ){	

	unsigned char			nLen  = 0;
									 	
	//  表头(2)
	PL_AddHead( pDest );
	//  消息类型(1)
	pDest[WHERE_MESSAGE_TYPE] = MT_SUBMIT;	 
	//  总长度(1)
	pDest[WHERE_MESSAGE_TOTAL_LENGHT] = 40;
	//  总包数(1)
	pDest[WHERE_MESSAGE_SEQNUM] = s_sequnce++;	
	//  DEUID长度(1)+DEUID内容(n)
	PL_AddTEUID( &pDest[5] );
	//  数据类型(1)
	pDest[WHERE_MESSAGE_DATA_TYPE] = CC_GPS_STATE;	
	//  GPS数据长度+硬件状态
	pDest[15] = 29;	
	//  GPS数据(18)
	PL_AddGPSData( &pDest[16] );
	//  硬件状态(11)
	PL_AddDEState( &pDest[34] );
	//  校验和
	pDest[45] = PL_CountVerifySum(pDest, 45);
	nLen = 46;
	
	return nLen;
}
/////////////////////////////////////////////////////////////////////////////
//  组织googleGPS数据
unsigned char 	 PL_ComposeGGGPSData( unsigned char  *pDest, unsigned char	nCmdType ){	

	unsigned char			nLen  = 0;
									 	
	//  表头(2)
	PL_AddHead( pDest );
	//  消息类型(1)
	pDest[WHERE_MESSAGE_TYPE] = MT_SUBMIT;	 
	//  总长度(1)
	pDest[WHERE_MESSAGE_TOTAL_LENGHT] = 40;
	//  总包数(1)
	pDest[WHERE_MESSAGE_SEQNUM] = s_sequnce++;	
	//  DEUID长度(1)+DEUID内容(n)
	PL_AddTEUID( &pDest[5] );
	//  数据类型(1)
	pDest[WHERE_MESSAGE_DATA_TYPE] = nCmdType;	
	//  GPS数据长度+硬件状态
	pDest[15] = 29;	
	//  GPS数据(18)
	PL_AddGPSData( &pDest[16] );
	//  硬件状态(11)
	PL_AddDEState( &pDest[34] );
	//  校验和
	pDest[45] = PL_CountVerifySum(pDest, 45);
	nLen = 46;
	
	return nLen;
}

#ifdef EXTERNAL_FLASH
///////////////////////////////////////////////////////////////////////////////////
//	 组织盲点数据上传
//		pDest :	  输出参数
//		pSrc  :   输入参数
//nPackageSize:   包数据大小
// 返回类型   :   返回数据长度
unsigned char	PL_ComposeGPSBlindData( unsigned char	*pDest, 
										unsigned char	*pSrc,  
										unsigned char	nPackageSize ){

	unsigned char	nCnt;
	unsigned char	nLen;
    unsigned char	nMessageType = 0;
	
	//  表头(2)
	PL_AddHead( pDest );
	//  消息类型(1)
	nMessageType = MT_SUBMIT;
	if( nPackageSize > 1 ){		
		nMessageType |= 0x80;
	}		
	pDest[WHERE_MESSAGE_TYPE] = nMessageType;	 
	//  总长度(1)
	pDest[WHERE_MESSAGE_TOTAL_LENGHT] = 0x00;
	//  总包数(1)
	pDest[WHERE_MESSAGE_SEQNUM] = s_sequnce++;	
	//  DEUID长度(1)+DEUID内容(n)
	PL_AddTEUID( &pDest[5] );
	
	if( nMessageType & 0x80 ){
		// 总包数
		pDest[WHERE_MESSAGE_DATA_TYPE] = nPackageSize;
		
		nLen = WHERE_MESSAGE_DATA_TYPE+1;
		for( nCnt = 0; nCnt < nPackageSize; nCnt++ ){
			
			pDest[nLen+nCnt*31] = CC_BLIND_GPS_STATE;
			pDest[nLen+1+nCnt*31] = 29;
			memcpy( &pDest[nLen+2+nCnt*31], &pSrc[nCnt*32], 29 );			
		}
		// GPS数据 29+命令码+长度
		nLen = nPackageSize*31;	
		// 表头(2)+主命令(1)+总长度(1)+累加号(1)+DE长度(1)+DE内容(8)+包数(1)
		nLen += 14+1;					
	}
	else{
		pDest[WHERE_MESSAGE_DATA_TYPE] = CC_BLIND_GPS_STATE;
		pDest[15] = 29;
		memcpy( &pDest[16], pSrc, 29 );
		nLen = 45;
	}
	//  总长度(1) 表头(2)+命令码(1)+总长茺(1)+序号(1)+Data内容+结束(1)
	pDest[WHERE_MESSAGE_TOTAL_LENGHT] = nLen+1-6;
	//  校验和	
	pDest[nLen] = PL_CountVerifySum(pDest, nLen );
	//  总长度
	nLen += 1;
	
	return nLen;
}
#endif //EXTERNAL_FLASH

/////////////////////////////////////////////////////////////////////////////
//	组织GSM数据信息
unsigned char	PL_ComposeGSMData( unsigned char	*pDest ){

	unsigned char			nLen = 0;
	int						nLac = 0;
	int						nID = 0;

	//  查询小区号的地址
	CMM_QueryNetworkLac( &nLac, &nID );
	//  表头(2)
	PL_AddHead( pDest );
	//  消息类型(1)
	pDest[WHERE_MESSAGE_TYPE] = MT_SUBMIT;	 
	//  总长度(1)
	pDest[WHERE_MESSAGE_TOTAL_LENGHT] = 32;
	//  总包数(1)
	pDest[WHERE_MESSAGE_SEQNUM] = s_sequnce++;	
	//  DEUID长度(1)+DEUID内容(n)
	PL_AddTEUID( &pDest[5] );
	//  数据类型(1)
	pDest[WHERE_MESSAGE_DATA_TYPE] = CC_GSM_STATE;	
	//  GSM数据长度+硬件状态
	pDest[15] = 21;		
	//  GPS时间
	pDest[16] = g_stGPSInfo.m_nYear;
	pDest[17] = g_stGPSInfo.m_nMonth;
	pDest[18] = g_stGPSInfo.m_nDay;
	pDest[19] = g_stGPSInfo.m_nHour;
	pDest[20] = g_stGPSInfo.m_nMinute;
	pDest[21] = g_stGPSInfo.m_nSecond;
	//  nLac 小区 2 Byte 
	memcpy( &pDest[22], &nLac, 2 );
	//  nID 地址  2 Byte
	memcpy( &pDest[24], &nID, 2 );
	//  状态信息
	PL_AddDEState( &pDest[26] );
	//  校验和
	pDest[37] = PL_CountVerifySum(pDest, 37);
	
	nLen = 38;	
	return nLen;
}
/////////////////////////////////////////////////////////////////////////////
// 织织握手信息
unsigned char 	 PL_ComposeHSData( unsigned char	*pDest ){
	
	unsigned  char	nResultLen = 0;
		
	//  表头(2)
	PL_AddHead( pDest );
	//  消息类型(1)
	pDest[WHERE_MESSAGE_TYPE] = MT_HANDSHAKE;	
	//  总长度(1)
	pDest[WHERE_MESSAGE_TOTAL_LENGHT] = 10;
	//  序号包数(1)
	pDest[WHERE_MESSAGE_SEQNUM] = s_sequnce++;	
	//  DEUID长度(1)+DEUID内容(n)
	PL_AddTEUID( &pDest[5] );
	//  数据类型(1)
	pDest[WHERE_MESSAGE_DATA_TYPE] = 0;	
	//  校验和
	pDest[15] = PL_CountVerifySum(pDest, 15 );	
	nResultLen = 16;
	
	return nResultLen;
}
/////////////////////////////////////////////////////////////////////////////
//
unsigned char	PL_ComposeLogin( unsigned char  *pDest ){ 
	return PL_ComposeHWSWSetup( MT_LOGIN, pDest );	
}
/////////////////////////////////////////////////////////////////////////////
//软件设置情况 
//表头(2)+消息类型(1)+总长度(1)+总包数(1)+DEUID长度(1)+DEUID内容(n)+数据类型(1)+数据长度(1)+数据内容(n)+校验和(1)
unsigned char	PL_ComposeHWSWSetup( unsigned char nMessageType ,unsigned char  *pDest ){
	
	unsigned short	nTmp = 0;
	
	
	memset( pDest, 0x00, 133 );
	//  表头(2)
	PL_AddHead( pDest );
	//  消息类型(1)
	pDest[WHERE_MESSAGE_TYPE] = nMessageType;	
	//  总长度(1)
	pDest[WHERE_MESSAGE_TOTAL_LENGHT] =  127;
	//  序号包数(1)
	pDest[WHERE_MESSAGE_SEQNUM] = s_sequnce++;
	//  DEUID长度(1)+DEUID内容(n)
	PL_AddTEUID( &pDest[5] );
	//  数据类型(1)
	pDest[WHERE_MESSAGE_DATA_TYPE] = CC_SW_SETUP;
	
	//	报警状态(4)
	memcpy( &pDest[WHERE_MESSAGE_DATA_ALSTATE], &g_stAlarmSetup.m_nALValid, 4 );
	//  硬件状态(4)
	memcpy( &pDest[WHERE_MESSAGE_DATA_HWSTATE], &g_stAlarmSetup.m_nHWValid, 4 );
		
	//  报警开关(4)
	memcpy( &pDest[WHERE_MESSAGE_ALARM_ENABLE], &g_stAlarmSetup.m_nAlarmEnable, 4 );	
	//  超速报警(1)
	pDest[WHERE_MESSAGE_OVERSPEED] = g_stAlarmSetup.m_nOverspeed;	
	//  疲劳驾驶(1)
	pDest[WHERE_MESSAGE_TIREDNESS] = g_stAlarmSetup.m_nTireDriver;
	//  非法开门报警设置(1)
	pDest[WHERE_MESSAGE_ANTISTERAL] = g_stAlarmSetup.m_nILLDoorOpen;
	//  停车报警(1)
	memcpy( &pDest[WHERE_MESSAGE_PARKING], &g_stAlarmSetup.m_nParking, 2);
	
	//  电子围栏1	方向标志+电子围栏+是否出入
	memset( &pDest[WHERE_MESSAGE_AREA1], 0x00, 18);	
	//  电子围栏2   方向标志+电子围栏+是否出入
	memset( &pDest[WHERE_MESSAGE_AREA2], 0x00, 18);
		
	//  服务器端口号(2)
	memcpy( &pDest[WHERE_MESSAGE_SERVERPORT], &g_stGSMSetup.m_nServerPort , 2);
	//  服务器IP地址(4)
	PL_AddIpAddrN( &pDest[WHERE_MESSAGE_SERVERIP] );	
	//  回传模式(3)
	pDest[WHERE_MESSAGE_CBMODE] = g_stGSMSetup.m_nCBMode;
	memcpy(  &pDest[WHERE_MESSAGE_CBMODE+1], &g_stGSMSetup.m_nCBSpace, 2 );
			
	//  主控号码 1:号码长度  10:号码内容
	nTmp = strlen( (char*)g_stGSMSetup.m_strMaterNo );		
	nTmp = P_StrToBCD(&pDest[WHERE_MESSAGE_MATERNO+1], g_stGSMSetup.m_strMaterNo, nTmp ); 
	pDest[WHERE_MESSAGE_MATERNO] = nTmp;
	
	//   短消息中心号码 1:号码长度  10:号码内容
	nTmp = strlen( (char*)g_stGSMSetup.m_strSMCNo );		
	nTmp = P_StrToBCD( &pDest[WHERE_MESSAGE_SMCNO+1], g_stGSMSetup.m_strSMCNo, nTmp ); 
	pDest[WHERE_MESSAGE_SMCNO] = nTmp;
	
	//   授权号码1 1:号码长度  10:号码内容
	nTmp = strlen( (char*)g_stGSMSetup.m_strAuthNo1 );		
	nTmp = P_StrToBCD( &pDest[WHERE_MESSAGE_AUTHNO1+1], g_stGSMSetup.m_strAuthNo1, nTmp ); 
	pDest[WHERE_MESSAGE_AUTHNO1] = nTmp;
	
	//   授权号码2 1:号码长度  10:号码内容
	nTmp = strlen( (char*)g_stGSMSetup.m_strAuthNo2 );		
	nTmp = P_StrToBCD( &pDest[WHERE_MESSAGE_AUTHNO2+1], g_stGSMSetup.m_strAuthNo2, nTmp ); 
	pDest[WHERE_MESSAGE_AUTHNO2] = nTmp;
	
	//   授权号码3 1:号码长度  10:号码内容
	nTmp = strlen( (char*)g_stGSMSetup.m_strAuthNo3 );		
	nTmp = P_StrToBCD( &pDest[WHERE_MESSAGE_AUTHNO3+1], g_stGSMSetup.m_strAuthNo3, nTmp ); 
	pDest[WHERE_MESSAGE_AUTHNO3] = nTmp;
	pDest[WHERE_MESSAGE_END] = PL_CountVerifySum( pDest, WHERE_MESSAGE_END);
	
	return WHERE_MESSAGE_END+1;
}

///////////////////////////////////////////////////////////////
// 	解释协议数据
unsigned char	PL_ParseData(unsigned char *pDest, unsigned char *pSrc, unsigned char nLen ){
									 	
	unsigned char	 	nResultLen = 0;
	unsigned char		nSequnce = 0;
	unsigned char		nDEUID[DEUID_LENGTH+1];
			
	if( pSrc[WHERE_MESSAGE_HEAD1] != SYN_HEADERCODE1 ||
			pSrc[WHERE_MESSAGE_HEAD2] != SYN_HEADERCODE2 ){
		return 0;
	}		
	
	nSequnce = pSrc[WHERE_MESSAGE_SEQNUM];
	
	s_sequnce = nSequnce;
	
	// DEUID compare
	nLen = pSrc[WHERE_MESSAGE_DEUIDLEN];
	if( nLen > DEUID_LENGTH ){
		nLen = DEUID_LENGTH;
	}
	memcpy( (char*)nDEUID, (char*)&pSrc[WHERE_MESSAGE_DEUID], nLen );
	nDEUID[nLen] = 0x00;	
	if( !strcmp_back( g_stGSMSetup.m_strUID, nDEUID, nLen ) ){
		return 0;
	}
			
	// 数据类型		
	switch( pSrc[WHERE_MESSAGE_TYPE] ){			
	case MT_DELIVERY:
		
		nResultLen = PL_ParseDelivery( pDest, pSrc+WHERE_MESSAGE_DATA_TYPE );						
		break;
	
	// 设置为登陆成功
	case MT_LOGIN_ACK:		
		TE_SetLoginSuccess( );	
		UDP_ClrHandShakeCnt( );
		TCP_ClrRCntionCnt();
		IP_ClrConnectFailedCnt( );
		break;
		
	// 收到握手应答数据
	case MT_ACK:				
		UDP_ClrHandShakeCnt( );	
		TCP_ClrRCntionCnt();
		IP_ClrConnectFailedCnt( );
		break;	
	}
	return nResultLen;									 	
}
////////////////////////////////////////////////////////////////////////////
//   发送终端数据信息  MT_DELIVERY
unsigned char	PL_ParseDelivery( unsigned char *pDest, unsigned char *pSrc ){								     	
	
	unsigned char		nFlag	= 0;
	unsigned char		nLen    = 0;
	unsigned char		nGSMState = 0;
	unsigned char		nCodeState = 0;
	unsigned char 		nCaller[TELNUMBER_LENGTH+1];

	nCodeState = *pSrc++;	
	switch( nCodeState ){
	case  CC_TE_QUERY_SW_SETUP:									// 6.	读取软件设置信息
		nLen = PL_ComposeHWSWSetup( MT_SUBMIT ,pDest );	
		g_stDeviceState.m_nCodeState = 0;
		nFlag = 1;	
		break;
		
	case  CC_TE_CTRLOIL:										// 7.	关闭/恢愎油路信息
		pSrc++;   												// 长度
		if( *pSrc == TE_CTRL_DISABLE ){							// 断油	
			HW_CTRLOIL_CUTOFF();
			HW_SetOilState();	
			GSM_SetOilState();
			g_stDeviceState.m_nCodeState = ACK_DISABLE_OIL;
		}
		else{																			// 恢复油路
			HW_ClrOilState( );
			HW_CTRLOIL_RESUME();
			GSM_ClrOilState();
			g_stDeviceState.m_nCodeState = ACK_ENABLE_OIL;
		}	
		TE_SetNeedWriteFlash( );	
		nFlag = 1;
		break;
		
	case  CC_TE_CTRLDOOR:										//8.	开/关 车门
		pSrc++;
		if ( *pSrc == TE_CTRL_DISABLE ){						//门关
			HW_ClrDoorState( );
			TE_SetNeedLockFlag();		//锁
			g_stDeviceState.m_nCodeState = ACK_CLOSE_DOOR;
		}
		else{																			//门开
			HW_SetDoorState( );
			TE_SetNeedUnLockFlag();		//开锁	
			g_stDeviceState.m_nCodeState = ACK_OPEN_DOOR;	
		}	
		nFlag = 1;
		break;
	
	case  CC_TE_CALLONCE:										//9.	点名呼叫: (立即回传最新信息)			
		g_stDeviceState.m_nCodeState = ACK_LOCATION;
		nFlag = 1;
		break;		
	case CC_GSM_STATE:		// GSM定位信息
		g_stDeviceState.m_nCodeState = ACK_LOCATION;
		nFlag = 1;	
		nGSMState = 1;
		break;	
	case  CC_TE_LISTEN:						//10.	电话监听: (打开车载终端话筒，远程监听车内动静)
		nLen = *pSrc++;
		if( nLen > TELNUMBER_LENGTH ){
			nLen = TELNUMBER_LENGTH;
		}
		if( nLen == 0 ){
			nFlag = 0;			
		}
		else{
			memcpy( (char*)nCaller, pSrc, nLen );
			nCaller[nLen] = 0x00;
			#ifdef HW_EM200	
			CMM_IPClose();
			CMM_DTUPPPClose();
			IP_SetConnectNG( );	
			#endif	
			#ifdef HW_MC323
			CMM_IPClose();
			CMM_DTUPPPClose();
			IP_SetConnectNG( );	
			#endif		
			if( CMM_DialNumber( nCaller ) == GSM_SUCCEED ){
				nLen = 0;
				nFlag = 1;
			}
			else{				
				nLen = 0;
				nFlag = 0;
			}
		}		
		g_stDeviceState.m_nCodeState = ACK_LISTEN;
		break;
	// 暂不支持
	case  CC_TE_TALK:						//11.	电话通话: (打开车载终端话筒，远程喊话)
		nLen = *pSrc++;
		if( nLen > TELNUMBER_LENGTH ){
			nLen = TELNUMBER_LENGTH;
		}
		if( nLen == 0 ){
			nFlag = 0;			
		}
		else{
			nFlag = 1;
			memcpy( (char*)nCaller, pSrc, nLen );			
			nCaller[nLen] = 0x00;
			#ifdef HW_EM200
			CMM_IPClose();
			CMM_DTUPPPClose();
			IP_SetConnectNG( );	
			#endif			
			#ifdef HW_MC323
			CMM_IPClose();
			CMM_DTUPPPClose();
			IP_SetConnectNG( );	
			#endif
			if( CMM_DialNumber( nCaller ) == GSM_SUCCEED ){
				nLen = 0;
				nFlag = 1;
			}
			else{				
				nLen = 0;
				nFlag = 0;
			}
			nCaller[nLen] = 0x00;		
			#ifdef HW_ET201B
			HW_PORT_SPKMUTE_OPEN();  //静音开启 
			#endif		
			#ifdef HW_ET201B_2
			HW_PORT_SPKMUTE_OPEN();  //静音开启
			#endif
			#ifdef HW_ET201B_3
			HW_PORT_SPKMUTE_OPEN();  //静音开启
			#endif
		}
		g_stDeviceState.m_nCodeState = ACK_TALK;
		break;
	case  CC_TE_SETUP_UPLOADMODE: 		    //12.	设置数据上传模式
		nFlag = PL_TESetupCBMode( pSrc );
		if( nFlag ){
			TE_SetNeedWriteFlash( );
		}
		g_stDeviceState.m_nCodeState = ACK_SETUP_MODE;
		break;
		
	case  CC_TE_SETUP_NUMBER:				//13.	设置各种号码
		nFlag = PL_TESetupNumber( pSrc );
		if( nFlag ){
			TE_SetNeedWriteFlash( );
		}
		g_stDeviceState.m_nCodeState = ACK_SETUP_PHONE;
		break;
	case  CC_TE_SETUP_SERVER:				//14.	设置服务器 
		nFlag = PL_TESetupServer( pSrc );
		if( nFlag ){
			TE_SetNeedWriteFlash( );
		}
		g_stDeviceState.m_nCodeState = ACK_SETUP_SERVER;
		break;
	case  CC_TE_SETUP_ALARM:				//15.	报警设置
		nFlag = PL_TESetupAlarm( pSrc );
		if( nFlag ){
			TE_SetNeedWriteFlash( );
		}
		g_stDeviceState.m_nCodeState = ACK_SETUP_ALARM;
		break;
	case  CC_TE_RESET:						//16.	终端复位
		TE_SetNeedTEReset( );
		g_stDeviceState.m_nCodeState = ACK_SETUP_RESET;
		nFlag = 1;
		break;
	case  CC_TE_RESTORE_FACTORY:			//17.	恢愎出厂设置	
		InitializeAlarmFlash();
		InitializeGSMFlash();	
		TE_SetNeedWriteFlash();
		nFlag = 1;
		g_stDeviceState.m_nCodeState = ACK_RESTORE_FACTORY;
		break;	
	case CC_TE_FENCE:					    //18.   设置电子围栏 		
		nLen = 0;
		nFlag = 1;
		PL_TESetupFence( pSrc );
		TE_SetNeedWriteFlash();
		g_stDeviceState.m_nCodeState = ACK_SETUP_FENCE; 
		break;
	//  得到GG地址	
	case CC_TE_RESULT_GG_ADDR:	
		nLen = *pSrc++;
		nLen -= 1;
		// 语言模式 0:中文 1:英文
	    if( *pSrc++ == 0 ){
	    	GSM_SetLangState();
	    }
	    else{
	    	GSM_ClrLangState();
	    }	    
	    memcpy( pDest,pSrc, nLen );	
	    pDest[nLen] = 0x00;
	    TE_SetResultGGAddrFlag();
		break;
	}		
	
	if( nCodeState != CC_TE_RESULT_GG_ADDR ){
	
		if ( nLen == 0 ){
			if ( nFlag ){
				g_stDeviceState.m_nCodeState |= 0x80;
			}
			else{
				g_stDeviceState.m_nCodeState &= ~0x80;
			}
			if( nGSMState == 1){
				nLen = PL_ComposeGSMData( pDest ); 
			}
			else{
				nLen = PL_ComposeGPSData( pDest );
			}
			g_stDeviceState.m_nCodeState = 0;
		}
		else{
			nFlag = 0;
			nLen = 0;
		}
	}	
	return nLen;			     	
}
/////////////////////////////////////////////////////////////////////////////
//	设置电子围栏 
unsigned char	PL_TESetupFence( unsigned char	*pDataMsg ){

	unsigned char	nFence = 0;
	unsigned char	nCnt   = 0;
	unsigned char	nLen = 0;
	
	nLen = *pDataMsg++;
	nFence = *pDataMsg++;
	nCnt = nFence & 0x1F;
	if( nCnt > 2 ){
		nCnt = 0;
	}	 
	else{
		nCnt -= 1;
	}
	g_stAlarmSetup.m_oStGFence[nCnt].m_nGEOFence = 0;
	g_stAlarmSetup.m_oStGFence[nCnt].m_nFlag = nFence;	
	// 增加或删除
	if( (nFence & 0x40) == 0x00 ){
		g_stAlarmSetup.m_oStGFence[nCnt].m_RLat = 0;
		g_stAlarmSetup.m_oStGFence[nCnt].m_RLng = 0;
		g_stAlarmSetup.m_oStGFence[nCnt].m_LLat = 0;
		g_stAlarmSetup.m_oStGFence[nCnt].m_LLng = 0;
//		if( nFence & 0x80 ){
//			AL_DisableEnterGEOFence();
//		}
//		else{
//			AL_DisableLeaveGEOFence();
//		}
	}
//	else{
//		if( nFence & 0x80 ){
//			AL_EnableEnterGEOFence();
//		}
//		else{
//			AL_EnableLeaveGEOFence();
//		}
//	}
	else{
		// 电子围栏
		memcpy( &g_stAlarmSetup.m_oStGFence[nCnt].m_RLat, pDataMsg, 4);
		pDataMsg += 4;
		memcpy( &g_stAlarmSetup.m_oStGFence[nCnt].m_RLng, pDataMsg, 4);
		pDataMsg += 4;
		memcpy( &g_stAlarmSetup.m_oStGFence[nCnt].m_LLat, pDataMsg, 4);
		pDataMsg += 4;
		memcpy( &g_stAlarmSetup.m_oStGFence[nCnt].m_LLng, pDataMsg, 4);
	}	
	
	return 1;
}
/////////////////////////////////////////////////////////////////////////////
//	设置数据上传模式 	
unsigned char	PL_TESetupCBMode( unsigned char	*pDataMsg ){
	
	unsigned short  nTmp = 0;

	if ( *pDataMsg++ != 5 ){
		return 0;
	}				
	g_stGSMSetup.m_nCBMode = *pDataMsg++;
	pDataMsg += 2;	
	memcpy( &nTmp, pDataMsg, 2);					
	g_stGSMSetup.m_nCBSpace = nTmp;
	if ( g_stGSMSetup.m_nCBSpace <= 5 ){
		g_stGSMSetup.m_nCBSpace = 5;
	}
	
	return 1;
}
/////////////////////////////////////////////////////////////////////////////
//	设置各种号码	
unsigned char	PL_TESetupNumber( unsigned char	*pDataMsg ){
	
	unsigned char 	nResult = 1;
	unsigned char 	nLen = 0;
	unsigned char 	nSaveMode = 0;
	
	nLen = *pDataMsg++;	
	nSaveMode = *pDataMsg++;		
	
	nLen--;
	
	if( nLen > TELNUMBER_LENGTH ){
		nLen = TELNUMBER_LENGTH;
	}
		
	switch( nSaveMode ){
	case PHONE_MASTER:			//主控号码		
		memcpy( (char*)g_stGSMSetup.m_strMaterNo, (char*)pDataMsg, nLen );
		g_stGSMSetup.m_strMaterNo[nLen] = 0x00;
		break;
	case PHONE_AUTHORIZATION1:	//授权号码
		memcpy( (char*)g_stGSMSetup.m_strAuthNo1, (char*)pDataMsg, nLen );
		g_stGSMSetup.m_strAuthNo1[nLen] = 0x00;
		break;
	case PHONE_AUTHORIZATION2:  //授权号码
		memcpy( (char*)g_stGSMSetup.m_strAuthNo2, (char*)pDataMsg, nLen );
		g_stGSMSetup.m_strAuthNo2[nLen] = 0x00;
		break;
	case PHONE_AUTHORIZATION3:  //授权号码
		memcpy( (char*)g_stGSMSetup.m_strAuthNo3, (char*)pDataMsg, nLen );
		g_stGSMSetup.m_strAuthNo3[nLen] = 0x00;
		break;
	case PHONE_SMSADDR:					//地址服务中心号码
		memcpy( (char*)g_stGSMSetup.m_strSMSAddrNo, (char*)pDataMsg, nLen );
		g_stGSMSetup.m_strSMSAddrNo[nLen] = 0x00;
		break;
	case PHONE_SMC:				//SMC中心号码
		memcpy( (char*)g_stGSMSetup.m_strSMCNo, (char*)pDataMsg, nLen );
		g_stGSMSetup.m_strSMCNo[nLen] = 0x00;
		break;
	default:
		nResult = 0;
		break;
	}	
	return nResult;
}
/////////////////////////////////////////////////////////////////////////////
//	设置服务器   
unsigned char	PL_TESetupServer( unsigned char	*pDataMsg ){
	
	
	if ( *pDataMsg++ != 6 ){
		return 0;
	}
	memcpy( (char*)&g_stGSMSetup.m_nServerPort, (char*)pDataMsg, 2);
	pDataMsg += 2;	
	sprintf( (char*)g_stGSMSetup.m_strServerIP, "%03d.%03d.%03d.%03d",
				 		pDataMsg[0], 
				 		pDataMsg[1], 
				 		pDataMsg[2], 
				 		pDataMsg[3] );	
	IP_SetConnectNG( );		
	TE_ClrLoginSuccess();		 			 		
	return 1;
}
/////////////////////////////////////////////////////////////////////////////
//	报警设置	
unsigned char	PL_TESetupAlarm( unsigned char	*pDataMsg ){
	
	unsigned char 	nResult = 1;
	unsigned char 	nDataType = 0;
	unsigned char	nTmp = 0;
	unsigned int	iTmp = 0;
	
	// CC_PARALEN
	pDataMsg++;
	// CC_PARADATA TYPE
	nDataType = *pDataMsg++;
	switch( nDataType ){
	case SETUP_ALARM_SET: 		  	    									//报警功能设置
		memcpy( &iTmp, pDataMsg, 4 );
		g_stAlarmSetup.m_nAlarmEnable = iTmp;
		break;
	
	case SETUP_ALARM_OVERSPEED:
		nTmp = *pDataMsg;
		if ( nTmp < MIN_OVERSPEED ){
			AL_DisableOverSpeed( );
		}
		else{
			g_stAlarmSetup.m_nOverspeed = nTmp;
			AL_EnableOverSpeed();
		}
		break;
	
	case SETUP_ALARM_PARKING:
		memcpy( &iTmp, (char*)pDataMsg, sizeof(short));	
		if ( iTmp == 0 ){
			AL_DisableParking( );
		}
		else{			
			g_stAlarmSetup.m_nParking = (short)iTmp;
			AL_EnableParking();
		}
		break;
	// 非法开门报警	
	case SETUP_ALARM_ANTISTEAL:		
		if ( *pDataMsg == 0 ){
			AL_DisableNotDoor( );
		}
		else{
			g_stAlarmSetup.m_nILLDoorOpen = *pDataMsg;	
			AL_EnableNotDoor( );
		}
		break;
	// 疲劳驾驶设置	
	case SETUP_ALARM_TIREDNESS:
		if ( *pDataMsg == 0 ){
			AL_DisableTireDrive();
		}
		else{
			g_stAlarmSetup.m_nTireDriver = *pDataMsg;
			AL_EnableTireDrive();
		}
		break;
    //  圆形距离报警
	case SETUP_ALARM_DISTANCE:
		memcpy( &iTmp, (char*)pDataMsg, sizeof(short));	
		if( iTmp == 0 ){
			g_stCustomSetup.m_nDistance = 0;
			TE_DisableOutGeo();
		}
		else{
			if( GPS_IsPosValid() ){		//GPS定位有效
				if( nTmp < MINGEO_SETUP ){
					nTmp = MINGEO_SETUP;
				}			
				g_stCustomSetup.m_nCLng = GPS_GetLongitude();	//经度
				g_stCustomSetup.m_nCLat = GPS_GetLatitude();		//纬度
				g_stCustomSetup.m_nDistance = nTmp;
				TE_EnableOutGeo();
			}
		}
		break;
	// 取消报警
	case SETUP_ALARM_CANCEL:
		memcpy( (char*)&iTmp, (char*)pDataMsg, sizeof(int) );
		TE_ClearAlarmState( );
		GPRS_ClrAlarmSMSMode();
		break;	
	default:
		nResult = 0;
		break;		
	}
	return nResult;
}		    

#endif		//SW_GPRSPROTOCOLB

/////////////////////////////////////////////////////////////////////////////
//  增加序列号  取后8位
unsigned char  PL_AddTEUID( unsigned char	 *pBuf ){
	
	unsigned char 	nLen = 0;
	unsigned char	nPos = 0;
	
	nLen = strlen((char*)g_stGSMSetup.m_strUID);
	if ( nLen > DEUID_LENGTH ){
		nPos = nLen - DEUID_LENGTH;
		nLen = DEUID_LENGTH;
	}
	pBuf[0] = nLen;
	memcpy( &pBuf[1], &g_stGSMSetup.m_strUID[nPos], nLen);
	return nLen + 1; 
}
/////////////////////////////////////////////////////////////
//   检测校验和   
unsigned char PL_VerifyCheckSum(unsigned char *lpDataBuf, int nDataLen ){
		
	unsigned char 	nCnt = 0;	
	unsigned char 	nSumCheck = CHKSUM_ERROR;
		
	// CRC校验法
	#ifdef  SW_PROTOCOL_ASICO	
		unsigned char	nBit = 0;
		unsigned char	hHig = 0;
		unsigned char	hLow = 0;
		unsigned short	nCRC = 0xffff; 

		if( nDataLen < 6){
			return nSumCheck;
		}		
		// 同步码,在这里采用同步码是为了CUP的速度, 防止在短信模式下的数据		
		if( lpDataBuf[WHERE_MESSAGE_HEAD1] != SYN_SVEREHEADERCODE1 ||
			lpDataBuf[WHERE_MESSAGE_HEAD2] != SYN_SVEREHEADERCODE2 ){
			return nSumCheck;
		}
		nSumCheck = 0;
		// 得到数据长度		
		nDataLen = (lpDataBuf[WHERE_DATALEN_HIGHT]<<8) & 0xff00; 
		nDataLen = lpDataBuf[WHERE_DATALEN_LOW];
		// 校验码(2B)|结束码(2B)
		for( nCnt = 0; nCnt< nDataLen-4; nCnt++ ){
			for( nBit = 0; nBit < 8; nBit++){						
				nSumCheck = ((lpDataBuf[nCnt]<<nBit)&0x80) ^ ((nCRC&0x8000)>>8);			
				nCRC <<= 1;				
				if(nSumCheck != 0){  
					nCRC^=0x1021;
				}
			}
		}
		hHig = (nCRC >> 8)&0xff;
		hLow = nCRC & 0xff;
		if( (lpDataBuf[nCnt] == hHig) &&
			(lpDataBuf[nCnt+1] == hLow) ){ 
			nSumCheck = CHKSUM_OK;
		}
		else{
			nSumCheck = CHKSUM_ERROR;
		}		
	#else		
		nSumCheck = 0;
		for( nCnt = 0; nCnt < nDataLen; nCnt++){
			nSumCheck += lpDataBuf[nCnt];
		}
	#endif
	return nSumCheck;
}
//////////////////////////////////////////////////////////////////////////
//   计算校验和
#ifdef  SW_PROTOCOL_ASICO
unsigned short  PL_CountVerifySum(unsigned char *lpDataBuf, int nDataLen){
	
	unsigned char  nCnt = 0;
	unsigned char  nBit = 0;
	unsigned char  nFlag = 0;
	unsigned short nSumCheck = 0xffff;	
		
	for( nCnt = 0; nCnt< nDataLen; nCnt++ ){
		for( nBit = 0; nBit < 8; nBit++){						
			nFlag = ((lpDataBuf[nCnt]<<nBit)&0x80) ^ ((nSumCheck&0x8000)>>8);			
			nSumCheck <<= 1;				
			if(nFlag != 0){  
				nSumCheck^=0x1021;
			}
		}
	}
	// 校验和
	lpDataBuf[nCnt] = (nSumCheck >> 8) &0xff;
	lpDataBuf[nCnt+1] = nSumCheck & 0xff;
		
	return nSumCheck;
}
#else
unsigned char  PL_CountVerifySum(unsigned char *lpDataBuf, int nDataLen){
	
	unsigned char nSumCheck = 0;
	unsigned char nCnt = 0;
	
	for ( nCnt = 0; nCnt < nDataLen; nCnt++){
		
		nSumCheck -= lpDataBuf[nCnt];
	}
	return nSumCheck;
}
#endif  //SW_PROTOCOL_ASICO 

/////////////////////////////////////////////////////////////
//参数：
// 分析数据
// nCaller	:  接收号码
// pMsg	  	:  数据内容
// nTPUDL 	:  数据长度
// nDCSType :  DCS 

// 1: 返回>0表示有暗文协议数据发回							x
// 2: 返回0, 但pMsg有文本数据。表示有明文信息发回			x
// 3: 返回0,且pMsg为空。表示不需回答数据					x


// nCodeType = 0: 7位数据。文本明文数据
// nCodeType = 1: 8位编码发出。暗文协议数据
// nCodeType = 2: unicode编码。 明文数据
// 返回为0，不需发数据

unsigned char 	PL_ParseSMSData( unsigned char    *pDataBuf,
								 unsigned char     nDataLen,	
								 unsigned char	  *pCaller,    				                 
								 unsigned char     nDCS,
								 unsigned char	   nTPUDL,
								 unsigned char 	  *pCodeType ){
 
	unsigned char   nResultLen = 0; 			 	
	unsigned char		*pSrc 	   = NULL;
									 	
	if ( PL_VerifyCheckSum( pDataBuf, nDataLen ) != CHKSUM_OK ){

		pSrc = lpLargeBuf2;	
		
		if( IsAddrServerNo( pCaller ) || IsForwardNo(pCaller) ){
			if ( nDCS == DCS_7BIT){	 
				P_Decode7bitTo8Bit( pSrc, pDataBuf, nDataLen);						
				nResultLen = P_GSMToASCII( (char*)pDataBuf, (char*)pSrc, nTPUDL); 
				*pCodeType = 0;
			}
			else if ( nDCS == DCS_8BIT ){
				nResultLen = nDataLen;
				*pCodeType = 1;
			}
			else{
				nResultLen = nDataLen;
				*pCodeType = 2;			
				
			}			
		}
		else{		
			if ( nDCS == DCS_7BIT){	 
				P_Decode7bitTo8Bit( pSrc, pDataBuf, nDataLen);						
				P_GSMToASCII( (char*)pSrc, (char*)pSrc, nTPUDL); 
			}
			else if ( nDCS == DCS_8BIT ){
				memcpy(pSrc, pDataBuf, nDataLen);
				pSrc[ nDataLen ] = 0x00;
			}
			else if ( nDCS == DCS_UNICODE ){ 
				P_UnicodeToASCII( pSrc, pDataBuf,  nDataLen );
			}		
			else{
				memset( pSrc, 0, 1);
			}	
			nResultLen = PLS_ParseCommandData( pDataBuf, pSrc, pCaller, pCodeType );
		}	
		pSrc = NULL;										 	
	}
	else{  
		nResultLen = PL_ParseData( pDataBuf, pDataBuf, nDataLen );
		*pCodeType = 2;
	}					  
	return  nResultLen;							 
}
///////////////////////////////////////////////////////////////////
//	解释参数配置
//////////////////////////////////////////////////////////////////////////
//报警有效位(8),报警开关(8), 超速设置(2),疲劳驾驶(2),非法开门报警设置(2),
//停车报警设置(2),服务器IP(8), 服务器端口(4),工作模式(1),回传模式(3),
//主控号码(4~21),授权号码1(4~21), 授权号码2, (4~21) 授权号码3(4~21),
//APN(?),APNUSER,APNPSD
unsigned  char	PL_ParseConfig( unsigned char *pDest ,unsigned char	*pSrc ){ 

	
	unsigned char   nFlag = 0;
	unsigned char   bExit = 0;
	char			*pTmp = NULL; 
	unsigned char 	*pBuf = NULL;
	

	nFlag = 0; 
	bExit = 0;

	pBuf = (unsigned char*)strchr( (char*)pSrc, ',' );
	if( pBuf == NULL ){
		return 0;
	}
	pSrc = pBuf+1;	
	while( bExit == 0 ){
		pBuf = (unsigned char*)strchr( (char*)pSrc, ',' );
		if( pBuf != NULL ){
			*pBuf = 0;
		}
		else{			
			bExit = 1;
		}
		// 参数配置
		switch( nFlag ){
		// 报警开关
		case 0:
			g_stAlarmSetup.m_nAlarmEnable = strtol( (char*)pSrc, &pTmp, 16 );
		break;
		// 速度
		case 1:
			g_stAlarmSetup.m_nOverspeed = (unsigned char)strtol( (char*)pSrc, &pTmp, 16 );
			break;
		// 疲劳驾驶 	
		case 2:
			g_stAlarmSetup.m_nTireDriver = (unsigned char)strtol( (char*)pSrc, &pTmp, 16 );
			break;
		// 非法开门报警
		case 3:
			g_stAlarmSetup.m_nILLDoorOpen = (unsigned char)strtol( (char*)pSrc, &pTmp, 16 );
			break;
		// 停车报警设置(2)
		case 4:
			g_stAlarmSetup.m_nParking = (short)strtol( (char*)pSrc, &pTmp, 16 );
			break;
		// 服务器IP地址
		case 5:
			memset(g_stGSMSetup.m_strServerIP,0x00, SERVERIP_LENGTH+1 );
			strncpy((char*)g_stGSMSetup.m_strServerIP, (char*)pSrc, SERVERIP_LENGTH );
			break;
		//  服务器端口号
		case 6:
			g_stGSMSetup.m_nServerPort = (short)strtol( (char*)pSrc, &pTmp, 16 );
			break;
		// GPRS工作模式
		case 7:
			g_stGSMSetup.m_nCommMode = (short)strtol( (char*)pSrc, &pTmp, 16 );
			break;
		//  工作模式
		case 8:
			g_stGSMSetup.m_nCBMode = (unsigned char)strtol( (char*)pSrc, &pTmp, 16);
			break;
		// 回传模式
		case 9:
			g_stGSMSetup.m_nCBSpace = (short)strtol( (char*)pSrc, &pTmp, 16 );
			break;
		// 主控号码
		case 10:
			memset( g_stGSMSetup.m_strMaterNo, 0x00, TELNUMBER_LENGTH+1 );
			strncpy( (char*)g_stGSMSetup.m_strMaterNo, (char*)pSrc,  TELNUMBER_LENGTH );
			break;
		// 授权号码1
		case 11:
			memset( g_stGSMSetup.m_strAuthNo1, 0x00, TELNUMBER_LENGTH+1 );
			strncpy( (char*)g_stGSMSetup.m_strAuthNo1, (char*)pSrc,  TELNUMBER_LENGTH );
			break;
		// 授权号码2
		case 12:
			memset( g_stGSMSetup.m_strAuthNo2, 0x00, TELNUMBER_LENGTH+1 );
			strncpy( (char*)g_stGSMSetup.m_strAuthNo2, (char*)pSrc, TELNUMBER_LENGTH );
			break;
		// 授权号码3
		case 13:
			memset( g_stGSMSetup.m_strAuthNo3, 0x00, TELNUMBER_LENGTH+1 );
			strncpy( (char*)g_stGSMSetup.m_strAuthNo3, (char*)pSrc,  TELNUMBER_LENGTH );
			break;
		// APN
		case 14:
			memset( g_stGSMSetup.m_strGateway, 0x00, SERVERDOMAIN_LENGTH+1 );
			strncpy( (char*)g_stGSMSetup.m_strGateway, (char*)pSrc, SERVERDOMAIN_LENGTH);
			break;
		// User name
		case 15:
			memset( g_stGSMSetup.m_strUser, 0x00, TELNUMBER_LENGTH+1 );
			strncpy( (char*)g_stGSMSetup.m_strUser, (char*)pSrc, TELNUMBER_LENGTH);
			break;
		// user Psd	
		case 16:	
			memset( g_stGSMSetup.m_strPsd, 0x00, TELNUMBER_LENGTH+1 );
			strncpy( (char*)g_stGSMSetup.m_strPsd, (char*)pSrc, TELNUMBER_LENGTH);		
			break;
		// dnsr
		case 17:
			memset( g_stGSMSetup.m_strServerDomain, 0x00, SERVERDOMAIN_LENGTH+1 );
			strncpy( (char*)g_stGSMSetup.m_strServerDomain, (char*)pSrc, SERVERDOMAIN_LENGTH);
			break;
		}	
		nFlag++;	
		if( pBuf != NULL ){ 
			pSrc = pBuf+1; 
		}
	}
	return PL_ReadConfig( pDest );
}
///////////////////////////////////////////////////////////////////
//	 读取参数配置
//////////////////////////////////////////////////////////////////////////
//DEUID(20),报警有效位(8),报警开关(8), 超速设置(2),疲劳驾驶(2),非法开门报警设置(2),
//停车报警设置(2),服务器IP(8), 服务器端口(4),工作模式(1),回传模式(3),
//主控号码(4~21),授权号码1(4~21), 授权号码2, (4~21) 授权号码3(4~21),
//APN(?),APNUSER,APNPSD
unsigned char	PL_ReadConfig( unsigned char	*pDest ){

	unsigned char	nLen = 0;
	
	sprintf((char*)pDest, "%08X,%08X,%02X,%02X,%02X,%02X,%s,%04X,%d,%d,%03X,%s,%s,%s,%s,%s,%s,%s,%s,%s",								
								g_stAlarmSetup.m_nALValid,
								g_stAlarmSetup.m_nAlarmEnable,
								g_stAlarmSetup.m_nOverspeed,
								g_stAlarmSetup.m_nTireDriver,
								g_stAlarmSetup.m_nILLDoorOpen,
								g_stAlarmSetup.m_nParking,
								g_stGSMSetup.m_strServerIP,
								g_stGSMSetup.m_nServerPort,
								g_stGSMSetup.m_nCommMode,
								g_stGSMSetup.m_nCBMode,
								g_stGSMSetup.m_nCBSpace,
								g_stGSMSetup.m_strMaterNo,
								g_stGSMSetup.m_strAuthNo1,
								g_stGSMSetup.m_strAuthNo2,
								g_stGSMSetup.m_strAuthNo3,
								g_stGSMSetup.m_strGateway,
								g_stGSMSetup.m_strUser,
								g_stGSMSetup.m_strPsd,
								g_stGSMSetup.m_strUID,
								g_stGSMSetup.m_strServerDomain );
	
	nLen = strlen( (char*)pDest );								
	return nLen;
}


