///////////////////////////////////////////////////////////////////////////
// file name	: APP_GTM900.h
// description: GPRS/GSM协议头文件定义(HW-GMT900)
// author		: real
// date			: 2009-02-17
// reamark		: GSM AT驱动
// optimize		: real


#include "..\hdr\main.h"

#ifndef  APP_GSM2_H_20090217
#define  APP_GSM2_H_20090217

#ifdef			HW_GTM900
extern  const	char	*s_strCMMCMD[];
enum{
	// 设置OK
	GSM_OK = 0,
	//	开启回显示
	GSM_ATE0,
	//  关闭回显
	GSM_ATE1,
	// 查GSM信号		at+csq?
	GSM_CSQ,
	// 关闭休眠功能
	GSM_SLEEP,
	//  设置波特率
	//GSM_IPR,
	//  设置GSM的全部功能
	GSM_CFUN,
	// 	设置扬器音量
	GSM_NFV,
	//  设置音频输出
	GSM_NFO,
	//  设置音频输入
	GSM_NFI, 
	//  回
	GSM_VLB,
	//	音频通道选择
	GSM_SNFS,
	//	短信格式
	GSM_CMGF,
	//  关闭网络连接
	GSM_IPCLOSE,
	//  显示文本格式
	GSM_CSDH,
	//  设置数据格式
	GSM_IOMODE,
	//	文本编码方式
	GSM_CSMP,
	//	短信提示
	GSM_CNMI,
	//	定义PDP上下文
	GSM_CGDCONT,
	// 初始化网络命令
	GSM_ETCPIP,
	//	查询GPRS网络状态 
	GSM_CGREG,
	//  打开GPRS连接
	//GSM_IPOPENX,
	//  打开单UDP连接
	GSM_IPOPEN,
	//	查询GSM网络状态
	GSM_CREG,
	//	打电话
	GSM_ATD,
	//	接电话
	GSM_ATA,
	//	挂电话
	GSM_ATH,
	//	删除短信
	GSM_CMGD,
	//	读短信
	GSM_CMGR,
	//  读取号码
	GSM_CLCC,
	//	卡检测	
	GSM_CPIN,
	//	发送GPRS数据
	//GSM_IPSENDX,
	//	发送GPRS数据
	GSM_IPSEND,
	//  发送短信内容
	GSM_CMGS,
	//	打电话
	GSM_RING,
	//  收到短信提示
	GSM_CMTI,
	//  通话结束
	GSM_NO_CARRIER,
	//  挂电话
	GSM_BUSY,
	//	无人接听
	GSM_NO_ANSWER,		
	//	收到GPRS数据
	GSM_IPDATA,
	//	收到连接命令
	GSM_CONNECT,	
	//	错误处理
	GSM_ERROR,
	//  主机序列号
	GSM_CGSN,
	//  短信中心号码
	GSM_CSCA,
	//   获取域名服务器
	GSM_DNSR,
	//  关机
	//GSM_MSO,
	//  附加GPRS网络
	GSM_CGATT,
	//  非结构化附加业务
	GMS_CUSD,
	//	PDP上下文激活
	GSM_CGACT,	
	//	连接请求
	GSM_TECNT,
	//	挂断请求
	GSM_TEDISCNT,
	//l	读取/设置 设备参数
	GSM_TESETUP,
	
	CMM_CMD_END
};

///////////////////////////////////////////////////////////
// GSM硬件初始化
void 					CMM_HWInit( void );
//	GSM900通信模块硬件重启	
void 					CMM_ReStart( void );
// power off
void 					CMM_PowerOff( void ); 
// power on
void 					CMM_PowerOn( void );
//初始化GSM模块
void 					CMM_Init(void);
//  模块锁频
unsigned char			CMM_ConfigCBand( void );
//初始化网络连接
unsigned char	CMM_IPInit(unsigned char *pStrGateway, unsigned char *pUser, unsigned char *pPsd);
unsigned char CMM_AT( void );
unsigned char CMM_IPClose( void );
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
//	查询地址小区ID 
unsigned char	CMM_QueryNetworkLac( int	*pLac, int	*pID);
//////////////////////////////////////////////////////////////
//
//unsigned char GSM_OffModule( void );
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

//	PDP上下文激活
void	CMM_ActivationPDP( void );
///////////////////////////////////////////////////////////////////
//
unsigned char GPRS_SetUserPsd( unsigned char *pUser, unsigned char *pPsd );

//////////////////////////////////////////////////////////////
// 连接网络
// 返回GSM_SUCCESS表示成功，否则失败
unsigned char CMM_IPConnect( 	unsigned char nMode, 
															unsigned char *pStrIP,
															unsigned short nPort );
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

#endif // HW_GTM900

#endif

