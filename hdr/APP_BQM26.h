/////////////////////////////////////////////////////////////
//file name: APP_GSMBENG.H
//     Date: 2008-3-02    
//	 Author: wyb	 
// optimize: real
// op_date : 2009-3-30
/////////////////////////////////////////////////////////////
#include "..\hdr\main.h"
#ifndef  	  APP_GSMBENG_H_20090330
#define  	  APP_GSMBENG_H_20090330


#if( HW_BQM26 )

extern  const	char	*s_strCMMCMD[];

enum{
	  GSM_OK = 0,
	  GSM_ATE0,			//关闭回显示
	  GSM_ATE1,			//开启回显
	  GSM_CMEE,			//
	  GSM_CFUN,			//set phone functionality
	  GSM_COPS,			//模式选择
	  GSM_PWR,			//关机	 
	  GSM_CSQ,			//查GSM信号		
	  GSM_CREG,         //查询GSM网络
	  GSM_CGREG,		//查询GPRS网络
	  GSM_CPIN,			//卡检测	  
	  GSM_CGSN,			//主机序列号
	  
	  //电话指令
	  GSM_CLCC,			//查询来电号码	  
	  GSM_ATD,		  	//打电话		
	  GSM_ATA,			//接电话		
	  GSM_ATH,			//挂电话		
	  GSM_NO_CARRIER,	//通话结束	
	  GSM_BUSY,			//挂电话	
	  GSM_NO_ANSWER,	//无人接听
	  GSM_CLIP,			//开启来电显示
	  GSM_SG,				//麦克风设置
	  GSM_SAMP,			//听筒设置
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
	  GSM_CGATT,		//注册GPRS网络
	  GSM_NWACT,		//开启或关闭网络
	  GSM_NWSO,		 	//开启通道 
	  GSM_NWCN,		    //开启或关闭连接服务器
	  GSM_NWDF,			//设置接收数据的格式
	  GSM_NWTX,			//发送数据到服务器
	  GSM_NWRX,		    //收到数据
	  GSM_NWDR,		    //从接收BUF读数据
	  GSM_ERROR,
	  GSM_CMS_ERROR,    //错误信息返回
	  GSM_CME_ERROR,	//网络注册错误返回
	  GSM_SLEEP,		//关闭休息状态
	  GSM_CGMR,
	  //连接请求
	  GSM_TECNT,
	  //挂断请求
	  GSM_TEDISCNT,
	  //l读取/设置 设备参数
	  GSM_TESETUP,
	  CMM_CMD_END
};

///////////////////////////////////////////////////////////
//  GSM硬件初始化
void 					CMM_HWInit( void );
//	GSM900通信模块硬件重启	
void 					CMM_ReStart( void );
//
void  				CMM_Reset(void );
//  初始化GSM模块
void 					CMM_Init(void);
//	初始化网络连接	
unsigned char CMM_IPInit( unsigned char *pStrGateway, unsigned char *pUser, unsigned char *pPsd );
//  返回命令类型, 并取出命令参数
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
unsigned char CMM_QueryNetworkStatus( void );
//	查询地址小区ID  +CREG: 2,1,"2483","0E9D"
unsigned char	CMM_QueryNetworkLac( int	*pLac, int	*pID );
// 打电话
unsigned char CMM_DialNumber( unsigned char *pStrNo );
// 接电话
unsigned char CMM_AcceptCall( void );
//  挂电话
unsigned char CMM_RejectCall( void );
//  查询IMEI序号
unsigned char	CMM_QueryIMEI( unsigned  char	*pDest );
/////////////////////////////////////////////////////////////
// 返回:			0： free
//						1:  on use
// +CLIP: "13480924411",129,,,,0
//	    : "13480924411",129,,,,0
unsigned char GSM_ParseCaller( unsigned char *pCaller, 
															 unsigned char *pSrc );
//
unsigned char GSM_OffModule( void );
// 删除短信
unsigned char CMM_DeleteSMS( unsigned short nCnt );
// 删除全部短信  
unsigned char CMM_DeleteAllSMS(void);
///////////////////////////////////////////////////////////////
// 有效返回1
// 无效返回0
unsigned char GSM_CheckSIMReady( void );
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
void 	CMM_ParseMasterCmd(unsigned char *pMsg, unsigned char nCmdType);
/////////////////////////////////////////////////////////////////
//  关闭GPRS网络
unsigned char CMM_IPClose(void);
//////////////////////////////////////////////////////////////
// 连接网络
// 返回GSM_SUCCEED表示成功，否则失败
unsigned char CMM_IPConnect(  unsigned char nMode, 
															unsigned char *pStrIP,
															unsigned short nPort );
//////////////////////////////////////////////////////////////															
//	读取GPRS数据	
unsigned short P_ReadGPRSData( unsigned char *pDest, 
							   							 unsigned short nTialLen );
//////////////////////////////////////////////////////////////////
// 发送GPRS数据 0: 失败  1:  成功
// send:
// AT$NWTX=1,<string_data>
unsigned char IP_SendData(unsigned char nMode, 
													unsigned char *pBuf, 
													unsigned char nLen);							   
													
///////////////////////////////////////////////////
// 提取短信内容
// send: at+CMGR=x
// recv1:
// +CMGR:stat,[alpha],length<CR><LF>
// pdu<CR><LF>
// <CR><LF>
// OK<CR><LF>
// recv2:
// ERROR
// 返回长度。失败返回0
unsigned char	GSM_ReadPDUSMS(unsigned char nCnt, unsigned char *pBuf );													
///////////////////////////////////////////////////////////////
// 发送PDU GSM短信
// send:
// AT+CMGS=12<CR><LF>
// recv:
// >
// send:
// pdu ^Z<CR><LF>
// recv1:
// +CMGS:xx<CR><LF> 
// OK 
// recv2:
// ERROR
unsigned char GSM_SendPDUSMS(unsigned char *pMsg,  unsigned char nDataLen, unsigned char nTitleLen);

//	发送连接成功数据
void	CMM_SendTECNT( void );
//	发送断开连接,开启GSM模块电源
void 	GSM_SendDISCNT( void );
/////////////////////////////////////////////////////////////////
//	PDP上下文激活
void	CMM_ActivationPDP( void );
#endif // HW_BQM26
															 															
#endif