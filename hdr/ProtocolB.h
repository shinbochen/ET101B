///////////////////////////////////////////////////////////////////////////
//   file name: ProtocolB.h
// description: GSM/GPRSЭ��ͷ�ļ�����
//      author:
//        date: 2009-02-15
//     reamark: 

#include "..\hdr\main.h"

#ifndef  GPRSPROTOCOLB_H
#define  GPRSPROTOCOLB_H

#if( SW_GPRSPROTOCOLB )
//=======================�궨����================================================================
// ͬ����
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
#define		WHERE_MESSAGE_DATA_TYPE				14	//  ��������(1)
#define 	WHERE_MESSAGE_DATA_ALSTATE		15	//  ����״̬(4)
#define		WHERE_MESSAGE_DATA_HWSTATE		19	//  Ӳ��״̬(4)
#define   WHERE_MESSAGE_ALARM_ENABLE		23  //  ��������(4)
#define   WHERE_MESSAGE_OVERSPEED				27  //  ���ٱ���(1)
#define		WHERE_MESSAGE_TIREDNESS				28  //  ƣ�ͼ�ʻ(1)
#define 	WHERE_MESSAGE_ANTISTERAL			29  //  �Ƿ����ű�������(1)
#define		WHERE_MESSAGE_PARKING					30  //  ͣ������(1)
#define		WHERE_MESSAGE_AREA1						32 	//  ����Χ��1(18)
#define		WHERE_MESSAGE_AREA2						50 	//  ����Χ��2(18)
#define		WHERE_MESSAGE_SERVERPORT			68 	//  �������˿ں�(2)
#define		WHERE_MESSAGE_SERVERIP				70 	//  ������IP��ַ(4)
#define		WHERE_MESSAGE_CBMODE					74 	//  �ش�ģʽ(3)
//#define		WHERE_MESSAGE_LISTENNO			77 69	//  �������� 1:���볤��  10:��������
#define		WHERE_MESSAGE_MATERNO					77	//  ���غ��� 1:���볤��  10:��������
#define		WHERE_MESSAGE_SMCNO						88  //  ����Ϣ���ĺ��� 1:���볤��  10:��������
#define		WHERE_MESSAGE_AUTHNO1 				99	//  ��Ȩ����1 1:���볤��  10:��������
#define		WHERE_MESSAGE_AUTHNO2 				110	//  ��Ȩ����1 1:���볤��  10:��������
#define		WHERE_MESSAGE_AUTHNO3 				121	//  ��Ȩ����1 1:���볤��  10:��������
#define		WHERE_MESSAGE_END							132	//  ����


//=====�Զ��ϴ�ģʽ================================================================================
enum{
	MODE_TIMES_INTERVAL = 0x01,	//�����ϴ���ʱ��
	MODE_TIMES_DISTANCE,		//�������ϴ�
	MODE_AUTO_INTERVAL,			//�Զ��ϴ���ʱ��
	MODE_AUTO_DISTANCE,			//�Զ��ϴ��������ϴ�
	MODE_STOP_SEND				//ֹͣ�ϴ�			 
};
//=====���ƺ�������================================================================================
enum{
	PHONE_MASTER = 0x00,					//���غ���
	PHONE_AUTHORIZATION1 ,			//��Ȩ����1
	PHONE_AUTHORIZATION2 ,      //��Ȩ����2 
	PHONE_AUTHORIZATION3 ,      //��Ȩ����3
	PHONE_SMSADDR,							//��ַ�������
	PHONE_SMC										//SMC����
};
//=====������������================================================================================
enum{
	SETUP_ALARM_SET = 0x00,			//������������
	SETUP_ALARM_OVERSPEED,			//���ٱ�������
	SETUP_ALARM_PARKING,			//ͣ����������
	SETUP_ALARM_ANTISTEAL,			//�Ƿ����ű���
	SETUP_ALARM_TIREDNESS,			//ƣ�ͼ�ʻ����
	
	SETUP_ALARM_DISTANCE = 0x06,	//���뱨������
	SETUP_ALARM_CANCEL = 0xFF		//�ر����б���
};
//=====Ӳ����ͷ�����=============================================================================
enum{
  	MT_LOGIN = 0x02,  
  	MT_LOGIN_ACK,
  	MT_DELIVERY,
  	MT_SUBMIT,
  	MT_HANDSHAKE,
  	MT_ACK
};
//=====Ӳ����������붨��============================================================================
enum{
	CC_HW_ERROR = 0x01,					//0.	������	
	CC_HW_SETUP = 0x02,					//2.	Ӳ��֧����� 	        (MT_LOGIN)
	CC_SW_SETUP,						//3.	���������� 	        (MT_LOGIN,MT_SUBMIT)
	CC_DE_STATE,						//4.	�ն�״̬ 		        (MT_SUBMIT)
	CC_GPS_STATE,						//5.	GPS��λ��Ϣ		        (MT_SUBMIT)
	CC_TE_QUERY_SW_SETUP,				//6.	��ȡ���������Ϣ        (MT_ DELIVERY)
	CC_TE_CTRLOIL,						//7.	�ر�/�����·��Ϣ       (MT_ DELIVERY)
	CC_TE_CTRLDOOR,						//8.	��/�� ����		        (MT_ DELIVERY)	
	CC_TE_CALLONCE,						//9.	��������: (�����ش�������Ϣ) 				   (MT_ DELIVERY)
	CC_TE_LISTEN,						//10.	�绰����: (�򿪳����ն˻�Ͳ��Զ�̼������ڶ���) (MT_ DELIVERY)
	CC_TE_TALK,							//11.	�绰ͨ��: (�򿪳����ն˻�Ͳ��Զ�̺���) 		   (MT_ DELIVERY)
	CC_TE_SETUP_UPLOADMODE, 			//12.	���������ϴ�ģʽ 	    (MT_ DELIVERY)
	CC_TE_SETUP_NUMBER,					//13.	���ø��ֺ���		    (MT_ DELIVERY)
	CC_TE_SETUP_SERVER,					//14.	���÷�����   		    (MT_ DELIVERY)
	CC_TE_SETUP_ALARM,					//15.	��������			    (MT_ DELIVERY)
	CC_TE_RESET,						//16.	�ն˸�λ		        (MT_ DELIVERY)
	CC_TE_RESTORE_FACTORY,				//17.	��㹳�������		    (MT_ DELIVERY)	
	CC_TE_FENCE,						//18.   ���õ���Χ��
	CC_GSM_STATE,						//19	GSM��λ��Ϣ	    (MT_SUBMIT)
	
	CC_BLIND_GPS_STATE = 32,			//32	����ä������			(MT_SUBMIT)
	CC_TE_QUERY_GG_ADDR_CN,				//33    ��ѯ���ĵ�ַ����		(MT_SUBMIT)
	CC_TE_QUERY_GG_ADDR_EN,				//34    ��ѯӢ�ĵ�ַ����		(MT_SUBMIT)
	CC_TE_RESULT_GG_ADDR				//35	���ص�ַ��
};

//=====�豸Ӧ������============================================================================
//bit7  1/0ִ�гɹ�/ʧ��, 
//bit0~6: ��������
//		0:��
//			1~7: reserved
//		7: ������·(old version)
//		8: Զ���п���(old version)
enum{
	ACK_LOCATION = 9,		//��λ
	ACK_LISTEN,				//����
	ACK_TALK,				//ͨ��
	ACK_SETUP_MODE,			//���������ϴ�ģʽ
	ACK_SETUP_PHONE,		//���ú���	
	ACK_SETUP_SERVER,		//���÷�����
	ACK_SETUP_ALARM,		//��������
	ACK_SETUP_RESET,		//�ն˸�λ
	ACK_RESTORE_FACTORY,	//��㹳���	
	ACK_SETUP_FENCE,		//����Χ��
		
	ACK_SETUP_PHOTO = 21,	//�ն�����
	
	ACK_DISABLE_OIL = 120,	//�������� (new version)
	ACK_ENABLE_OIL,			//�����·(new version)	
	ACK_CLOSE_DOOR,			//Զ�̹���(new version)
	ACK_OPEN_DOOR 			//Զ�̿���(new version)
};
//==============����������===============================================================

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

//	���õ���Χ��
unsigned char	    PL_TESetupFence( unsigned char	*pDataMsg);
				     	
unsigned char	 	PL_ComposeLogin( unsigned char  *pDest );
//	��֯GSM������Ϣ
unsigned char		PL_ComposeGSMData( unsigned char *pDest );

#endif //SW_GPRSPROTOCOLB

/////////////////////////////////////////////////////////////////////////////
// ------------------------public ------------------------------------------
/////////////////////////////////////////////////////////////////////////////
// ������
// ��������
// nCaller	:  ���պ���
// pMsg	  	:  ��������
// nTPUDL 	:  ���ݳ���
// nDCSType :  DCS  /DCS_7BIT/DCS_8BIT...

// 1: ����>0��ʾ�а���Э�����ݷ���   								x
// 2: ����0, ��pMsg���ı����ݡ���ʾ��������Ϣ����		x
// 3: ����0,��pMsgΪ�ա���ʾ����ش�����						x

// nCodeType = 0: 7λ���ݡ��ı���������
// nCodeType = 1: 8λ���뷢��������Э������
// nCodeType = 2: unicode���롣 ��������
// ����Ϊ0�����跢����
unsigned char 	PL_ParseSMSData( unsigned char  *pDataBuf,
							     unsigned char  nDataLen,	
							     unsigned char	*pCaller,    				                 
							     unsigned char  nDCS,
							     unsigned char	nTPUDL,
							     unsigned char 	*pCodeType );

unsigned char	PL_AddTEUID( unsigned char *pDest );

unsigned char   	PL_VerifyCheckSum(unsigned char *lpDataBuf, int nDataLen );

#if( SW_PROTOCOL_ASICO ) //CRC У�鷨
unsigned short  PL_CountVerifySum( unsigned char *lpDataBuf, int nDataLen);
#else
unsigned char   PL_CountVerifySum( unsigned char *lpDataBuf, int nDataLen);
#endif //SW_PROTOCOL_ASICO

//	���Ͳ�������
unsigned  char	PL_ParseConfig( unsigned char *pDest ,unsigned char	*pSrc );
//	 ��ȡ��������
unsigned char	PL_ReadConfig( unsigned char	*pDest );

#if	(EXTERNAL_FLASH)
unsigned char	PL_ComposeGPSBlindData( unsigned char	*pDest, 
										unsigned char	*pSrc,  
										unsigned char	nPackageSize );
#endif //EXTERNAL_FLASH		
						
#endif  //GPRSPROTOCOLB_H