/////////////////////////////////////////////////////////////
//file name: APP_CDMA.c
//     Date: 2008-3-02    
//	 Author: wyb	 
// optimize: real
// op_date : 2009-3-8
/////////////////////////////////////////////////////////////
#include "..\hdr\main.h"

#ifndef  	  APP_CDMA_H_20090327
#define  	  APP_CDMA_H_20090327

#if( HW_EM200 )

extern  const	char	*s_strCMMCMD[];
extern  const	char	*s_strCMMCMD2[];

enum{
	 CDMA_OK,	 
	 CDMA_ATE0,			//关闭回显示
	 CDMA_ATE1,			//开启回显
	 CDMA_GSN,		  //ESN查询命令
	 CDMA_CIMI,			//IMSI查询命令
	 
	 CDMA_CDV, 		  // 打电话	 
	 CDMA_CHV, 		  // 桂电话	 
	 CDMA_QCCAV,    // 接电话	 	 
	 CDMA_MICV,     // 调节MIC
	
	 CDMA_CCNT,			// 连接指示 0: 语音呼叫 1:数据呼叫 2:其它类型呼叫		
	 CDMA_CEND,		  // 结束通话
	 CDMA_RING,			// 被叫提示	 
	 CDMA_CPIN,		  // 检测卡
	 CDMA_CSQ,			// 查信号
	 // 短信操作功能 
	 CDMA_SMSSS,		  // 选择短信提示参数 '1,0,0,0'	 
	 CDMA_CMGF,			  // 设置短信格式	1：文本 
	 CDMA_CNMI,		    // 短信通短命令
	 CDMA_CMGR,			// 读取SMS短信
	 CDMA_CPMS,		    // 选择短信存储器"ME"
	 CDMA_CMGSS,		// 短信发送成功标志
	 CDMA_CMGSF,		// 发送短信失败
	 CDMA_CMGS,			// 发送短信
	 CDMA_CMTI,		    // 收到新的短信
	 CDMA_CMGD,		    // 删除短信
	 CDMA_HSMSRD,			// 读取一条短信
	 
	 // 语音指令
	 CDMA_SETEC,		  // 回音消除控制
	 // GPRS网络管理
	 CDMA_ACCNT,		  // 中国电信的CDMA 用户名card,密码card
	 CDMA_PPPOPEN,	  // 建立PPP连接
	 CDMA_PPPCLOSE,		// 关闭PPP连接
	 CDMA_CORG,			  // 发启呼叫提示
	 CDMA_PPPSTATUS,  // 拨号连接状态报告
	 CDMA_DTUIP,			// 获取模块IP地址	
	 // TCP管理
	 CDMA_TCPOPEN,		// DTU 建立TCP连接
	 CDMA_TCPCNNTIMER,// DTU 建立TCP连接时间
	 CDMA_TCPCNNABORT,// DTU 设置建立,TCP连接的最长时间
	 CDMA_TCPSEND,    // DTU 发送TCP数据
	 CDMA_TCPRECV,		// DTU 接收TCP数据
	 CDMA_TCPSTATUS,  // DTU TCP通道连接状态上报 0: TCP连接成功 1: TCP连接关闭
	 CDMA_TCPCLOSE,   // DTU 关闭TCP连接	
	 // UDP管理
	 CDMA_UDPOPEN,		// DTU 建立UDP连接
	 CDMA_UDPSEND,    // DTU 发送UDP数据
	 CDMA_UDPRECV,	  // DTU 接收UDP数据	 
	 CDMA_UDPCLOSE,   // DTU 关闭UDP连接
	 CDMA_UDPSTATUS,  // DTU UDP通道连接状态上报 0: UDP连接成功 1: UDP连接关闭 		 
	 //  其它指令
	 CDMA_CME,		  // 错误提示
	 CDMA_CMS,
	 CDMA_CID,		  // 来电提示号码
	 CDMA_IFC,		  // 关闭硬件流
	 CDMA_CLIP,		  // 来电显示号码
	 CDMA_HCTCM,	  // 兼容开产命令	
	 //	连接请求
	 GSM_TECNT,
	 //	挂断请求
	 GSM_TEDISCNT,
	 //l 读取/设置 设备参数
	 GSM_TESETUP,	 
	 CMM_CMD_END,
	 
	 CMM_CMD_UNKNOW
};

//////////////////////////////////////////////////////////////////
// 
void 					CMM_HWInit( void );
void 					CMM_ReStart( void );
void 					CMM_Init(void);
unsigned char CMM_IPInit( unsigned char *pStrGateway, unsigned char *pUser, unsigned char *pPsd );
/////////////////////////////////////////////////////////////////
// 解释AT指令格式
// 返回命令类型, 并取出命令参数
unsigned char CMM_GetCmdType(unsigned char *pMsg);

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
//  查询DTU拨号连接状态上报
unsigned char	CMM_DTUQueryPPPStatus( void );
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
												   unsigned char nDataLen );	
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
// %CMGR:13760374664,2009,04,06,10,04,1,4,1,0,0
unsigned char CMM_ParseSMSFormatA( unsigned char *pCaller, 
								  unsigned char *pMsg,  
								  unsigned char *pCodeType,
								  unsigned char *pDataLen );													 
//  解释短信格式
//  $HSMSRD: 13480924411, <2009/03/28> <23:09>, 0, 1, 14, 1, 0
//           13480924411, <2009/03/28> <23:09>, 0, 1, 14, 1, 0
unsigned char CMM_ParseSMSFormatB( unsigned char *pCaller, 
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
unsigned char	IP_RecvTCPData(unsigned char  *pMsg,
							unsigned char   *pDest,
							unsigned char	  *pSocketNum );
													
// 解释来电显示
// CID: 13480924411
void 		 CMM_ReadCallerID( unsigned char *pMsg, unsigned char *pCaller );
// 解释来电显示
//+CLIP:13480924411,0
void 		 CMM_ReadCallerCLIP( unsigned char *pMsg, unsigned char *pCaller );
//  $HCEND: 7, 1, 00:02:59 
// Return :
//       0:  语音呼叫END
//       1:  IP数据END
//       2:  其他END
unsigned char CMM_ParseHCEnd( unsigned char  *pMsg );													  	
													   
/////////////////////////////////////////////////////////////////
//  查询话费
unsigned char	GSM_ExecCUSD( unsigned char	*pDest, unsigned char	*pSrc, unsigned short	nTimeout );
//////////////////////////////////////////////////////////////
//  查询IMEI序号
unsigned char	CMM_QueryIMEI( unsigned  char	*pDest );
//	发送连接成功数据
void	CMM_SendTECNT( void );
//	发送断开连接,开启GSM模块电源
void 	GSM_SendDISCNT( void );

//   TCP/IP连接方式
unsigned char	CMM_TCPConnect( unsigned char nCmdType, unsigned char	*pCmdData, unsigned char	*pReturnMsg );
/////////////////////////////////////////////////////////////////
// 解释域名服务器															
unsigned char GSM_ParseDNS( unsigned char *pStrHost, unsigned char *pStrIP );
//	PDP上下文激活
void	CMM_ActivationPDP( void );
unsigned char CMM_QueryNetworkStatus( void );
unsigned char	CMM_ConfigCBand( void );
unsigned char GSM_ReadCaller( unsigned char *pCaller );
#endif 			// HW_EM200
													   							    											 
#endif //APP_CDMA_H_20090327

