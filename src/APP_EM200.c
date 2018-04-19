/////////////////////////////////////////////////////////////
//file name: APP_CDMA.c
//     Date: 2008-3-02    
//	 Author: wyb	 
// optimize: real
// op_date : 2009-3-8
/////////////////////////////////////////////////////////////
// include file declare
#include  <stdio.h>
#include  <stdlib.h>
#include "..\hdr\includes.h"
#include "..\hdr\queue.h"
#include "..\hdr\main.h"
#include "..\hdr\APP_GSM.h"
#include "..\hdr\APP_GPS.h"
#include "..\hdr\HW_UART.h"
#include "..\hdr\HW_flash.h"
#include "..\hdr\APP_LoopDetect.h"
#include "..\hdr\APP_Terminal.h"
#include "..\hdr\HW_GPIO.h"
#include "..\hdr\APP_EM200.h"
#include "..\hdr\ProtocolA.h"
#include "..\hdr\ProtocolB.h"
#include "..\hdr\ProtocolC.h"

#if( HW_EM200 )
//=====================================================
// CM320
const char	*s_strCMMCMD2[]={
		"OK",				//OK
		"E0",				//�رջ���ʾ
		"E1",				//��������
		"+GSN",		  //ESN��ѯ����
	 	"+CIMI",		//IMSI��ѯ����
		
		"+CDV",			//��绰
		"+CHV",			//�ҵ绰
		"$QCCAV",		//�����绰
		"%MICV",    //����MIC
		
		"$HCCNT",	    //��������ָʾ
		"$HCEND",		//����ͨ��	
		"$HRIG",   		//������ʾ
		"+CPIN",  		//��⿨
		"+CSQ",     	//���ź� 
		// ���Ų�������
		"%SMSSS",		//ѡ�������ʾ����  %SMSSS
		"+CMGF",		//���ö��Ÿ�ʽ	1���ı� 
		"+CNMI",		//����ͨ������ 1,1,0,1,0
		"%CMGR",		//��ȡ����
		"+CPMS",		//ѡ����Ŵ洢��
		"%CMGSS",		//���ŷ��ͳɹ���־CMGSS
		"%CMGSF",		// ���Ͷ���ʧ��
		"%CMGS",	    //���Ͷ���
		"+CMTI",		//�յ��¶��� 
		"+CMGD",		//ɾ������
		"$HSMSRD",	    //��ȡһ������		
		// ����ָ��
		"$HSETEC",	  	// ������������
		// GPRS�������
		"$HACCNT",		// �й����ŵ�CDMA �û���card,����card
		"$HPPPOPEN",	// ����PPP����
		"$HPPPCLOSE",	// DTU �ر�PPP����
		"$HCORG",		  // �������ָʾ 
		"$HPPPSTATUS",// ��������״̬����
		"$HDTUIP",		// ��ȡģ��IP��ַ
		// TCP����
		"$HTCPOPEN",    // DTU����TCP����	  
		"$HTCPCNNTIMER",// DTU����TCP����ʱ��
		"%TCPCNNABORT", //DTU���ý���,TCP���ӵ��ʱ��
		"$HTCPSEND",	  // DTU����TCP����
		"$HTCPRECV",    // DTU����TCP����
		"$HTCPSTATUS",  // DTU TCPͨ������״̬�ϱ� 0: TCP���ӳɹ� 1:TCP���ӹر�
		"$HTCPCLOSE",   // DTU �ر�TCP����	
		//  UDP����
		"$HUDPOPEN",    // DTU����UDP����
		"$HUDPSEND",	  // DTU����UDP����
		"$HUDPRECV",   // DTU����UDP����		
		"$HUDPCLOSE",   // DTU �ر�UDP����
		"$HUDPSTATUS",  // DTU UDPͨ������״̬�ϱ� 0: UDP���ӳɹ� 1:UDP���ӹر� 
		//  ����ָ��
		"+CME",			// ������ʾ
		"+CMS",			//
		"CID",			// ������ʾ����
		"+IFC",			// �ر�Ӳ����
		"+CLIP", 		// ������ʾ����
		"$HCTCM",		// ���ݿ�������
		"+TECNT", 		//��������
		"+TEDISCNT",	//�Ҷ�����
		"+TESETUP",     //l	��ȡ/���� �豸����
		
};
//=====================================================
//  ME200
const char	*s_strCMMCMD[]={
		"OK",			//OK
		"E0",			//�رջ���ʾ
		"E1",			//��������
		"+GSN",		  	//ESN��ѯ����
		"+CIMI",		//IMSI��ѯ����
		
		"+CDV",			//��绰
		"+CHV",			//�ҵ绰
		"$QCCAV",		//�����绰
		"%MICV",    	//����MIC
		
		"%CCNT",		//��������ָʾ
		"%CEND",		//����ͨ��
		"RING",   		//������ʾ	    
		"+CPIN",  		//��⿨
		"+CSQ",     	//���ź� 
		// ���Ų�������
		"%SMSSS",	  //ѡ�������ʾ����
		"+CMGF",		//���ö��Ÿ�ʽ	1���ı� 
		"+CNMI",		//����ͨ������ 1,1,0,1,0
		"%CMGR",		//��ȡ����
		"+CPMS",		//ѡ����Ŵ洢��
		"%CMGSS",		//���ŷ��ͳɹ���־
		"%CMGSF",		// ���Ͷ���ʧ��
		"%CMGS",	 	 //���Ͷ���
		"+CMTI",		//�յ��¶��� 
		"+CMGD",		//ɾ������
		"$HSMSRD",		//��ȡһ������
		// ����ָ��
		"%SETEC",		// ������������
		// GPRS�������
		"%ACCNT",		// �й����ŵ�CDMA �û���card,����card
		"%PPPOPEN",		// ����PPP����
		"%PPPCLOSE",	// DTU �ر�PPP����
		"%CORG",		// �������ָʾ 
		"%PPPSTATUS", 	// ��������״̬����
		"%DTUIP",		// ��ȡģ��IP��ַ
		// TCP����
		"%TCPOPEN",   // DTU����TCP����	  
		"%TCPCNNTIMER",//DTU����TCP����ʱ��
		"%TCPCNNABORT",//DTU���ý���,TCP���ӵ��ʱ��
		"%TCPSEND",	  // DTU����TCP����
		"%TCPRECV",   // DTU����TCP����
		"%TCPSTATUS", // DTU TCPͨ������״̬�ϱ� 0: TCP���ӳɹ� 1:TCP���ӹر�
		"%TCPCLOSE",  // DTU �ر�TCP����
		// UDP����
		"%UDPOPEN",     // DTU����UDP����
		"%UDPSEND",	    // DTU����UDP����
		"%UDPRECV",     // DTU����UDP����	
		"%UDPCLOSE",    // DTU �ر�UDP����
		"%UDPSTATUS",   // DTU UDPͨ������״̬�ϱ� 0: UDP���ӳɹ� 1:UDP���ӹر�
		//  ����ָ��
		"+CME",			// ������ʾ
		"+CMS",			//
		"CID",			// ������ʾ����
		"+IFC",			// �ر�Ӳ����	
		"+CLIP", 		// ������ʾ����
		"$HCTCM",		// ���ݿ�������EM310
		"+TECNT", 		//��������
		"+TEDISCNT",	//�Ҷ�����
		"+TESETUP",     //l	��ȡ/���� �豸����
};

extern  OS_EVENT 			*pGSMReceiveSem;

//////////////////////////////////////////////////////////////////
// CDMAӲ����ʼ��
void CMM_HWInit( void ){
	
  GSMUARTInit(9600);		// GSM ����  
  // GSMUARTInit(115200);		// GSM ����  
 // GSMģ�鸴λ����û��,������,��ʱ����
 	
//  GPIO_Enable(HW_PORT_GSMRESET);
//  GPIO_SetDirection(HW_PORT_GSMRESET, HW_PORT_OUTPUT); 
//  HW_GSM_RESET_HIGH();

  // GSMģ�鿪������	
  GPIO_Enable(HW_PORT_WARMPOWER);			
  GPIO_SetDirection(HW_PORT_WARMPOWER, HW_PORT_OUTPUT);		
  HW_GSM_WARM_POWERON( );
  
  #if( HW_COLDRESTART_GSM )
  GPIO_Enable(HW_PORT_COLDPOWER);			
  GPIO_SetDirection(HW_PORT_COLDPOWER, HW_PORT_OUTPUT);			
  HW_GSM_COLDPOWEROFF();
  #endif	
  
}
/////////////////////////////////////////////////////////
// CDMAͨ��ģ��Ӳ������	
void CMM_ReStart( void ){
		
//	HW_GSM_RESET_LOW();
//	OSTimeDlyHMSM(0,0,0,100);
//	HW_GSM_RESET_HIGH();
//	OSTimeDlyHMSM(0,0,0,50);
	//2011-03-26 ����������������
	#if( HW_COLDRESTART_GSM )
		OSTimeDlyHMSM(0,0,0,500);
		HW_GSM_COLDPOWEROFF();
		OSTimeDlyHMSM(0,0,5,0);	
		HW_GSM_COLDPOWERON();			
		//  �ߵ�
		HW_GSM_WARM_POWERON( );				//H
		OSTimeDlyHMSM(0,0,0,500 );		
		// power on
		HW_GSM_WARM_POWEROFF( );	
		OSTimeDlyHMSM(0,0,0,200);	
		HW_GSM_WARM_POWERON( );
		OSTimeDlyHMSM(0,0,0,100);
	#else
		// power off
		HW_GSM_WARM_POWEROFF();
	    OSTimeDlyHMSM(0,0,3,0);
	    HW_GSM_WARM_POWERON( );
	    OSTimeDlyHMSM(0,0,5,0);	

		// power on
		HW_GSM_WARM_POWEROFF( );	
		OSTimeDlyHMSM(0,0,0,200);	
		HW_GSM_WARM_POWERON( );
		OSTimeDlyHMSM(0,0,0,100);
	#endif
}
///////////////////////////////////////////////////////////////////
//��ʼ��GSMģ��
void CMM_Init(void){  
	
	int 	nCnt = 0;
	
	// �ر�Ӳ����
	while( nCnt++ <= 50 ){
		if (CMM_ExecATCmd( CDMA_IFC, (unsigned char*)"=0,0", GSM_SENDSPACE_CHECK, CDMA_OK, CDMA_CME, NULL )  == GSM_SUCCEED){
			break;
		}
	}	
	nCnt = 0; 
	// �رջ���
	while( (CMM_ExecATCmd( CDMA_ATE0, NULL, GSM_ATTIME_CHECK, CDMA_OK, CDMA_CME, NULL ) != GSM_SUCCEED) && 
				 (nCnt++ < 50) ){
		OSTimeDlyHMSM(0,0,1,0);
	}
	// ��⿨�Ƿ����
	nCnt = 0;
	while( 	( CMM_CheckSIMReady() != GSM_SUCCEED) && 
					( nCnt++ < 50 ) ){		
		OSTimeDlyHMSM(0,0,1,0);
	}
	// ���ݿ�������
	nCnt = 0;
	while( nCnt++ < 50 ){
		if( CMM_ExecATCmd( CDMA_HCTCM, (unsigned char*)"=0", GSM_ATTIME_CHECK, CDMA_HCTCM, CDMA_CME , NULL ) == GSM_SUCCEED ){
			break;
		}
		OSTimeDlyHMSM(0,0,1,0);		
	}
	/*
	// �ն����к�
	#if( SW_PROTOCOL_ASICO == 0 )
	if( strlen((char*) g_stGSMSetup.m_strUID ) <= 0 ){
		TE_SetNeedWriteFlash( );
	}
	nCnt = 0;
	while( ( CMM_QueryModuleSN(g_stGSMSetup.m_strUID) != GSM_SUCCEED )&&
				 ( nCnt++ < 50 ) ){	 	
		OSTimeDlyHMSM(0,0,1,0);
	}
	#endif //SW_PROTOCOL_ASICO
	*/
	
	//  �¶���֪ͨ����
	nCnt = 0;
	while( nCnt++ < 50 ){
		if( CMM_ExecATCmd( CDMA_CNMI, (unsigned char*)"=1,1,0,1,0", GSM_ATTIME_CHECK, CDMA_OK, CDMA_CMS, NULL ) == GSM_SUCCEED ){
			break;
		}
		OSTimeDlyHMSM(0,0,1,0);		
	}			
	//  ���ö��ŷ��ͷ�ʽ	
    CMM_ExecATCmd( CDMA_SMSSS, (unsigned char*)"=1,0,1,0", GSM_ATTIME_CHECK, CDMA_OK, CDMA_CME, NULL );	
	//  ѡ����Ŵ洢��
	CMM_ExecATCmd( CDMA_CPMS, (unsigned char*)"=\"ME\"", GSM_ATTIME_CHECK, CDMA_CPMS, CDMA_CME, NULL );
	
	// ���ö��Ÿ�ʽ
	CMM_ExecATCmdN( CDMA_CMGF, 1, GSM_ATTIME_CHECK, CDMA_OK, CDMA_CME, NULL );
	// ����MIC
	CMM_ExecATCmdN( CDMA_MICV, 4, GSM_ATTIME_CHECK, CDMA_OK, CDMA_CME, NULL );
	// ������������
	CMM_ExecATCmdN( CDMA_SETEC, 4, GSM_ATTIME_CHECK, CDMA_OK, CDMA_CME, NULL );
	// ����������ʾ
	CMM_ExecATCmdN( CDMA_CLIP, 1, GSM_ATTIME_CHECK, CDMA_OK, CDMA_CME, NULL );
		
	// ɾ������
	CMM_DeleteAllSMS();
}
// �ܷ���1
unsigned char CMM_QueryIPNetworkStatus( ){
	return 1;
}
/////////////////////////////////////////////////////////////////
// 
unsigned char CMM_AT( void ){
	return CMM_ExecATCmd( CMM_CMD_END, NULL, GSM_ATTIME_CHECK, CDMA_OK, CDMA_CME, NULL );	
}
/////////////////////////////////////////////////////////////
//��ʼ����������
unsigned char CMM_IPInit( unsigned char *pStrGateway, unsigned char *pUser, unsigned char *pPsd ){
		
	CMM_SetPPPUserName( pUser, pPsd );  //����PPP����
	CMM_DTUPPPClose();					//�ر�PPP����
	CMM_DTUPPPOpen();					//��PP����
	return GSM_SUCCEED;
}
/////////////////////////////////////////////////////////////
//	PDP�����ļ���
void	CMM_ActivationPDP( void ){
	
	
}
///////////////////////////////////////////////////////////	
// ��ѯGSM����״̬
// 0,2,3,4: δע��
// 1:				ע�ᣬ����
// 5:				ע��, ���
// ��Ч����1 ��Ч����0
unsigned char CMM_QueryNetworkStatus( void ){

	unsigned char 	nState = 0;	

	return nState;
}
////////////////////////////////////////////////////////////////////////////
//   TCP/IP����
// ����GSM_SUCCEED��ʾ�ɹ�������ʧ��
// $HUDPOPEN: 1, 0
unsigned char CMM_IPConnect( unsigned char nMode, 
							 unsigned char *pStrIP,
							 unsigned short nPort ){ 														 	
														 	
	unsigned char	  nResult = GSM_NG;
	unsigned char	  *pch  = NULL;
	unsigned char 	  nReturnMsg[50];

	if( nMode ){ //TCP
		sprintf((char*)nReturnMsg, "=1,%s,%d",  pStrIP, nPort);	
		nResult = CMM_TCPConnect( CDMA_TCPOPEN, nReturnMsg, nReturnMsg ); 
	}
	else{       //UDP
		sprintf((char*)nReturnMsg, "=2,%s,%d",  pStrIP, nPort);			
		nResult = CMM_TCPConnect( CDMA_UDPOPEN, nReturnMsg, nReturnMsg );
	}	
	if( nResult == GSM_SUCCEED ){		
		pch = (unsigned char*)strchr( (char*)nReturnMsg, ',');
		if ( pch != NULL ){
			nResult = atoi( (char*)pch+1 );
			if ( 0 == nResult ){
				nResult = GSM_SUCCEED;
			}
			else{
				nResult = GSM_NG;
			}
		}
	}
	return  nResult;
}
////////////////////////////////////////////////////////////////////////////
//   TCP/IP���ӷ�ʽ
unsigned char	CMM_TCPConnect(unsigned char nCmdType, unsigned char	*pCmdData, unsigned char	*pReturnMsg ){

	unsigned char 	nResult = GSM_NG;	
	unsigned char	nError = 0;
	unsigned char	nType = 0;
	unsigned char	nCnt = 0;
	unsigned short	nTimeout = GPRS_GPRSDATA_CHECK;
	unsigned char	*lpLargeBuf = NULL;
	
	lpLargeBuf	= lpLargeBuf4;
	CMM_SendATCmd( nCmdType, 0, pReturnMsg );	
	while ( 1 ){		
		OSSemPend(pGSMReceiveSem, nTimeout, &nError);
		if( nError == OS_TIMEOUT ){
			if ( nCnt++ > 4 ){
				break;
			}
		}
		else{ 
			CMM_ReadCmd( lpLargeBuf , GSM_RECMAXBUFFER );						
			nType = CMM_GetCmdType(lpLargeBuf);				
			if( (nType == CDMA_TCPSTATUS)||(nType == CDMA_UDPSTATUS) ){	
				strcpy( (char*)pReturnMsg, (char*)lpLargeBuf );
				nTimeout = GSM_LOWAIT_TIME;
				nResult = GSM_SUCCEED;
			}
			else if( nType == CDMA_CME ){
				nTimeout = GSM_LOWAIT_TIME;			
			} 
			
			else if ( nType == CDMA_OK ){
				nTimeout = GSM_SENDSPACE_CHECK;	
			}
			else{
				CMM_ParseMasterCmd(lpLargeBuf, nType);				
			}	
		}				
	}
	return nResult;
}
/////////////////////////////////////////////////////////////////
// ��������������
unsigned char GSM_ParseDNS( unsigned char *pStrHost, unsigned char *pStrIP ){

	return GSM_NG;
}
///////////////////////////////////////////////////////////////////////////
//   �ر� TCP/IP����
//   $HTCPCLOSE: 1, 0
void	CMM_IPClose( ){
	CMM_IPClose2(1);
	CMM_IPClose2(0);
	return;
}
///////////////////////////////////////////////////////////////////////////
//   �ر� TCP/IP����
//   $HTCPCLOSE: 1, 0
unsigned char	CMM_IPClose2( unsigned char	nMode ){
	
	unsigned  char	nResult = GSM_NG;
	unsigned char	*pch = NULL;
	unsigned char	*pch1 = NULL;
	unsigned char 	nReturnMsg[40]; 
	
	if( nMode ){ //TCP
		nResult = CMM_ExecATCmdN( CDMA_TCPCLOSE, 1, GPRS_GPRSDATA_CHECK, CDMA_TCPCLOSE, CDMA_CME, nReturnMsg );
	}
	else{    			//UDP
		nResult = CMM_ExecATCmdN( CDMA_UDPCLOSE, 2, GPRS_GPRSDATA_CHECK, CDMA_UDPCLOSE, CDMA_CME, nReturnMsg );
	}
	if( GSM_SUCCEED == nResult ){
		pch = nReturnMsg;
		pch1 = (unsigned char*)strchr( (char*)pch, ',');
		if ( pch != NULL ){
			pch = pch1+1;
			nResult = atoi( (char*)pch );
			if ( 0 == nResult ){
				nResult = GSM_SUCCEED;
			}
			else{
				nResult = GSM_NG;
			}
		}
	}
	return nResult;
}
/////////////////////////////////////////////////////////////
// ����PPP�û���������
unsigned char	CMM_SetPPPUserName( unsigned char	*pUser, unsigned char  *pPsd ){
			
	unsigned char		nBuf[50];
	
	sprintf( (char*)nBuf, "=%s,%s"	, pUser, pPsd );				
	return CMM_ExecATCmd(CDMA_ACCNT, nBuf, GPRS_GPRSDATA_CHECK, CDMA_OK, CDMA_CME, NULL);	
}
/////////////////////////////////////////////////////////////
//  DTU���������ϱ� NG
unsigned char	CMM_DTUPPPOpen( void ){
		
	unsigned char 	nResult = GSM_NG;
	unsigned char   err = 0;
	unsigned char	nCnt = 0;
    unsigned char   nCmdType = 0;
    unsigned short  nTimeOut = 0;
    unsigned char	*lpLargeBuf = NULL;
	
	
	lpLargeBuf	= lpLargeBuf4;
    
	// ����ʱ���Ƶ��ź���	
 	if( CMM_ExecATCmd(CDMA_PPPOPEN, NULL, GPRS_GPRSDATA_CHECK, CDMA_OK, CDMA_CME, NULL) == GSM_SUCCEED ){ 	
		nTimeOut = GPRS_GPRSDATA_CHECK;
		nCnt = 0;
		while( 1 ){
			OSSemPend(pGSMReceiveSem, nTimeOut, &err);
			if( err == OS_TIMEOUT ){
				if ( nCnt++ > 4 ){
					 break;
				}			
			}
			else{
				CMM_ReadCmd( lpLargeBuf, GSM_RECMAXBUFFER );	
				nCmdType = CMM_GetCmdType( lpLargeBuf );
				if ( nCmdType == CDMA_PPPSTATUS ){	
					if( atoi((char*)lpLargeBuf) == 0 ){
						nResult = GSM_SUCCEED;
					}
					else{
						nResult = GSM_NG;
					}
					nTimeOut = GSM_SENDSPACE_CHECK;
				}
				// �յ�OK����֮ǰ���յ����ݣ����Ϸ���
				else if( nCmdType == CDMA_OK ){					
					break;
				}
				else if( nCmdType == CDMA_CME){
					nTimeOut = GSM_SENDSPACE_CHECK;
				}
				else{
					CMM_ParseMasterCmd( lpLargeBuf, nCmdType );
				}
			}	
		}
	}
	return nResult;
}
/////////////////////////////////////////////////////////////
//  DTU�ر������ϱ�
unsigned char CMM_DTUPPPClose( void ){ //

	return CMM_ExecATCmd(CDMA_PPPCLOSE, NULL, GPRS_GPRSDATA_CHECK, CDMA_OK, CDMA_CME, NULL);
}
/////////////////////////////////////////////////////////////
//  ��ѯDTU��������״̬�ϱ�
unsigned char	CMM_DTUQueryPPPStatus( void ){
	
	unsigned char 	nResult = GSM_NG;
	unsigned char 	nReturnMsg[20];

	if ( CMM_ExecATCmd(CDMA_PPPSTATUS, NULL, GPRS_GPRSDATA_CHECK, CDMA_PPPSTATUS, CDMA_CME, NULL) == GSM_SUCCEED ){
		 nResult = atoi( (char*)nReturnMsg );
		 if ( 0 == nResult ){
		 	nResult = GSM_SUCCEED;
		 }
		 else{
		 	nResult = GSM_NG;	
		 }
	}
	return nResult;	
}

/////////////////////////////////////////////////////////////////
// ����ATָ���ʽ
// ������������, ��ȡ���������
unsigned char CMM_GetCmdType(unsigned char *pMsg){
	
	unsigned char 	nCnt = 0;
	unsigned char		nLen = 0;
	unsigned char 	*pTmp = NULL;
	
	for( nCnt = 0; nCnt < CMM_CMD_END; nCnt++ ){		
		nLen = strlen( s_strCMMCMD2[nCnt] );
	 	if( memcmp( pMsg, s_strCMMCMD2[nCnt], nLen ) == 0 ){
	 		
	 		pTmp = (unsigned char*)strchr( (char*)pMsg, ':');
	 		if( pTmp ){
	 			pTmp++;
	 			strcpy( (char*)pMsg, (char*)pTmp );
	 		}	 		
	 		break;		 		
	 	}	 	
	}
	if ( nCnt >= CMM_CMD_END ){
		for( nCnt = 0; nCnt < CMM_CMD_END; nCnt++ ){
		nLen = strlen( s_strCMMCMD[nCnt] );
	 	if( memcmp( pMsg, s_strCMMCMD[nCnt], nLen ) == 0 ){	
		 	 pTmp = (unsigned char*)strchr( (char*)pMsg, ':');
			 if( pTmp ){
			  	pTmp++;
					strcpy( (char*)pMsg, (char*)pTmp );
			}	 		
			break;		 		
	 	 }	 	
	  }
	}
	return nCnt;
}
/////////////////////////////////////////////////////////////////////////
// ��⿨�Ƿ����
unsigned char CMM_CheckSIMReady( ){ 
		
	unsigned char 	nResult = 0;
	unsigned char 	nReturnMsg[20];
	
	if( CMM_ExecATCmd( CDMA_CPIN, (unsigned char *)TEXT_QUESTION, GSM_ATTIME_CHECK, CDMA_CPIN, CDMA_CME, nReturnMsg ) == GSM_SUCCEED ){
		if( strstr( (char*)nReturnMsg, (char*)"READY" ) != NULL ){
			 nResult = 1;
		}		
	} 
	return nResult;
}
//////////////////////////////////////////////////////////
//
unsigned char CMM_QueryModuleSN( unsigned char *pStr ){	
	
	return CMM_ExecATCmd( CDMA_GSN, NULL, GSM_ATTIME_CHECK, CDMA_GSN, CDMA_CME, pStr );
}
/////////////////////////////////////////////////////////////////////////
// ��绰                                                                                                                                                                                                                                  
unsigned char CMM_DialNumber( unsigned char *pStrNo ){
	
	unsigned char		nBuf[50];
	unsigned char		nResult;
	
	OSTimeDly(200);	
	sprintf((char*)nBuf, "%s", (char*)pStrNo ); 
	nResult = CMM_ExecATCmd(CDMA_CDV, nBuf, GSM_ATTIME_CHECK, CDMA_CORG, CDMA_CME, NULL);	
	if( nResult == GSM_SUCCEED ){
		CMM_SetCallOnLine( );		
	};
	return nResult;
}
//////////////////////////////////////////////////////////////
// �ӵ绰
unsigned char CMM_AcceptCall( void ){
	
	return CMM_ExecATCmd(CDMA_QCCAV, NULL, GSM_ATTIME_CHECK, CDMA_OK, CDMA_CME, NULL);	
}
//////////////////////////////////////////////////////////////
//  �ҵ绰
unsigned char CMM_RejectCall( void ){
	
	return CMM_ExecATCmd(CDMA_CHV, NULL, GSM_ATTIME_CHECK, CDMA_OK, CDMA_CME, NULL);	
}
////////////////////////////////////////////////////////////////////////////
// �����źż���
unsigned char CMM_QuerySignal( void ){
	
	unsigned char   nReturnMsg[20];
	
	
	unsigned char	nResult = 0;
	
	if( CMM_ExecATCmd( CDMA_CSQ, (unsigned char*)TEXT_QUESTION, GSM_ATTIME_CHECK, CDMA_CSQ, CDMA_CME, nReturnMsg) == GSM_SUCCEED ){
		nResult = atoi( (char*)nReturnMsg );
		if( nResult == 99 ){
			 nResult = 0;
		}
	}
	return nResult;
}
//	��ѯ��ַС��ID  +CREG: 2,1,"2483","0E9D"
unsigned char	CMM_QueryNetworkLac( int	*pLac, int	*pID ){

	return GSM_NG;
}
////////////////////////////////////////////////////////////////////////////
//  ɾ������
void CMM_DeleteAllSMS( void ){
	
	unsigned char	 nCnt = 0;
	
	for( nCnt = 1; nCnt < 100; nCnt++ ){
		if( CMM_DeleteSMS(nCnt) != GSM_SUCCEED ){
			break;
		} 
	}
}
//////////////////////////////////////////////////////////////
// ɾ������
unsigned char CMM_DeleteSMS( unsigned short nCnt ){	
				
	return CMM_ExecATCmdN(CDMA_CMGD, nCnt, GSM_ATTIME_CHECK, CDMA_CMGD, CDMA_CMS, NULL);		
}															 															 
//////////////////////////////////////////////////////////////
// ASCII bit
unsigned char CMM_SendASCIIData( unsigned char *pCaller, 
								 unsigned char *pMsg, 
								 unsigned char nLen ){	

	//  ���ö��ŷ��ͷ�ʽ
	CMM_ExecATCmd( CDMA_SMSSS, (unsigned char*)"=0,0,1,0", GSM_ATTIME_CHECK, CDMA_OK, CDMA_CME, NULL );								 
	if( nLen > 157 ){
		nLen = 157;
	}

	return CMM_SendSMS( pCaller, pMsg, nLen );
}
//////////////////////////////////////////////////////////////
// OCTET
unsigned char CMM_SendOctetData( unsigned char *pCaller, 
								 unsigned char *pMsg, 
								 unsigned char nLen ){		
	//  ���ö��ŷ��ͷ�ʽ
	CMM_ExecATCmd( CDMA_SMSSS, (unsigned char*)"=0,0,3,0", GSM_ATTIME_CHECK, CDMA_OK, CDMA_CME, NULL );								 
	if( nLen > 137 ){
		nLen = 137;
	}
	return CMM_SendSMS( pCaller, pMsg, nLen);
}
//////////////////////////////////////////////////////////////
// unicode
unsigned char CMM_SendUnicodeData( unsigned char *pCaller, 
								   unsigned char *pMsg, 
								   unsigned char nLen ){																 	 	

		//  ���ö��ŷ��ͷ�ʽ
	CMM_ExecATCmd( CDMA_SMSSS, (unsigned char*)"=0,0,6,0", GSM_ATTIME_CHECK, CDMA_OK, CDMA_CME, NULL );								   
	if( nLen > 140 ){
		nLen = 140;
	}
	return CMM_SendSMS( pCaller, pMsg, nLen);											 	
}
///////////////////////////////////////////////////////////////
// ���� GSM����
// send:
// AT+CMGS="Phone Number"<CR><LF>
// recv:
// >
// send:
// pdu ^Z<CR><LF>
// recv1:OK
// recv2:%CMGSS:xx<CR><LF><CR><LF>  (option)
// recv3:OK
unsigned char CMM_SendSMS( unsigned char *pCaller,
						   unsigned char *pMsg,  
						   unsigned char nDataLen ){

	unsigned char 	nResult  = GSM_NG; 
	unsigned char 	err 	 = 0;	
	unsigned char 	nCnt 	 = 0;
	unsigned char 	nCmdType = 0;
	unsigned short	nTimeout = 0;
	unsigned char	*lpLargeBuf = NULL;
	
	
	lpLargeBuf	= lpLargeBuf4;	
	
	CMM_SetSendSMSUnReady( );	
	sprintf((char*)lpLargeBuf, "=\"%s\"", pCaller);
	CMM_SendATCmd( CDMA_CMGS, 1, lpLargeBuf );	
	nTimeout = GSM_SENDSPACE_CHECK;
	while ( 1 ){
		OSSemPend(pGSMReceiveSem, nTimeout, &err);
						
		if( err == OS_TIMEOUT ){	
			if( CMM_IsSendSMSReady( ) ){						
				CMM_SetSendSMSUnReady( );			
				GSMUARTSendData(pMsg, nDataLen);
				GSMUARTSendStr((unsigned char*)TEXT_SMSSEND_END);
				GSMUARTSendStr((unsigned char*)TEXT_LF_RN);			
				while( !GSMUARTIsSendBufEmpty( ) ){
					OSTimeDly( 2 );	
				}
				nTimeout = GSM_ATTIME_CHECK;
				nCnt = 0;
				continue;
			}  
			else{	
				if( nCnt++ >= 4 ){
					break;
				}
			}
		}		
		else{		
			CMM_ReadCmd( lpLargeBuf , GSM_RECMAXBUFFER );
			nCmdType = CMM_GetCmdType(lpLargeBuf);		 	
			if(nCmdType ==  CDMA_CMGSS){
				nTimeout = GSM_SENDSPACE_CHECK; 				
				nResult = GSM_SUCCEED;
				nCnt = 0;
			} 	
			else if( nCmdType == CDMA_CME ){
				nResult = GSM_NG;	
				break;
			}
			else if( nCmdType == CDMA_CMGSF ){
				nResult = GSM_NG;	
				break;
			}
			else if( nCmdType == CDMA_OK ){
				if( nResult == GSM_SUCCEED ){
					break;
				}
				else{
					nTimeout = GSM_ATTIME_CHECK; 
					nCnt = 0;
				}
			}			
			else{
				CMM_ParseMasterCmd( lpLargeBuf, nCmdType );
			}
		}	
	}		
					
	return nResult;
}
//////////////////////////////////////////////////////////////////
// ����GPRS���� 0: ʧ��  1:  �ɹ�
// send:
// AT+IPSEND=ͨ��,����
// recv1:ok
// send data
// $HUDPSEND:ͨ��,����
unsigned char IP_SendData( unsigned char nMode,
						   unsigned char *pBuf, 
						   unsigned char nLen){
			
	unsigned char  		nResult = GSM_NG;	
	unsigned char		nCnt = 0;
	unsigned short		nTimeOut = 0;
	unsigned char  		err = 0;
	unsigned char		nCMError = 0;
	unsigned char  		nCmdType = 0;
	unsigned char		*lpLargeBuf = NULL;
	
	
	lpLargeBuf	= lpLargeBuf4;	
	
	if ( nMode ){ //TCP
		 sprintf((char*)lpLargeBuf, "=1,%d", nLen );
		 CMM_SendATCmd( CDMA_TCPSEND, 1, lpLargeBuf );
	}
	else{         //UDP
		 sprintf((char*)lpLargeBuf, "=2,%d", nLen );
		 CMM_SendATCmd( CDMA_UDPSEND, 1, lpLargeBuf );
	}	
	nTimeOut = GSM_ATTIME_CHECK;
	nCnt = 0;
	while( 1 ){
		OSSemPend(pGSMReceiveSem, nTimeOut, &err); 
		if (err == OS_TIMEOUT){
			if ( nCnt++ > 2 ){
				break;
			}			
		}
		else{		
			CMM_ReadCmd( lpLargeBuf , GSM_RECMAXBUFFER );		
			nCmdType = CMM_GetCmdType(lpLargeBuf);							
			if( nCmdType == CDMA_OK )	{
				if ( 0 == nCMError ){
					nResult = GSM_SUCCEED;
				}
				break;
			}		
			else if( CDMA_CME == nCmdType ){
				nCMError = 1;
			}
		}
	}
	// ���Է�������
	if ( nResult == GSM_SUCCEED ){
		
		GSMUARTSendData(pBuf, nLen);
		GSMUARTSendStr((unsigned char *)TEXT_LF_RN);			
		while( !GSMUARTIsSendBufEmpty( ) ){
			OSTimeDly( 2 );	
		}
		nTimeOut = GPRS_GPRSDATA_CHECK;
		nCnt = 0;
		while( 1 ){
			OSSemPend(pGSMReceiveSem, nTimeOut, &err); 		
			nTimeOut = GSM_ATTIME_CHECK;
			if (err == OS_TIMEOUT){
				if ( nCnt++ > 4 ){
					break;
				}			
			}
			else{
					CMM_ReadCmd( lpLargeBuf , GSM_RECMAXBUFFER);		
					nCmdType = CMM_GetCmdType(lpLargeBuf);	
					if( nCmdType == CDMA_TCPSEND )	{
						nResult = GSM_SUCCEED;
						break;
					}
					else if( nCmdType == CDMA_UDPSEND ){
						 nResult= GSM_SUCCEED;
						 break;
					}
					else if( nCmdType == CDMA_CME ){
						 nResult = GSM_NG;
						 break;
					}
					else{
						 CMM_ParseMasterCmd( lpLargeBuf, nCmdType );
					}
			}			
		}
	}	
	return nResult;	
}
///////////////////////////////////////////////////
// ��ȡ��������
// send: at+CMGR=x
// recv1:
// pCodeType: 0: ASCII
//			  1: OCT
//            2: UNICODE
unsigned char CMM_ReadSMS( unsigned char nCnt, 
						   unsigned char *pCaller, 
						   unsigned char *pMsg,
						   unsigned char *pCodeType ){
													 	
   unsigned char  err = 0;
   unsigned char  nLen = 0;
   unsigned char  nRecordCnt = 0;
   unsigned char  nRecvLen = 0;
   unsigned char  nHsmsrd = 0;
   unsigned char  nCmdType = 0;	 

   unsigned short nTimeOut = 0;
   unsigned short nCntTmp = 0;
   unsigned char  nCmdBuf[20];

 //  if ( nCnt > 0 ){
 //		nCnt -= 1;
 //  }
   sprintf((char*)nCmdBuf, "=%d", nCnt );
   CMM_SendATCmd( CDMA_CMGR, 0, nCmdBuf );
   // ��ȡ���ű�ͷ	
	//         <callerid>   <date>			 <time><lang><fmt><length><prt���ȼ�><prv���ܼ�>
	// $HSMSRD: 13480924411, <2009/03/28> <23:09>, 0, 1, 14, 1, 0
	// *excute**888##
	// 1: ascii  (length:0~160)(���轫GSM������ascii)
	// 2: unicode(length:0~70)
	// 3: 8λ����(length:0~140)
	 nLen = 0;	
	 nTimeOut = GSM_ATTIME_CHECK;
	 nCnt = 0;
	 nRecordCnt = 0;
	 while( 1 ){
	 		OSSemPend(pGSMReceiveSem, nTimeOut, &err);
	 		if( err == OS_TIMEOUT ){
				if ( nRecordCnt++ > 4 ){
					 break;
				}			
			}
			else{
				nCntTmp = CMM_ReadCmd((unsigned char *)pMsg+nLen, GSM_RECMAXBUFFER);	
				nCmdType = CMM_GetCmdType( pMsg+nLen );
				if ( nCmdType == CDMA_CMGR ){
					CMM_ParseSMSFormatA( pCaller, pMsg+nLen,  pCodeType, &nRecvLen );									
					nHsmsrd = 1;						
					nTimeOut = GSM_SENDSPACE_CHECK;
				}
				else if( nCmdType == CDMA_HSMSRD ){										
					CMM_ParseSMSFormatB( pCaller, pMsg+nLen,  pCodeType, &nRecvLen );									
					nHsmsrd = 1;						
					nTimeOut = GSM_SENDSPACE_CHECK;
				}				
				// �յ�OK����֮ǰ���յ����ݣ����Ϸ���
				else if( nCmdType == CDMA_OK ){
					if((nLen > 0) && (nHsmsrd == 1) ){
						 break;
					}
				}
				else if( nCmdType == CDMA_CME){
					break;				
				}
				else if( nCmdType == CMM_CMD_END ){
								
				   if ( (nHsmsrd == 1) && (nCntTmp > 0) ){				   								
						// ASCII
						nLen = nCntTmp;
						/*
						if ( *pCodeType == 0 ){
						   nLen = P_GSMToASCII( (char*)pMsg+nLen, (char*)pMsg+nLen, nCntTmp);
						}
						else if( *pCodeType == 1 ){
							nLen = nCntTmp;
						}
						else if ( *pCodeType == 2 ){
							nLen = nCntTmp * 2;
						}		
						*/				
						nTimeOut = GSM_SENDSPACE_CHECK;
					}
			   }
			   else{
				 CMM_ParseMasterCmd( pMsg+nLen, nCmdType );
			   }
			}
	 }
	 return  nLen;
}
//  ���Ͷ��Ÿ�ʽ
///////////////////////////////////////////////////
// %CMGR:13760374664,2009,04,06,10,04,1,4,1,0,0
unsigned char CMM_ParseSMSFormatA( unsigned char *pCaller, 
								  unsigned char *pMsg,  
								  unsigned char *pCodeType,
								  unsigned char *pDataLen ){ 
	unsigned char		nType = 0;
	unsigned char		nCnt = 0;
	unsigned char		*pch = NULL;							  
		
	if( *pMsg == ' ' ){
		pMsg += 1;
	}
	pch = (unsigned char*)strchr( (char*)pMsg, ',');
	if ( pch == NULL ){
		return GSM_NG;
	}
	*pch = 0x00;
	strcpy( (char*)pCaller, (char*)pMsg);
	pMsg = pch + 1;
	
	while( nCnt++ <= 4 ){
		pch = (unsigned char*)strchr( (char*)pMsg, ',');
		if ( pch == NULL ){
			return GSM_NG;
		}
		pMsg = pch + 1;
	}
	nType = atoi( (char*)pMsg );
	switch( nType ){
	case 1: //ASCII
		*pCodeType = 0;
		break;
				
	case 6: // UNICODE
		*pCodeType = 2;
		break;
		
	case 3: // OCTET
	default:
		*pCodeType = 1;
		break;
	}		
	pMsg = pch + 1;
	pch = (unsigned char*)strchr((char*)pMsg, ',');
	if( pch == NULL ){
		return GSM_NG;
	}
	*pch = 0x00;		
	*pDataLen = atoi((char*)pMsg );		
	
	return GSM_SUCCEED;								  
}
///////////////////////////////////////////////////
//  ���Ͷ��Ÿ�ʽ
//  $HSMSRD: 13480924411, <2009/03/28> <23:09>, 0, 1, 14, 1, 0
//           13480924411, <2009/03/28> <23:09>, 0, 1, 14, 1, 0
unsigned char CMM_ParseSMSFormatB( unsigned char *pCaller, 
								  unsigned char *pMsg,  
								  unsigned char *pCodeType,
								  unsigned char *pDataLen ){
	unsigned char		nType = 0;
	unsigned char		*pch = NULL;
	
	
	if( *pMsg == ' ' ){
		pMsg += 1;
	}
	pch = (unsigned char*)strchr( (char*)pMsg, ',');
	if ( pch == NULL ){
		return GSM_NG;
	}
	*pch = 0x00;
	strcpy( (char*)pCaller, (char*)pMsg);
	pMsg = pch + 1;
	// ���� ʱ��
	// <2009/03/28> <23:09>, 0, 1, 14, 1, 0
	pch = (unsigned char*)strchr( (char*)pMsg, ',');
	if( pch == NULL ){
		return GSM_NG;
	}
	pMsg = pch + 1;
	// ����
	//  0, 1, 14, 1, 0
	pch = (unsigned char*)strchr( (char*)pMsg, ',');
	if( pch == NULL ){
		return GSM_NG;
	}
	pMsg = pch + 1;
	// �����ʽ
	
	// 1, 14, 1, 0
	pch = (unsigned char*)strchr( (char*)pMsg, ',');
	if( pch == NULL ){
			return GSM_NG;
	}
	*pch = 0x00;
	nType = atoi( (char*)pMsg );
	switch( nType ){
	case 1: //ASCII
		*pCodeType = 0;
		break;
				
	case 6: // UNICODE
		*pCodeType = 2;
		break;
		
	case 3: // OCTET
	default:
		*pCodeType = 1;
		break;
	}		
	pMsg = pch + 1;
	
	// ���ݳ���
	
	// 14, 1, 0
	pch = (unsigned char*)strchr((char*)pMsg, ',');
	if( pch == NULL ){
		return GSM_NG;
	}
	*pch = 0x00;		
	*pDataLen = atoi((char*)pMsg );		
	return GSM_SUCCEED;
}
///////////////////////////////////////////////////////////////
//  ģ����Ƶ
unsigned char	CMM_ConfigCBand( void ){

	return 0;
}
/////////////////////////////////////////////////////////////
// ����:				0�� free
//						1:  on use
unsigned char GSM_ReadCaller( unsigned char *pCaller ){
	
	unsigned char			nResult = 0;
	
	if( g_stGSMState.m_nDialLatLng > 0 ){
	
		 nResult = 1;
	}
	return nResult;	
}
// �������洢������Ϣ
void CMM_ParseMasterCmd(unsigned char *pMsg, unsigned char nCmdType){
	
	unsigned char		nCnt = 0;
	unsigned short		nLen;
	unsigned char 		nCaller[TELNUMBER_LENGTH+5]; 
	unsigned char		*pTmp = NULL;
	

	switch(nCmdType){
	//case CDMA_RING:
	case CDMA_CID:
		nCaller[0] = 0x00;
		CMM_ReadCallerID( pMsg, nCaller );
		if( strlen( (char*)nCaller) > 0 ){
			if( IsAuthorizationNo( (unsigned char*)nCaller) ){
							
				CMM_AcceptCall( );		
				CMM_SetCallOnLine( );								
						
				#if( HW_ET201B || HW_ET201B_2 || HW_ET201B_3 )	
				if(memcmp("669475", nCaller, PHONE_COMPARE_BIT) == 0 ){
					
					HW_PORT_SPKMUTE_OPEN(); // �򿪾���
				}
				else{ 
					#if( DIAL_CALL )
					HW_PORT_SPKMUTE_OPEN(); // �򿪾���
					#else	
					HW_PORT_SPKMUTE_CLOE(); // �رվ���	
					#endif 
				}
				#endif 
			}
			else{
				CMM_RejectCall( );
				CMM_SetCallOffLine( );		
			}
		}	
		break; 	
	case CDMA_CLIP:
		nCaller[0] = 0x00;
		CMM_ReadCallerCLIP(pMsg, nCaller);
		if( strlen( (char*)nCaller) > 0 ){
					
			if( IsAuthorizationNo( (unsigned char*)nCaller) ){						
						
				if(memcmp("669475", nCaller, PHONE_COMPARE_BIT) == 0 ){
					
					g_stGSMState.m_nDialLatLng = 0;
					CMM_AcceptCall( );		
					CMM_SetCallOnLine( );
				}
				else{				
				//	if( g_stGSMState.m_nDialLatLng++ >= 3 ){
					
						g_stGSMState.m_nDialLatLng = 0;
						CMM_AcceptCall( );		
						CMM_SetCallOnLine( );
				//	}					
					// ȡ�绰����
				//	nCnt = strlen( (char*)nCaller);							
				//	memcpy( g_stGSMState.m_strDialNo, nCaller, nCnt );
				//	g_stGSMState.m_strDialNo[nCnt] = 0x00;
				}				
				
				#if( HW_ET201B || HW_ET201B_2 || HW_ET201B_3 )	
				if(memcmp("669475", nCaller, PHONE_COMPARE_BIT) == 0 ){
					
					HW_PORT_SPKMUTE_OPEN(); // �򿪾���
				}
				else{ 
					#if( DIAL_CALL )
					HW_PORT_SPKMUTE_OPEN(); // �򿪾���
					#else	
					HW_PORT_SPKMUTE_CLOE(); // �رվ���	
					#endif 
				}
				#endif
				
			}
			else{
				CMM_RejectCall( );
				CMM_SetCallOffLine( );		
			}
		}	
		break;		
						
	case CDMA_CMTI:		
		pTmp = (unsigned char *)strstr((char *)pMsg, ",");
		if( pTmp ){
			pTmp++;
			nCnt = atoi( (char*)pTmp );
			InsertNewMsg( nCnt );
		}
		break;
				
	case CDMA_TCPRECV:	
		nLen = IP_RecvTCPData( pMsg, pMsg, NULL ); 		
		nLen = PL_ParseData( pMsg, pMsg, nLen );	
		if( nLen ){
			IP_SendData( IP_IsTCPCommMode(), pMsg, nLen );
		}
		break;	
	case CDMA_UDPRECV:
		nLen = IP_RecvTCPData(pMsg, pMsg, NULL );
		nLen = PL_ParseData( pMsg, pMsg, nLen );	
		
		if( nLen ){
			IP_SendData( IP_IsTCPCommMode(), pMsg, nLen );
		}
		break;
		
	// ����Ͽ�
	// ��
	// ͨ������
	// ͨ�����
	// ���˽���
	case CDMA_CEND:
		nLen = CMM_ParseHCEnd( pMsg );
		if( nLen == 0 ){
			// �رվ���
			#if( HW_ET201B || HW_ET201B_2 || HW_ET201B_3 )	
	    HW_PORT_SPKMUTE_CLOE();
	    #endif
			CMM_SetCallOffLine( );
			g_stGSMState.m_nDialLatLng = 0;
		}
		else if( nLen == 1 ){
			IP_SetConnectNG( );			
		}
		break;		
	//	��������
	case GSM_TECNT:
		CMM_SendTECNT();
		nLen = PL_ReadConfig( pMsg );		
		if( nLen > 0 ){	
			sprintf( (char*)nCaller, "AT%s:", s_strCMMCMD[GSM_TESETUP] );		
			GSMUARTSendStr( nCaller );
			GSMUARTSendData( pMsg, nLen );
			GSMUARTSendStr( (unsigned char*)"\r\n" ); 
		}
		break;
	//	�Ҷ�����
	case GSM_TEDISCNT:
		GSM_SendDISCNT();
		break;
	//	l��ȡ/���� �豸����
	case GSM_TESETUP:
		if( *pMsg == '?' ){
			nLen = PL_ReadConfig( pMsg );
		}
		else{
			nLen = PL_ParseConfig(pMsg, pMsg );
			nCnt = 1;			
		}
		if( nLen > 0 ){	
			sprintf( (char*)nCaller, "AT%s:", s_strCMMCMD[GSM_TESETUP] );		
			GSMUARTSendStr( nCaller );
			GSMUARTSendData( pMsg, nLen );
			GSMUARTSendStr( (unsigned char*)"\r\n" ); 
		}
		//дFLASH
		if( nCnt == 1 ){		
			GSM_WriteSetupData();
			WriteAlarmSetupData(); 
		}
		break;	
	default:
		break; 
	}
	return;
}
//////////////////////////////////////////////////////////////
//	�������ӳɹ�,�ر�GSMģ���Դ
void	CMM_SendTECNT( void ){

	GSM_SetSetupConfig();
	CMM_ExecATCmd(GSM_TECNT, NULL, GSM_SENDSPACE_CHECK, CDMA_OK, CDMA_CME, NULL );
}
//////////////////////////////////////////////////////////////
//	���ͶϿ�����,����GSMģ���Դ
void 	GSM_SendDISCNT( void ){

	CMM_ExecATCmd( GSM_TEDISCNT, NULL, GSM_SENDSPACE_CHECK, CDMA_OK, CDMA_CME, NULL );
	TE_SetNeedTEReset();
	GSM_ClrSetupConfig();
}
/////////////////////////////////////////////////////////////////////
// ���յ�GPRS����
// $HUDPRECV: 3, 4  $HTCPRECV: 3, 4
// <CR><LF>
// 1 0��(0x31,0x20,0x30,0x89)
unsigned char IP_RecvTCPData(unsigned char  *pMsg,
							 unsigned char  *pDest,
							 unsigned char  *pSocketNum ){
							
	unsigned char	nDataLen = 0;
	//unsigned char   err = 0;		
	unsigned char	*pch = NULL;
			
	pch = (unsigned char*)strchr((char*)pMsg, ',');
	if ( pch != NULL ){
	
		// SOCKETͨ��
		if( pSocketNum ){
			*pSocketNum = atoi((char*)pMsg);
		}		
		pch += 1;
		
		// ���ݳ���
		nDataLen = atoi( (char*)pch );
		
			
	//	OSSemPend(pGSMReceiveSem, GSM_SENDSPACE_CHECK, &err);
	//	if( err != OS_TIMEOUT ){
			// ����<CR><LF>	
	//		CMM_ReadCmd((unsigned char *)pDest, GSM_RECMAXBUFFER);
			OSTimeDlyHMSM(0,0,0,nDataLen*10);
			CMM_ReadCmd((unsigned char *)pDest, nDataLen);
	//	}	
	}
	return nDataLen;						
}

/////////////////////////////////////////////////////////////////////
// ����������ʾ
// CID: 13480924411
void CMM_ReadCallerID( unsigned char *pMsg, unsigned char *pCaller ){
	
	if( *pMsg == ' ' ){
		 pMsg += 1;
	}				 
	strcpy( (char*)pCaller, (char*)pMsg );
	return;
}
/////////////////////////////////////////////////////////////////////
// ����������ʾ
//+CLIP:13480924411,0
void CMM_ReadCallerCLIP( unsigned char *pMsg, unsigned char *pCaller ){
	
	unsigned char	*pch = NULL;
	
	if( *pMsg == ' ' ){
		 pMsg += 1;
	}
	pch = (unsigned char*)strchr( (char*)pMsg, ',');
	if( pch != NULL ){
		*pch = 0x00;
		strcpy( (char*)pCaller, (char*)pMsg );
	}	
	return;
}
/////////////////////////////////////////////////////////////////////
//  $HCEND: 7, 1, 00:02:59 
// Return :
//       0:  ��������END
//       1:  IP����END
//       2:  ����END
unsigned char CMM_ParseHCEnd( unsigned char  *pMsg ){
	
	unsigned char		nResult = 2;
	unsigned char		*pch = NULL;
	
	pch = (unsigned char*)strchr( (char*)pMsg, ',');
	if ( pch ){
		pch++;
		nResult = atoi( (char*)pch );
	}
	return nResult;
}
/////////////////////////////////////////////////////////////////
//  ��ѯ����
unsigned char	GSM_ExecCUSD( unsigned char	*pDest, unsigned char	*pSrc, unsigned short	nTimeout ){

	unsigned char		nResult = 2;
	
	
	return nResult;
}
//////////////////////////////////////////////////////////////
//  ��ѯIMEI���
unsigned char	CMM_QueryIMEI( unsigned  char	*pDest ){
	
	unsigned char		nResult = 2;
	
	
	return nResult;
}

#endif //HW_EM200