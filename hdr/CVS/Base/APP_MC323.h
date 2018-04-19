/////////////////////////////////////////////////////////////
//file name: MC323.c
//     Date: 2011-06-08  
//	 Author: wyb	 
// optimize: 
// op_date : 
/////////////////////////////////////////////////////////////
#include "..\hdr\main.h"

#ifndef  	  APP_CDMA_H_20110608
#define  	  APP_CDMA_H_20110608

#ifdef HW_MC323

extern  const	char	*s_strCMMCMD[];

enum{
	 CDMA_OK,	 
	 CDMA_ATE0,			//关闭回显示
	 CDMA_ATE1,			//开启回显
	 CDMA_IFC,      //流量控制设置命令
	 CDMA_CPIN,			//检测卡
	 CDMA_CMGF,			//
	 CDMA_GSN,			//
	 
	 //网络服务接口命令
	 CDMA_CREG,		  //网络注册
	 CDMA_CSQ,			//RSSI查询
	 CDMA_RSSIREP,	//上报指示设置 
	 
	 //呼叫控制命令	 
	 CDMA_CDV,			//语音呼叫
	 CDMA_CHV,			//挂断语音呼叫
	 CDMA_A,				//接听命令 
	 CDMA_RING,			//来电指示
	 CDMA_ORIG,			//呼叫发起指示
	 CDMA_CONN,		  //呼叫接通指示
	 CDMA_CEND,		  //通话结束指示
	 CDMA_CLIP,			//来电显示功能设置及指示
	 CDMA_CLVL,			//耳机音量设置
	 CDMA_CMIC, 		//麦克风音量调节	 
	 CDMA_CLCC,			//呼叫状态查询
	 CDMA_DTMF,		  //二次拨号命令
	 
	 //短消息业务接口命令
	 CDMA_CPMS,			//设置短信存储位置
	 CDMA_CNMI,		  //知信通知方式
	 CDMA_CMTI,			//短信到达提示
	 CDMA_HSMSSS,		//设置短信参数
	 CDMA_HCMGS,		//发送短信
	 CDMA_HCMGSS,	  //短信发送成功上报指示
	 CDMA_HCMGSF,	  //短信发送失败上报指示
	 CDMA_HCMGR,		//读取一条短信
	 CDMA_CMGD,			//删除短信
	 
	 //Internet服务接口命令
	 CDMA_PPPCFG,		//设置PPP用户名和密码
	 CDMA_IPINIT,		//TCP/UDP连接初始化
	 CDMA_IPOPEN,		//建立TCP/UDP链接
	 CDMA_IPSEND,   	//TCP/UDP数据发送
	 CDMA_IPSENDEX,		//扩展TCP/UDP数据发送
	 CDMA_IPDATMODE,	//TCP/UDP数据到达指示设置
	 CDMA_IPDATA,	    //TCP/UDP数据到达指示
	 CMDA_IPCLOSE,		//关闭TCP/UDP链接
	 
	 //
	 CDMA_CME,			// 错误提示
	 CDMA_CMS,			//
	 CDMA_ERROR,		// 错误	 
	  
	 CDMA_TECNT,		//	连接请求	 
	 CDMA_TEDISCNT,	//	挂断请求	 
	 CDMA_TESETUP,	//l 读取/设置 设备参数 
	 
	 CMM_CMD_END, 
	 CMM_CMD_UNKNOW
};

// 短信编码方式
enum{
	CODE_ASCII = 0x00,
	CODE_OCTET,
	CODE_UNICODE
};

//////////////////////////////////////////////////////////////////
// 
void 					CMM_HWInit( void );
void 					CMM_ReStart( void );
void 					CMM_Init(void);

unsigned char		CMM_IPInit( unsigned char *pStrGateway, unsigned char *pUser, unsigned char *pPsd );
/////////////////////////////////////////////////////////////////
// 解释AT指令格式
// 返回命令类型, 并取出命令参数
unsigned char CMM_GetCmdType(unsigned char *pMsg, unsigned short nDataLen);
/////////////////////////////////////////////////////////////////
// 解释AT指令格式
// 返回命令类型, 并取出命令参数
// 扩展指令,处理发短信,返回的状态码 HCMGS, HCMGSS,HCMGSF
unsigned char CMM_GetCmdTypeEx(unsigned char *pMsg, unsigned short nDataLen );

// 总返回1
unsigned char CMM_QueryIPNetworkStatus( void );
/////////////////////////////////////////////////////////////////
// 检测卡是否就绪
unsigned char CMM_CheckSIMReady( void );
// 查终端序号 
unsigned char CMM_QueryModuleSN( unsigned char *pStr );
// 删除短信
unsigned char CMM_DeleteSMS( unsigned short nCnt );
// 删除短信
void          CMM_DeleteAllSMS( void );
// 打电话
unsigned char CMM_DialNumber( unsigned char *pStrNo );
//////////////////////////////////////////////////////////////
// 接电话
unsigned char CMM_AcceptCall( void );
//	查询地址小区ID  +CREG: 2,1,"2483","0E9D"
unsigned char	CMM_QueryNetworkLac( int	*pLac, int	*pID );
//////////////////////////////////////////////////////////////
//
unsigned char CMM_RejectCall( void );
// 返回信号级别
unsigned char CMM_QuerySignal( void );
// 设置PPP用户名和密码
unsigned char	CMM_SetPPPUserName( unsigned char	*pUser, unsigned char  *pPsd );
//  DTU拨号连接上报
unsigned char	CMM_DTUPPPOpen( void );
//  DTU关闭连接上报
unsigned char CMM_DTUPPPClose( void );
/////////////////////////////////////////////////////////////////
unsigned char CMM_AT( void );
////////////////////////////////////////////////////////////////////////////
//   TCP/IP连接
// 返回GSM_SUCCEED表示成功，否则失败
// $HTCPOPEN: 1, 0
unsigned char CMM_IPConnect(  unsigned char nMode, 
															unsigned char *pStrIP,
															unsigned short nPort );
void		  CMM_IPClose( void  );			
unsigned char	CMM_IPClose2( unsigned char	nMode );		
// ASCII bit
unsigned char CMM_SendASCIIData( unsigned char *pCaller, 
																 unsigned char *pMsg, 
																 unsigned char nLen );
// OCTET
unsigned char CMM_SendOctetData( unsigned char *pCaller, 
																 unsigned char *pMsg, 
																 unsigned char nLen );	
// unicode
unsigned char CMM_SendUnicodeData( unsigned char *pCaller, 
																 	 unsigned char *pMsg, 
																 	 unsigned char nLen );
// 发送 GSM短信																 	 
unsigned char CMM_SendSMS( unsigned char *pCaller,
							unsigned char *pMsg,  
							unsigned char nDataLen,
							unsigned char nCodeType );	
// 提取短信内容
// send: at+CMGR=x
// recv1:
// pCodeType: 0: ASCII
//					  1: OCT
//            2: UNICODE
unsigned char CMM_ReadSMS( unsigned char nCnt, 
													 unsigned char *pCaller, 
													 unsigned char *pMsg,
													 unsigned char *pCodeType );												   
//  解释短信格式
//^HCMGR:13480924411,2011,06,08,16,24,57,0,1,9,0,0,0,1
unsigned char CMM_ParseSMSFormat( unsigned char *pCaller, 
																	  unsigned char *pMsg,  
																	  unsigned char *pCodeType,
																	  unsigned char *pDataLen );	
																	
// 解析并存储最新信息
void 					CMM_ParseMasterCmd(unsigned char *pMsg, unsigned char nCmdType);				
// 发送GPRS数据 0: 失败  1:  成功
// send:
// AT+IPSEND=通道,长度
// 
unsigned char IP_SendData( 	unsigned char nMode,
							unsigned char *pBuf, 
							unsigned char nLen);				

//  接收到GPRS数据
// $HTCPRECV: 3, 4
//
// 0 5H    // 0x30 0x20 0x35 'H'
unsigned char	CMM_IP_RecvData(unsigned char  *pMsg,
																unsigned char   *pDest,
																unsigned char	  *pSocketNum );
													
// 解释来电显示
// CID: 13480924411
void 		 			CMM_ReadCallerID( unsigned char *pMsg, unsigned char *pCaller );
// 解释来电显示
//+CLIP:13480924411,0
void 		 			CMM_ReadCallerCLIP( unsigned char *pMsg, unsigned char *pCaller );
/////////////////////////////////////////////////////////////////
//  查询话费
unsigned char		GSM_ExecCUSD( unsigned char	*pDest, unsigned char	*pSrc, unsigned short	nTimeout );
//////////////////////////////////////////////////////////////
//  查询IMEI序号
unsigned char		CMM_QueryIMEI( unsigned  char	*pDest );
//	发送连接成功数据
void					  CMM_SendTECNT( void );
//	发送断开连接,开启GSM模块电源
void 					  CMM_SendDISCNT( void );

//   TCP/IP连接方式
unsigned char		CMM_TCPConnect( unsigned char nCmdType, unsigned char	*pCmdData, unsigned char	*pReturnMsg );
/////////////////////////////////////////////////////////////////
// 解释域名服务器															
unsigned char		GSM_ParseDNS( unsigned char *pStrHost, unsigned char *pStrIP );
//	PDP上下文激活
void					  CMM_ActivationPDP( void );
unsigned char 	CMM_QueryNetworkStatus( void );
unsigned char		CMM_ConfigCBand( void );
unsigned char 	GSM_ReadCaller( unsigned char *pCaller );
#endif 			// HW_MC323
													   							    											 
#endif //APP_CDMA_H_20110608

