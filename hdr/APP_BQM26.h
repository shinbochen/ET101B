/////////////////////////////////////////////////////////////
//file name: APP_GSMBENG.H
//     Date: 2008-3-02    
//	 Author: wyb	 
// optimize: real
// op_date : 2009-3-30
/////////////////////////////////////////////////////////////
#include "..\hdr\main.h"
#ifndef  	  APP_GSMBENG_H_20090330
#define  	  APP_GSMBENG_H_20090330


#if( HW_BQM26 )

extern  const	char	*s_strCMMCMD[];

enum{
	  GSM_OK = 0,
	  GSM_ATE0,			//�رջ���ʾ
	  GSM_ATE1,			//��������
	  GSM_CMEE,			//
	  GSM_CFUN,			//set phone functionality
	  GSM_COPS,			//ģʽѡ��
	  GSM_PWR,			//�ػ�	 
	  GSM_CSQ,			//��GSM�ź�		
	  GSM_CREG,         //��ѯGSM����
	  GSM_CGREG,		//��ѯGPRS����
	  GSM_CPIN,			//�����	  
	  GSM_CGSN,			//�������к�
	  
	  //�绰ָ��
	  GSM_CLCC,			//��ѯ�������	  
	  GSM_ATD,		  	//��绰		
	  GSM_ATA,			//�ӵ绰		
	  GSM_ATH,			//�ҵ绰		
	  GSM_NO_CARRIER,	//ͨ������	
	  GSM_BUSY,			//�ҵ绰	
	  GSM_NO_ANSWER,	//���˽���
	  GSM_CLIP,			//����������ʾ
	  GSM_SG,				//��˷�����
	  GSM_SAMP,			//��Ͳ����
	  GMS_CUSD,			//�ǽṹ������ҵ�� 
		
	  //����ָ��
	  GSM_CSMP,			//���͵ı��뷽ʽ	  
	  GSM_CSCA,			//��ȡ���ĺ���
	  GSM_CMGF,			//���Ÿ�ʽ
	  GSM_CSDH,			//��ʾ�ı���ʽ
	  GSM_CNMI,			//���ö�����ʾ	  
	  GSM_CMTI,			//�յ�������ʾ	
	  GSM_CMGR,			//��ȡ����
	  GSM_CMGS,			//���Ͷ���
	  GSM_CMGD,			//ɾ������
	  
	  //����ע��	  
	  GSM_CGATT,		//ע��GPRS����
	  GSM_NWACT,		//������ر�����
	  GSM_NWSO,		 	//����ͨ�� 
	  GSM_NWCN,		    //������ر����ӷ�����
	  GSM_NWDF,			//���ý������ݵĸ�ʽ
	  GSM_NWTX,			//�������ݵ�������
	  GSM_NWRX,		    //�յ�����
	  GSM_NWDR,		    //�ӽ���BUF������
	  GSM_ERROR,
	  GSM_CMS_ERROR,    //������Ϣ����
	  GSM_CME_ERROR,	//����ע����󷵻�
	  GSM_SLEEP,		//�ر���Ϣ״̬
	  GSM_CGMR,
	  //��������
	  GSM_TECNT,
	  //�Ҷ�����
	  GSM_TEDISCNT,
	  //l��ȡ/���� �豸����
	  GSM_TESETUP,
	  CMM_CMD_END
};

///////////////////////////////////////////////////////////
//  GSMӲ����ʼ��
void 					CMM_HWInit( void );
//	GSM900ͨ��ģ��Ӳ������	
void 					CMM_ReStart( void );
//
void  				CMM_Reset(void );
//  ��ʼ��GSMģ��
void 					CMM_Init(void);
//	��ʼ����������	
unsigned char CMM_IPInit( unsigned char *pStrGateway, unsigned char *pUser, unsigned char *pPsd );
//  ������������, ��ȡ���������
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
unsigned char CMM_QueryNetworkStatus( void );
//	��ѯ��ַС��ID  +CREG: 2,1,"2483","0E9D"
unsigned char	CMM_QueryNetworkLac( int	*pLac, int	*pID );
// ��绰
unsigned char CMM_DialNumber( unsigned char *pStrNo );
// �ӵ绰
unsigned char CMM_AcceptCall( void );
//  �ҵ绰
unsigned char CMM_RejectCall( void );
//  ��ѯIMEI���
unsigned char	CMM_QueryIMEI( unsigned  char	*pDest );
/////////////////////////////////////////////////////////////
// ����:			0�� free
//						1:  on use
// +CLIP: "13480924411",129,,,,0
//	    : "13480924411",129,,,,0
unsigned char GSM_ParseCaller( unsigned char *pCaller, 
															 unsigned char *pSrc );
//
unsigned char GSM_OffModule( void );
// ɾ������
unsigned char CMM_DeleteSMS( unsigned short nCnt );
// ɾ��ȫ������  
unsigned char CMM_DeleteAllSMS(void);
///////////////////////////////////////////////////////////////
// ��Ч����1
// ��Ч����0
unsigned char GSM_CheckSIMReady( void );
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
void 	CMM_ParseMasterCmd(unsigned char *pMsg, unsigned char nCmdType);
/////////////////////////////////////////////////////////////////
//  �ر�GPRS����
unsigned char CMM_IPClose(void);
//////////////////////////////////////////////////////////////
// ��������
// ����GSM_SUCCEED��ʾ�ɹ�������ʧ��
unsigned char CMM_IPConnect(  unsigned char nMode, 
															unsigned char *pStrIP,
															unsigned short nPort );
//////////////////////////////////////////////////////////////															
//	��ȡGPRS����	
unsigned short P_ReadGPRSData( unsigned char *pDest, 
							   							 unsigned short nTialLen );
//////////////////////////////////////////////////////////////////
// ����GPRS���� 0: ʧ��  1:  �ɹ�
// send:
// AT$NWTX=1,<string_data>
unsigned char IP_SendData(unsigned char nMode, 
													unsigned char *pBuf, 
													unsigned char nLen);							   
													
///////////////////////////////////////////////////
// ��ȡ��������
// send: at+CMGR=x
// recv1:
// +CMGR:stat,[alpha],length<CR><LF>
// pdu<CR><LF>
// <CR><LF>
// OK<CR><LF>
// recv2:
// ERROR
// ���س��ȡ�ʧ�ܷ���0
unsigned char	GSM_ReadPDUSMS(unsigned char nCnt, unsigned char *pBuf );													
///////////////////////////////////////////////////////////////
// ����PDU GSM����
// send:
// AT+CMGS=12<CR><LF>
// recv:
// >
// send:
// pdu ^Z<CR><LF>
// recv1:
// +CMGS:xx<CR><LF> 
// OK 
// recv2:
// ERROR
unsigned char GSM_SendPDUSMS(unsigned char *pMsg,  unsigned char nDataLen, unsigned char nTitleLen);

//	�������ӳɹ�����
void	CMM_SendTECNT( void );
//	���ͶϿ�����,����GSMģ���Դ
void 	GSM_SendDISCNT( void );
/////////////////////////////////////////////////////////////////
//	PDP�����ļ���
void	CMM_ActivationPDP( void );
#endif // HW_BQM26
															 															
#endif