///////////////////////////////////////////////////////////////////////////
// file name: app_gps.h
// description: GPSЭ��ͷ�ļ�����
// author: wyb
#include "..\hdr\main.h"

#ifndef		GPS_H_2008_04_12
#define		GPS_H_2008_04_12

#define 		E_LONGITUDE		0x01
#define			N_LATITUDE		0x02

// ��ȡGPS����ָ���
#define   		GPS_RESULT_ERROR			0x01
// ���ص�������
#define   	    GPS_RESULT_ERROR_MASTER		0x03
// 2s = 400*5ms
#define		    GPS_ANSWER_TIMEOUT			400
//========================================================================================
// ���������ջ�ռ��С(60 * 4�ֽڰ����Ĵ���)
#define  		TASK_GPS_RECEIVE_STK_SIZE   60 		
// �������ݽ��պ�Ĵ���������ջ�ռ��С(128 * 4�ֽڰ����Ĵ���)
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
	
	// �����ȡ��֣�С��λ
	unsigned char	  m_nLONGDegree;		
	unsigned char 	m_nLONGMinute;
	unsigned short	m_nLONGPrecision;	
	
	// γ���ȡ��֣�С��λ
	unsigned char	  m_nLATIDegree;		
	unsigned char 	m_nLATIMinute;
	unsigned short	m_nLATIPrecision;	
	//�ٶ�52.3  С��λ����һλ  523
	unsigned short  m_nCurSpeed;		
	unsigned short	m_nCurDirection;	
}STGPSINFO,*PSTGPSINFO;
		
//  GPS���ݽṹ
extern STGPSINFO  g_stGPSInfo;

//========================================================================================
//����GPS����
void GPS_Task_Creat( void );
// ��������
void GPS_TaskGPSDataReceive(void *pData);
// �������ݽ��պ�Ĵ�������
void GPS_TaskGPSApp(void *pData);
//��ȡGPS�������� 
unsigned char GPS_GetCmdType(unsigned char *pMsg);
// ��ȡGPS����
void GPS_ReadCmd( unsigned char *pBuf );
// ��ȡGPS�ź�
void  GPS_ParseGPGGACmd( unsigned char	*pMsg );
//����GPS��$GPRMC���ݸ�ʽ
void  GPS_ParseGPRMCCmd(unsigned char *pData);
//  ����
int GPS_GetLatitude( void );
//  γ��
int GPS_GetLongitude( void );
//	����-4*4+γ��-16*19
int	 GPS_CalculateData( void );
unsigned char	GPS_GetGPRMCData( unsigned char	*pDest );
unsigned char   GPS_CountVerifySum( unsigned char	*pSrc, unsigned char	nLen );
//	�����ٶ�
unsigned short	GPS_GetMilimetre( void );
//========================================================================================
#define		GPS_IsPosValid( )			g_stGPSInfo.m_bPosValid 
//  �����ٶ�
#define		GSP_GetSeaMile( )			g_stGPSInfo.m_nCurSpeed

#endif
