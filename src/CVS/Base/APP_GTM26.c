/////////////////////////////////////////////////////////////
//file name: APP_GSMBENG.H
//     Date: 2008-3-02    
//	 Author: wyb	 
// optimize: real
// op_date : 2009-3-30
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
#include "..\hdr\APP_GTM26.h"
#include "..\hdr\ProtocolA.h"
#include "..\hdr\ProtocolB.h"
#include "..\hdr\ProtocolC.h"


#ifdef			HW_GTM26
//=====================================================
// M26
const char	*s_strCMMCMD[]={
			"OK",
			"E0",				//�رջ���ʾ
			"E1",				//��������
			"+CMEE",		//
			"+CFUN",		//set phone functionality
			"+COPS",		//ģʽѡ��
			"$PWR",			//�ػ�	��������Ϊ100ms			
			"+CSQ",		    //��GSM�ź�		 		
			"+CREG",		//��ѯGSM����
			"+CGREG",		//��ѯGPRS����
			"+CPIN",		//�����
			"+CGSN",		//�������к�
			
			//�绰ָ��
			"+CLCC",    //��ѯ�������
			"D",
			"A",
			"H",
			"NO CARRIER",
			"BUSY",
		  	"NO ANSWER",
			"+CLIP",		//����������ʾ
			"$SG",			//��˷�����
			"$SAMP",		//������������
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
			"+CGATT",		//ע��GPRS����			
			"$NWACT",		//������ر�����
			"$NWSO",		//����ͨ��
			"$NWCN",		//������ر����ӷ�����
			"$NWDF",		//���ý������ݵĸ�ʽ
			"$NWTX",		//�������ݵ�������
			"$NWRX",		//�յ�����
			"$NWDR",		//�ӽ���BUF������
			"ERROR",		//������
};

extern  OS_EVENT 			*pGSMReceiveSem;

//////////////////////////////////////////////////////////////////
//  GSMӲ����ʼ��
void CMM_HWInit( void ){	
	
  GSMUARTInit( 9600 );		// GSM����     
	// GSMģ�鸴λ����
  GPIO_Enable( HW_PORT_GSMRESET );
  GPIO_SetDirection( HW_PORT_GSMRESET, HW_PORT_OUTPUT ); 
  HW_GSM_RESET_HIGH();

  // GSMģ�鿪������	
  GPIO_Enable(HW_PORT_GSMPOWER);			
  GPIO_SetDirection(HW_PORT_GSMPOWER, HW_PORT_OUTPUT);		
  HW_GSM_POWERON( );		
}
/////////////////////////////////////////////////////////
//	GSMͨ��ģ��Ӳ������	
void CMM_ReStart( void ){
	
	HW_GSM_RESET_LOW( );
	OSTimeDlyHMSM(0,0,0,50);
	HW_GSM_RESET_HIGH( );
		
	// power off
	HW_GSM_POWEROFF( );
	OSTimeDlyHMSM(0,0,3,0);
	HW_GSM_POWERON( );	
	OSTimeDlyHMSM(0,0,1,0);
	
	// power on
	HW_GSM_POWEROFF( );
	OSTimeDlyHMSM(0,0,0,100 );
	HW_GSM_POWERON( );	
	OSTimeDlyHMSM(0,0,5,0);	
} 
///////////////////////////////////////////////////////////////////
//  ��ʼ��GSMģ��
void CMM_Init(void){
	
	int 	nCnt = 0;
		
	while( (CMM_ExecATCmd( GSM_ATE0, NULL, GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL ) != GSM_SUCCEED) && 
		   (nCnt++ < 50) ){
		OSTimeDlyHMSM(0,0,1,0);
	}	
	//��⿨�Ƿ����
	nCnt = 0;
	while( ( GSM_CheckSIMReady() != GSM_SUCCEED) && 
		   ( nCnt++ < 50 ) ){		
		OSTimeDlyHMSM(0,0,1,0);
	}	
	// ����	
	CMM_ExecATCmdN( GSM_CMEE, 2, GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL );	
	//  ���ö�����ʾ
	nCnt = 0;
	while( (CMM_ExecATCmdN( GSM_CFUN, 1, GPRS_GPRSDATA_CHECK, GSM_OK, GSM_ERROR, NULL ) != GSM_SUCCEED) &&
				 (nCnt++ < 50) ){
		OSTimeDlyHMSM(0,0,1,0);
	}
	//  ģʽѡ��
	nCnt = 0;
	while( (CMM_ExecATCmdN( GSM_COPS, 0, GPRS_GPRSDATA_CHECK, GSM_OK, GSM_ERROR, NULL ) != GSM_SUCCEED) &&
				  ( nCnt++ < 50)){
		OSTimeDlyHMSM(0,0,1,0);
	}
	nCnt = 0;
	while( ( CMM_QueryModuleSN(g_stGSMSetup.m_strUID) != GSM_SUCCEED )&&
		   ( nCnt++ < 50 ) ){
		OSTimeDlyHMSM(0,0,1,0);
	}		
	//	�������ĺ���
	nCnt = 0;	
	while( ( GSM_QuerySMC(g_stGSMSetup.m_strSMCNo) != GSM_SUCCEED )&&
		   ( nCnt++ < 50 ) ){
		OSTimeDlyHMSM(0,0,1,0);
	}	
	//  ��˷�����
	CMM_ExecATCmd( GSM_SG, (unsigned char*)"=0,8", GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL );
	//  ������������ 
	CMM_ExecATCmd( GSM_SAMP, (unsigned char*)"=0,4", GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL );
	//  ���Ÿ�ʽ	 PDU��ʽ
	CMM_ExecATCmdN( GSM_CMGF, 0, GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL );
	//  ��ʾ�ı���ʽ�µĲ���
	CMM_ExecATCmdN( GSM_CSDH, 1, GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL );
	//  ����������ʾ 
	CMM_ExecATCmdN( GSM_CLIP, 1, GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL );
	//  ɾ��ȫ������
	CMM_DeleteAllSMS();  
	//  ���ö�����ʾ
	while( CMM_ExecATCmd( GSM_CNMI, (unsigned char*)"=2,1", GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL ) != GSM_SUCCEED){
		OSTimeDlyHMSM(0,0,1,0);
	}
}
/////////////////////////////////////////////////////////////
//  ��ʼ����������	
unsigned char CMM_IPInit( unsigned char *pStrGateway, unsigned char *pUser, unsigned char *pPsd ){
	
	unsigned char nBuf[80];	

	CMM_IPClose( );	
	// ע������
	CMM_ExecATCmd( GSM_CGATT, (unsigned char*)"=1", GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL );	
	// �����Զ�ģʽ
	CMM_ExecATCmdN( GSM_CFUN, 1, GPRS_GPRSDATA_CHECK, GSM_OK, GSM_ERROR, NULL );		
	// ģʽѡ��
	CMM_ExecATCmdN( GSM_COPS, 0, GPRS_GPRSDATA_CHECK, GSM_OK, GSM_ERROR, NULL );
	// ��ʼ������ 
	sprintf( (char*)nBuf, "=1,\"%s\",\"%s\",\"%s\"", pStrGateway , pUser, pPsd);	
	if( CMM_ExecATCmd( GSM_NWACT, 
									   nBuf, 
									   GPRS_GPRSDATA_CHECK, 
									   GSM_OK, 
									   GSM_ERROR, NULL ) &&
			CMM_ExecATCmd( GSM_NWSO, 
			 	 					   (unsigned char*)"=1,1", 
			 	 					   GPRS_GPRSDATA_CHECK, 
			 	 					   GSM_OK, 
			 	 					   GSM_ERROR,NULL) ){			
			CMM_ExecATCmd( GSM_NWDF, (unsigned char*)"=1,0", GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL );	 	 					   	
		return 1;			
	}
	else{
		return 0;	
	}
}
//////////////////////////////////////////////////////////////
// ��������
// ����GSM_SUCCEED��ʾ�ɹ�������ʧ��
unsigned char CMM_IPConnect(  unsigned char nMode, 
															unsigned char *pStrIP,
															unsigned short nPort ){
							 
	unsigned char nBuf[50];
	
	if( nMode ){
		sprintf((char*)nBuf, "=1,1,0,\"%s\",%d", pStrIP, nPort);	
	}
	else{
		sprintf((char*)nBuf, "=1,1,1,\"%s\",%d", pStrIP, nPort);			
	}		
	return CMM_ExecATCmd( GSM_NWCN, nBuf, GPRS_GPRSDATA_CHECK, GSM_OK, GSM_ERROR, NULL );
}
/////////////////////////////////////////////////////////////////
//  �ر�GPRS����
unsigned char CMM_IPClose(void){
			
	CMM_ExecATCmd( GSM_NWCN, (unsigned char*)"=1,0", GPRS_GPRSDATA_CHECK, GSM_OK, GSM_ERROR,NULL );
	return CMM_ExecATCmd( GSM_NWACT, (unsigned char*)"=0", GPRS_GPRSDATA_CHECK, GSM_OK, GSM_ERROR,NULL ); 	
}
/////////////////////////////////////////////////////////////////
//  ����ATָ���ʽ
//  ������������, ��ȡ���������
unsigned char CMM_GetCmdType(unsigned char *pMsg){
	
	unsigned char 	nCnt = 0;
	unsigned char		nLen = 0;
	unsigned char 	*pTmp = NULL;
	
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
	return nCnt;
}
/////////////////////////////////////////////////////////////////
// 
unsigned char CMM_AT( void ){
	
	return CMM_ExecATCmd( CMM_CMD_END, NULL, GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL );	
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
///////////////////////////////////////////////////////////	
// ��ѯGSM����״̬
// 0,2,3,4: δע��
// 1:				ע�ᣬ����
// 5:				ע��, ���
// ��Ч����1 ��Ч����0
unsigned char CMM_QueryNetworkStatus(void){
	
	unsigned char 	nBuf[40];
	unsigned char 	nState = 0;	
	unsigned char 	*pTmp = NULL;
	
	memset( (char *)nBuf, 0, 2 );
	
	CMM_ExecATCmd(GSM_CREG, (unsigned char*)TEXT_QUESTION, GSM_ATTIME_CHECK, GSM_CGREG, GSM_ERROR, nBuf);	
				
	pTmp = (unsigned char*)strchr( (char*)nBuf, ',' );
	if( pTmp ){
		pTmp++;
		nState = atoi( (char*)pTmp );
	}
	if( nState == 1 || nState == 5 ){
		return 1;
	}
	else{
		return 0;
	}
}
///////////////////////////////////////////////////////////////
// ��绰
unsigned char CMM_DialNumber( unsigned char *pStrNo ){
	
	unsigned char		nBuf[50];
	unsigned char 		nResult;
	

	sprintf((char*)nBuf, "%s;", (char*)pStrNo ); 
	nResult = CMM_ExecATCmd(GSM_ATD, nBuf, GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL);	
	if( nResult == GSM_SUCCEED ){
		CMM_SetCallOnLine( );		
	}
	return nResult;
}
//////////////////////////////////////////////////////////////
//  �ӵ绰
unsigned char CMM_AcceptCall( void ){	
	
	return CMM_ExecATCmd(GSM_ATA, NULL, GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL);	
}
//////////////////////////////////////////////////////////////
//  �ҵ绰
unsigned char CMM_RejectCall( void ){
	
	return CMM_ExecATCmd(GSM_ATH, NULL, GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL);		
}
//////////////////////////////////////////////////////////////
//  ��ѯIMEI���
unsigned char	CMM_QueryIMEI( unsigned  char	*pDest ){
	
	unsigned char	nResult = GSM_NG;
		
	if( pDest != NULL ){
		strcpy( (char*)pDest, (char*)g_stGSMSetup.m_strUID);
		nResult = GSM_SUCCEED;
	}
	return nResult;	
} 
/////////////////////////////////////////////////////////////
// ����:			0�� free
//						1:  on use
// +CLIP: "13480924411",129,,,,0
//	    : "13480924411",129,,,,0
unsigned char GSM_ParseCaller( unsigned char *pCaller, 
															 unsigned char *pSrc ){
	unsigned char			nBuf[50];
	unsigned char			nResult = 0;
	unsigned char 		*pTmp = NULL;		
		
	pTmp = (unsigned char*)strchr( (char*)pSrc, '"' );
	if( pTmp != NULL ){
		pTmp++;
		strcpy( (char*)nBuf, (char*)pTmp );
				
		pTmp = (unsigned char*)strchr( (char*)nBuf, '"' );
		if( pTmp != NULL && pCaller != NULL){
			memcpy( (char*)pCaller, (char*)nBuf, pTmp-nBuf );
			pCaller[pTmp-nBuf] = 0x00; 
			nResult = 1;
		}
	}	
	return nResult;	
}
//////////////////////////////////////////////////////////////
//
unsigned char GSM_OffModule( void ){
	
	return CMM_ExecATCmd(GSM_PWR, (unsigned char*)"=1", GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL);		
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
	
	return CMM_ExecATCmd(GSM_CMGD, (unsigned char*)"=1,4", GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL);			
}
///////////////////////////////////////////////////////////////
// ��Ч����  1
// ��Ч����  0
unsigned char GSM_CheckSIMReady( ){
	
	unsigned char 	nResult = 0;
	unsigned char 	nReturnMsg[20];
	
	if( CMM_ExecATCmd( GSM_CPIN, (unsigned char *)TEXT_QUESTION, GSM_CHECK_SIGNAL_TIMEOUT, GSM_CPIN,GSM_ERROR, nReturnMsg ) == GSM_SUCCEED ){
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
/////////////////////////////////////////////////////////////
// ��ȡ�������ĺ���
unsigned char GSM_QuerySMC( unsigned char *pStr ){
	
	unsigned char 	nBuf[50];
	unsigned char		nResult = GSM_NG;
	unsigned char		*pTmp = NULL;
	unsigned char 	nTmp = 0;
		
	if( CMM_ExecATCmd( GSM_CSCA, (unsigned char *)TEXT_QUESTION, GSM_ATTIME_CHECK, GSM_CSCA, GSM_ERROR, nBuf ) == GSM_SUCCEED ){
		
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
unsigned char GSM_ParseDNS( unsigned char *pStrHost, unsigned char *pStrIP ){
	
	return 1;
}
/////////////////////////////////////////////////////////////////
//  ��ѯ����
unsigned char	GSM_ExecCUSD( unsigned char	*pDest ,unsigned char	*pSrc, unsigned short	nTimeout ){	

	return  CMM_ExecATCmd( GMS_CUSD, pSrc, nTimeout , GMS_CUSD, GSM_ERROR,  pDest);
}
////////////////////////////////////////////////////////////////////
// ��ѯGPRS����
// ��������״̬
// ��Ч���� 1
// ��Ч���� 0
unsigned char CMM_QueryIPNetworkStatus(void){
	
	unsigned char 	nBuf[40];
	unsigned char		nState = 0;
	unsigned char 	*pTmp = NULL;
	
	memset( (char*)nBuf, 0, 2 );
	CMM_ExecATCmd(GSM_CGREG, (unsigned char*)TEXT_QUESTION,  GSM_ATTIME_CHECK, GSM_CGREG, GSM_ERROR, nBuf);
	
	pTmp = (unsigned char*)strchr( (char*)nBuf, ',' );
	if( pTmp ){		
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
//////////////////////////////////////////////////////////////////
// ����GPRS���� 0: ʧ��  1:  �ɹ�
// send:
// AT+IPSEND=
// nMode not user
unsigned char IP_SendData(unsigned char nMode, unsigned char *pBuf, unsigned char nLen){
			
	unsigned char  			nResult = 0;	
	unsigned char				nCnt = 0;
	unsigned short			nTimeOut = 0;
	unsigned char  			err = 0;
	unsigned char  			nCmdType = 0;	
	unsigned char				*lpLargeBuf = NULL;
		
	lpLargeBuf	= lpLargeBuf4;	
	nMode = nMode;	
	
	sprintf((char*)lpLargeBuf, "%s%s=1,\"", TEXT_AT, s_strCMMCMD[GSM_NWTX] );	
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
			CMM_ReadCmd( lpLargeBuf , GSM_RECMAXBUFFER );		
			nCmdType = CMM_GetCmdType(lpLargeBuf);	
			
			if ( nCmdType == GSM_OK )	{
				nResult = 1;
				break;
			}
			else if( nCmdType == GSM_ERROR ){
				nResult = 0;
				break;
			}			
			else{
				CMM_ParseMasterCmd( lpLargeBuf, nCmdType );
			}
		}	
	}
	return nResult;	
}
////////////////////////////////////////////////////////////////////
//	��ȡGPRS����	
//  $NWDR: 1,6,0,"112233"
unsigned short P_ReadGPRSData( unsigned char *pDest, 
							   							 unsigned short nTialLen ){
		
  
  unsigned char 	err = 0;	
	unsigned char 	nCmdType = 0;	
	unsigned char		nCnt = 0;
	unsigned short	nResultLen = 0;
  unsigned short  nLen = 0;
  unsigned short	nTimeOut = 0;
  unsigned char	  nCmdBuf[20];	
	unsigned char		*pBuf = NULL;
	unsigned char		*pTmp = NULL;
		
	pBuf = lpLargeBuf4;	
	sprintf( (char*)nCmdBuf, "=1,%d", nTialLen );
	CMM_SendATCmd( GSM_NWDR, 0, nCmdBuf );		
	// $NWDR: 1,6,0,"112233"
	nResultLen = 0;	
	nTimeOut = GSM_ATTIME_CHECK;	
	nLen = 0;	
	while( 1 ){
		OSSemPend(pGSMReceiveSem, nTimeOut, &err);
		if( err == OS_TIMEOUT ){
			if( nCnt++ > 4 ){
				break;
			}			
		}
		else{
			CMM_ReadCmd((unsigned char *)pBuf, GSM_RECMAXBUFFER);	
			nCmdType = CMM_GetCmdType( pBuf );
			if( nCmdType == GSM_NWDR ){
				 pTmp = (unsigned char*)strchr((char*)pBuf, ',');
				 if( pTmp){
				 		pTmp++;
				 		nLen = atoi( (char*)pTmp );				 					 		
				 } 
				 pTmp = (unsigned char*)strchr( (char*)pBuf, '"');	
				 if( pTmp ){
				 		nResultLen += P_StrToHex( pDest+nResultLen, pBuf, nLen*2 );	
				 }
				 // ����ȫ������
				 if( nResultLen == nTialLen){
				 		nTimeOut = GSM_ATTIME_CHECK;	
				 }
			}
			// �յ�OK����֮ǰ���յ����ݣ����Ϸ���
			else if( nCmdType == GSM_OK ){
				if( nResultLen > 0 ){
					break;
				}
			}
			else if( nCmdType == GSM_ERROR){
				break;				
			}
			else{
				 CMM_ParseMasterCmd( pBuf, nCmdType );
			}	
		}
	}			
	return nResultLen;
}
////////////////////////////////////////////////////////////////////
// �������洢������Ϣ
void CMM_ParseMasterCmd(unsigned char *pMsg, unsigned char nCmdType){
		
	unsigned char 		nCnt = 0;
	unsigned short		nLen = 0;	
	unsigned char 		nCaller[TELNUMBER_LENGTH+5];
	unsigned char			*lpLargeBuf = NULL;
	unsigned char			*pTmp = NULL;
	
	lpLargeBuf = lpLargeBuf3;
	switch(nCmdType){
	case GSM_CLIP: //+CLIP: "13480924411",129,,,,0
		GSM_ParseCaller( nCaller, pMsg);
		if( strlen( (char*)nCaller) > 0 ){
			if( IsAuthorizationNo( (unsigned char*)nCaller) ){						
				CMM_AcceptCall();		
				CMM_SetCallOnLine();
				// �رվ���				
				#ifdef HW_ET201
				HW_PORT_SPKMUTE_CLOE();
				#endif
			}
			else{
				CMM_RejectCall();
				CMM_SetCallOffLine();		
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
	//�յ����ݸ�ʽ $NWRX: 1,6,6	
	case GSM_NWRX:
		pTmp = (unsigned char*)strchr( (char*)pMsg, ',');
		if( pTmp ){
			 pMsg = pTmp+1;
			 pTmp = (unsigned char*)strchr( (char*)pMsg, ',');
			 if( pTmp ){
			 	pTmp++;			 		
			 	nLen = atoi((char*)pTmp ); // �ܳ���
			 }
			 nLen = P_ReadGPRSData( lpLargeBuf ,nLen );
			 if( nLen > 0 ){
			 		nLen = PL_ParseData( lpLargeBuf, lpLargeBuf, nLen );
			 }
			 // ����ʧ���Ƿ�Ҫ����ģ��
			 if( nLen ){
			 		IP_SendData( IP_IsTCPCommMode(), lpLargeBuf, nLen );
			 }
		}
		break;		
	// ����Ͽ�
	case GSM_NWCN:
		IP_SetConnectNG();
		break;		
	// ͨ������
	// ͨ�����
	// ���˽���
	case GSM_NO_CARRIER:
	case GSM_BUSY:
	case GSM_NO_ANSWER:		
		#ifdef HW_ET201		
	    HW_PORT_SPKMUTE_CLOE();	// �رվ���
	  #endif
		CMM_SetCallOffLine();
		break;			
	default:
		break; 
	}
	return;
}

#endif //HW_GTM26
