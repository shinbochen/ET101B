///////////////////////////////////////////////////////////////////////////
// file name: app_gps.h
// description: GPS协议头文件定义
// author: wyb
#include "..\hdr\main.h"

#ifndef		GPS_H_2008_04_12
#define		GPS_H_2008_04_12

#define 		E_LONGITUDE		0x01
#define			N_LATITUDE		0x02

// 提取GPS数据指令错
#define   		GPS_RESULT_ERROR			0x01
// 返回到主函数
#define   	    GPS_RESULT_ERROR_MASTER		0x03
// 2s = 400*5ms
#define		    GPS_ANSWER_TIMEOUT			400
//========================================================================================
// 接收任务堆栈空间大小(60 * 4字节包括寄存器)
#define  		TASK_GPS_RECEIVE_STK_SIZE   60 		
// 串口数据接收后的处理的任务堆栈空间大小(128 * 4字节包括寄存器)
#define  		TASK_GPS_APP_STK_SIZE  		128 


//========================================================================================
enum{
	GPS_NG = 0,
	GPS_OK
};
//========================================================================================
enum{
	GPS_GPRMC = 0,	
	GPS_GPGGA,
	GPS_CMD_END
};
//========================================================================================
typedef struct _STGPSINFO{
	unsigned char	m_nYear;
	unsigned char 	m_nMonth;
	unsigned char 	m_nDay;
	unsigned char 	m_nHour;
	unsigned char 	m_nMinute;
	unsigned char 	m_nSecond; 
	
	unsigned char	m_nGPSSignal;
	
	unsigned char   m_bPosValid;
	// E_LONGITUDE/W_LONGITUDE/N_LATITUDE/S_LATITUDE
	unsigned char		m_nLLFlag;
	
	// 经：度、分，小数位
	unsigned char	  m_nLONGDegree;		
	unsigned char 	m_nLONGMinute;
	unsigned short	m_nLONGPrecision;	
	
	// 纬：度、分，小数位
	unsigned char	  m_nLATIDegree;		
	unsigned char 	m_nLATIMinute;
	unsigned short	m_nLATIPrecision;	
	//速度52.3  小数位扩大一位  523
	unsigned short  m_nCurSpeed;		
	unsigned short	m_nCurDirection;	
}STGPSINFO,*PSTGPSINFO;
		
//  GPS数据结构
extern STGPSINFO  g_stGPSInfo;

//========================================================================================
//建立GPS任务
void GPS_Task_Creat( void );
// 接收数据
void GPS_TaskGPSDataReceive(void *pData);
// 串口数据接收后的处理任务
void GPS_TaskGPSApp(void *pData);
//获取GPS数据类型 
unsigned char GPS_GetCmdType(unsigned char *pMsg);
// 提取GPS数据
void GPS_ReadCmd( unsigned char *pBuf );
// 读取GPS信号
void  GPS_ParseGPGGACmd( unsigned char	*pMsg );
//解释GPS　$GPRMC数据格式
void  GPS_ParseGPRMCCmd(unsigned char *pData);
//  经度
int GPS_GetLatitude( void );
//  纬度
int GPS_GetLongitude( void );
//	经度-4*4+纬度-16*19
int	 GPS_CalculateData( void );
unsigned char	GPS_GetGPRMCData( unsigned char	*pDest );
unsigned char   GPS_CountVerifySum( unsigned char	*pSrc, unsigned char	nLen );
//	公里速度
unsigned short	GPS_GetMilimetre( void );
//========================================================================================
#define		GPS_IsPosValid( )			g_stGPSInfo.m_bPosValid 
//  海里速度
#define		GSP_GetSeaMile( )			g_stGPSInfo.m_nCurSpeed

#endif
