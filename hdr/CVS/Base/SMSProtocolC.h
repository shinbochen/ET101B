///////////////////////////////////////////////////////////////////////////
//   file name: SMSProtocolData1.h
// description: SMS协议头文件定义\old default function
//      author:
//        date: 2009-02-24
//     reamark: 

#include "..\hdr\main.h"

#ifndef  SMSPROTOCOLDATAC_H
#define  SMSPROTOCOLDATAC_H

#ifdef		SW_SMSPROTOCOLC
enum{
	SMS_RESTART = 0x00,					// 重启复位
	SMS_DEFAULT,					   		// 恢复出厂设置
	SMS_SERIAL_SN,					   	// 序号	
	SMS_QUERY_INF,					   	// 定位状态	
	SMS_VERSION,					   		// 读取版本号
	SMS_POSITION,					   		// 定位
	
	SMS_ALARM_ENABLE,				   	// 开启报警
	SMS_ALARM_DISABLE,				 	// 关闭报警
	
	SMS_PER_SPACE,					   	// 跟踪间隔
	SMS_SETUP_PARKING,				  // 停车报警
	SMS_DOOR_TIME,				      // 车门报警时间
	SMS_OVERSPEED,					   	// 超速报警
	SMS_CLOSE_OIL,					   	// 断油
	SMS_RESUME_OIL,					   	// 恢复油路
	SMS_CONTROL_LISTEN,				  // 监听
	SMS_SETUP_SP1,					   	// 设置号码
	SMS_SETUP_SP2,					   	// 设置号码
	SMS_SETUP_SP3,					   	// 设置号码	
	SMS_SETUP_HOME,			   			// 设置主控号码
	SMS_SETUP_CSCA,					   	// 设置中心号码
	SMS_SETUP_PSD,					   	// 更改密码
	
	SMS_OPEN_TRACK,					   	// 开启跟踪
	SMS_CLOSE_TRACK,				   	// 关闭跟踪
	
	SMS_NET_SETUP,					   	// 设置用户名和密码
	SMS_SETUP_APN,					   	// 设置APN网关 
	SMS_SETUP_DNSR,					   	// 域名服务器
	
	SMS_TOW_ENABLE,					   	// 开启防盗功能 
	SMS_TOW_DISABLE,				   	// 关闭防盗
	
	SMS_AUTO_TIME,				     	// 自动布防功能
	SMS_GPRS,					   		    // 工作方式为GPRS时 TCP/UDP传输
	SMS_SETUP_IP,					   	  // 服务器IP地址设置
	SMS_SETUP_GP,					   	  // 工作模式
	SMS_FORWARD,					   	  // 转发功能 	
	SMS_EXCUTE,						   	  // 话费查询功能
	SMS_IMEI,							      // 查询IMEI
	
	SMS_TEUID,								  // 设置终端序号
	SMS_NMEA,										// 上传GPRMC格式
	SMS_SMS,										// 上传自己定义的格式
	SMS_CLOSE,									// 关闭网络功能
	SMS_POWEROFF,				//开启域关闭电源
	SMS_LOWPOWER,				//开启域关闭低电源	
	SMS_SETUP,							//APN
	SMS_SERVER_DNSR,
	SMS_CMD_END
};
//  保存GPS数据

/////////////////////////////////////////////////////////////
// 获取数据类型 
unsigned char PLS_GetCommandType( unsigned char  *pBuf );
unsigned char PLS_ParseCommandData( unsigned char  *pDest, 
																		unsigned char  *pSrc,
																		unsigned char  *pCaller,
																		unsigned char  *pCodeType  );
unsigned char PLS_CarListen( unsigned char  *pDest,	unsigned char  *pSrc, unsigned char nLangCN );
unsigned char PLS_SetupMasterNo( unsigned char  *pDest, unsigned char  *pSrc, unsigned char nLangCN  );
unsigned char PLS_SetupTEUID( unsigned char  *pDest,	unsigned char  *pSrc, unsigned char nLangCN );
#endif		//SW_SMSPROTOCOLC
							 								 						 					 						 
#endif		//SMSPROTOCOLDATAC_H
