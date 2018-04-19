///////////////////////////////////////////////////////////////////////////
// file name	: APP_GTM900.h
// description: GPRS/GSMЭ��ͷ�ļ�����(HW-GMT900)
// author		: real
// date			: 2009-02-17
// reamark		: GSM AT����
// optimize		: real


#include "..\hdr\main.h"

#ifndef  APP_GSM2_H_20090217
#define  APP_GSM2_H_20090217

#ifdef			HW_GTM900
extern  const	char	*s_strCMMCMD[];
enum{
	// ����OK
	GSM_OK = 0,
	//	��������ʾ
	GSM_ATE0,
	//  �رջ���
	GSM_ATE1,
	// ��GSM�ź�		at+csq?
	GSM_CSQ,
	// �ر����߹���
	GSM_SLEEP,
	//  ���ò�����
	//GSM_IPR,
	//  ����GSM��ȫ������
	GSM_CFUN,
	// 	������������
	GSM_NFV,
	//  ������Ƶ���
	GSM_NFO,
	//  ������Ƶ����
	GSM_NFI, 
	//  ��
	GSM_VLB,
	//	��Ƶͨ��ѡ��
	GSM_SNFS,
	//	���Ÿ�ʽ
	GSM_CMGF,
	//  �ر���������
	GSM_IPCLOSE,
	//  ��ʾ�ı���ʽ
	GSM_CSDH,
	//  �������ݸ�ʽ
	GSM_IOMODE,
	//	�ı����뷽ʽ
	GSM_CSMP,
	//	������ʾ
	GSM_CNMI,
	//	����PDP������
	GSM_CGDCONT,
	// ��ʼ����������
	GSM_ETCPIP,
	//	��ѯGPRS����״̬ 
	GSM_CGREG,
	//  ��GPRS����
	//GSM_IPOPENX,
	//  �򿪵�UDP����
	GSM_IPOPEN,
	//	��ѯGSM����״̬
	GSM_CREG,
	//	��绰
	GSM_ATD,
	//	�ӵ绰
	GSM_ATA,
	//	�ҵ绰
	GSM_ATH,
	//	ɾ������
	GSM_CMGD,
	//	������
	GSM_CMGR,
	//  ��ȡ����
	GSM_CLCC,
	//	�����	
	GSM_CPIN,
	//	����GPRS����
	//GSM_IPSENDX,
	//	����GPRS����
	GSM_IPSEND,
	//  ���Ͷ�������
	GSM_CMGS,
	//	��绰
	GSM_RING,
	//  �յ�������ʾ
	GSM_CMTI,
	//  ͨ������
	GSM_NO_CARRIER,
	//  �ҵ绰
	GSM_BUSY,
	//	���˽���
	GSM_NO_ANSWER,		
	//	�յ�GPRS����
	GSM_IPDATA,
	//	�յ���������
	GSM_CONNECT,	
	//	������
	GSM_ERROR,
	//  �������к�
	GSM_CGSN,
	//  �������ĺ���
	GSM_CSCA,
	//   ��ȡ����������
	GSM_DNSR,
	//  �ػ�
	//GSM_MSO,
	//  ����GPRS����
	GSM_CGATT,
	//  �ǽṹ������ҵ��
	GMS_CUSD,
	//	PDP�����ļ���
	GSM_CGACT,	
	//	��������
	GSM_TECNT,
	//	�Ҷ�����
	GSM_TEDISCNT,
	//l	��ȡ/���� �豸����
	GSM_TESETUP,
	
	CMM_CMD_END
};

///////////////////////////////////////////////////////////
// GSMӲ����ʼ��
void 					CMM_HWInit( void );
//	GSM900ͨ��ģ��Ӳ������	
void 					CMM_ReStart( void );
// power off
void 					CMM_PowerOff( void ); 
// power on
void 					CMM_PowerOn( void );
//��ʼ��GSMģ��
void 					CMM_Init(void);
//  ģ����Ƶ
unsigned char			CMM_ConfigCBand( void );
//��ʼ����������
unsigned char	CMM_IPInit(unsigned char *pStrGateway, unsigned char *pUser, unsigned char *pPsd);
unsigned char CMM_AT( void );
unsigned char CMM_IPClose( void );
// ����ATָ���ʽ
// ������������, ��ȡ���������
unsigned char CMM_GetCmdType(unsigned char *pMsg);
///////////////////////////////////////////////////////////
// �����źż���
unsigned char CMM_QuerySignal(void);

///////////////////////////////////////////////////////////	
// ��ѯGSM����״̬
// 0,2,3,4: δע��
// 1:				ע�ᣬ����
// 5:				ע��, ���
// ��Ч����1 ��Ч����0
unsigned char CMM_QueryNetworkStatus( void );
///////////////////////////////////////////////////////////////
// ��绰
unsigned char CMM_DialNumber( unsigned char *pStrNo );
// �ӵ绰
unsigned char CMM_AcceptCall( void );
//
unsigned char CMM_RejectCall( void );
// ����:				0�� free
//							1:  on use
unsigned char GSM_ReadCaller( unsigned char *pCaller );
/////////////////////////////////////////////////////////////
// ����:				���
unsigned char	CMM_QueryIMEI( unsigned  char	*pDest );
//	��ѯ��ַС��ID 
unsigned char	CMM_QueryNetworkLac( int	*pLac, int	*pID);
//////////////////////////////////////////////////////////////
//
//unsigned char GSM_OffModule( void );
//////////////////////////////////////////////////////////////
// ɾ������
unsigned char CMM_DeleteSMS( unsigned short nCnt );
//////////////////////////////////////////////////////////////
// ��ɾ��ȫ������
unsigned char CMM_DeleteAllSMS(void);
///////////////////////////////////////////////////////////////
//  ����PDU GSM����
unsigned char GSM_SendPDUSMS(unsigned char *pMsg,  unsigned char nDataLen, unsigned char nTitleLen );
unsigned char GSM_ReadPDUSMS(unsigned char nCnt, unsigned char *pBuf );
//////////////////////////////////////////////////////////////
// ��Ч����1
// ��Ч����0
unsigned char GSM_CheckSIMReady( void );
//////////////////////////////////////////////////////////////
//
unsigned char CMM_QueryModuleSN( unsigned char *pStr );
/////////////////////////////////////////////////////////////
// ��ȡ�������ĺ���
unsigned char GSM_QuerySMC( unsigned char *pStr );
/////////////////////////////////////////////////////////////////
// ��������������
unsigned char GSM_ParseDNS( unsigned char *pStrHost, unsigned char *pStrIP );
/////////////////////////////////////////////////////////////////
// 
/////////////////////////////////////////////////////////////////
//  ��ѯ����
unsigned char	GSM_ExecCUSD( unsigned char	*pDest, unsigned char	*pSrc, unsigned short	nTimeout );
////////////////////////////////////////////////////////////////////
// ��ѯGPRS����
// ��������״̬
// ��Ч����1
// ��Ч����0
unsigned char CMM_QueryIPNetworkStatus(void);
///////////////////////////////////////////////////////////////////
//
unsigned char GPRS_SetGateWay( unsigned char *pStr );

//	PDP�����ļ���
void	CMM_ActivationPDP( void );
///////////////////////////////////////////////////////////////////
//
unsigned char GPRS_SetUserPsd( unsigned char *pUser, unsigned char *pPsd );

//////////////////////////////////////////////////////////////
// ��������
// ����GSM_SUCCESS��ʾ�ɹ�������ʧ��
unsigned char CMM_IPConnect( 	unsigned char nMode, 
															unsigned char *pStrIP,
															unsigned short nPort );
// �������洢������Ϣ
void 					CMM_ParseMasterCmd(unsigned char *pMsg, unsigned char nCmdType);	
//����GPRS���� 0: ʧ��  1:  �ɹ�   2: ����״̬
unsigned char IP_SendData(unsigned char nMode, 
													unsigned char *pBuf, 
													unsigned char nLen);

//	�������ӳɹ�����
void	CMM_SendTECNT( void );
//	���ͶϿ�����,����GSMģ���Դ
void 	GSM_SendDISCNT( void );

#endif // HW_GTM900

#endif

