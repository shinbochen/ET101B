///////////////////////////////////////////////////////////////////////////
// file name	: app_GSM.h
// description: GPRS/GSMЭ��ͷ�ļ�����
// author			: wyb
// date				: 2008-04-07
// reamark		: GSM AT����
// optimize		: real

#ifndef  APP_GSM_H_20080408
#define  APP_GSM_H_20080408   

#define			TEXT_SMSSEND_END			"\x1A"
#define			TEXT_IP						"IP"
#define			TEXT_TCP					"TCP"
#define			TEXT_UDP					"UDP"
#define			TEXT_AT						"AT"
#define			TEXT_QUESTION				"?"
#define			TEXT_LF_RN					"\r\n"
#define         TEXT_LF						"\r"
#define 		TEXT_CR						"\n"

#define 		GSM_RECMAXBUFFER  										512
#define 		GSM_SETMAXBUFFER   										255
// ���������ջ�ռ��С(512 * 4�ֽڰ����Ĵ���) 
#define  		TASK_GSMDATA_RECEIVE_STK_SIZE         					40 // 128		
// �������ݽ��պ�Ĵ���������ջ�ռ��С(512 * 4�ֽڰ����Ĵ���)
#define  		TASK_GSMAPP_STK_SIZE  				  					400//292		

// ����GSM ATʱ��ش����� 5*600 = 3000 ms = 3second
#define 		GSM_ATTIME_CHECK										600
// ׼������GSM����ʱ����5*100 = 500 ms
#define 		GSM_SENDSPACE_CHECK										100
//  ����GPRSʱ��ȴ���־ 5*2400 = 10000ms = 10second
#define			GPRS_GPRSDATA_CHECK										2400
//  ��ʱ��ģ��ͨ�� 2*60000 = 120000ms = 120s = 2minte
#define    		GSM_AT_CHECK_TIMEOUT									24000
// 10*5=50ms
#define 		GSM_LOWAIT_TIME											10
// ��С�ź�ֵ
#define 		GSM_SIGNAL_LOWBOUND										6 
// ���GSM�ź� 5*12000 = 60000 = 60��
#define			GSM_CHECK_SIGNAL_TIMEOUT							    12000
// ���GSM�ź�һ��20����û���ź�,����ģ��
#define     	GSM_LOW_SINGLE_MAXCNT									20 
// ��½ָ��ͼ��	2000*5  = 20000ms = 10second
#define			GSM_LOGIN_SPACE											2000
// GPRS ʮ��������һ��
#define 		GSM_CONNECT_DEGREE										4000
//  ��¼��������
#define			IP_FAIL_CONNECT_MAX										5
// ��ѯGPRS�����ź�, 20����û���ź�����ģ��, 20��1��, 54������
#define			GSM_CGREG_MAXCNT										54
// ��ʱ���ä�������ϴ�	(20���Ӽ��һ��)
#define			CHECK_GPSDATA_FLASH										12000*20
// ����GPSä������,һ����෢��7��GPS����
#define			GSM_SENDFLASH_MAXDATA									7
// ����Ϊ30��
#define			MIN_TIME_SEC											30

enum{
	DCS_7BIT = 0x00,
	DCS_8BIT,
	DCS_UNICODE
};
enum{
	CB_BYTIME = 0x03,			// ��ʱ��ش���
	CB_BYDISTANCE,				// ������ش�
	CB_STOP						// ֹͣ�ش�
};	
//AT���÷���״ֵ̬
enum {
	GSM_NG	= 0,
	GSM_SUCCEED,   
	GSM_RESULT_ERROR_MASTER
};
///////////////////////////////////////////////////////////////////////
#define		COMM_USE_MARK										  0x01
#define		COMM_USE_GPRS										  0x01

#define		COMM_GPRS_MARK									  	  0x02
#define		COMM_GPRS_TCP										  0x02
// �ͻ�Э��
#define     SMS_POSITION_FEEDBACK_FORMAT		0x04		// 1/0 nmea/maptrack

#define     GPRS_PROTOCOL_TYPE					0x08		// 0/1 self /jaya

#define		COMM_SMS_WORK_MODE					0x10		// 0/1 ����Э��/����Э��

// ��·״̬ 
#define		DS_OIL_STATE						0x01    // 0/1 �ָ���·/����
// ��Ӣ��ת��
#define		DS_LANG_CN							0x02	// 0/1 Ӣ��	
	


#define		TELNUMBER_LENGTH					20
#define		PASSWORD_LENGTH						4
#define		SERVERDOMAIN_LENGTH					29
#define		SERVERIP_LENGTH						15

#define   	DEUID_LENGTH						8			
// ����������, ��������
#define		UDP_HANDSHAKE_CNT 				    4		
		
typedef struct _STGSMSETUP{
	unsigned char 	m_nCommMode;  							// SMS/GPRS	
	unsigned char  	m_nCBMode;		   												 
	unsigned short	m_nCBSpace;
	
	// SMS Section
	unsigned char 	m_strPassword[PASSWORD_LENGTH+1]; 	// ��������
	unsigned char	m_strSMCNo[TELNUMBER_LENGTH+1];		// �������ĺ���
	unsigned char 	m_strMaterNo[TELNUMBER_LENGTH+1]; 	// ���غ���
	unsigned char 	m_strAuthNo1[TELNUMBER_LENGTH+1]; 	// ��Ȩ����1
	unsigned char 	m_strAuthNo2[TELNUMBER_LENGTH+1]; 	// ��Ȩ����2
	unsigned char 	m_strAuthNo3[TELNUMBER_LENGTH+1]; 	// ��Ȩ����3
	
	unsigned char 	m_strAuthNo4[TELNUMBER_LENGTH+1]; 	// ��Ȩ����4
	unsigned char 	m_strAuthNo5[TELNUMBER_LENGTH+1]; 	// ��Ȩ����5
	
	unsigned char	m_strSMSAddrNo[TELNUMBER_LENGTH+1];	// ��ַ����Ϣ�������
	
	// GPRS section
	unsigned char	m_strServerDomain[SERVERDOMAIN_LENGTH+1];
	unsigned char 	m_strServerIP[SERVERIP_LENGTH+1];
	unsigned short	m_nServerPort;	
	unsigned char 	m_strGateway[SERVERDOMAIN_LENGTH+1];
	unsigned char	m_strUser[TELNUMBER_LENGTH+1];
	unsigned char	m_strPsd[TELNUMBER_LENGTH+1];	
	unsigned char 	m_strUID[TELNUMBER_LENGTH+1];
	// ����ʱ�䣬Ĭ��30second
	unsigned short	m_nHandShakeTime;
	//
	unsigned char	m_nDeviceState;
	// ӡ��ר��UID�ϴ���
	unsigned char	m_strTEUID[TELNUMBER_LENGTH+1]; 
	
}STGSMSETUP,*PSTGSMSETUP;


/////////////////////////////////////////////////////////////////////////////////////////////
//
typedef struct _STGSMSTATE {
	unsigned char		m_nFlag;
	unsigned char		m_nGSMSignal;
	unsigned char		m_nHandShakeCnt;					// ���ִ���
	unsigned char		m_nConnectFailedCnt;  				// ��¼��������
	unsigned char		m_nTCPRCntionCnt;					// TCP���Ӳ��ɹ�,����
	unsigned char		m_nQueryGPRSCnt;					// ��ѯGPRS�����ź�
	unsigned char		m_nDialLatLng;						// ��绰���λض���
	unsigned char 		m_strFWDNo[TELNUMBER_LENGTH+1];		// ��������ת������
	unsigned char		m_strForward[TELNUMBER_LENGTH+1];	// ����ת������
	unsigned char		m_strDialNo[TELNUMBER_LENGTH+1];	// ��绰�ض���
	unsigned int		m_nGPRSAlarmSMSState;				// GPRSģʽ�µ�״̬		
}STGSMSTATE,*PGSMSTATE;

/////////////////////////////////////////////////////////////////////////////////////////////
// step is 5ms n*200*5 = n second
#define	GSM_GetMaxHandShakeTime( )						(g_stGSMSetup.m_nHandShakeTime*200)
#define GSM_SetToSMSCommMode( )							(g_stGSMSetup.m_nCommMode &= ~COMM_USE_GPRS)	
#define GSM_SetToGPRSCommMode( )						(g_stGSMSetup.m_nCommMode |= COMM_USE_GPRS)
#define	GSM_IsGPRSCommMode( )							(g_stGSMSetup.m_nCommMode & COMM_USE_GPRS)
#define	IP_IsTCPCommMode( )								(g_stGSMSetup.m_nCommMode & COMM_GPRS_TCP)
#define IP_SetTCPCommMode( )							(g_stGSMSetup.m_nCommMode |= COMM_GPRS_TCP)
#define IP_SetUDPCommMode( )						    (g_stGSMSetup.m_nCommMode &= ~COMM_GPRS_TCP)

#define GSM_SetSignal(nSignal)  				(g_stGSMState.m_nGSMSignal = (nSignal))
#define GSM_GetSignal( )			  			(g_stGSMState.m_nGSMSignal)	

#define	CALL_ONLINE										0x01
#define	SEND_SMS_READY									0x02
#define	GSM_WORK_BUSY									0x04
#define	GSM_NEED_RESET									0x08

#define	GPRS_NET_AVAI									0x10
#define	GPRS_CONNECT_OK									0x20
// ���ò�������
#define	DS_SETUP_CONFIG									0x40	// 0/1 ����/����

#define	CMM_SetCallOnLine( )							(g_stGSMState.m_nFlag |= CALL_ONLINE )
#define	CMM_SetCallOffLine( )							(g_stGSMState.m_nFlag &= ~CALL_ONLINE )

#define	CMM_SetSendSMSReady( )							(g_stGSMState.m_nFlag |= SEND_SMS_READY )
#define	CMM_SetSendSMSUnReady( )						(g_stGSMState.m_nFlag &= ~SEND_SMS_READY )
#define CMM_IsSendSMSReady( )  							(g_stGSMState.m_nFlag & SEND_SMS_READY )


#define	GSM_SetNeedReset( )								(g_stGSMState.m_nFlag |= GSM_NEED_RESET )
#define	GSM_SetUnNeedReset( )							(g_stGSMState.m_nFlag &= ~GSM_NEED_RESET )
#define	GSM_IsNeedReset( )								(g_stGSMState.m_nFlag & GSM_NEED_RESET )

#define	IP_SetConnectOK( )							    (g_stGSMState.m_nFlag |= GPRS_CONNECT_OK )
#define	IP_SetConnectNG( )							    (g_stGSMState.m_nFlag &= ~GPRS_CONNECT_OK )
#define	IP_IsConnected( )								(g_stGSMState.m_nFlag & GPRS_CONNECT_OK )

// ���ò�������  (�¼ӹ���)
#define  GSM_IsSetupConfig() 						(g_stGSMState.m_nFlag &   DS_SETUP_CONFIG )
#define  GSM_SetSetupConfig()						(g_stGSMState.m_nFlag |=  DS_SETUP_CONFIG )  
#define  GSM_ClrSetupConfig()						(g_stGSMState.m_nFlag &= ~DS_SETUP_CONFIG ) 

#define UDP_IncHandShakeCnt( )						(g_stGSMState.m_nHandShakeCnt++ )
#define UDP_ClrHandShakeCnt( )						(g_stGSMState.m_nHandShakeCnt = 0 )
#define	UDP_GetHandShakeCnt( )						(g_stGSMState.m_nHandShakeCnt)

#define	IP_GetConnectFailedCnt( )					( g_stGSMState.m_nConnectFailedCnt )
#define IP_IncConnectFailedCnt( )					( g_stGSMState.m_nConnectFailedCnt++ )	
#define IP_ClrConnectFailedCnt( )					( g_stGSMState.m_nConnectFailedCnt = 0 )	

#define TCP_GetRCntionCnt( )						( g_stGSMState.m_nTCPRCntionCnt )
#define	TCP_IncRCntionCnt( )						( g_stGSMState.m_nTCPRCntionCnt++ )
#define	TCP_ClrRCntionCnt( )						( g_stGSMState.m_nTCPRCntionCnt = 0 )

//  SMS�������
#define  COM_IsSMSFeedbackUseJave()  ( g_stGSMSetup.m_nCommMode & SMS_POSITION_FEEDBACK_FORMAT  )
#define  COM_SetSMSFeedbackUseJave() ( g_stGSMSetup.m_nCommMode |= SMS_POSITION_FEEDBACK_FORMAT )
#define  COM_ClrSMSFeedbackUseJave() ( g_stGSMSetup.m_nCommMode &= ~SMS_POSITION_FEEDBACK_FORMAT ) 

//  GPRS�������
#define	 COM_IsGPRSProtocolType()    ( g_stGSMSetup.m_nCommMode & GPRS_PROTOCOL_TYPE  ) 
#define  COM_SetGPRSProtocolType()   ( g_stGSMSetup.m_nCommMode |= GPRS_PROTOCOL_TYPE )
#define	 COM_ClrGPRSProtocolType()   ( g_stGSMSetup.m_nCommMode &= ~GPRS_PROTOCOL_TYPE ) 

// SMSЭ�鷢�ͷ�ʽ
#define	 COMM_IsSMSFeedbackUseProtocol()		 ( g_stGSMSetup.m_nCommMode & COMM_SMS_WORK_MODE  ) 
#define	 COMM_SetSMSFeedbackUseProtocol()		 ( g_stGSMSetup.m_nCommMode |= COMM_SMS_WORK_MODE  )
#define	 COMM_ClrSMSFeedbackUseProtocol()		 ( g_stGSMSetup.m_nCommMode &= ~COMM_SMS_WORK_MODE  )

//  ����������,��·��ǰ�Ƿ�Ͽ�
#define  GSM_IsOilState()  (g_stGSMSetup.m_nDeviceState &   DS_OIL_STATE )
#define  GSM_SetOilState() (g_stGSMSetup.m_nDeviceState |=  DS_OIL_STATE ) 
#define  GSM_ClrOilState() (g_stGSMSetup.m_nDeviceState &= ~DS_OIL_STATE ) 

//  ��Ӣ��ת��
#define  GSM_IsLangState()  (g_stGSMSetup.m_nDeviceState  &   DS_LANG_CN ) 
#define  GSM_SetLangState() (g_stGSMSetup.m_nDeviceState  |=  DS_LANG_CN ) 
#define  GSM_ClrLangState() (g_stGSMSetup.m_nDeviceState  &= ~DS_LANG_CN ) 

//  ��ѯGPRS����
#define  GSM_GetCGREGCnt()   ( g_stGSMState.m_nQueryGPRSCnt )
#define  GSM_IncCGREGCnt()	 ( g_stGSMState.m_nQueryGPRSCnt++ )
#define	 GSM_ClrCGREGCnt()	 ( g_stGSMState.m_nQueryGPRSCnt = 0 )


// ��GPRSģʽ�²�������,ͬʱ������
#define  GPRS_ClrAlarmSMSMode()  			 ( g_stGSMState.m_nGPRSAlarmSMSState = 0 )
#define  GPRS_ClrNumberSMSMode( nALState )   ( g_stGSMState.m_nGPRSAlarmSMSState &= (~nALState) )

extern	STGSMSTATE			g_stGSMState;  
extern  STGSMSETUP			g_stGSMSetup; 
extern  unsigned char		*lpLargeBuf1;
extern  unsigned char		*lpLargeBuf2;
extern  unsigned char 		*lpLargeBuf3;
extern  unsigned char 		*lpLargeBuf4;

// дGSM���ݽṹ
void GSM_WriteSetupData(void);
// ��ȡGSM���ݽ�
void GSM_ReadSetupData(void);
// ��ʼ��GSM state
void CMM_InitState( void );
//GSM ATָ�  
//GSM ATָ�
unsigned char CMM_SendATCmd(unsigned char nCmdType, 
														unsigned char nLFRN,
														unsigned char *pMsg);
// ִ��GSM ATָ��
// nCmdType 	: ��������
// pPara			: �������
// pReturnMsg	: ��Ϊ�ձ�ʾ��Ҫ�õ����ص�����( �����������ַ����� )
//						: Ϊ�� ֻҪ�ȵ�OK
unsigned char CMM_ExecATCmdN( unsigned char  nCmdType, 
												  	  unsigned short nPara,
												  	  unsigned short  nTimeout,
														  unsigned char  nOKType,		
														  unsigned char  nNGType,
														  unsigned char *pReturnMsg );														 
// ִ��GSM ATָ��
// nCmdType 	: ��������
// pPara			: �������
// pReturnMsg	: ��Ϊ�ձ�ʾ��Ҫ�õ����ص�����( �����������ַ����� )
//						: Ϊ�� ֻҪ�ȵ�OK
// CGSN�ر���
unsigned char CMM_ExecATCmd( unsigned char nCmdType, 	
														 unsigned char *pPara,
														 unsigned short  nTimeout,
														 unsigned char nOKType,		
														 unsigned char nNGType,
														 unsigned char *pReturnMsg );


void 					InsertNewMsg( unsigned char nCnt );
// GSM���߳̿�
void 					GSM_Task_Creat( void );
// ���ڽ�������
// GSM
void 					GSM_TaskDataReceive(void * pData);
//�������ݽ��պ�Ĵ�������
void 					GSM_TaskApp(void *pData);
//��ȡGSM ATָ�����
int 					CMM_ReadCmd(unsigned char *pBuf, unsigned short nDataLen);
void 					CMM_SignalTest( void );
unsigned char 			CMM_IPMAINTENANCE( unsigned char	*lpLargeBuf );
// 
void 					CMM_ATTest(void);
//���ű� �뷽ʽ
unsigned char GSM_GetDCSType( unsigned char nType );
// ����: "1BE08602"
// ���: "abcd"
unsigned char	GSM_ParseTPUD( unsigned char *lpDest, unsigned char* lpSrc,  unsigned char nLen, unsigned char nDCSType);
// ����: 0x68,0x31,0x08,0x00,0x75,0x80,0xF5
// ���: "8613800057085"
unsigned char GSM_ParseTPOA(  unsigned char *lpDest, unsigned char* lpSrc, unsigned char nLen);
// ����:0x08,0x91,0x68
//      from"0891683108705505F0040D91683135804115F9000080508241947523041BE08602"
//      
// ���:	
//       pBuf: 			TP_UD
//       pSMC: 			SMSC
//       pCallerNo: TP_OA
//			 pDCSType:  DCS type
// ����: ���ݳ���
unsigned char GSM_ParsePDUSMS( unsigned char	*pDest,
															 unsigned char	*pSMC,
															 unsigned char 	*pCallerNo,
															 unsigned char	*pTPUDL,
															 unsigned char 	*pDCSType,
															 unsigned char	*pSrc  );

//////////////////////////////////////////////////////////////////////////
// ����TPUDʵ�ʵĳ���.AT+CMGSʱ�õ�
// Ϊ�򵥣�����һ�ɲ���7λ����(160���ֽ�)
unsigned char	GSM_ComposeSend7GSMData( unsigned char	*lpDest,
																			 unsigned char	*pResultLen,
																			 unsigned char	*lpSMCNo, 
																			 unsigned char	*lpRecvNo,
																			 unsigned char	*lpMsg  );
																			 
//////////////////////////////////////////////////////////////////////////
// ����TPUDʵ�ʵĳ���.AT+CMGSʱ�õ�
// ���140
unsigned char	GSM_ComposeSend8GSMData( unsigned char	*lpDest,
																			 unsigned char	*pResultLen,
																			 
																			 unsigned char	*lpSMCNo, 
																			 unsigned char	*lpRecvNo,
																			 unsigned char	*lpBuf, 
																			 unsigned char 	nLen );		
																			 
unsigned char	GSM_ComposeSend16GSMData( unsigned char	*lpDest,
										 unsigned char	*pResultLen,
										 
										 unsigned char	*lpSMCNo, 
										 unsigned char	*lpRecvNo,
										 unsigned char	*lpMsg, 
										 unsigned char 	nLen  );																			 
//////////////////////////////////////////////////////////////////////////
// ����: TPUDʵ�ʵĳ���.AT+CMGSʱ�õ�
// input		: nDCS :TP-DCS
// lpTPUDBuf: TP_UDL + TP_UD
// nLen			: length of lpBuf
unsigned char	GSM_ComposeSendGSMData( unsigned char *lpDest,
																			unsigned char *pResultLen,
																			
																 			unsigned char *lpSMCNo, 
																 			unsigned char *lpRecvNo, 
																 			unsigned char *lpTPUDBuf,
																 			unsigned char nLen, 
																 			unsigned char nTPUDL,
																			unsigned char nDCS  );			
//  ���ͱ�������
void  			CMM_SMSSendAlarmData( unsigned char *lpLargeBuf );
void 			CMM_ParseNewMsg( unsigned char nCnt );

//////////////////////////////////////////////////////////////////////////
// ����:"089124"
// ���:0x08,0x91,0x24 ������3
unsigned short P_StrToHex( unsigned char* lpDestBuf, unsigned char* lpSrcBuf, unsigned short nLen );
// ����:  "1358008"
// ���:	0x31, 0x85, 0x00, 0xF8
unsigned char P_StrToBCD( unsigned char* lpDestBuf, unsigned char* lpSrcBuf, unsigned char nLen );
// 7-bit����
// pSrc:		Դ�ַ���ָ��
// pDst:		Ŀ����봮ָ��
// nSrcLength:  Դ�ַ�������
// ����:		Ŀ����봮����
unsigned char P_Code8BitTo7bit(unsigned char* pDest, unsigned char* pSrc, unsigned char nSrcLength);
// 7-bit����
// pSrc:		Դ���봮ָ��
// pDest:		Ŀ���ַ���ָ��
// nSrcLength:  Դ���봮����
// ����:		Ŀ���ַ�������
unsigned char P_Decode7bitTo8Bit(unsigned char* pDest, unsigned char* pSrc, unsigned char nSrcLength);
// GSM����ASCII��
unsigned char P_GSMToASCII(  char* lpDestBuf, char *lpSrcBuf, unsigned char nLen );
// 
void 		  P_UnicodeToASCII( unsigned char	*pDest, unsigned char	*pSrc , unsigned char nLen );
// 	����Flash���ݵķ���
unsigned char	CMM_FlashManagement( unsigned char *lpLargeBuf );
// ������ʱ����(6)|��λ��־(1)|����(4)|γ��(4)|����(2)|�ٶ�(1)|Ӳ��״̬(4)|����״̬(4)|ACC����ʱ��(2)|������(1)
unsigned char	CMM_SaveGPSToFlash( void );
// 	��֯���ŷ��� (2010-05-12) ��Ϸ�����,��ʡ����ռ�
//	    pPhone:	 ���ͺ���
//	lpLargeBuf:  ��������
//		  nLen:  ���ݳ���
//	 nCodeType:  ���͸�ʽ
void CMM_SendSMSData( unsigned char	*pPhone,unsigned char	*lpLargeBuf,unsigned char	nLen,unsigned char	nCodeType );
#endif

