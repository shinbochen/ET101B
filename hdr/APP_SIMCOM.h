/////////////////////////////////////////////////////////////
//file name: APP_SIM300.H
//     Date: 2009-6-11    
//	 Author: wyb	 
// optimize: real
// op_date : 2009-6-11 
//"PGSM_MODE",                      900
//"DCS_MODE",                       1800
//"PCS_MODE",                       1900
//"EGSM_DCS_MODE",                  900/1800
//"GSM850_PCS_MODE",                850/1900
//"GSM850__PCS_MODE")       		850/900/1800/1900
/////////////////////////////////////////////////////////////

#ifndef  APP_SIMCOM_H
#define  APP_SIMCOM_H

#if( HW_SIMCOM )

#define			GSM_GPRSWORK_CHECK				30*200

extern  const	char	*s_strCMMCMD[];

enum{
	  GSM_OK = 0,
	  GSM_ATE0,			//关闭回显示
	  GSM_ATE1,			//开启回显
	  GSM_CMEE,			
	  GSM_CFUN,			//set phone functionality
	  GSM_CSQ,			//查GSM信号		
	  GSM_CREG,     //查询GSM网络
	  GSM_CGREG,		//查询GPRS网络
	  GSM_CPIN,			//卡检测	  
	  GSM_IPR,			//设置波特率
	  GSM_CGSN,			//主机序列号 
	  GSM_CMUT,			//静噪控制
	  GSM_CLVL,			//音量控制
	  
	  GSM_SIDET,	  //侧音灵敏度大小
	  GSM_CMIC,		  //改变麦克风的增益
	  GSM_CHFA,		  //0/1此命令用来切换两个音频通道
	  GSM_IFC,		   // 关闭硬件流	
	  
	  //电话指令
	  GSM_CLCC,			//查询来电号码	
	  GSM_RING,			//来电提示 
	  GSM_ATD,		  //打电话		
	  GSM_ATA,			//接电话		
	  GSM_ATH,			//挂电话		
	  GSM_NO_CARRIER,	//通话结束	
	  GSM_BUSY,			//挂电话	
	  GSM_NO_ANSWER,	//无人接听	 
	  GSM_SHUT_OK,		//断开PDP成功
	  GMS_CUSD,			//非结构化附加业务 
		
	  //短信指令
	  GSM_CSMP,			//发送的编码方式	  
	  GSM_CSCA,			//读取中心号码
	  GSM_CMGF,			//短信格式
	  GSM_CSDH,			//显示文本格式
	  GSM_CNMI,			//设置短信提示	  
	  GSM_CMTI,			//收到短信提示	
	  GSM_CMGR,			//读取短信
	  GSM_CMGS,			//发送短信
	  GSM_CMGD,			//删除短信
	  
	  //网络注册	  
	  GSM_CIPDPDP,		//PDP激活	
	  GSM_CIPSHUT,		//关闭PDP激活
	  GSM_CGATT,		//注册GPRS网络
	  GSM_CGDCONT,		//PDP激活
	  GSM_CGACT,		//激活移动场景
	  GSM_CSTT,			//设置APN，USER NAME, PASD WORD
	  GSM_CIICR,		//激活移动场景
	  GSM_CIFSR,		//获取本地IP
	  GSM_CIPSTART,		//连接服务器
	  GSM_CIPSEND,		//发送UDP或TCP数据
	  GSM_CIPCLOSE,		//断开TCP/UDP连接
	  GSM_CIIPSHUT,		//关闭GPRS PDP网络
	  GSM_CIPHEAD,		//设置GPRS数据
	  GSM_IPD,		  	//接收IP数据
	  GSM_SLEEP,		//关闭休息状态
	  GSM_CONNECT_OK,
	  GSM_CONNECT_FAIL,	  
	  GSM_SEND_OK,			
	  GSM_SEND_FAIL,	  
	  GSM_CLOSE_OK,
	  GSM_CLOSED,
	  GSM_ERROR,
	  GSM_CME_ERROR,
	  GSM_CMS_ERROR,
	  // 网络连接状态
	  GSM_STATE,
	  GSM_CBAND,	    //锁频	cband
	  
	//  GSM_CDNSCFG,
	//  GSM_CDNSORIP,
	  //连接请求
	  GSM_TECNT,
	  //挂断请求
	  GSM_TEDISCNT,
	  //l读取/设置 设备参数
	  GSM_TESETUP,
	  //查版本号
	  GSM_GMR,
	  CMM_CMD_END
};

///////////////////////////////////////////////////////////
// GSM硬件初始化
void 					CMM_HWInit( void );
//	GSM900通信模块硬件重启	
void 					CMM_ReStart( void );
//初始化GSM模块
void 					CMM_Init(void);
//  模块锁频
unsigned char			CMM_ConfigCBand( void );
//初始化网络连接
unsigned char	CMM_IPInit(unsigned char *pStrGateway, unsigned char *pUser, unsigned char *pPsd);
void	CMM_ActivationPDP( void );
unsigned char CMM_AT( void );
unsigned char CMM_IPClose( void );
unsigned char CMM_CIPSHUT( void );
// 解释AT指令格式
// 返回命令类型, 并取出命令参数
unsigned char CMM_GetCmdType(unsigned char *pMsg);
///////////////////////////////////////////////////////////
// 返回信号级别
unsigned char CMM_QuerySignal(void);

///////////////////////////////////////////////////////////	
// 查询GSM网络状态
// 0,2,3,4: 未注册
// 1:				注册，本地
// 5:				注册, 外地
// 有效返回1 无效返回0
unsigned char CMM_QueryNetworkStatus( void ); 
//	查询地址小区ID  +CREG: 2,1,"2483","0E9D"
unsigned char	CMM_QueryNetworkLac( int	*pLac, int	*pID );
///////////////////////////////////////////////////////////////
// 打电话
unsigned char CMM_DialNumber( unsigned char *pStrNo );
// 接电话
unsigned char CMM_AcceptCall( void );
//
unsigned char CMM_RejectCall( void );
// 返回:				0： free
//							1:  on use
unsigned char GSM_ReadCaller( unsigned char *pCaller );
/////////////////////////////////////////////////////////////
// 返回:				序号
unsigned char	CMM_QueryIMEI( unsigned  char	*pDest );
//////////////////////////////////////////////////////////////
//
unsigned char GSM_OffModule( void );
//////////////////////////////////////////////////////////////
// 删除短信
unsigned char CMM_DeleteSMS( unsigned short nCnt );
//////////////////////////////////////////////////////////////
// 　删除全部短信
unsigned char CMM_DeleteAllSMS(void);
///////////////////////////////////////////////////////////////
//  发送PDU GSM短信
unsigned char GSM_SendPDUSMS(unsigned char *pMsg,  unsigned char nDataLen, unsigned char nTitleLen );
unsigned char GSM_ReadPDUSMS(unsigned char nCnt, unsigned char *pBuf );
//////////////////////////////////////////////////////////////
// 有效返回1
// 无效返回0
unsigned char GSM_CheckSIMReady( void );
//////////////////////////////////////////////////////////////
//
unsigned char CMM_QueryModuleSN( unsigned char *pStr );
/////////////////////////////////////////////////////////////
// 提取短信中心号码
unsigned char GSM_QuerySMC( unsigned char *pStr );
/////////////////////////////////////////////////////////////////
// 解释域名服务器
unsigned char GSM_ParseDNS( unsigned char *pStrHost, unsigned char *pStrIP );
/////////////////////////////////////////////////////////////////
// 
/////////////////////////////////////////////////////////////////
//  查询话费
unsigned char	GSM_ExecCUSD( unsigned char	*pDest, unsigned char	*pSrc, unsigned short	nTimeout );
////////////////////////////////////////////////////////////////////
// 查询GPRS网络
// 返回网络状态
// 有效返回1
// 无效返回0
unsigned char CMM_QueryIPNetworkStatus(void);
///////////////////////////////////////////////////////////////////
//
unsigned char GPRS_SetGateWay( unsigned char *pStr );
///////////////////////////////////////////////////////////////////
//
unsigned char GPRS_SetUserPsd( unsigned char *pGateWay, unsigned char *pUser, unsigned char *pPsd );

//////////////////////////////////////////////////////////////
// 连接网络
// 返回GSM_SUCCESS表示成功，否则失败
unsigned char CMM_IPConnect( 	unsigned char nMode, 
															unsigned char *pStrIP,															
															unsigned short nPort );
/////////////////////////////////////////////////////////////////
// 解释域名服务器															
//unsigned char GSM_ParseDNS( unsigned char *pStrHost, unsigned char *pStrIP );															
// 解析并存储最新信息
void 					CMM_ParseMasterCmd(unsigned char *pMsg, unsigned char nCmdType);	
//发送GPRS数据 0: 失败  1:  成功   2: 其它状态
unsigned char IP_SendData(unsigned char nMode, 
													unsigned char *pBuf, 
													unsigned char nLen);
//	发送连接成功数据
void	CMM_SendTECNT( void );
//	发送断开连接,开启GSM模块电源
void 	GSM_SendDISCNT( void );
													
#endif //HW_SIMCOM


#endif //APP_SIMCOM_H
