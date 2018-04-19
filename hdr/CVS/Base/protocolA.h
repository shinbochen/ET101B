///////////////////////////////////////////////////////////////////////////
//   file name: ProtocolData.h
// description: GSM/GPRS协议头文件定义
//      author:
//        date: 2009-02-15
//     reamark: 


#include "..\hdr\main.h"

#ifndef  GPRSPROTOCOLA_H
#define  GPRSPROTOCOLA_H

#ifdef SW_GPRSPROTOCOLA 

///////////////////////////////////////////////////////////////////////////
//	解释命令头
enum{
	TE_ERROR_HEAD = 0x00,
	TE_LOGIN,
	TE_LOGIN_ACK,
	TE_DELIVERY,
	TE_SUBMIT,	
	TE_HANDSHAKE,
	TE_ACK
};

///////////////////////////////////////////////////////////////////////////
// 协议命令头定义
enum{
	//	GSM卡 序号
	CC_GSM_IMI  =  0x01,    
	//  硬件技持情况
	CC_HW_SUPPORT,
	//  软件设置情况
	CC_SW_SETUP ,
	//  GPS定位信息	CC_GPS_TIME 
	CC_GPS_POSITION,
	//	终端报警信息
	CC_HW_ALARMSTATE,
	//  关闭/恢复油路   
	CC_HW_CTRLOIL,
	//	开/关 车门
	CC_HW_CTRLDOOR,
	//  点名呼叫
	CC_GPS_CALLONCE,
	//	定次呼叫按时间
	CC_GPS_TCALLINTERVAL,
	// 	定次呼叫按距离
	CC_GPS_TCALLDISTANCE,
	//  连续呼叫按时间
	CC_GPS_ACALLINTERVAL ,
	//  连续呼叫按距离
	CC_GPS_ACALLDISTANCE,
	// 停止呼叫
	CC_GPS_CALLSTOP,
	//  电话监听
	CC_HW_LISTEN,
	//  设置各种号码
	CC_SETUP_TELNUMBER,
	//  更改IP地址端口号
	CC_SETUP_SERVER,
	//  终端复位
	CC_TE_RESET,
	//  设置超速报警
	CC_SETUP_SPEEDALARM,
	//  设置停车报警
	CC_SETUP_PARKALARM,
	//  取消报警
	CC_SETUP_CANCELALARM,
	//  停止报警
	CC_STOP_ALARM,
	//  维护报文连接
	CC_GPS_MESSAGETIMESPACE,
	//  恢复出厂设置
	CC_GPS_RESTORE_FACTORY
};

unsigned char	 PL_ComposeLogin( unsigned char  *pDest );
unsigned char	 PL_ComposeHSData( unsigned char	*pDest );
unsigned char    PL_ComposeGPSData(unsigned char *pDest);
unsigned char    PL_ParseData( unsigned char *pDest, 
							   unsigned char *pSrc, 
							   unsigned char nLen );
							 
unsigned char	 PL_ParseDelivery( unsigned char *pDest, 
							       unsigned char *pSrc );

#endif //SW_GPRSPROTOCOLA 

#endif //GPRSPROTOCOLA_H
