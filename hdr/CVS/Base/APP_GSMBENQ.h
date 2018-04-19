/////////////////////////////////////////////////////////////
//file name: APP_GSMBENG.H
//     Date: 2008-3-02    
//	 Author: wyb	 
// optimize: real
// op_date : 2009-3-30
/////////////////////////////////////////////////////////////
#ifndef  	  APP_GSMBENG_H_20090330
#define  	  APP_GSMBENG_H_20090330

#define  	  BQ_GSM

#define			TEXT_SMSSEND_END			"\x1A"
#define			TEXT_IP								"IP"
#define			TEXT_AT								"AT"
#define			TEXT_QUESTION					"?"
#define			TEXT_LF_RN						"\r\n"

enum{
		GSM_OK,
		GSM_ATE0,			//�رջ���ʾ
	  GSM_ATE1,			//��������
	  GSM_CSQ,			//��GSM�ź�		at+csq?
	  GSM_CGSN,			//IMEI ���
	  GSM_CREG,     //��ѯGSM����
	  GSM_CGREG,		//��ѯGPRS����
	  GSM_CPIN,			//�����	  
	  GSM_CGSN,			//�������к�
	  
	  //�绰ָ��
	  GSM_CLCC,			//��ѯ�������	  
		GSM_ATD,		  //��绰		
		GSM_ATA,			//�ӵ绰		
		GSM_ATH,			//�ҵ绰		
		GSM_NO_CARRIER,//ͨ������	
		GSM_BUSY,			//�ҵ绰	
		GSM_NO_ANSWER,//���˽���
		GSM_CLIP,			//����������ʾ
		GSM_SG,				//��˷�����
		GSM_SAMP,			//��Ͳ����
		
	  //����ָ��
	  GSM_CSMP,			//���͵ı��뷽ʽ	  
	  GSM_CSCA,			//��ȡ���ĺ���
	  GSM_CMGF,			//���Ÿ�ʽ
	  GSM_CSDH,			//��ʾ�ı���ʽ
	  GSM_CNMI,			//������ʾ
	  GSM_CMGR,			//��ȡ����
	  GSM_CMGS,			//���Ͷ���
	  GSM_CMGD,			//ɾ������
	  GSM_CSMP,			//�����ʽ
	  
	  //����ע��
	  GSM_CGDCONT,	//����PDP������
	  GSM_CGATT,		//ע��GPRS����
	  GSM_CUSD,			//�ǽṹ������ҵ��
	  GSM_CLCC,		  //��ѯ����
	  GSM_CPIN,			//��ѯ��
};

///////////////////////////////////////////////////////////////////
//  GSMӲ����ʼ��
void CMM_HWInit( void );
//	GSMͨ��ģ��Ӳ������	
void CMM_ReStart( void );
//  ��ʼ��GSMģ��
void CMM_Init( void );
//	��ʼ����������	
unsigned char CMM_IPInit( unsigned char *pStrGateway, unsigned char *pUser, unsigned char *pPsd );
// ������������, ��ȡ���������
unsigned char CMM_GetCmdType(unsigned char *pMsg);
// 
unsigned char CMM_AT( void );
// �����źż���
unsigned char CMM_QuerySignal( void );
// ��ѯGSM����״̬
// 0,2,3,4: δע��
// 1:				ע�ᣬ����
// 5:				ע��, ���
// ��Ч����1 ��Ч����0
unsigned char CMM_QueryNetworkStatus(void);
// ��绰
unsigned char CMM_DialNumber( unsigned char *pStrNo );
// �ӵ绰
unsigned char CMM_AcceptCall( void );
//  �ҵ绰
unsigned char CMM_RejectCall( void );
//  ��ѯIMEI���
unsigned char	CMM_QueryIMEI( unsigned  char	*pDest );
/////////////////////////////////////////////////////////////
// ����:				0�� free
//						1:  on use
// +CLCC : 1,1,4,0,0,"13902288001",161
unsigned char GSM_ReadCaller( unsigned char *pCaller );
//
unsigned char GSM_OffModule( void );
// ɾ������
unsigned char CMM_DeleteSMS( unsigned short nCnt );
// ɾ��ȫ������  
unsigned char CMM_DeleteAllSMS(void);
///////////////////////////////////////////////////////////////
// ��Ч����1
// ��Ч����0
unsigned char GSM_CheckSIMReady( );
//
unsigned char CMM_QueryModuleSN( unsigned char *pStr );
// ��ȡ�������ĺ���
unsigned char GSM_QuerySMC( unsigned char *pStr );
// ��������������
unsigned char GSM_ParseDNS( unsigned char *pStrHost, unsigned char *pStrIP );
//  ��ѯ����
unsigned char	GSM_ExecCUSD( unsigned char	*pDest ,unsigned char	*pSrc, unsigned short	nTimeout );
////////////////////////////////////////////////////////////////////
// ��ѯGPRS����
// ��������״̬
// ��Ч����1
// ��Ч����0
unsigned char CMM_QueryIPNetworkStatus(void);
// �������洢������Ϣ
void CMM_ParseMasterCmd(unsigned char *pMsg, unsigned char nCmdType);

#endif