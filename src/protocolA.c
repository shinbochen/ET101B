/////////////////////////////////////////////////////////////
//file name: TE_Terminal.c
//     Date: 2008-09-02
//	 Author: wyb	 
/////////////////////////////////////////////////////////////
// include file declare
#include    <stdlib.h>
#include    <stdio.h>
#include  <string.h>
#include  "..\hdr\protocolA.h"
#include  "..\hdr\protocolB.h"
#include  "..\hdr\APP_Terminal.h" 
#include  "..\hdr\APP_GPS.h"
#include  "..\hdr\HW_GPIO.h"
#include  "..\hdr\APP_GSM.h"
#include "..\hdr\APP_GSM.h"
#include "..\hdr\APP_EM200.h"
#include "..\hdr\APP_SIMCOM.h"
#include "..\hdr\APP_GSM.h"
#include "..\hdr\APP_GTM900.h"
#include  "..\hdr\APP_BQM26.h"


#if( SW_GPRSPROTOCOLA )

//////////////////////////////////////////////////////////////
//  获取报警状态
unsigned short	PL_GetAlarmState( void ){
	
	unsigned short	nResult  = 0;
	
	if ( AL_IsSOS() ){
		  nResult |= 0x01;
	}
	if ( AL_IsCustom1() ){
		  nResult |= 0x02;
	}
	if ( AL_IsCustom2() ){
		  nResult |= 0x04;
	}
	if ( AL_IsCustom3() ){
		  nResult |= 0x08;
	}
	if ( AL_IsCustom4() ){
		  nResult |= 0x10;
	} 
	if ( AL_IsPowerOff() ){
		nResult |= 0x20;
	}
	if ( AL_IsLowPower() ){
		nResult |= 0x40;
	}
	if ( AL_IsParking() ){
		nResult |= 0x80;
	}
	if ( AL_IsEnterGEOFence() ){
		nResult |= 0x100;
	}
	if ( AL_IsLeaveGEOFence() ){
		nResult |= 0x200;
	}
	if ( AL_IsNotIgnition() ){
		nResult |= 0x400;
	}
	if ( AL_IsNotDoor() ){
		nResult |= 0x800;
	}
	if ( AL_IsOverspeed() ){
		nResult |= 0x1000;
	}
	return nResult;
}
////////////////////////////////////////////////////////////
//
unsigned short	PL_GetCarState( void ){
	
	unsigned short nResult = 0;
	
	if ( HW_IsDoorState() ) {
		nResult |= 0x01;
	}
	if ( HW_IsAccState() ){ 
		nResult |= 0x04;
	}
	return nResult;
}
////////////////////////////////////////////////////////////
//
unsigned short	PL_GetTerminalState( void ){
	
	unsigned short	nResult = 0;
	
	//北纬度  "+%.6f,"
	if ( g_stGPSInfo.m_nLLFlag & N_LATITUDE){	
		nResult |= 0x20;  		
	}
	else{  //"-%.6f,"
		 nResult &= 0xDF;   
	}
	//东经度 "+%.6f"
	if (g_stGPSInfo.m_nLLFlag & E_LONGITUDE){	
		nResult |= 0x40;
	}
	else{	// "-%.6f"
		nResult &= 0xBF;
	}	
	nResult |= 0x80;
	//  定位标志
	if ( g_stGPSInfo.m_bPosValid){		
		 nResult &= ~0x10;
		 nResult |= 0x10;		
	}
	else{
		 nResult &= ~0x10;
	}
	return nResult;
}
////////////////////////////////////////////////////////////
// 登录报文
unsigned char	PL_ComposeLogin( unsigned char  *pDest ){ 
		
	unsigned char nLen = 0;
	unsigned char nTELen = 0;
	unsigned char nPos = 0;
	
	pDest[nLen++] = TE_LOGIN;	
	nTELen = strlen((char*)g_stGSMSetup.m_strUID);
	if ( nTELen > DEUID_LENGTH ){
		nPos = nTELen - DEUID_LENGTH;
		nTELen = DEUID_LENGTH;
	}
	pDest[nLen++] = nTELen+1;
	memcpy( &pDest[nLen], &g_stGSMSetup.m_strUID[nPos], nTELen);
	nLen +=	nTELen;
	pDest[nLen] = PL_CountVerifySum( pDest, nLen );	
	return nLen+1;
}
///////////////////////////////////////////////////////////////
// 
unsigned char	PL_ComposeHSData( unsigned char	*pDest ){

	unsigned char nLen = 0; 
	unsigned char nTELen = 0;
	unsigned char nPos = 0;
	
	pDest[nLen++] = TE_HANDSHAKE;	
	nTELen = strlen((char*)g_stGSMSetup.m_strUID);
	if ( nTELen > DEUID_LENGTH ){
		nPos = nTELen - DEUID_LENGTH;
		nTELen = DEUID_LENGTH;
	}
	pDest[nLen++] = nTELen+1;
	memcpy( &pDest[nLen], &g_stGSMSetup.m_strUID[nPos], nTELen);	
	nLen +=	nTELen;
	pDest[nLen] = PL_CountVerifySum( pDest, nLen );	
	return nLen+1;
}
////////////////////////////////////////////////////////////////
// 组织GPS数据
unsigned char PL_ComposegpsData(unsigned char *pDest){  
		
	unsigned char 		*pData = NULL;
	unsigned short 		nData  = 0;
	int			   		nTemp  = 0;
	
	pData  = pDest;
	*pData++ = 32;		//数据长度
	*pData++  = ((g_stGPSInfo.m_nYear % 10)*16) + g_stGPSInfo.m_nYear / 10;
	*pData++  = ((g_stGPSInfo.m_nMonth % 10)*16) + g_stGPSInfo.m_nMonth / 10;
	*pData++  = ((g_stGPSInfo.m_nDay % 10)*16) + g_stGPSInfo.m_nDay / 10;
	*pData++  = ((g_stGPSInfo.m_nHour % 10)*16) + g_stGPSInfo.m_nHour / 10;
	*pData++  = ((g_stGPSInfo.m_nMinute % 10)*16) + g_stGPSInfo.m_nMinute / 10;
	*pData++  = ((g_stGPSInfo.m_nSecond % 10)*16) + g_stGPSInfo.m_nSecond / 10;
	
	nTemp =  g_stGPSInfo.m_nLONGDegree*3600000;
	nTemp += g_stGPSInfo.m_nLONGMinute * 60000;
	nTemp += g_stGPSInfo.m_nLONGPrecision * 6;
	memcpy(pData, &nTemp, 4);
	pData += 4;
	
	nTemp = g_stGPSInfo.m_nLATIDegree*3600000;
	nTemp += g_stGPSInfo.m_nLATIMinute * 60000;
	nTemp += g_stGPSInfo.m_nLATIPrecision * 6;
	memcpy(pData, &nTemp, 4);
	pData += 4;
	memcpy(pData , 0x00, 4);		// 高度
	pData += 4;
	memcpy(pData,  &g_stGPSInfo.m_nCurDirection, 4);
	pData += 4;	
	nTemp = 0;
	nTemp = GPS_GetMilimetre();	
	memcpy(pData,  &nTemp, 4);
	pData += 4;	
	//  报警状态
	nData = PL_GetAlarmState( );
	memcpy(pData,  &nData, 2);
	pData += 2;
	//  车辆状态
	nData = PL_GetCarState( );
	memcpy(pData,  &nData, 2);
	pData += 2;	
	//  终端状态
	nData = PL_GetTerminalState( );
	memcpy(pData,  &nData, 2);
	pData += 2;
	
	nData = 33;	
	return nData;
}
//////////////////////////////////////////////////////////////
//  组织命令数据 32
unsigned char PL_ComposeCmdData( unsigned char *pDest, unsigned char nHead, unsigned char nCCode ){

	unsigned char nLen = 0;
	unsigned char nPos = 0;
	unsigned char nTELen = 0;   //UID长度
	
	pDest[nLen++] = nHead;
	pDest[nLen++] = nCCode;
		
	nTELen = strlen((char*)g_stGSMSetup.m_strUID);
	if ( nTELen > DEUID_LENGTH ){
		nPos = nTELen - DEUID_LENGTH;
		nTELen = DEUID_LENGTH;
	}
	pDest[nLen++] = nTELen+1;
	memcpy( &pDest[nLen], &g_stGSMSetup.m_strUID[nPos], nTELen);	
	nLen +=	nTELen+1;	
	nLen += PL_ComposegpsData( pDest+nLen );	
	pDest[nLen] = PL_CountVerifySum( pDest, nLen );	
	return nLen + 1;
}
/////////////////////////////////////////////////////////
//  组织上传GPS数据
unsigned char PL_ComposeGPSData( unsigned char  *pDest ){
		
	return PL_ComposeCmdData( pDest, TE_SUBMIT, CC_GPS_POSITION);
}
/////////////////////////////////////////////////////////////
//参数：phone
// 分析数据
// nDCS为原DCS
// | Type | control code | nsumlen | 8 deuid | para | chksum |
unsigned char 	PL_ParseData(unsigned char *pDest, unsigned char *pSrc, unsigned char nLen ){

	unsigned char			nResult = 0;
	unsigned char			nType = 0;
	
	nType = pSrc[0]; 
	
	if( nType == TE_DELIVERY ){
		nResult = PL_ParseDelivery( pDest, pSrc );
	}		
	else if( nType == TE_LOGIN_ACK ){
		TE_SetLoginSuccess( );	
		UDP_ClrHandShakeCnt( );
		TCP_ClrRCntionCnt();
		IP_ClrConnectFailedCnt( );
	}
	else if( nType == TE_ACK ){			
		UDP_ClrHandShakeCnt( );	
		TCP_ClrRCntionCnt();
		IP_ClrConnectFailedCnt( );
	}
	return nResult;  
}
////////////////////////////////////////////////////////////////////////////
//   发送终端数据信息  MT_DELIVERY
// | Type | control code | nsumlen | 8 deuid | para | chksum |
unsigned char	PL_ParseDelivery( unsigned char *pDest, unsigned char *pSrc ){								     	
	
	unsigned char	nFlag  = 1;
	unsigned char	nLen   = 0;
	unsigned short  nCnt   = 0;
	unsigned char 	nCCode = 0;
	unsigned char 	nCaller[TELNUMBER_LENGTH+1];	
	unsigned char 	*pTmp = NULL;
	
	nCCode = *( pSrc+1 );	
		
	switch( nCCode){		   
	case CC_GPS_POSITION: 	    // GPS定位信息 | CC_GPS_TIME
	case CC_HW_ALARMSTATE: 	    // 终端报警信息
	case CC_HW_CTRLDOOR: 				//
	case CC_GPS_CALLONCE: 	    // 呼叫一次		
		break;
		
	case CC_GPS_MESSAGETIMESPACE:	// 报文维护按时间		
	case CC_GPS_TCALLINTERVAL:  	// 定次呼叫按时间		
	case CC_GPS_TCALLDISTANCE:  	// 定次呼叫按距离
		
	case CC_GPS_CALLSTOP:				// 停止呼叫		
		g_stGSMSetup.m_nCBMode = CB_STOP;
		TE_SetNeedWriteFlash( );
		break;		
							
	case CC_GPS_ACALLINTERVAL:	  // 连续呼叫按时间 
		pSrc += 11;		
		g_stGSMSetup.m_nCBMode = CB_BYTIME;
		memcpy(&g_stGSMSetup.m_nCBSpace, pSrc, 2);
		if (g_stGSMSetup.m_nCBSpace < 5){
			  g_stGSMSetup.m_nCBSpace = 5;
		}	
		TE_SetNeedWriteFlash( );
		break;		
	
	case CC_GPS_ACALLDISTANCE: 		// 连续呼叫按距离			
		pSrc += 11;		
		g_stGSMSetup.m_nCBMode = CB_BYDISTANCE;
		memcpy(&g_stGSMSetup.m_nCBSpace, pSrc, 2);
		if (g_stGSMSetup.m_nCBSpace < 5){
			  g_stGSMSetup.m_nCBSpace = 5;
		}	
		TE_SetNeedWriteFlash( );
		break;			
		
	case CC_HW_CTRLOIL: 		    // 关闭/恢复油路		 		
		pSrc += 11;
		if( *pSrc == TE_CTRL_DISABLE ){
			HW_CTRLOIL_CUTOFF();
			HW_SetOilState();	
			GSM_SetOilState();
		}
		else{			
			HW_CTRLOIL_RESUME();
			HW_ClrOilState( );
			GSM_ClrOilState();
		}
		TE_SetNeedWriteFlash( );
		break;				
	case CC_HW_LISTEN: 						//  电话监听		
		pSrc += 3;		    			 	// Type | control code | sum| nTelLen |deuid(8)|
		nLen = *pSrc++;		 				// 号码长度
		pSrc += 8;			     			// DEUID	
		if ( nLen > TELNUMBER_LENGTH ){
				nLen = TELNUMBER_LENGTH;
		}
		memcpy(nCaller, pSrc, nLen);
		nCaller[nLen] = 0;
		CMM_DialNumber( nCaller );
		nFlag = 0;
		break;	
		
	case CC_SETUP_TELNUMBER: 			// 设置各种号码
		pSrc += 3;		    			 		// Type | control code | sum | nTelLen |nTelType |deuid(8)|
		nLen = *pSrc++;		 					// 号码长度
		nCnt = *pSrc++;
		pSrc += 8;			     				
		switch( nCnt ){
		case 0:		//主控号码
			pTmp = g_stGSMSetup.m_strMaterNo;
			break;
		case 1:		//地址服务号码
			pTmp = g_stGSMSetup.m_strSMSAddrNo;
			break;
		case 2:		//授权号码1
			pTmp = g_stGSMSetup.m_strAuthNo1;
			break;
		case 3:		//授权号码2
			pTmp = g_stGSMSetup.m_strAuthNo2;
			break;
		case 4:		//授机号码3
			pTmp = g_stGSMSetup.m_strAuthNo3;
			break;
		}	
		if ( nLen > TELNUMBER_LENGTH ){
				nLen = TELNUMBER_LENGTH;
		}
		memcpy(pTmp, pSrc, nLen);
		pTmp[nLen] = 0;
		TE_SetNeedWriteFlash( );
		break;	
		
	case CC_SETUP_SERVER: 				// 更改IP地址端口号
		pSrc += 11;								// Type | control code | sum | deuid(8)| port(2) | ip(4) |	
		memcpy(&g_stGSMSetup.m_nServerPort, pSrc, 2);
		pSrc += 2;
		sprintf((char*)g_stGSMSetup.m_strServerIP, "%03d.%03d.%03d.%03d", pSrc[0],pSrc[1], pSrc[2],pSrc[3]);
		IP_SetConnectNG( );
		TE_ClrLoginSuccess();
		TE_SetNeedWriteFlash( );
		break;	
		
	case CC_TE_RESET: 						// 终端复位
		TE_SetNeedTEReset( );
		break;	
	case CC_SETUP_SPEEDALARM: 		// 设置超速报警				
		pSrc += 11;
		nCnt = *pSrc ;
		if ( nCnt < MIN_OVERSPEED ){
			AL_DisableOverSpeed( );
		}
		else{			
			g_stAlarmSetup.m_nOverspeed = nCnt;	
			AL_EnableOverSpeed();
		}					
		TE_SetNeedWriteFlash( );
		break;	
		
	case CC_SETUP_PARKALARM: 			// 设置停车报警
		pSrc += 11;
		memcpy(&g_stAlarmSetup.m_nParking , pSrc, 1);		
		TE_SetNeedWriteFlash( );
		break;	
		
	case CC_SETUP_CANCELALARM: 			// 取消报警
	case CC_STOP_ALARM: 				// 取消全部报警
		TE_ClearAlarmState();
		GPRS_ClrAlarmSMSMode();
		break;	
		
	case CC_GPS_RESTORE_FACTORY:		// 恢复出厂设置
		InitializeGSMFlash();
		InitializeAlarmFlash();
		TE_SetNeedWriteFlash( );	
		break;
		
	default:
		nFlag = 0;
		break;
	}	
	nLen = 0;
	if ( nFlag == 1 ){
		nLen = PL_ComposeCmdData( pDest, TE_ACK, nCCode );
	}
	return nLen;			     	
}

#endif //SW_GPRSPROTOCOLA
