/////////////////////////////////////////////////////////////
//file name: APP_GSMBENG.H
//     Date: 2008-3-02    
//	 Author: wyb	 
// optimize: real
// op_date : 2009-3-30
/////////////////////////////////////////////////////////////
#ifndef  	  APP_GSMBENG_H_20090330
#define  	  APP_GSMBENG_H_20090330

#define  	  BQ_GSM

#define			TEXT_SMSSEND_END			"\x1A"
#define			TEXT_IP								"IP"
#define			TEXT_AT								"AT"
#define			TEXT_QUESTION					"?"
#define			TEXT_LF_RN						"\r\n"

enum{
		GSM_OK,
		GSM_ATE0,			//关闭回显示
	  GSM_ATE1,			//开启回显
	  GSM_CSQ,			//查GSM信号		at+csq?
	  GSM_CGSN,			//IMEI 序号
	  GSM_CREG,     //查询GSM网络
	  GSM_CGREG,		//查询GPRS网络
	  GSM_CPIN,			//卡检测	  
	  GSM_CGSN,			//主机序列号
	  
	  //电话指令
	  GSM_CLCC,			//查询来电号码	  
		GSM_ATD,		  //打电话		
		GSM_ATA,			//接电话		
		GSM_ATH,			//挂电话		
		GSM_NO_CARRIER,//通话结束	
		GSM_BUSY,			//挂电话	
		GSM_NO_ANSWER,//无人接听
		GSM_CLIP,			//开启来电显示
		GSM_SG,				//麦克风设置
		GSM_SAMP,			//听筒设置
		
	  //短信指令
	  GSM_CSMP,			//发送的编码方式	  
	  GSM_CSCA,			//读取中心号码
	  GSM_CMGF,			//短信格式
	  GSM_CSDH,			//显示文本格式
	  GSM_CNMI,			//短信提示
	  GSM_CMGR,			//读取短信
	  GSM_CMGS,			//发送短信
	  GSM_CMGD,			//删除短信
	  GSM_CSMP,			//编码格式
	  
	  //网络注册
	  GSM_CGDCONT,	//定义PDP上下文
	  GSM_CGATT,		//注册GPRS网络
	  GSM_CUSD,			//非结构化附加业务
	  GSM_CLCC,		  //查询来电
	  GSM_CPIN,			//查询卡
};

///////////////////////////////////////////////////////////////////
//  GSM硬件初始化
void CMM_HWInit( void );
//	GSM通信模块硬件重启	
void CMM_ReStart( void );
//  初始化GSM模块
void CMM_Init( void );
//	初始化网络连接	
unsigned char CMM_IPInit( unsigned char *pStrGateway, unsigned char *pUser, unsigned char *pPsd );
// 返回命令类型, 并取出命令参数
unsigned char CMM_GetCmdType(unsigned char *pMsg);
// 
unsigned char CMM_AT( void );
// 返回信号级别
unsigned char CMM_QuerySignal( void );
// 查询GSM网络状态
// 0,2,3,4: 未注册
// 1:				注册，本地
// 5:				注册, 外地
// 有效返回1 无效返回0
unsigned char CMM_QueryNetworkStatus(void);
// 打电话
unsigned char CMM_DialNumber( unsigned char *pStrNo );
// 接电话
unsigned char CMM_AcceptCall( void );
//  挂电话
unsigned char CMM_RejectCall( void );
//  查询IMEI序号
unsigned char	CMM_QueryIMEI( unsigned  char	*pDest );
/////////////////////////////////////////////////////////////
// 返回:				0： free
//						1:  on use
// +CLCC : 1,1,4,0,0,"13902288001",161
unsigned char GSM_ReadCaller( unsigned char *pCaller );
//
unsigned char GSM_OffModule( void );
// 删除短信
unsigned char CMM_DeleteSMS( unsigned short nCnt );
// 删除全部短信  
unsigned char CMM_DeleteAllSMS(void);
///////////////////////////////////////////////////////////////
// 有效返回1
// 无效返回0
unsigned char GSM_CheckSIMReady( );
//
unsigned char CMM_QueryModuleSN( unsigned char *pStr );
// 提取短信中心号码
unsigned char GSM_QuerySMC( unsigned char *pStr );
// 解释域名服务器
unsigned char GSM_ParseDNS( unsigned char *pStrHost, unsigned char *pStrIP );
//  查询话费
unsigned char	GSM_ExecCUSD( unsigned char	*pDest ,unsigned char	*pSrc, unsigned short	nTimeout );
////////////////////////////////////////////////////////////////////
// 查询GPRS网络
// 返回网络状态
// 有效返回1
// 无效返回0
unsigned char CMM_QueryIPNetworkStatus(void);
// 解析并存储最新信息
void CMM_ParseMasterCmd(unsigned char *pMsg, unsigned char nCmdType);

#endif