/////////////////////////////////////////////////////////////
//file name: APP_GSMBENG.H
//     Date: 2008-3-02    
//	 Author: wyb	 
// optimize: real
// op_date : 2009-3-30
/////////////////////////////////////////////////////////////
#include  <stdio.h>
#include  <stdlib.h>
#include "..\hdr\includes.h"
#include "..\hdr\queue.h"
#include "..\hdr\main.h"
#include "..\hdr\APP_GSM.h"
#include "..\hdr\APP_GPS.h"
#include "..\hdr\HW_UART.h"
#include "..\hdr\HW_flash.h"
#include "..\hdr\APP_LoopDetect.h"
#include "..\hdr\APP_Terminal.h"
#include "..\hdr\HW_GPIO.h"
#include "..\hdr\APP_GMSBENGQ.h"
#include "..\hdr\ProtocolA.h"
#include "..\hdr\ProtocolB.h"
#include "..\hdr\ProtocolC.h"


#ifdef			HW_BENQM26
//=====================================================
// M26
const char	*s_strCMMCMD[]={
			"OK",
			"E0",				//关闭回显示
			"E1",				//开启回显
			"+CSQ",		  //查GSM信号		at+csq?
			"+CGSN",		//IMEI 序号
			"+CREG",		//查询GSM网络
			"+CGREG",		//查询GPRS网络
			"+CPIN",		//卡检测
			"+CGSN",		//主机序列号
			
			//电话指令
			"+CLCC",    //查询来电号码
			"D",
			"A",
			"H",
			"NO CARRIER",
			"BUSY",
		  "NO ANSWER",
			"+CLIP",		//开启来电显示
			"$SG",			//麦克风设置
			"$SAMP",		//设置扬器音量
			
			//短信指令
			"+CSMP",		//发送的编码方式,,,0			
			"+CSCA",		//读取中心号码
			"+CMGF",		//短信格式
			"+CSDH",		//显示文本格式
			"+CNMI",		//短信提示
			"+CMGR",		//读取短信
			"+CMGS",		//发送短信
			"+CMGD",		//删除短信
			"+CSMP",		//编码格式
			
			//网络注册
			"+CGDCONT",	//定义PDP上下文
			"+CGATT",		//注册GPRS网络
			"+CUSD",		//非结构化附加业务
			"+CLCC",		//查询来电
			"+CPIN",		//查询卡
};

extern  OS_EVENT 			*pGSMReceiveSem;

//////////////////////////////////////////////////////////////////
//  GSM硬件初始化
void CMM_HWInit( void ){	
	
  GSMUARTInit( 9600 );		// GSM串口     
	// GSM模块复位引脚
  GPIO_Enable( HW_PORT_GSMRESET );
  GPIO_SetDirection( HW_PORT_GSMRESET, HW_PORT_OUTPUT ); 
  HW_GSM_RESET_HIGH();

  // GSM模块开关引脚	
  GPIO_Enable(HW_PORT_GSMPOWER);			
  GPIO_SetDirection(HW_PORT_GSMPOWER, HW_PORT_OUTPUT);		
  HW_GSM_POWERON( );		
}
/////////////////////////////////////////////////////////
//	GSM通信模块硬件重启	
void CMM_ReStart( void ){
	
	HW_GSM_RESET_LOW( );
	OSTimeDlyHMSM(0,0,0,50);
	HW_GSM_RESET_HIGH( );
		
	// power off
	HW_GSM_POWEROFF( );
	OSTimeDlyHMSM(0,0,3,0);
	HW_GSM_POWERON( );	
	OSTimeDlyHMSM(0,0,1,0);
	
	// power on
	HW_GSM_POWEROFF( );
	OSTimeDlyHMSM(0,0,0,100 );
	HW_GSM_POWERON( );	
	OSTimeDlyHMSM(0,0,5,0);	
} 
///////////////////////////////////////////////////////////////////
//  初始化GSM模块
void CMM_Init(void){
	
	int 	nCnt = 0;
		
	while( (CMM_ExecATCmd( GSM_ATE0, NULL, GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL ) != GSM_SUCCEED) && 
		   (nCnt++ < 50) ){
		OSTimeDlyHMSM(0,0,1,0);
	}	
	//检测卡是否就绪
	nCnt = 0;
	while( ( GSM_CheckSIMReady() != GSM_SUCCEED) && 
		   ( nCnt++ < 50 ) ){		
		OSTimeDlyHMSM(0,0,1,0);
	}	
	// 终端序列号
	nCnt = 0;
	while( ( CMM_QueryModuleSN(g_stGSMSetup.m_strUID) != GSM_SUCCEED )&&
		   ( nCnt++ < 50 ) ){
		OSTimeDlyHMSM(0,0,1,0);
	}		
	//	短信中心号码
	nCnt = 0;	
	while( ( GSM_QuerySMC(g_stGSMSetup.m_strSMCNo) != GSM_SUCCEED )&&
		   ( nCnt++ < 50 ) ){
		OSTimeDlyHMSM(0,0,1,0);
	}	
	//  全功能模式
	CMM_ExecATCmdN( GSM_CFUN, 1, GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL );	
	//  麦克风设置
	CMM_ExecATCmd( GSM_SG, (unsigned char*)"=0,8", GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL );
	//  设置扬器音量 
	CMM_ExecATCmd( GSM_SAMP, "=0,4", GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL );
	//  短信格式	 PDU方式
	CMM_ExecATCmdN( GSM_CMGF, 0, GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL );
	//  显示文本格式下的参数
	CMM_ExecATCmdN( GSM_CSDH, 1, GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL );
	//  GPRSSEND模式
	CMM_ExecATCmd(GSM_IOMODE, (unsigned char*)"=1,1", GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL);
	//  删除全部短信
	CMM_DeleteAllSMS();  
	//  设置短信提示
	while( CMM_ExecATCmd( GSM_CNMI, (unsigned char*)"=2,1", GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL ) != GSM_SUCCEED){
		OSTimeDlyHMSM(0,0,1,0);
	}
}
/////////////////////////////////////////////////////////////
//  初始化网络连接	
unsigned char CMM_IPInit( unsigned char *pStrGateway, unsigned char *pUser, unsigned char *pPsd ){
	
	CMM_IPClose();
	CMM_ExecATCmd( GSM_CGATT, (unsigned char*)"=1", GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL );	
	
	if( GPRS_SetGateWay( g_stGSMSetup.m_strGateway ) == GSM_SUCCEED &&
		GPRS_SetUserPsd( g_stGSMSetup.m_strUser, g_stGSMSetup.m_strPsd) == GSM_SUCCEED ){
		return 1;			
	}
	else{
		return 0;	
	}
}
/////////////////////////////////////////////////////////////////
//  解释AT指令格式
//  返回命令类型, 并取出命令参数
unsigned char CMM_GetCmdType(unsigned char *pMsg){
	
	unsigned char 	nCnt = 0;
	unsigned char		nLen = 0;
	unsigned char 	*pTmp = NULL;
	
	for( nCnt = 0; nCnt < CMM_CMD_END; nCnt++ ){
		
		nLen = strlen( s_strCMMCMD[nCnt] );
	 	if( memcmp( pMsg, s_strCMMCMD[nCnt], nLen ) == 0 ){
	 		
	 		pTmp = (unsigned char*)strchr( (char*)pMsg, ':');
	 		if( pTmp ){
	 			pTmp++;
	 			strcpy( (char*)pMsg, (char*)pTmp );
	 		}	 		
	 		break;		 		
	 	}	 	
	}
	return nCnt;
}
/////////////////////////////////////////////////////////////////
// 
unsigned char CMM_AT( void ){
	
	return CMM_ExecATCmd( CMM_CMD_END, NULL, GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL );	
}
////////////////////////////////////////////////////////////////////////////
// 返回信号级别
unsigned char CMM_QuerySignal( void ){
	
	unsigned char   nReturnMsg[20];
	unsigned char	nResult = 0;
	
	if( CMM_ExecATCmd( GSM_CSQ, NULL, GSM_ATTIME_CHECK, GSM_CSQ, GSM_ERROR, nReturnMsg) == GSM_SUCCEED ){
		nResult = atoi( (char*)nReturnMsg );
		if( nResult == 99 ){
			nResult = 0;
		}
	}
	return nResult;
}
///////////////////////////////////////////////////////////	
// 查询GSM网络状态
// 0,2,3,4: 未注册
// 1:				注册，本地
// 5:				注册, 外地
// 有效返回1 无效返回0
unsigned char CMM_QueryNetworkStatus(void){
	
	unsigned char 	nBuf[40];
	unsigned char 	nState = 0;	
	unsigned char 	*pTmp = NULL;
	
	memset( (char *)nBuf, 0, 2 );
	
	CMM_ExecATCmd(GSM_CREG, (unsigned char*)TEXT_QUESTION, GSM_ATTIME_CHECK, GSM_CGREG, GSM_ERROR, nBuf);	
				
	pTmp = (unsigned char*)strchr( (char*)nBuf, ',' );
	if( pTmp ){
		pTmp++;
		nState = atoi( (char*)pTmp );
	}
	if( nState == 1 || nState == 5 ){
		return 1;
	}
	else{
		return 0;
	}
}
///////////////////////////////////////////////////////////////
// 打电话
unsigned char CMM_DialNumber( unsigned char *pStrNo ){
	
	unsigned char		nBuf[50];
	unsigned char 	nResult;
	
	CMM_ExecATCmdN( GSM_VLB, 1, GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL );	
	
	sprintf((char*)nBuf, "%s;", (char*)pStrNo ); 
	nResult = CMM_ExecATCmd(GSM_ATD, nBuf, GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL);	
	if( nResult == GSM_SUCCEED ){
		CMM_SetCallOnLine( );		
	}
	return nResult;
}
//////////////////////////////////////////////////////////////
//  接电话
unsigned char CMM_AcceptCall( void ){	
	
	return CMM_ExecATCmd(GSM_ATA, NULL, GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL);	
}
//////////////////////////////////////////////////////////////
//  挂电话
unsigned char CMM_RejectCall( void ){
	
	return CMM_ExecATCmd(GSM_ATH, NULL, GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL);		
}
//////////////////////////////////////////////////////////////
//  查询IMEI序号
unsigned char	CMM_QueryIMEI( unsigned  char	*pDest ){
	
	unsigned char	nBuf[20];
	
	//sprintf( (char*)nBuf, "*#06#" );		
	return CMM_ExecATCmd(GSM_ATD, nBuf, GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, pDest );	
} 
/////////////////////////////////////////////////////////////
// 返回:			0： free
//						1:  on use
// +CLCC : 1,1,4,0,0,"13902288001",161
unsigned char GSM_ReadCaller( unsigned char *pCaller ){
	
	unsigned char			nBuf[50];
	unsigned char			nResult = 0;
	unsigned char 			*pTmp = NULL;
	
	if( CMM_ExecATCmd(GSM_CLCC, NULL, GSM_ATTIME_CHECK, GSM_CLCC, GSM_ERROR, nBuf) == GSM_SUCCEED ){
		nResult = 1;
		
		pTmp = (unsigned char*)strchr( (char*)nBuf, '"' );
		if( pTmp != NULL ){
			pTmp++;
			strcpy( (char*)nBuf, (char*)pTmp );
			
			pTmp = (unsigned char*)strchr( (char*)nBuf, '"' );
			if( pTmp != NULL && pCaller != NULL){
				memcpy( (char*)pCaller, (char*)nBuf, pTmp-nBuf );
				pCaller[pTmp-nBuf] = 0x00; 
			}
		}		
	}
	return nResult;	
}
//////////////////////////////////////////////////////////////
//
unsigned char GSM_OffModule( void ){
	
	return CMM_ExecATCmd(GSM_MSO, NULL, GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL);		
}
//////////////////////////////////////////////////////////////
// 删除短信
unsigned char CMM_DeleteSMS( unsigned short nCnt ){
	
	unsigned char 	nBuf[10];
		
	sprintf((char*)nBuf, "=%d", nCnt); 		
	return CMM_ExecATCmd(GSM_CMGD, nBuf, GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL);		
}
////////////////////////////////////////////////////////////////////
// 　删除全部短信 
unsigned char CMM_DeleteAllSMS(void){
	return CMM_ExecATCmd(GSM_CMGD, (unsi
	gned char*)"=1,4", GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL);			
}
///////////////////////////////////////////////////////////////
// 有效返回  1
// 无效返回  0
unsigned char GSM_CheckSIMReady( ){
	
	unsigned char 	nResult = 0;
	unsigned char 	nReturnMsg[20];
	
	if( CMM_ExecATCmd( GSM_CPIN, (unsigned char *)TEXT_QUESTION, GSM_ATTIME_CHECK, GSM_CPIN,GSM_ERROR, nReturnMsg ) == GSM_SUCCEED ){
		if( strstr( (char*)nReturnMsg, (char*)"READY" ) != NULL ){
			nResult = 1;
		}		
	}
	return nResult;
}
//////////////////////////////////////////////////////////
//
unsigned char CMM_QueryModuleSN( unsigned char *pStr ){
			
	return CMM_ExecATCmd( GSM_CGSN, NULL, GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, pStr );
}
/////////////////////////////////////////////////////////////
// 提取短信中心号码
unsigned char GSM_QuerySMC( unsigned char *pStr ){
	
	unsigned char 	nBuf[50];
	unsigned char		nResult = GSM_NG;
	unsigned char		*pTmp = NULL;
	unsigned char 	nTmp = 0;
		
	if( CMM_ExecATCmd( GSM_CSCA, (unsigned char *)TEXT_QUESTION, GSM_ATTIME_CHECK, GSM_CSCA, GSM_ERROR, nBuf ) == GSM_SUCCEED ){
		
		pTmp = (unsigned char*)strchr( (char*)nBuf, '"' );
		if( pTmp != NULL ){
			pTmp++;
			strcpy( (char*)nBuf, (char*)pTmp );
			
			pTmp = (unsigned char*)strchr( (char*)nBuf, '"' );
			if( pTmp != NULL ){
				nResult = GSM_SUCCEED;
				nTmp = pTmp-nBuf;
				memcpy( (char*)pStr, (char*)nBuf, nTmp );
				*(pStr+nTmp) = 0x00;
			}			
		}
	}
	return nResult;
}
/////////////////////////////////////////////////////////////////
// 解释域名服务器
unsigned char GSM_ParseDNS( unsigned char *pStrHost, unsigned char *pStrIP ){
	
	unsigned char			nBuf[50];	
	sprintf((char*)nBuf, "=\"%s\"", pStrHost );	
	return CMM_ExecATCmd( GSM_DNSR, nBuf, GSM_ATTIME_CHECK, GSM_DNSR, GSM_ERROR, pStrIP );
}
/////////////////////////////////////////////////////////////////
//  查询话费
unsigned char	GSM_ExecCUSD( unsigned char	*pDest ,unsigned char	*pSrc, unsigned short	nTimeout ){	

	return  CMM_ExecATCmd( GMS_CUSD, pSrc, nTimeout , GMS_CUSD, GSM_ERROR,  pDest);
}
////////////////////////////////////////////////////////////////////
// 查询GPRS网络
// 返回网络状态
// 有效返回 1
// 无效返回 0
unsigned char CMM_QueryIPNetworkStatus(void){
	
	unsigned char 	nBuf[40];
	unsigned char		nState = 0;
	unsigned char 	*pTmp = NULL;
	
	memset( (char*)nBuf, 0, 2 );
	CMM_ExecATCmd(GSM_CGREG, (unsigned char*)TEXT_QUESTION,  GSM_ATTIME_CHECK, GSM_CGREG, GSM_ERROR, nBuf);
	
	pTmp = (unsigned char*)strchr( (char*)nBuf, ',' );
	if( pTmp ){		
		pTmp++;
		nState = (unsigned char)atoi( (char*)pTmp );		
	}
	if( nState == 1 || nState == 5 ){
		return 1;
	}
	else{
		return 0;
	}
}
////////////////////////////////////////////////////////////////////
// 解析并存储最新信息
void CMM_ParseMasterCmd(unsigned char *pMsg, unsigned char nCmdType){
	
	unsigned char			nCnt = 0;
	unsigned short		nLen = 0;
	unsigned char 		nCaller[TELNUMBER_LENGTH+5];
	unsigned char			*pTmp = NULL;
	
	switch(nCmdType){
	case GSM_RING:
		nCaller[0] = 0x00;
		GSM_ReadCaller( nCaller );
		if( strlen( (char*)nCaller) > 0 ){
			if( IsAuthorizationNo( (unsigned char*)nCaller) ){						
				CMM_AcceptCall();		
				CMM_SetCallOnLine();
				// 关闭静音				
				#ifdef HW_ET201
				HW_PORT_SPKMUTE_CLOE();
				#endif
			}
			else{
				CMM_RejectCall();
				CMM_SetCallOffLine();		
			}
		}	
		break;	
	case GSM_CMTI:		
		pTmp = (unsigned char *)strstr((char *)pMsg, ",");
		if( pTmp ){
			pTmp++;
			nCnt = atoi( (char*)pTmp );
			InsertNewMsg( nCnt );
		}
		break;		
	case GSM_IPDATA:		
		nLen = atoi( (char*)pMsg );
		pTmp = (unsigned char*)strchr( (char*)pMsg, '"' );
		if( pTmp ){
			pTmp++;
			nLen = P_StrToHex( pMsg, pTmp, nLen*2 );			
			nLen = PL_ParseData( pMsg, pMsg, nLen );			
			// 发送失败是否要重启模块
			if( nLen ){
				IP_SendData( IP_IsTCPCommMode(), pMsg, nLen );
			}
		}
		break;		
	// 网络断开
	case GSM_IPCLOSE:
		IP_SetConnectNG();
		break;		
	// 通话结束
	// 通话桂机
	// 无人接听
	case GSM_NO_CARRIER:
	case GSM_BUSY:
	case GSM_NO_ANSWER:		
		#ifdef HW_ET201		
	    HW_PORT_SPKMUTE_CLOE();	// 关闭静音
	  #endif
		CMM_SetCallOffLine();
		break;			
	default:
		break; 
	}
	return;
}

#endif //HW_BENQM26
