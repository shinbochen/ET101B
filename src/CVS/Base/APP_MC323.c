/////////////////////////////////////////////////////////////
//file name: MC323.c
//     Date: 2011-06-20    
//	 Author: wyb	 
// optimize: 
// op_date : 
/////////////////////////////////////////////////////////////
// include file declare
#include  <stdio.h>
#include  <stdlib.h>
#include "..\hdr\includes.h"
#include "..\hdr\queue.h"
#include "..\hdr\main.h"
#include "..\hdr\APP_GPS.h"
#include "..\hdr\HW_UART.h"
#include "..\hdr\HW_flash.h"
#include "..\hdr\APP_LoopDetect.h"
#include "..\hdr\APP_Terminal.h"
#include "..\hdr\HW_GPIO.h"
#include "..\hdr\APP_MC323.h"
#include "..\hdr\ProtocolB.h"
#include "..\hdr\APP_GSM.h"

#ifdef HW_MC323
//=====================================================
//           
const char	*s_strCMMCMD[]={
		"OK",				//OK
		"E0",				//�رջ���ʾ
		"E1",				//��������		
		"+IFC",		  //����������������
		"+CPIN",		//��⿨	
		"+CMGF",		//
		"+GSN",			//
		
		//�������ӿ�����
	 	"+CREG",		//����ע��
	 	"+CSQ",			//RSSI��ѯ 
		"^RSSIREP",	//�ϱ�ָʾ����		
		
		//���п�������
		"+CDV",			//��������
		"+CHV",			//�Ҷ���������
		"A",				//�������� 
		"RING",			//����ָʾ
		"^ORIG",		//���з���ָʾ
		"^CONN",		//���н�ָͨʾ
		"^CEND",		//ͨ������ָʾ
		"+CLIP",		//������ʾ�������ü�ָʾ
		"+CLVL",		//������������
		"+CMIC",		//��˷���������
		"+CLCC",		//����״̬��ѯ
		"^DTMF",		//���β�������		
				
		//����Ϣҵ��ӿ�����
		"+CPMS",		//���ö��Ŵ洢λ��
		"+CNMI",	  //����֪ͨ��ʽ
		"+CMTI",		//���ŵ�����ʾ
		"^HSMSSS",	//���ö��Ų���
		"^HCMGS",		//���Ͷ���
		"^HCMGSS",	//���ŷ��ͳɹ��ϱ�ָʾ
		"^HCMGSF",	//���ŷ���ʧ���ϱ�ָʾ
		"^HCMGR",		//��ȡһ������
		"+CMGD",		//ɾ������
		
		//Internet����ӿ�����
		"^PPPCFG",	//����PPP�û���������
		"^IPINIT",  //TCP/UDP���ӳ�ʼ��
		"^IPOPEN",	//����TCP/UDP����
		"^IPSEND",	//TCP/UDP���ݷ���
		"^IPSENDEX",//��չָ�� 
		"^IPDATMODE",//TCP/UDP���ݵ���ָʾ����
		"^IPDATA",	//TCP/UDP���ݵ���ָʾ
		"^IPCLOSE", //�ر�TCP/UDP����
		
		"+CME",			// ������ʾ
		"+CMS",			//
		"ERROR",		//����
		
		"+TECNT", 	// ��������
		"+TEDISCNT",// �Ҷ�����
		"+TESETUP", // l��ȡ/���� �豸����
};

extern  OS_EVENT 			*pGSMReceiveSem;

//////////////////////////////////////////////////////////////////
// CDMAӲ����ʼ��
void CMM_HWInit( void ){
	
  GSMUARTInit(9600);		// CDMA ���� 
  	
  // ģ�鸴λ����
// GPIO_Enable(HW_PORT_GSMRESET);
// GPIO_SetDirection(HW_PORT_GSMRESET, HW_PORT_OUTPUT); 
//  HW_GSM_RESET_HIGH();
  	
  // CDMAģ�鿪������	
  GPIO_Enable(HW_PORT_WARMPOWER);			
  GPIO_SetDirection(HW_PORT_WARMPOWER, HW_PORT_OUTPUT);		
  HW_GSM_WARM_POWERON( );
    
  // ֱ�Ӷ�CDMAģ���Դ  
  GPIO_Enable(HW_PORT_COLDPOWER);			
  GPIO_SetDirection(HW_PORT_COLDPOWER, HW_PORT_OUTPUT);			
  HW_GSM_COLDPOWEROFF();  
}
/////////////////////////////////////////////////////////
// CDMAͨ��ģ��Ӳ������	
void CMM_ReStart( void ){
		
	OSTimeDlyHMSM(0,0,0,500);
	HW_GSM_COLDPOWEROFF();
	OSTimeDlyHMSM(0,0,5,0);	
	HW_GSM_COLDPOWERON();
		
//	HW_GSM_RESET_LOW();
//	OSTimeDlyHMSM(0,0,0,50);
//	HW_GSM_RESET_HIGH();
//	OSTimeDlyHMSM(0,0,0,50);
		
	//  �ߵ�
	HW_GSM_WARM_POWERON( );				//H
	OSTimeDlyHMSM(0,0,0,500 );		
	// power on
	HW_GSM_WARM_POWEROFF( );
		
	OSTimeDlyHMSM(0,0,0,200);	
	HW_GSM_WARM_POWERON( );
	OSTimeDlyHMSM(0,0,0,100);
	
}
///////////////////////////////////////////////////////////////////
//��ʼ��CDMAģ��
void CMM_Init(void){  
	
	int 	nCnt = 0;
	
	// �ر�Ӳ����
	while( nCnt++ <= 50 ){
		if (CMM_ExecATCmd( CDMA_IFC, (unsigned char*)"=0,0", GSM_SENDSPACE_CHECK, CDMA_OK, CDMA_ERROR, NULL )  == GSM_SUCCEED){
			break;
		}
	}	
	nCnt = 0; 
	// �رջ���
	while( (CMM_ExecATCmd( CDMA_ATE0, NULL, GSM_ATTIME_CHECK, CDMA_OK, CDMA_ERROR, NULL ) != GSM_SUCCEED) && 
				 (nCnt++ < 50) ){
		OSTimeDlyHMSM(0,0,1,0);
	}
	// ��⿨�Ƿ����
	nCnt = 0;
	while( 	( CMM_CheckSIMReady() != GSM_SUCCEED) && 
					( nCnt++ <= 50 ) ){		
		OSTimeDlyHMSM(0,0,1,0);
	}	
	//  �¶���֪ͨ����
	nCnt = 0; 
	while( nCnt++ < 50 ){
		if( CMM_ExecATCmd( CDMA_CNMI, (unsigned char*)"=1,1,0,1,0", GSM_ATTIME_CHECK, CDMA_OK, CDMA_CMS, NULL ) == GSM_SUCCEED ){
			break;
		}                                                           
		OSTimeDlyHMSM(0,0,1,0);		
	}			
	//  ���ö��ŷ��ͷ�ʽ	
  CMM_ExecATCmd( CDMA_HSMSSS, (unsigned char*)"=1,0,1,0", GSM_ATTIME_CHECK, CDMA_OK, CDMA_CMS, NULL );	
	//  ѡ����Ŵ洢��
	CMM_ExecATCmd( CDMA_CPMS, (unsigned char*)"=\"ME\"", GSM_ATTIME_CHECK, CDMA_CPMS, CDMA_CMS, NULL );
	
	// ���ö��Ÿ�ʽ
	CMM_ExecATCmdN( CDMA_CMGF, 1, GSM_ATTIME_CHECK, CDMA_OK, CDMA_ERROR, NULL );
	// ����MIC
	CMM_ExecATCmdN( CDMA_CMIC, 4, GSM_ATTIME_CHECK, CDMA_OK, CDMA_ERROR, NULL );
	//
	CMM_ExecATCmdN( CDMA_CLVL, 5, GSM_ATTIME_CHECK, CDMA_OK, CDMA_ERROR, NULL );	
	// ������������
	//CMM_ExecATCmdN( CDMA_SETEC, 4, GSM_ATTIME_CHECK, CDMA_OK, CDMA_CME, NULL );
	// ����������ʾ
	CMM_ExecATCmdN( CDMA_CLIP, 1, GSM_ATTIME_CHECK, CDMA_OK, CDMA_ERROR, NULL );
	// �ر�RSSI�ϱ�ָʾ
	CMM_ExecATCmdN( CDMA_RSSIREP, 0, GSM_ATTIME_CHECK, CDMA_OK, CDMA_ERROR, NULL );
	//TCP/UDP���ݵ���ָʾ����	
	CMM_ExecATCmdN( CDMA_IPDATMODE, 1, GSM_ATTIME_CHECK, CDMA_OK, CDMA_ERROR, NULL );
		
	// ɾ������
	CMM_DeleteAllSMS();	
}
/////////////////////////////////////////////////////////////////
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
	CMM_DTUPPPClose();									 //�ر�PPP����
	CMM_DTUPPPOpen();									 //��PP����
	return GSM_SUCCEED;
}
/////////////////////////////////////////////////////////////
//	PDP�����ļ���
void	CMM_ActivationPDP( void ){
	
	
}
///////////////////////////////////////////////////////////	
// ��ѯCDMA����״̬
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
														 	
	unsigned char	    nResult = GSM_NG;
	unsigned char 	  	nReturnMsg[60];

	if( nMode ){ //TCP
		sprintf((char*)nReturnMsg, "=1,\"TCP\",\"%s\",%d,8801",  pStrIP, nPort);	
		nResult = CMM_TCPConnect( CDMA_IPOPEN, nReturnMsg, nReturnMsg ); 
	}
	else{       //UDP
		sprintf((char*)nReturnMsg, "=2,\"UDP\",\"%s\",%d,8802",  pStrIP, nPort);			
		nResult = CMM_TCPConnect( CDMA_IPOPEN, nReturnMsg, nReturnMsg );
	}		
	return  nResult;
}
////////////////////////////////////////////////////////////////////////////
//   TCP/IP���ӷ�ʽ
unsigned char	CMM_TCPConnect(unsigned char nCmdType, unsigned char	*pCmdData, unsigned char	*pReturnMsg ){

	unsigned char 	nResult = GSM_NG;	
	unsigned short	nShortLen = 0;
	unsigned char	nType = 0;
	unsigned char	nError = 0;
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
			nShortLen = CMM_ReadCmd( lpLargeBuf , GSM_RECMAXBUFFER );						
			nType = CMM_GetCmdType(lpLargeBuf, nShortLen );	
			if ( nType == CDMA_OK ){
				nTimeout = GSM_SENDSPACE_CHECK;	
				nResult = GSM_SUCCEED;
			}
			else if(   (nType == CDMA_CME)
				       ||(nType == CDMA_ERROR) ){
				nTimeout = GSM_LOWAIT_TIME;			
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
	
	if( nMode ){ //TCP
		nResult = CMM_ExecATCmdN( CMDA_IPCLOSE, 1, GPRS_GPRSDATA_CHECK, CDMA_OK, CDMA_ERROR, NULL );
	}
	else{    			//UDP
		nResult = CMM_ExecATCmdN( CMDA_IPCLOSE, 2, GPRS_GPRSDATA_CHECK, CDMA_OK, CDMA_ERROR, NULL );
	}	
	return nResult;
}
/////////////////////////////////////////////////////////////
// ����PPP�û���������
unsigned char	CMM_SetPPPUserName( unsigned char	*pUser, unsigned char  *pPsd ){
			
	unsigned char		nBuf[50];
	
	sprintf( (char*)nBuf, "=%s,%s"	, pUser, pPsd );				
	return CMM_ExecATCmd(CDMA_PPPCFG, nBuf, GPRS_GPRSDATA_CHECK, CDMA_OK, CDMA_CME, NULL);	
}
/////////////////////////////////////////////////////////////
//  TCP/UDP���ӳ�ʼ��
unsigned char	CMM_DTUPPPOpen( void ){
			
	return CMM_ExecATCmd(CDMA_IPINIT, NULL, GPRS_GPRSDATA_CHECK, CDMA_OK, CDMA_ERROR, NULL);
}
/////////////////////////////////////////////////////////////
//  �ر�TCP/UDP���ӳ�ʼ��
unsigned char CMM_DTUPPPClose( void ){ //
		
	return CMM_ExecATCmdN( CMDA_IPCLOSE, 7, GPRS_GPRSDATA_CHECK, CDMA_OK, CDMA_ERROR, NULL );
}
/////////////////////////////////////////////////////////////////
// ����ATָ���ʽ
// ������������, ��ȡ���������
unsigned char CMM_GetCmdType(unsigned char *pMsg, unsigned short nDataLen){
	
	unsigned char 	nCnt = 0;
	unsigned char		nLen = 0;
	unsigned char 	*pTmp = NULL;
	
	for( nCnt = 0; nCnt < CMM_CMD_END; nCnt++ ){		
		nLen = strlen( s_strCMMCMD[nCnt] );
	 	if( memcmp( pMsg, s_strCMMCMD[nCnt], nLen ) == 0 ){
	 		
	 		pTmp = (unsigned char*)strchr( (char*)pMsg, ':');
	 		if( pTmp ){
	 			pTmp++;
	 			//strcpy( (char*)pMsg, (char*)pTmp );
	 			memcpy( pMsg, pTmp, nDataLen-(nLen+1) );
	 		}	 		
	 		break;		 		
	 	}	 	
	}
	return nCnt;
}
/////////////////////////////////////////////////////////////////
// ����ATָ���ʽ
// ������������, ��ȡ���������
// ��չָ��,��������,���ص�״̬�� HCMGS, HCMGSS,HCMGSF
unsigned char CMM_GetCmdTypeEx(unsigned char *pMsg, unsigned short nDataLen ){
	
	unsigned char	nLen = 0;
	unsigned char 	nCnt = 0;
	unsigned char 	*pTmp = NULL;	
	
	// ��^HCMGSS ����ֵ
	nLen = strlen( s_strCMMCMD[CDMA_HCMGSS] );
	if( memcmp( pMsg, s_strCMMCMD[CDMA_HCMGSS], nLen ) == 0 ){
	
 		return CDMA_HCMGSS;	
 	}
 	// ��^HCMGSF ����ֵ
 	nLen = strlen( s_strCMMCMD[CDMA_HCMGSF] );
 	if( memcmp( pMsg, s_strCMMCMD[CDMA_HCMGSF], nLen ) == 0 ){
	
 		return CDMA_HCMGSF;	
 	}
 	// ��IPSENDEX ����ֵ�Ĵ���
	nLen = strlen( s_strCMMCMD[CDMA_IPSENDEX] );
 	if( memcmp( pMsg, s_strCMMCMD[CDMA_IPSENDEX], nLen ) == 0 ){
	
 		return CDMA_IPSENDEX;	
 	}
 	//
 	for( nCnt = 0; nCnt < CMM_CMD_END; nCnt++ ){		
		nLen = strlen( s_strCMMCMD[nCnt] );
	 	if( memcmp( pMsg, s_strCMMCMD[nCnt], nLen ) == 0 ){
	 		
	 		pTmp = (unsigned char*)strchr( (char*)pMsg, ':');
	 		if( pTmp ){
	 			pTmp++;
	 			//strcpy( (char*)pMsg, (char*)pTmp );
	 			memcpy( pMsg, pTmp, nDataLen-(nLen+1) );
	 		}	 		
	 		break;		 		
	 	}	 	
	}
	return nCnt;
}

/////////////////////////////////////////////////////////////////////////
// ��⿨�Ƿ����
unsigned char CMM_CheckSIMReady( ){ 
		
	unsigned char 	nResult = GSM_NG;
	unsigned char 	nReturnMsg[20];
	
	if( CMM_ExecATCmd( CDMA_CPIN, (unsigned char *)TEXT_QUESTION, GSM_ATTIME_CHECK, CDMA_CPIN, CDMA_CME, nReturnMsg ) == GSM_SUCCEED ){
		if( strstr( (char*)nReturnMsg, (char*)"READY" ) != NULL ){
			 nResult = GSM_SUCCEED;
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
	nResult = CMM_ExecATCmd(CDMA_CDV, nBuf, GSM_ATTIME_CHECK, CDMA_ORIG, CDMA_CME, NULL);	
	if( nResult == GSM_SUCCEED ){
		CMM_SetCallOnLine( );		
	};
	return nResult;
}
//////////////////////////////////////////////////////////////
// �ӵ绰
unsigned char CMM_AcceptCall( void ){
	
	return CMM_ExecATCmd(CDMA_A, NULL, GSM_ATTIME_CHECK, CDMA_OK, CDMA_CME, NULL);	
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
	CMM_ExecATCmd( CDMA_HSMSSS, (unsigned char*)"=0,0,1,0", GSM_ATTIME_CHECK, CDMA_OK, CDMA_CMS, NULL );								 
	if( nLen > 157 ){
		nLen = 156;
	}

	return CMM_SendSMS( pCaller, pMsg, nLen, CODE_ASCII );
}
//////////////////////////////////////////////////////////////
// OCTET
unsigned char CMM_SendOctetData( unsigned char *pCaller, 
								 unsigned char *pMsg, 
								 unsigned char nLen ){		
	
	//  ���ö��ŷ��ͷ�ʽ
/*	CMM_ExecATCmd( CDMA_HSMSSS, (unsigned char*)"=1,0,3,0", GSM_ATTIME_CHECK, CDMA_OK, CDMA_CMS, NULL );								 
	if( nLen > 137 ){
		nLen = 137;
	}	
	return CMM_SendSMS( pCaller, pMsg, nLen, CODE_OCTET);
*/	
	CMM_ExecATCmd( CDMA_HSMSSS, (unsigned char*)"=0,0,6,0", GSM_ATTIME_CHECK, CDMA_OK, CDMA_CMS, NULL );								   
	if( nLen > 140 ){
		nLen = 140;
	}
	return CMM_SendSMS( pCaller, pMsg, nLen, CODE_UNICODE);
}
//////////////////////////////////////////////////////////////
// unicode
unsigned char CMM_SendUnicodeData( unsigned char *pCaller, 
								   unsigned char *pMsg, 
								   unsigned char nLen ){																 	 	

	//  ���ö��ŷ��ͷ�ʽ
	CMM_ExecATCmd( CDMA_HSMSSS, (unsigned char*)"=1,0,6,0", GSM_ATTIME_CHECK, CDMA_OK, CDMA_CMS, NULL );								   
	if( nLen > 140 ){
		nLen = 140;
	}
	return CMM_SendSMS( pCaller, pMsg, nLen, CODE_UNICODE);											 	
}
///////////////////////////////////////////////////////////////
// ���� CDMA����
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
							unsigned char nDataLen,
							unsigned char nCodeType ){

	unsigned char 	nResult  = GSM_NG; 
	unsigned short	nShortLen = 0;
	unsigned char 	err 	 = 0;	
	unsigned char 	nCnt 	 = 0;
	unsigned char 	nCmdType = 0;
	unsigned short	nTimeout = 0;
	unsigned char	  *lpLargeBuf = NULL;
	
	
	lpLargeBuf	= lpLargeBuf4;	
	
	CMM_SetSendSMSUnReady( );	
	sprintf((char*)lpLargeBuf, "=\"%s\"", pCaller);
	CMM_SendATCmd( CDMA_HCMGS, 1, lpLargeBuf );	
	nTimeout = GSM_SENDSPACE_CHECK;
	while ( 1 ){
		OSSemPend(pGSMReceiveSem, nTimeout, &err);
						
		if( err == OS_TIMEOUT ){	
			if( CMM_IsSendSMSReady( ) ){						
				CMM_SetSendSMSUnReady( );			
				GSMUARTSendData(pMsg, nDataLen);
				if( nCodeType == CODE_UNICODE ){
					GSMUARTPutch(0x00);
					GSMUARTPutch(0x1A);
				}
				else{
					GSMUARTSendStr((unsigned char*)TEXT_SMSSEND_END);
					GSMUARTSendStr((unsigned char*)TEXT_LF_RN);
				}	
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
			nShortLen = CMM_ReadCmd( lpLargeBuf , GSM_RECMAXBUFFER );
			nCmdType = CMM_GetCmdTypeEx(lpLargeBuf, nShortLen);		 	
			if(nCmdType ==  CDMA_HCMGSS){ 
				nTimeout = GSM_SENDSPACE_CHECK; 				
				nResult = GSM_SUCCEED;
				nCnt = 0;
			} 	
			else if( nCmdType == CDMA_CMS ){
				nResult = GSM_NG;	
				break;
			}
			else if( nCmdType == CDMA_HCMGSF ){
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
unsigned char IP_SendData(  unsigned char nMode,
															   unsigned char *pBuf, 
															   unsigned char nLen){
			
	unsigned char  		nResult = GSM_NG;	
	unsigned char		nCnt = 0;
	unsigned short		nTimeOut = 0;
	unsigned char  		err = 0;
	unsigned char  		nCmdType = 0;
	unsigned short	nShortLen = 0;
	unsigned char		*lpLargeBuf = NULL;
	
	
	lpLargeBuf	= lpLargeBuf4;	
			
	if ( nMode ){ //TCP
		sprintf((char*)lpLargeBuf, "%s%s=1,1,\"", (unsigned char*)TEXT_AT,  (unsigned char*)s_strCMMCMD[CDMA_IPSENDEX]);
	}
	else{         //UDP
		sprintf((char*)lpLargeBuf, "%s%s=2,1,\"", (unsigned char*)TEXT_AT, (unsigned char*)s_strCMMCMD[CDMA_IPSENDEX] );
	}
	
	GSMUARTSendStr( (unsigned char*)lpLargeBuf );
	for (nCnt = 0; nCnt < nLen; nCnt++){
		sprintf( (char *)lpLargeBuf, "%X", pBuf[nCnt] >> 4 );
		GSMUARTPutch( lpLargeBuf[0] );
		sprintf( (char*)lpLargeBuf, "%X", pBuf[nCnt] & 0x0f);
		GSMUARTPutch( lpLargeBuf[0] );
	}
	GSMUARTSendStr((unsigned char *)"\"\r\n");	
				
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
				nShortLen = CMM_ReadCmd( lpLargeBuf , GSM_RECMAXBUFFER);		
				nCmdType = CMM_GetCmdTypeEx(lpLargeBuf, nShortLen);	
				if( nCmdType == CDMA_IPSENDEX )	{
					nResult = GSM_SUCCEED;
					break;
				}
				else if( nCmdType == CDMA_CME ){
					 nResult = GSM_NG;
					 break;
				}
				else if( nCmdType == CDMA_ERROR ){
					 nResult = GSM_NG;
					 break;
				}					
				else{
					 CMM_ParseMasterCmd( lpLargeBuf, nCmdType );
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
//			  	  1: OCT
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


   sprintf((char*)nCmdBuf, "=%d", nCnt );
   CMM_SendATCmd( CDMA_HCMGR, 0, nCmdBuf );
   // ��ȡ���ű�ͷ	
	//        <callerid> <date>		 <time>   <lang><fmt><length><prt���ȼ�><prv���ܼ�><type��������><stat��������>
	//^HCMGR:13480924411,2011,06,08,16,24,57,  0,   1,     9,      0,        0,           0,  			 1
	//*VERSION#
	// 1: ascii  (length:0~160)(���轫CDMA������ascii)
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
				nCmdType = CMM_GetCmdType( pMsg+nLen, nCntTmp );
				if ( nCmdType == CDMA_HCMGR ){
					CMM_ParseSMSFormat( pCaller, pMsg+nLen,  pCodeType, &nRecvLen );									
					nHsmsrd = 1;						
					nTimeOut = GSM_SENDSPACE_CHECK;
				}	
				// �յ�OK����֮ǰ���յ����ݣ����Ϸ���
				else if( nCmdType == CDMA_OK ){
					if((nLen > 0) && (nHsmsrd == 1) ){
						 break;
					}
				}
				else if( (nCmdType == CDMA_CME) ||
								 (nCmdType == CDMA_CMS)	){
					break;				
				}
				else if( nCmdType == CMM_CMD_END ){
								
				   if ( (nHsmsrd == 1) && (nCntTmp > 0) ){				   								
						// ASCII
						nLen = nCntTmp;
								
						nTimeOut = GSM_SENDSPACE_CHECK;
					}
			   }
			   else{
				 CMM_ParseMasterCmd( pMsg+nLen, nCmdType );
			   }
			}
	 }
	 //�����յ�����,�����0x1A,0x00,0x1A
	 if( nLen > 0 ){
		 switch( *pCodeType ){
		 case 0:	//ASCII
		 case 1:	//OCTET
		 	if( pMsg[nLen-1] == 0x1A ){
		 		nLen -= 1;
		 	} 
		 	break;
		 case 2:	// UNICODE
		 	if( pMsg[nLen-1] == 0x1A ){
		 		nLen -= 2;
		 	} 
		 	break;
		 }
	 }
	 return  nLen;
}
//  ���Ͷ��Ÿ�ʽ
///////////////////////////////////////////////////
// ^HCMGR:13480924411,2011,06,08,16,24,57,0,1,9,0,0,0,1
unsigned char CMM_ParseSMSFormat( unsigned char *pCaller, 
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

///////////////////////////////////////////////////////////////
//  ģ����Ƶ
unsigned char	CMM_ConfigCBand( void ){

	return 0;
}
/////////////////////////////////////////////////////////////
// ����:				
// +CLCC:2,1,4,0,0,"13480924411",129
unsigned char GSM_ReadCaller( unsigned char *pCaller ){

	unsigned char	nResult = GSM_NG;
	
	if( CMM_ExecATCmd( CDMA_CLCC, NULL, GSM_ATTIME_CHECK, CDMA_CLCC, CDMA_CME, pCaller ) == GSM_SUCCEED ){
		
	 	return GSM_SUCCEED;	
	}			
	return nResult;
}
/////////////////////////////////////////////////////////////
// �������洢������Ϣ
void CMM_ParseMasterCmd(unsigned char *pMsg, unsigned char nCmdType){ 
	
	unsigned char		nCnt = 0;
	unsigned char		nFlag = 0;
	unsigned short		nLen;
	unsigned char 		nCaller[TELNUMBER_LENGTH+5]; 
	unsigned char		*pTmp = NULL;
	unsigned char		*pch = NULL;
	

	switch(nCmdType){		
	case CDMA_CLIP:
		nCaller[0] = 0x00;
		CMM_ReadCallerCLIP(pMsg, nCaller);
		if( strlen( (char*)nCaller) > 0 ){
					
			if( IsAuthorizationNo( (unsigned char*)nCaller) ){						
						
				if(memcmp("669475", nCaller, PHONE_COMPARE_BIT) == 0 ){
										
					CMM_AcceptCall( );		
					CMM_SetCallOnLine( );
					g_stGSMState.m_nDialLatLng = 0;
					HW_PORT_SPKMUTE_OPEN(); // �򿪾���
				}
				else{				
					if( g_stGSMState.m_nDialLatLng++ >= 3 ){
					
						g_stGSMState.m_nDialLatLng = 0;
						CMM_AcceptCall( );		
						CMM_SetCallOnLine( );
					}					
					// ȡ�绰����
					nCnt = strlen( (char*)nCaller);							
					memcpy( g_stGSMState.m_strDialNo, nCaller, nCnt );
					g_stGSMState.m_strDialNo[nCnt] = 0x00;
					
					#ifdef DIAL_CALL
					HW_PORT_SPKMUTE_OPEN(); // �򿪾���
					#else	
					HW_PORT_SPKMUTE_CLOE(); // �رվ���	
					#endif 
				}
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
				
	case CDMA_IPDATA:	
		//ͨ��
		pTmp = (unsigned char*)strchr((char*)pMsg, ',');
		if( pTmp != NULL ){
			
			pch = pTmp+1;
			// ����
			pTmp = (unsigned char*)strchr((char*)pch, ',');
			if( pTmp != NULL ){
			
				*pTmp = 0x00;
				nLen = atoi( (char*)pch );
				pch = pTmp + 1;
				memcpy( pMsg, pch, nLen );
				nFlag = 1;
			}			
		}			
		//nLen = CMM_IP_RecvData( pMsg, pMsg, NULL ); 		
		if( nFlag == 1 ){
			nLen = PL_ParseData( pMsg, pMsg, nLen );	
			
			if( TE_IsResultGGAddrFlag() ){				
				TE_ClrResultGGAddrFlag();
				// �����ѯ��־
				TE_ClrQueryGGAddrFlag();			
				OSTimeDlyHMSM(0,0,1,0);			
				if( nLen ){
					nCnt = 0;
					while( nCnt++ < 3 ){
						if( GSM_IsLangState() ){ //����
					 		if( CMM_SendUnicodeData( g_stGSMState.m_strForward, pMsg, nLen) == GSM_SUCCEED ){
								break;
							}
						}
						else{					 					//Ӣ��
							if( CMM_SendASCIIData( g_stGSMState.m_strForward, pMsg, nLen) == GSM_SUCCEED ){
								break;
							}
						}
						OSTimeDlyHMSM(0,0,2,0);
					}	
				}				
			}
			else{	
				if( nLen ){
					IP_SendData( IP_IsTCPCommMode(), pMsg, nLen );
				}
			}
		}
		break;
		
	// ����Ͽ�
	// ��
	// ͨ������
	// ͨ�����
	// ���˽���
	case CDMA_CEND:		
		HW_PORT_SPKMUTE_CLOE();
		CMM_SetCallOffLine( );
		//g_stGSMState.m_nDialLatLng = 0;
		break;		
	//	��������
	case CDMA_TECNT:
		CMM_SendTECNT();
		nLen = PL_ReadConfig( pMsg );		
		if( nLen > 0 ){	
			sprintf( (char*)nCaller, "AT%s:", s_strCMMCMD[CDMA_TESETUP] );		
			GSMUARTSendStr( nCaller );
			GSMUARTSendData( pMsg, nLen );
			GSMUARTSendStr( (unsigned char*)"\r\n" ); 
		}
		break;
	//	�Ҷ�����
	case CDMA_TEDISCNT:
		CMM_SendDISCNT();
		break;
	//	l��ȡ/���� �豸����
	case CDMA_TESETUP:
		if( *pMsg == '?' ){
			nLen = PL_ReadConfig( pMsg );
		}
		else{
			nLen = PL_ParseConfig(pMsg, pMsg );
			nCnt = 1;			
		}
		if( nLen > 0 ){	
			sprintf( (char*)nCaller, "AT%s:", s_strCMMCMD[CDMA_TESETUP] );		
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
//	�������ӳɹ�,�ر�CDMAģ���Դ
void	CMM_SendTECNT( void ){

	GSM_SetSetupConfig();
	CMM_ExecATCmd(CDMA_TECNT, NULL, GSM_SENDSPACE_CHECK, CDMA_OK, CDMA_CME, NULL );
}
//////////////////////////////////////////////////////////////
//	���ͶϿ�����,����CDMAģ���Դ
void 	CMM_SendDISCNT( void ){

	CMM_ExecATCmd( CDMA_TEDISCNT, NULL, GSM_SENDSPACE_CHECK, CDMA_OK, CDMA_CME, NULL );
	TE_SetNeedTEReset();
	GSM_ClrSetupConfig();
}
/////////////////////////////////////////////////////////////////////
// ���յ�GPRS����
// ^IPDATA:<���Ӻ�> <���ݳ���>, <�յ���������Ϣ>
// ^IPDATA:2,15,<��������>
// <CR><LF>
// 1 0��(0x31,0x20,0x30,0x89)
unsigned char CMM_IP_RecvData(  unsigned char  *pMsg,
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
		pMsg = pch+1;
		
		//���ݳ���
		pch = (unsigned char*)strchr( (char*)pMsg, ',');
		if( pch == NULL){
			return nDataLen;
		}
		*pch = 0x00;
		nDataLen = atoi( (char*)pMsg);
		
		//�ᴦGPRS����
		pMsg = pch+1;
		memcpy( pDest, pMsg, nDataLen );
		pDest[nDataLen] = 0x00;
		
		// ���ݳ���
		//nDataLen = atoi( (char*)pch );
		//OSTimeDlyHMSM(0,0,0,nDataLen*10);
		//CMM_ReadCmd((unsigned char *)pDest, nDataLen);
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
//+CLIP:"13480924411",129,,,,0  
void CMM_ReadCallerCLIP( unsigned char *pMsg, unsigned char *pCaller ){
	
	unsigned char	*pch = NULL;
		
	pch = (unsigned char*)strchr( (char*)pMsg, '"');
	if( pch == NULL ){
		return ;
	}
	pMsg = pch + 1;
	pch = (unsigned char*)strchr((char*)pMsg, '"');
	if( pch == NULL){
		return ;
	}
	*pch = 0x00;
	strcpy( (char*)pCaller, (char*)pMsg );
	
	return;
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

#endif //HW_MC323