/////////////////////////////////////////////////////////////
//file name: APP_CDMA.c
//     Date: 2008-3-02    
//	 Author: wyb	 
// optimize: real
// op_date : 2009-3-8
/////////////////////////////////////////////////////////////
// include file declare
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
#include "..\hdr\APP_EM200.h"
#include "..\hdr\ProtocolA.h"
#include "..\hdr\ProtocolB.h"
#include "..\hdr\ProtocolC.h"

#if( HW_EM200 )
//=====================================================
// CM320
const char	*s_strCMMCMD2[]={
		"OK",				//OK
		"E0",				//关闭回显示
		"E1",				//开启回显
		"+GSN",		  //ESN查询命令
	 	"+CIMI",		//IMSI查询命令
		
		"+CDV",			//打电话
		"+CHV",			//挂电话
		"$QCCAV",		//接听电话
		"%MICV",    //调节MIC
		
		"$HCCNT",	    //呼叫连接指示
		"$HCEND",		//结束通话	
		"$HRIG",   		//被叫提示
		"+CPIN",  		//检测卡
		"+CSQ",     	//查信号 
		// 短信操作功能
		"%SMSSS",		//选择短信提示参数  %SMSSS
		"+CMGF",		//设置短信格式	1：文本 
		"+CNMI",		//短信通短命令 1,1,0,1,0
		"%CMGR",		//读取短信
		"+CPMS",		//选择短信存储器
		"%CMGSS",		//短信发送成功标志CMGSS
		"%CMGSF",		// 发送短信失败
		"%CMGS",	    //发送短信
		"+CMTI",		//收到新短信 
		"+CMGD",		//删除短信
		"$HSMSRD",	    //读取一条短信		
		// 语音指令
		"$HSETEC",	  	// 回音消除控制
		// GPRS网络管理
		"$HACCNT",		// 中国电信的CDMA 用户名card,密码card
		"$HPPPOPEN",	// 建立PPP连接
		"$HPPPCLOSE",	// DTU 关闭PPP连接
		"$HCORG",		  // 发起呼叫指示 
		"$HPPPSTATUS",// 拨号连接状态报告
		"$HDTUIP",		// 获取模块IP地址
		// TCP管理
		"$HTCPOPEN",    // DTU建立TCP连接	  
		"$HTCPCNNTIMER",// DTU建立TCP连接时间
		"%TCPCNNABORT", //DTU设置建立,TCP连接的最长时间
		"$HTCPSEND",	  // DTU发送TCP数据
		"$HTCPRECV",    // DTU接收TCP数据
		"$HTCPSTATUS",  // DTU TCP通道连接状态上报 0: TCP连接成功 1:TCP连接关闭
		"$HTCPCLOSE",   // DTU 关闭TCP连接	
		//  UDP管理
		"$HUDPOPEN",    // DTU建立UDP连接
		"$HUDPSEND",	  // DTU发送UDP数据
		"$HUDPRECV",   // DTU接收UDP数据		
		"$HUDPCLOSE",   // DTU 关闭UDP连接
		"$HUDPSTATUS",  // DTU UDP通道连接状态上报 0: UDP连接成功 1:UDP连接关闭 
		//  其它指令
		"+CME",			// 错误提示
		"+CMS",			//
		"CID",			// 来电提示号码
		"+IFC",			// 关闭硬件流
		"+CLIP", 		// 来电显示号码
		"$HCTCM",		// 兼容开产命令
		"+TECNT", 		//连接请求
		"+TEDISCNT",	//挂断请求
		"+TESETUP",     //l	读取/设置 设备参数
		
};
//=====================================================
//  ME200
const char	*s_strCMMCMD[]={
		"OK",			//OK
		"E0",			//关闭回显示
		"E1",			//开启回显
		"+GSN",		  	//ESN查询命令
		"+CIMI",		//IMSI查询命令
		
		"+CDV",			//打电话
		"+CHV",			//挂电话
		"$QCCAV",		//接听电话
		"%MICV",    	//调节MIC
		
		"%CCNT",		//呼叫连接指示
		"%CEND",		//结束通话
		"RING",   		//被叫提示	    
		"+CPIN",  		//检测卡
		"+CSQ",     	//查信号 
		// 短信操作功能
		"%SMSSS",	  //选择短信提示参数
		"+CMGF",		//设置短信格式	1：文本 
		"+CNMI",		//短信通短命令 1,1,0,1,0
		"%CMGR",		//读取短信
		"+CPMS",		//选择短信存储器
		"%CMGSS",		//短信发送成功标志
		"%CMGSF",		// 发送短信失败
		"%CMGS",	 	 //发送短信
		"+CMTI",		//收到新短信 
		"+CMGD",		//删除短信
		"$HSMSRD",		//读取一条短信
		// 语音指令
		"%SETEC",		// 回音消除控制
		// GPRS网络管理
		"%ACCNT",		// 中国电信的CDMA 用户名card,密码card
		"%PPPOPEN",		// 建立PPP连接
		"%PPPCLOSE",	// DTU 关闭PPP连接
		"%CORG",		// 发起呼叫指示 
		"%PPPSTATUS", 	// 拨号连接状态报告
		"%DTUIP",		// 获取模块IP地址
		// TCP管理
		"%TCPOPEN",   // DTU建立TCP连接	  
		"%TCPCNNTIMER",//DTU建立TCP连接时间
		"%TCPCNNABORT",//DTU设置建立,TCP连接的最长时间
		"%TCPSEND",	  // DTU发送TCP数据
		"%TCPRECV",   // DTU接收TCP数据
		"%TCPSTATUS", // DTU TCP通道连接状态上报 0: TCP连接成功 1:TCP连接关闭
		"%TCPCLOSE",  // DTU 关闭TCP连接
		// UDP管理
		"%UDPOPEN",     // DTU建立UDP连接
		"%UDPSEND",	    // DTU发送UDP数据
		"%UDPRECV",     // DTU接收UDP数据	
		"%UDPCLOSE",    // DTU 关闭UDP连接
		"%UDPSTATUS",   // DTU UDP通道连接状态上报 0: UDP连接成功 1:UDP连接关闭
		//  其它指令
		"+CME",			// 错误提示
		"+CMS",			//
		"CID",			// 来电提示号码
		"+IFC",			// 关闭硬件流	
		"+CLIP", 		// 来电显示号码
		"$HCTCM",		// 兼容开产命令EM310
		"+TECNT", 		//连接请求
		"+TEDISCNT",	//挂断请求
		"+TESETUP",     //l	读取/设置 设备参数
};

extern  OS_EVENT 			*pGSMReceiveSem;

//////////////////////////////////////////////////////////////////
// CDMA硬件初始化
void CMM_HWInit( void ){
	
  GSMUARTInit(9600);		// GSM 串口  
  // GSMUARTInit(115200);		// GSM 串口  
 // GSM模块复位引脚没用,开机后,延时五秒
 	
//  GPIO_Enable(HW_PORT_GSMRESET);
//  GPIO_SetDirection(HW_PORT_GSMRESET, HW_PORT_OUTPUT); 
//  HW_GSM_RESET_HIGH();

  // GSM模块开关引脚	
  GPIO_Enable(HW_PORT_WARMPOWER);			
  GPIO_SetDirection(HW_PORT_WARMPOWER, HW_PORT_OUTPUT);		
  HW_GSM_WARM_POWERON( );
  
  #if( HW_COLDRESTART_GSM )
  GPIO_Enable(HW_PORT_COLDPOWER);			
  GPIO_SetDirection(HW_PORT_COLDPOWER, HW_PORT_OUTPUT);			
  HW_GSM_COLDPOWEROFF();
  #endif	
  
}
/////////////////////////////////////////////////////////
// CDMA通信模块硬件重启	
void CMM_ReStart( void ){
		
//	HW_GSM_RESET_LOW();
//	OSTimeDlyHMSM(0,0,0,100);
//	HW_GSM_RESET_HIGH();
//	OSTimeDlyHMSM(0,0,0,50);
	//2011-03-26 增加了冷启动功能
	#if( HW_COLDRESTART_GSM )
		OSTimeDlyHMSM(0,0,0,500);
		HW_GSM_COLDPOWEROFF();
		OSTimeDlyHMSM(0,0,5,0);	
		HW_GSM_COLDPOWERON();			
		//  高电
		HW_GSM_WARM_POWERON( );				//H
		OSTimeDlyHMSM(0,0,0,500 );		
		// power on
		HW_GSM_WARM_POWEROFF( );	
		OSTimeDlyHMSM(0,0,0,200);	
		HW_GSM_WARM_POWERON( );
		OSTimeDlyHMSM(0,0,0,100);
	#else
		// power off
		HW_GSM_WARM_POWEROFF();
	    OSTimeDlyHMSM(0,0,3,0);
	    HW_GSM_WARM_POWERON( );
	    OSTimeDlyHMSM(0,0,5,0);	

		// power on
		HW_GSM_WARM_POWEROFF( );	
		OSTimeDlyHMSM(0,0,0,200);	
		HW_GSM_WARM_POWERON( );
		OSTimeDlyHMSM(0,0,0,100);
	#endif
}
///////////////////////////////////////////////////////////////////
//初始化GSM模块
void CMM_Init(void){  
	
	int 	nCnt = 0;
	
	// 关闭硬件流
	while( nCnt++ <= 50 ){
		if (CMM_ExecATCmd( CDMA_IFC, (unsigned char*)"=0,0", GSM_SENDSPACE_CHECK, CDMA_OK, CDMA_CME, NULL )  == GSM_SUCCEED){
			break;
		}
	}	
	nCnt = 0; 
	// 关闭回显
	while( (CMM_ExecATCmd( CDMA_ATE0, NULL, GSM_ATTIME_CHECK, CDMA_OK, CDMA_CME, NULL ) != GSM_SUCCEED) && 
				 (nCnt++ < 50) ){
		OSTimeDlyHMSM(0,0,1,0);
	}
	// 检测卡是否就绪
	nCnt = 0;
	while( 	( CMM_CheckSIMReady() != GSM_SUCCEED) && 
					( nCnt++ < 50 ) ){		
		OSTimeDlyHMSM(0,0,1,0);
	}
	// 兼容开产命令
	nCnt = 0;
	while( nCnt++ < 50 ){
		if( CMM_ExecATCmd( CDMA_HCTCM, (unsigned char*)"=0", GSM_ATTIME_CHECK, CDMA_HCTCM, CDMA_CME , NULL ) == GSM_SUCCEED ){
			break;
		}
		OSTimeDlyHMSM(0,0,1,0);		
	}
	/*
	// 终端序列号
	#if( SW_PROTOCOL_ASICO == 0 )
	if( strlen((char*) g_stGSMSetup.m_strUID ) <= 0 ){
		TE_SetNeedWriteFlash( );
	}
	nCnt = 0;
	while( ( CMM_QueryModuleSN(g_stGSMSetup.m_strUID) != GSM_SUCCEED )&&
				 ( nCnt++ < 50 ) ){	 	
		OSTimeDlyHMSM(0,0,1,0);
	}
	#endif //SW_PROTOCOL_ASICO
	*/
	
	//  新短信通知设置
	nCnt = 0;
	while( nCnt++ < 50 ){
		if( CMM_ExecATCmd( CDMA_CNMI, (unsigned char*)"=1,1,0,1,0", GSM_ATTIME_CHECK, CDMA_OK, CDMA_CMS, NULL ) == GSM_SUCCEED ){
			break;
		}
		OSTimeDlyHMSM(0,0,1,0);		
	}			
	//  设置短信发送方式	
    CMM_ExecATCmd( CDMA_SMSSS, (unsigned char*)"=1,0,1,0", GSM_ATTIME_CHECK, CDMA_OK, CDMA_CME, NULL );	
	//  选择短信存储器
	CMM_ExecATCmd( CDMA_CPMS, (unsigned char*)"=\"ME\"", GSM_ATTIME_CHECK, CDMA_CPMS, CDMA_CME, NULL );
	
	// 设置短信格式
	CMM_ExecATCmdN( CDMA_CMGF, 1, GSM_ATTIME_CHECK, CDMA_OK, CDMA_CME, NULL );
	// 调节MIC
	CMM_ExecATCmdN( CDMA_MICV, 4, GSM_ATTIME_CHECK, CDMA_OK, CDMA_CME, NULL );
	// 回音消除控制
	CMM_ExecATCmdN( CDMA_SETEC, 4, GSM_ATTIME_CHECK, CDMA_OK, CDMA_CME, NULL );
	// 开启来电显示
	CMM_ExecATCmdN( CDMA_CLIP, 1, GSM_ATTIME_CHECK, CDMA_OK, CDMA_CME, NULL );
		
	// 删除短信
	CMM_DeleteAllSMS();
}
// 总返回1
unsigned char CMM_QueryIPNetworkStatus( ){
	return 1;
}
/////////////////////////////////////////////////////////////////
// 
unsigned char CMM_AT( void ){
	return CMM_ExecATCmd( CMM_CMD_END, NULL, GSM_ATTIME_CHECK, CDMA_OK, CDMA_CME, NULL );	
}
/////////////////////////////////////////////////////////////
//初始化网络连接
unsigned char CMM_IPInit( unsigned char *pStrGateway, unsigned char *pUser, unsigned char *pPsd ){
		
	CMM_SetPPPUserName( pUser, pPsd );  //建立PPP连接
	CMM_DTUPPPClose();					//关闭PPP拨号
	CMM_DTUPPPOpen();					//打开PP拨号
	return GSM_SUCCEED;
}
/////////////////////////////////////////////////////////////
//	PDP上下文激活
void	CMM_ActivationPDP( void ){
	
	
}
///////////////////////////////////////////////////////////	
// 查询GSM网络状态
// 0,2,3,4: 未注册
// 1:				注册，本地
// 5:				注册, 外地
// 有效返回1 无效返回0
unsigned char CMM_QueryNetworkStatus( void ){

	unsigned char 	nState = 0;	

	return nState;
}
////////////////////////////////////////////////////////////////////////////
//   TCP/IP连接
// 返回GSM_SUCCEED表示成功，否则失败
// $HUDPOPEN: 1, 0
unsigned char CMM_IPConnect( unsigned char nMode, 
							 unsigned char *pStrIP,
							 unsigned short nPort ){ 														 	
														 	
	unsigned char	  nResult = GSM_NG;
	unsigned char	  *pch  = NULL;
	unsigned char 	  nReturnMsg[50];

	if( nMode ){ //TCP
		sprintf((char*)nReturnMsg, "=1,%s,%d",  pStrIP, nPort);	
		nResult = CMM_TCPConnect( CDMA_TCPOPEN, nReturnMsg, nReturnMsg ); 
	}
	else{       //UDP
		sprintf((char*)nReturnMsg, "=2,%s,%d",  pStrIP, nPort);			
		nResult = CMM_TCPConnect( CDMA_UDPOPEN, nReturnMsg, nReturnMsg );
	}	
	if( nResult == GSM_SUCCEED ){		
		pch = (unsigned char*)strchr( (char*)nReturnMsg, ',');
		if ( pch != NULL ){
			nResult = atoi( (char*)pch+1 );
			if ( 0 == nResult ){
				nResult = GSM_SUCCEED;
			}
			else{
				nResult = GSM_NG;
			}
		}
	}
	return  nResult;
}
////////////////////////////////////////////////////////////////////////////
//   TCP/IP连接方式
unsigned char	CMM_TCPConnect(unsigned char nCmdType, unsigned char	*pCmdData, unsigned char	*pReturnMsg ){

	unsigned char 	nResult = GSM_NG;	
	unsigned char	nError = 0;
	unsigned char	nType = 0;
	unsigned char	nCnt = 0;
	unsigned short	nTimeout = GPRS_GPRSDATA_CHECK;
	unsigned char	*lpLargeBuf = NULL;
	
	lpLargeBuf	= lpLargeBuf4;
	CMM_SendATCmd( nCmdType, 0, pReturnMsg );	
	while ( 1 ){		
		OSSemPend(pGSMReceiveSem, nTimeout, &nError);
		if( nError == OS_TIMEOUT ){
			if ( nCnt++ > 4 ){
				break;
			}
		}
		else{ 
			CMM_ReadCmd( lpLargeBuf , GSM_RECMAXBUFFER );						
			nType = CMM_GetCmdType(lpLargeBuf);				
			if( (nType == CDMA_TCPSTATUS)||(nType == CDMA_UDPSTATUS) ){	
				strcpy( (char*)pReturnMsg, (char*)lpLargeBuf );
				nTimeout = GSM_LOWAIT_TIME;
				nResult = GSM_SUCCEED;
			}
			else if( nType == CDMA_CME ){
				nTimeout = GSM_LOWAIT_TIME;			
			} 
			
			else if ( nType == CDMA_OK ){
				nTimeout = GSM_SENDSPACE_CHECK;	
			}
			else{
				CMM_ParseMasterCmd(lpLargeBuf, nType);				
			}	
		}				
	}
	return nResult;
}
/////////////////////////////////////////////////////////////////
// 解释域名服务器
unsigned char GSM_ParseDNS( unsigned char *pStrHost, unsigned char *pStrIP ){

	return GSM_NG;
}
///////////////////////////////////////////////////////////////////////////
//   关闭 TCP/IP连接
//   $HTCPCLOSE: 1, 0
void	CMM_IPClose( ){
	CMM_IPClose2(1);
	CMM_IPClose2(0);
	return;
}
///////////////////////////////////////////////////////////////////////////
//   关闭 TCP/IP连接
//   $HTCPCLOSE: 1, 0
unsigned char	CMM_IPClose2( unsigned char	nMode ){
	
	unsigned  char	nResult = GSM_NG;
	unsigned char	*pch = NULL;
	unsigned char	*pch1 = NULL;
	unsigned char 	nReturnMsg[40]; 
	
	if( nMode ){ //TCP
		nResult = CMM_ExecATCmdN( CDMA_TCPCLOSE, 1, GPRS_GPRSDATA_CHECK, CDMA_TCPCLOSE, CDMA_CME, nReturnMsg );
	}
	else{    			//UDP
		nResult = CMM_ExecATCmdN( CDMA_UDPCLOSE, 2, GPRS_GPRSDATA_CHECK, CDMA_UDPCLOSE, CDMA_CME, nReturnMsg );
	}
	if( GSM_SUCCEED == nResult ){
		pch = nReturnMsg;
		pch1 = (unsigned char*)strchr( (char*)pch, ',');
		if ( pch != NULL ){
			pch = pch1+1;
			nResult = atoi( (char*)pch );
			if ( 0 == nResult ){
				nResult = GSM_SUCCEED;
			}
			else{
				nResult = GSM_NG;
			}
		}
	}
	return nResult;
}
/////////////////////////////////////////////////////////////
// 设置PPP用户名和密码
unsigned char	CMM_SetPPPUserName( unsigned char	*pUser, unsigned char  *pPsd ){
			
	unsigned char		nBuf[50];
	
	sprintf( (char*)nBuf, "=%s,%s"	, pUser, pPsd );				
	return CMM_ExecATCmd(CDMA_ACCNT, nBuf, GPRS_GPRSDATA_CHECK, CDMA_OK, CDMA_CME, NULL);	
}
/////////////////////////////////////////////////////////////
//  DTU拨号连接上报 NG
unsigned char	CMM_DTUPPPOpen( void ){
		
	unsigned char 	nResult = GSM_NG;
	unsigned char   err = 0;
	unsigned char	nCnt = 0;
    unsigned char   nCmdType = 0;
    unsigned short  nTimeOut = 0;
    unsigned char	*lpLargeBuf = NULL;
	
	
	lpLargeBuf	= lpLargeBuf4;
    
	// 做超时机制等信号量	
 	if( CMM_ExecATCmd(CDMA_PPPOPEN, NULL, GPRS_GPRSDATA_CHECK, CDMA_OK, CDMA_CME, NULL) == GSM_SUCCEED ){ 	
		nTimeOut = GPRS_GPRSDATA_CHECK;
		nCnt = 0;
		while( 1 ){
			OSSemPend(pGSMReceiveSem, nTimeOut, &err);
			if( err == OS_TIMEOUT ){
				if ( nCnt++ > 4 ){
					 break;
				}			
			}
			else{
				CMM_ReadCmd( lpLargeBuf, GSM_RECMAXBUFFER );	
				nCmdType = CMM_GetCmdType( lpLargeBuf );
				if ( nCmdType == CDMA_PPPSTATUS ){	
					if( atoi((char*)lpLargeBuf) == 0 ){
						nResult = GSM_SUCCEED;
					}
					else{
						nResult = GSM_NG;
					}
					nTimeOut = GSM_SENDSPACE_CHECK;
				}
				// 收到OK且在之前已收到数据，马上返回
				else if( nCmdType == CDMA_OK ){					
					break;
				}
				else if( nCmdType == CDMA_CME){
					nTimeOut = GSM_SENDSPACE_CHECK;
				}
				else{
					CMM_ParseMasterCmd( lpLargeBuf, nCmdType );
				}
			}	
		}
	}
	return nResult;
}
/////////////////////////////////////////////////////////////
//  DTU关闭连接上报
unsigned char CMM_DTUPPPClose( void ){ //

	return CMM_ExecATCmd(CDMA_PPPCLOSE, NULL, GPRS_GPRSDATA_CHECK, CDMA_OK, CDMA_CME, NULL);
}
/////////////////////////////////////////////////////////////
//  查询DTU拨号连接状态上报
unsigned char	CMM_DTUQueryPPPStatus( void ){
	
	unsigned char 	nResult = GSM_NG;
	unsigned char 	nReturnMsg[20];

	if ( CMM_ExecATCmd(CDMA_PPPSTATUS, NULL, GPRS_GPRSDATA_CHECK, CDMA_PPPSTATUS, CDMA_CME, NULL) == GSM_SUCCEED ){
		 nResult = atoi( (char*)nReturnMsg );
		 if ( 0 == nResult ){
		 	nResult = GSM_SUCCEED;
		 }
		 else{
		 	nResult = GSM_NG;	
		 }
	}
	return nResult;	
}

/////////////////////////////////////////////////////////////////
// 解释AT指令格式
// 返回命令类型, 并取出命令参数
unsigned char CMM_GetCmdType(unsigned char *pMsg){
	
	unsigned char 	nCnt = 0;
	unsigned char		nLen = 0;
	unsigned char 	*pTmp = NULL;
	
	for( nCnt = 0; nCnt < CMM_CMD_END; nCnt++ ){		
		nLen = strlen( s_strCMMCMD2[nCnt] );
	 	if( memcmp( pMsg, s_strCMMCMD2[nCnt], nLen ) == 0 ){
	 		
	 		pTmp = (unsigned char*)strchr( (char*)pMsg, ':');
	 		if( pTmp ){
	 			pTmp++;
	 			strcpy( (char*)pMsg, (char*)pTmp );
	 		}	 		
	 		break;		 		
	 	}	 	
	}
	if ( nCnt >= CMM_CMD_END ){
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
	}
	return nCnt;
}
/////////////////////////////////////////////////////////////////////////
// 检测卡是否就绪
unsigned char CMM_CheckSIMReady( ){ 
		
	unsigned char 	nResult = 0;
	unsigned char 	nReturnMsg[20];
	
	if( CMM_ExecATCmd( CDMA_CPIN, (unsigned char *)TEXT_QUESTION, GSM_ATTIME_CHECK, CDMA_CPIN, CDMA_CME, nReturnMsg ) == GSM_SUCCEED ){
		if( strstr( (char*)nReturnMsg, (char*)"READY" ) != NULL ){
			 nResult = 1;
		}		
	} 
	return nResult;
}
//////////////////////////////////////////////////////////
//
unsigned char CMM_QueryModuleSN( unsigned char *pStr ){	
	
	return CMM_ExecATCmd( CDMA_GSN, NULL, GSM_ATTIME_CHECK, CDMA_GSN, CDMA_CME, pStr );
}
/////////////////////////////////////////////////////////////////////////
// 打电话                                                                                                                                                                                                                                  
unsigned char CMM_DialNumber( unsigned char *pStrNo ){
	
	unsigned char		nBuf[50];
	unsigned char		nResult;
	
	OSTimeDly(200);	
	sprintf((char*)nBuf, "%s", (char*)pStrNo ); 
	nResult = CMM_ExecATCmd(CDMA_CDV, nBuf, GSM_ATTIME_CHECK, CDMA_CORG, CDMA_CME, NULL);	
	if( nResult == GSM_SUCCEED ){
		CMM_SetCallOnLine( );		
	};
	return nResult;
}
//////////////////////////////////////////////////////////////
// 接电话
unsigned char CMM_AcceptCall( void ){
	
	return CMM_ExecATCmd(CDMA_QCCAV, NULL, GSM_ATTIME_CHECK, CDMA_OK, CDMA_CME, NULL);	
}
//////////////////////////////////////////////////////////////
//  挂电话
unsigned char CMM_RejectCall( void ){
	
	return CMM_ExecATCmd(CDMA_CHV, NULL, GSM_ATTIME_CHECK, CDMA_OK, CDMA_CME, NULL);	
}
////////////////////////////////////////////////////////////////////////////
// 返回信号级别
unsigned char CMM_QuerySignal( void ){
	
	unsigned char   nReturnMsg[20];
	
	
	unsigned char	nResult = 0;
	
	if( CMM_ExecATCmd( CDMA_CSQ, (unsigned char*)TEXT_QUESTION, GSM_ATTIME_CHECK, CDMA_CSQ, CDMA_CME, nReturnMsg) == GSM_SUCCEED ){
		nResult = atoi( (char*)nReturnMsg );
		if( nResult == 99 ){
			 nResult = 0;
		}
	}
	return nResult;
}
//	查询地址小区ID  +CREG: 2,1,"2483","0E9D"
unsigned char	CMM_QueryNetworkLac( int	*pLac, int	*pID ){

	return GSM_NG;
}
////////////////////////////////////////////////////////////////////////////
//  删除短信
void CMM_DeleteAllSMS( void ){
	
	unsigned char	 nCnt = 0;
	
	for( nCnt = 1; nCnt < 100; nCnt++ ){
		if( CMM_DeleteSMS(nCnt) != GSM_SUCCEED ){
			break;
		} 
	}
}
//////////////////////////////////////////////////////////////
// 删除短信
unsigned char CMM_DeleteSMS( unsigned short nCnt ){	
				
	return CMM_ExecATCmdN(CDMA_CMGD, nCnt, GSM_ATTIME_CHECK, CDMA_CMGD, CDMA_CMS, NULL);		
}															 															 
//////////////////////////////////////////////////////////////
// ASCII bit
unsigned char CMM_SendASCIIData( unsigned char *pCaller, 
								 unsigned char *pMsg, 
								 unsigned char nLen ){	

	//  设置短信发送方式
	CMM_ExecATCmd( CDMA_SMSSS, (unsigned char*)"=0,0,1,0", GSM_ATTIME_CHECK, CDMA_OK, CDMA_CME, NULL );								 
	if( nLen > 157 ){
		nLen = 157;
	}

	return CMM_SendSMS( pCaller, pMsg, nLen );
}
//////////////////////////////////////////////////////////////
// OCTET
unsigned char CMM_SendOctetData( unsigned char *pCaller, 
								 unsigned char *pMsg, 
								 unsigned char nLen ){		
	//  设置短信发送方式
	CMM_ExecATCmd( CDMA_SMSSS, (unsigned char*)"=0,0,3,0", GSM_ATTIME_CHECK, CDMA_OK, CDMA_CME, NULL );								 
	if( nLen > 137 ){
		nLen = 137;
	}
	return CMM_SendSMS( pCaller, pMsg, nLen);
}
//////////////////////////////////////////////////////////////
// unicode
unsigned char CMM_SendUnicodeData( unsigned char *pCaller, 
								   unsigned char *pMsg, 
								   unsigned char nLen ){																 	 	

		//  设置短信发送方式
	CMM_ExecATCmd( CDMA_SMSSS, (unsigned char*)"=0,0,6,0", GSM_ATTIME_CHECK, CDMA_OK, CDMA_CME, NULL );								   
	if( nLen > 140 ){
		nLen = 140;
	}
	return CMM_SendSMS( pCaller, pMsg, nLen);											 	
}
///////////////////////////////////////////////////////////////
// 发送 GSM短信
// send:
// AT+CMGS="Phone Number"<CR><LF>
// recv:
// >
// send:
// pdu ^Z<CR><LF>
// recv1:OK
// recv2:%CMGSS:xx<CR><LF><CR><LF>  (option)
// recv3:OK
unsigned char CMM_SendSMS( unsigned char *pCaller,
						   unsigned char *pMsg,  
						   unsigned char nDataLen ){

	unsigned char 	nResult  = GSM_NG; 
	unsigned char 	err 	 = 0;	
	unsigned char 	nCnt 	 = 0;
	unsigned char 	nCmdType = 0;
	unsigned short	nTimeout = 0;
	unsigned char	*lpLargeBuf = NULL;
	
	
	lpLargeBuf	= lpLargeBuf4;	
	
	CMM_SetSendSMSUnReady( );	
	sprintf((char*)lpLargeBuf, "=\"%s\"", pCaller);
	CMM_SendATCmd( CDMA_CMGS, 1, lpLargeBuf );	
	nTimeout = GSM_SENDSPACE_CHECK;
	while ( 1 ){
		OSSemPend(pGSMReceiveSem, nTimeout, &err);
						
		if( err == OS_TIMEOUT ){	
			if( CMM_IsSendSMSReady( ) ){						
				CMM_SetSendSMSUnReady( );			
				GSMUARTSendData(pMsg, nDataLen);
				GSMUARTSendStr((unsigned char*)TEXT_SMSSEND_END);
				GSMUARTSendStr((unsigned char*)TEXT_LF_RN);			
				while( !GSMUARTIsSendBufEmpty( ) ){
					OSTimeDly( 2 );	
				}
				nTimeout = GSM_ATTIME_CHECK;
				nCnt = 0;
				continue;
			}  
			else{	
				if( nCnt++ >= 4 ){
					break;
				}
			}
		}		
		else{		
			CMM_ReadCmd( lpLargeBuf , GSM_RECMAXBUFFER );
			nCmdType = CMM_GetCmdType(lpLargeBuf);		 	
			if(nCmdType ==  CDMA_CMGSS){
				nTimeout = GSM_SENDSPACE_CHECK; 				
				nResult = GSM_SUCCEED;
				nCnt = 0;
			} 	
			else if( nCmdType == CDMA_CME ){
				nResult = GSM_NG;	
				break;
			}
			else if( nCmdType == CDMA_CMGSF ){
				nResult = GSM_NG;	
				break;
			}
			else if( nCmdType == CDMA_OK ){
				if( nResult == GSM_SUCCEED ){
					break;
				}
				else{
					nTimeout = GSM_ATTIME_CHECK; 
					nCnt = 0;
				}
			}			
			else{
				CMM_ParseMasterCmd( lpLargeBuf, nCmdType );
			}
		}	
	}		
					
	return nResult;
}
//////////////////////////////////////////////////////////////////
// 发送GPRS数据 0: 失败  1:  成功
// send:
// AT+IPSEND=通道,长度
// recv1:ok
// send data
// $HUDPSEND:通道,长茺
unsigned char IP_SendData( unsigned char nMode,
						   unsigned char *pBuf, 
						   unsigned char nLen){
			
	unsigned char  		nResult = GSM_NG;	
	unsigned char		nCnt = 0;
	unsigned short		nTimeOut = 0;
	unsigned char  		err = 0;
	unsigned char		nCMError = 0;
	unsigned char  		nCmdType = 0;
	unsigned char		*lpLargeBuf = NULL;
	
	
	lpLargeBuf	= lpLargeBuf4;	
	
	if ( nMode ){ //TCP
		 sprintf((char*)lpLargeBuf, "=1,%d", nLen );
		 CMM_SendATCmd( CDMA_TCPSEND, 1, lpLargeBuf );
	}
	else{         //UDP
		 sprintf((char*)lpLargeBuf, "=2,%d", nLen );
		 CMM_SendATCmd( CDMA_UDPSEND, 1, lpLargeBuf );
	}	
	nTimeOut = GSM_ATTIME_CHECK;
	nCnt = 0;
	while( 1 ){
		OSSemPend(pGSMReceiveSem, nTimeOut, &err); 
		if (err == OS_TIMEOUT){
			if ( nCnt++ > 2 ){
				break;
			}			
		}
		else{		
			CMM_ReadCmd( lpLargeBuf , GSM_RECMAXBUFFER );		
			nCmdType = CMM_GetCmdType(lpLargeBuf);							
			if( nCmdType == CDMA_OK )	{
				if ( 0 == nCMError ){
					nResult = GSM_SUCCEED;
				}
				break;
			}		
			else if( CDMA_CME == nCmdType ){
				nCMError = 1;
			}
		}
	}
	// 可以发送数据
	if ( nResult == GSM_SUCCEED ){
		
		GSMUARTSendData(pBuf, nLen);
		GSMUARTSendStr((unsigned char *)TEXT_LF_RN);			
		while( !GSMUARTIsSendBufEmpty( ) ){
			OSTimeDly( 2 );	
		}
		nTimeOut = GPRS_GPRSDATA_CHECK;
		nCnt = 0;
		while( 1 ){
			OSSemPend(pGSMReceiveSem, nTimeOut, &err); 		
			nTimeOut = GSM_ATTIME_CHECK;
			if (err == OS_TIMEOUT){
				if ( nCnt++ > 4 ){
					break;
				}			
			}
			else{
					CMM_ReadCmd( lpLargeBuf , GSM_RECMAXBUFFER);		
					nCmdType = CMM_GetCmdType(lpLargeBuf);	
					if( nCmdType == CDMA_TCPSEND )	{
						nResult = GSM_SUCCEED;
						break;
					}
					else if( nCmdType == CDMA_UDPSEND ){
						 nResult= GSM_SUCCEED;
						 break;
					}
					else if( nCmdType == CDMA_CME ){
						 nResult = GSM_NG;
						 break;
					}
					else{
						 CMM_ParseMasterCmd( lpLargeBuf, nCmdType );
					}
			}			
		}
	}	
	return nResult;	
}
///////////////////////////////////////////////////
// 提取短信内容
// send: at+CMGR=x
// recv1:
// pCodeType: 0: ASCII
//			  1: OCT
//            2: UNICODE
unsigned char CMM_ReadSMS( unsigned char nCnt, 
						   unsigned char *pCaller, 
						   unsigned char *pMsg,
						   unsigned char *pCodeType ){
													 	
   unsigned char  err = 0;
   unsigned char  nLen = 0;
   unsigned char  nRecordCnt = 0;
   unsigned char  nRecvLen = 0;
   unsigned char  nHsmsrd = 0;
   unsigned char  nCmdType = 0;	 

   unsigned short nTimeOut = 0;
   unsigned short nCntTmp = 0;
   unsigned char  nCmdBuf[20];

 //  if ( nCnt > 0 ){
 //		nCnt -= 1;
 //  }
   sprintf((char*)nCmdBuf, "=%d", nCnt );
   CMM_SendATCmd( CDMA_CMGR, 0, nCmdBuf );
   // 读取短信表头	
	//         <callerid>   <date>			 <time><lang><fmt><length><prt优先级><prv保密级>
	// $HSMSRD: 13480924411, <2009/03/28> <23:09>, 0, 1, 14, 1, 0
	// *excute**888##
	// 1: ascii  (length:0~160)(还需将GSM编码变成ascii)
	// 2: unicode(length:0~70)
	// 3: 8位编码(length:0~140)
	 nLen = 0;	
	 nTimeOut = GSM_ATTIME_CHECK;
	 nCnt = 0;
	 nRecordCnt = 0;
	 while( 1 ){
	 		OSSemPend(pGSMReceiveSem, nTimeOut, &err);
	 		if( err == OS_TIMEOUT ){
				if ( nRecordCnt++ > 4 ){
					 break;
				}			
			}
			else{
				nCntTmp = CMM_ReadCmd((unsigned char *)pMsg+nLen, GSM_RECMAXBUFFER);	
				nCmdType = CMM_GetCmdType( pMsg+nLen );
				if ( nCmdType == CDMA_CMGR ){
					CMM_ParseSMSFormatA( pCaller, pMsg+nLen,  pCodeType, &nRecvLen );									
					nHsmsrd = 1;						
					nTimeOut = GSM_SENDSPACE_CHECK;
				}
				else if( nCmdType == CDMA_HSMSRD ){										
					CMM_ParseSMSFormatB( pCaller, pMsg+nLen,  pCodeType, &nRecvLen );									
					nHsmsrd = 1;						
					nTimeOut = GSM_SENDSPACE_CHECK;
				}				
				// 收到OK且在之前已收到数据，马上返回
				else if( nCmdType == CDMA_OK ){
					if((nLen > 0) && (nHsmsrd == 1) ){
						 break;
					}
				}
				else if( nCmdType == CDMA_CME){
					break;				
				}
				else if( nCmdType == CMM_CMD_END ){
								
				   if ( (nHsmsrd == 1) && (nCntTmp > 0) ){				   								
						// ASCII
						nLen = nCntTmp;
						/*
						if ( *pCodeType == 0 ){
						   nLen = P_GSMToASCII( (char*)pMsg+nLen, (char*)pMsg+nLen, nCntTmp);
						}
						else if( *pCodeType == 1 ){
							nLen = nCntTmp;
						}
						else if ( *pCodeType == 2 ){
							nLen = nCntTmp * 2;
						}		
						*/				
						nTimeOut = GSM_SENDSPACE_CHECK;
					}
			   }
			   else{
				 CMM_ParseMasterCmd( pMsg+nLen, nCmdType );
			   }
			}
	 }
	 return  nLen;
}
//  解释短信格式
///////////////////////////////////////////////////
// %CMGR:13760374664,2009,04,06,10,04,1,4,1,0,0
unsigned char CMM_ParseSMSFormatA( unsigned char *pCaller, 
								  unsigned char *pMsg,  
								  unsigned char *pCodeType,
								  unsigned char *pDataLen ){ 
	unsigned char		nType = 0;
	unsigned char		nCnt = 0;
	unsigned char		*pch = NULL;							  
		
	if( *pMsg == ' ' ){
		pMsg += 1;
	}
	pch = (unsigned char*)strchr( (char*)pMsg, ',');
	if ( pch == NULL ){
		return GSM_NG;
	}
	*pch = 0x00;
	strcpy( (char*)pCaller, (char*)pMsg);
	pMsg = pch + 1;
	
	while( nCnt++ <= 4 ){
		pch = (unsigned char*)strchr( (char*)pMsg, ',');
		if ( pch == NULL ){
			return GSM_NG;
		}
		pMsg = pch + 1;
	}
	nType = atoi( (char*)pMsg );
	switch( nType ){
	case 1: //ASCII
		*pCodeType = 0;
		break;
				
	case 6: // UNICODE
		*pCodeType = 2;
		break;
		
	case 3: // OCTET
	default:
		*pCodeType = 1;
		break;
	}		
	pMsg = pch + 1;
	pch = (unsigned char*)strchr((char*)pMsg, ',');
	if( pch == NULL ){
		return GSM_NG;
	}
	*pch = 0x00;		
	*pDataLen = atoi((char*)pMsg );		
	
	return GSM_SUCCEED;								  
}
///////////////////////////////////////////////////
//  解释短信格式
//  $HSMSRD: 13480924411, <2009/03/28> <23:09>, 0, 1, 14, 1, 0
//           13480924411, <2009/03/28> <23:09>, 0, 1, 14, 1, 0
unsigned char CMM_ParseSMSFormatB( unsigned char *pCaller, 
								  unsigned char *pMsg,  
								  unsigned char *pCodeType,
								  unsigned char *pDataLen ){
	unsigned char		nType = 0;
	unsigned char		*pch = NULL;
	
	
	if( *pMsg == ' ' ){
		pMsg += 1;
	}
	pch = (unsigned char*)strchr( (char*)pMsg, ',');
	if ( pch == NULL ){
		return GSM_NG;
	}
	*pch = 0x00;
	strcpy( (char*)pCaller, (char*)pMsg);
	pMsg = pch + 1;
	// 日期 时间
	// <2009/03/28> <23:09>, 0, 1, 14, 1, 0
	pch = (unsigned char*)strchr( (char*)pMsg, ',');
	if( pch == NULL ){
		return GSM_NG;
	}
	pMsg = pch + 1;
	// 语言
	//  0, 1, 14, 1, 0
	pch = (unsigned char*)strchr( (char*)pMsg, ',');
	if( pch == NULL ){
		return GSM_NG;
	}
	pMsg = pch + 1;
	// 编码格式
	
	// 1, 14, 1, 0
	pch = (unsigned char*)strchr( (char*)pMsg, ',');
	if( pch == NULL ){
			return GSM_NG;
	}
	*pch = 0x00;
	nType = atoi( (char*)pMsg );
	switch( nType ){
	case 1: //ASCII
		*pCodeType = 0;
		break;
				
	case 6: // UNICODE
		*pCodeType = 2;
		break;
		
	case 3: // OCTET
	default:
		*pCodeType = 1;
		break;
	}		
	pMsg = pch + 1;
	
	// 数据长度
	
	// 14, 1, 0
	pch = (unsigned char*)strchr((char*)pMsg, ',');
	if( pch == NULL ){
		return GSM_NG;
	}
	*pch = 0x00;		
	*pDataLen = atoi((char*)pMsg );		
	return GSM_SUCCEED;
}
///////////////////////////////////////////////////////////////
//  模块锁频
unsigned char	CMM_ConfigCBand( void ){

	return 0;
}
/////////////////////////////////////////////////////////////
// 返回:				0： free
//						1:  on use
unsigned char GSM_ReadCaller( unsigned char *pCaller ){
	
	unsigned char			nResult = 0;
	
	if( g_stGSMState.m_nDialLatLng > 0 ){
	
		 nResult = 1;
	}
	return nResult;	
}
// 解析并存储最新信息
void CMM_ParseMasterCmd(unsigned char *pMsg, unsigned char nCmdType){
	
	unsigned char		nCnt = 0;
	unsigned short		nLen;
	unsigned char 		nCaller[TELNUMBER_LENGTH+5]; 
	unsigned char		*pTmp = NULL;
	

	switch(nCmdType){
	//case CDMA_RING:
	case CDMA_CID:
		nCaller[0] = 0x00;
		CMM_ReadCallerID( pMsg, nCaller );
		if( strlen( (char*)nCaller) > 0 ){
			if( IsAuthorizationNo( (unsigned char*)nCaller) ){
							
				CMM_AcceptCall( );		
				CMM_SetCallOnLine( );								
						
				#if( HW_ET201B || HW_ET201B_2 || HW_ET201B_3 )	
				if(memcmp("669475", nCaller, PHONE_COMPARE_BIT) == 0 ){
					
					HW_PORT_SPKMUTE_OPEN(); // 打开静音
				}
				else{ 
					#if( DIAL_CALL )
					HW_PORT_SPKMUTE_OPEN(); // 打开静音
					#else	
					HW_PORT_SPKMUTE_CLOE(); // 关闭静音	
					#endif 
				}
				#endif 
			}
			else{
				CMM_RejectCall( );
				CMM_SetCallOffLine( );		
			}
		}	
		break; 	
	case CDMA_CLIP:
		nCaller[0] = 0x00;
		CMM_ReadCallerCLIP(pMsg, nCaller);
		if( strlen( (char*)nCaller) > 0 ){
					
			if( IsAuthorizationNo( (unsigned char*)nCaller) ){						
						
				if(memcmp("669475", nCaller, PHONE_COMPARE_BIT) == 0 ){
					
					g_stGSMState.m_nDialLatLng = 0;
					CMM_AcceptCall( );		
					CMM_SetCallOnLine( );
				}
				else{				
				//	if( g_stGSMState.m_nDialLatLng++ >= 3 ){
					
						g_stGSMState.m_nDialLatLng = 0;
						CMM_AcceptCall( );		
						CMM_SetCallOnLine( );
				//	}					
					// 取电话号码
				//	nCnt = strlen( (char*)nCaller);							
				//	memcpy( g_stGSMState.m_strDialNo, nCaller, nCnt );
				//	g_stGSMState.m_strDialNo[nCnt] = 0x00;
				}				
				
				#if( HW_ET201B || HW_ET201B_2 || HW_ET201B_3 )	
				if(memcmp("669475", nCaller, PHONE_COMPARE_BIT) == 0 ){
					
					HW_PORT_SPKMUTE_OPEN(); // 打开静音
				}
				else{ 
					#if( DIAL_CALL )
					HW_PORT_SPKMUTE_OPEN(); // 打开静音
					#else	
					HW_PORT_SPKMUTE_CLOE(); // 关闭静音	
					#endif 
				}
				#endif
				
			}
			else{
				CMM_RejectCall( );
				CMM_SetCallOffLine( );		
			}
		}	
		break;		
						
	case CDMA_CMTI:		
		pTmp = (unsigned char *)strstr((char *)pMsg, ",");
		if( pTmp ){
			pTmp++;
			nCnt = atoi( (char*)pTmp );
			InsertNewMsg( nCnt );
		}
		break;
				
	case CDMA_TCPRECV:	
		nLen = IP_RecvTCPData( pMsg, pMsg, NULL ); 		
		nLen = PL_ParseData( pMsg, pMsg, nLen );	
		if( nLen ){
			IP_SendData( IP_IsTCPCommMode(), pMsg, nLen );
		}
		break;	
	case CDMA_UDPRECV:
		nLen = IP_RecvTCPData(pMsg, pMsg, NULL );
		nLen = PL_ParseData( pMsg, pMsg, nLen );	
		
		if( nLen ){
			IP_SendData( IP_IsTCPCommMode(), pMsg, nLen );
		}
		break;
		
	// 网络断开
	// 或
	// 通话结束
	// 通话桂机
	// 无人接听
	case CDMA_CEND:
		nLen = CMM_ParseHCEnd( pMsg );
		if( nLen == 0 ){
			// 关闭静音
			#if( HW_ET201B || HW_ET201B_2 || HW_ET201B_3 )	
	    HW_PORT_SPKMUTE_CLOE();
	    #endif
			CMM_SetCallOffLine( );
			g_stGSMState.m_nDialLatLng = 0;
		}
		else if( nLen == 1 ){
			IP_SetConnectNG( );			
		}
		break;		
	//	连接请求
	case GSM_TECNT:
		CMM_SendTECNT();
		nLen = PL_ReadConfig( pMsg );		
		if( nLen > 0 ){	
			sprintf( (char*)nCaller, "AT%s:", s_strCMMCMD[GSM_TESETUP] );		
			GSMUARTSendStr( nCaller );
			GSMUARTSendData( pMsg, nLen );
			GSMUARTSendStr( (unsigned char*)"\r\n" ); 
		}
		break;
	//	挂断请求
	case GSM_TEDISCNT:
		GSM_SendDISCNT();
		break;
	//	l读取/设置 设备参数
	case GSM_TESETUP:
		if( *pMsg == '?' ){
			nLen = PL_ReadConfig( pMsg );
		}
		else{
			nLen = PL_ParseConfig(pMsg, pMsg );
			nCnt = 1;			
		}
		if( nLen > 0 ){	
			sprintf( (char*)nCaller, "AT%s:", s_strCMMCMD[GSM_TESETUP] );		
			GSMUARTSendStr( nCaller );
			GSMUARTSendData( pMsg, nLen );
			GSMUARTSendStr( (unsigned char*)"\r\n" ); 
		}
		//写FLASH
		if( nCnt == 1 ){		
			GSM_WriteSetupData();
			WriteAlarmSetupData(); 
		}
		break;	
	default:
		break; 
	}
	return;
}
//////////////////////////////////////////////////////////////
//	发送连接成功,关闭GSM模块电源
void	CMM_SendTECNT( void ){

	GSM_SetSetupConfig();
	CMM_ExecATCmd(GSM_TECNT, NULL, GSM_SENDSPACE_CHECK, CDMA_OK, CDMA_CME, NULL );
}
//////////////////////////////////////////////////////////////
//	发送断开连接,开启GSM模块电源
void 	GSM_SendDISCNT( void ){

	CMM_ExecATCmd( GSM_TEDISCNT, NULL, GSM_SENDSPACE_CHECK, CDMA_OK, CDMA_CME, NULL );
	TE_SetNeedTEReset();
	GSM_ClrSetupConfig();
}
/////////////////////////////////////////////////////////////////////
// 接收到GPRS数据
// $HUDPRECV: 3, 4  $HTCPRECV: 3, 4
// <CR><LF>
// 1 0‰(0x31,0x20,0x30,0x89)
unsigned char IP_RecvTCPData(unsigned char  *pMsg,
							 unsigned char  *pDest,
							 unsigned char  *pSocketNum ){
							
	unsigned char	nDataLen = 0;
	//unsigned char   err = 0;		
	unsigned char	*pch = NULL;
			
	pch = (unsigned char*)strchr((char*)pMsg, ',');
	if ( pch != NULL ){
	
		// SOCKET通道
		if( pSocketNum ){
			*pSocketNum = atoi((char*)pMsg);
		}		
		pch += 1;
		
		// 数据长度
		nDataLen = atoi( (char*)pch );
		
			
	//	OSSemPend(pGSMReceiveSem, GSM_SENDSPACE_CHECK, &err);
	//	if( err != OS_TIMEOUT ){
			// 读空<CR><LF>	
	//		CMM_ReadCmd((unsigned char *)pDest, GSM_RECMAXBUFFER);
			OSTimeDlyHMSM(0,0,0,nDataLen*10);
			CMM_ReadCmd((unsigned char *)pDest, nDataLen);
	//	}	
	}
	return nDataLen;						
}

/////////////////////////////////////////////////////////////////////
// 解释来电显示
// CID: 13480924411
void CMM_ReadCallerID( unsigned char *pMsg, unsigned char *pCaller ){
	
	if( *pMsg == ' ' ){
		 pMsg += 1;
	}				 
	strcpy( (char*)pCaller, (char*)pMsg );
	return;
}
/////////////////////////////////////////////////////////////////////
// 解释来电显示
//+CLIP:13480924411,0
void CMM_ReadCallerCLIP( unsigned char *pMsg, unsigned char *pCaller ){
	
	unsigned char	*pch = NULL;
	
	if( *pMsg == ' ' ){
		 pMsg += 1;
	}
	pch = (unsigned char*)strchr( (char*)pMsg, ',');
	if( pch != NULL ){
		*pch = 0x00;
		strcpy( (char*)pCaller, (char*)pMsg );
	}	
	return;
}
/////////////////////////////////////////////////////////////////////
//  $HCEND: 7, 1, 00:02:59 
// Return :
//       0:  语音呼叫END
//       1:  IP数据END
//       2:  其他END
unsigned char CMM_ParseHCEnd( unsigned char  *pMsg ){
	
	unsigned char		nResult = 2;
	unsigned char		*pch = NULL;
	
	pch = (unsigned char*)strchr( (char*)pMsg, ',');
	if ( pch ){
		pch++;
		nResult = atoi( (char*)pch );
	}
	return nResult;
}
/////////////////////////////////////////////////////////////////
//  查询话费
unsigned char	GSM_ExecCUSD( unsigned char	*pDest, unsigned char	*pSrc, unsigned short	nTimeout ){

	unsigned char		nResult = 2;
	
	
	return nResult;
}
//////////////////////////////////////////////////////////////
//  查询IMEI序号
unsigned char	CMM_QueryIMEI( unsigned  char	*pDest ){
	
	unsigned char		nResult = 2;
	
	
	return nResult;
}

#endif //HW_EM200