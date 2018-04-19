/////////////////////////////////////////////////////////////
//file name: APP_SIM300.H
//     Date: 2009-6-11    
//	 Author: wyb	 
// optimize: real
// op_date : 2009-6-11 
//"PGSM_MODE",                      900
//"DCS_MODE",                       1800
//"PCS_MODE",                       1900
//"EGSM_DCS_MODE",                  900/1800
//"GSM850_PCS_MODE",                850/1900
//"GSM850__PCS_MODE")       		850/900/1800/1900
/////////////////////////////////////////////////////////////

#ifndef  APP_SIMCOM_H
#define  APP_SIMCOM_H

#if( HW_SIMCOM )

#define			GSM_GPRSWORK_CHECK				30*200

extern  const	char	*s_strCMMCMD[];

enum{
	  GSM_OK = 0,
	  GSM_ATE0,			//�رջ���ʾ
	  GSM_ATE1,			//��������
	  GSM_CMEE,			
	  GSM_CFUN,			//set phone functionality
	  GSM_CSQ,			//��GSM�ź�		
	  GSM_CREG,     //��ѯGSM����
	  GSM_CGREG,		//��ѯGPRS����
	  GSM_CPIN,			//�����	  
	  GSM_IPR,			//���ò�����
	  GSM_CGSN,			//�������к� 
	  GSM_CMUT,			//�������
	  GSM_CLVL,			//��������
	  
	  GSM_SIDET,	  //���������ȴ�С
	  GSM_CMIC,		  //�ı���˷������
	  GSM_CHFA,		  //0/1�����������л�������Ƶͨ��
	  GSM_IFC,		   // �ر�Ӳ����	
	  
	  //�绰ָ��
	  GSM_CLCC,			//��ѯ�������	
	  GSM_RING,			//������ʾ 
	  GSM_ATD,		  //��绰		
	  GSM_ATA,			//�ӵ绰		
	  GSM_ATH,			//�ҵ绰		
	  GSM_NO_CARRIER,	//ͨ������	
	  GSM_BUSY,			//�ҵ绰	
	  GSM_NO_ANSWER,	//���˽���	 
	  GSM_SHUT_OK,		//�Ͽ�PDP�ɹ�
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
	  GSM_CIPDPDP,		//PDP����	
	  GSM_CIPSHUT,		//�ر�PDP����
	  GSM_CGATT,		//ע��GPRS����
	  GSM_CGDCONT,		//PDP����
	  GSM_CGACT,		//�����ƶ�����
	  GSM_CSTT,			//����APN��USER NAME, PASD WORD
	  GSM_CIICR,		//�����ƶ�����
	  GSM_CIFSR,		//��ȡ����IP
	  GSM_CIPSTART,		//���ӷ�����
	  GSM_CIPSEND,		//����UDP��TCP����
	  GSM_CIPCLOSE,		//�Ͽ�TCP/UDP����
	  GSM_CIIPSHUT,		//�ر�GPRS PDP����
	  GSM_CIPHEAD,		//����GPRS����
	  GSM_IPD,		  	//����IP����
	  GSM_SLEEP,		//�ر���Ϣ״̬
	  GSM_CONNECT_OK,
	  GSM_CONNECT_FAIL,	  
	  GSM_SEND_OK,			
	  GSM_SEND_FAIL,	  
	  GSM_CLOSE_OK,
	  GSM_CLOSED,
	  GSM_ERROR,
	  GSM_CME_ERROR,
	  GSM_CMS_ERROR,
	  // ��������״̬
	  GSM_STATE,
	  GSM_CBAND,	    //��Ƶ	cband
	  
	//  GSM_CDNSCFG,
	//  GSM_CDNSORIP,
	  //��������
	  GSM_TECNT,
	  //�Ҷ�����
	  GSM_TEDISCNT,
	  //l��ȡ/���� �豸����
	  GSM_TESETUP,
	  //��汾��
	  GSM_GMR,
	  CMM_CMD_END
};

///////////////////////////////////////////////////////////
// GSMӲ����ʼ��
void 					CMM_HWInit( void );
//	GSM900ͨ��ģ��Ӳ������	
void 					CMM_ReStart( void );
//��ʼ��GSMģ��
void 					CMM_Init(void);
//  ģ����Ƶ
unsigned char			CMM_ConfigCBand( void );
//��ʼ����������
unsigned char	CMM_IPInit(unsigned char *pStrGateway, unsigned char *pUser, unsigned char *pPsd);
void	CMM_ActivationPDP( void );
unsigned char CMM_AT( void );
unsigned char CMM_IPClose( void );
unsigned char CMM_CIPSHUT( void );
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
//	��ѯ��ַС��ID  +CREG: 2,1,"2483","0E9D"
unsigned char	CMM_QueryNetworkLac( int	*pLac, int	*pID );
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
//////////////////////////////////////////////////////////////
//
unsigned char GSM_OffModule( void );
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
///////////////////////////////////////////////////////////////////
//
unsigned char GPRS_SetUserPsd( unsigned char *pGateWay, unsigned char *pUser, unsigned char *pPsd );

//////////////////////////////////////////////////////////////
// ��������
// ����GSM_SUCCESS��ʾ�ɹ�������ʧ��
unsigned char CMM_IPConnect( 	unsigned char nMode, 
															unsigned char *pStrIP,															
															unsigned short nPort );
/////////////////////////////////////////////////////////////////
// ��������������															
//unsigned char GSM_ParseDNS( unsigned char *pStrHost, unsigned char *pStrIP );															
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
													
#endif //HW_SIMCOM


#endif //APP_SIMCOM_H
