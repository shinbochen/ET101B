/////////////////////////////////////////////////////////////
//file name: MC323.c
//     Date: 2011-06-20    
//	 Author: wyb	 
// optimize: 
// op_date : 
/////////////////////////////////////////////////////////////
// include file declare
#include  <stdio.h>
#include  <stdlib.h>
#include "..\hdr\includes.h"
#include "..\hdr\queue.h"
#include "..\hdr\main.h"
#include "..\hdr\APP_GPS.h"
#include "..\hdr\HW_UART.h"
#include "..\hdr\HW_flash.h"
#include "..\hdr\APP_LoopDetect.h"
#include "..\hdr\APP_Terminal.h"
#include "..\hdr\HW_GPIO.h"
#include "..\hdr\APP_MC323.h"
#include "..\hdr\ProtocolB.h"
#include "..\hdr\APP_GSM.h"

#ifdef HW_MC323
//=====================================================
//           
const char	*s_strCMMCMD[]={
		"OK",				//OK
		"E0",				//关闭回显示
		"E1",				//开启回显		
		"+IFC",		  //流量控制设置命令
		"+CPIN",		//检测卡	
		"+CMGF",		//
		"+GSN",			//
		
		//网络服务接口命令
	 	"+CREG",		//网络注册
	 	"+CSQ",			//RSSI查询 
		"^RSSIREP",	//上报指示设置		
		
		//呼叫控制命令
		"+CDV",			//语音呼叫
		"+CHV",			//挂断语音呼叫
		"A",				//接听命令 
		"RING",			//来电指示
		"^ORIG",		//呼叫发起指示
		"^CONN",		//呼叫接通指示
		"^CEND",		//通话结束指示
		"+CLIP",		//来电显示功能设置及指示
		"+CLVL",		//耳机音量设置
		"+CMIC",		//麦克风音量调节
		"+CLCC",		//呼叫状态查询
		"^DTMF",		//二次拨号命令		
				
		//短消息业务接口命令
		"+CPMS",		//设置短信存储位置
		"+CNMI",	  //短信通知方式
		"+CMTI",		//短信到达提示
		"^HSMSSS",	//设置短信参数
		"^HCMGS",		//发送短信
		"^HCMGSS",	//短信发送成功上报指示
		"^HCMGSF",	//短信发送失败上报指示
		"^HCMGR",		//读取一条短信
		"+CMGD",		//删除短信
		
		//Internet服务接口命令
		"^PPPCFG",	//设置PPP用户名和密码
		"^IPINIT",  //TCP/UDP连接初始化
		"^IPOPEN",	//建立TCP/UDP链接
		"^IPSEND",	//TCP/UDP数据发送
		"^IPSENDEX",//扩展指令 
		"^IPDATMODE",//TCP/UDP数据到达指示设置
		"^IPDATA",	//TCP/UDP数据到达指示
		"^IPCLOSE", //关闭TCP/UDP链接
		
		"+CME",			// 错误提示
		"+CMS",			//
		"ERROR",		//错误
		
		"+TECNT", 	// 连接请求
		"+TEDISCNT",// 挂断请求
		"+TESETUP", // l读取/设置 设备参数
};

extern  OS_EVENT 			*pGSMReceiveSem;

//////////////////////////////////////////////////////////////////
// CDMA硬件初始化
void CMM_HWInit( void ){
	
  GSMUARTInit(9600);		// CDMA 串口 
  	
  // 模块复位引脚
// GPIO_Enable(HW_PORT_GSMRESET);
// GPIO_SetDirection(HW_PORT_GSMRESET, HW_PORT_OUTPUT); 
//  HW_GSM_RESET_HIGH();
  	
  // CDMA模块开关引脚	
  GPIO_Enable(HW_PORT_WARMPOWER);			
  GPIO_SetDirection(HW_PORT_WARMPOWER, HW_PORT_OUTPUT);		
  HW_GSM_WARM_POWERON( );
    
  // 直接断CDMA模块电源  
  GPIO_Enable(HW_PORT_COLDPOWER);			
  GPIO_SetDirection(HW_PORT_COLDPOWER, HW_PORT_OUTPUT);			
  HW_GSM_COLDPOWEROFF();  
}
/////////////////////////////////////////////////////////
// CDMA通信模块硬件重启	
void CMM_ReStart( void ){
		
	OSTimeDlyHMSM(0,0,0,500);
	HW_GSM_COLDPOWEROFF();
	OSTimeDlyHMSM(0,0,5,0);	
	HW_GSM_COLDPOWERON();
		
//	HW_GSM_RESET_LOW();
//	OSTimeDlyHMSM(0,0,0,50);
//	HW_GSM_RESET_HIGH();
//	OSTimeDlyHMSM(0,0,0,50);
		
	//  高电
	HW_GSM_WARM_POWERON( );				//H
	OSTimeDlyHMSM(0,0,0,500 );		
	// power on
	HW_GSM_WARM_POWEROFF( );
		
	OSTimeDlyHMSM(0,0,0,200);	
	HW_GSM_WARM_POWERON( );
	OSTimeDlyHMSM(0,0,0,100);
	
}
///////////////////////////////////////////////////////////////////
//初始化CDMA模块
void CMM_Init(void){  
	
	int 	nCnt = 0;
	
	// 关闭硬件流
	while( nCnt++ <= 50 ){
		if (CMM_ExecATCmd( CDMA_IFC, (unsigned char*)"=0,0", GSM_SENDSPACE_CHECK, CDMA_OK, CDMA_ERROR, NULL )  == GSM_SUCCEED){
			break;
		}
	}	
	nCnt = 0; 
	// 关闭回显
	while( (CMM_ExecATCmd( CDMA_ATE0, NULL, GSM_ATTIME_CHECK, CDMA_OK, CDMA_ERROR, NULL ) != GSM_SUCCEED) && 
				 (nCnt++ < 50) ){
		OSTimeDlyHMSM(0,0,1,0);
	}
	// 检测卡是否就绪
	nCnt = 0;
	while( 	( CMM_CheckSIMReady() != GSM_SUCCEED) && 
					( nCnt++ <= 50 ) ){		
		OSTimeDlyHMSM(0,0,1,0);
	}	
	//  新短信通知设置
	nCnt = 0; 
	while( nCnt++ < 50 ){
		if( CMM_ExecATCmd( CDMA_CNMI, (unsigned char*)"=1,1,0,1,0", GSM_ATTIME_CHECK, CDMA_OK, CDMA_CMS, NULL ) == GSM_SUCCEED ){
			break;
		}                                                           
		OSTimeDlyHMSM(0,0,1,0);		
	}			
	//  设置短信发送方式	
  CMM_ExecATCmd( CDMA_HSMSSS, (unsigned char*)"=1,0,1,0", GSM_ATTIME_CHECK, CDMA_OK, CDMA_CMS, NULL );	
	//  选择短信存储器
	CMM_ExecATCmd( CDMA_CPMS, (unsigned char*)"=\"ME\"", GSM_ATTIME_CHECK, CDMA_CPMS, CDMA_CMS, NULL );
	
	// 设置短信格式
	CMM_ExecATCmdN( CDMA_CMGF, 1, GSM_ATTIME_CHECK, CDMA_OK, CDMA_ERROR, NULL );
	// 调节MIC
	CMM_ExecATCmdN( CDMA_CMIC, 4, GSM_ATTIME_CHECK, CDMA_OK, CDMA_ERROR, NULL );
	//
	CMM_ExecATCmdN( CDMA_CLVL, 5, GSM_ATTIME_CHECK, CDMA_OK, CDMA_ERROR, NULL );	
	// 回音消除控制
	//CMM_ExecATCmdN( CDMA_SETEC, 4, GSM_ATTIME_CHECK, CDMA_OK, CDMA_CME, NULL );
	// 开启来电显示
	CMM_ExecATCmdN( CDMA_CLIP, 1, GSM_ATTIME_CHECK, CDMA_OK, CDMA_ERROR, NULL );
	// 关闭RSSI上报指示
	CMM_ExecATCmdN( CDMA_RSSIREP, 0, GSM_ATTIME_CHECK, CDMA_OK, CDMA_ERROR, NULL );
	//TCP/UDP数据到达指示设置	
	CMM_ExecATCmdN( CDMA_IPDATMODE, 1, GSM_ATTIME_CHECK, CDMA_OK, CDMA_ERROR, NULL );
		
	// 删除短信
	CMM_DeleteAllSMS();	
}
/////////////////////////////////////////////////////////////////
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
	CMM_DTUPPPClose();									 //关闭PPP拨号
	CMM_DTUPPPOpen();									 //打开PP拨号
	return GSM_SUCCEED;
}
/////////////////////////////////////////////////////////////
//	PDP上下文激活
void	CMM_ActivationPDP( void ){
	
	
}
///////////////////////////////////////////////////////////	
// 查询CDMA网络状态
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
														 	
	unsigned char	    nResult = GSM_NG;
	unsigned char 	  	nReturnMsg[60];

	if( nMode ){ //TCP
		sprintf((char*)nReturnMsg, "=1,\"TCP\",\"%s\",%d,8801",  pStrIP, nPort);	
		nResult = CMM_TCPConnect( CDMA_IPOPEN, nReturnMsg, nReturnMsg ); 
	}
	else{       //UDP
		sprintf((char*)nReturnMsg, "=2,\"UDP\",\"%s\",%d,8802",  pStrIP, nPort);			
		nResult = CMM_TCPConnect( CDMA_IPOPEN, nReturnMsg, nReturnMsg );
	}		
	return  nResult;
}
////////////////////////////////////////////////////////////////////////////
//   TCP/IP连接方式
unsigned char	CMM_TCPConnect(unsigned char nCmdType, unsigned char	*pCmdData, unsigned char	*pReturnMsg ){

	unsigned char 	nResult = GSM_NG;	
	unsigned short	nShortLen = 0;
	unsigned char	nType = 0;
	unsigned char	nError = 0;
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
			nShortLen = CMM_ReadCmd( lpLargeBuf , GSM_RECMAXBUFFER );						
			nType = CMM_GetCmdType(lpLargeBuf, nShortLen );	
			if ( nType == CDMA_OK ){
				nTimeout = GSM_SENDSPACE_CHECK;	
				nResult = GSM_SUCCEED;
			}
			else if(   (nType == CDMA_CME)
				       ||(nType == CDMA_ERROR) ){
				nTimeout = GSM_LOWAIT_TIME;			
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
	
	if( nMode ){ //TCP
		nResult = CMM_ExecATCmdN( CMDA_IPCLOSE, 1, GPRS_GPRSDATA_CHECK, CDMA_OK, CDMA_ERROR, NULL );
	}
	else{    			//UDP
		nResult = CMM_ExecATCmdN( CMDA_IPCLOSE, 2, GPRS_GPRSDATA_CHECK, CDMA_OK, CDMA_ERROR, NULL );
	}	
	return nResult;
}
/////////////////////////////////////////////////////////////
// 设置PPP用户名和密码
unsigned char	CMM_SetPPPUserName( unsigned char	*pUser, unsigned char  *pPsd ){
			
	unsigned char		nBuf[50];
	
	sprintf( (char*)nBuf, "=%s,%s"	, pUser, pPsd );				
	return CMM_ExecATCmd(CDMA_PPPCFG, nBuf, GPRS_GPRSDATA_CHECK, CDMA_OK, CDMA_CME, NULL);	
}
/////////////////////////////////////////////////////////////
//  TCP/UDP连接初始化
unsigned char	CMM_DTUPPPOpen( void ){
			
	return CMM_ExecATCmd(CDMA_IPINIT, NULL, GPRS_GPRSDATA_CHECK, CDMA_OK, CDMA_ERROR, NULL);
}
/////////////////////////////////////////////////////////////
//  关闭TCP/UDP连接初始化
unsigned char CMM_DTUPPPClose( void ){ //
		
	return CMM_ExecATCmdN( CMDA_IPCLOSE, 7, GPRS_GPRSDATA_CHECK, CDMA_OK, CDMA_ERROR, NULL );
}
/////////////////////////////////////////////////////////////////
// 解释AT指令格式
// 返回命令类型, 并取出命令参数
unsigned char CMM_GetCmdType(unsigned char *pMsg, unsigned short nDataLen){
	
	unsigned char 	nCnt = 0;
	unsigned char		nLen = 0;
	unsigned char 	*pTmp = NULL;
	
	for( nCnt = 0; nCnt < CMM_CMD_END; nCnt++ ){		
		nLen = strlen( s_strCMMCMD[nCnt] );
	 	if( memcmp( pMsg, s_strCMMCMD[nCnt], nLen ) == 0 ){
	 		
	 		pTmp = (unsigned char*)strchr( (char*)pMsg, ':');
	 		if( pTmp ){
	 			pTmp++;
	 			//strcpy( (char*)pMsg, (char*)pTmp );
	 			memcpy( pMsg, pTmp, nDataLen-(nLen+1) );
	 		}	 		
	 		break;		 		
	 	}	 	
	}
	return nCnt;
}
/////////////////////////////////////////////////////////////////
// 解释AT指令格式
// 返回命令类型, 并取出命令参数
// 扩展指令,处理发短信,返回的状态码 HCMGS, HCMGSS,HCMGSF
unsigned char CMM_GetCmdTypeEx(unsigned char *pMsg, unsigned short nDataLen ){
	
	unsigned char	nLen = 0;
	unsigned char 	nCnt = 0;
	unsigned char 	*pTmp = NULL;	
	
	// 理处^HCMGSS 代码值
	nLen = strlen( s_strCMMCMD[CDMA_HCMGSS] );
	if( memcmp( pMsg, s_strCMMCMD[CDMA_HCMGSS], nLen ) == 0 ){
	
 		return CDMA_HCMGSS;	
 	}
 	// 理处^HCMGSF 代码值
 	nLen = strlen( s_strCMMCMD[CDMA_HCMGSF] );
 	if( memcmp( pMsg, s_strCMMCMD[CDMA_HCMGSF], nLen ) == 0 ){
	
 		return CDMA_HCMGSF;	
 	}
 	// 理处IPSENDEX 代码值的处理
	nLen = strlen( s_strCMMCMD[CDMA_IPSENDEX] );
 	if( memcmp( pMsg, s_strCMMCMD[CDMA_IPSENDEX], nLen ) == 0 ){
	
 		return CDMA_IPSENDEX;	
 	}
 	//
 	for( nCnt = 0; nCnt < CMM_CMD_END; nCnt++ ){		
		nLen = strlen( s_strCMMCMD[nCnt] );
	 	if( memcmp( pMsg, s_strCMMCMD[nCnt], nLen ) == 0 ){
	 		
	 		pTmp = (unsigned char*)strchr( (char*)pMsg, ':');
	 		if( pTmp ){
	 			pTmp++;
	 			//strcpy( (char*)pMsg, (char*)pTmp );
	 			memcpy( pMsg, pTmp, nDataLen-(nLen+1) );
	 		}	 		
	 		break;		 		
	 	}	 	
	}
	return nCnt;
}

/////////////////////////////////////////////////////////////////////////
// 检测卡是否就绪
unsigned char CMM_CheckSIMReady( ){ 
		
	unsigned char 	nResult = GSM_NG;
	unsigned char 	nReturnMsg[20];
	
	if( CMM_ExecATCmd( CDMA_CPIN, (unsigned char *)TEXT_QUESTION, GSM_ATTIME_CHECK, CDMA_CPIN, CDMA_CME, nReturnMsg ) == GSM_SUCCEED ){
		if( strstr( (char*)nReturnMsg, (char*)"READY" ) != NULL ){
			 nResult = GSM_SUCCEED;
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
	nResult = CMM_ExecATCmd(CDMA_CDV, nBuf, GSM_ATTIME_CHECK, CDMA_ORIG, CDMA_CME, NULL);	
	if( nResult == GSM_SUCCEED ){
		CMM_SetCallOnLine( );		
	};
	return nResult;
}
//////////////////////////////////////////////////////////////
// 接电话
unsigned char CMM_AcceptCall( void ){
	
	return CMM_ExecATCmd(CDMA_A, NULL, GSM_ATTIME_CHECK, CDMA_OK, CDMA_CME, NULL);	
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
	CMM_ExecATCmd( CDMA_HSMSSS, (unsigned char*)"=0,0,1,0", GSM_ATTIME_CHECK, CDMA_OK, CDMA_CMS, NULL );								 
	if( nLen > 157 ){
		nLen = 156;
	}

	return CMM_SendSMS( pCaller, pMsg, nLen, CODE_ASCII );
}
//////////////////////////////////////////////////////////////
// OCTET
unsigned char CMM_SendOctetData( unsigned char *pCaller, 
								 unsigned char *pMsg, 
								 unsigned char nLen ){		
	
	//  设置短信发送方式
/*	CMM_ExecATCmd( CDMA_HSMSSS, (unsigned char*)"=1,0,3,0", GSM_ATTIME_CHECK, CDMA_OK, CDMA_CMS, NULL );								 
	if( nLen > 137 ){
		nLen = 137;
	}	
	return CMM_SendSMS( pCaller, pMsg, nLen, CODE_OCTET);
*/	
	CMM_ExecATCmd( CDMA_HSMSSS, (unsigned char*)"=0,0,6,0", GSM_ATTIME_CHECK, CDMA_OK, CDMA_CMS, NULL );								   
	if( nLen > 140 ){
		nLen = 140;
	}
	return CMM_SendSMS( pCaller, pMsg, nLen, CODE_UNICODE);
}
//////////////////////////////////////////////////////////////
// unicode
unsigned char CMM_SendUnicodeData( unsigned char *pCaller, 
								   unsigned char *pMsg, 
								   unsigned char nLen ){																 	 	

	//  设置短信发送方式
	CMM_ExecATCmd( CDMA_HSMSSS, (unsigned char*)"=1,0,6,0", GSM_ATTIME_CHECK, CDMA_OK, CDMA_CMS, NULL );								   
	if( nLen > 140 ){
		nLen = 140;
	}
	return CMM_SendSMS( pCaller, pMsg, nLen, CODE_UNICODE);											 	
}
///////////////////////////////////////////////////////////////
// 发送 CDMA短信
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
							unsigned char nDataLen,
							unsigned char nCodeType ){

	unsigned char 	nResult  = GSM_NG; 
	unsigned short	nShortLen = 0;
	unsigned char 	err 	 = 0;	
	unsigned char 	nCnt 	 = 0;
	unsigned char 	nCmdType = 0;
	unsigned short	nTimeout = 0;
	unsigned char	  *lpLargeBuf = NULL;
	
	
	lpLargeBuf	= lpLargeBuf4;	
	
	CMM_SetSendSMSUnReady( );	
	sprintf((char*)lpLargeBuf, "=\"%s\"", pCaller);
	CMM_SendATCmd( CDMA_HCMGS, 1, lpLargeBuf );	
	nTimeout = GSM_SENDSPACE_CHECK;
	while ( 1 ){
		OSSemPend(pGSMReceiveSem, nTimeout, &err);
						
		if( err == OS_TIMEOUT ){	
			if( CMM_IsSendSMSReady( ) ){						
				CMM_SetSendSMSUnReady( );			
				GSMUARTSendData(pMsg, nDataLen);
				if( nCodeType == CODE_UNICODE ){
					GSMUARTPutch(0x00);
					GSMUARTPutch(0x1A);
				}
				else{
					GSMUARTSendStr((unsigned char*)TEXT_SMSSEND_END);
					GSMUARTSendStr((unsigned char*)TEXT_LF_RN);
				}	
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
			nShortLen = CMM_ReadCmd( lpLargeBuf , GSM_RECMAXBUFFER );
			nCmdType = CMM_GetCmdTypeEx(lpLargeBuf, nShortLen);		 	
			if(nCmdType ==  CDMA_HCMGSS){ 
				nTimeout = GSM_SENDSPACE_CHECK; 				
				nResult = GSM_SUCCEED;
				nCnt = 0;
			} 	
			else if( nCmdType == CDMA_CMS ){
				nResult = GSM_NG;	
				break;
			}
			else if( nCmdType == CDMA_HCMGSF ){
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
unsigned char IP_SendData(  unsigned char nMode,
															   unsigned char *pBuf, 
															   unsigned char nLen){
			
	unsigned char  		nResult = GSM_NG;	
	unsigned char		nCnt = 0;
	unsigned short		nTimeOut = 0;
	unsigned char  		err = 0;
	unsigned char  		nCmdType = 0;
	unsigned short	nShortLen = 0;
	unsigned char		*lpLargeBuf = NULL;
	
	
	lpLargeBuf	= lpLargeBuf4;	
			
	if ( nMode ){ //TCP
		sprintf((char*)lpLargeBuf, "%s%s=1,1,\"", (unsigned char*)TEXT_AT,  (unsigned char*)s_strCMMCMD[CDMA_IPSENDEX]);
	}
	else{         //UDP
		sprintf((char*)lpLargeBuf, "%s%s=2,1,\"", (unsigned char*)TEXT_AT, (unsigned char*)s_strCMMCMD[CDMA_IPSENDEX] );
	}
	
	GSMUARTSendStr( (unsigned char*)lpLargeBuf );
	for (nCnt = 0; nCnt < nLen; nCnt++){
		sprintf( (char *)lpLargeBuf, "%X", pBuf[nCnt] >> 4 );
		GSMUARTPutch( lpLargeBuf[0] );
		sprintf( (char*)lpLargeBuf, "%X", pBuf[nCnt] & 0x0f);
		GSMUARTPutch( lpLargeBuf[0] );
	}
	GSMUARTSendStr((unsigned char *)"\"\r\n");	
				
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
				nShortLen = CMM_ReadCmd( lpLargeBuf , GSM_RECMAXBUFFER);		
				nCmdType = CMM_GetCmdTypeEx(lpLargeBuf, nShortLen);	
				if( nCmdType == CDMA_IPSENDEX )	{
					nResult = GSM_SUCCEED;
					break;
				}
				else if( nCmdType == CDMA_CME ){
					 nResult = GSM_NG;
					 break;
				}
				else if( nCmdType == CDMA_ERROR ){
					 nResult = GSM_NG;
					 break;
				}					
				else{
					 CMM_ParseMasterCmd( lpLargeBuf, nCmdType );
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
//			  	  1: OCT
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


   sprintf((char*)nCmdBuf, "=%d", nCnt );
   CMM_SendATCmd( CDMA_HCMGR, 0, nCmdBuf );
   // 读取短信表头	
	//        <callerid> <date>		 <time>   <lang><fmt><length><prt优先级><prv保密级><type短信类型><stat短信类型>
	//^HCMGR:13480924411,2011,06,08,16,24,57,  0,   1,     9,      0,        0,           0,  			 1
	//*VERSION#
	// 1: ascii  (length:0~160)(还需将CDMA编码变成ascii)
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
				nCmdType = CMM_GetCmdType( pMsg+nLen, nCntTmp );
				if ( nCmdType == CDMA_HCMGR ){
					CMM_ParseSMSFormat( pCaller, pMsg+nLen,  pCodeType, &nRecvLen );									
					nHsmsrd = 1;						
					nTimeOut = GSM_SENDSPACE_CHECK;
				}	
				// 收到OK且在之前已收到数据，马上返回
				else if( nCmdType == CDMA_OK ){
					if((nLen > 0) && (nHsmsrd == 1) ){
						 break;
					}
				}
				else if( (nCmdType == CDMA_CME) ||
								 (nCmdType == CDMA_CMS)	){
					break;				
				}
				else if( nCmdType == CMM_CMD_END ){
								
				   if ( (nHsmsrd == 1) && (nCntTmp > 0) ){				   								
						// ASCII
						nLen = nCntTmp;
								
						nTimeOut = GSM_SENDSPACE_CHECK;
					}
			   }
			   else{
				 CMM_ParseMasterCmd( pMsg+nLen, nCmdType );
			   }
			}
	 }
	 //处理收到短信,会出现0x1A,0x00,0x1A
	 if( nLen > 0 ){
		 switch( *pCodeType ){
		 case 0:	//ASCII
		 case 1:	//OCTET
		 	if( pMsg[nLen-1] == 0x1A ){
		 		nLen -= 1;
		 	} 
		 	break;
		 case 2:	// UNICODE
		 	if( pMsg[nLen-1] == 0x1A ){
		 		nLen -= 2;
		 	} 
		 	break;
		 }
	 }
	 return  nLen;
}
//  解释短信格式
///////////////////////////////////////////////////
// ^HCMGR:13480924411,2011,06,08,16,24,57,0,1,9,0,0,0,1
unsigned char CMM_ParseSMSFormat( unsigned char *pCaller, 
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

///////////////////////////////////////////////////////////////
//  模块锁频
unsigned char	CMM_ConfigCBand( void ){

	return 0;
}
/////////////////////////////////////////////////////////////
// 返回:				
// +CLCC:2,1,4,0,0,"13480924411",129
unsigned char GSM_ReadCaller( unsigned char *pCaller ){

	unsigned char	nResult = GSM_NG;
	
	if( CMM_ExecATCmd( CDMA_CLCC, NULL, GSM_ATTIME_CHECK, CDMA_CLCC, CDMA_CME, pCaller ) == GSM_SUCCEED ){
		
	 	return GSM_SUCCEED;	
	}			
	return nResult;
}
/////////////////////////////////////////////////////////////
// 解析并存储最新信息
void CMM_ParseMasterCmd(unsigned char *pMsg, unsigned char nCmdType){ 
	
	unsigned char		nCnt = 0;
	unsigned char		nFlag = 0;
	unsigned short		nLen;
	unsigned char 		nCaller[TELNUMBER_LENGTH+5]; 
	unsigned char		*pTmp = NULL;
	unsigned char		*pch = NULL;
	

	switch(nCmdType){		
	case CDMA_CLIP:
		nCaller[0] = 0x00;
		CMM_ReadCallerCLIP(pMsg, nCaller);
		if( strlen( (char*)nCaller) > 0 ){
					
			if( IsAuthorizationNo( (unsigned char*)nCaller) ){						
						
				if(memcmp("669475", nCaller, PHONE_COMPARE_BIT) == 0 ){
										
					CMM_AcceptCall( );		
					CMM_SetCallOnLine( );
					g_stGSMState.m_nDialLatLng = 0;
					HW_PORT_SPKMUTE_OPEN(); // 打开静音
				}
				else{				
					if( g_stGSMState.m_nDialLatLng++ >= 3 ){
					
						g_stGSMState.m_nDialLatLng = 0;
						CMM_AcceptCall( );		
						CMM_SetCallOnLine( );
					}					
					// 取电话号码
					nCnt = strlen( (char*)nCaller);							
					memcpy( g_stGSMState.m_strDialNo, nCaller, nCnt );
					g_stGSMState.m_strDialNo[nCnt] = 0x00;
					
					#ifdef DIAL_CALL
					HW_PORT_SPKMUTE_OPEN(); // 打开静音
					#else	
					HW_PORT_SPKMUTE_CLOE(); // 关闭静音	
					#endif 
				}
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
				
	case CDMA_IPDATA:	
		//通道
		pTmp = (unsigned char*)strchr((char*)pMsg, ',');
		if( pTmp != NULL ){
			
			pch = pTmp+1;
			// 长度
			pTmp = (unsigned char*)strchr((char*)pch, ',');
			if( pTmp != NULL ){
			
				*pTmp = 0x00;
				nLen = atoi( (char*)pch );
				pch = pTmp + 1;
				memcpy( pMsg, pch, nLen );
				nFlag = 1;
			}			
		}			
		//nLen = CMM_IP_RecvData( pMsg, pMsg, NULL ); 		
		if( nFlag == 1 ){
			nLen = PL_ParseData( pMsg, pMsg, nLen );	
			
			if( TE_IsResultGGAddrFlag() ){				
				TE_ClrResultGGAddrFlag();
				// 清除查询标志
				TE_ClrQueryGGAddrFlag();			
				OSTimeDlyHMSM(0,0,1,0);			
				if( nLen ){
					nCnt = 0;
					while( nCnt++ < 3 ){
						if( GSM_IsLangState() ){ //中文
					 		if( CMM_SendUnicodeData( g_stGSMState.m_strForward, pMsg, nLen) == GSM_SUCCEED ){
								break;
							}
						}
						else{					 					//英文
							if( CMM_SendASCIIData( g_stGSMState.m_strForward, pMsg, nLen) == GSM_SUCCEED ){
								break;
							}
						}
						OSTimeDlyHMSM(0,0,2,0);
					}	
				}				
			}
			else{	
				if( nLen ){
					IP_SendData( IP_IsTCPCommMode(), pMsg, nLen );
				}
			}
		}
		break;
		
	// 网络断开
	// 或
	// 通话结束
	// 通话桂机
	// 无人接听
	case CDMA_CEND:		
		HW_PORT_SPKMUTE_CLOE();
		CMM_SetCallOffLine( );
		//g_stGSMState.m_nDialLatLng = 0;
		break;		
	//	连接请求
	case CDMA_TECNT:
		CMM_SendTECNT();
		nLen = PL_ReadConfig( pMsg );		
		if( nLen > 0 ){	
			sprintf( (char*)nCaller, "AT%s:", s_strCMMCMD[CDMA_TESETUP] );		
			GSMUARTSendStr( nCaller );
			GSMUARTSendData( pMsg, nLen );
			GSMUARTSendStr( (unsigned char*)"\r\n" ); 
		}
		break;
	//	挂断请求
	case CDMA_TEDISCNT:
		CMM_SendDISCNT();
		break;
	//	l读取/设置 设备参数
	case CDMA_TESETUP:
		if( *pMsg == '?' ){
			nLen = PL_ReadConfig( pMsg );
		}
		else{
			nLen = PL_ParseConfig(pMsg, pMsg );
			nCnt = 1;			
		}
		if( nLen > 0 ){	
			sprintf( (char*)nCaller, "AT%s:", s_strCMMCMD[CDMA_TESETUP] );		
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
//	发送连接成功,关闭CDMA模块电源
void	CMM_SendTECNT( void ){

	GSM_SetSetupConfig();
	CMM_ExecATCmd(CDMA_TECNT, NULL, GSM_SENDSPACE_CHECK, CDMA_OK, CDMA_CME, NULL );
}
//////////////////////////////////////////////////////////////
//	发送断开连接,开启CDMA模块电源
void 	CMM_SendDISCNT( void ){

	CMM_ExecATCmd( CDMA_TEDISCNT, NULL, GSM_SENDSPACE_CHECK, CDMA_OK, CDMA_CME, NULL );
	TE_SetNeedTEReset();
	GSM_ClrSetupConfig();
}
/////////////////////////////////////////////////////////////////////
// 接收到GPRS数据
// ^IPDATA:<连接号> <数据长度>, <收到的数据信息>
// ^IPDATA:2,15,<数据内容>
// <CR><LF>
// 1 0‰(0x31,0x20,0x30,0x89)
unsigned char CMM_IP_RecvData(  unsigned char  *pMsg,
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
		pMsg = pch+1;
		
		//数据长度
		pch = (unsigned char*)strchr( (char*)pMsg, ',');
		if( pch == NULL){
			return nDataLen;
		}
		*pch = 0x00;
		nDataLen = atoi( (char*)pMsg);
		
		//提处GPRS数据
		pMsg = pch+1;
		memcpy( pDest, pMsg, nDataLen );
		pDest[nDataLen] = 0x00;
		
		// 数据长度
		//nDataLen = atoi( (char*)pch );
		//OSTimeDlyHMSM(0,0,0,nDataLen*10);
		//CMM_ReadCmd((unsigned char *)pDest, nDataLen);
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
//+CLIP:"13480924411",129,,,,0  
void CMM_ReadCallerCLIP( unsigned char *pMsg, unsigned char *pCaller ){
	
	unsigned char	*pch = NULL;
		
	pch = (unsigned char*)strchr( (char*)pMsg, '"');
	if( pch == NULL ){
		return ;
	}
	pMsg = pch + 1;
	pch = (unsigned char*)strchr((char*)pMsg, '"');
	if( pch == NULL){
		return ;
	}
	*pch = 0x00;
	strcpy( (char*)pCaller, (char*)pMsg );
	
	return;
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

#endif //HW_MC323