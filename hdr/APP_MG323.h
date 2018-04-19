///////////////////////////////////////////////////////////////////////////
// file name	: APP_MG323.h
// description: GPRS/GSMЭ��ͷ�ļ�����(HW-MG323)
// author		  : wyb
// date			  : 2011-11-16
// reamark		: GSM AT����
// optimize		: wyb


#include "..\hdr\main.h"

#ifndef  APP_MG323_H_20111116
#define  APP_MG323_H_20111116

#if( HW_MG323 )

#define   MIN_GPRS_SERVER_ABORT				1
#define   MAX_GPRS_SERVER_ABORT				2000



extern  const	char	*s_strCMMCMD[];
enum{
	// ��ָͨ������	
	GSM_OK = 0,				//  ����OK	
	GSM_ATE0,				//	��������ʾ	
	GSM_ATE1,				//  �رջ���
	GSM_CGSN,				//  ��ѯIMEI
	GSM_CPIN,				//	�����		
	GSM_CFUN,				//  ����GSM��ȫ������
	GSM_CSQ,				//  ��GSM�ź�		
	GSM_CREG,				//	��ѯGSM����״̬	
	GSM_CGREG,				//	��ѯGPRS����״̬ 	
	GSM_Q0,
	
	// ��������ָ��	
	GSM_CLVL,				// 	������������	
	GSM_CMIC, 				//  ������Ƶ���	
	GSM_ECHO,				//  �������ƹ���	
	GSM_SWSPATH,			//	��Ƶͨ��ѡ��
	
	// ͨ��ָ��	
	GSM_ATD,				//	��绰
	GSM_ATA,				//	�ӵ绰	
	GSM_ATH,				//	�ҵ绰
	GSM_CHUP,			  	//  �ҵ绰
	GSM_CLIP,			 	//  ������ʾ����
	GSM_VTS,				//  ��������1��DTMF��
	GSM_CLCC,				//  ��ȡ����	
	GSM_RING,				//	��绰	
	GSM_NO_CARRIER, 		//  ͨ������	
	GSM_BUSY,				//  �ҵ绰	
	GSM_NO_ANSWER,			//	���˽���
	GSM_CEND,				//  ����ͨ��
	
	// ����ָ��		
	GSM_CSCA,				//  �������ĺ���
	GSM_CMGF,				//	���Ÿ�ʽ
	GSM_CNMI,				//	������ʾ
	GSM_CMTI,				//  �յ�������ʾ	
	GSM_CMGS,				//  ���Ͷ�������
	GSM_CMGD,				//	ɾ������
	GSM_CMGR,				//	������
	
	//��������
	GSM_CGATT,				//  ��ѯ������GPRS ״̬
	GSM_CGACT,				//  ����/ȥ���� PDP
	GSM_IOMODE,				//  ��������ģʽ
	GSM_SICS,				//  ������������
	GSM_SISO,				//  �򿪷���������
	GSM_SICI,				//  ��ѯ����IP��ַ
	GSM_SISW,				//  ����GPRS����
	GSM_SISR,				//  ��ȡGPRS����
	GSM_SISC,				//  �ر���������
	GSM_SISS,				//  ͨ��ѡ��
	GSM_SIS,				//  �Զ��ϱ�internt ״̬
		
	GSM_ERROR,	
	GSM_CMS_ERROR,		   //
	
	// ��������
	GSM_TECNT,			//	��������	
	GSM_TEDISCNT,		//	�Ҷ�����	
	GSM_TESETUP,		//l	��ȡ/���� �豸����
	
	CMM_CMD_END
};
	

void 		  CMM_HWInit( void );
void 		  CMM_ReStart( void );
unsigned char CMM_GetCmdType(unsigned char *pMsg);

void		  CMM_Init(void);
unsigned char CMM_AT( void );
unsigned char CMM_QueryModuleSN( unsigned char *pStr );
unsigned char CMM_CheckSIMReady( void );
unsigned char CMM_ConfigCBand( void );
unsigned char CMM_QuerySignal( void );
unsigned char CMM_QueryNetworkStatus( void );
unsigned char CMM_QueryNetworkLac( int	*pLac, int	*pID );

unsigned char CMM_DialNumber( unsigned char *pStrNo );
unsigned char CMM_AcceptCall( void );
unsigned char CMM_RejectCall( void );
unsigned char GSM_ReadCaller( unsigned char *pCaller );
unsigned char CMM_QueryIMEI( unsigned  char	*pDest );

unsigned char GSM_SendPDUSMS(unsigned char *pMsg,  unsigned char nDataLen, unsigned char nTitleLen);
unsigned char GSM_ReadPDUSMS(unsigned char nCnt, unsigned char *pBuf );
unsigned char CMM_DeleteAllSMS(void);
unsigned char CMM_DeleteSMS( unsigned short nCnt );

unsigned char IP_ParseCloseState( unsigned char  *pMsg );
void 		  CMM_ParseMasterCmd(unsigned char *pMsg, unsigned char nCmdType);

unsigned char GSM_QuerySMC( unsigned char *pStr );
unsigned char GSM_ExecCUSD( unsigned char	*pDest ,unsigned char	*pSrc, unsigned short	nTimeout );

unsigned char CMM_QueryIPNetworkStatus(void);
unsigned char CMM_IPInit( unsigned char *pStrGateway, unsigned char *pUser, unsigned char *pPsd );
void		  CMM_ActivationPDP( void );
unsigned char GPRS_SetGateWay( unsigned char *pStr );
unsigned char GPRS_SetUserPsd( unsigned char *pUser, unsigned char *pPsd );
unsigned char GSM_ParseDNS( unsigned char *pStrHost, unsigned char *pStrIP );
unsigned char CMM_IPConnect(  unsigned char nMode, unsigned char *pStrIP,unsigned short nPort );
unsigned char IP_SendData(unsigned char nMode, unsigned char *pBuf, unsigned char nLen);
unsigned char CMM_IPClose( void );

void		  CMM_SendTECNT( void );
void 		  GSM_SendDISCNT( void );

#endif

#endif //APP_MG323_H_20111116