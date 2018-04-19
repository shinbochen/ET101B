///////////////////////////////////////////////////////////////////////////
// file name	: app_GSM.h
// description: GPRS/GSM协议头文件定义
// author			: wyb
// date				: 2008-04-07
// reamark		: GSM AT驱动
// optimize		: real

#ifndef  APP_GSM_H_20080408
#define  APP_GSM_H_20080408   

#define			TEXT_SMSSEND_END			"\x1A"
#define			TEXT_IP						"IP"
#define			TEXT_TCP					"TCP"
#define			TEXT_UDP					"UDP"
#define			TEXT_AT						"AT"
#define			TEXT_QUESTION				"?"
#define			TEXT_LF_RN					"\r\n"
#define         TEXT_LF						"\r"
#define 		TEXT_CR						"\n"

#define 		GSM_RECMAXBUFFER  										512
#define 		GSM_SETMAXBUFFER   										255
// 接收任务堆栈空间大小(512 * 4字节包括寄存器) 
#define  		TASK_GSMDATA_RECEIVE_STK_SIZE         					40 // 128		
// 串口数据接收后的处理的任务堆栈空间大小(512 * 4字节包括寄存器)
#define  		TASK_GSMAPP_STK_SIZE  				  					400//292		

// 设置GSM AT时间回传设置 5*600 = 3000 ms = 3second
#define 		GSM_ATTIME_CHECK										600
// 准备发送GSM短信时间间隔5*100 = 500 ms
#define 		GSM_SENDSPACE_CHECK										100
//  发送GPRS时间等待标志 5*2400 = 10000ms = 10second
#define			GPRS_GPRSDATA_CHECK										2400
//  定时与模块通信 2*60000 = 120000ms = 120s = 2minte
#define    		GSM_AT_CHECK_TIMEOUT									24000
// 10*5=50ms
#define 		GSM_LOWAIT_TIME											10
// 最小信号值
#define 		GSM_SIGNAL_LOWBOUND										6 
// 检测GSM信号 5*12000 = 60000 = 60秒
#define			GSM_CHECK_SIGNAL_TIMEOUT							    12000
// 检测GSM信号一个20分钟没有信号,重启模块
#define     	GSM_LOW_SINGLE_MAXCNT									20 
// 登陆指令发送间隔	2000*5  = 20000ms = 10second
#define			GSM_LOGIN_SPACE											2000
// GPRS 十秒重连接一次
#define 		GSM_CONNECT_DEGREE										4000
//  启录重连次数
#define			IP_FAIL_CONNECT_MAX										5
// 查询GPRS网络信号, 20分钟没有信号重启模块, 20秒1次, 54次重启
#define			GSM_CGREG_MAXCNT										54
// 定时检测盲点数据上传	(20分钟检测一次)
#define			CHECK_GPSDATA_FLASH										12000*20
// 发送GPS盲点数据,一次最多发送7条GPS数据
#define			GSM_SENDFLASH_MAXDATA									7
// 最少为30秒
#define			MIN_TIME_SEC											30

enum{
	DCS_7BIT = 0x00,
	DCS_8BIT,
	DCS_UNICODE
};
enum{
	CB_BYTIME = 0x03,			// 按时间回传达
	CB_BYDISTANCE,				// 按距离回传
	CB_STOP						// 停止回传
};	
//AT设置返回状态值
enum {
	GSM_NG	= 0,
	GSM_SUCCEED,   
	GSM_RESULT_ERROR_MASTER
};
///////////////////////////////////////////////////////////////////////
#define		COMM_USE_MARK										  0x01
#define		COMM_USE_GPRS										  0x01

#define		COMM_GPRS_MARK									  	  0x02
#define		COMM_GPRS_TCP										  0x02
// 客户协议
#define     SMS_POSITION_FEEDBACK_FORMAT		0x04		// 1/0 nmea/maptrack

#define     GPRS_PROTOCOL_TYPE					0x08		// 0/1 self /jaya

#define		COMM_SMS_WORK_MODE					0x10		// 0/1 明码协议/暗码协议

// 油路状态 
#define		DS_OIL_STATE						0x01    // 0/1 恢复油路/断油
// 中英文转换
#define		DS_LANG_CN							0x02	// 0/1 英文	
	


#define		TELNUMBER_LENGTH					20
#define		PASSWORD_LENGTH						4
#define		SERVERDOMAIN_LENGTH					29
#define		SERVERIP_LENGTH						15

#define   	DEUID_LENGTH						8			
// 握手最大次数, 否则重连
#define		UDP_HANDSHAKE_CNT 				    4		
		
typedef struct _STGSMSETUP{
	unsigned char 	m_nCommMode;  							// SMS/GPRS	
	unsigned char  	m_nCBMode;		   												 
	unsigned short	m_nCBSpace;
	
	// SMS Section
	unsigned char 	m_strPassword[PASSWORD_LENGTH+1]; 	// 操作密码
	unsigned char	m_strSMCNo[TELNUMBER_LENGTH+1];		// 短信中心号码
	unsigned char 	m_strMaterNo[TELNUMBER_LENGTH+1]; 	// 主控号码
	unsigned char 	m_strAuthNo1[TELNUMBER_LENGTH+1]; 	// 授权号码1
	unsigned char 	m_strAuthNo2[TELNUMBER_LENGTH+1]; 	// 授权号码2
	unsigned char 	m_strAuthNo3[TELNUMBER_LENGTH+1]; 	// 授权号码3
	
	unsigned char 	m_strAuthNo4[TELNUMBER_LENGTH+1]; 	// 授权号码4
	unsigned char 	m_strAuthNo5[TELNUMBER_LENGTH+1]; 	// 授权号码5
	
	unsigned char	m_strSMSAddrNo[TELNUMBER_LENGTH+1];	// 地址短消息服务号码
	
	// GPRS section
	unsigned char	m_strServerDomain[SERVERDOMAIN_LENGTH+1];
	unsigned char 	m_strServerIP[SERVERIP_LENGTH+1];
	unsigned short	m_nServerPort;	
	unsigned char 	m_strGateway[SERVERDOMAIN_LENGTH+1];
	unsigned char	m_strUser[TELNUMBER_LENGTH+1];
	unsigned char	m_strPsd[TELNUMBER_LENGTH+1];	
	unsigned char 	m_strUID[TELNUMBER_LENGTH+1];
	// 握手时间，默认30second
	unsigned short	m_nHandShakeTime;
	//
	unsigned char	m_nDeviceState;
	// 印尼专用UID上传的
	unsigned char	m_strTEUID[TELNUMBER_LENGTH+1]; 
	
}STGSMSETUP,*PSTGSMSETUP;


/////////////////////////////////////////////////////////////////////////////////////////////
//
typedef struct _STGSMSTATE {
	unsigned char		m_nFlag;
	unsigned char		m_nGSMSignal;
	unsigned char		m_nHandShakeCnt;					// 握手次数
	unsigned char		m_nConnectFailedCnt;  				// 启录重连次数
	unsigned char		m_nTCPRCntionCnt;					// TCP连接不成功,重启
	unsigned char		m_nQueryGPRSCnt;					// 查询GPRS网络信号
	unsigned char		m_nDialLatLng;						// 打电话两次回短信
	unsigned char 		m_strFWDNo[TELNUMBER_LENGTH+1];		// 保存数据转发号码
	unsigned char		m_strForward[TELNUMBER_LENGTH+1];	// 短信转发号码
	unsigned char		m_strDialNo[TELNUMBER_LENGTH+1];	// 打电话回短信
	unsigned int		m_nGPRSAlarmSMSState;				// GPRS模式下的状态		
}STGSMSTATE,*PGSMSTATE;

/////////////////////////////////////////////////////////////////////////////////////////////
// step is 5ms n*200*5 = n second
#define	GSM_GetMaxHandShakeTime( )						(g_stGSMSetup.m_nHandShakeTime*200)
#define GSM_SetToSMSCommMode( )							(g_stGSMSetup.m_nCommMode &= ~COMM_USE_GPRS)	
#define GSM_SetToGPRSCommMode( )						(g_stGSMSetup.m_nCommMode |= COMM_USE_GPRS)
#define	GSM_IsGPRSCommMode( )							(g_stGSMSetup.m_nCommMode & COMM_USE_GPRS)
#define	IP_IsTCPCommMode( )								(g_stGSMSetup.m_nCommMode & COMM_GPRS_TCP)
#define IP_SetTCPCommMode( )							(g_stGSMSetup.m_nCommMode |= COMM_GPRS_TCP)
#define IP_SetUDPCommMode( )						    (g_stGSMSetup.m_nCommMode &= ~COMM_GPRS_TCP)

#define GSM_SetSignal(nSignal)  				(g_stGSMState.m_nGSMSignal = (nSignal))
#define GSM_GetSignal( )			  			(g_stGSMState.m_nGSMSignal)	

#define	CALL_ONLINE										0x01
#define	SEND_SMS_READY									0x02
#define	GSM_WORK_BUSY									0x04
#define	GSM_NEED_RESET									0x08

#define	GPRS_NET_AVAI									0x10
#define	GPRS_CONNECT_OK									0x20
// 设置参数配置
#define	DS_SETUP_CONFIG									0x40	// 0/1 正常/设置

#define	CMM_SetCallOnLine( )							(g_stGSMState.m_nFlag |= CALL_ONLINE )
#define	CMM_SetCallOffLine( )							(g_stGSMState.m_nFlag &= ~CALL_ONLINE )

#define	CMM_SetSendSMSReady( )							(g_stGSMState.m_nFlag |= SEND_SMS_READY )
#define	CMM_SetSendSMSUnReady( )						(g_stGSMState.m_nFlag &= ~SEND_SMS_READY )
#define CMM_IsSendSMSReady( )  							(g_stGSMState.m_nFlag & SEND_SMS_READY )


#define	GSM_SetNeedReset( )								(g_stGSMState.m_nFlag |= GSM_NEED_RESET )
#define	GSM_SetUnNeedReset( )							(g_stGSMState.m_nFlag &= ~GSM_NEED_RESET )
#define	GSM_IsNeedReset( )								(g_stGSMState.m_nFlag & GSM_NEED_RESET )

#define	IP_SetConnectOK( )							    (g_stGSMState.m_nFlag |= GPRS_CONNECT_OK )
#define	IP_SetConnectNG( )							    (g_stGSMState.m_nFlag &= ~GPRS_CONNECT_OK )
#define	IP_IsConnected( )								(g_stGSMState.m_nFlag & GPRS_CONNECT_OK )

// 设置参数配置  (新加功能)
#define  GSM_IsSetupConfig() 						(g_stGSMState.m_nFlag &   DS_SETUP_CONFIG )
#define  GSM_SetSetupConfig()						(g_stGSMState.m_nFlag |=  DS_SETUP_CONFIG )  
#define  GSM_ClrSetupConfig()						(g_stGSMState.m_nFlag &= ~DS_SETUP_CONFIG ) 

#define UDP_IncHandShakeCnt( )						(g_stGSMState.m_nHandShakeCnt++ )
#define UDP_ClrHandShakeCnt( )						(g_stGSMState.m_nHandShakeCnt = 0 )
#define	UDP_GetHandShakeCnt( )						(g_stGSMState.m_nHandShakeCnt)

#define	IP_GetConnectFailedCnt( )					( g_stGSMState.m_nConnectFailedCnt )
#define IP_IncConnectFailedCnt( )					( g_stGSMState.m_nConnectFailedCnt++ )	
#define IP_ClrConnectFailedCnt( )					( g_stGSMState.m_nConnectFailedCnt = 0 )	

#define TCP_GetRCntionCnt( )						( g_stGSMState.m_nTCPRCntionCnt )
#define	TCP_IncRCntionCnt( )						( g_stGSMState.m_nTCPRCntionCnt++ )
#define	TCP_ClrRCntionCnt( )						( g_stGSMState.m_nTCPRCntionCnt = 0 )

//  SMS输出数据
#define  COM_IsSMSFeedbackUseJave()  ( g_stGSMSetup.m_nCommMode & SMS_POSITION_FEEDBACK_FORMAT  )
#define  COM_SetSMSFeedbackUseJave() ( g_stGSMSetup.m_nCommMode |= SMS_POSITION_FEEDBACK_FORMAT )
#define  COM_ClrSMSFeedbackUseJave() ( g_stGSMSetup.m_nCommMode &= ~SMS_POSITION_FEEDBACK_FORMAT ) 

//  GPRS输出数据
#define	 COM_IsGPRSProtocolType()    ( g_stGSMSetup.m_nCommMode & GPRS_PROTOCOL_TYPE  ) 
#define  COM_SetGPRSProtocolType()   ( g_stGSMSetup.m_nCommMode |= GPRS_PROTOCOL_TYPE )
#define	 COM_ClrGPRSProtocolType()   ( g_stGSMSetup.m_nCommMode &= ~GPRS_PROTOCOL_TYPE ) 

// SMS协议发送方式
#define	 COMM_IsSMSFeedbackUseProtocol()		 ( g_stGSMSetup.m_nCommMode & COMM_SMS_WORK_MODE  ) 
#define	 COMM_SetSMSFeedbackUseProtocol()		 ( g_stGSMSetup.m_nCommMode |= COMM_SMS_WORK_MODE  )
#define	 COMM_ClrSMSFeedbackUseProtocol()		 ( g_stGSMSetup.m_nCommMode &= ~COMM_SMS_WORK_MODE  )

//  重启程序是,油路以前是否断开
#define  GSM_IsOilState()  (g_stGSMSetup.m_nDeviceState &   DS_OIL_STATE )
#define  GSM_SetOilState() (g_stGSMSetup.m_nDeviceState |=  DS_OIL_STATE ) 
#define  GSM_ClrOilState() (g_stGSMSetup.m_nDeviceState &= ~DS_OIL_STATE ) 

//  中英文转换
#define  GSM_IsLangState()  (g_stGSMSetup.m_nDeviceState  &   DS_LANG_CN ) 
#define  GSM_SetLangState() (g_stGSMSetup.m_nDeviceState  |=  DS_LANG_CN ) 
#define  GSM_ClrLangState() (g_stGSMSetup.m_nDeviceState  &= ~DS_LANG_CN ) 

//  查询GPRS网络
#define  GSM_GetCGREGCnt()   ( g_stGSMState.m_nQueryGPRSCnt )
#define  GSM_IncCGREGCnt()	 ( g_stGSMState.m_nQueryGPRSCnt++ )
#define	 GSM_ClrCGREGCnt()	 ( g_stGSMState.m_nQueryGPRSCnt = 0 )


// 在GPRS模式下产生报警,同时报短信
#define  GPRS_ClrAlarmSMSMode()  			 ( g_stGSMState.m_nGPRSAlarmSMSState = 0 )
#define  GPRS_ClrNumberSMSMode( nALState )   ( g_stGSMState.m_nGPRSAlarmSMSState &= (~nALState) )

extern	STGSMSTATE			g_stGSMState;  
extern  STGSMSETUP			g_stGSMSetup; 
extern  unsigned char		*lpLargeBuf1;
extern  unsigned char		*lpLargeBuf2;
extern  unsigned char 		*lpLargeBuf3;
extern  unsigned char 		*lpLargeBuf4;

// 写GSM数据结构
void GSM_WriteSetupData(void);
// 读取GSM数据结
void GSM_ReadSetupData(void);
// 初始化GSM state
void CMM_InitState( void );
//GSM AT指令集  
//GSM AT指令集
unsigned char CMM_SendATCmd(unsigned char nCmdType, 
														unsigned char nLFRN,
														unsigned char *pMsg);
// 执行GSM AT指令
// nCmdType 	: 命令类型
// pPara			: 命令参数
// pReturnMsg	: 不为空表示需要得到返回的数据( 不包括命令字符本身 )
//						: 为空 只要等到OK
unsigned char CMM_ExecATCmdN( unsigned char  nCmdType, 
												  	  unsigned short nPara,
												  	  unsigned short  nTimeout,
														  unsigned char  nOKType,		
														  unsigned char  nNGType,
														  unsigned char *pReturnMsg );														 
// 执行GSM AT指令
// nCmdType 	: 命令类型
// pPara			: 命令参数
// pReturnMsg	: 不为空表示需要得到返回的数据( 不包括命令字符本身 )
//						: 为空 只要等到OK
// CGSN特别处理
unsigned char CMM_ExecATCmd( unsigned char nCmdType, 	
														 unsigned char *pPara,
														 unsigned short  nTimeout,
														 unsigned char nOKType,		
														 unsigned char nNGType,
														 unsigned char *pReturnMsg );


void 					InsertNewMsg( unsigned char nCnt );
// GSM主线程开
void 					GSM_Task_Creat( void );
// 串口接收任务
// GSM
void 					GSM_TaskDataReceive(void * pData);
//串口数据接收后的处理任务
void 					GSM_TaskApp(void *pData);
//读取GSM AT指令集数据
int 					CMM_ReadCmd(unsigned char *pBuf, unsigned short nDataLen);
void 					CMM_SignalTest( void );
unsigned char 			CMM_IPMAINTENANCE( unsigned char	*lpLargeBuf );
// 
void 					CMM_ATTest(void);
//短信编 码方式
unsigned char GSM_GetDCSType( unsigned char nType );
// 输入: "1BE08602"
// 输出: "abcd"
unsigned char	GSM_ParseTPUD( unsigned char *lpDest, unsigned char* lpSrc,  unsigned char nLen, unsigned char nDCSType);
// 输入: 0x68,0x31,0x08,0x00,0x75,0x80,0xF5
// 输出: "8613800057085"
unsigned char GSM_ParseTPOA(  unsigned char *lpDest, unsigned char* lpSrc, unsigned char nLen);
// 输入:0x08,0x91,0x68
//      from"0891683108705505F0040D91683135804115F9000080508241947523041BE08602"
//      
// 输出:	
//       pBuf: 			TP_UD
//       pSMC: 			SMSC
//       pCallerNo: TP_OA
//			 pDCSType:  DCS type
// 返回: 数据长度
unsigned char GSM_ParsePDUSMS( unsigned char	*pDest,
															 unsigned char	*pSMC,
															 unsigned char 	*pCallerNo,
															 unsigned char	*pTPUDL,
															 unsigned char 	*pDCSType,
															 unsigned char	*pSrc  );

//////////////////////////////////////////////////////////////////////////
// 返回TPUD实际的长度.AT+CMGS时用到
// 为简单，发送一律采用7位编码(160个字节)
unsigned char	GSM_ComposeSend7GSMData( unsigned char	*lpDest,
																			 unsigned char	*pResultLen,
																			 unsigned char	*lpSMCNo, 
																			 unsigned char	*lpRecvNo,
																			 unsigned char	*lpMsg  );
																			 
//////////////////////////////////////////////////////////////////////////
// 返回TPUD实际的长度.AT+CMGS时用到
// 最多140
unsigned char	GSM_ComposeSend8GSMData( unsigned char	*lpDest,
																			 unsigned char	*pResultLen,
																			 
																			 unsigned char	*lpSMCNo, 
																			 unsigned char	*lpRecvNo,
																			 unsigned char	*lpBuf, 
																			 unsigned char 	nLen );		
																			 
unsigned char	GSM_ComposeSend16GSMData( unsigned char	*lpDest,
										 unsigned char	*pResultLen,
										 
										 unsigned char	*lpSMCNo, 
										 unsigned char	*lpRecvNo,
										 unsigned char	*lpMsg, 
										 unsigned char 	nLen  );																			 
//////////////////////////////////////////////////////////////////////////
// 返回: TPUD实际的长度.AT+CMGS时用到
// input		: nDCS :TP-DCS
// lpTPUDBuf: TP_UDL + TP_UD
// nLen			: length of lpBuf
unsigned char	GSM_ComposeSendGSMData( unsigned char *lpDest,
																			unsigned char *pResultLen,
																			
																 			unsigned char *lpSMCNo, 
																 			unsigned char *lpRecvNo, 
																 			unsigned char *lpTPUDBuf,
																 			unsigned char nLen, 
																 			unsigned char nTPUDL,
																			unsigned char nDCS  );			
//  发送报警数据
void  			CMM_SMSSendAlarmData( unsigned char *lpLargeBuf );
void 			CMM_ParseNewMsg( unsigned char nCnt );

//////////////////////////////////////////////////////////////////////////
// 输入:"089124"
// 输出:0x08,0x91,0x24 并返回3
unsigned short P_StrToHex( unsigned char* lpDestBuf, unsigned char* lpSrcBuf, unsigned short nLen );
// 输入:  "1358008"
// 输出:	0x31, 0x85, 0x00, 0xF8
unsigned char P_StrToBCD( unsigned char* lpDestBuf, unsigned char* lpSrcBuf, unsigned char nLen );
// 7-bit编码
// pSrc:		源字符串指针
// pDst:		目标编码串指针
// nSrcLength:  源字符串长度
// 返回:		目标编码串长度
unsigned char P_Code8BitTo7bit(unsigned char* pDest, unsigned char* pSrc, unsigned char nSrcLength);
// 7-bit解码
// pSrc:		源编码串指针
// pDest:		目标字符串指针
// nSrcLength:  源编码串长度
// 返回:		目标字符串长度
unsigned char P_Decode7bitTo8Bit(unsigned char* pDest, unsigned char* pSrc, unsigned char nSrcLength);
// GSM码变成ASCII码
unsigned char P_GSMToASCII(  char* lpDestBuf, char *lpSrcBuf, unsigned char nLen );
// 
void 		  P_UnicodeToASCII( unsigned char	*pDest, unsigned char	*pSrc , unsigned char nLen );
// 	管理Flash数据的发送
unsigned char	CMM_FlashManagement( unsigned char *lpLargeBuf );
// 年月日时分秒(6)|定位标志(1)|经度(4)|纬度(4)|方向(2)|速度(1)|硬件状态(4)|报警状态(4)|ACC运行时间(2)|命令码(1)
unsigned char	CMM_SaveGPSToFlash( void );
// 	组织短信发送 (2010-05-12) 组合发短信,节省代码空间
//	    pPhone:	 发送号码
//	lpLargeBuf:  数据内容
//		  nLen:  数据长度
//	 nCodeType:  发送格式
void CMM_SendSMSData( unsigned char	*pPhone,unsigned char	*lpLargeBuf,unsigned char	nLen,unsigned char	nCodeType );
#endif

