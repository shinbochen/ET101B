/////////////////////////////////////////////////////////////
//file name: APP_GSM.C
//     Date: 2008-9-02    
//	 Author: wyb	 
// optimize: real
// op_date : 2009-2-8
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
#include "..\hdr\APP_GTM900.h"
#include "..\hdr\ProtocolA.h"
#include "..\hdr\ProtocolB.h"
#include "..\hdr\ProtocolC.h"


#if( HW_GTM900 )
/////////////////////////////////////////////////////////////
// variable declare
const char *s_strCMMCMD[]={
	"OK",
	"E0",
	"E1",	
	"+CSQ",	
	"%SLEEP",	
//	"+IPR",
	"+CFUN",
	"%NFV",
	"%NFO",
	"%NFI",
	"%VLB",
	"%SNFS",
	"+CMGF",
	"%IPCLOSE",
	"+CSDH",
	"%IOMODE",
	"+CSMP",
	"+CNMI",
	"+CGDCONT",
	"%ETCPIP",
	"+CGREG",
	//"%IPOPENX",			//"CONNECT",  ��Ӧ
	"%IPOPEN",			//������
	"+CREG",
	"D",
	"A",
	"H",
	"+CMGD",
	"+CMGR",
	"+CLCC",
	"+CPIN",
	//"%IPSENDX",
	"%IPSEND",
	"+CMGS",	
	"RING",
	"+CMTI",
	"NO CARRIER",
	"BUSY",
	"NO ANSWER",
	"%IPDATA",
	"CONNECT",
	"ERROR",
	"+CGSN",		//�ն����к�
	"+CSCA",		//�������ĺ���
	"%DNSR",
//	"%MSO",
	"+CGATT",
	"+CUSD",
	"+CGACT",
	"+TECNT", 		//��������
	"+TEDISCNT",	//�Ҷ�����
	"+TESETUP",     //l	��ȡ/���� �豸����
};



extern  OS_EVENT 			*pGSMReceiveSem;
//////////////////////////////////////////////////////////////////
// GSMӲ����ʼ��
void CMM_HWInit( void ){	

  GSMUARTInit(9600);		// GSM ����     
  // GSMģ�鸴λ����
  GPIO_Enable(HW_PORT_GSMRESET);
  GPIO_SetDirection(HW_PORT_GSMRESET, HW_PORT_OUTPUT); 
  HW_GSM_RESET_HIGH();

  // GSMģ�鿪������	WARM
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
//	GSMͨ��ģ��Ӳ������	
void CMM_ReStart( void ){
			
	static unsigned char	sResetFlag = 0;		//CPU��λ��������־
			
	#if( HW_COLDRESTART_GSM )
	OSTimeDlyHMSM(0,0,0,500);
	HW_GSM_COLDPOWEROFF(); //COLD
	OSTimeDlyHMSM(0,0,5,0);	
	HW_GSM_COLDPOWERON();
	
	HW_GSM_RESET_LOW();
	OSTimeDlyHMSM(0,0,0,50);
	HW_GSM_RESET_HIGH();
	
	CMM_PowerOn();
	
	#else	
	   #if( HUAWEI_900C ) //900C
		 if( sResetFlag	 == 0 ){
		 	
		 	sResetFlag = 1;
		 	CMM_PowerOn();
		 }
		 else{		 
		 	CMM_PowerOff();	
		 	
		 	HW_GSM_RESET_LOW();
			OSTimeDlyHMSM(0,0,0,50);
			HW_GSM_RESET_HIGH();
			
			CMM_PowerOn();			
		 }
	   #else  //900A(B)
	   	 CMM_PowerOff();	
		 	
		 HW_GSM_RESET_LOW();
		 OSTimeDlyHMSM(0,0,0,50);
		 HW_GSM_RESET_HIGH();
			
	     CMM_PowerOn();
	   #endif	
	#endif	
	

//	CMM_PowerOff();	
	
//	HW_GSM_RESET_LOW();
//	OSTimeDlyHMSM(0,0,0,50);
//	HW_GSM_RESET_HIGH();
	
//	CMM_PowerOn();
}
///////////////////////////////////////////////////////////////////
// power off
void CMM_PowerOff( void ){ 

	// power off
	#if( HUAWEI_900C )
	HW_GSM_WARM_POWEROFF( );
	OSTimeDlyHMSM(0,0,0,100 );
	HW_GSM_WARM_POWERON( );	
	OSTimeDlyHMSM(0,0,8,0);	
	#else	
	HW_GSM_WARM_POWEROFF( );
	OSTimeDlyHMSM(0,0,3,0);
	HW_GSM_WARM_POWERON( );	
	OSTimeDlyHMSM(0,0,1,0);
	#endif
}
///////////////////////////////////////////////////////////////////
// power on
void CMM_PowerOn( void ){
	
	#if( HUAWEI_900C )
	OSTimeDlyHMSM(0,0,1,0 );
	#endif
	// power on
	HW_GSM_WARM_POWEROFF( );
	OSTimeDlyHMSM(0,0,0,100 );
	HW_GSM_WARM_POWERON( );	
	OSTimeDlyHMSM(0,0,5,0);	
}

///////////////////////////////////////////////////////////////////
//��ʼ��GSMģ��
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
	
	#if( SW_PROTOCOL_ASICO == 0 )
	if( strlen( (char*)g_stGSMSetup.m_strUID ) <= 0 ){
		TE_SetNeedWriteFlash( );
	}
	// �ն����к�
	nCnt = 0;
	while( ( CMM_QueryModuleSN(g_stGSMSetup.m_strUID) != GSM_SUCCEED )&&
		   ( nCnt++ < 50 ) ){
		OSTimeDlyHMSM(0,0,1,0);
	}	
	#endif //SW_PROTOCOL_ASICO
	
	//	�������ĺ���
	nCnt = 0;	
	while( ( GSM_QuerySMC(g_stGSMSetup.m_strSMCNo) != GSM_SUCCEED )&&
		   ( nCnt++ < 50 ) ){
		OSTimeDlyHMSM(0,0,1,0);
	}	
	// �ر����߹���	
	CMM_ExecATCmdN( GSM_SLEEP, 0, GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL );
	// ȫ����ģʽ
	CMM_ExecATCmdN( GSM_CFUN, 1, GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL );	
	//  ��Ƶͨ��ѡ��	
	CMM_ExecATCmdN( GSM_SNFS, 0, GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL );	
	//  ������Ƶ��� 0,6,0
	CMM_ExecATCmd( GSM_NFO, (unsigned char*)"=4,6,0", GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL );
	//  ������Ƶ����
	CMM_ExecATCmd( GSM_NFI, (unsigned char*)"=0,12,1,0", GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL );
	//  ������������ 
	CMM_ExecATCmdN( GSM_NFV, 4, GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL );
	//  ���Ÿ�ʽ	 PDU��ʽ
	CMM_ExecATCmdN( GSM_CMGF, 0, GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL );
	//  ��ʾ�ı���ʽ�µĲ���
	CMM_ExecATCmdN( GSM_CSDH, 1, GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL );
	//  GPRSSENDģʽ
	CMM_ExecATCmd(GSM_IOMODE, (unsigned char*)"=1,1", GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL);
	// ɾ��ȫ������
	CMM_DeleteAllSMS();  
	// �����ź��Զ���ʾ
	CMM_ExecATCmdN( GSM_CREG, 2, GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL );
	//���ö�����ʾ
	while( CMM_ExecATCmd( GSM_CNMI, (unsigned char*)"=2,1", GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL ) != GSM_SUCCEED){
		OSTimeDlyHMSM(0,0,1,0);
	}
	return;
}

/////////////////////////////////////////////////////////////
//��ʼ����������	NG+?
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
}
/////////////////////////////////////////////////////////////////
// ����ATָ���ʽ
// ������������, ��ȡ���������
unsigned char CMM_GetCmdType(unsigned char *pMsg) 	{
	
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
unsigned char CMM_IPClose( void ){
	return CMM_ExecATCmd( GSM_IPCLOSE, NULL, GPRS_GPRSDATA_CHECK, GSM_OK, GSM_ERROR,NULL ); 	
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
unsigned char CMM_QueryNetworkStatus( void )	{
	
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
	
	//CMM_ExecATCmdN( GSM_VLB, 1, GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL );	
	
	sprintf((char*)nBuf, "%s;", (char*)pStrNo ); 
	nResult = CMM_ExecATCmd(GSM_ATD, nBuf, GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL);	
	if( nResult == GSM_SUCCEED ){
		CMM_ExecATCmdN( GSM_VLB, 1, GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL );	
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
	
	unsigned char	nBuf[20];
	
	sprintf( (char*)nBuf, "*#06#" );		
	return CMM_ExecATCmd(GSM_ATD, nBuf, GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, pDest );	
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
//////////////////////////////////////////////////////////////
//
//unsigned char GSM_OffModule( void ){
//	return CMM_ExecATCmd(GSM_MSO, NULL, GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL);		
//}
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
	return CMM_ExecATCmd(GSM_CMGD, (unsigned char*)"=1,4", GSM_ATTIME_CHECK, GSM_OK, GSM_ERROR, NULL);			
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
			else if( nCmdType == GSM_ERROR){
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
/////////////////////////////////////////////////////////////
// ��ȡ�������ĺ���
// 
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
	
	unsigned char			nBuf[50];	
	sprintf((char*)nBuf, "=\"%s\"", pStrHost );	
	return CMM_ExecATCmd( GSM_DNSR, nBuf, GSM_CHECK_SIGNAL_TIMEOUT, GSM_DNSR, GSM_ERROR, pStrIP );
}
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
//	return  CMM_ExecATCmd( GMS_CUSD, pSrc, nTimeout , GMS_CUSD, GSM_ERROR,  pDest);
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
	
	sprintf( (char*)nBuf, "=1,\"IP\",\"%s\"", pStr );	
	return CMM_ExecATCmd( GSM_CGDCONT, nBuf, GPRS_GPRSDATA_CHECK, GSM_OK, GSM_ERROR, NULL );
	
}
///////////////////////////////////////////////////////////////////
//
unsigned char GPRS_SetUserPsd( unsigned char *pUser, unsigned char *pPsd ){
	
	unsigned char nBuf[50];
	sprintf( (char*)nBuf, "=\"%s\",\"%s\"",  pUser, pPsd );	
	return CMM_ExecATCmd( GSM_ETCPIP, nBuf, GPRS_GPRSDATA_CHECK, GSM_OK, GSM_ERROR, NULL );
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
		sprintf((char*)nBuf, "=\"%s\",\"%s\",%d", TEXT_TCP, pStrIP, nPort);	
	}
	else{
		sprintf((char*)nBuf, "=\"%s\",\"%s\",%d", TEXT_UDP, pStrIP, nPort);			
	}		
	return CMM_ExecATCmd( GSM_IPOPEN, nBuf, GPRS_GPRSDATA_CHECK, GSM_CONNECT, GSM_ERROR, NULL );
}
//////////////////////////////////////////////////////////////////
// ����GPRS���� 0: ʧ��  1:  �ɹ�
// send:
// AT+IPSEND=
// nMode not user
unsigned char IP_SendData(unsigned char nMode, unsigned char *pBuf, unsigned char nLen){
			
	unsigned char  					nResult = 0;	
	unsigned char					nCnt = 0;
	unsigned short					nTimeOut = 0;
	unsigned char  					err = 0;
	unsigned char  					nCmdType = 0;	
	unsigned char					*lpLargeBuf = NULL;
	
	
	lpLargeBuf	= lpLargeBuf4;	
	nMode = nMode;
	
	
	sprintf((char*)lpLargeBuf, "%s%s=\"", TEXT_AT, s_strCMMCMD[GSM_IPSEND] );	
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
			else if ( nCmdType == GSM_IPSEND){			
				err = atoi( (char*)lpLargeBuf );				
				if( err < 12 ){
					nResult = 0;
				}
				else{
					nResult = 1;
				}
				// Ϊ�˽���OK
				nTimeOut = GSM_SENDSPACE_CHECK;				
			} 
			else{
				CMM_ParseMasterCmd( lpLargeBuf, nCmdType );
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
		
	case GSM_IPDATA:		
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
		
	// ����Ͽ�
	case GSM_IPCLOSE:
		IP_SetConnectNG( );
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

#endif //HW_GTM900