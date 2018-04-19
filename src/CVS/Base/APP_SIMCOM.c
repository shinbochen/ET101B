/////////////////////////////////////////////////////////////
//file name: APP_SIMCOM.H
//     Date: 2009-6-11    
//	 Author: wyb	 
// optimize: real
// op_date : 2009-6-11
/////////////////////////////////////////////////////////////
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
#include "..\hdr\APP_SIMCOM.h"
#include "..\hdr\ProtocolA.h"
#include "..\hdr\ProtocolB.h"
#include "..\hdr\ProtocolC.h"
#include "..\hdr\ProtocolASICO.h"


#ifdef			HW_SIMCOM

unsigned char	g_nNetWork = 0;
//=====================================================
// SIM300
const char	*s_strCMMCMD[]={
			"OK",
			"E0",				//�رջ���ʾ
			"E1",				//��������
			"+CMEE",		//
			"+CFUN",		//set phone functionality
			"+CSQ",		  //��GSM�ź�		 		
			"+CREG",		//��ѯGSM����
			"+CGREG",		//��ѯGPRS����
			"+CPIN",		//�����
			"+IPR",			//���ò�����
			"+GSN",			//IMEI
			"+CMUT",    //�������
			"+CLVL",		//��������
			
			"+SIDET",		//���������ȴ�С
			"+CMIC",			//�ı���˷������
			"+CHFA",			//0/1�����������л�������Ƶͨ��
			"+IFC",			// �ر�Ӳ����
						
			//�绰ָ��
			"+CLCC",    //��ѯ�������
			"RING",
			"D",
			"A",
			"H",
			"NO CARRIER",
			"BUSY",
		  	"NO ANSWER",			
		  	"SHUT OK",		//�Ͽ�PDP�ɹ�
			"+CUSD",		//�ǽṹ������ҵ��
			
			//����ָ��
			"+CSMP",		//���͵ı��뷽ʽ,,,0			
			"+CSCA",		//��ȡ���ĺ���
			"+CMGF",		//���Ÿ�ʽ
			"+CSDH",		//��ʾ�ı���ʽ
			"+CNMI",		//���ö�����ʾ
			"+CMTI",		//�յ�������ʾ	
			"+CMGR",		//��ȡ����
			"+CMGS",		//���Ͷ���
			"+CMGD",		//ɾ������
			
			//����ע��			
			"+CIPDPDP",		//PDP����
			"+CIPSHUT",		//�ر�PDP����
			"+CGATT",		//ע��GPRS����
			"+CGDCONT", 	//PDP����
			"+CGACT",		//�����ƶ����� 
			"+CSTT",		//����APN��USER NAME, PASD WORD
			"+CIICR",		//�����ƶ�����		
			"+CIFSR",		//��ȡ����IP
			"+CIPSTART",	//���ӷ�����	
			"+CIPSEND",		//����UDP��TCP����
			"+CIPCLOSE",	//�Ͽ�TCP/UDP����
			"+CIIPSHUT",	//�ر�GPRS PDP����
			"+CIPHEAD", 	//����GPRS����
			"+IPD",			//����GPRS����
			"$SLEEP",		//�ر���Ϣ״̬
			"CONNECT OK",	//���ӷ������ɹ�
			"CONNECT FAIL",//���ӷ�����ʧ��
			"SEND OK",
			"SEND FAIL",
			"CLOSE OK",
			"CLOSED",
			"ERROR",
			"+CME ERROR",
			"+CMS ERROR",
			"STATE",		//
			"+CBAND",		//ģ����Ƶ
			
		//	"+CDNSCFG",
		//	"+CDNSORIP",
			
			"+TECNT", 		//��������
			"+TEDISCNT",	//�Ҷ�����			
			"+TESETUP",     //l	��ȡ/���� �豸����
			"+GMR",			//��汾��			
};


extern  OS_EVENT 			*pGSMReceiveSem;


///////////////////////////////////////////////////////////
// GSMӲ����ʼ��
void 	 CMM_HWInit( void ){
	
	GSMUARTInit(9600);		// GSM ����     

	// GSMģ�鿪������
	GPIO_Enable(HW_PORT_WARMPOWER);			
	GPIO_SetDirection(HW_PORT_WARMPOWER, HW_PORT_OUTPUT);		
	HW_GSM_WARM_POWERON( );	
	
	#ifdef HW_COLDRESTART_GSM
	GPIO_Enable(HW_PORT_COLDPOWER);			
	GPIO_SetDirection(HW_PORT_COLDPOWER, HW_PORT_OUTPUT);			
	HW_GSM_COLDPOWEROFF();
	#endif
}
///////////////////////////////////////////////////////////
//	ͨ��ģ��Ӳ������	
//------   �ػ�(1~2) ---------------     ����(>2)   ---------------
//      |           |  ��(2~8)      |               |
//      ------------                -----------------
void 	 CMM_ReStart( void ){
			
	#ifdef HW_COLDRESTART_GSM
	OSTimeDlyHMSM(0,0,0,500);
	HW_GSM_COLDPOWEROFF();
	OSTimeDlyHMSM(0,0,5,0);	
	HW_GSM_COLDPOWERON();	
//  �ߵ�
	HW_GSM_WARM_POWERON( );				//H
	OSTimeDlyHMSM(0,0,0,500 );	
	#else
//  �ߵ�
	HW_GSM_WARM_POWERON( );				//H
	OSTimeDlyHMSM(0,0,0,500 );	
//  off module
	HW_GSM_WARM_POWEROFF( );				//L	
  	OSTimeDlyHMSM(0,0,0,800 );
	HW_GSM_WARM_POWERON( );	
	OSTimeDlyHMSM(0,0,5,0);	
	#endif		
// poweron module
	HW_GSM_WARM_POWEROFF( );	
	OSTimeDlyHMSM(0,0,2,0);	
	HW_GSM_WARM_POWERON( );		
}
///////////////////////////////////////////////////////////
//��ʼ��GSMģ��
void 	 CMM_Init(void){

	int 	nCnt = 0;
		
	nCnt = 0;	
	while( (CMM_ExecATCmd( GSM_ATE0, NULL, GSM_SENDSPACE_CHECK, GSM_OK, GSM_ERROR, NULL ) != GSM_SUCCEED) && 
		   (nCnt++ < 10) ){
		OSTimeDlyHMSM(0,0,1,0);
	}	
	//��汾
	//CMM_ExecATCmd( GSM_GMR, NULL, GSM_SENDSPACE_CHECK, GSM_OK, GSM_ERROR, NULL );
	//��⿨�Ƿ����
	nCnt = 0;
	while( ( GSM_CheckSIMReady() != GSM_SUCCEED) && 
		   ( nCnt++ < 10 ) ){		
		OSTimeDlyHMSM(0,0,1,0);
	}
	
	#ifndef SW_PROTOCOL_ASICO
	if( strlen( (char*)g_stGSMSetup.m_strUID ) <= 0 ){ 
		TE_SetNeedWriteFlash( );
	}
	// �ն����к�
	nCnt = 0;
	while( ( CMM_QueryModuleSN(g_stGSMSetup.m_strUID) != GSM_SUCCEED )&&
		   ( nCnt++ < 10 ) ){
		OSTimeDlyHMSM(0,0,1,0);
	}	
	#endif //SW_PROTOCOL_ASICO
	
	//	�������ĺ���
	nCnt = 0;	
	while( ( GSM_QuerySMC(g_stGSMSetup.m_strSMCNo) != GSM_SUCCEED )&&
		   ( nCnt++ < 30 ) ){
		OSTimeDlyHMSM(0,0,1,0);
	}	
	//  �ر����߹���	
	//CMM_ExecATCmdN( GSM_SLEEP, 0, GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL );
	//  ȫ����ģʽ
	#ifdef 	SIMCOM_340
	CMM_ExecATCmdN( GSM_CFUN, 1, GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL );		
	//	�������	
	CMM_ExecATCmdN( GSM_CMUT, 0, GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL );	
	#endif	
	//  ������Ƶ���
	CMM_ExecATCmdN( GSM_CLVL, 99, GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL );	
	//  ���������ȴ�С
	#ifdef SIMCOM_340
	CMM_ExecATCmdN( GSM_SIDET, 0, GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL );	
	#else
	CMM_ExecATCmd( GSM_SIDET, (unsigned char*)"=0,0", GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL );
	#endif
	//	�ı���˷������
	CMM_ExecATCmd( GSM_CMIC, (unsigned char*)"=0,12", GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL );
	//0/1�����������л�������Ƶͨ�� 
	CMM_ExecATCmdN( GSM_CHFA, 0, GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL );
	//  ���Ÿ�ʽ	 PDU��ʽ
	CMM_ExecATCmdN( GSM_CMGF, 0, GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL );
	//  ��ʾ�ı���ʽ�µĲ���
	CMM_ExecATCmdN( GSM_CSDH, 1, GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL );
	//  ɾ��ȫ������
	CMM_DeleteAllSMS();  
	//  ���ö�����ʾ	
	while( CMM_ExecATCmd( GSM_CNMI, (unsigned char*)"=2,1", GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL ) != GSM_SUCCEED){
		OSTimeDlyHMSM(0,0,1,0);
	}	
	CMM_ExecATCmd( GSM_CIPHEAD, (unsigned char*)"=1", GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL );
	
	#ifdef SIMCOM_900	
	CMM_ExecATCmd(GSM_CREG, (unsigned char*)"=0", GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL );	
	#else
	CMM_ExecATCmd(GSM_CREG, (unsigned char*)"=2", GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL );	
	#endif
	// ����ι��
	TE_ClrNeedTEReset();
	//  ��Ƶ����
	CMM_ConfigCBand();
	return;
}
/////////////////////////////////////////////////////////////
//��ʼ����������	
unsigned char CMM_IPInit( unsigned char *pStrGateway, unsigned char *pUser, unsigned char *pPsd ){
	
		
	CMM_IPClose();
	CMM_CIPSHUT();
	CMM_ExecATCmd( GSM_CGATT, (unsigned char*)"=1", GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL );	
	
	#ifndef SIMCOM_900
	CMM_ExecATCmd( GSM_CIPDPDP, (unsigned char*)"=1", GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL );						
	#endif
	GPRS_SetUserPsd( pStrGateway,pUser, pPsd);
	CMM_ExecATCmd( GSM_CIICR, NULL, GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL );
	CMM_ExecATCmd( GSM_CIFSR, NULL, GSM_SENDSPACE_CHECK, GSM_OK, GSM_ERROR, NULL );			
	CMM_ExecATCmd( GSM_CIPHEAD, (unsigned char*)"=1", GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL );
		
	return 1;
}
/////////////////////////////////////////////////////////////////
// ��������������
unsigned char GSM_ParseDNS( unsigned char *pStrHost, unsigned char *pStrIP ){

	return GSM_NG;
}
/////////////////////////////////////////////////////////////////
//	PDP�����ļ���
void	CMM_ActivationPDP( void ){

	CMM_ExecATCmd( GSM_CGATT, (unsigned char*)"=1", GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL );
	//CMM_ExecATCmd( GSM_CGACT, (unsigned char*)"=1,1", GPRS_GPRSDATA_CHECK, GSM_OK, GSM_ERROR, NULL );	
}
/////////////////////////////////////////////////////////////////
// ����ATָ���ʽ
// ������������, ��ȡ���������
unsigned char CMM_GetCmdType(unsigned char *pMsg) 	{
	
	unsigned char 	nCnt = 0;
	unsigned char		nLen = 0;
	unsigned char 	*pTmp = NULL;
	
	for( nCnt = 0; nCnt < CMM_CMD_END; nCnt++ ){
		
		nLen = strlen( s_strCMMCMD[nCnt] );
	 	if( memcmp( pMsg, s_strCMMCMD[nCnt], nLen ) == 0 ){
	 		
	 		if( GSM_IPD != nCnt ){
		 		pTmp = (unsigned char*)strchr( (char*)pMsg, ':');
		 		if( pTmp ){
		 			pTmp++;
		 			strcpy( (char*)pMsg, (char*)pTmp );
		 		}	
		 	} 		
	 		break;		 		
	 	}	 	
	}
	return nCnt;
}
///////////////////////////////////////////////////////////////
// ��Ч����1
// ��Ч����0
unsigned char GSM_CheckSIMReady( ){
	
	unsigned char 	nResult = 0;
	unsigned char 	nReturnMsg[20];
	
	if( CMM_ExecATCmd( GSM_CPIN, (unsigned char *)TEXT_QUESTION, GSM_ATTIME_CHECK, GSM_CPIN,GSM_ERROR, nReturnMsg ) == GSM_SUCCEED ){
		if( strstr( (char*)nReturnMsg, (char*)"READY" ) != NULL ){
			nResult = 1;
		}		
	}
	return nResult;
}
//////////////////////////////////////////////////////////
//
unsigned char CMM_QueryModuleSN( unsigned char *pStr ){		
	return CMM_ExecATCmd( GSM_CGSN, NULL, GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, pStr );
}
///////////////////////////////////////////////////////////////
// ��绰
unsigned char CMM_DialNumber( unsigned char *pStrNo ){
	
	unsigned char		nBuf[50];
	unsigned char 	nResult;
	
	OSTimeDlyHMSM(0,0,3,0 );
	sprintf((char*)nBuf, "%s;", (char*)pStrNo ); 
	nResult = CMM_ExecATCmd(GSM_ATD, nBuf, GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL);	
	if( nResult == GSM_SUCCEED ){
		CMM_SetCallOnLine( );				
	}
	
	return nResult;
}
///////////////////////////////////////////////////////////////
//  ģ����Ƶ
unsigned char	CMM_ConfigCBand( void ){

	unsigned char	nResult = GSM_NG;	
	
	#ifdef	GSM_CBAND_9001800
		nResult = CMM_ExecATCmd(GSM_CBAND, (unsigned char*)"=\"EGSM_DCS_MODE\"", GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL );
	#endif
	#ifdef  SIMCOM_CBAND_8501900
		nResult = CMM_ExecATCmd(GSM_CBAND, (unsigned char*)"=\"GSM850_PCS_MODE\"", GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL );
	#endif
	
	return nResult;
}	
///////////////////////////////////////////////////////////	
// ��ѯGSM����״̬
// 0,2,3,4: δע��
// 1:				ע�ᣬ����
// 5:				ע��, ���
// ��Ч����1 ��Ч����0
unsigned char CMM_QueryNetworkStatus( void)	{
	
	unsigned char 	nBuf[40];
	unsigned char 	nState = 0;	
	unsigned char 	*pTmp = NULL;
	
	memset( (char *)nBuf, 0, 2 );
	
	CMM_ExecATCmd(GSM_CGREG, (unsigned char*)TEXT_QUESTION, GSM_ATTIME_CHECK, GSM_CGREG, GSM_ERROR, nBuf);	
				
	pTmp = (unsigned char*)strchr( (char*)nBuf, ',' );
	if( pTmp ){
		pTmp++;
		nState = atoi( (char*)pTmp );
	}
	return nState;
}
///////////////////////////////////////////////////////////	
//	��ѯ��ַС��ID  +CREG: 2,1,"2483","0E9D"
unsigned char	CMM_QueryNetworkLac( int	*pLac, int	*pID ){

	unsigned char	nBuf[30];
	unsigned char	*pch = NULL;
	unsigned char	*pch1 = NULL;
	char			*pTmp = NULL;
		
	memset( nBuf, 0x00, 30);	
	if( CMM_ExecATCmd(GSM_CREG, (unsigned char*)"?", GSM_ATTIME_CHECK, GSM_CREG, GSM_ERROR, nBuf ) == GSM_SUCCEED ){
		
		pch = (unsigned char*)strchr((char*)nBuf, '"');
		if( pch == NULL){
			return 0;
		}
		pch++;	
		pch1 = (unsigned char*)strchr((char*)pch, '"');
		if( pch1 == NULL ){
			return 0;
		}
		*pch1 = 0x00;
		*pLac = strtol((char*)pch, &pTmp, 16);
		pch = pch1 + 1;
		pch1 = (unsigned char*)strchr((char*)pch, '"');
		if( pch1 == NULL ){
			return 0;
		}  		
		pch = pch1 + 1;
		*pID = strtol( (char*)pch, &pTmp, 16 );
	}
	return 0;
}
//////////////////////////////////////////////////////////////
// �ӵ绰
unsigned char CMM_AcceptCall( void ){	
	return CMM_ExecATCmd(GSM_ATA, NULL, GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL);	
}
//////////////////////////////////////////////////////////////
//
unsigned char CMM_RejectCall( void ){
	return CMM_ExecATCmd(GSM_ATH, NULL, GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL);		
}
//////////////////////////////////////////////////////////////
//  ��ѯIMEI���
unsigned char	CMM_QueryIMEI( unsigned  char	*pDest ){
	
	strcpy( (char*)pDest, (char*)g_stGSMSetup.m_strUID );
	
	return GSM_SUCCEED;	
} 
/////////////////////////////////////////////////////////////
// ����:				0�� free
//						1:  on use
// +CLCC : 1,1,4,0,0,"13902288001",161
unsigned char GSM_ReadCaller( unsigned char *pCaller ){
	unsigned char			nBuf[50];
	unsigned char			nResult = 0;
	unsigned char 			*pTmp = NULL;
	
	if( CMM_ExecATCmd(GSM_CLCC, NULL, GSM_ATTIME_CHECK, GSM_CLCC, GSM_ERROR, nBuf) == GSM_SUCCEED ){
		nResult = 1;
		
		pTmp = (unsigned char*)strchr( (char*)nBuf, '"' );
		if( pTmp != NULL ){
			pTmp++;
			strcpy( (char*)nBuf, (char*)pTmp );
			
			pTmp = (unsigned char*)strchr( (char*)nBuf, '"' );
			if( pTmp != NULL && pCaller != NULL){
				memcpy( (char*)pCaller, (char*)nBuf, pTmp-nBuf );
				pCaller[pTmp-nBuf] = 0x00; 
			}
		}		
	}
	return nResult;	
}
/////////////////////////////////////////////////////////////
// ��ȡ�������ĺ���
// 
unsigned char GSM_QuerySMC( unsigned char *pStr ){
	unsigned char 	nBuf[50];
	unsigned char		nResult = GSM_NG;
	unsigned char		*pTmp = NULL;
	unsigned char 	nTmp = 0;
		
	if( CMM_ExecATCmd( GSM_CSCA, (unsigned char *)TEXT_QUESTION, GSM_ATTIME_CHECK, GSM_CSCA, GSM_CMS_ERROR, nBuf ) == GSM_SUCCEED ){
		
		pTmp = (unsigned char*)strchr( (char*)nBuf, '"' );
		if( pTmp != NULL ){
			pTmp++;
			strcpy( (char*)nBuf, (char*)pTmp );
			
			pTmp = (unsigned char*)strchr( (char*)nBuf, '"' );
			if( pTmp != NULL ){
				nResult = GSM_SUCCEED;
				nTmp = pTmp-nBuf;
				memcpy( (char*)pStr, (char*)nBuf, nTmp );
				*(pStr+nTmp) = 0x00;
			}			
		}
	}
	return nResult;
}
/////////////////////////////////////////////////////////////////
// ��������������
//unsigned char GSM_ParseDNS( unsigned char *pStrHost, unsigned char *pStrIP ){
	
	//unsigned char			nBuf[50];	
	//sprintf((char*)nBuf, "=\"%s\"", pStrHost );	
	//return CMM_ExecATCmd( GSM_DNSR, nBuf, GSM_ATTIME_CHECK, GSM_DNSR, GSM_ERROR, pStrIP );
//	return 1;
//}
/////////////////////////////////////////////////////////////////
//  ��ѯ����
unsigned char	GSM_ExecCUSD( unsigned char	*pDest ,unsigned char	*pSrc, unsigned short	nTimeout ){	

	unsigned char 	nResult = GSM_NG;
	unsigned char	nFlag = 0 ;
	unsigned char	nCnt = 0;
	unsigned char 	nError = 0;
	unsigned char 	nType = 0;
	unsigned char	*lpLargeBuf = NULL;
	
	
	lpLargeBuf	= lpLargeBuf4;	
	sprintf( (char*)lpLargeBuf, "=1,\"%s\",15", pSrc );
	CMM_SendATCmd( GMS_CUSD, 0, lpLargeBuf );
	
	nCnt = 0;
	while ( 1 ){		
		OSSemPend(pGSMReceiveSem, nTimeout, &nError);
		if( nError == OS_TIMEOUT ){
			if ( nCnt++ >= 4 ){
				break;
			}
		}
		else{ 		
			CMM_ReadCmd( lpLargeBuf , GSM_RECMAXBUFFER );			
			nType = CMM_GetCmdType(lpLargeBuf);	
			if( nType == GMS_CUSD ){
				if( pDest  && (strlen((char*)lpLargeBuf) > 0 )){
					strcpy( (char*)pDest, (char*)lpLargeBuf );						
				}
				nTimeout = GSM_LOWAIT_TIME;
				nFlag = 1;
				nResult = GSM_SUCCEED;				
			}
			else if( nType == GSM_ERROR ){			
				nResult = GSM_NG;
				break;
			}
			else if( nType == GSM_OK ){
				if( nFlag == 1 ){
					break;
				}
				else{
					nTimeout = GSM_ATTIME_CHECK;
				}
			}			
			else{
				CMM_ParseMasterCmd(lpLargeBuf, nType);				
			}	
		}				
	}	
	return nResult;
}
////////////////////////////////////////////////////////////////////
// ��ѯGPRS����
// ��������״̬
// ��Ч����1
// ��Ч����0
unsigned char CMM_QueryIPNetworkStatus(void)	{
	
	unsigned char 	nBuf[40];
	unsigned char	nState = 0;
	unsigned char 	*pTmp = NULL;
	
	memset( (char*)nBuf, 0, 2 );
	CMM_ExecATCmd(GSM_CGREG, (unsigned char*)TEXT_QUESTION,  GSM_ATTIME_CHECK, GSM_CGREG, GSM_ERROR, nBuf);
	
	pTmp = (unsigned char*)strchr( (char*)nBuf, ',' );
	if( pTmp ) {		
		pTmp++;
		nState = (unsigned char)atoi( (char*)pTmp );		
	}
	if( nState == 1 || nState == 5 ){
		return 1;
	}
	else{
		return 0;
	}
}
//////////////////////////////////////////////////////////////
// ɾ������
unsigned char CMM_DeleteSMS( unsigned short nCnt ){
	unsigned char 	nBuf[10];
		
	sprintf((char*)nBuf, "=%d", nCnt); 		
	return CMM_ExecATCmd(GSM_CMGD, nBuf, GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL);		
}
////////////////////////////////////////////////////////////////////
// ��ɾ��ȫ������  
unsigned char CMM_DeleteAllSMS(void){
	
	unsigned char	 nCnt = 0;
	
	for( nCnt = 1; nCnt < 5; nCnt++ ){
		if( CMM_DeleteSMS(nCnt) != GSM_SUCCEED ){
			break;
		} 
	}
	return 0;
}
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
unsigned char GSM_SendPDUSMS(unsigned char *pMsg,  unsigned char nDataLen, unsigned char nTitleLen){

	unsigned char 	nResult  = GSM_NG; 
	unsigned char 	err 	 = 0;
	unsigned char 	nCmdType = 0;
	unsigned char 	nCnt 	 = 0;
	unsigned short	nTimeout = 0;
	unsigned char	*lpLargeBuf = NULL;
	
//	OSTimeDlyHMSM(0,0,0,500 );	
	
	lpLargeBuf	= lpLargeBuf4;		
	
	CMM_SetSendSMSUnReady( );
	nCnt = 0;
	nTimeout = GSM_SENDSPACE_CHECK;
	
	sprintf((char*)lpLargeBuf, "=%d", nTitleLen);
//	#ifdef SIMCOM_340
	CMM_SendATCmd( GSM_CMGS, 0, lpLargeBuf );
//	#else
//	CMM_SendATCmd( GSM_CMGS, 2, lpLargeBuf );
//	#endif
//	OSTimeDlyHMSM(0,0,0,500 );		
	while ( 1 ){
		OSSemPend(pGSMReceiveSem, nTimeout, &err);
				
		if( err == OS_TIMEOUT ){	
			if( CMM_IsSendSMSReady( ) ){						
				CMM_SetSendSMSUnReady( );
				for( nCnt = 0; nCnt < nDataLen; nCnt++){
					
					nCmdType = *(pMsg+nCnt);				
					sprintf((char*)lpLargeBuf, "%X%X", (nCmdType>>4), (nCmdType&0x0f) );
					GSMUARTSendStr( lpLargeBuf );
				}		
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
			CMM_ReadCmd( lpLargeBuf , GSM_RECMAXBUFFER);
			nCmdType = CMM_GetCmdType(lpLargeBuf);		 	
			if(nCmdType ==  GSM_CMGS){
				// Ϊ�˽���OK
				nTimeout = GSM_SENDSPACE_CHECK; 
				nCnt = 0;
				nResult = GSM_SUCCEED;
			} 	
			else if ( nCmdType == GSM_ERROR ){
				nResult = GSM_NG;	
				break;
			}
			else if( nCmdType == GSM_CMS_ERROR ){
				// ����ٵ�6��
				nCnt = 2;
				nResult = GSM_NG;
			}
			else if( nCmdType == GSM_OK ){			
				if( nResult == GSM_SUCCEED ){
					break;
				}
				else{
					// ����ٵ�6��
					nCnt = 2;
					nResult = GSM_SUCCEED;
				}
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
// +CMGR:stat,[alpha],length<CR><LF>
// pdu<CR><LF>
// <CR><LF>
// OK<CR><LF>
// recv2:
// ERROR
// ���س��ȡ�ʧ�ܷ���0
unsigned char	GSM_ReadPDUSMS(unsigned char nCnt, unsigned char *pBuf ){
	
	unsigned char 	err = 0;
	unsigned char 	nCmdType = 0;	
	unsigned char	nReCnt = 0;
	unsigned short 	nLen = 0;
	
	unsigned short	nTimeOut = 0;
	unsigned short	nCntTmp = 0;
	unsigned char	nCmdBuf[20];
	
	
	sprintf((char*)nCmdBuf, "=%d", nCnt);	
	CMM_SendATCmd( GSM_CMGR, 0, nCmdBuf );
	
	// ��ȡ���ű�ͷ	
	// +CMGR: 0,,47  
	// ��ȡ��������
	// 0891683108705505F0240D91683184904214F10000806010810214231EB1996C66B3DD68369A8D368BC568B6DA4C16A3E57033598C96BB01

	nLen = 0;	
	nTimeOut = GSM_ATTIME_CHECK;
	nCnt = 0;
	nReCnt = 0;
	while( 1 ){
		OSSemPend(pGSMReceiveSem, nTimeOut, &err);
		if( err == OS_TIMEOUT ){
			if ( nReCnt++ > 4 ){
				break;
			}			
		}
		else{
			nCntTmp = CMM_ReadCmd((unsigned char *)pBuf+nLen, GSM_RECMAXBUFFER);	
			nCmdType = CMM_GetCmdType( pBuf+nLen );
			if( nCmdType == GSM_CMGR ){
				nTimeOut = GSM_ATTIME_CHECK;
			}
			// �յ�OK����֮ǰ���յ����ݣ����Ϸ���
			else if( nCmdType == GSM_OK ){
				if( nLen > 0 ){
					break;
				}
			}
			else if( nCmdType == GSM_ERROR){
				break;				
			}
			else if( nCmdType == GSM_CMS_ERROR){
				break;
			}
			else if( nCmdType == CMM_CMD_END && nCntTmp > 10){	
				nLen = P_StrToHex( pBuf, pBuf, nCntTmp);			
			}
			else{
				CMM_ParseMasterCmd( pBuf+nLen, nCmdType );
			}
		}
	}	
	return nLen;	
}
///////////////////////////////////////////////////////////////////
//
unsigned char GPRS_SetGateWay( unsigned char *pStr ){
	
//	unsigned char nBuf[50];
//	sprintf( (char*)nBuf, "=1,\"IP\",\"%s\"", pStr );	
//	return CMM_ExecATCmd( GSM_CGDCONT, nBuf, GPRS_GPRSDATA_CHECK, GSM_OK, GSM_CME_ERROR, NULL );	
	return GSM_NG;
}
///////////////////////////////////////////////////////////////////
//
unsigned char GPRS_SetUserPsd( unsigned char *pGateWay,unsigned char *pUser, unsigned char *pPsd ){
	
	unsigned char nBuf[50];
	sprintf( (char*)nBuf, "=\"%s\",\"%s\",\"%s\"",pGateWay,  pUser, pPsd );	
	return CMM_ExecATCmd( GSM_CSTT, nBuf, GPRS_GPRSDATA_CHECK, GSM_CONNECT_OK, GSM_CME_ERROR, NULL );
}
//////////////////////////////////////////////////////////////
// ��������
// ����GSM_SUCCEED��ʾ�ɹ�������ʧ��
unsigned char CMM_IPConnect(  unsigned char nMode, 
															unsigned char *pStrIP,
															unsigned short nPort ){
							 
	unsigned char nBuf[50];
		
	if( g_nNetWork == 1 ){		
		CMM_IPInit( g_stGSMSetup.m_strGateway, 
					g_stGSMSetup.m_strUser, 
					g_stGSMSetup.m_strPsd );
		g_nNetWork = 0;
	}
	else{
		// ����GPRS,Ҫ�Ӵ�ָ��,Ҫ��û��ָ��ͷ
		#ifdef  SIMCOM_900
			CMM_ExecATCmd( GSM_CIPHEAD, (unsigned char*)"=1", GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL );
		#endif
	}
	if( nMode ){
		sprintf((char*)nBuf, "=\"%s\",\"%s\",\"%d\"", TEXT_TCP, pStrIP, nPort);	
	}
	else{
		sprintf((char*)nBuf, "=\"%s\",\"%s\",\"%d\"", TEXT_UDP, pStrIP, nPort);			
	}		
	return CMM_ExecATCmd( GSM_CIPSTART, nBuf, GPRS_GPRSDATA_CHECK, GSM_OK, GSM_ERROR, NULL );
}
/////////////////////////////////////////////////////////////////
// 
unsigned char CMM_AT( void ){
	
	return CMM_ExecATCmd( CMM_CMD_END, NULL, GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL );	
}
/////////////////////////////////////////////////////////////////
//
unsigned char CMM_IPClose( void ){
	
	#ifdef SIMCOM_900		
		return CMM_ExecATCmd( GSM_CIPCLOSE, (unsigned char*)"=1", GPRS_GPRSDATA_CHECK, GSM_CLOSE_OK, GSM_ERROR,NULL ); 	
	#else
		return CMM_ExecATCmd( GSM_CIPCLOSE, NULL, GPRS_GPRSDATA_CHECK, GSM_CLOSE_OK, GSM_ERROR,NULL ); 	
	#endif 
}
//////////////////////////////////////////////////////////////////
//
unsigned char CMM_CIPSHUT( void ){
	
	return CMM_ExecATCmd( GSM_CIPSHUT, NULL, GPRS_GPRSDATA_CHECK, GSM_SHUT_OK, GSM_ERROR,NULL ); 		
}

////////////////////////////////////////////////////////////////////////////
// �����źż���
unsigned char CMM_QuerySignal( void ){
	
	unsigned char   nReturnMsg[20];
	unsigned char	nResult = 0;
	
	if( CMM_ExecATCmd( GSM_CSQ, NULL, GSM_ATTIME_CHECK, GSM_CSQ, GSM_ERROR, nReturnMsg) == GSM_SUCCEED ){
		nResult = atoi( (char*)nReturnMsg );
		if( nResult == 99 ){
			nResult = 0;
		}
	}
	return nResult;
}
//////////////////////////////////////////////////////////////////
// ����GPRS���� 0: ʧ��  1:  �ɹ�
// send:
// AT+IPSEND=
// nMode not user
unsigned char IP_SendData(unsigned char nMode, unsigned char *pBuf, unsigned char nLen){
		
	unsigned char 	nResult  = GSM_NG; 
	unsigned char 	err 	 = 0;
	unsigned char 	nCmdType = 0;
	unsigned char 	nCnt 	 = 0;
	unsigned short	nTimeout = 0;
	unsigned char	*lpLargeBuf = NULL;
	
	
	lpLargeBuf	= lpLargeBuf4;	
	
	CMM_SetSendSMSUnReady( );
	nCnt = 0;
	
	sprintf((char*)lpLargeBuf, "=%d", nLen);
	CMM_SendATCmd( GSM_CIPSEND, 0, lpLargeBuf );	
	while(1){				
		if( CMM_IsSendSMSReady( ) ){						
			CMM_SetSendSMSUnReady();
			GSMUARTSendData(pBuf, nLen);			
			GSMUARTSendStr((unsigned char*)TEXT_SMSSEND_END);
			while( !GSMUARTIsSendBufEmpty( ) ){
				 OSTimeDly( 2 );	
			}	
			nCmdType = 1;		
			break;
		}
		if( nCnt++ >= 10 ){
			break;
		}
		OSTimeDlyHMSM(0,0,0,50);
	}
	CMM_SetSendSMSUnReady();
	nCnt = 0;
	if( nCmdType == 1 ){
		nCmdType = 0;
		nTimeout = GSM_ATTIME_CHECK;	
		while ( 1 ){
			OSSemPend(pGSMReceiveSem, nTimeout, &err);
			if( err == OS_TIMEOUT ){
				if( nCnt++ > 4 ){
					break;
				}
			}				
			else{		
				CMM_ReadCmd( lpLargeBuf , GSM_RECMAXBUFFER);
				nCmdType = CMM_GetCmdType(lpLargeBuf);	
				// ���ͳɹ�
				if( nCmdType == GSM_SEND_OK ){
					 nResult = GSM_SUCCEED;
					 break;
				}
				// ����ʧ��
				else if( nCmdType == GSM_SEND_FAIL ){
					 nResult = GSM_NG;	
					 break;
				}
				else if( nCmdType == GSM_CME_ERROR ){
					nResult = GSM_NG;
					break;
				}
				// ����ʧ��
				else if( nCmdType == GSM_ERROR ){
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
// �������洢������Ϣ
void CMM_ParseMasterCmd(unsigned char *pMsg, unsigned char nCmdType){
	
	unsigned char			nCnt = 0;
	unsigned char			nResultLen = 0;
	unsigned short			nLen;
	unsigned char 			nCaller[TELNUMBER_LENGTH+5];
	unsigned char			*pTmp = NULL;
	unsigned char			nListenFlag = 0;
	

	switch(nCmdType){
	case GSM_RING:
		nCaller[0] = 0x00;
		GSM_ReadCaller( nCaller ); 
		if( strlen( (char*)nCaller) > 0 ){
		
			//��������
			#ifdef SUKE_FRED
			if( IsListenNo( (unsigned char*)nCaller)  ){
			
				nListenFlag = 1;
				CMM_AcceptCall( );		
				CMM_SetCallOnLine( );
				HW_PORT_SPKMUTE_CLOE(); // �رվ���	
			}
			#endif //SUKE_FRED
			
			if( IsAuthorizationNo( (unsigned char*)nCaller) && (nListenFlag == 0) ){						
				
				if(memcmp("669475", nCaller, PHONE_COMPARE_BIT) == 0 ){
					
					g_stGSMState.m_nDialLatLng = 0;
					CMM_AcceptCall( );		
					CMM_SetCallOnLine( );
				}
				else{				
					#ifdef DIAL_CALL
						g_stGSMState.m_nDialLatLng = 0;
						CMM_AcceptCall( );		
						CMM_SetCallOnLine( );
					#else	
						if( g_stGSMState.m_nDialLatLng++ >= 3 ){
						
							g_stGSMState.m_nDialLatLng = 0;
							CMM_AcceptCall( );		
							CMM_SetCallOnLine( );
						}					
						// ȡ�绰����
						nCnt = strlen( (char*)nCaller);							
						memcpy( g_stGSMState.m_strDialNo, nCaller, nCnt );
						g_stGSMState.m_strDialNo[nCnt] = 0x00;
					#endif //DIAL_CALL
				}
								
				#ifdef HW_ET201B
				if(memcmp("669475", nCaller, PHONE_COMPARE_BIT) == 0 ){
					
					HW_PORT_SPKMUTE_OPEN(); // �򿪾���
				}
				else{ 
					#ifdef DIAL_CALL
					HW_PORT_SPKMUTE_OPEN(); // �򿪾���
					#else	
					HW_PORT_SPKMUTE_CLOE(); // �رվ���	
					#endif 
				}
				#endif //HW_ET201B
				#ifdef HW_ET201B_2
				if(memcmp("669475", nCaller, PHONE_COMPARE_BIT) == 0 ){
					
					HW_PORT_SPKMUTE_OPEN();	// �򿪾���
				}
				else{ 
					#ifdef DIAL_CALL
					HW_PORT_SPKMUTE_OPEN(); // �򿪾���
					#else	
					HW_PORT_SPKMUTE_CLOE(); // �رվ���	
					#endif 
				}
				#endif //HW_ET201B_2
				#ifdef HW_ET201B_3
				if(memcmp("669475", nCaller, PHONE_COMPARE_BIT) == 0 ){
					
					HW_PORT_SPKMUTE_OPEN(); // �򿪾���
				}
				else{ 
					#ifdef DIAL_CALL
					HW_PORT_SPKMUTE_OPEN(); // �򿪾���
					#else	
					HW_PORT_SPKMUTE_CLOE(); // �رվ���	
					#endif 
				}
				#endif //HW_ET201B_3				
			}
			else{
				if( nListenFlag == 0 ){	
					CMM_RejectCall( );
					CMM_SetCallOffLine( );		
				}
			}
		}	 
		break; 			
						
	case GSM_CMTI:		
		pTmp = (unsigned char *)strstr((char *)pMsg, ",");
		if( pTmp ){
			pTmp++;
			nCnt = atoi( (char*)pTmp );
			InsertNewMsg( nCnt );
		}
		break;
	// +IPD28:	
	case GSM_IPD:	
		pMsg += 4;	
		pTmp = (unsigned char*)strchr((char*)pMsg, ':' );
		if( pTmp ){
			*pTmp = 0x00;
			nLen = atoi( (char*)pMsg );
			pMsg = pTmp+1;
			nLen = PL_ParseData( pMsg, pMsg, nLen );
			
			if( TE_IsResultGGAddrFlag() ){
							
				TE_ClrResultGGAddrFlag();
				// �����ѯ��־
				TE_ClrQueryGGAddrFlag();				
				OSTimeDlyHMSM(0,0,1,0);
				if( GSM_IsLangState() ){ //����
					nLen = GSM_ComposeSend16GSMData( pMsg,
												 	 &nResultLen, 
												
												 	 g_stGSMSetup.m_strSMCNo, 
												 	 g_stGSMState.m_strForward,
												 	 pMsg,
													 nLen ); 
				}
				else{					 //Ӣ��
					nLen = GSM_ComposeSend7GSMData( pMsg,
												    &nResultLen, 
												
												    g_stGSMSetup.m_strSMCNo, 
												    g_stGSMState.m_strForward,
												    pMsg );
				}
				if( nLen ){
					nCnt = 0;
					while( nCnt++ < 3 ){
						if(GSM_SendPDUSMS( pMsg, nResultLen, nLen ) == GSM_SUCCEED ){
							break;
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
	case GSM_STATE:			//����ʧ��
	case GSM_CLOSED:
	case GSM_CLOSE_OK:
		g_nNetWork = 1;
		IP_SetConnectNG( );
		break;
	case GSM_CIPCLOSE:
	case GSM_CIIPSHUT:
	case GSM_CONNECT_FAIL:
		IP_SetConnectNG( );
		break;
	//���ӷ������ɹ�
	case GSM_CONNECT_OK:
		IP_SetConnectOK( );
		break;
		
	// ͨ������
	// ͨ�����
	// ���˽���
	case GSM_NO_CARRIER:
	case GSM_BUSY:
	case GSM_NO_ANSWER:
		// �رվ���
		#ifdef HW_ET201B
	    HW_PORT_SPKMUTE_CLOE();
	  	#endif
	  	#ifdef HW_ET201B_2
	    HW_PORT_SPKMUTE_CLOE();
	  	#endif
	  	#ifdef HW_ET201B_3
	    HW_PORT_SPKMUTE_CLOE();
	  	#endif
		CMM_SetCallOffLine();
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
	return ;
}
//////////////////////////////////////////////////////////////
//	�������ӳɹ�,�ر�GSMģ���Դ
void	CMM_SendTECNT( void ){

	GSM_SetSetupConfig();
	CMM_ExecATCmd( GSM_TECNT, NULL, GSM_LOWAIT_TIME, GSM_OK, GSM_ERROR, NULL );
}
//////////////////////////////////////////////////////////////
//	���ͶϿ�����,����GSMģ���Դ
void 	GSM_SendDISCNT( void ){

	CMM_ExecATCmd( GSM_TEDISCNT, NULL, GSM_LOWAIT_TIME, GSM_OK, GSM_ERROR, NULL );
	TE_SetNeedTEReset(  );
	GSM_ClrSetupConfig();
}

#endif //HW_SIMCOM