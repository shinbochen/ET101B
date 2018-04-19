#include "..\hdr\main.h"

#ifndef  GPRSPROTOCOL_ASICO_H
#define  GPRSPROTOCOL_ASICO_H

#if( SW_GPRSPROTOCOL_ASICO )

// ͬ����  �ն��ϴ���Ϣ��ͷ
#ifndef		SYN_TEHEADERCODE1
#define		SYN_TEHEADERCODE1				0x24  //$
#endif
#ifndef		SYN_TEHEADERCODE2
#define		SYN_TEHEADERCODE2				0x24  //$
#endif

// ͬ����  �������ϴ���Ϣ��ͷ
#ifndef		SYN_SVEREHEADERCODE1
#define		SYN_SVEREHEADERCODE1			0x40  //@
#endif
#ifndef		SYN_SVEREHEADERCODE2
#define		SYN_SVEREHEADERCODE2			0x40  //@
#endif

// ������
#ifndef		SYN_R_ENDCODE					
#define		SYN_R_ENDCODE					0x0d
#endif
#ifndef		SYN_N_ENDCODE					
#define		SYN_N_ENDCODE					0x0A
#endif    

#define		ASICO_DEUID_LEN					0x07

// ��ͷ
#define		WHERE_MESSAGE_HEAD1				0x00
#define		WHERE_MESSAGE_HEAD2				0x01
// ���ݳ���
#define		WHERE_DATALEN_HIGHT				0x02
#define		WHERE_DATALEN_LOW				0x03
// DEUID������ʼλ
#define		WHERE_MESSAGE_DEUID				0x04
// ������
#define		WHERE_CMD_HIGHT		    		0x0B
#define		WHERE_CMD_LOW		    		0x0C
// ��������
#define		WHERE_MESSAGE_DATA			    0x0D

//=====��½���============================================================================
enum{
	CMD_ERROR = 0x00,
	CMD_SUCCEED
};

//=====Ӳ����������붨��============================================================================
enum{
	CC_TE_LOGIN = 0x5000,		 	 //�ն˵�½
	
	CC_TE_LOGIN_ACK = 0x4000, 		 //��������ȷ���ն˵�½
	
	
	CC_TE_CALLONCE = 0x4101,		 //��������
	CC_TE_SETUP_TRACKTIME,			 //���ö�λʱ��
	CC_TE_SETUP_NUMBER,				 //������Ȩ����
	CC_TE_SETUP_OVERSPEED = 0x4105,			 //���ó��ٱ���
	CC_TE_SETUP_OUTGEO,				 //������λ����
	CC_TE_SETUP_FENCE,				 //���õ���դ��
	CC_TE_RESTORE_FACTORY = 0x4110,	 //��㹳�������
	CC_TE_MULTI_SETUP =	0x4115,	//�Զ������	
	CC_TE_SETUP_FENCE2 = 0x4127,	 //���õ���դ��
	
	CC_TE_EXPAND_CALLONCE = 0x5101,	 //��������
	CC_TE_ACK_TRACKTIME = 0x5100,	 //Ӧ������ʱ����	
	
	CC_TE_HS_DATA = 0x8001,			 //�����ź�
	CC_TE_LISTEN_CAR = 0x8002,		 //���ͳ��ؼ���
	CC_TE_TWO_WAY_CALL,				 //����˫��ͨ��
	CC_TE_FUEL_CONTROL,				 //������·����
	CC_TE_CENTER_LOCK_CONTROL,		 //�����п���	
	CC_TE_ALARM_SETUP,				 //��������
	CC_TE_READ_ALARM_STATUS,		 //������״̬
	SETUP_ALARM_PARKING,		 	 //����ͣ������	
	SETUP_ALARM_ANTISTEAL,			 //�Ƿ����ű���
	SETUP_ALARM_TIREDNESS,			 //ƣ�ͼ�ʻ����	
	
	CC_TE_READ_TRACKTIME =0x9002,	 //��ȡ��λʱ��
	CC_TE_READ_NUMBER,				 //��ȡ��Ȩ����
	CC_TE_READ_OVERSPEED,			 //��ȡ���ٱ���
	CC_TE_READ_OUTGEO,				 //��ȡ��λ����
	CC_TE_READ_FENCE,				 //��ȡ����դ��

	
	CC_TE_SINGLE_LOCATION_REPORT = 0x9955,	 //��һ�ϴ�	
	
	CC_TE_ALARM_CMD = 0x9999		 //��������
};

//	BCDת��Ϊʮ������
unsigned char   BCDToHex( unsigned char	nData);
//	�ֽ��͵�ʮ������
unsigned short  ByteToInt(unsigned char* pSrc);
//�ó�DEUID
void  			PL_ParseDEUID( unsigned char	*pDest, unsigned char *pSrc );
//  ��֯ ��ͷ(2B)|����(2B)|DEUID(7B)|������(2B) 
void			PL_ComposeHeadData( unsigned char	*pDest, 
									unsigned char	nDataLen,
									unsigned short	nCmdCode );
unsigned char	PL_ComposeGPSData( unsigned char  *pDest );			
unsigned char 	PL_ComposeHSData( unsigned char	*pDest );	
unsigned char	PL_ComposeLogin( unsigned char  *pDest );
unsigned char	PL_ParseData(unsigned char *pDest, unsigned char *pSrc, unsigned char nLen );	
//   �����ն�������Ϣ  MT_DELIVERY
unsigned char	PL_ParseDelivery( unsigned char  *pDest, 
								  unsigned char  *pSrc,
								  unsigned char	 nDataLen,
								  unsigned int	 nCmdCode );
//	���ø���ʱ��
unsigned char   PL_ComposeTrackTime( unsigned char	*pSrc,
								 	unsigned short	nCmdCode,
								 	unsigned char	nStuts );
//  ���ú���
unsigned char	PLS_SetupGpsMasterNo( unsigned char	*pSrc );							
//	����������λ����
unsigned char	PLS_SetupGpsOutGeo( unsigned char	*pSrc );
//	���þ���
unsigned char	PLS_SetupFence( unsigned char	*pSrc,
								unsigned char	nCmdCode );	
//  ��ȡ��Ȩ����  17+16+16;
unsigned char   PL_ComposeReadNumber( unsigned char	*pDest, 
									  unsigned char	nPhoneType,	
									  unsigned char	*pPhone );																
//									  
int 			PLS_ExplainLatitudeData(unsigned char *pData);	
int  			PLS_ExplainLongitudeData(unsigned char *pData);							
//	��������Ӧ������
unsigned char	PL_ComposeACKData( unsigned char	*pDest,
								   unsigned char	nFlag,	
								   unsigned short	nCmdCode );

						//  ����������״̬
unsigned char	PL_ComposeReadAlarm( unsigned char	*pDest, 
									 unsigned short	nCmdCode );	   
////////////////////////////////////////////////////////////////////////////////
//   ��·����
//   һ�����ֽ�
//  1 Byte: ������·  0: �ָ���·	1: ����	  2: ������
//  2 Byte: ��������  0: ����		1: ���� 	  2: ������
unsigned char	PL_ComposeMultiSetup( unsigned char	*pSrc );
//   ����״̬��Ϣ
void			PL_UPDataStatus( void );
//	û�ж�λ��ʱ��ظ�
unsigned char	PL_ComposeNoValidData( unsigned char	*pDest );

#endif //SW_GPRSPROTOCOL_ASICO

#endif //GPRSPROTOCOL_ASICO_H 

