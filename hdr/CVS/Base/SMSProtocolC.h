///////////////////////////////////////////////////////////////////////////
//   file name: SMSProtocolData1.h
// description: SMSЭ��ͷ�ļ�����\old default function
//      author:
//        date: 2009-02-24
//     reamark: 

#include "..\hdr\main.h"

#ifndef  SMSPROTOCOLDATAC_H
#define  SMSPROTOCOLDATAC_H

#ifdef		SW_SMSPROTOCOLC
enum{
	SMS_RESTART = 0x00,					// ������λ
	SMS_DEFAULT,					   		// �ָ���������
	SMS_SERIAL_SN,					   	// ���	
	SMS_QUERY_INF,					   	// ��λ״̬	
	SMS_VERSION,					   		// ��ȡ�汾��
	SMS_POSITION,					   		// ��λ
	
	SMS_ALARM_ENABLE,				   	// ��������
	SMS_ALARM_DISABLE,				 	// �رձ���
	
	SMS_PER_SPACE,					   	// ���ټ��
	SMS_SETUP_PARKING,				  // ͣ������
	SMS_DOOR_TIME,				      // ���ű���ʱ��
	SMS_OVERSPEED,					   	// ���ٱ���
	SMS_CLOSE_OIL,					   	// ����
	SMS_RESUME_OIL,					   	// �ָ���·
	SMS_CONTROL_LISTEN,				  // ����
	SMS_SETUP_SP1,					   	// ���ú���
	SMS_SETUP_SP2,					   	// ���ú���
	SMS_SETUP_SP3,					   	// ���ú���	
	SMS_SETUP_HOME,			   			// �������غ���
	SMS_SETUP_CSCA,					   	// �������ĺ���
	SMS_SETUP_PSD,					   	// ��������
	
	SMS_OPEN_TRACK,					   	// ��������
	SMS_CLOSE_TRACK,				   	// �رո���
	
	SMS_NET_SETUP,					   	// �����û���������
	SMS_SETUP_APN,					   	// ����APN���� 
	SMS_SETUP_DNSR,					   	// ����������
	
	SMS_TOW_ENABLE,					   	// ������������ 
	SMS_TOW_DISABLE,				   	// �رշ���
	
	SMS_AUTO_TIME,				     	// �Զ���������
	SMS_GPRS,					   		    // ������ʽΪGPRSʱ TCP/UDP����
	SMS_SETUP_IP,					   	  // ������IP��ַ����
	SMS_SETUP_GP,					   	  // ����ģʽ
	SMS_FORWARD,					   	  // ת������ 	
	SMS_EXCUTE,						   	  // ���Ѳ�ѯ����
	SMS_IMEI,							      // ��ѯIMEI
	
	SMS_TEUID,								  // �����ն����
	SMS_NMEA,										// �ϴ�GPRMC��ʽ
	SMS_SMS,										// �ϴ��Լ�����ĸ�ʽ
	SMS_CLOSE,									// �ر����繦��
	SMS_POWEROFF,				//������رյ�Դ
	SMS_LOWPOWER,				//������رյ͵�Դ	
	SMS_SETUP,							//APN
	SMS_SERVER_DNSR,
	SMS_CMD_END
};
//  ����GPS����

/////////////////////////////////////////////////////////////
// ��ȡ�������� 
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
