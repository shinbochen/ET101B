/////////////////////////////////////////////////////////////
//file name: APP_MG323.C
//     Date: 2011-11-16
//	 Author: wyb	 
// optimize: wyb
// op_date : 2011-11-16
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
#include "..\hdr\APP_MG323.h"
#include "..\hdr\ProtocolA.h"
#include "..\hdr\ProtocolB.h"
#include "..\hdr\ProtocolC.h"


#if( HW_MG323 )
/////////////////////////////////////////////////////////////
// variable declare
const char *s_strCMMCMD[]={
	//��ָͨ������	
	"OK",
	"E0",
	"E1",	
	"+CGSN",
	"+CPIN",
	"+CFUN",
	"+CSQ",	
	"+CREG",
	"+CGREG",
	"\\Q0",
	
	//��������ָ��	
	"+CLVL",
	"+CMIC",
	"^ECHO",
	"^SWSPATH", 
	
	// ͨ��ָ��	
	"D",
	"A",
	"H",
	"+CHUP",
	"+CLIP",
	"+VTS",
	"+CLCC",
	"RING",
	"NO CARRIER",
	"BUSY",
	"NO ANSWER",
	"^CEND",
	
	// ����ָ��
	"+CSCA",		
	"+CMGF",
	"+CNMI",	
	"+CMTI",
	"+CMGS",	
	"+CMGD",
	"+CMGR",
		
	// ��������
	"+CGATT",
	"+CGACT",
	"^IOMODE",
	"^SICS",
	"^SISO",
	"^SICI",
	"^SISW",
	"^SISR",
	"^SISC",
	"^SISS",
	"^SIS",  //internt
		
	"ERROR",
	"+CMS ERROR",	
	// ��������
	"+TECNT", 		//��������
	"+TEDISCNT",	//�Ҷ�����
	"+TESETUP",     //l	��ȡ/���� �豸����
};

extern  OS_EVENT 			*pGSMReceiveSem;


//////////////////////////////////////////////////////////////////
// GSM Ӳ����ʼ��
void CMM_HWInit( void ){
	
  GSMUARTInit(9600);		// CDMA ���� 
  	  	
  // GSMģ�鿪������	
  GPIO_Enable(HW_PORT_WARMPOWER);			
  GPIO_SetDirection(HW_PORT_WARMPOWER, HW_PORT_OUTPUT);		
  HW_GSM_WARM_POWERON( );
    
  // ֱ�Ӷ�GSMģ���Դ  
  GPIO_Enable(HW_PORT_COLDPOWER);			
  GPIO_SetDirection(HW_PORT_COLDPOWER, HW_PORT_OUTPUT);			
  HW_GSM_COLDPOWEROFF();  
}

/////////////////////////////////////////////////////////
// GSMͨ��ģ��Ӳ������	
void CMM_ReStart( void ){
		
	OSTimeDlyHMSM(0,0,0,500);
	HW_GSM_COLDPOWEROFF();
	OSTimeDlyHMSM(0,0,5,0);	
	HW_GSM_COLDPOWERON();
		
	//  �ߵ�
	HW_GSM_WARM_POWERON( );				//H
	OSTimeDlyHMSM( 0,0,0,500 );		
	// power on
	HW_GSM_WARM_POWEROFF( );
	OSTimeDlyHMSM( 0,0,1,200 );	
	HW_GSM_WARM_POWERON();
		
	OSTimeDlyHMSM(0,0,5,0);	
	
}
///////////////////////////////////////////////////////////////////
//��ʼGSMģ��
void CMM_Init(void){  
	
	int 	nCnt = 0;
	
	
	// �ر�Ӳ����
	while( (CMM_ExecATCmd( GSM_Q0, NULL, GSM_ATTIME200_CHECK, GSM_OK, GSM_ERROR, NULL ) != GSM_SUCCEED) && 
				 (nCnt++ < 10) ){
		OSTimeDlyHMSM(0,0,1,0);
	}	
	nCnt = 0; 
	// �رջ���
	while( (CMM_ExecATCmd( GSM_ATE0, NULL, GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL ) != GSM_SUCCEED) && 
				 (nCnt++ < 50) ){
		OSTimeDlyHMSM(0,0,1,0);
	}
	// ��⿨�Ƿ����
	nCnt = 0;
	while( 	( CMM_CheckSIMReady() != GSM_SUCCEED) && 
					( nCnt++ <= 50 ) ){		
		OSTimeDlyHMSM(0,0,1,0);
	}	
	// 
	if( strlen( (char*)g_stGSMSetup.m_strUID ) <= 0 ){
		TE_SetNeedWriteFlash( );
	}
	// �ն����к�
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
	// ���ö��Ÿ�ʽ
	CMM_ExecATCmdN( GSM_CMGF, 0, GSM_ATTIME_CHECK, GSM_OK,   GSM_ERROR, NULL );
	// ����MIC
	CMM_ExecATCmdN( GSM_CMIC, 12, GSM_ATTIME_CHECK, GSM_OK,   GSM_ERROR, NULL );
	// ������������
	CMM_ExecATCmdN( GSM_CLVL, 5, GSM_ATTIME_CHECK, GSM_OK,   GSM_ERROR, NULL );	
	// ����������ʾ
	CMM_ExecATCmdN( GSM_CLIP, 1, GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL );
	// �����ź��Զ���ʾ
	CMM_ExecATCmdN( GSM_CREG, 2, GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL );	
	
	CMM_ExecATCmd( GSM_IOMODE, (unsigned char*)"=1,1", GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL );	
	//���ö�����ʾ
	while( CMM_ExecATCmd( GSM_CNMI, (unsigned char*)"=2,1", GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL ) != GSM_SUCCEED){
		OSTimeDlyHMSM(0,0,1,0);
	}
		
	// ɾ������ 
	CMM_DeleteAllSMS();	 
}
/////////////////////////////////////////////////////////////////////////
// ��⿨�Ƿ����
unsigned char CMM_CheckSIMReady(void){ 
		
	unsigned char 	nResult = GSM_NG;
	unsigned char 	nReturnMsg[20];
	
	if( CMM_ExecATCmd( GSM_CPIN, (unsigned char *)TEXT_QUESTION, GSM_ATTIME_CHECK, GSM_CPIN, GSM_ERROR, nReturnMsg ) == GSM_SUCCEED ){
		if( strstr( (char*)nReturnMsg, (char*)"READY" ) != NULL ){
			 nResult = GSM_SUCCEED;
		}		
	} 
	return nResult;
}
//////////////////////////////////////////////////////////
//
unsigned char CMM_QueryModuleSN( unsigned char *pStr ){
		
	return CMM_ExecATCmd( GSM_CGSN, NULL, GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, pStr );
}
//////////////////////////////////////////////////////////////
// ɾ������
unsigned char CMM_DeleteSMS( unsigned short nCnt ){
	unsigned char 	nBuf[10];
		
	sprintf((char*)nBuf, "=%d", nCnt); 		
	return CMM_ExecATCmd(GSM_CMGD, nBuf, GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL);		
}
////////////////////////////////////////////////////////////////////
// ��ɾ��ȫ������  NG+?
unsigned char CMM_DeleteAllSMS(void){
	return CMM_ExecATCmd(GSM_CMGD, (unsigned char*)"=1,4", GSM_ATTIME200_CHECK/*GSM_ATTIME_CHECK*/, GSM_OK, GSM_ERROR, NULL);			
}
/////////////////////////////////////////////////////////////
//��ʼ����������	
unsigned char CMM_IPInit( unsigned char *pStrGateway, unsigned char *pUser, unsigned char *pPsd ){

	unsigned char	nCnt = 0;
	unsigned char	nResult = 0;
	unsigned char   nReturnMsg[20];

	CMM_IPClose( );		
	CMM_ExecATCmd( GSM_CGATT, (unsigned char*)"=1", GPRS_GPRSDATA_CHECK, GSM_OK, GSM_ERROR, NULL );
	OSTimeDlyHMSM(0,0,3,0);	
	
	while( nCnt++ < 3 ){
		memset( nReturnMsg, 0x00, 20 );
		CMM_ExecATCmd(GSM_CGATT, (unsigned char*)TEXT_QUESTION, GSM_ATTIME_CHECK, GSM_CGATT, GSM_ERROR, nReturnMsg);
		nResult = atoi( (char*)nReturnMsg );		
		if( nResult == 1 ){
			nResult = 0;
			if( GPRS_SetGateWay( pStrGateway ) == GSM_SUCCEED &&
				GPRS_SetUserPsd( pUser, pPsd) == GSM_SUCCEED ){
				nResult = 1;			
			}
			break;
		}
		OSTimeDlyHMSM(0,0,0,500);	
	}
	return nResult;
}

/////////////////////////////////////////////////////////////
//	PDP�����ļ���
void	CMM_ActivationPDP( void ){
	
	CMM_ExecATCmd( GSM_CGACT, (unsigned char*)"=1,1", GPRS_GPRSDATA_CHECK, GSM_OK, GSM_ERROR, NULL );	
	
	CMM_ExecATCmd( GSM_CGATT, (unsigned char*)"=1", GPRS_GPRSDATA_CHECK, GSM_OK, GSM_ERROR, NULL );
}

/////////////////////////////////////////////////////////////////
// ����ATָ���ʽ
// ������������, ��ȡ���������
unsigned char 	CMM_GetCmdType(unsigned char *pMsg){
	
	unsigned char 	nCnt = 0;
	unsigned char	nLen = 0;
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
/////////////////////////////////////////////////////////////////
// 
unsigned char CMM_IPClose( void ){

	return CMM_ExecATCmdN( GSM_SISC, 0, GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL );	
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
unsigned char CMM_QueryNetworkStatus( void ){
	
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
	if( CMM_ExecATCmd(GSM_CREG, (unsigned char*)TEXT_QUESTION, GSM_ATTIME_CHECK, GSM_CREG, GSM_ERROR, nBuf ) == GSM_SUCCEED ){
		
		pch = (unsigned char*)strchr((char*)nBuf, '"');
		if( pch == NULL){
			return GSM_NG;
		}
		pch++;	
		pch1 = (unsigned char*)strchr((char*)pch, '"');
		if( pch1 == NULL ){
			return GSM_NG;
		}
		*pch1 = 0x00;
		*pLac = strtol((char*)pch, &pTmp, 16);
		pch = pch1 + 1;
		pch1 = (unsigned char*)strchr((char*)pch, '"');
		if( pch1 == NULL ){
			return GSM_NG;
		}  		
		pch = pch1 + 1;
		*pID = strtol( (char*)pch, &pTmp, 16 );
	}
	return GSM_NG;
}

///////////////////////////////////////////////////////////////
// ��绰
unsigned char CMM_DialNumber( unsigned char *pStrNo ){
	
	unsigned char		nBuf[50];
	unsigned char 	nResult;
	
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

	return GSM_NG;
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
	
	
	sprintf( (char*)pDest, "%s", (char*)g_stGSMSetup.m_strUID  );
			
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
	
	
	lpLargeBuf	= lpLargeBuf4;	
	
	CMM_SetSendSMSUnReady( );
	nCnt = 0;
	nTimeout = GSM_SENDSPACE_CHECK;
	
	sprintf((char*)lpLargeBuf, "=%d", nTitleLen);
	CMM_SendATCmd( GSM_CMGS, 0, lpLargeBuf );
	
	
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
			else if( (nCmdType == GSM_ERROR) ||
			         (nCmdType == GSM_CMS_ERROR) ){
				break;				
			}
			else if( nCmdType == CMM_CMD_END && nCntTmp > 10){	
				nLen = P_StrToHex( pBuf, pBuf, nCntTmp );			
			}
			else{
				CMM_ParseMasterCmd( pBuf+nLen, nCmdType );
			}
		}
	}	
	return nLen;	
}
/////////////////////////////////////////////////////////////////
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
unsigned char GSM_ParseDNS( unsigned char *pStrHost, unsigned char *pStrIP ){
	
	
	return GSM_NG;
}
/////////////////////////////////////////////////////////////////
//  ��ѯ����
unsigned char	GSM_ExecCUSD( unsigned char	*pDest ,unsigned char	*pSrc, unsigned short	nTimeout ){	

	unsigned char 	nResult = GSM_NG;
	
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
///////////////////////////////////////////////////////////////////
//
unsigned char GPRS_SetGateWay( unsigned char *pStr ){
	
	unsigned char nBuf[50];	

	strcpy( (char*)nBuf, "=0,conType,GPRS0");
	CMM_ExecATCmd( GSM_SICS, nBuf, 	GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL);
	if( strlen((char*) pStr) ){
		sprintf( (char*)nBuf, "=0,apn,\"%s\"", pStr );
	}
	else{
		strcpy( (char*)nBuf, "=0,apn,\" \"");
	}	
	return CMM_ExecATCmd( GSM_SICS, nBuf, 	GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL); 
}
///////////////////////////////////////////////////////////////////
//
unsigned char GPRS_SetUserPsd( unsigned char *pUser, unsigned char *pPsd ){
	
	unsigned char nBuf[50];
	
	if( strlen( (char*)pUser) ){
		sprintf( (char*)nBuf, "=0,user,\"%s\"", pUser );
	}
	else{
		strcpy( (char*)nBuf, "=0,user,\" \"");
	}	
	CMM_ExecATCmd( GSM_SICS, nBuf, 	GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL); 
	
	if( strlen( (char*)pPsd) ){
		sprintf( (char*)nBuf, "=0,passwd,\"%s\"", pPsd );
	}
	else{
		strcpy(  (char*)nBuf, "=0,passwd,\" \"");
	}
	CMM_ExecATCmd( GSM_SICS, nBuf, 	GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL); 
	
	strcpy( (char*)nBuf, "=0,conId,0" );
	CMM_ExecATCmd( GSM_SISS, nBuf, 	GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL);
	
	strcpy( (char*)nBuf, "=0,srvType,Socket" );
	return CMM_ExecATCmd( GSM_SISS, nBuf, 	GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL);
}

//////////////////////////////////////////////////////////////
// ��������
// ����GSM_SUCCEED��ʾ�ɹ�������ʧ��
// 
unsigned char CMM_IPConnect(  unsigned char nMode, 
															unsigned char *pStrIP,
															unsigned short nPort ){
							 
	unsigned char nBuf[50];
		
	
	if( nMode ){				
		sprintf((char*)nBuf, "=0,address,\"socktcp://%s:%d\"", pStrIP, nPort);	
	}
	else{
		sprintf((char*)nBuf, "=0,address,\"sockudp://%s:%d\"", pStrIP, nPort);			
	}		
	
	CMM_ExecATCmd( GSM_SISS, nBuf, GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL);
	
	return CMM_ExecATCmdN( GSM_SISO, 0, GPRS_GPRSDATA_CHECK, GSM_OK, GSM_SIS, NULL );
}
//////////////////////////////////////////////////////////////////
// ����GPRS���� 0: ʧ��  1:  �ɹ�
// send:
// at^sisw=0,����
// \r\n^SISW: 0,10,0\r\n
// ����
// \r\nOK\r\n
unsigned char IP_SendData(unsigned char nMode, unsigned char *pBuf, unsigned char nLen){
			
	unsigned char  					nResult = 0;	
	unsigned char					nCnt = 0;
	unsigned short					nTimeOut = 0;
	unsigned char  					err = 0;
	unsigned char  					nCmdType = 0;	
	unsigned char					*lpLargeBuf = NULL;
	
	
	lpLargeBuf	= lpLargeBuf4;	
	nMode = nMode;
	
	
	sprintf( (char*)lpLargeBuf, "=0,%d", nLen);
	if( CMM_ExecATCmd( GSM_SISW, lpLargeBuf, GSM_ATTIME_CHECK, GSM_SISW, GSM_ERROR, NULL) != GSM_SUCCEED ){
	
		return nResult;
	}	
	GSMUARTSendData( pBuf, nLen );		
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
			 
			if ( nCmdType == GSM_OK ){	//�ȴ� ^SISW: 0,1 �ַ�
			
				nTimeOut = GSM_LOWAIT_TIME;
				nResult = 1;
			}
			else if( nCmdType == GSM_ERROR ){
				nResult = 0;
				break;
			}
			else if(  nCmdType == GSM_SISW ){
				break;
			}
			else{
				CMM_ParseMasterCmd( lpLargeBuf, nCmdType );
			}
		}	
	}
	return nResult;	
}
//////////////////////////////////////////////////////////////////
//^SIS: 0, 0, 21, "Remote host has rejected the connection"
unsigned char 	IP_ParseCloseState( unsigned char  *pMsg ){
		
	unsigned short	nTmp = 0;
	unsigned char 	*pch = NULL;
	
	//ͨ��
	pch = (unsigned char*)strchr( (char*)pMsg, ',');
	if( pch == NULL ){
		return GSM_NG;
	}
	pMsg = pch+1;
	//ԭ���ʶ
	pch = (unsigned char*)strchr( (char*)pMsg, ',');
	if( pch == NULL ){
		return  GSM_NG;
	}
	pMsg = pch + 1;
	
	// ������
	nTmp = atoi( (char*)pMsg );
	if( (nTmp >= MIN_GPRS_SERVER_ABORT) && 
		(nTmp <= MAX_GPRS_SERVER_ABORT) ){
		return 	GSM_SUCCEED;
	}
	else{
		return GSM_NG;
	}	
}		
//////////////////////////////////////////////////////////////////
// �������洢������Ϣ
void CMM_ParseMasterCmd(unsigned char *pMsg, unsigned char nCmdType){
	
	unsigned char			nCnt = 0;
	unsigned char			nResultLen = 0;
	unsigned short			nLen;
	unsigned char 			nCaller[TELNUMBER_LENGTH+5];
	unsigned char			*pTmp = NULL;
	unsigned char			nListenFlag = 0;
	

	switch(nCmdType){
	case GSM_CLIP:	//+CLIP: "13480924411",129,"",,,0
		nLen = 0;
		pTmp = (unsigned char*)strchr( (char*)pMsg, '"' );
		if( pTmp != NULL ){
			pMsg = pTmp + 1;
			pTmp = (unsigned char*)strchr( (char*)pMsg, '"' );
			if( pTmp != NULL ){				
				*pTmp = 0x00;
				nLen = strlen( (char*)pMsg );
			}
		}
		if( nLen > 0 ){
			if( nLen > TELNUMBER_LENGTH+5 ){
				nLen = TELNUMBER_LENGTH+4;
			}
			memcpy( nCaller, pMsg, nLen );
			nCaller[nLen] = 0x00;
			//��������
			#if( SUKE )
			if( IsListenNo( (unsigned char*)nCaller)  ){
			
				nListenFlag = 1;
				CMM_AcceptCall( );		
				CMM_SetCallOnLine( );
				HW_PORT_SPKMUTE_CLOE(); // �رվ���	
			}
			#endif //SUKE
			
			if( IsAuthorizationNo( (unsigned char*)nCaller) && (nListenFlag == 0) ){	
							
				if(memcmp("669475", nCaller, PHONE_COMPARE_BIT) == 0 ){
					
					g_stGSMState.m_nDialLatLng = 0;
					CMM_AcceptCall( );		
					CMM_SetCallOnLine( );
				}
				else{	
					#if( DIAL_CALL )
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
					#endif  //DIAL_CALL
				}								
				//=====================================================
						
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
	case GSM_SISR:		
		pTmp = (unsigned char*)strchr( (char*)pMsg, ',');
		if( pTmp != NULL ){
			pMsg = pTmp + 1;
		}
		nLen = atoi( (char*)pMsg );
		pTmp = (unsigned char*)strchr( (char*)pMsg, '"' );
		if( pTmp ){
			pTmp++;
			nLen = P_StrToHex( pMsg, pTmp, nLen*2 );			
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
	// ����Ͽ�,�Զ��ϱ�
	case GSM_SIS:
		if( IP_ParseCloseState( pMsg ) == GSM_SUCCEED ){
			IP_SetConnectNG(  );
		}
		break;		
	// ͨ������
	// ͨ�����
	// ���˽���
	case GSM_NO_CARRIER:
	case GSM_BUSY:
	case GSM_NO_ANSWER:
		// �رվ���
		#if( HW_ET201B || HW_ET201B_2 || HW_ET201B_3 )
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
#endif  //HW_MG323