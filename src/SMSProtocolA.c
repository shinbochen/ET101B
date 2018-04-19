///////////////////////////////////////////////////////////////////////////
//   file name: SMSProtocolData1.h
// description: SMS协议头文件定义\old default function
//      author:
//        date: 2009-02-24
//     reamark: 
#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>
#include  "..\hdr\APP_BQM26.h"
#include "..\hdr\APP_GTM900.h"
#include  "..\hdr\APP_GSM.h"
#include "..\hdr\APP_EM200.h"
#include "..\hdr\APP_SIMCOM.h"
#include  "..\hdr\APP_Terminal.h"
#include  "..\hdr\SMSProtocolA.h"
#include  "..\hdr\SMSProtocolB.h"
#include  "..\hdr\APP_GPS.h"

#if( SW_SMSPROTOCOLA )
char*   g_strSetCmd[]={
   "restart", 		// 重启复位
   "default",		// 恢复出厂设置
   "sn",			// 序号	
   "inf",			// 状态
   "version",		// 读取版本号
   "w",				// 定位
   
   "enable",  		// 开启报警	
   "disable", 		// 关闭报警
   
   "per",			// 跟踪间隔
   "parking",		// 停车报警	
   "open",			// 非法开门报警时间
   "overspeed",		// 超速报警		
   "stop",			// 断油	
   "re",				// 恢复油路
   "l",					// 监听
   "sp1",				// 设置号码
   "sp2",				// 设置号码
   "sp3",				// 设置号码	  
   "h",					// 设置主控号码
   "csca",			// 设置中心号码					   
   "p",					// 更改密码
   
   "t",					// 开启跟踪					   
   "td",				// 关闭跟踪
   
   "up",      	// 设置用户名和密码
   "apn",				// 设置APN网关 
   "dnsr",    	// 域名服务器
   
   "towe",    	// 开启防盗功能 
   "towd",		// 关闭防盗
   
   "time",   	// 自动布防功能
   "gprs",		// GPRS传输
   "ip",		// 服务IP地址设置
   "gp", 		// 工作模式	( 1:sms 2:gprs )
   "forward",	// 查询话费内容 *Forward=10086:CXCY#
   "excute",	// 查询手机话费 *EXCUTE=*888##
   "IMEI",		// IMEI
   "poweroff",
   "lowpower",
   "setup",
   "dnsr",
   "aslis",			//0:配置为门检输入报警 1:配置为原车防盗输入 ASLIS
   "reinput",
};

/////////////////////////////////////////////////////////////
// 获取数据类型 
unsigned char PLS_GetCommandType( unsigned char  *pBuf ){

	unsigned char 	nCnt 	  = 0;
	unsigned char	*pTmp 	  = NULL;
	unsigned char	*pKeyword = NULL;
	
	// pTmp指向数据
	// pKeyword指向关键词	
	
	if( *pBuf != '*' ){
		return SMS_CMD_END;		
	}
	
	pTmp = (unsigned char*)strchr( (char*)pBuf+1, '*' );
	if( pTmp == NULL ){
		pTmp = (unsigned char*)strchr( (char*)pBuf+1, ':' );
		if( pTmp ){
			*pTmp = 0x00;
			 pTmp++;
		}
		else{
			pTmp = (unsigned char*)strchr( (char*)pBuf+1, '#' );
			if( pTmp ){
				*pTmp = 0x00;				
			}						
		}
	}
	else{
		*pTmp = 0x00;
		pTmp++;		
	}	 	
	pKeyword = pBuf+1; 
	
	// 命令码比较 	
	for( nCnt = 0; nCnt < SMS_CMD_END; nCnt++ ){
	 	if( strcmpnc((unsigned char*)g_strSetCmd[nCnt], pKeyword ) ){
			break;
		}	 	
	}
	if( nCnt < SMS_CMD_END ){		
		strcpy( (char*)pBuf, (char*)pTmp );	
		pTmp = (unsigned char*)strrchr( (char*)pBuf, '#' );		
		if( pTmp ){		
			*pTmp = 0x00;
		}
	}
	return nCnt;	
}				
////////////////////////////////////////////////////////////////////
//
unsigned char	PLS_ParseCommandData( unsigned char  *pDest, 
																	  unsigned char  *pSrc,
																	  unsigned char  *pCaller,
																	  unsigned char  *pCodeType ){
													  	
	unsigned char	nCmdType  = 0;	
	unsigned char	nTmp = 0;
	unsigned char nResult = 0;
	unsigned char nLangCN;
	
	// 适用蒙苦第一个字母子
	if( *pSrc == ' ' ){	
		pSrc++;
	}
	
	nCmdType = PLS_GetCommandType( pSrc ); 
	*pCodeType = 0;	
	nLangCN = 0;
	
	if( !IsAuthorizationNo(pCaller) ){
		if( nCmdType == SMS_SETUP_HOME ||
		  	nCmdType == SMS_CONTROL_LISTEN||
		  	nCmdType == SMS_VERSION ||
		  	nCmdType == SMS_SERIAL_SN ||
		  	nCmdType == SMS_SETUP){ 
		  	//continue	
		}
		else if( nCmdType < SMS_CMD_END){
			strcpy( (char*)pDest, (char*)TEXT_NOT_AUTHORIZED );		
			return strlen((char*)pDest);			
		}
		else{
			*pDest = 0x00;
			return 0;
		}
	}
	else{
		if( nCmdType >= SMS_CMD_END ){			
			strcpy( (char*)pDest, (char*)TEXT_CMD_ERROR );
			return strlen((char*)pDest);
		}
		else if( nCmdType == SMS_DEFAULT   ||
				 nCmdType == SMS_SETUP_SP1 ||
				 nCmdType == SMS_SETUP_SP2 ||
				 nCmdType == SMS_SETUP_SP3 ||
				 nCmdType == SMS_SETUP_PSD ){			
			if( !IsMasterNo(pCaller) ){
				strcpy( (char*)pDest, (char*)TEXT_NOT_AUTHORIZED );	
				return strlen((char*)pDest);
			}			
		}
	}	
	switch( nCmdType ){	
	case SMS_RESTART:
		TE_SetNeedTEReset(  );
		sprintf( (char*)pDest, "%s %s", g_strSetCmd[nCmdType], TEXT_SUCCESS );	
		nResult = strlen( (char*)pDest );
		break;					   
		
	case SMS_DEFAULT:
		TE_SetNeedWriteFlash();
		InitializeAlarmFlash();
		InitializeGSMFlash();
		sprintf( (char*)pDest, "%s %s", g_strSetCmd[nCmdType], TEXT_SUCCESS );	
		nResult = strlen( (char*)pDest );
		break;					   
		
	case SMS_SERIAL_SN:		
		sprintf( (char*)pDest, "%s:%s",TEXT_SN,g_stGSMSetup.m_strUID );		
		nResult = strlen( (char*)pDest );
		break;
		
	case SMS_QUERY_INF:
		nResult = PLS_ReadState( pDest, nLangCN );
		break;					  
	case SMS_VERSION:	
		sprintf( (char*)pDest, "%s:%s",	TEXT_VERSION, g_strVer );	
		nResult = strlen( (char*)pDest );	
		break;					
		   
	case SMS_POSITION:
		nResult = PLS_ComposePosition( pDest, nLangCN );
		sprintf( (char*)(pDest+nResult), "%s?lng=%.6f&lat=%.6f&key=%d",   TEXT_GOGPS,
															     		  (double)GPS_GetLongitude()/3600000,
															     		  (double)GPS_GetLatitude()/3600000,
															     		  GPS_CalculateData() );
		nResult = strlen( (char*)pDest );	 
		break;					   
		
	case SMS_ALARM_ENABLE:	// 开启报警
		nResult = PLS_SetupAlarmEnable( pDest, 1, nLangCN );			
		break;				   
	case SMS_ALARM_DISABLE:	// 关闭报警
		nResult = PLS_SetupAlarmEnable( pDest, 0, nLangCN ); 	
		break;				   
		
	case SMS_PER_SPACE:			// 跟踪间隔
		TE_SetNeedWriteFlash();
		g_stGSMSetup.m_nCBMode = CB_BYTIME;
		g_stGSMSetup.m_nCBSpace = atoi((char*)pSrc);
		if ( g_stGSMSetup.m_nCBSpace < 30  ){
			g_stGSMSetup.m_nCBSpace = 30;
		}
		sprintf( (char*)pDest, "%s %s %d(s)", TEXT_AUTOTRACK, TEXT_EVERY, g_stGSMSetup.m_nCBSpace );
		nResult = strlen( (char*)pDest );
		break;					   
	case SMS_SETUP_PARKING:	// 停车报警
		nResult = PLS_SetupAlarmParking( pDest, pSrc, nLangCN );
		break;				  
	case SMS_DOOR_TIME:  		// 车门报警时间
		nResult = PLS_SetupAlarmAntisteal( pDest, pSrc, nLangCN );
		break;				    
	case SMS_OVERSPEED:			// 超速报警
		nResult = PLS_SetupAlarmOverspeed( pDest, pSrc, nLangCN );
		break;					
	case SMS_TOW_ENABLE:
		nResult = PLS_SetupAlarmTow( pDest, (unsigned char*)"1", nLangCN );
		break;					   
	case SMS_TOW_DISABLE:
		nResult = PLS_SetupAlarmTow( pDest, (unsigned char*)"0", nLangCN );
		break;				  	
	case SMS_CLOSE_OIL:			// 断油
		nResult =PLS_CtrlOil( pDest, 0, nLangCN);		
		break;					   	
	case SMS_RESUME_OIL:			// 恢复油路
		nResult =PLS_CtrlOil( pDest, 1, nLangCN ); 		
		break;					 
		  	
	case SMS_CONTROL_LISTEN:
		pDest[0] = 0;
		nResult = PLS_CarListen( pDest, pSrc, nLangCN );
		break;	
		
	case SMS_SETUP_SP1:
		nResult = PLS_SetupSlaveNo( pDest, pSrc, 1, nLangCN );
		break;					  
	case SMS_SETUP_SP2:
		nResult = PLS_SetupSlaveNo( pDest, pSrc, 2, nLangCN );
		break;					  
	case SMS_SETUP_SP3:
		nResult = PLS_SetupSlaveNo( pDest, pSrc, 3, nLangCN );
		break;					
		
	case SMS_SETUP_HOME:
		nResult = PLS_SetupMasterNo( pDest, pSrc, nLangCN );
		break;			
	case SMS_SETUP_CSCA:
		nResult = PLS_SetupSMCNo( pDest, pSrc, nLangCN );
		break;			
	case SMS_SETUP_PSD:
		nResult = PLS_SetupPassword( pDest, pSrc, '*', nLangCN );
		break;	
		
	case SMS_SETUP_IP:	   
	case SMS_SETUP_DNSR:
		nResult = PLS_SetupServer( pDest, pSrc, ';', nLangCN  );
		break;			   	
	case SMS_SETUP_APN:
		nResult = PLS_SetupAPN( pDest, pSrc, nLangCN  );
		break;			
	case SMS_NET_SETUP:		// 设置用户名和密码
		nResult = PLS_SetupAPNUserPsd( pDest, pSrc, ',', nLangCN); 
		break;					 		
		
	case SMS_OPEN_TRACK:		
		TE_SetNeedWriteFlash();
		g_stGSMSetup.m_nCBMode = CB_BYTIME;
		sprintf( (char*)pDest, "%s %s %d(s)", TEXT_AUTOTRACK, TEXT_EVERY, g_stGSMSetup.m_nCBSpace );
		nResult = strlen((char*)pDest);
		break;					  
	case SMS_CLOSE_TRACK:
		TE_SetNeedWriteFlash();
		g_stGSMSetup.m_nCBMode = CB_STOP;
		sprintf( (char*)pDest, "%s %s", TEXT_AUTOTRACK, TEXT_DISABLE );	
		nResult = strlen((char*)pDest);	
		break;
						   
	case SMS_SETUP_GP:
		nResult = PLS_SetupWorkMode( pDest, pSrc, nLangCN );
		break;		   			
		
	case SMS_AUTO_TIME:
		nResult = PLS_SetupDefensive( pDest, pSrc, nLangCN );
		break;				    
	case SMS_GPRS:
		if( strcmpnc( pSrc, (unsigned char*)"TCP") ){
			nResult = PLS_SetupWorkMode( pDest, (unsigned char*)"2A", nLangCN );			
		}
		else{
			nResult = PLS_SetupWorkMode( pDest, (unsigned char*)"2B", nLangCN );				
		} 
		break;			
	case SMS_FORWARD:
		nResult = PLS_ForwardMsg( pCaller, pDest, pSrc, nLangCN );
		break;					
	case SMS_EXCUTE:
		nResult = PLS_ExecuteExtCmd( pDest, pSrc, nLangCN );
		break;		
	case SMS_IMEI:
		strcpy( (char*)pDest, "IMEI: ");
		nTmp = strlen((char*)pDest);
		CMM_QueryIMEI( pDest+nTmp );
		nResult = strlen((char*)pDest);
		break;
	case SMS_POWEROFF:	//开启域关闭电源
		sprintf((char*)pDest, "%s %s ",TEXT_POWEROFF , TEXT_ALARM );
		if( atoi((char*)pSrc) == 0 ){ //关闭
			AL_DisablePowerOff();
			strcat((char*)pDest, TEXT_DISABLE );
		}
		else{						  //开启	
			AL_EnablePowerOff();
			strcat((char*)pDest, TEXT_ENABLE );
		}
		TE_SetNeedWriteFlash();	
		nResult = strlen((char*)pDest);
		break;
	case SMS_LOWPOWER:  //开启域关闭低电源 	
		sprintf((char*)pDest, "%s %s ",TEXT_LOW_POWER , TEXT_ALARM );
		if( atoi((char*)pSrc) == 0 ){ //关闭
			AL_DisableLowPower();
			strcat((char*)pDest, TEXT_DISABLE );
		}
		else{						  //开启	
			AL_EnableLowPower();
			strcat((char*)pDest, TEXT_ENABLE );
		}		
		TE_SetNeedWriteFlash();	
		nResult = strlen((char*)pDest);
		break;
	case SMS_SETUP:		//设置设备的参数	
		PLS_SetupDeviceInfo( pDest, pSrc );
		TE_SetNeedWriteFlash();
		break;	
	case  SMS_SERVER_DNSR:	//设置域名
		nResult = PLS_SetupServerDNSR( pDest, pSrc, nLangCN );
		break;
	case SMS_ASLIS_SETUP: 
		if( atoi((char*)pSrc) == 0){ //配置门检输入报警
			TE_ClrAntiSteal();
			TE_SetASILTrigger();	 //负触发 	
			// 100秒,开ACC
			g_stAlarmSetup.m_nILLDoorOpen = 100;   
			// antisteal alarm input: door
			if( nLangCN ){
				sprintf((char*)pDest, "%s%s%s", TEXT_ANTISTEAL_CN, TEXT_ALARM_CN,TEXT_INPUT_CN);
				nTmp = strlen((char*)pDest);
				nTmp += unicodecpy( pDest+nTmp, (unsigned char*)":" );
				strcpy( (char*)pDest+nTmp, TEXT_DOOR_CN );
				nTmp +=strlen(TEXT_DOOR_CN);
				nResult = nTmp;
			}
			else{
				sprintf((char*)pDest, "%s %s %s:%s", TEXT_ANTISTEAL, TEXT_ALARM, TEXT_INPUT,TEXT_DOOR);
				nResult = strlen( (char*)pDest );
			}
		}
		else{						 //配置原车防盗输入				
			TE_SetAntiSteal();			
			TE_SetASIHTrigger();	 //正触发
			// 触发报警5秒
			g_stAlarmSetup.m_nILLDoorOpen = 5;
			// antisteal alarm input: external alarm system 原车防盗
			if( nLangCN ){
				sprintf((char*)pDest, "%s%s%s", TEXT_ANTISTEAL_CN, TEXT_ALARM_CN,TEXT_INPUT_CN);
				nTmp = strlen((char*)pDest);
				nTmp += unicodecpy( pDest+nTmp, (unsigned char*)":" ); 
				strcpy( (char*)pDest+nTmp, TEXT_ENTAL_SYSTEM_CN );
				nTmp +=strlen(TEXT_ENTAL_SYSTEM_CN);
				nResult = nTmp;				
			}
			else{
				sprintf((char*)pDest, "%s %s %s:%s", TEXT_ANTISTEAL, TEXT_ALARM,TEXT_INPUT,TEXT_ENTAL_SYSTEM );
				nResult = strlen((char*)pDest);
			}
		}
		TE_SetNeedWriteFlash();
		break;		
	case SMS_REINPUT:	//设置自定义高低电平	
		nResult = PLS_SetReInput( pDest, pSrc, TEXT_COLON, nLangCN );
		TE_SetNeedWriteFlash( );			
		break;
	default:
		break;	
	}
	return 	nResult;												  	
}				

/////////////////////////////////////////////////////////////////////////////
// 电话监听
unsigned char PLS_CarListen( unsigned char  *pDest,	unsigned char  *pSrc, unsigned char nLangCN ){

	unsigned char		*pTmp = NULL;
			
	pTmp = (unsigned char*)strchr( (char*)pSrc, '*');	
	
	if ( pTmp == NULL ){
		if( nLangCN ){			
			strcpy( (char*)pDest, TEXT_PARA_ERROR_CN );		
		}
		else{
			strcpy( (char*)pDest, TEXT_PARA_ERROR );				
		}
	}
	else{ 
		*pTmp = 0x00;
		pTmp++;
		if( strcmp((char*)pSrc, (char*)g_stGSMSetup.m_strPassword) == 0x00 ){
			
			CMM_DialNumber( pTmp );	
		}
		else{
			if( nLangCN ){			
				strcpy( (char*)pDest, TEXT_PSD_ERROR_CN );		
			}
			else{
				strcpy( (char*)pDest, TEXT_PSD_ERROR );				
			}	
		}				
	}
	return strlen((char*)pDest);
} 

/////////////////////////////////////////////////////////////////////////////
//	设置主控号码  *h*1234*13480924411#
unsigned char PLS_SetupMasterNo( unsigned char  *pDest, unsigned char  *pSrc, unsigned char nLangCN ){

	unsigned char  nLen = 0;
	unsigned char  *pTmp = NULL;
	unsigned char	 nResult = 0;
		
	pTmp = (unsigned char*)strchr( (char*)pSrc, '*' );
	if ( pTmp == NULL ){
		if( nLangCN){			
			strcpy( (char*)pDest, TEXT_PARA_ERROR_CN );		
		}
		else{
			strcpy( (char*)pDest, TEXT_PARA_ERROR );				
		}
		nResult = strlen( (char*)pDest );
	}
	else{
		*pTmp = 0x00;	
		pTmp++;	
		if( strcmp((char*)pSrc, (char*)g_stGSMSetup.m_strPassword) == 0x00 ){
			
			nLen = strlen( (char*)pTmp );
			if( nLen >= TELNUMBER_LENGTH ){
				nLen = TELNUMBER_LENGTH;
			}
			strncpy( (char*)g_stGSMSetup.m_strMaterNo, (char*)pTmp, nLen );
			g_stGSMSetup.m_strMaterNo[nLen] = 0x00;
						
			if( nLangCN ){		
				sprintf( (char*)pDest, "%s%s", TEXT_MASTER_CN, TEXT_NO_CN );	
				nResult = strlen( (char*)pDest );
				nResult += unicodecpy( pDest+nResult, (unsigned char*)":" );
				nResult += unicodecpy( pDest+nResult, g_stGSMSetup.m_strMaterNo );
			}
			else{
				sprintf( (char*)pDest, "%s %s:%s", TEXT_MASTER, TEXT_NO, g_stGSMSetup.m_strMaterNo );					
				nResult = strlen( (char*)pDest );
			}				
			TE_SetNeedWriteFlash( );	
		}
		else{
			if( nLangCN ){			
				strcpy( (char*)pDest, TEXT_PSD_ERROR_CN );		
			}
			else{
				strcpy( (char*)pDest, TEXT_PSD_ERROR );				
			}		
			nResult = strlen( (char*)pDest );			
		}				
	}
	return nResult;
}

#endif	 	//SW_SMSPROTOCOLA


