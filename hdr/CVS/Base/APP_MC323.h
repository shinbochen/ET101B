/////////////////////////////////////////////////////////////
//file name: MC323.c
//     Date: 2011-06-08  
//	 Author: wyb	 
// optimize: 
// op_date : 
/////////////////////////////////////////////////////////////
#include "..\hdr\main.h"

#ifndef  	  APP_CDMA_H_20110608
#define  	  APP_CDMA_H_20110608

#ifdef HW_MC323

extern  const	char	*s_strCMMCMD[];

enum{
	 CDMA_OK,	 
	 CDMA_ATE0,			//�رջ���ʾ
	 CDMA_ATE1,			//��������
	 CDMA_IFC,      //����������������
	 CDMA_CPIN,			//��⿨
	 CDMA_CMGF,			//
	 CDMA_GSN,			//
	 
	 //�������ӿ�����
	 CDMA_CREG,		  //����ע��
	 CDMA_CSQ,			//RSSI��ѯ
	 CDMA_RSSIREP,	//�ϱ�ָʾ���� 
	 
	 //���п�������	 
	 CDMA_CDV,			//��������
	 CDMA_CHV,			//�Ҷ���������
	 CDMA_A,				//�������� 
	 CDMA_RING,			//����ָʾ
	 CDMA_ORIG,			//���з���ָʾ
	 CDMA_CONN,		  //���н�ָͨʾ
	 CDMA_CEND,		  //ͨ������ָʾ
	 CDMA_CLIP,			//������ʾ�������ü�ָʾ
	 CDMA_CLVL,			//������������
	 CDMA_CMIC, 		//��˷���������	 
	 CDMA_CLCC,			//����״̬��ѯ
	 CDMA_DTMF,		  //���β�������
	 
	 //����Ϣҵ��ӿ�����
	 CDMA_CPMS,			//���ö��Ŵ洢λ��
	 CDMA_CNMI,		  //֪��֪ͨ��ʽ
	 CDMA_CMTI,			//���ŵ�����ʾ
	 CDMA_HSMSSS,		//���ö��Ų���
	 CDMA_HCMGS,		//���Ͷ���
	 CDMA_HCMGSS,	  //���ŷ��ͳɹ��ϱ�ָʾ
	 CDMA_HCMGSF,	  //���ŷ���ʧ���ϱ�ָʾ
	 CDMA_HCMGR,		//��ȡһ������
	 CDMA_CMGD,			//ɾ������
	 
	 //Internet����ӿ�����
	 CDMA_PPPCFG,		//����PPP�û���������
	 CDMA_IPINIT,		//TCP/UDP���ӳ�ʼ��
	 CDMA_IPOPEN,		//����TCP/UDP����
	 CDMA_IPSEND,   	//TCP/UDP���ݷ���
	 CDMA_IPSENDEX,		//��չTCP/UDP���ݷ���
	 CDMA_IPDATMODE,	//TCP/UDP���ݵ���ָʾ����
	 CDMA_IPDATA,	    //TCP/UDP���ݵ���ָʾ
	 CMDA_IPCLOSE,		//�ر�TCP/UDP����
	 
	 //
	 CDMA_CME,			// ������ʾ
	 CDMA_CMS,			//
	 CDMA_ERROR,		// ����	 
	  
	 CDMA_TECNT,		//	��������	 
	 CDMA_TEDISCNT,	//	�Ҷ�����	 
	 CDMA_TESETUP,	//l ��ȡ/���� �豸���� 
	 
	 CMM_CMD_END, 
	 CMM_CMD_UNKNOW
};

// ���ű��뷽ʽ
enum{
	CODE_ASCII = 0x00,
	CODE_OCTET,
	CODE_UNICODE
};

//////////////////////////////////////////////////////////////////
// 
void 					CMM_HWInit( void );
void 					CMM_ReStart( void );
void 					CMM_Init(void);

unsigned char		CMM_IPInit( unsigned char *pStrGateway, unsigned char *pUser, unsigned char *pPsd );
/////////////////////////////////////////////////////////////////
// ����ATָ���ʽ
// ������������, ��ȡ���������
unsigned char CMM_GetCmdType(unsigned char *pMsg, unsigned short nDataLen);
/////////////////////////////////////////////////////////////////
// ����ATָ���ʽ
// ������������, ��ȡ���������
// ��չָ��,��������,���ص�״̬�� HCMGS, HCMGSS,HCMGSF
unsigned char CMM_GetCmdTypeEx(unsigned char *pMsg, unsigned short nDataLen );

// �ܷ���1
unsigned char CMM_QueryIPNetworkStatus( void );
/////////////////////////////////////////////////////////////////
// ��⿨�Ƿ����
unsigned char CMM_CheckSIMReady( void );
// ���ն���� 
unsigned char CMM_QueryModuleSN( unsigned char *pStr );
// ɾ������
unsigned char CMM_DeleteSMS( unsigned short nCnt );
// ɾ������
void          CMM_DeleteAllSMS( void );
// ��绰
unsigned char CMM_DialNumber( unsigned char *pStrNo );
//////////////////////////////////////////////////////////////
// �ӵ绰
unsigned char CMM_AcceptCall( void );
//	��ѯ��ַС��ID  +CREG: 2,1,"2483","0E9D"
unsigned char	CMM_QueryNetworkLac( int	*pLac, int	*pID );
//////////////////////////////////////////////////////////////
//
unsigned char CMM_RejectCall( void );
// �����źż���
unsigned char CMM_QuerySignal( void );
// ����PPP�û���������
unsigned char	CMM_SetPPPUserName( unsigned char	*pUser, unsigned char  *pPsd );
//  DTU���������ϱ�
unsigned char	CMM_DTUPPPOpen( void );
//  DTU�ر������ϱ�
unsigned char CMM_DTUPPPClose( void );
/////////////////////////////////////////////////////////////////
unsigned char CMM_AT( void );
////////////////////////////////////////////////////////////////////////////
//   TCP/IP����
// ����GSM_SUCCEED��ʾ�ɹ�������ʧ��
// $HTCPOPEN: 1, 0
unsigned char CMM_IPConnect(  unsigned char nMode, 
															unsigned char *pStrIP,
															unsigned short nPort );
void		  CMM_IPClose( void  );			
unsigned char	CMM_IPClose2( unsigned char	nMode );		
// ASCII bit
unsigned char CMM_SendASCIIData( unsigned char *pCaller, 
																 unsigned char *pMsg, 
																 unsigned char nLen );
// OCTET
unsigned char CMM_SendOctetData( unsigned char *pCaller, 
																 unsigned char *pMsg, 
																 unsigned char nLen );	
// unicode
unsigned char CMM_SendUnicodeData( unsigned char *pCaller, 
																 	 unsigned char *pMsg, 
																 	 unsigned char nLen );
// ���� GSM����																 	 
unsigned char CMM_SendSMS( unsigned char *pCaller,
							unsigned char *pMsg,  
							unsigned char nDataLen,
							unsigned char nCodeType );	
// ��ȡ��������
// send: at+CMGR=x
// recv1:
// pCodeType: 0: ASCII
//					  1: OCT
//            2: UNICODE
unsigned char CMM_ReadSMS( unsigned char nCnt, 
													 unsigned char *pCaller, 
													 unsigned char *pMsg,
													 unsigned char *pCodeType );												   
//  ���Ͷ��Ÿ�ʽ
//^HCMGR:13480924411,2011,06,08,16,24,57,0,1,9,0,0,0,1
unsigned char CMM_ParseSMSFormat( unsigned char *pCaller, 
																	  unsigned char *pMsg,  
																	  unsigned char *pCodeType,
																	  unsigned char *pDataLen );	
																	
// �������洢������Ϣ
void 					CMM_ParseMasterCmd(unsigned char *pMsg, unsigned char nCmdType);				
// ����GPRS���� 0: ʧ��  1:  �ɹ�
// send:
// AT+IPSEND=ͨ��,����
// 
unsigned char IP_SendData( 	unsigned char nMode,
							unsigned char *pBuf, 
							unsigned char nLen);				

//  ���յ�GPRS����
// $HTCPRECV: 3, 4
//
// 0 5H    // 0x30 0x20 0x35 'H'
unsigned char	CMM_IP_RecvData(unsigned char  *pMsg,
																unsigned char   *pDest,
																unsigned char	  *pSocketNum );
													
// ����������ʾ
// CID: 13480924411
void 		 			CMM_ReadCallerID( unsigned char *pMsg, unsigned char *pCaller );
// ����������ʾ
//+CLIP:13480924411,0
void 		 			CMM_ReadCallerCLIP( unsigned char *pMsg, unsigned char *pCaller );
/////////////////////////////////////////////////////////////////
//  ��ѯ����
unsigned char		GSM_ExecCUSD( unsigned char	*pDest, unsigned char	*pSrc, unsigned short	nTimeout );
//////////////////////////////////////////////////////////////
//  ��ѯIMEI���
unsigned char		CMM_QueryIMEI( unsigned  char	*pDest );
//	�������ӳɹ�����
void					  CMM_SendTECNT( void );
//	���ͶϿ�����,����GSMģ���Դ
void 					  CMM_SendDISCNT( void );

//   TCP/IP���ӷ�ʽ
unsigned char		CMM_TCPConnect( unsigned char nCmdType, unsigned char	*pCmdData, unsigned char	*pReturnMsg );
/////////////////////////////////////////////////////////////////
// ��������������															
unsigned char		GSM_ParseDNS( unsigned char *pStrHost, unsigned char *pStrIP );
//	PDP�����ļ���
void					  CMM_ActivationPDP( void );
unsigned char 	CMM_QueryNetworkStatus( void );
unsigned char		CMM_ConfigCBand( void );
unsigned char 	GSM_ReadCaller( unsigned char *pCaller );
#endif 			// HW_MC323
													   							    											 
#endif //APP_CDMA_H_20110608

