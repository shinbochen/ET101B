///////////////////////////////////////////////////////////////////////////
//   file name: SMSProtocol.h
// description: SMS协议头文件定义\new default function
//      author:
//        date: 2009-02-23
//     reamark: 协议B
#include  <stdio.h> 
#include  <stdlib.h>
#include  <string.h>
#include  <ctype.h>
#include "..\hdr\includes.h"
#include  "..\hdr\APP_Terminal.h"
#include  "..\hdr\APP_GSM.h"
#include  "..\hdr\Main.h"
#include  "..\hdr\HW_GPIO.h"
#include  "..\hdr\APP_GPS.h"
#include  "..\hdr\APP_GTM900.h"
#include  "..\hdr\APP_BQM26.h"
#include  "..\hdr\APP_EM200.h"
#include  "..\hdr\APP_MC323.h"
#include  "..\hdr\APP_MG323.h"
#include "..\hdr\APP_SIMCOM.h"
#include  "..\hdr\APP_LoopDetect.h"
#include  "..\hdr\SMSProtocolB.h"
#include  "..\hdr\ProtocolB.h"

//#define			LANG_CN			0x01					  
//unsigned char		g_PBFlag = 0;

#if( SW_SMSPROTOCOLB )
						 
const char*	g_strAbbrCmd[]={
						"rst",			//重启复位
						"dft",			//恢复出厂设置
						"sn",			//设置序号
						"sts",		//终端机器状态
						"ver",			//读取版本号
						"pos",			//定位
						"pos2",
						"pos3",			//定位3
						"pos6",			//查询地址
						"alm",			//报警允许/禁止
						"trk",			//自动上传
						"pkg",			//停车报警设置
						"asl",			//非法开门报警设置	
						"ais",			//0:配置为门检输入报警 1:配置为原车防盗输入
											
				  	"ovd",			//超速报警设置
				  	"fuel",			//油路控制
				  	"lsn",			//电话监听
				  	"Talk",			//车载通话
				  	"slv1",			//设置号码
				  	"slv2",			//设置号码
				  	"slv3",			//设置号码
				  	"slv4",			//设置号码
				  	"slv5",			//设置号码
				  	"msr",			//设置主控号码
				  	"smc",			//设置短信中心号码
				  	"psd",			//更改密码
				  	"svr",			//设置IP地址
				  	
				  	"apn",			//设置APN网关 
				  	"net",			//设置上网用户名和密码
				  	"wrk",			//工作模式
				  	"fwd",			//查询话费内容 *FWD=10086:CXCY#
				  	"exe",			//查询手机话费 *EXE=*888##
				  	"tow",			//拖车报警
				  	"dfs",
				  	"IMEI",			//查询IMEI
				  	"Lock",			//中控锁控制
				  	"prf",		
					"lpr",
					"setup",		//APN多参数设置
				  	
					"igeo",
					"ogeo",
					"nc",
					"rip",
					"dnsr",			//设置域名	
					
					"handshake",    //设置握手信号		
					"accalarm",     //设置ACC报警	
					"accidle",		//ACC空闲设置	
					"nogpsalarm",	//GPS未定位报警	
					"dataformat",	//设置GPS数据回传格式	
					
					"MR",			//设置里程		
					"TM", 			//查询总里程	
					"CUT",
					"ON",
					"SOS",			//紧急
					"autofuel",		//门检报警断油
					"crash"			//撞车报警开关
					};
const char*   g_strSetCmd[]={
					  "restart",		//重启复位					  
					  "default",		//恢复出厂设置					  
					  "sn",				//序号
					  "status",			//终端机器状态
					  "version",		//读取版本号					  
					  "position",		//定位					  
					  "position2",		//定位2
					  "position3",		//定位3
					  "position6",		//查询地址
					  "alarm",			//报警允许/禁止	
					  "track",			//自动上传					  
					  "parking",		//停车报警设置					  
					  "antisteal",		//非法开门报警设置	
					  "aslis",			//0:配置为门检输入报警 1:配置为原车防盗输入 ASLIS
					  
					  "overspeed",		//超速报警设置					  
					  "fuel",			//油路控制
					  "listen",			//电话监听
					  "Talk",			//车载通话
					  "slave1",			//设置号码
					  "slave2",			//设置号码 aslis
					  "slave3",			//设置号码
					  "slave4",			//设置号码
					  "slave5",			//设置号码
					  "master",			//设置主控号码
					  "smc",				//设置短信中心号码
					  "password",		//更改密码
					  "server",			//设置IP地址					  
					  
					  "apn",			//设置APN网关
					  "net",			//设置上网用户名和密码
					  "work",			//工作模式
					  "forward",		//查询话费内容 *Forward=10086:CXCY#
					  "execute",		//查询手机话费	*EXCUTE=*888##
					  "tow",			//拖车报警
					  "Defensive",
					  "IMEI",			//查询IMEI
					  "Lock",			//中控锁控制
					  "poweroff",
					  "lowpower",
					  "setup",			//APN多参数设置
					  
					  "ingeo",
					  "outgeo",
					  "namec",
					  "reinput",
					  "dnsr",			//设置域名	
					  
					  "handshake",    	//设置握手信号  	
					  "accalarm",   	//设置ACC报警
					  "accidle",		//ACC空闲设置	
					  "nogpsalarm",		//GPS未定位报警
					  "dataformat",		//设置GPS数据回传格式
					  
					  "MileageRemark",	//设置里程
					  "TotalMileage", //查询总里程
					  "CUT",
					  "ON",
					  "SOS",			//紧急
					  "autofuel",		//门检报警断油
					  "crash"			//撞车报警开关
					  };
const char*   g_strSetCmdCN[]={
					  "cq",			//重启复位					  
					  "cc",			//恢复出厂设置					  
					  "sn",			//序号
					  "zt",			//终端机器状态
					  "version",	//读取版本号					  
					  "dw",			//定位
					  "dw2",		//定位2	
					  "dw3",		//定位3	 
					  "dw6",		//查询地址 
					  "bj",			//报警允许/禁止	
					  "zz",			//自动上传					  
					  "tc",			//停车报警设置					  
					  "fd",			//非法开门报警设置	
					  "fdsz",		//0:配置为门检输入报警 1:配置为原车防盗输入
					  
					  "cs",			//超速报警设置					  
					  "dy",			//油路控制
					  "jt",			//电话监听
					  "th",			//车载通话
					  "sq1",		//设置号码
					  "sq2",		//设置号码
					  "sq3",		//设置号码
					  "sq4",		//设置号码
					  "sq5",		//设置号码
					  "zk",			//设置主控号码
					  "smc",		//设置短信中心号码
					  "mm",			//更改密码
					  "server",	    //设置IP地址
					  
					  "apn",		//设置APN网关
					  "net",		//设置上网用户名和密码
					  "work",		//工作模式
					  "forward",	//转发 例如查询话费内容 *Forward=10086:CXCY#
					  "excute",		//扩展指令 例如查询手机话费	*EXCUTE=*888##
					  "yd",			//拖车报警
					  "sf",
					  "imei",		//查询IMEI
					  "sm",			//中控锁控制
					  "dd",
					  "qy",
					  "setup",		//APN多参数设置
					  
					  "ingeo",
					  "CJ",			//出界报警
					  "namec",
					  "reinput",
					  "dnsr",		//设置域名		
					  			  
					  "handshake",  //设置握手信号
					  "accalarm",   //设置ACC报警
					  "accidle",	//ACC空闲设置	
					  "nogpsalarm",	//GPS未定位报警
					  "dataformat",	//设置GPS数据回传格式
					  
					  "MR",			//设置里程	
					  "TM", 		//查询总里程
					  "CUT",
					  "ON",
					  "SOS",		//紧急
					  "autofuel",	//门检报警断油
					  "ZC"			//撞车报警开关
					  };					  

/////////////////////////////////////////////////////////////
// 
unsigned char PLS_GetCommandType( unsigned char  *pBuf ){

	unsigned char   nCnt 	  = 0;
	unsigned char   *pTmp 	  = NULL;
	unsigned char	*pKeyword = NULL;
		
	GSM_ClrLangState();
	// pTmp指向数据
	// pKeyword指向关键词	
	
	if( *pBuf != '*' ){
		return SMS_CMD_END;		
	}
	
	pTmp = (unsigned char*)strchr( (char*)pBuf, TEXT_EQUAL );
	if( pTmp == NULL ){
		pTmp = (unsigned char*)strchr( (char*)pBuf, TEXT_CMD_END );
		if( pTmp ){
			*pTmp = 0x00;
		}
	}
	else{
		*pTmp = 0x00;
		pTmp++;		
	}
	 
	if( *pBuf != TEXT_CMD_START || pTmp == NULL ){
		return SMS_CMD_END;		
	}		
	pKeyword = pBuf+1;
	
	// 命令码比较 and
	// 简单命令码比较
	for( nCnt = 0; nCnt < SMS_CMD_END; nCnt++ ){
	 	if( strcmpnc((unsigned char*)g_strSetCmd[nCnt], pKeyword ) ||
	 			strcmpnc((unsigned char*)g_strAbbrCmd[nCnt],pKeyword ) ){
			break;
		}	 	
	}
	// 中文指令查询
	if( nCnt >= SMS_CMD_END ){
		for( nCnt = 0; nCnt < SMS_CMD_END; nCnt++ ){		
		 	if( strcmpnc((unsigned char*)g_strSetCmdCN[nCnt], pKeyword )  ){
		 		GSM_SetLangState();			 		
				break;
			}	 	
		}
	}

	if( nCnt < SMS_CMD_END ){		
		strcpy( (char*)pBuf, (char*)pTmp );	
		pTmp = (unsigned char*)strrchr( (char*)pBuf, TEXT_CMD_END );		
		if( pTmp ){		
			*pTmp = 0x00;
		}
	}
	return nCnt;	
}				
/////////////////////////////////////////////////////////////
//功能： 数据协议解释
unsigned char	PLS_ParseCommandData( unsigned char  *pDest, 
																	  unsigned char  *pSrc,
																	  unsigned char  *pCaller,
																	  unsigned char  *pCodeType  ){
										
	unsigned char	nCmdType  = 0;			
	unsigned short	nTmp = 0;
	unsigned char	nResult = 0;
	unsigned char	nLangCN;
	unsigned char 	nBuf[20];
	
	// 适用蒙苦第一个字母子
	if( *pSrc == ' ' ){	
		pSrc++;
	}
	
	nCmdType = PLS_GetCommandType( pSrc );
	
	if( GSM_IsLangState() ){
		nLangCN = 1;	
		*pCodeType = 2;	
	}
	else{
		nLangCN = 0;	
		*pCodeType = 0;			
	}
	
	if( !IsAuthorizationNo(pCaller) ){
		if( nCmdType == SMS_MASTER_SETUP ||
		  	nCmdType == SMS_CONTROL_LISTEN ||
		  	nCmdType == SMS_VERSION ||
		  	nCmdType == SMS_SERIAL_SN ||
		  	nCmdType == SMS_CONTROL_TALK||
		  	nCmdType == SMS_SETUP ){ 
		  	//continue	
		}
		else if( nCmdType < SMS_CMD_END){		
			if( nLangCN ){
				strcpy( (char*)pDest, (char*)TEXT_NOT_AUTHORIZED_CN );	
			}
			else{
				strcpy( (char*)pDest, (char*)TEXT_NOT_AUTHORIZED );	
			}
			return strlen((char*)pDest);			
		}
		else{
			*pDest = 0x00;
			return strlen((char*)pDest);			
		}
	}
	else{
		if( nCmdType >= SMS_CMD_END ){	
					
			if( nLangCN ){
				strcpy( (char*)pDest, (char*)TEXT_CMD_ERROR_CN );
			}
			else{
				strcpy( (char*)pDest, (char*)TEXT_CMD_ERROR );				
			}
			return strlen((char*)pDest);
		}
		else if( nCmdType == SMS_DEFAULT ||
				 nCmdType == SMS_SLAVE_SETUP1 ||
				 nCmdType == SMS_SLAVE_SETUP2 ||
				 nCmdType == SMS_SLAVE_SETUP3 ||
				 nCmdType == SMS_PASSWORD_SETUP ){			
			if( !IsMasterNo(pCaller) ){
				if( nLangCN ){
					strcpy( (char*)pDest, (char*)TEXT_NOT_AUTHORIZED_CN );		
				}
				else{
					strcpy( (char*)pDest, (char*)TEXT_NOT_AUTHORIZED );						
				}
				return strlen((char*)pDest);			
			}			
		}
	}	
	
	
	switch( nCmdType ){	
	case  SMS_RESTART:						//重启复位
		TE_SetNeedTEReset(  );
		if( nLangCN ){			
			strcpy( (char*)pDest, TEXT_RESET_SUCCESS_CN );	
			nResult = strlen( (char*)pDest );
		}
		else{
			sprintf( (char*)pDest, "%s %s", g_strSetCmd[nCmdType], TEXT_SUCCESS );
			nResult = strlen( (char*)pDest );
		}
		break;
		
	case  SMS_DEFAULT:						//恢复出厂设置
		TE_SetNeedWriteFlash( );
		InitializeAlarmFlash();
		InitializeGSMFlash();
		if( nLangCN ){			
			strcpy( (char*)pDest, TEXT_RESTORE_SUCCESS_CN );	
			nResult = strlen( (char*)pDest ); 
		}
		else{
			sprintf( (char*)pDest, "%s %s", g_strSetCmd[nCmdType], TEXT_SUCCESS );	
			nResult = strlen( (char*)pDest );
		}
		break;
	
	// 只有英文命令
	// *pCodeType must equal 0	
	case  SMS_SERIAL_SN:
		#if( SW_PROTOCOL_ASICO )	
		if( strlen((char*)pSrc) == 0 ){	 		
			sprintf( (char*)pDest, "%s:%s",	TEXT_SN, g_stGSMSetup.m_strUID );	
			nResult = strlen( (char*)pDest );		
			*pCodeType = 0;		
		}
		else{	
			memset( g_stGSMSetup.m_strUID, 0x00, TELNUMBER_LENGTH+1 );
			strncpy( (char*)g_stGSMSetup.m_strUID, (char*)pSrc, TELNUMBER_LENGTH );
					
			TE_SetNeedWriteFlash( );
			sprintf( (char*)pDest, "%s:%s",	TEXT_SN, g_stGSMSetup.m_strUID );	
			nResult = strlen( (char*)pDest );		
			*pCodeType = 0;					
		}
		#else			 
			#if( HW_EM200 )
				if( pSrc != NULL ){
					if( strlen((char*)pSrc) > 0 ){
						if( pSrc[0] != TEXT_QUERY ){					
							memset( g_stGSMSetup.m_strUID, 0x00, TELNUMBER_LENGTH+1 );
							strncpy( (char*)g_stGSMSetup.m_strUID, (char*)pSrc, TELNUMBER_LENGTH );
							TE_SetNeedWriteFlash( );
						}
					}
				}
				sprintf( (char*)pDest, "%s:%s",	TEXT_SN, g_stGSMSetup.m_strUID );	
				nResult = strlen( (char*)pDest );		
			#else	
				#if( HW_MC323 )
					if( pSrc != NULL ){
						if( strlen((char*)pSrc) > 0 ){
							if( pSrc[0] != TEXT_QUERY ){					
								memset( g_stGSMSetup.m_strUID, 0x00, TELNUMBER_LENGTH+1 );
								strncpy( (char*)g_stGSMSetup.m_strUID, (char*)pSrc, TELNUMBER_LENGTH );
								TE_SetNeedWriteFlash( );
							}
						}
					}
					sprintf( (char*)pDest, "%s:%s",	TEXT_SN, g_stGSMSetup.m_strUID );	
					nResult = strlen( (char*)pDest );		
				#else
					sprintf( (char*)pDest, "%s:%s",	TEXT_SN, g_stGSMSetup.m_strUID );	
					nResult = strlen( (char*)pDest );		
				#endif	
			#endif
		*pCodeType = 0;	
		#endif
		break;
		
	case  SMS_STATE:
		nResult = PLS_ReadState( pDest, nLangCN );
		break;
	
	// 只有英文命令
	// *pCodeType must equal 0		
	case  SMS_VERSION:
		sprintf( (char*)pDest, "%s:%s",	TEXT_VERSION, g_strVer );	
		nResult = strlen( (char*)pDest );
		*pCodeType = 0;				
		break;
		
	case  SMS_POSITION:						//定位
	case  SMS_POSITION6:					//查询地址
		if( ( nCmdType == SMS_POSITION6 ) && TE_IsLoginSuccess( ) ){
		
			strcpy( (char*)g_stGSMState.m_strForward, (char*)pCaller );
			TE_SetQueryGGAddrFlag();
			nResult = 0;
		}
		else{
			if ( COM_IsSMSFeedbackUseJave() ){
				sprintf( (char*)pDest, "%s:", g_stGSMSetup.m_strUID );
				nResult = strlen((char*)pDest);	
				GPS_GetGPRMCData( pDest+nResult ); 
				nResult = strlen((char*)pDest);	
			}
			else{
				nLangCN = 0;
				nResult = PLS_ComposePosition( pDest, nLangCN );
				if( nLangCN == 0 ){		
					
					sprintf( (char*)(pDest+nResult), "%s?lng=%.6f&lat=%.6f&key=%d",   TEXT_GOGPS,
																	     		  (double)GPS_GetLongitude()/3600000,
																	     		  (double)GPS_GetLatitude( )/3600000,
																	     		  GPS_CalculateData() );
					nResult = strlen( (char*)pDest );	
				}		    	
			}	
		}
		*pCodeType = 0;										     
		break;	
		
	case SMS_POSITION2:
		if( strlen((char*) g_stGSMSetup.m_strSMSAddrNo) == 0 ){
			nResult = PLS_ComposePosition( pDest, nLangCN );
		}
		else{
			nResult = PL_ComposeGPSData( pDest );
			strcpy( (char*)g_stGSMState.m_strForward, (char*)pCaller );
			strcpy( (char*)pCaller, (char*)g_stGSMSetup.m_strSMSAddrNo );
			*pCodeType = 1;
		}
		break;
	case SMS_POSITION3:
		nResult = PLS_ComposeGPRMCPosition( pDest );
		break;	
	case  SMS_ALARM_SETUP:					//设置报警允许/禁止
		if( atoi((char*)pSrc ) == 0 ){
			nResult = PLS_SetupAlarmEnable( pDest, 0, nLangCN );
		}
		else{
			nResult = PLS_SetupAlarmEnable( pDest, 1, nLangCN );			
		}
		break;
	// 按距离 *TRACK=n:d#	 
	// 按时间 *TRACK=n:s#	 
	case  SMS_TRACK_SETUP:
		if ( *pSrc != TEXT_QUERY ){
			TE_SetNeedWriteFlash( );			
			PLS_SetupTrackSetup( pSrc );
		}
		if( nLangCN ){
			
			strcpy( (char*)pDest, (char*)TEXT_AUTOTRACK_CN );
			nResult = strlen( (char*)pDest );
			nResult += unicodecpy( pDest+nResult, (unsigned char*)":" );
			if( g_stGSMSetup.m_nCBMode == CB_STOP ){			
				strcpy( (char*)pDest+nResult, (char*)TEXT_DISABLE_CN );
				nResult += strlen( (char*)TEXT_DISABLE_CN );
			}
			else if( g_stGSMSetup.m_nCBMode == CB_BYDISTANCE ){
				sprintf( (char*)nBuf, "%d(m)", g_stGSMSetup.m_nCBSpace );			
				nResult += unicodecpy( pDest+nResult, nBuf );
			}
			else{
				sprintf( (char*)nBuf, "%d(sec)", g_stGSMSetup.m_nCBSpace );		
				nResult += unicodecpy( pDest+nResult, nBuf );			
			}				
		}
		else{
			if( g_stGSMSetup.m_nCBMode == CB_STOP ){			
				sprintf( (char*)pDest, "%s %s", TEXT_AUTOTRACK, TEXT_DISABLE );
			}
			else if( g_stGSMSetup.m_nCBMode == CB_BYDISTANCE ){
				
				sprintf( (char*)pDest, "%s %s %d(m)", TEXT_AUTOTRACK, TEXT_EVERY, g_stGSMSetup.m_nCBSpace );
			}
			else{
				sprintf( (char*)pDest, "%s %s %d(s)", TEXT_AUTOTRACK, TEXT_EVERY, g_stGSMSetup.m_nCBSpace );			
				
			}	
			nResult = strlen( (char*)pDest );
		}
		break;
	case  SMS_SERVER_DNSR:	//设置域名
		nResult = PLS_SetupServerDNSR( pDest, pSrc, nLangCN );
		break;	
	case  SMS_PARKING_SETUP:
		nResult = PLS_SetupAlarmParking( pDest, pSrc, nLangCN );
		break;
	case  SMS_ANTISTEAL_SETUP:
		nResult = PLS_SetupAlarmAntisteal( pDest, pSrc, nLangCN );
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
			// 20110305 V2.79 modify		
			TE_SetASILTrigger();	 //负触发
			
			AL_EnableNotDoor();		 //开启了防盗报警
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
	case  SMS_OVERSPEED_SETUP:
		nResult = PLS_SetupAlarmOverspeed( pDest, pSrc, nLangCN);
		break;
	case SMS_TOW:
		nResult = PLS_SetupAlarmTow( pDest, pSrc, nLangCN );
		break;
		
	case  SMS_CORTROL_OIL:
		if( atoi((char*)pSrc) == 0 ){	
			nResult = PLS_CtrlOil( pDest, 0, nLangCN );		
		}
		else{
			nResult = PLS_CtrlOil( pDest, 1, nLangCN );					
		}
		break;			
	// V2.81 modify
	case SMS_CLOSE_OIL:
		nResult = PLS_CtrlOil( pDest, 0, nLangCN );		
		break;	
	case SMS_RESUME_OIL:
		nResult = PLS_CtrlOil( pDest, 1, nLangCN );					
		break;	
		
	case  SMS_CONTROL_LISTEN:
		nResult = PLS_CarListen(  pDest, pSrc, nLangCN );
		
		#if( HW_ET201B || HW_ET201B_2 || HW_ET201B_3 )
		HW_PORT_SPKMUTE_CLOE();		//静音开启 
		#endif
		break;
	case SMS_CONTROL_TALK:
		nResult = PLS_CarListen( pDest, pSrc, nLangCN);
		if( nResult == 0 ){			
			#if( HW_ET201B || HW_ET201B_2 || HW_ET201B_3 )
			HW_PORT_SPKMUTE_OPEN();  //静音关闭
			#endif
		}
		break;
	case  SMS_SLAVE_SETUP1:				
		nResult = PLS_SetupSlaveNo( pDest, pSrc, 1, nLangCN );
		break;
	case  SMS_SLAVE_SETUP2:					
		nResult = PLS_SetupSlaveNo( pDest, pSrc, 2, nLangCN );
		break;
	case  SMS_SLAVE_SETUP3:					
		nResult = PLS_SetupSlaveNo( pDest, pSrc, 3, nLangCN );
		break;
	case  SMS_SLAVE_SETUP4:					
		nResult = PLS_SetupSlaveNo( pDest, pSrc, 4, nLangCN );
		break;
	case  SMS_SLAVE_SETUP5:					
		nResult = PLS_SetupSlaveNo( pDest, pSrc, 5, nLangCN );
		break;
	case  SMS_MASTER_SETUP:		
		nResult = PLS_SetupMasterNo( pDest, pSrc, nLangCN );
		break;
	case  SMS_SMC_SETUP:	
		nResult = PLS_SetupSMCNo( pDest, pSrc, nLangCN );
		break;
	case  SMS_PASSWORD_SETUP:	
		nResult = PLS_SetupPassword( pDest, pSrc, TEXT_COLON, nLangCN );
		break;
	// only english
	case  SMS_SERVER_SETUP:	
		nResult = PLS_SetupServer( pDest, pSrc, TEXT_COLON, nLangCN );
		break;
	// only english
	case  SMS_APN_SETUP:	
		nResult = PLS_SetupAPN( pDest, pSrc, nLangCN  );
		break;
	// only english
	case  SMS_NET_SETUP:
		nResult = PLS_SetupAPNUserPsd( pDest, pSrc, TEXT_COLON, nLangCN );
		break;
	// only english
	case SMS_WORKMODE_SETUP:	
		nResult = PLS_SetupWorkMode( pDest, pSrc, nLangCN  );
		break;
	case SMS_DEFENSIVE:
		nResult = PLS_SetupDefensive( pDest, pSrc, nLangCN );
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
		*pCodeType = 0;				
		nResult = strlen( (char*)pDest );
		break;
	case SMS_INGEO:		//设置入区域报警
		nResult = PLS_SetupInGeo( pDest, pSrc );
		TE_SetNeedWriteFlash( );
		break;
	case SMS_OUTGEO:	//设置进出区域报警	
		nResult = PLS_SetupOutGeo( pDest, pSrc, nLangCN);
		TE_SetNeedWriteFlash( );
		break;
	case SMS_NAMEC:		//自定义命名
		nResult = PLS_SetupCustomName( pDest, pSrc );
		break;	
	case SMS_REINPUT:	//设置自定义高低电平	
		nResult = PLS_SetReInput( pDest, pSrc, TEXT_COLON,nLangCN );
		TE_SetNeedWriteFlash( );			
		break;
	case SMS_LOCK:		//中控锁
		nResult = PLS_SetupLock( pDest, pSrc, nLangCN);
		break;
	case SMS_POWEROFF:	//开启域关闭电源
		if( nLangCN ){
			sprintf((char*)pDest, "%s%s",TEXT_POWEROFF_CN,TEXT_ALARM_CN );
		}
		else{
			sprintf((char*)pDest, "%s %s ",TEXT_POWEROFF, TEXT_ALARM );
		}		
		if( atoi((char*)pSrc) == 0 ){ //关闭
			AL_DisablePowerOff();
			if( nLangCN ){
				strcat( (char*)pDest, TEXT_DISABLE_CN);
			}
			else{
				strcat( (char*)pDest, TEXT_DISABLE);
			}			 
		}
		else{						  //开启	
			AL_EnablePowerOff();
			if( nLangCN ){
				strcat( (char*)pDest, TEXT_ENABLE_CN);
			}
			else{
				strcat( (char*)pDest, TEXT_ENABLE );
			}
		}
		TE_SetNeedWriteFlash();	
		nResult = strlen((char*)pDest);
		break;
	case SMS_LOWPOWER:  //开启域关闭低电源 
		if( nLangCN ){
			sprintf((char*)pDest, "%s%s",TEXT_LOW_POWER_CN,TEXT_ALARM_CN );
		}
		else{
			sprintf((char*)pDest, "%s %s ",TEXT_LOW_POWER,TEXT_ALARM );
		}		
		if( atoi((char*)pSrc) == 0 ){ //关闭
			AL_DisableLowPower();
			if( nLangCN ){
				strcat( (char*)pDest, TEXT_DISABLE_CN);
			}
			else{
				strcat( (char*)pDest, TEXT_DISABLE);
			}	
		}
		else{						  //开启	
			AL_EnableLowPower();
			if( nLangCN ){
				strcat( (char*)pDest, TEXT_ENABLE_CN);
			}
			else{
				strcat( (char*)pDest, TEXT_ENABLE );
			}
		}
		TE_SetNeedWriteFlash();	
		nResult = strlen((char*)pDest);
		break;
	case SMS_SETUP:	//设置设备的参数	
		nResult = PLS_SetupDeviceInfo( pDest, pSrc );
		TE_SetNeedWriteFlash();		
		break;
	case SMS_HANDSHAKE:				//设置握手信号		
		if( *pSrc != TEXT_QUERY ){
			g_stGSMSetup.m_nHandShakeTime = atoi( (char*)pSrc );
			if( g_stGSMSetup.m_nHandShakeTime < MIN_TIME_SEC ){
				g_stGSMSetup.m_nHandShakeTime = MIN_TIME_SEC;			
			} 		
			TE_SetNeedWriteFlash();
		}
		sprintf( (char*)pDest, "%s:%d %s", TEXT_HANDSHAKE_TIME, g_stGSMSetup.m_nHandShakeTime, TEXT_SECOND );
		nResult = strlen( (char*)pDest);
		break;
	case SMS_ACCALARM:				//设置ACC报警
		if( *pSrc != TEXT_QUERY ){
			nTmp = atoi( (char*)pSrc );
			if( nTmp == 0 ){
				AL_DisableACCAlarm();
			}	
			else{
				AL_EnableACCAlarm();
			}		
			TE_SetNeedWriteFlash();
		}
		if( AL_IsEnableACCAlarm() ){
			sprintf( (char*)pDest, "%s %s %s", TEXT_ACC, TEXT_ALARM, TEXT_ENABLE );
		}
		else{
			sprintf( (char*)pDest, "%s %s %s", TEXT_ACC, TEXT_ALARM, TEXT_DISABLE );
		}
		nResult = strlen( (char*)pDest);
		break;
	case SMS_ACCIDLE:				//ACC空闲设置
		if( *pSrc != TEXT_QUERY ){
			g_stAlarmSetup.m_nACCIdle = atoi( (char*)pSrc );
			TE_SetNeedWriteFlash();
		}
		if( g_stAlarmSetup.m_nACCIdle > 0 ){
			sprintf( (char*)pDest, "%s:%d Minute", TEXT_IDLE_AFTER_ACC_OFF,
												    g_stAlarmSetup.m_nACCIdle );
		}
		else{
			sprintf( (char*)pDest, "%s:%s",TEXT_IDLE_AFTER_ACC_OFF, TEXT_DISABLE );
		}
		nResult = strlen( (char*)pDest);
		break;
	case SMS_NOGPSALARM:			//GPS未定位报警
		if( *pSrc != TEXT_QUERY ){
			nTmp = atoi( (char*)pSrc );
			if( nTmp == 0 ){
				AL_DisableNoGPSAlarm();
			}
			else{
				AL_EnableNoGPSAlarm();
			}
			g_stAlarmSetup.m_nNoGPS = nTmp;
			TE_SetNeedWriteFlash();
		}		
		if( AL_IsEnableNoGPSAlarm() ){
			sprintf( (char*)pDest, "%s:%d Minute", TEXT_ALARM_NOGPS_SIGNAL,g_stAlarmSetup.m_nNoGPS );
		}
		else{
			sprintf( (char*)pDest, "%s:%s", TEXT_ALARM_NOGPS_SIGNAL, TEXT_DISABLE );
		}
		nResult = strlen( (char*)pDest);
		break;
	case SMS_DATAFORMAT:			//GPS数据格式
		if( *pSrc != TEXT_QUERY ){
			nTmp = atoi( (char*)pSrc );
			if( nTmp == 0 ){
				COM_SetSMSFeedbackUseJave();
			}
			else{
				COM_ClrSMSFeedbackUseJave();
			}
			TE_SetNeedWriteFlash();
		}
		if( COM_IsSMSFeedbackUseJave() ){
			sprintf((char*)pDest, "%s %s %s ",TEXT_CHANFE_TO, TEXT_NMEA, TEXT_FORMAT );	
		}
		else{
			sprintf( (char*)pDest, "%s %s %s ", TEXT_CHANFE_TO, TEXT_MAPTRACK, TEXT_FORMAT );
		}
		nResult = strlen( (char*)pDest );
		break;
	case SMS_MILEAGE_REMARK:			//设置里程
		if( *pSrc != TEXT_QUERY ){
		
			nTmp = atoi( (char*)pSrc );
			if( nTmp == 0 ){				
				SetMaxMileage(0);				
			}
			else{
				SetMaxMileage( nTmp*1000 );
			}
			ClearCurrentMileage();
			TE_SetNeedWriteFlash();
		}
		if( GetMaxMileage() > 0 ){
			sprintf( (char*)pDest, "%s: %d KM", TEXT_MILEAGE_REMARK, GetMaxMileage()/1000 );			
		}
		else{
			sprintf( (char*)pDest, "%s: %s.", TEXT_MILEAGE_REMARK, TEXT_DISABLE );			
		}
		nResult = strlen( (char*)pDest );
		break;
	case SMS_TOTAL_MILEAGE:				//查询总里程			
		*pCodeType = 0;	
		sprintf( (char*)pDest, "%s:%.2f KM", TEXT_TOTAL_MILEAGE, (float)GetTotalMileage()/1000 );
		nResult = strlen( (char*)pDest );
		break;
	case SMS_SOS:
		*pCodeType = 0;
		if( *pSrc != TEXT_QUERY ){
			nTmp = atoi( (char*)pSrc );
			if( nTmp == 0 ){
				AL_DisableSOS();
			}
			else{
				AL_EnableSOS();
			}
			TE_SetNeedWriteFlash();
		}
		if( AL_IsEnableSOS() ){
			sprintf( (char*)pDest, "%s %s %s", TEXT_SOS, TEXT_ALARM, TEXT_ENABLE  );
		}
		else{
			sprintf( (char*)pDest, "%s %s %s", TEXT_SOS,TEXT_ALARM, TEXT_DISABLE );
		}		
		nResult = strlen( (char*)pDest );
		break;
	case SMS_SETUP_AUTOFUEL:
		*pCodeType = 0;
		if( *pSrc != TEXT_QUERY ){
			nTmp = atoi( (char*)pSrc );
			if( nTmp == 0 ){
				HW_DisableAutoFuel();
			}
			else{
				HW_EnableAutoFuel();
			}
			TE_SetNeedWriteFlash();
		}
		if( HW_IsEnableAutoFuel() ){
			sprintf( (char*)pDest, "%s %s %s", TEXT_AUTOFUEL, TEXT_ALARM, TEXT_ENABLE  );
		}
		else{
			sprintf( (char*)pDest, "%s %s %s", TEXT_AUTOFUEL,TEXT_ALARM, TEXT_DISABLE );
		}		
		nResult = strlen( (char*)pDest );
		break;
	case SMS_CRASH:
	//	*pCodeType = 0;
		if( *pSrc != TEXT_QUERY ){
			nTmp = atoi( (char*)pSrc );
			if( nTmp == 0 ){
				AL_DisableCrashAlarm();
			}
			else{
				AL_EnableCrashAlarm();
			} 
			TE_SetNeedWriteFlash();
		}
		if( AL_IsEnableCrashAlarm() ){
			if( nLangCN ){
				sprintf((char*)pDest, "%s%s%s", TEXT_CRASH_CN, TEXT_ALARM_CN,TEXT_ENABLE_CN);
			}
			else{
				sprintf( (char*)pDest, "%s %s %s", TEXT_CRASH, TEXT_ALARM, TEXT_ENABLE  );
			}
		}
		else{
			if( nLangCN ){
				sprintf((char*)pDest, "%s%s%s", TEXT_CRASH_CN, TEXT_ALARM_CN,TEXT_DISABLE_CN);
			}
			else{
				sprintf( (char*)pDest, "%s %s %s", TEXT_CRASH, TEXT_ALARM, TEXT_ENABLE  );
			}
		}		
		nResult = strlen( (char*)pDest );
		break;
	default:
		break;	
	}		
	return nResult;								
}
/////////////////////////////////////////////////////////////////////////////
//  设置入界报警
unsigned char	PLS_SetupInGeo( unsigned char	*pDest, unsigned char	*pSrc ){
	
	unsigned char	nResult;
	unsigned short	nTmp = 0;
	
	nTmp = atoi( (char*)pSrc);
	if( nTmp == 0 ){
		g_stCustomSetup.m_nInGeo = 0;
		TE_DisableInGeo();
		sprintf((char*)pDest, "%s-%s:%s",TEXT_IN,
										 TEXT_GEOFENCE,
										 TEXT_DISABLE );
	}
	else{
		if( GPS_IsPosValid() ){		//GPS定位有效	
			if( nTmp < MINGEO_SETUP ){
				nTmp = MINGEO_SETUP;
			}
			g_stCustomSetup.m_nInGeo = nTmp; 
			g_stCustomSetup.m_nInLog = GPS_GetLongitude();	//经度
			g_stCustomSetup.m_nInLag = GPS_GetLatitude();		//纬度
			TE_EnableInGeo();
			sprintf((char*)pDest, "%s-%s:%d %s",TEXT_IN,
										     TEXT_GEOFENCE,
										     g_stCustomSetup.m_nInGeo,
										     TEXT_M );
		}	
		else{
			sprintf((char*)pDest, "%s", TEXT_CAN_SETUP );
		}		
	}
	nResult = strlen((char*)pDest);
	return nResult;
}
/////////////////////////////////////////////////////////
//  GPRMC数据	N 2234.4271,E 11351.2681
unsigned char PLS_ComposeGPRMCPosition( unsigned char *pDest ){

	unsigned char	nLen = 0;
	unsigned char	nChar = 0;
	
	if( g_stGPSInfo.m_nLLFlag & N_LATITUDE ){ 	
		nLen = 'N';
	}
	else{
		nLen = 'S';
	}
	if ( g_stGPSInfo.m_nLLFlag & E_LONGITUDE) {	
		nChar = 'E';	
	}
	else{
		nChar = 'W';
	}
	sprintf( (char*)pDest, "%c %d%02d.%04d,%c %d%02d.%04d,%dkm/h",
						   nLen,
						   g_stGPSInfo.m_nLATIDegree,
						   g_stGPSInfo.m_nLATIMinute,
						   g_stGPSInfo.m_nLATIPrecision,
						   nChar,
						   g_stGPSInfo.m_nLONGDegree,
						   g_stGPSInfo.m_nLONGMinute,
						   g_stGPSInfo.m_nLONGPrecision,
						   GPS_GetMilimetre() );
	
	nLen = strlen( (char*)pDest);
	return nLen;
}
/////////////////////////////////////////////////////////////////////////////
//	设置出界报警
unsigned char	PLS_SetupOutGeo( unsigned char	*pDest, 
								 unsigned char	*pSrc, 
								 unsigned char   nLangCN ){
	
	unsigned char	nResult;
	unsigned short	nTmp = 0;
	
	nTmp = atoi( (char*)pSrc);
	if( nTmp == 0 ){
		g_stCustomSetup.m_nDistance = 0;
		TE_DisableOutGeo();
		if( nLangCN ){
			sprintf( (char*)pDest, "%s%s", TEXT_OUTGEOFENCE_CN, TEXT_DISABLE_CN  );		
		}
		else{
			sprintf((char*)pDest, "%s-%s:%s",TEXT_OUT,
											 TEXT_GEOFENCE,
											 TEXT_DISABLE );
		}
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
			sprintf((char*)pDest, "%s-%s:%d %s",TEXT_OUT,
										        TEXT_GEOFENCE,
										        g_stCustomSetup.m_nDistance,
										        TEXT_M );
		}	
		else{
			sprintf((char*)pDest, "%s", TEXT_CAN_SETUP );
		}									 
	}
	nResult = strlen((char*)pDest);
	return nResult;
}
/////////////////////////////////////////////////////////////////////////////
//	设置自定义命名 x:yyyyy#
unsigned char	PLS_SetupCustomName( unsigned char  *pDest, 
									 unsigned char	*pSrc ){

	unsigned  char	*pch = NULL;
	unsigned  char  nLen = 0;
	unsigned  char	nID = 0;
	
	pch = (unsigned char*)strchr((char*)pSrc, TEXT_COLON );
	if( pch != NULL ){
		*pch = 0x00;
		nID = atoi((char*)pSrc );
		pch++;
		nLen = strlen( (char*)pch );
		if( nLen > TELNUMBER_LENGTH ){			
			nLen = TELNUMBER_LENGTH;
		}
		switch( nID ){
		case 1:
			memcpy(g_stCustomSetup.m_nCustom1, pch, nLen );
			g_stCustomSetup.m_nCustom1[nLen] = 0;
			break;
		case 2:
			memcpy(g_stCustomSetup.m_nCustom2, pch, nLen );
			g_stCustomSetup.m_nCustom2[nLen] = 0;
			break;
		case 3:
			memcpy(g_stCustomSetup.m_nCustom3, pch, nLen );
			g_stCustomSetup.m_nCustom3[nLen] = 0;
			break;
		case 4:
			memcpy(g_stCustomSetup.m_nCustom4, pch, nLen );
			g_stCustomSetup.m_nCustom4[nLen] = 0;
			break;
		}		
		sprintf( (char*)pDest, "%s %s%d:%s", TEXT_CUSTOME,
											 TEXT_ALARM,
											 nID,
											 (char*)pch );
		TE_SetNeedWriteFlash( );
	}	
	else{
		strcpy( (char*)pDest, TEXT_PARA_ERROR );
	}	
	return strlen((char*)pDest);										 									 
}
/////////////////////////////////////////////////////////////////////////////
//    TRACK=1000:S#
unsigned char PLS_SetupTrackSetup( unsigned char  *pSrc ){
	
	unsigned int		nTmp  = 0;	
	unsigned char		nChar = 0;
	unsigned char		*pTmp = NULL;
	
	nTmp = atoi( (char*)pSrc );	
	if( nTmp > SHORT_MAXVALUE ){		
		nTmp = SHORT_MAXVALUE;
	}	
	pTmp = (unsigned char*)strchr( (char*)pSrc, TEXT_COLON);			
	if( pTmp != NULL ){
		pSrc = pTmp+1;
		nChar = *pSrc;
	}	
	if( nTmp == 0 ){
		g_stGSMSetup.m_nCBMode = CB_STOP;
	}
	else{
		if( (nChar == 'D') || (nChar == 'd') ){
			if ( nTmp < MINGEO_SETUP ){			
				nTmp = MINGEO_SETUP;
			}
			g_stGSMSetup.m_nCBSpace = nTmp;
			g_stGSMSetup.m_nCBMode = CB_BYDISTANCE;
		}
		else{
			if( nTmp < 60 ){
				nTmp = 60;						
			}			
			g_stGSMSetup.m_nCBSpace = nTmp;
			g_stGSMSetup.m_nCBMode = CB_BYTIME;
		}	
	}
	return 1;	
}
/////////////////////////////////////////////////////////////////////////////
// 电话监听
unsigned char PLS_CarListen( 	unsigned char  *pDest,	
															unsigned char  *pSrc, 
															unsigned char   nLangCN){

	unsigned char	*pTmp = NULL;
		
	pTmp = (unsigned char*)strchr( (char*)pSrc, TEXT_COLON);	
	
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
		if( strcmp((char*)pTmp, (char*)g_stGSMSetup.m_strPassword) == 0x00 ){
			#if( HW_EM200 )	
				if( GSM_IsGPRSCommMode() ){
					CMM_IPClose();
					CMM_DTUPPPClose();
					IP_SetConnectNG( );	
				}
			#endif	//HW_EM200
			#if( HW_MC323 )
				if( GSM_IsGPRSCommMode() ){
					CMM_IPClose();
					CMM_DTUPPPClose();
					IP_SetConnectNG( );	
				}	
			#endif //HW_MC323
			CMM_DialNumber( pSrc );
			*pDest = 0x00;
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
//	设置主控号码
unsigned char PLS_SetupMasterNo( unsigned char  *pDest, unsigned char  *pSrc, unsigned char nLangCN ){

//	unsigned char  nLen = 0;
	unsigned char  *pTmp = NULL;
	unsigned char  nResult = 0;
	
	pTmp = (unsigned char*)strchr( (char*)pSrc, TEXT_COLON );
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
		if( strcmp((char*)pTmp, (char*)g_stGSMSetup.m_strPassword) == 0x00 ){
		
			memset( g_stGSMSetup.m_strMaterNo, 0x00, TELNUMBER_LENGTH+1 );
			strncpy( (char*)g_stGSMSetup.m_strMaterNo, (char*)pSrc, TELNUMBER_LENGTH );
			
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
#endif	//SW_SMSPROTOCOLB

//////////////////////////////////////////////////////////////////////////////
//
unsigned char unicodecpy( unsigned char	*pDest, unsigned char *pSrc ){
	
	unsigned char			nLen;
	unsigned char 		nCnt;
	
	nLen = strlen( (char*)pSrc );
	
	for( nCnt = 0; nCnt < nLen; nCnt++ ){		
		*pDest++ = 0x00;
		*pDest++ = *pSrc++;		
	}	
	return nLen*2;
}
//////////////////////////////////////////////////////////////////////////////
//
unsigned char PLS_SetupAlarmEnable(unsigned char	*pDest, unsigned char	nFlag, unsigned char nLangCN ){
	
	unsigned char			nResult = 0;	
		
	if ( nFlag ){
		g_stAlarmSetup.m_nAlarmEnable = 0xFFFFFFFF;	
		
		if( nLangCN ){		
			strcpy( (char*)pDest, TEXT_ALARM_ENABLE_CN );
		}
		else{
			sprintf( (char*)pDest, "%s %s %s %s", 
													TEXT_ENABLE,
													TEXT_ALL,
													TEXT_ALARM,
													TEXT_SUCCESS );
		}
	}
	else{
		g_stAlarmSetup.m_nAlarmEnable = AL_SOS;
		if( nLangCN ){			
			strcpy( (char*)pDest, TEXT_ALARM_DISABLE_CN );			
		}
		else{
			sprintf( (char*)pDest, "%s %s %s %s", 
										  	TEXT_DISABLE,
												TEXT_ALL,
												TEXT_ALARM,
												TEXT_SUCCESS );
		}
	}	
	TE_SetNeedWriteFlash( );	
	nResult = strlen( (char*)pDest );
	return nResult;
}		
/////////////////////////////////////////////////////////////////////////////
//  设置电平的输入
unsigned char	PLS_SetReInput( unsigned char	*pDest, 
								unsigned char	*pSrc, 
								unsigned char   nColon,
								unsigned char   nLangCN ){
	
	unsigned char	nResult;
	unsigned char	nData = 0;
	unsigned char	nCmd = 255;
	unsigned char	*pTmp = NULL;
	
	nData = atoi( (char*)pSrc );		
	pTmp = (unsigned char*)strchr((char*)pSrc, nColon );
	if( pTmp == NULL ){
		if( nLangCN ){			
			strcpy( (char*)pDest, TEXT_PARA_ERROR_CN );		
		}
		else{
			strcpy( (char*)pDest, TEXT_PARA_ERROR );				
		}
		nResult = strlen((char*)pDest );
	}	
	else{
		 pTmp++;
		 nCmd = atoi((char*)pTmp);
		 switch( nData ){
		 case 0:	//门检    bit4
		 	sprintf((char*)pDest,"%s ", TEXT_ANTISTEAL );
		 	if( nCmd == 0 ){  //负触发 
			 	TE_SetASILTrigger();
			 }
			 else{				//正角发
			 	TE_SetASIHTrigger();
			 } 
		 	break;
		 case 1:	//自定义1 bit0
		 	sprintf((char*)pDest,"%s1 ", TEXT_CUSTOME );
		 	if( nCmd == 0 ){  //负触发
			 	TE_SetC1LTrigger();
			 }
			 else{				//正角发
			 	TE_SetC1HTrigger();
			 }
		 	break;
		 case 2:	//自定义2 bit1
		 	sprintf((char*)pDest,"%s2 ", TEXT_CUSTOME );
		 	if( nCmd == 0 ){  //负触发
			 	TE_SetC2LTrigger();
			 }
			 else{				//正角发
			 	TE_SetC2HTrigger();
			 }
		 	break;
		 case 3:	//自定义3 bit2
		 	sprintf((char*)pDest,"%s3 ", TEXT_CUSTOME );
		 	if( nCmd == 0 ){  //负触发
			 	TE_SetC3LTrigger();
			 }
			 else{				//正角发
			 	TE_SetC3HTrigger();
			 }
		 	break;
		 case 4:	//自定义4 bit3
		 	sprintf((char*)pDest,"%s4 ", TEXT_CUSTOME );
		 	if( nCmd == 0 ){  //负触发
			 	TE_SetC4LTrigger();
			 }
			 else{				//正角发
			 	TE_SetC4HTrigger();
			 }
		 	break;
		 } //reinput=0:1  antisteal check input: low/high 		
		 nData = strlen((char*)pDest);
		 if( nCmd == 0 ){
		 	sprintf((char*)pDest+nData, "%s %s:%s", TEXT_CHECK, TEXT_INPUT,TEXT_LOW);
		 }
		 else{
		 	sprintf((char*)pDest+nData, "%s %s:%s", TEXT_CHECK, TEXT_INPUT,TEXT_HIGH);
		 }		 
		 nResult = strlen((char*)pDest);
		 TE_SetNeedWriteFlash( );		 
	}	
	return nResult;
}
///////////////////////////////////////////////////////////////
//	终端机器状态
unsigned char PLS_ReadState( unsigned char  *pDest, unsigned char nLangCN ){
	
	short 	 			nTmp = 0;	 
	unsigned char 		nResult = 0;
	unsigned char 		nBuf[40];
	unsigned char 		*pTmp = NULL;
	
	nTmp =  g_stDeviceState.m_nBattery*170 / 20;  
	
	if( nLangCN ){			
		// POWER
		strcpy( (char*)pDest, (char*)TEXT_POWER_CN);
		nResult = strlen( (char*)pDest );
		
		sprintf( (char*)nBuf,  ":%.3fv,", (float)nTmp/1000 );
		nResult += unicodecpy( pDest+nResult, nBuf );
		
		// GPS
		if( GPS_IsPosValid() ){
			pTmp = (unsigned char*)TEXT_LOCKED_CN;
		}
		else{
			pTmp = (unsigned char*)TEXT_UNLOCKED_CN;
		}	
		strcpy((char*)pDest+nResult, (char*)pTmp ); 
		nResult += strlen( (char*)pTmp );		
		
		// ACC
		if( HW_IsAccState() ){
			sprintf((char*)nBuf,",%s:%s,", TEXT_ACC, TEXT_ON );
		}
		else{
			sprintf((char*)nBuf,",%s:%s,", TEXT_ACC, TEXT_OFF );
		}
		nResult += unicodecpy( pDest+nResult, nBuf );
		
		// Door  
		if( TE_IsAntiSteal() == 0 ){
			strcpy( (char*)pDest+nResult, (char*)TEXT_DOOR_CN );
			nResult += strlen( (char*)TEXT_DOOR_CN );		
			if ( HW_IsDoorState() ){						
				sprintf((char*)nBuf, ":%s,", TEXT_OPEN );			
			}
			else{						
				sprintf((char*)nBuf, ":%s,", TEXT_CLOSE );			
			}
			nResult += unicodecpy( pDest+nResult, nBuf );
		}
		
		// 记录设防状态 armed, disarmed
		if ( HW_IsAntiStealState() ) {	
			pTmp = (unsigned char*)TEXT_ARMED_CN;
		}
		else{		
			pTmp = (unsigned char*)TEXT_DISARMED_CN;
		}
		strcpy((char*)pDest+nResult, (char*)pTmp ); 
		nResult += strlen( (char*)pTmp );		
		nResult += unicodecpy( pDest+nResult, (unsigned char*)"," );
		
		// 信号
		strcpy((char*)pDest+nResult, TEXT_GSM_SIGNAL_CN ); 
		nResult += strlen((char*)TEXT_GSM_SIGNAL_CN );
		nResult += unicodecpy( pDest+nResult, (unsigned char*)":" );		
		if ((GSM_GetSignal() >= 21) && (GSM_GetSignal() <= 31)){		
			pTmp = (unsigned char*)TEXT_STRONG_CN;
		}
		else if ((GSM_GetSignal() >= 8) && (GSM_GetSignal() <= 20) ){		
			pTmp = (unsigned char*)TEXT_NORMAL_CN;
		}
		else{ 		
			pTmp = (unsigned char*)TEXT_WEAK_CN;
		}
		strcpy((char*)pDest+nResult, (char*)pTmp ); 
		nResult += strlen( (char*)pTmp );		
	}
	else{
		// POWER
		sprintf((char*)pDest,"%s:%.3fV,", TEXT_POWER, (float)nTmp/1000 );				
		nResult = strlen( (char*)pDest );
		
			// GPS
		if( GPS_IsPosValid() ){
			pTmp = (unsigned char*)TEXT_LOCKED;
		}
		else{
			pTmp = (unsigned char*)TEXT_UNLOCKED;
		}	
		strcpy((char*)pDest+nResult, (char*)pTmp ); 
		nResult += strlen( (char*)pTmp );		

		// ACC
		if( HW_IsAccState() ){
			sprintf((char*)nBuf,",%s:%s,", TEXT_ACC, TEXT_ON );
		}
		else{
			sprintf((char*)nBuf,",%s:%s,", TEXT_ACC, TEXT_OFF );
		}
		strcpy((char*)pDest+nResult, (char*)nBuf ); 
		nResult += strlen( (char*)nBuf );		
		
		// Door
		if( TE_IsAntiSteal() == 0 ){
			if ( HW_IsDoorState() ){	 	
				sprintf((char*)nBuf,"%s:%s,", TEXT_DOOR, TEXT_OPEN);
			}
			else{
				sprintf((char*)nBuf,"%s:%s,", TEXT_DOOR, TEXT_CLOSE);
			}		
			strcpy((char*)pDest+nResult, (char*)nBuf ); 
			nResult += strlen( (char*)nBuf );			
		}

		// 记录设防状态 armed, disarmed
		if ( HW_IsAntiStealState() ) {	 	
			pTmp = (unsigned char*)TEXT_ARMED;
		}
		else{	
			pTmp = (unsigned char*)TEXT_DISARMED;
		}
		strcpy((char*)pDest+nResult, (char*)pTmp );   
		nResult += strlen( (char*)pTmp );			
		
		// GSM信号
		if ((GSM_GetSignal() >= 21) && (GSM_GetSignal() <= 31)){	
			sprintf((char*)nBuf,",%s:%s", TEXT_GSM_SIGNAL, TEXT_STRONG);
		}
		else if ((GSM_GetSignal() >= 8) && (GSM_GetSignal() <= 20) ){	
			sprintf((char*)nBuf,",%s:%s", TEXT_GSM_SIGNAL, TEXT_NORMAL);
		}
		else{ 		
			sprintf((char*)nBuf,",%s:%s", TEXT_GSM_SIGNAL, TEXT_WEAK);
		}		
		strcpy((char*)pDest+nResult, (char*)nBuf ); 
		nResult += strlen( (char*)nBuf );
		
		// GPS信号
		if( (g_stGPSInfo.m_nGPSSignal >= 1) && (g_stGPSInfo.m_nGPSSignal <= 3) ){		//信号差
				
		    sprintf((char*)nBuf,",%s:%s", TEXT_GPS_SIGNAL, TEXT_WEAK);
		}
		else if( (g_stGPSInfo.m_nGPSSignal >= 4) &&	(g_stGPSInfo.m_nGPSSignal <= 5) ){  //信号一般
		
		    sprintf((char*)nBuf,",%s:%s", TEXT_GPS_SIGNAL, TEXT_NORMAL);
		}
		else if ( g_stGPSInfo.m_nGPSSignal > 5 ){   									//信号强
		
			sprintf((char*)nBuf,",%s:%s", TEXT_GPS_SIGNAL, TEXT_STRONG);
		}
		else{																			//没有信号			
			sprintf((char*)nBuf,",%s:%s", TEXT_GPS_SIGNAL, TEXT_WEAK);
		}
		strcpy((char*)pDest+nResult, (char*)nBuf ); 
		nResult += strlen( (char*)nBuf );
		
		// 查询GPRS网络
		nTmp = CMM_QueryNetworkStatus();
		sprintf( (char*)nBuf, ",%s:%d",TEXT_GPRS, nTmp );				
		strcpy((char*)pDest+nResult, (char*)nBuf ); 
		nResult += strlen( (char*)nBuf );
	}
	return nResult;
}
/////////////////////////////////////////////////////////////////////////////
// 	
unsigned char PLS_SetupDefensive( unsigned char  *pDest, unsigned char  *pSrc, unsigned char nLangCN ){
	
	unsigned short		nTmp = 0;	
	unsigned char			nResult = 0;	
	unsigned char			nBuf[10];
			
	if( *pSrc != TEXT_QUERY ){ 
		TE_SetNeedWriteFlash( );
		nTmp = atoi( (char*)pSrc );
		if( nTmp >= 255){
			nTmp =255;		
		}		
		g_stAlarmSetup.m_nDefence = nTmp;
	}	
	if( nLangCN ){	
		strcpy( (char*)pDest, (char*)TEXT_DEFENSIVE_CN);
		nResult = strlen( (char*)pDest );		
		sprintf( (char*)nBuf, ":%d(min)", g_stAlarmSetup.m_nDefence  );	
		nResult += unicodecpy( pDest+nResult, nBuf );
	}
	else{
		sprintf( (char*)pDest, "%s:%d(min)", TEXT_DEFENSIVE, g_stAlarmSetup.m_nDefence  );		
		nResult = strlen( (char*)pDest );
	}
	return nResult;
}	
/////////////////////////////////////////////////////////////////////////////
//停车报警设置
unsigned char PLS_SetupAlarmParking( unsigned char  *pDest, unsigned char  *pSrc, unsigned char nLangCN ){
	
	unsigned short			nTmp;
	unsigned char				nResult = 0;
	unsigned char				nBuf[20];
		
	if( *pSrc != TEXT_QUERY ){	//设置
		TE_SetNeedWriteFlash( );
		nTmp = atoi( (char*)pSrc );
		if( nTmp == 0 ){
			AL_DisableParking( );
		}
		else{
			if ( nTmp >= 255 ){
				nTmp = 255;
			}			
			g_stAlarmSetup.m_nParkingCnt = 0;
			g_stAlarmSetup.m_nParking = nTmp;
			AL_EnableParking();
		}		
	}
	if( nLangCN ){
		sprintf( (char*)pDest, "%s%s", TEXT_PARKING_CN, TEXT_ALARM_CN );
		nResult = strlen( (char*)pDest );
		
		nResult += unicodecpy( pDest+nResult, (unsigned char*)":" );		
		if( AL_IsEnableParking() ){
			sprintf( (char*)nBuf, "%d(min)", g_stAlarmSetup.m_nParking );
			nResult += unicodecpy( pDest+nResult, nBuf );
		}
		else{
			strcpy( (char*)pDest+nResult, TEXT_DISABLE_CN );	
			nResult += strlen( (char*)TEXT_DISABLE_CN );
		}
	}
	else{		
		if( AL_IsEnableParking() ){		
			sprintf( (char*)pDest, "%s %s:%d(min)", TEXT_PARKING, TEXT_ALARM, g_stAlarmSetup.m_nParking  );
		}
		else{
			sprintf( (char*)pDest, "%s %s:%s", TEXT_PARKING, TEXT_ALARM, TEXT_DISABLE );	
		}	
		nResult = strlen( (char*)pDest );		
	}
	return nResult;
}
/////////////////////////////////////////////////////////////////////////////
// 非法开门报警设置	
unsigned char PLS_SetupAlarmAntisteal( unsigned char *pDest, 
																			 unsigned char *pSrc,
																			 unsigned char nLangCN ){
	
	unsigned short	nTmp = 0;
	unsigned char		nResult = 0;
	unsigned char		nBuf[20];
		
	if( *pSrc != TEXT_QUERY ){
		TE_SetNeedWriteFlash( );
		nTmp = atoi( (char*)pSrc );				
		if ( nTmp == 0 ){
			AL_DisableNotDoor( ); 													   
		}
		else{
			if ( nTmp >= 255 ){
				nTmp = 255;
			}
			g_stAlarmSetup.m_nILLDoorOpen = nTmp;
			AL_EnableNotDoor();												   		  
		}
	}	
	
	
	if( nLangCN ){
		
		sprintf( (char*)pDest, "%s%s", TEXT_ANTISTEAL_CN, TEXT_ALARM_CN );
		nResult = strlen( (char*)pDest );
		
		nResult += unicodecpy( pDest+nResult, (unsigned char*)":" );		
		if( AL_IsEnableNotDoor() ){
			sprintf( (char*)nBuf, "%d(s)", g_stAlarmSetup.m_nILLDoorOpen  );
			nResult += unicodecpy( pDest+nResult, nBuf );
		}
		else{
			strcpy( (char*)pDest+nResult, TEXT_DISABLE_CN );	
			nResult += strlen( (char*)TEXT_DISABLE_CN );
		}
	}
	else{
		if( AL_IsEnableNotDoor() ){		
			sprintf( (char*)pDest, "%s %s:%d(s)", TEXT_ANTISTEAL, TEXT_ALARM, g_stAlarmSetup.m_nILLDoorOpen  );	
		}
		else{
			sprintf( (char*)pDest, "%s %s:%s", TEXT_ANTISTEAL, TEXT_ALARM, TEXT_DISABLE );	
		}	
		nResult = strlen( (char*)pDest );
	}	
	return nResult;
}	
/////////////////////////////////////////////////////////////////////////////
// 超速报警设置
unsigned char PLS_SetupAlarmOverspeed( unsigned char *pDest, 
																			 unsigned char *pSrc,
																			 unsigned char nLangCN){
	
	unsigned short	nTmp = 0;
	unsigned char		nResult = 0;
	unsigned char		nBuf[20];
		
	if( *pSrc != TEXT_QUERY ){	
		TE_SetNeedWriteFlash( ); 
		nTmp = atoi((char*)pSrc );		
		if( nTmp <=  MIN_OVERSPEED ){			//关闭超速报警			
			AL_DisableOverSpeed( );											
		}
		else{
			AL_EnableOverSpeed( );
			g_stAlarmSetup.m_nOverspeed = nTmp;			
		}
	}	
	
	if( nLangCN ){
		sprintf( (char*)pDest, "%s%s", TEXT_OVERSPEED_CN, TEXT_ALARM_CN );
		nResult = strlen( (char*)pDest );
		
		nResult += unicodecpy( pDest+nResult, (unsigned char*)":" );		
		
		if( AL_IsEnableOverSpeed() ){
			sprintf( (char*)nBuf, "%d(km/h)", g_stAlarmSetup.m_nOverspeed  );
			nResult += unicodecpy( pDest+nResult, nBuf );
		}
		else{
			strcpy( (char*)pDest+nResult, TEXT_DISABLE_CN );	
			nResult += strlen( (char*)TEXT_DISABLE_CN );
		}		
		
	}
	else{
		if( AL_IsEnableOverSpeed() ){		
			sprintf( (char*)pDest, "%s %s:%dkm/h", TEXT_OVERSPEED, TEXT_ALARM, g_stAlarmSetup.m_nOverspeed  );
		}
		else{
			sprintf( (char*)pDest, "%s %s:%s", TEXT_OVERSPEED, TEXT_ALARM, TEXT_DISABLE );	
		}	
		nResult = strlen( (char*)pDest );
	}
	return nResult;					
}
/////////////////////////////////////////////////////////////////////////////
//  拖车报警
unsigned char PLS_SetupAlarmTow( unsigned char *pDest, 
																 unsigned char *pSrc,
																 unsigned char nLangCN){		
	unsigned char			nResult = 0;
					  	
	if( *pSrc != TEXT_QUERY ){
		TE_SetNeedWriteFlash( );		
		if( *pSrc == '0' ){
			AL_DisableEnTow();
		}	
		else{ 
		 	AL_EnableTow( ); 
		}			 
	}	
	
	if( nLangCN ){
		if( AL_IsEnableTow() ){		
			sprintf( (char*)pDest, "%s%s%s", TEXT_TOWING_CN, TEXT_ALARM_CN, TEXT_ENABLE_CN  );		
		}
		else{
			sprintf( (char*)pDest, "%s%s%s", TEXT_TOWING_CN, TEXT_ALARM_CN, TEXT_DISABLE_CN );		
		}		
		nResult = strlen( (char*)pDest );	   
	}
	else{
		if( AL_IsEnableTow() ){		
			sprintf( (char*)pDest, "%s %s:%s", TEXT_TOWING, TEXT_ALARM, TEXT_ENABLE  );		
		}
		else{
			sprintf( (char*)pDest, "%s %s:%s", TEXT_TOWING, TEXT_ALARM, TEXT_DISABLE );	
		}	
		nResult = strlen( (char*)pDest );	  
	}	
	return nResult;
}
//////////////////////////////////////////////////////////////////////////////
//  
unsigned char PLS_CtrlOil( unsigned char *pDest, 
													 unsigned char nFlag,
													 unsigned char nLangCN ){		
	
	unsigned char				nResult;
	unsigned char				*pTmp = NULL;
	
	nResult = PLS_ComposePosition( pDest, nLangCN );
	TE_SetNeedWriteFlash( );	
	if( nFlag == 0 ){
		HW_CTRLOIL_CUTOFF();
		GSM_SetOilState();
		HW_SetOilState();
		if( nLangCN ){	
			pTmp = (unsigned char*)TEXT_FUEL_OFF_CN;		
		}
		else{
			pTmp = (unsigned char*)TEXT_FUEL_OFF;		
		}		
	}
	else{
		HW_CTRLOIL_RESUME();
		GSM_ClrOilState();
		HW_ClrOilState();
		if( nLangCN ){	
			pTmp = (unsigned char*)TEXT_FUEL_ON_CN;	
		}
		else{			
			pTmp = (unsigned char*)TEXT_FUEL_ON;		
		}
	}	
	strcpy( (char*)pDest+nResult, (char*)pTmp);		
	nResult += strlen( (char*)pTmp );
	return nResult; 
}
//////////////////////////////////////////////////////////////////////////////
//	中控锁控制 
unsigned char PLS_SetupLock(  unsigned char *pDest, 
							  unsigned char *pSrc,
							  unsigned char nLangCN ){	
							  
	unsigned char				nResult = 0;
		
	if( atoi( (char*)pSrc) == 0 ){  //开锁
			
		TE_SetNeedUnLockFlag();
		if( nLangCN ){	
			sprintf( (char*)pDest, "%s%s", TEXT_DOOR_CN, TEXT_UNLOCKED_CN );
		}
		else{
			sprintf( (char*)pDest, "%s %s", TEXT_DOOR, TEXT_UNLOCKED );	
		}	
	}
	else{							//锁
		TE_SetNeedLockFlag();
		if( nLangCN ){	
			sprintf( (char*)pDest, "%s%s", TEXT_DOOR_CN, TEXT_LOCKED_CN );
		}
		else{
			sprintf( (char*)pDest, "%s %s", TEXT_DOOR, TEXT_LOCKED );		
		}
	}	
	nResult = strlen((char*)pDest);
	return nResult;												 
}
/////////////////////////////////////////////////////////////////////////////
//	设置号码
unsigned char PLS_SetupSlaveNo( unsigned char  *pDest, 
																unsigned char  *pSrc, 
																unsigned char  nFlag,
																unsigned char  nLangCN ){	

//	unsigned char   nLen = 0;
	unsigned char	*pTmp= NULL;
	unsigned char 	nResult = 0;
	unsigned char	nBuf[10];
	
	switch( nFlag ){
	case 1:
		pTmp = g_stGSMSetup.m_strAuthNo1;
		break;
	case 2:
		pTmp = g_stGSMSetup.m_strAuthNo2;
		break;
	case 3:
		pTmp = g_stGSMSetup.m_strAuthNo3;
		break;
	case 4:
		pTmp = g_stGSMSetup.m_strAuthNo4;
		break;
	case 5:
	default:
		pTmp = g_stGSMSetup.m_strAuthNo5;
		break;
	}
	if( *pSrc != TEXT_QUERY ){	
	
		memset( pTmp, 0x00, TELNUMBER_LENGTH+1 );
		strncpy( (char*)pTmp, (char*)pSrc, TELNUMBER_LENGTH );
				
		TE_SetNeedWriteFlash( );
	}	
	if( nLangCN ){	
		sprintf( (char*)pDest, "%s%s", TEXT_AUTHORIZED_CN, TEXT_NO_CN );
		nResult = strlen((char*)pDest);
		
		sprintf( (char*)nBuf, "%d:", nFlag );
		nResult += unicodecpy( pDest+ nResult, nBuf );	
		nResult += unicodecpy( pDest+ nResult, pTmp );	
	}
	else{
		sprintf( (char*)pDest, "%s%d %s:%s", TEXT_AUTHORIZED, nFlag, TEXT_NO, pTmp);		
		nResult = strlen((char*)pDest );
	}
	return nResult;
}
/////////////////////////////////////////////////////////////////////////////
//设置短信中心号码
unsigned char	PLS_SetupSMCNo( unsigned char  *pDest, unsigned char  *pSrc, unsigned char nLangCN ){
	unsigned char				nLen = 0;
	unsigned char				nResult = 0;
	
	if( *pSrc != TEXT_QUERY ){	
	
		nLen = strlen( (char*)pSrc );
		if( nLen >= 6 ){			
			memset( g_stGSMSetup.m_strSMCNo, 0x00, TELNUMBER_LENGTH+1 );
			strncpy( (char*)g_stGSMSetup.m_strSMCNo,  (char*)pSrc, TELNUMBER_LENGTH );
			TE_SetNeedWriteFlash( );
		}
	}	
	if( nLangCN ){
		sprintf( (char*)pDest, "%s%s", TEXT_SMC, TEXT_NO );
		nResult = strlen( (char*)pDest );
		nResult += unicodecpy( pDest+nResult, (unsigned char*)":");
		nResult += unicodecpy( pDest+nResult, g_stGSMSetup.m_strSMCNo);		
	}
	else{
		sprintf( (char*)pDest, "%s %s:%s", TEXT_SMC, TEXT_NO, g_stGSMSetup.m_strSMCNo);
		nResult = strlen( (char*)pDest );
	}
	return nResult;
}
/////////////////////////////////////////////////////////////////////////////
//	更改密码
unsigned char	PLS_SetupPassword( 	unsigned char  *pDest, 
									unsigned char  *pSrc, 
									unsigned char  nColon,
									unsigned char  nLangCN ){
	
//	unsigned char	nLen = 0;
	unsigned char	*pTmp = NULL;
	unsigned char	nResult = 0;
		
	pTmp = (unsigned char*)strchr((char*)pSrc, nColon );
	if( pTmp == NULL ){
		if( nLangCN ){			
			strcpy( (char*)pDest, TEXT_PARA_ERROR_CN );		
		}
		else{
			strcpy( (char*)pDest, TEXT_PARA_ERROR );				
		}
		nResult = strlen((char*)pDest );
	}	
	else{
		*pTmp = 0x00;
		if( strcmp( (char*)pSrc, (char*)g_stGSMSetup.m_strPassword ) == 0x00 ){
			pTmp++;
			
			memset( g_stGSMSetup.m_strPassword, 0x00, PASSWORD_LENGTH+1 );
			strncpy( (char*)g_stGSMSetup.m_strPassword, (char*)pTmp, PASSWORD_LENGTH );	
			
			TE_SetNeedWriteFlash( );								
			if( nLangCN ){
				sprintf( (char*)pDest, "%s", TEXT_PASSWORD_CN );	
				nResult = strlen((char*)pDest );	
				nResult += unicodecpy( pDest+nResult, (unsigned char*)":");	
				nResult += unicodecpy( pDest+nResult, g_stGSMSetup.m_strPassword );				
			}
			else{
				sprintf( (char*)pDest, "%s:%s", TEXT_PASSWORD, g_stGSMSetup.m_strPassword  );	
				nResult = strlen((char*)pDest );
			}
		}
		else{
			if( nLangCN ){			
				strcpy( (char*)pDest, TEXT_PSD_ERROR_CN );		
			}
			else{
				strcpy( (char*)pDest, TEXT_PSD_ERROR );				
			}		
			nResult = strlen((char*)pDest );
		}		
	}
	return nResult;									  	  
}
/////////////////////////////////////////////////////////////////////////////
//
unsigned char PLS_IsIPAddr( unsigned char *pSrc ){
	
	unsigned char 		nLen = 0;
	unsigned char		*pTmp = NULL;
				
	pTmp = pSrc;
	
	while( 1 ){
		
		pTmp = (unsigned char*)strchr( (char*)pSrc, '.' );
		if( pTmp == NULL ){
			break;
		}
		pSrc = pTmp + 1;
		nLen++;
	}	
	if( nLen == 3 ){
		return 1;
	}
	else{
		return 0;
	}
}
/////////////////////////////////////////////////////////////////////////////
//设置IP地址
unsigned char PLS_SetupServer( unsigned char  *pDest, unsigned char  *pSrc, unsigned char nColon, unsigned char nLangCN ){
					  	
//	unsigned char	nLen = 0;			    
	unsigned char	*pTmp = NULL;
	
	if( *pSrc != TEXT_QUERY ){
		pTmp = (unsigned char*)strchr((char*)pSrc, nColon);
		if( pTmp == NULL ){
				strcpy( (char*)pDest, TEXT_PARA_ERROR );				
		}
		else{
			*pTmp = 0x00;
			pTmp++;
			g_stGSMSetup.m_nServerPort = atoi( (char*)pTmp );			
			if( PLS_IsIPAddr( pSrc ) ){
				
				memset( g_stGSMSetup.m_strServerIP, 0x00, SERVERIP_LENGTH+1 );
				strncpy((char*)g_stGSMSetup.m_strServerIP, (char*)pSrc , SERVERIP_LENGTH );
				sprintf( (char*)pDest, "%s:%s:%d", TEXT_SERVER, g_stGSMSetup.m_strServerIP, g_stGSMSetup.m_nServerPort );	
			}			
			else{
				memset( g_stGSMSetup.m_strServerDomain, 0x00, SERVERDOMAIN_LENGTH+1 );
				strncpy((char*)g_stGSMSetup.m_strServerDomain, (char*)pSrc , SERVERDOMAIN_LENGTH );
				sprintf( (char*)pDest, "%s:%s:%d", TEXT_SERVER, g_stGSMSetup.m_strServerDomain, g_stGSMSetup.m_nServerPort );				
			}			
			IP_SetConnectNG( );
			TE_ClrLoginSuccess();
			TE_SetNeedWriteFlash( );			
		}
	}
	else{
		sprintf( (char*)pDest, "%s:%s:%d", TEXT_SERVER, g_stGSMSetup.m_strServerIP, g_stGSMSetup.m_nServerPort );
	}
	return strlen((char*)pDest);
}
/////////////////////////////////////////////////////////////////////////////
//	设置域名 *DNSR=www.track-earth.com#
unsigned char PLS_SetupServerDNSR( unsigned char  *pDest, unsigned char  *pSrc, unsigned char nLangCN  ){

	unsigned char	nLen=0;
	
	if( pSrc == NULL ){	
		strcpy( (char*)pDest, TEXT_PARA_ERROR );
	}
	else{	
		nLen = strlen( (char*)pSrc);
		if( nLen == 0 ){			
			memset( g_stGSMSetup.m_strServerDomain, 0x00,SERVERDOMAIN_LENGTH+1 );
			sprintf( (char*)pDest, "%s %s", TEXT_DNSR, TEXT_DISABLE);		
		}
		else{
			if( pSrc[0] != TEXT_QUERY ){
							
				memset( g_stGSMSetup.m_strServerDomain, 0x00,SERVERDOMAIN_LENGTH+1 );				
				strncpy( (char*)g_stGSMSetup.m_strServerDomain, (char*)pSrc, SERVERDOMAIN_LENGTH );							
			}
			sprintf( (char*)pDest, "%s:%s", TEXT_DNSR,
									 		(char*)g_stGSMSetup.m_strServerDomain);	
		}		
		TE_SetNeedWriteFlash( );
	}	
	return strlen((char*)pDest);	
}
/////////////////////////////////////////////////////////////////////////////
//	设置APN网关 APN:User:Psd
unsigned char  PLS_SetupAPN( unsigned char  *pDest, unsigned char  *pSrc, unsigned char nLangCN  ){

//	unsigned char	nLen = 0;
	unsigned char	*pch = NULL;
	unsigned char   *pch1 = NULL;
	
	if( *pSrc != TEXT_QUERY ){
	
		TE_SetNeedWriteFlash( );
		GSM_SetNeedReset();	
		pch = pSrc;
		pch1 = (unsigned char*)strchr((char*)pch, ':');
		if( pch1 == NULL ){
		
			memset( g_stGSMSetup.m_strGateway, 0x00, SERVERDOMAIN_LENGTH+1 );		
			strncpy( (char*)g_stGSMSetup.m_strGateway,(char*) pSrc, SERVERDOMAIN_LENGTH );
		}
		else{
			*pch1 = 0x00;
			//APN
			memset(g_stGSMSetup.m_strGateway, 0x00, SERVERDOMAIN_LENGTH+1 );
			strncpy( (char*)g_stGSMSetup.m_strGateway,(char*)pch, SERVERDOMAIN_LENGTH );
			
			pch = pch1 + 1;
			// USER
			pch1 = (unsigned char*)strchr( (char*)pch, ':');
			if( pch1 != NULL ){
				*pch1 = 0x00;
				memset( g_stGSMSetup.m_strUser, 0x00, TELNUMBER_LENGTH+1 );
				strncpy( (char*)g_stGSMSetup.m_strUser, (char*)pch,TELNUMBER_LENGTH ); 
				pch = pch1+ 1;
				// USER PSD
				memset( g_stGSMSetup.m_strPsd, 0x00, TELNUMBER_LENGTH + 1 );
				strncpy( (char*)g_stGSMSetup.m_strPsd, (char*)pch, TELNUMBER_LENGTH );
				
			}
		}	
	}						
	sprintf((char*)pDest, "%s:%s, %s:%s:%s", TEXT_APN, 
											 g_stGSMSetup.m_strGateway,
											 TEXT_LOGIN_INFO,
											 g_stGSMSetup.m_strUser,
											 g_stGSMSetup.m_strPsd);
 	return strlen((char*)pDest);
}
/////////////////////////////////////////////////////////////////////////////
//设置上网用户名和密码
unsigned char PLS_SetupAPNUserPsd( unsigned char  *pDest, unsigned char  *pSrc, unsigned char nColon, unsigned char nLangCN  ){

//	unsigned char	nLen = 0;
	unsigned char	*pTmp = NULL;
	
	pTmp = (unsigned char*)strchr( (char*)pSrc, nColon );
	if( pTmp == NULL ){	
		strcpy( (char*)pDest, TEXT_PARA_ERROR );				
	}
	else{
		*pTmp = 0;
		
		memset( g_stGSMSetup.m_strUser, 0x00, TELNUMBER_LENGTH+1 );
		strncpy( (char*)g_stGSMSetup.m_strUser, (char*)pSrc, TELNUMBER_LENGTH );
		
		pSrc = pTmp + 1;
		
		memset( g_stGSMSetup.m_strPsd, 0x00, TELNUMBER_LENGTH+1 );
		strncpy( (char*)g_stGSMSetup.m_strPsd, (char*)pSrc, TELNUMBER_LENGTH );
				
		sprintf( (char*)pDest, "%s %s:%s:%s", 
											  TEXT_APN, 
										      TEXT_LOGIN_INFO,
										      g_stGSMSetup.m_strUser,
											  g_stGSMSetup.m_strPsd );				
		TE_SetNeedWriteFlash( );
		GSM_SetNeedReset();									
	}
	return strlen((char*)pDest);
}
/////////////////////////////////////////////////////////////////////////////
// 工作模式
unsigned char PLS_SetupWorkMode( unsigned char  *pDest,unsigned char  *pSrc,unsigned char nLangCN ){
		
	if( pSrc[0] == '1'){		
		
		TE_SetNeedWriteFlash();	
		GSM_SetToSMSCommMode();
		COM_ClrSMSFeedbackUseJave();			
		if( (pSrc[1] == 'a') ||	(pSrc[1] == 'A' ) ){
			COMM_ClrSMSFeedbackUseProtocol();
		}
		else if ( (pSrc[1] == 'b') || (pSrc[1] == 'B')){
			COMM_SetSMSFeedbackUseProtocol();
		}					
		g_stGSMSetup.m_nCBMode  = CB_STOP;
		CMM_IPClose( );
	}
	else if( pSrc[0] == '2' ){	
		
		TE_SetNeedWriteFlash();
		GSM_SetToGPRSCommMode();
		if( (pSrc[1] == 'a') ||	(pSrc[1] == 'A' ) ){
			IP_SetTCPCommMode();
		}
		else if ( (pSrc[1] == 'b') || (pSrc[1] == 'B')){
			IP_SetUDPCommMode();
		}		
		IP_SetConnectNG();			
		if ( g_stGSMSetup.m_nCBMode == CB_STOP ){	
			g_stGSMSetup.m_nCBMode = CB_BYTIME;
		}	
	}
	if( GSM_IsGPRSCommMode() ){
		if( IP_IsTCPCommMode() ){
			sprintf((char*)pDest, "%s:%s", TEXT_WORKMODE, TEXT_TCP );		
		}
		else{
			sprintf((char*)pDest, "%s:%s", TEXT_WORKMODE, TEXT_UDP );		
		}		
	}
	else{
		if( COMM_IsSMSFeedbackUseProtocol() ){
			sprintf((char*)pDest, "%s:%s SB", TEXT_WORKMODE, TEXT_SMS );
		}
		else{
			sprintf((char*)pDest, "%s:%s SA", TEXT_WORKMODE, TEXT_SMS );		
		}
	}	
	return strlen((char*)pDest);
}
/////////////////////////////////////////////////////////////////////////////
//	短信转发功能 *Forward=10086:CXCY#	  
unsigned char PLS_ForwardMsg( unsigned char  *pCaller,unsigned char  *pDest, unsigned char  *pSrc, unsigned char nLangCN ){

//	unsigned char	nLen = 0;
	unsigned char	*pTmp = NULL;
		
	pTmp = (unsigned char*)strchr( (char*)pSrc, TEXT_COLON );
	if ( pTmp == NULL ){
		if( nLangCN ){			
			strcpy( (char*)pDest, TEXT_PARA_ERROR_CN );		
		}
		else{
			strcpy( (char*)pDest, TEXT_PARA_ERROR );				
		}
	}
	else{
		strcpy((char*)g_stGSMState.m_strForward, (char*)pCaller );
		*pTmp = 0;
				
		memset( g_stGSMState.m_strFWDNo, 0x00, TELNUMBER_LENGTH+1 );
		strncpy( (char*)g_stGSMState.m_strFWDNo, (char*)pSrc, TELNUMBER_LENGTH );
			
		strcpy( (char*)pCaller, (char*)g_stGSMState.m_strFWDNo);
		strcpy( (char*)pDest, (char*)pTmp+1 );	
	}
	return strlen((char*)pDest);
}
/////////////////////////////////////////////////////////////////////////////
//	执行SIM指令 *EXCUTE=*888##
unsigned char PLS_ExecuteExtCmd( unsigned char  *pDest, unsigned char  *pSrc, unsigned char nLangCN ){									
	
	unsigned char	*pch = NULL;
		
	if ( GSM_ExecCUSD( pDest, pSrc, GPRS_GPRSDATA_CHECK ) == GSM_SUCCEED ){		
					
		 pch = (unsigned char*)strchr((char*)pDest, '"' );	
		 if ( pch != NULL ){
		 	 pch += 1;
		 	 memcpy( pDest, pch, strlen( (char*)pch ));
		 	 pch = (unsigned char*)strchr((char*)pDest, '"');
		 	 if ( pch != NULL ){
		 	 	*pch = 0x00;
		 	 }
		 	 if( strlen((char*)pDest) >= 160){
		 	 	pDest[160] = 0x00;
		 	 }
		 }
	}
	else{
		sprintf((char*)pDest, "%s", TEXT_EXEC_ERROR );
	}
	return strlen((char*)pDest);
}
/////////////////////////////////////////////////////////////////////////////
//  设置设备的基本参数信息 *setup=NaterNo:work:APN:user:psd:IP:Port#
unsigned char PLS_SetupDeviceInfo( unsigned char	*pDest, unsigned char	*pSrc ){

	unsigned char 	*pBuf = NULL;
	unsigned char   nFlag = 0;
	unsigned char   bExit = 0;  


	nFlag = 0; 
	bExit = 0;

	while( bExit == 0 ){
		pBuf = (unsigned char*)strchr( (char*)pSrc, ':' );
		if( pBuf != NULL ){
			*pBuf = 0;
		}
		else{
			bExit = 1;
		}
		switch( nFlag ){
		// 中心号码
		case 0:
			memset(g_stGSMSetup.m_strMaterNo, 0x00, SERVERDOMAIN_LENGTH+1);
			strncpy( (char*)g_stGSMSetup.m_strMaterNo,(char*)pSrc, TELNUMBER_LENGTH);
		break;
		// 工作模式
		case 1: 
			if( *pSrc == '0' ){ 
				COM_ClrSMSFeedbackUseJave();
				GSM_SetToSMSCommMode();
				return 0;
			}
			else if( *pSrc == '1'){
				GSM_SetToGPRSCommMode();
				IP_SetUDPCommMode();
			}
		break;
		// APN
		case 2:
			memset(g_stGSMSetup.m_strGateway, 0x00, SERVERDOMAIN_LENGTH+1);
			strncpy( (char*)g_stGSMSetup.m_strGateway, (char*)pSrc, SERVERDOMAIN_LENGTH);
		break;
		// User Name
		case 3:
			memset(g_stGSMSetup.m_strUser, 0x00, TELNUMBER_LENGTH+1);
			strncpy( (char*)g_stGSMSetup.m_strUser, (char*)pSrc, TELNUMBER_LENGTH);
		break;
		// User Psd
		case 4:
			memset(g_stGSMSetup.m_strPsd, 0x00, TELNUMBER_LENGTH+1);
			strncpy( (char*)g_stGSMSetup.m_strPsd,(char*)pSrc, TELNUMBER_LENGTH);
		break;
		// Server IP
		case 5:
			memset(g_stGSMSetup.m_strServerIP, 0x00, SERVERIP_LENGTH+1);
			strncpy( (char*)g_stGSMSetup.m_strServerIP, (char*)pSrc, SERVERIP_LENGTH);	
		break;
		// Server Port
		case 6:
			g_stGSMSetup.m_nServerPort = atoi( (char*)pSrc );
			g_stGSMSetup.m_nCBMode = CB_BYTIME;
			break;
		} 
		nFlag++;
		if( pBuf != NULL ){ 
			pSrc = pBuf+1; 
		} 
	}
	GSM_SetNeedReset();
	
	return 0;
}
/////////////////////////////////////////////////////////////////////////////
// 
unsigned char PLS_ComposePosition( unsigned char *pDest, unsigned char nLangCN ){
	
	unsigned char			nBuf[40];
	unsigned char			nResult = 0;	
	int						nLac = 0;
	int						nID  = 0;	
	
	
	if( g_stGPSInfo.m_bPosValid != 1 ){
		CMM_QueryNetworkLac( &nLac, &nID );
		sprintf( (char*)nBuf, 	"%.6f,%.6f,%dkm/h,%d,%d ", (double)GPS_GetLatitude( )/3600000,
												 	       (double)GPS_GetLongitude()/3600000,
											   	 	       GPS_GetMilimetre(),
											   	 	       nLac, 
											   	 	       nID );	
    }
    else{
    	sprintf( (char*)nBuf, 	"%.6f,%.6f,%dkm/h ", (double)GPS_GetLatitude( )/3600000,
												   	 (double)GPS_GetLongitude()/3600000,
												   	 GPS_GetMilimetre());
		
    }												   	
								   	
	
	if( nLangCN ){
		nResult = unicodecpy( pDest, nBuf );
	}
	else{		
        strcpy( (char*)pDest, (char*)nBuf );													     
		nResult = strlen( (char*)pDest );		
	}											   	
	return nResult;	
}
/////////////////////////////////////////////////////////////////////////////
// 组织位置信息
unsigned char PLS_ComposeUploadData( unsigned char	*pDest, unsigned char *pCodeType ){

	unsigned char 		nResult;
	unsigned char		nLangCN = 0;
	unsigned short		nTmp = 0;
	unsigned char		nBuf[20];	
	
	if( GSM_IsLangState() ){
		nLangCN = 1;
		*pCodeType = 2;		
	}
	else{
		nLangCN = 0;
		*pCodeType = 0;
	}	
	
	if( COM_IsSMSFeedbackUseJave() && (nLangCN == 0) ){
	
		sprintf( (char*)pDest, "%s:", g_stGSMSetup.m_strUID );
		nResult = strlen( (char*)pDest );
		GPS_GetGPRMCData( pDest+nResult );
		nResult = strlen( (char*)pDest );
	}
	else{
		nResult = PLS_ComposePosition( pDest, nLangCN );
	}
																     		  
	if( nLangCN ){
			
		if( AL_IsSOS( ) ){
			nResult += unicodecpy( pDest+nResult, (unsigned char*)"|" );
			strcpy( (char*)pDest+nResult, TEXT_SOS_CN );
			nResult += strlen( (char*)TEXT_SOS_CN );	
		}
		
		if( AL_IsOverspeed( ) ){
			nResult += unicodecpy( pDest+nResult, (unsigned char*)"|" );
			strcpy( (char*)pDest+nResult, TEXT_OVERSPEED_CN );
			nResult += strlen( (char*)TEXT_OVERSPEED_CN );	
		}
		
		if( AL_IsParking( ) ){
			nResult += unicodecpy( pDest+nResult, (unsigned char*)"|" );
			strcpy( (char*)pDest+nResult, TEXT_PARKING_CN );
			nResult += strlen( (char*)TEXT_PARKING_CN );
		}
		
		if( AL_IsTow( ) ){
			nResult += unicodecpy( pDest+nResult, (unsigned char*)"|" );
			strcpy( (char*)pDest+nResult, TEXT_TOWING_CN );
			nResult += strlen( (char*)TEXT_TOWING_CN );
		}
		
		if( AL_IsEnterGEOFence( ) ){
			nResult += unicodecpy( pDest+nResult, (unsigned char*)"|" );
			strcpy( (char*)pDest+nResult, TEXT_INGEOFENCE_CN );
			nResult += strlen( (char*)TEXT_INGEOFENCE_CN );
		}
		
		if ( AL_IsLeaveGEOFence() ){
			nResult += unicodecpy( pDest+nResult, (unsigned char*)"|" );
			strcpy( (char*)pDest+nResult, TEXT_OUTGEOFENCE_CN );
			nResult += strlen( (char*)TEXT_OUTGEOFENCE_CN );
		} 	
		
		if ( AL_IsPowerOff() ){
			nResult += unicodecpy( pDest+nResult, (unsigned char*)"|" );
			strcpy( (char*)pDest+nResult, TEXT_POWEROFF_CN );
			nResult += strlen( (char*)TEXT_POWEROFF_CN );
		}	
		
		if ( AL_IsLowPower() ){
			nResult += unicodecpy( pDest+nResult, (unsigned char*)"|" );
			
			strcpy( (char*)pDest+nResult, (char*)TEXT_POWER_CN );
			nResult += strlen( (char*)TEXT_POWER_CN );
			
			nTmp =  g_stDeviceState.m_nBattery*170/20;  
			sprintf( (char*)nBuf, ":%.3fv ", (float)nTmp/1000 );
			
			nResult += unicodecpy( pDest+nResult, nBuf );
			
			strcpy( (char*)pDest+nResult, TEXT_LOW_POWER_CN );
			nResult += strlen( (char*)TEXT_LOW_POWER_CN );		
		}
		
		if ( AL_IsNotDoor( ) ){
			nResult += unicodecpy( pDest+nResult, (unsigned char*)"|" );
			strcpy( (char*)pDest+nResult, TEXT_ANTISTEAL_CN );
			nResult += strlen( (char*)TEXT_ANTISTEAL_CN );
		}	
			
		if ( AL_IsTireDrive( ) ){
			nResult += unicodecpy( pDest+nResult, (unsigned char*)"|" );
			strcpy( (char*)pDest+nResult, TEXT_TIREDDRIVE_CN );
			nResult += strlen( (char*)TEXT_TIREDDRIVE_CN );
		}
		
		if ( AL_IsCustom1() ){
			nResult += unicodecpy( pDest+nResult, (unsigned char*)"|" );
			strcpy( (char*)pDest+nResult, TEXT_CUSTOME_CN );
			nResult += strlen( (char*)TEXT_CUSTOME_CN );
			nResult += unicodecpy( pDest+nResult, (unsigned char*)"1" );
		}	
		if ( AL_IsCustom2() ){
			nResult += unicodecpy( pDest+nResult, (unsigned char*)"|" );
			strcpy( (char*)pDest+nResult, TEXT_CUSTOME_CN );
			nResult += strlen( (char*)TEXT_CUSTOME_CN );
			nResult += unicodecpy( pDest+nResult, (unsigned char*)"2" );
		}	
		if ( AL_IsCustom3() ){
			nResult += unicodecpy( pDest+nResult, (unsigned char*)"|" );
			strcpy( (char*)pDest+nResult, TEXT_CUSTOME_CN );
			nResult += strlen( (char*)TEXT_CUSTOME_CN );
			nResult += unicodecpy( pDest+nResult, (unsigned char*)"3" );
		}	
		if ( AL_IsCustom4() ){
			nResult += unicodecpy( pDest+nResult, (unsigned char*)"|" );
			strcpy( (char*)pDest+nResult, TEXT_CUSTOME_CN );
			nResult += strlen( (char*)TEXT_CUSTOME_CN );
			nResult += unicodecpy( pDest+nResult, (unsigned char*)"4" );
		}
		if ( AL_IsACCAlarm() ){
			nResult += unicodecpy( pDest+nResult, (unsigned char*)"|" );			
			nResult += unicodecpy( pDest+nResult, (unsigned char*)TEXT_ACC );
		}
		if ( AL_IsNoGPSAlarm() ){
			nResult += unicodecpy( pDest+nResult, (unsigned char*)"|" );
			strcpy( (char*)pDest+nResult, TEXT_UNLOCKED_CN );
			nResult += strlen( (char*)TEXT_UNLOCKED_CN );
		}		
		if ( AL_IsCrashAlarm() ){
			nResult += unicodecpy( pDest+nResult, (unsigned char*)"|" );
			strcpy( (char*)pDest+nResult, TEXT_CRASH_CN );
			nResult += strlen( (char*)TEXT_CRASH_CN );
		}
		if( TE_HaveAlarm() ){
			strcpy( (char*)pDest+nResult, TEXT_ALARM_CN );
			nResult += strlen( (char*)TEXT_ALARM_CN );
		}		
	}
	else{
		if( AL_IsSOS( ) ){
			sprintf( (char*)pDest+nResult, ",%s", TEXT_SOS );
			nResult = strlen( (char*)pDest );	
		}		
		if( AL_IsOverspeed( ) ){
			sprintf( (char*)pDest+nResult, ",%s", TEXT_OVERSPEED );
			nResult = strlen( (char*)pDest );	
		}		
		if( AL_IsParking( ) ){
			sprintf( (char*)pDest+nResult, ",%s", TEXT_PARKING );
			nResult = strlen( (char*)pDest );	
		}		
		if( AL_IsTow( ) ){
			sprintf( (char*)pDest+nResult, ",%s", TEXT_TOWING );
			nResult = strlen( (char*)pDest );	
		}		
		if( AL_IsEnterGEOFence( ) ){
			sprintf( (char*)pDest+nResult, ",%s-%s", TEXT_IN, TEXT_GEOFENCE );
			nResult = strlen( (char*)pDest );	
		}		
		if ( AL_IsLeaveGEOFence() ){ 
			sprintf( (char*)pDest+nResult, ",%s-%s", TEXT_OUT, TEXT_GEOFENCE );
			nResult = strlen( (char*)pDest );	
		} 	
		if ( AL_IsPowerOff() ){
			sprintf( (char*)pDest+nResult, ",%s",  TEXT_POWEROFF );
			nResult = strlen( (char*)pDest );	
		}			
		if ( AL_IsLowPower() ){
			nTmp =  g_stDeviceState.m_nBattery*170/20;  
			sprintf( (char*)pDest+nResult, ",%s:%.3fv,%s", TEXT_POWER, (float)nTmp/1000, TEXT_LOW_POWER );
			nResult = strlen( (char*)pDest );	
		}		
		if ( AL_IsNotDoor( ) ){
			sprintf( (char*)pDest+nResult, ",%s", TEXT_ANTISTEAL );
			nResult = strlen( (char*)pDest );	
		}				
		if ( AL_IsTireDrive( ) ){
			sprintf( (char*)pDest+nResult, ",%s", TEXT_TIREDDRIVE );
			nResult = strlen( (char*)pDest );	
		}		
		if ( AL_IsCustom1() ){
			sprintf( (char*)pDest+nResult, ",%s", g_stCustomSetup.m_nCustom1 );
			nResult = strlen( (char*)pDest );	
		}	
		if ( AL_IsCustom2() ){
			sprintf( (char*)pDest+nResult, ",%s",  g_stCustomSetup.m_nCustom2 );
			nResult = strlen( (char*)pDest );	
		}	
		if ( AL_IsCustom3() ){ 
			sprintf( (char*)pDest+nResult, ",%s",  g_stCustomSetup.m_nCustom3 );
			nResult = strlen( (char*)pDest );	
		}	
		if ( AL_IsCustom4() ){
			sprintf( (char*)pDest+nResult, ",%s",  g_stCustomSetup.m_nCustom4 );
			nResult = strlen( (char*)pDest );	
		}
		if ( AL_IsACCAlarm() ){
			if( HW_IsAccState() ){
				sprintf( (char*)pDest+nResult, ",%s %s", TEXT_ACC, TEXT_OPEN  );
			}
			else{
				sprintf( (char*)pDest+nResult, ",%s %s", TEXT_ACC, TEXT_CLOSE  );
			}
			nResult = strlen( (char*)pDest );
		}
		if ( AL_IsNoGPSAlarm() ){
			sprintf( (char*)pDest+nResult, ",GPS:%s", TEXT_UNLOCKED  );
			nResult = strlen( (char*)pDest );
		}
		if ( AL_IsCrashAlarm() ){
			sprintf( (char*)pDest+nResult, ",CAR:%s", TEXT_CRASH  ); 
			nResult = strlen( (char*)pDest );
		}
		if( TE_HaveAlarm() ){
			strcat( (char*)pDest,(char*)" " ); 
			strcat( (char*)pDest,(char*)TEXT_ALARM );    
		}		
		nResult = strlen( (char*)pDest );	
		
		if( COM_IsSMSFeedbackUseJave() == 0 ){ 
			sprintf( (char*)(pDest+nResult), ",%s?lng=%.6f&lat=%.6f&key=%d", TEXT_GOGPS,
																     	    (double)GPS_GetLongitude()/3600000,
																     	    (double)GPS_GetLatitude( )/3600000,
																     	     GPS_CalculateData() );
																	     	    
		}	
																		     	   
		nResult = strlen( (char*)pDest );
	}
	return nResult;
}
/////////////////////////////////////////////////////////////////////////////
//  组织客户定义的信息
unsigned char PLS_ComposeCLUploadData( unsigned char *pDest, unsigned char *pCodeType ){ 
	
	unsigned char	nResult = 0;
	
	#if (SW_PROTOCOLB)
		sprintf( (char*)pDest, "%s:", g_stGSMSetup.m_strUID );
		nResult = strlen( (char*)pDest );
		GPS_GetGPRMCData( pDest+nResult );		
	#else
		if( strlen((char*)g_stGSMSetup.m_strTEUID) > 0 ){	
			//sprintf( (char*)pDest, "%s:%s", g_stGSMSetup.m_strTEUID, g_strGPRMC );
			sprintf( (char*)pDest, "%s:", g_stGSMSetup.m_strTEUID );
			nResult = strlen( (char*)pDest );
			GPS_GetGPRMCData( pDest+nResult );
		}
		else{
			GPS_GetGPRMCData( pDest );
			//sprintf( (char*)pDest, "%s", g_strGPRMC );
		}
	#endif
	*pCodeType = 0;
	
	return strlen((char*)pDest);
}
///////////////////////////////////////////////////////////
//  组织里程数据
unsigned char  PLS_ComposeMileage( unsigned char 	*pDest, unsigned char *pCodeType ){
	
	unsigned char	nResult = 0;	
	
	
	*pCodeType = 0;
	sprintf( (char*)pDest,"%s:%.2f KM, %s:%d KM", TEXT_TOTAL_MILEAGE, 
												  (float)GetTotalMileage()/1000,
												  TEXT_MILEAGE_REMARK, 
												  GetCurrentMileage()/1000 ); 
	nResult = strlen( (char*)pDest );
	return  nResult;
}




