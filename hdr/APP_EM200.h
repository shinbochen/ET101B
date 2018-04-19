/////////////////////////////////////////////////////////////
//file name: APP_CDMA.c
//     Date: 2008-3-02    
//	 Author: wyb	 
// optimize: real
// op_date : 2009-3-8
/////////////////////////////////////////////////////////////
#include "..\hdr\main.h"

#ifndef  	  APP_CDMA_H_20090327
#define  	  APP_CDMA_H_20090327

#if( HW_EM200 )

extern  const	char	*s_strCMMCMD[];
extern  const	char	*s_strCMMCMD2[];

enum{
	 CDMA_OK,	 
	 CDMA_ATE0,			//�رջ���ʾ
	 CDMA_ATE1,			//��������
	 CDMA_GSN,		  //ESN��ѯ����
	 CDMA_CIMI,			//IMSI��ѯ����
	 
	 CDMA_CDV, 		  // ��绰	 
	 CDMA_CHV, 		  // ��绰	 
	 CDMA_QCCAV,    // �ӵ绰	 	 
	 CDMA_MICV,     // ����MIC
	
	 CDMA_CCNT,			// ����ָʾ 0: �������� 1:���ݺ��� 2:�������ͺ���		
	 CDMA_CEND,		  // ����ͨ��
	 CDMA_RING,			// ������ʾ	 
	 CDMA_CPIN,		  // ��⿨
	 CDMA_CSQ,			// ���ź�
	 // ���Ų������� 
	 CDMA_SMSSS,		  // ѡ�������ʾ���� '1,0,0,0'	 
	 CDMA_CMGF,			  // ���ö��Ÿ�ʽ	1���ı� 
	 CDMA_CNMI,		    // ����ͨ������
	 CDMA_CMGR,			// ��ȡSMS����
	 CDMA_CPMS,		    // ѡ����Ŵ洢��"ME"
	 CDMA_CMGSS,		// ���ŷ��ͳɹ���־
	 CDMA_CMGSF,		// ���Ͷ���ʧ��
	 CDMA_CMGS,			// ���Ͷ���
	 CDMA_CMTI,		    // �յ��µĶ���
	 CDMA_CMGD,		    // ɾ������
	 CDMA_HSMSRD,			// ��ȡһ������
	 
	 // ����ָ��
	 CDMA_SETEC,		  // ������������
	 // GPRS�������
	 CDMA_ACCNT,		  // �й����ŵ�CDMA �û���card,����card
	 CDMA_PPPOPEN,	  // ����PPP����
	 CDMA_PPPCLOSE,		// �ر�PPP����
	 CDMA_CORG,			  // ����������ʾ
	 CDMA_PPPSTATUS,  // ��������״̬����
	 CDMA_DTUIP,			// ��ȡģ��IP��ַ	
	 // TCP����
	 CDMA_TCPOPEN,		// DTU ����TCP����
	 CDMA_TCPCNNTIMER,// DTU ����TCP����ʱ��
	 CDMA_TCPCNNABORT,// DTU ���ý���,TCP���ӵ��ʱ��
	 CDMA_TCPSEND,    // DTU ����TCP����
	 CDMA_TCPRECV,		// DTU ����TCP����
	 CDMA_TCPSTATUS,  // DTU TCPͨ������״̬�ϱ� 0: TCP���ӳɹ� 1: TCP���ӹر�
	 CDMA_TCPCLOSE,   // DTU �ر�TCP����	
	 // UDP����
	 CDMA_UDPOPEN,		// DTU ����UDP����
	 CDMA_UDPSEND,    // DTU ����UDP����
	 CDMA_UDPRECV,	  // DTU ����UDP����	 
	 CDMA_UDPCLOSE,   // DTU �ر�UDP����
	 CDMA_UDPSTATUS,  // DTU UDPͨ������״̬�ϱ� 0: UDP���ӳɹ� 1: UDP���ӹر� 		 
	 //  ����ָ��
	 CDMA_CME,		  // ������ʾ
	 CDMA_CMS,
	 CDMA_CID,		  // ������ʾ����
	 CDMA_IFC,		  // �ر�Ӳ����
	 CDMA_CLIP,		  // ������ʾ����
	 CDMA_HCTCM,	  // ���ݿ�������	
	 //	��������
	 GSM_TECNT,
	 //	�Ҷ�����
	 GSM_TEDISCNT,
	 //l ��ȡ/���� �豸����
	 GSM_TESETUP,	 
	 CMM_CMD_END,
	 
	 CMM_CMD_UNKNOW
};

//////////////////////////////////////////////////////////////////
// 
void 					CMM_HWInit( void );
void 					CMM_ReStart( void );
void 					CMM_Init(void);
unsigned char CMM_IPInit( unsigned char *pStrGateway, unsigned char *pUser, unsigned char *pPsd );
/////////////////////////////////////////////////////////////////
// ����ATָ���ʽ
// ������������, ��ȡ���������
unsigned char CMM_GetCmdType(unsigned char *pMsg);

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
//  ��ѯDTU��������״̬�ϱ�
unsigned char	CMM_DTUQueryPPPStatus( void );
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
												   unsigned char nDataLen );	
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
// %CMGR:13760374664,2009,04,06,10,04,1,4,1,0,0
unsigned char CMM_ParseSMSFormatA( unsigned char *pCaller, 
								  unsigned char *pMsg,  
								  unsigned char *pCodeType,
								  unsigned char *pDataLen );													 
//  ���Ͷ��Ÿ�ʽ
//  $HSMSRD: 13480924411, <2009/03/28> <23:09>, 0, 1, 14, 1, 0
//           13480924411, <2009/03/28> <23:09>, 0, 1, 14, 1, 0
unsigned char CMM_ParseSMSFormatB( unsigned char *pCaller, 
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
unsigned char	IP_RecvTCPData(unsigned char  *pMsg,
							unsigned char   *pDest,
							unsigned char	  *pSocketNum );
													
// ����������ʾ
// CID: 13480924411
void 		 CMM_ReadCallerID( unsigned char *pMsg, unsigned char *pCaller );
// ����������ʾ
//+CLIP:13480924411,0
void 		 CMM_ReadCallerCLIP( unsigned char *pMsg, unsigned char *pCaller );
//  $HCEND: 7, 1, 00:02:59 
// Return :
//       0:  ��������END
//       1:  IP����END
//       2:  ����END
unsigned char CMM_ParseHCEnd( unsigned char  *pMsg );													  	
													   
/////////////////////////////////////////////////////////////////
//  ��ѯ����
unsigned char	GSM_ExecCUSD( unsigned char	*pDest, unsigned char	*pSrc, unsigned short	nTimeout );
//////////////////////////////////////////////////////////////
//  ��ѯIMEI���
unsigned char	CMM_QueryIMEI( unsigned  char	*pDest );
//	�������ӳɹ�����
void	CMM_SendTECNT( void );
//	���ͶϿ�����,����GSMģ���Դ
void 	GSM_SendDISCNT( void );

//   TCP/IP���ӷ�ʽ
unsigned char	CMM_TCPConnect( unsigned char nCmdType, unsigned char	*pCmdData, unsigned char	*pReturnMsg );
/////////////////////////////////////////////////////////////////
// ��������������															
unsigned char GSM_ParseDNS( unsigned char *pStrHost, unsigned char *pStrIP );
//	PDP�����ļ���
void	CMM_ActivationPDP( void );
unsigned char CMM_QueryNetworkStatus( void );
unsigned char	CMM_ConfigCBand( void );
unsigned char GSM_ReadCaller( unsigned char *pCaller );
#endif 			// HW_EM200
													   							    											 
#endif //APP_CDMA_H_20090327

