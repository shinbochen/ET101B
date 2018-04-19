///////////////////////////////////////////////////////////////////////////
//   file name: ProtocolData.h
// description: GSM/GPRSЭ��ͷ�ļ�����
//      author:
//        date: 2009-02-15
//     reamark: 


#include "..\hdr\main.h"

#ifndef  GPRSPROTOCOLA_H
#define  GPRSPROTOCOLA_H

#ifdef SW_GPRSPROTOCOLA 

///////////////////////////////////////////////////////////////////////////
//	��������ͷ
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
// Э������ͷ����
enum{
	//	GSM�� ���
	CC_GSM_IMI  =  0x01,    
	//  Ӳ���������
	CC_HW_SUPPORT,
	//  ����������
	CC_SW_SETUP ,
	//  GPS��λ��Ϣ	CC_GPS_TIME 
	CC_GPS_POSITION,
	//	�ն˱�����Ϣ
	CC_HW_ALARMSTATE,
	//  �ر�/�ָ���·   
	CC_HW_CTRLOIL,
	//	��/�� ����
	CC_HW_CTRLDOOR,
	//  ��������
	CC_GPS_CALLONCE,
	//	���κ��а�ʱ��
	CC_GPS_TCALLINTERVAL,
	// 	���κ��а�����
	CC_GPS_TCALLDISTANCE,
	//  �������а�ʱ��
	CC_GPS_ACALLINTERVAL ,
	//  �������а�����
	CC_GPS_ACALLDISTANCE,
	// ֹͣ����
	CC_GPS_CALLSTOP,
	//  �绰����
	CC_HW_LISTEN,
	//  ���ø��ֺ���
	CC_SETUP_TELNUMBER,
	//  ����IP��ַ�˿ں�
	CC_SETUP_SERVER,
	//  �ն˸�λ
	CC_TE_RESET,
	//  ���ó��ٱ���
	CC_SETUP_SPEEDALARM,
	//  ����ͣ������
	CC_SETUP_PARKALARM,
	//  ȡ������
	CC_SETUP_CANCELALARM,
	//  ֹͣ����
	CC_STOP_ALARM,
	//  ά����������
	CC_GPS_MESSAGETIMESPACE,
	//  �ָ���������
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
