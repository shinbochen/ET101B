///////////////////////////////////////////////////////////////////////////
// file name	: APP_MG323.h
// description: GPRS/GSM协议头文件定义(HW-MG323)
// author		  : wyb
// date			  : 2011-11-16
// reamark		: GSM AT驱动
// optimize		: wyb


#include "..\hdr\main.h"

#ifndef  APP_MG323_H_20111116
#define  APP_MG323_H_20111116

#if( HW_MG323 )

#define   MIN_GPRS_SERVER_ABORT				1
#define   MAX_GPRS_SERVER_ABORT				2000



extern  const	char	*s_strCMMCMD[];
enum{
	// 普通指令设置	
	GSM_OK = 0,				//  设置OK	
	GSM_ATE0,				//	开启回显示	
	GSM_ATE1,				//  关闭回显
	GSM_CGSN,				//  查询IMEI
	GSM_CPIN,				//	卡检测		
	GSM_CFUN,				//  设置GSM的全部功能
	GSM_CSQ,				//  查GSM信号		
	GSM_CREG,				//	查询GSM网络状态	
	GSM_CGREG,				//	查询GPRS网络状态 	
	GSM_Q0,
	
	// 语音控制指令	
	GSM_CLVL,				// 	设置扬器音量	
	GSM_CMIC, 				//  设置音频输出	
	GSM_ECHO,				//  回声抑制功能	
	GSM_SWSPATH,			//	音频通道选择
	
	// 通话指令	
	GSM_ATD,				//	打电话
	GSM_ATA,				//	接电话	
	GSM_ATH,				//	挂电话
	GSM_CHUP,			  	//  挂电话
	GSM_CLIP,			 	//  来电显示设置
	GSM_VTS,				//  播放数字1的DTMF音
	GSM_CLCC,				//  读取号码	
	GSM_RING,				//	打电话	
	GSM_NO_CARRIER, 		//  通话结束	
	GSM_BUSY,				//  挂电话	
	GSM_NO_ANSWER,			//	无人接听
	GSM_CEND,				//  结束通话
	
	// 短信指令		
	GSM_CSCA,				//  短信中心号码
	GSM_CMGF,				//	短信格式
	GSM_CNMI,				//	短信提示
	GSM_CMTI,				//  收到短信提示	
	GSM_CMGS,				//  发送短信内容
	GSM_CMGD,				//	删除短信
	GSM_CMGR,				//	读短信
	
	//网络设置
	GSM_CGATT,				//  查询或连接GPRS 状态
	GSM_CGACT,				//  激活/去激活 PDP
	GSM_IOMODE,				//  设置数据模式
	GSM_SICS,				//  设置上网参数
	GSM_SISO,				//  打开服务器连接
	GSM_SICI,				//  查询分配IP地址
	GSM_SISW,				//  发送GPRS数据
	GSM_SISR,				//  读取GPRS数据
	GSM_SISC,				//  关闭网络连接
	GSM_SISS,				//  通道选择
	GSM_SIS,				//  自动上报internt 状态
		
	GSM_ERROR,	
	GSM_CMS_ERROR,		   //
	
	// 参数配置
	GSM_TECNT,			//	连接请求	
	GSM_TEDISCNT,		//	挂断请求	
	GSM_TESETUP,		//l	读取/设置 设备参数
	
	CMM_CMD_END
};
	

void 		  CMM_HWInit( void );
void 		  CMM_ReStart( void );
unsigned char CMM_GetCmdType(unsigned char *pMsg);

void		  CMM_Init(void);
unsigned char CMM_AT( void );
unsigned char CMM_QueryModuleSN( unsigned char *pStr );
unsigned char CMM_CheckSIMReady( void );
unsigned char CMM_ConfigCBand( void );
unsigned char CMM_QuerySignal( void );
unsigned char CMM_QueryNetworkStatus( void );
unsigned char CMM_QueryNetworkLac( int	*pLac, int	*pID );

unsigned char CMM_DialNumber( unsigned char *pStrNo );
unsigned char CMM_AcceptCall( void );
unsigned char CMM_RejectCall( void );
unsigned char GSM_ReadCaller( unsigned char *pCaller );
unsigned char CMM_QueryIMEI( unsigned  char	*pDest );

unsigned char GSM_SendPDUSMS(unsigned char *pMsg,  unsigned char nDataLen, unsigned char nTitleLen);
unsigned char GSM_ReadPDUSMS(unsigned char nCnt, unsigned char *pBuf );
unsigned char CMM_DeleteAllSMS(void);
unsigned char CMM_DeleteSMS( unsigned short nCnt );

unsigned char IP_ParseCloseState( unsigned char  *pMsg );
void 		  CMM_ParseMasterCmd(unsigned char *pMsg, unsigned char nCmdType);

unsigned char GSM_QuerySMC( unsigned char *pStr );
unsigned char GSM_ExecCUSD( unsigned char	*pDest ,unsigned char	*pSrc, unsigned short	nTimeout );

unsigned char CMM_QueryIPNetworkStatus(void);
unsigned char CMM_IPInit( unsigned char *pStrGateway, unsigned char *pUser, unsigned char *pPsd );
void		  CMM_ActivationPDP( void );
unsigned char GPRS_SetGateWay( unsigned char *pStr );
unsigned char GPRS_SetUserPsd( unsigned char *pUser, unsigned char *pPsd );
unsigned char GSM_ParseDNS( unsigned char *pStrHost, unsigned char *pStrIP );
unsigned char CMM_IPConnect(  unsigned char nMode, unsigned char *pStrIP,unsigned short nPort );
unsigned char IP_SendData(unsigned char nMode, unsigned char *pBuf, unsigned char nLen);
unsigned char CMM_IPClose( void );

void		  CMM_SendTECNT( void );
void 		  GSM_SendDISCNT( void );

#endif

#endif //APP_MG323_H_20111116