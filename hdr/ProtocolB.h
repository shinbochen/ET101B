///////////////////////////////////////////////////////////////////////////
//   file name: ProtocolB.h
// description: GSM/GPRS协议头文件定义
//      author:
//        date: 2009-02-15
//     reamark: 

#include "..\hdr\main.h"

#ifndef  GPRSPROTOCOLB_H
#define  GPRSPROTOCOLB_H

#if( SW_GPRSPROTOCOLB )
//=======================宏定义区================================================================
// 同步码
#ifndef		SYN_HEADERCODE1
#define		SYN_HEADERCODE1				0xE6
#endif
#ifndef		SYN_HEADERCODE2
#define		SYN_HEADERCODE2				0xE6
#endif

//======================================================================
#define		WHERE_MESSAGE_HEAD1						0
#define		WHERE_MESSAGE_HEAD2						1
#define		WHERE_MESSAGE_TYPE						2
#define		WHERE_MESSAGE_TOTAL_LENGHT		3
#define   WHERE_MESSAGE_SEQNUM					4

#define		WHERE_MESSAGE_DEUIDLEN				5
#define		WHERE_MESSAGE_DEUID						6

//======================================================================
#define		WHERE_MESSAGE_DATA_TYPE				14	//  数据类型(1)
#define 	WHERE_MESSAGE_DATA_ALSTATE		15	//  报警状态(4)
#define		WHERE_MESSAGE_DATA_HWSTATE		19	//  硬件状态(4)
#define   WHERE_MESSAGE_ALARM_ENABLE		23  //  报警开关(4)
#define   WHERE_MESSAGE_OVERSPEED				27  //  超速报警(1)
#define		WHERE_MESSAGE_TIREDNESS				28  //  疲劳驾驶(1)
#define 	WHERE_MESSAGE_ANTISTERAL			29  //  非法开门报警设置(1)
#define		WHERE_MESSAGE_PARKING					30  //  停车报警(1)
#define		WHERE_MESSAGE_AREA1						32 	//  电子围栏1(18)
#define		WHERE_MESSAGE_AREA2						50 	//  电子围栏2(18)
#define		WHERE_MESSAGE_SERVERPORT			68 	//  服务器端口号(2)
#define		WHERE_MESSAGE_SERVERIP				70 	//  服务器IP地址(4)
#define		WHERE_MESSAGE_CBMODE					74 	//  回传模式(3)
//#define		WHERE_MESSAGE_LISTENNO			77 69	//  监听号码 1:号码长度  10:号码内容
#define		WHERE_MESSAGE_MATERNO					77	//  主控号码 1:号码长度  10:号码内容
#define		WHERE_MESSAGE_SMCNO						88  //  短消息中心号码 1:号码长度  10:号码内容
#define		WHERE_MESSAGE_AUTHNO1 				99	//  授权号码1 1:号码长度  10:号码内容
#define		WHERE_MESSAGE_AUTHNO2 				110	//  授权号码1 1:号码长度  10:号码内容
#define		WHERE_MESSAGE_AUTHNO3 				121	//  授权号码1 1:号码长度  10:号码内容
#define		WHERE_MESSAGE_END							132	//  结束


//=====自动上传模式================================================================================
enum{
	MODE_TIMES_INTERVAL = 0x01,	//定次上传按时间
	MODE_TIMES_DISTANCE,		//按距离上传
	MODE_AUTO_INTERVAL,			//自动上传按时间
	MODE_AUTO_DISTANCE,			//自动上传按距离上传
	MODE_STOP_SEND				//停止上传			 
};
//=====控制号码设置================================================================================
enum{
	PHONE_MASTER = 0x00,					//主控号码
	PHONE_AUTHORIZATION1 ,			//授权号码1
	PHONE_AUTHORIZATION2 ,      //授权号码2 
	PHONE_AUTHORIZATION3 ,      //授权号码3
	PHONE_SMSADDR,							//地址服务号码
	PHONE_SMC										//SMC号码
};
//=====报警参数设置================================================================================
enum{
	SETUP_ALARM_SET = 0x00,			//报警功能设置
	SETUP_ALARM_OVERSPEED,			//超速报警设置
	SETUP_ALARM_PARKING,			//停车报警设置
	SETUP_ALARM_ANTISTEAL,			//非法开门报警
	SETUP_ALARM_TIREDNESS,			//疲劳驾驶设置
	
	SETUP_ALARM_DISTANCE = 0x06,	//距离报警设置
	SETUP_ALARM_CANCEL = 0xFF		//关闭所有报警
};
//=====硬件表头命令定义=============================================================================
enum{
  	MT_LOGIN = 0x02,  
  	MT_LOGIN_ACK,
  	MT_DELIVERY,
  	MT_SUBMIT,
  	MT_HANDSHAKE,
  	MT_ACK
};
//=====硬件命令控制码定义============================================================================
enum{
	CC_HW_ERROR = 0x01,					//0.	错误码	
	CC_HW_SETUP = 0x02,					//2.	硬件支持情况 	        (MT_LOGIN)
	CC_SW_SETUP,						//3.	软件设置情况 	        (MT_LOGIN,MT_SUBMIT)
	CC_DE_STATE,						//4.	终端状态 		        (MT_SUBMIT)
	CC_GPS_STATE,						//5.	GPS定位信息		        (MT_SUBMIT)
	CC_TE_QUERY_SW_SETUP,				//6.	读取软件设置信息        (MT_ DELIVERY)
	CC_TE_CTRLOIL,						//7.	关闭/恢愎油路信息       (MT_ DELIVERY)
	CC_TE_CTRLDOOR,						//8.	开/关 车门		        (MT_ DELIVERY)	
	CC_TE_CALLONCE,						//9.	点名呼叫: (立即回传最新信息) 				   (MT_ DELIVERY)
	CC_TE_LISTEN,						//10.	电话监听: (打开车载终端话筒，远程监听车内动静) (MT_ DELIVERY)
	CC_TE_TALK,							//11.	电话通话: (打开车载终端话筒，远程喊话) 		   (MT_ DELIVERY)
	CC_TE_SETUP_UPLOADMODE, 			//12.	设置数据上传模式 	    (MT_ DELIVERY)
	CC_TE_SETUP_NUMBER,					//13.	设置各种号码		    (MT_ DELIVERY)
	CC_TE_SETUP_SERVER,					//14.	设置服务器   		    (MT_ DELIVERY)
	CC_TE_SETUP_ALARM,					//15.	报警设置			    (MT_ DELIVERY)
	CC_TE_RESET,						//16.	终端复位		        (MT_ DELIVERY)
	CC_TE_RESTORE_FACTORY,				//17.	恢愎出厂设置		    (MT_ DELIVERY)	
	CC_TE_FENCE,						//18.   设置电子围栏
	CC_GSM_STATE,						//19	GSM定位信息	    (MT_SUBMIT)
	
	CC_BLIND_GPS_STATE = 32,			//32	增加盲点数据			(MT_SUBMIT)
	CC_TE_QUERY_GG_ADDR_CN,				//33    查询中文地址数据		(MT_SUBMIT)
	CC_TE_QUERY_GG_ADDR_EN,				//34    查询英文地址数据		(MT_SUBMIT)
	CC_TE_RESULT_GG_ADDR				//35	返回地址数
};

//=====设备应答数据============================================================================
//bit7  1/0执行成功/失败, 
//bit0~6: 命令类型
//		0:无
//			1~7: reserved
//		7: 控制油路(old version)
//		8: 远程中控锁(old version)
enum{
	ACK_LOCATION = 9,		//定位
	ACK_LISTEN,				//监听
	ACK_TALK,				//通话
	ACK_SETUP_MODE,			//设置数据上传模式
	ACK_SETUP_PHONE,		//设置号码	
	ACK_SETUP_SERVER,		//设置服务器
	ACK_SETUP_ALARM,		//报警设置
	ACK_SETUP_RESET,		//终端复位
	ACK_RESTORE_FACTORY,	//恢愎出厂	
	ACK_SETUP_FENCE,		//设置围栏
		
	ACK_SETUP_PHOTO = 21,	//终端拍照
	
	ACK_DISABLE_OIL = 120,	//断油命令 (new version)
	ACK_ENABLE_OIL,			//恢愎油路(new version)	
	ACK_CLOSE_DOOR,			//远程关门(new version)
	ACK_OPEN_DOOR 			//远程开门(new version)
};
//==============函数定义区===============================================================

void  				PL_AddHead( unsigned char	*pDest );

void  				PL_AddGPSData( unsigned char *pDest );
void  				PL_AddDEState( unsigned char *pDest );
void  				PL_AddIpAddrN( unsigned char *pDest );
unsigned char 		PL_ComposeGPSData( unsigned char  *pDest );	
unsigned char 	 	PL_ComposeGGGPSData( unsigned char  *pDest, unsigned char	nCmdType );		

unsigned char 		PL_ComposeHSData( unsigned char	*pDest );	
unsigned char		PL_ComposeHWSWSetup( unsigned char nMessageType ,unsigned char  *pDest);					
unsigned char		PL_ParseData(unsigned char *pDest, unsigned char *pSrc, unsigned char nLen );	
unsigned char		PL_ParseDelivery( unsigned char *pDest, unsigned char *pSrc );	
unsigned char		PL_TESetupNumber( unsigned char	*pDataMsg );
unsigned char		PL_TESetupServer( unsigned char	*pDataMsg );
unsigned char		PL_TESetupAlarm( unsigned char	*pDataMsg );
unsigned char		PL_TESetupCBMode( unsigned char	*pDataMsg );

//	设置电子围栏
unsigned char	    PL_TESetupFence( unsigned char	*pDataMsg);
				     	
unsigned char	 	PL_ComposeLogin( unsigned char  *pDest );
//	组织GSM数据信息
unsigned char		PL_ComposeGSMData( unsigned char *pDest );

#endif //SW_GPRSPROTOCOLB

/////////////////////////////////////////////////////////////////////////////
// ------------------------public ------------------------------------------
/////////////////////////////////////////////////////////////////////////////
// 参数：
// 分析数据
// nCaller	:  接收号码
// pMsg	  	:  数据内容
// nTPUDL 	:  数据长度
// nDCSType :  DCS  /DCS_7BIT/DCS_8BIT...

// 1: 返回>0表示有暗文协议数据发回   								x
// 2: 返回0, 但pMsg有文本数据。表示有明文信息发回		x
// 3: 返回0,且pMsg为空。表示不需回答数据						x

// nCodeType = 0: 7位数据。文本明文数据
// nCodeType = 1: 8位编码发出。暗文协议数据
// nCodeType = 2: unicode编码。 明文数据
// 返回为0，不需发数据
unsigned char 	PL_ParseSMSData( unsigned char  *pDataBuf,
							     unsigned char  nDataLen,	
							     unsigned char	*pCaller,    				                 
							     unsigned char  nDCS,
							     unsigned char	nTPUDL,
							     unsigned char 	*pCodeType );

unsigned char	PL_AddTEUID( unsigned char *pDest );

unsigned char   	PL_VerifyCheckSum(unsigned char *lpDataBuf, int nDataLen );

#if( SW_PROTOCOL_ASICO ) //CRC 校验法
unsigned short  PL_CountVerifySum( unsigned char *lpDataBuf, int nDataLen);
#else
unsigned char   PL_CountVerifySum( unsigned char *lpDataBuf, int nDataLen);
#endif //SW_PROTOCOL_ASICO

//	解释参数配置
unsigned  char	PL_ParseConfig( unsigned char *pDest ,unsigned char	*pSrc );
//	 读取参数配置
unsigned char	PL_ReadConfig( unsigned char	*pDest );

#if	(EXTERNAL_FLASH)
unsigned char	PL_ComposeGPSBlindData( unsigned char	*pDest, 
										unsigned char	*pSrc,  
										unsigned char	nPackageSize );
#endif //EXTERNAL_FLASH		
						
#endif  //GPRSPROTOCOLB_H