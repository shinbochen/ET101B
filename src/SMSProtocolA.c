///////////////////////////////////////////////////////////////////////////
//   file name: SMSProtocolData1.h
// description: SMSЭ��ͷ�ļ�����\old default function
//      author:
//        date: 2009-02-24
//     reamark: 
#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>
#include  "..\hdr\APP_BQM26.h"
#include "..\hdr\APP_GTM900.h"
#include  "..\hdr\APP_GSM.h"
#include "..\hdr\APP_EM200.h"
#include "..\hdr\APP_SIMCOM.h"
#include  "..\hdr\APP_Terminal.h"
#include  "..\hdr\SMSProtocolA.h"
#include  "..\hdr\SMSProtocolB.h"
#include  "..\hdr\APP_GPS.h"

#if( SW_SMSPROTOCOLA )
char*   g_strSetCmd[]={
   "restart", 		// ������λ
   "default",		// �ָ���������
   "sn",			// ���	
   "inf",			// ״̬
   "version",		// ��ȡ�汾��
   "w",				// ��λ
   
   "enable",  		// ��������	
   "disable", 		// �رձ���
   
   "per",			// ���ټ��
   "parking",		// ͣ������	
   "open",			// �Ƿ����ű���ʱ��
   "overspeed",		// ���ٱ���		
   "stop",			// ����	
   "re",				// �ָ���·
   "l",					// ����
   "sp1",				// ���ú���
   "sp2",				// ���ú���
   "sp3",				// ���ú���	  
   "h",					// �������غ���
   "csca",			// �������ĺ���					   
   "p",					// ��������
   
   "t",					// ��������					   
   "td",				// �رո���
   
   "up",      	// �����û���������
   "apn",				// ����APN���� 
   "dnsr",    	// ����������
   
   "towe",    	// ������������ 
   "towd",		// �رշ���
   
   "time",   	// �Զ���������
   "gprs",		// GPRS����
   "ip",		// ����IP��ַ����
   "gp", 		// ����ģʽ	( 1:sms 2:gprs )
   "forward",	// ��ѯ�������� *Forward=10086:CXCY#
   "excute",	// ��ѯ�ֻ����� *EXCUTE=*888##
   "IMEI",		// IMEI
   "poweroff",
   "lowpower",
   "setup",
   "dnsr",
   "aslis",			//0:����Ϊ�ż����뱨�� 1:����Ϊԭ���������� ASLIS
   "reinput",
};

/////////////////////////////////////////////////////////////
// ��ȡ�������� 
unsigned char PLS_GetCommandType( unsigned char  *pBuf ){

	unsigned char 	nCnt 	  = 0;
	unsigned char	*pTmp 	  = NULL;
	unsigned char	*pKeyword = NULL;
	
	// pTmpָ������
	// pKeywordָ��ؼ���	
	
	if( *pBuf != '*' ){
		return SMS_CMD_END;		
	}
	
	pTmp = (unsigned char*)strchr( (char*)pBuf+1, '*' );
	if( pTmp == NULL ){
		pTmp = (unsigned char*)strchr( (char*)pBuf+1, ':' );
		if( pTmp ){
			*pTmp = 0x00;
			 pTmp++;
		}
		else{
			pTmp = (unsigned char*)strchr( (char*)pBuf+1, '#' );
			if( pTmp ){
				*pTmp = 0x00;				
			}						
		}
	}
	else{
		*pTmp = 0x00;
		pTmp++;		
	}	 	
	pKeyword = pBuf+1; 
	
	// ������Ƚ� 	
	for( nCnt = 0; nCnt < SMS_CMD_END; nCnt++ ){
	 	if( strcmpnc((unsigned char*)g_strSetCmd[nCnt], pKeyword ) ){
			break;
		}	 	
	}
	if( nCnt < SMS_CMD_END ){		
		strcpy( (char*)pBuf, (char*)pTmp );	
		pTmp = (unsigned char*)strrchr( (char*)pBuf, '#' );		
		if( pTmp ){		
			*pTmp = 0x00;
		}
	}
	return nCnt;	
}				
////////////////////////////////////////////////////////////////////
//
unsigned char	PLS_ParseCommandData( unsigned char  *pDest, 
																	  unsigned char  *pSrc,
																	  unsigned char  *pCaller,
																	  unsigned char  *pCodeType ){
													  	
	unsigned char	nCmdType  = 0;	
	unsigned char	nTmp = 0;
	unsigned char nResult = 0;
	unsigned char nLangCN;
	
	// �����ɿ��һ����ĸ��
	if( *pSrc == ' ' ){	
		pSrc++;
	}
	
	nCmdType = PLS_GetCommandType( pSrc ); 
	*pCodeType = 0;	
	nLangCN = 0;
	
	if( !IsAuthorizationNo(pCaller) ){
		if( nCmdType == SMS_SETUP_HOME ||
		  	nCmdType == SMS_CONTROL_LISTEN||
		  	nCmdType == SMS_VERSION ||
		  	nCmdType == SMS_SERIAL_SN ||
		  	nCmdType == SMS_SETUP){ 
		  	//continue	
		}
		else if( nCmdType < SMS_CMD_END){
			strcpy( (char*)pDest, (char*)TEXT_NOT_AUTHORIZED );		
			return strlen((char*)pDest);			
		}
		else{
			*pDest = 0x00;
			return 0;
		}
	}
	else{
		if( nCmdType >= SMS_CMD_END ){			
			strcpy( (char*)pDest, (char*)TEXT_CMD_ERROR );
			return strlen((char*)pDest);
		}
		else if( nCmdType == SMS_DEFAULT   ||
				 nCmdType == SMS_SETUP_SP1 ||
				 nCmdType == SMS_SETUP_SP2 ||
				 nCmdType == SMS_SETUP_SP3 ||
				 nCmdType == SMS_SETUP_PSD ){			
			if( !IsMasterNo(pCaller) ){
				strcpy( (char*)pDest, (char*)TEXT_NOT_AUTHORIZED );	
				return strlen((char*)pDest);
			}			
		}
	}	
	switch( nCmdType ){	
	case SMS_RESTART:
		TE_SetNeedTEReset(  );
		sprintf( (char*)pDest, "%s %s", g_strSetCmd[nCmdType], TEXT_SUCCESS );	
		nResult = strlen( (char*)pDest );
		break;					   
		
	case SMS_DEFAULT:
		TE_SetNeedWriteFlash();
		InitializeAlarmFlash();
		InitializeGSMFlash();
		sprintf( (char*)pDest, "%s %s", g_strSetCmd[nCmdType], TEXT_SUCCESS );	
		nResult = strlen( (char*)pDest );
		break;					   
		
	case SMS_SERIAL_SN:		
		sprintf( (char*)pDest, "%s:%s",TEXT_SN,g_stGSMSetup.m_strUID );		
		nResult = strlen( (char*)pDest );
		break;
		
	case SMS_QUERY_INF:
		nResult = PLS_ReadState( pDest, nLangCN );
		break;					  
	case SMS_VERSION:	
		sprintf( (char*)pDest, "%s:%s",	TEXT_VERSION, g_strVer );	
		nResult = strlen( (char*)pDest );	
		break;					
		   
	case SMS_POSITION:
		nResult = PLS_ComposePosition( pDest, nLangCN );
		sprintf( (char*)(pDest+nResult), "%s?lng=%.6f&lat=%.6f&key=%d",   TEXT_GOGPS,
															     		  (double)GPS_GetLongitude()/3600000,
															     		  (double)GPS_GetLatitude()/3600000,
															     		  GPS_CalculateData() );
		nResult = strlen( (char*)pDest );	 
		break;					   
		
	case SMS_ALARM_ENABLE:	// ��������
		nResult = PLS_SetupAlarmEnable( pDest, 1, nLangCN );			
		break;				   
	case SMS_ALARM_DISABLE:	// �رձ���
		nResult = PLS_SetupAlarmEnable( pDest, 0, nLangCN ); 	
		break;				   
		
	case SMS_PER_SPACE:			// ���ټ��
		TE_SetNeedWriteFlash();
		g_stGSMSetup.m_nCBMode = CB_BYTIME;
		g_stGSMSetup.m_nCBSpace = atoi((char*)pSrc);
		if ( g_stGSMSetup.m_nCBSpace < 30  ){
			g_stGSMSetup.m_nCBSpace = 30;
		}
		sprintf( (char*)pDest, "%s %s %d(s)", TEXT_AUTOTRACK, TEXT_EVERY, g_stGSMSetup.m_nCBSpace );
		nResult = strlen( (char*)pDest );
		break;					   
	case SMS_SETUP_PARKING:	// ͣ������
		nResult = PLS_SetupAlarmParking( pDest, pSrc, nLangCN );
		break;				  
	case SMS_DOOR_TIME:  		// ���ű���ʱ��
		nResult = PLS_SetupAlarmAntisteal( pDest, pSrc, nLangCN );
		break;				    
	case SMS_OVERSPEED:			// ���ٱ���
		nResult = PLS_SetupAlarmOverspeed( pDest, pSrc, nLangCN );
		break;					
	case SMS_TOW_ENABLE:
		nResult = PLS_SetupAlarmTow( pDest, (unsigned char*)"1", nLangCN );
		break;					   
	case SMS_TOW_DISABLE:
		nResult = PLS_SetupAlarmTow( pDest, (unsigned char*)"0", nLangCN );
		break;				  	
	case SMS_CLOSE_OIL:			// ����
		nResult =PLS_CtrlOil( pDest, 0, nLangCN);		
		break;					   	
	case SMS_RESUME_OIL:			// �ָ���·
		nResult =PLS_CtrlOil( pDest, 1, nLangCN ); 		
		break;					 
		  	
	case SMS_CONTROL_LISTEN:
		pDest[0] = 0;
		nResult = PLS_CarListen( pDest, pSrc, nLangCN );
		break;	
		
	case SMS_SETUP_SP1:
		nResult = PLS_SetupSlaveNo( pDest, pSrc, 1, nLangCN );
		break;					  
	case SMS_SETUP_SP2:
		nResult = PLS_SetupSlaveNo( pDest, pSrc, 2, nLangCN );
		break;					  
	case SMS_SETUP_SP3:
		nResult = PLS_SetupSlaveNo( pDest, pSrc, 3, nLangCN );
		break;					
		
	case SMS_SETUP_HOME:
		nResult = PLS_SetupMasterNo( pDest, pSrc, nLangCN );
		break;			
	case SMS_SETUP_CSCA:
		nResult = PLS_SetupSMCNo( pDest, pSrc, nLangCN );
		break;			
	case SMS_SETUP_PSD:
		nResult = PLS_SetupPassword( pDest, pSrc, '*', nLangCN );
		break;	
		
	case SMS_SETUP_IP:	   
	case SMS_SETUP_DNSR:
		nResult = PLS_SetupServer( pDest, pSrc, ';', nLangCN  );
		break;			   	
	case SMS_SETUP_APN:
		nResult = PLS_SetupAPN( pDest, pSrc, nLangCN  );
		break;			
	case SMS_NET_SETUP:		// �����û���������
		nResult = PLS_SetupAPNUserPsd( pDest, pSrc, ',', nLangCN); 
		break;					 		
		
	case SMS_OPEN_TRACK:		
		TE_SetNeedWriteFlash();
		g_stGSMSetup.m_nCBMode = CB_BYTIME;
		sprintf( (char*)pDest, "%s %s %d(s)", TEXT_AUTOTRACK, TEXT_EVERY, g_stGSMSetup.m_nCBSpace );
		nResult = strlen((char*)pDest);
		break;					  
	case SMS_CLOSE_TRACK:
		TE_SetNeedWriteFlash();
		g_stGSMSetup.m_nCBMode = CB_STOP;
		sprintf( (char*)pDest, "%s %s", TEXT_AUTOTRACK, TEXT_DISABLE );	
		nResult = strlen((char*)pDest);	
		break;
						   
	case SMS_SETUP_GP:
		nResult = PLS_SetupWorkMode( pDest, pSrc, nLangCN );
		break;		   			
		
	case SMS_AUTO_TIME:
		nResult = PLS_SetupDefensive( pDest, pSrc, nLangCN );
		break;				    
	case SMS_GPRS:
		if( strcmpnc( pSrc, (unsigned char*)"TCP") ){
			nResult = PLS_SetupWorkMode( pDest, (unsigned char*)"2A", nLangCN );			
		}
		else{
			nResult = PLS_SetupWorkMode( pDest, (unsigned char*)"2B", nLangCN );				
		} 
		break;			
	case SMS_FORWARD:
		nResult = PLS_ForwardMsg( pCaller, pDest, pSrc, nLangCN );
		break;					
	case SMS_EXCUTE:
		nResult = PLS_ExecuteExtCmd( pDest, pSrc, nLangCN );
		break;		
	case SMS_IMEI:
		strcpy( (char*)pDest, "IMEI: ");
		nTmp = strlen((char*)pDest);
		CMM_QueryIMEI( pDest+nTmp );
		nResult = strlen((char*)pDest);
		break;
	case SMS_POWEROFF:	//������رյ�Դ
		sprintf((char*)pDest, "%s %s ",TEXT_POWEROFF , TEXT_ALARM );
		if( atoi((char*)pSrc) == 0 ){ //�ر�
			AL_DisablePowerOff();
			strcat((char*)pDest, TEXT_DISABLE );
		}
		else{						  //����	
			AL_EnablePowerOff();
			strcat((char*)pDest, TEXT_ENABLE );
		}
		TE_SetNeedWriteFlash();	
		nResult = strlen((char*)pDest);
		break;
	case SMS_LOWPOWER:  //������رյ͵�Դ 	
		sprintf((char*)pDest, "%s %s ",TEXT_LOW_POWER , TEXT_ALARM );
		if( atoi((char*)pSrc) == 0 ){ //�ر�
			AL_DisableLowPower();
			strcat((char*)pDest, TEXT_DISABLE );
		}
		else{						  //����	
			AL_EnableLowPower();
			strcat((char*)pDest, TEXT_ENABLE );
		}		
		TE_SetNeedWriteFlash();	
		nResult = strlen((char*)pDest);
		break;
	case SMS_SETUP:		//�����豸�Ĳ���	
		PLS_SetupDeviceInfo( pDest, pSrc );
		TE_SetNeedWriteFlash();
		break;	
	case  SMS_SERVER_DNSR:	//��������
		nResult = PLS_SetupServerDNSR( pDest, pSrc, nLangCN );
		break;
	case SMS_ASLIS_SETUP: 
		if( atoi((char*)pSrc) == 0){ //�����ż����뱨��
			TE_ClrAntiSteal();
			TE_SetASILTrigger();	 //������ 	
			// 100��,��ACC
			g_stAlarmSetup.m_nILLDoorOpen = 100;   
			// antisteal alarm input: door
			if( nLangCN ){
				sprintf((char*)pDest, "%s%s%s", TEXT_ANTISTEAL_CN, TEXT_ALARM_CN,TEXT_INPUT_CN);
				nTmp = strlen((char*)pDest);
				nTmp += unicodecpy( pDest+nTmp, (unsigned char*)":" );
				strcpy( (char*)pDest+nTmp, TEXT_DOOR_CN );
				nTmp +=strlen(TEXT_DOOR_CN);
				nResult = nTmp;
			}
			else{
				sprintf((char*)pDest, "%s %s %s:%s", TEXT_ANTISTEAL, TEXT_ALARM, TEXT_INPUT,TEXT_DOOR);
				nResult = strlen( (char*)pDest );
			}
		}
		else{						 //����ԭ����������				
			TE_SetAntiSteal();			
			TE_SetASIHTrigger();	 //������
			// ��������5��
			g_stAlarmSetup.m_nILLDoorOpen = 5;
			// antisteal alarm input: external alarm system ԭ������
			if( nLangCN ){
				sprintf((char*)pDest, "%s%s%s", TEXT_ANTISTEAL_CN, TEXT_ALARM_CN,TEXT_INPUT_CN);
				nTmp = strlen((char*)pDest);
				nTmp += unicodecpy( pDest+nTmp, (unsigned char*)":" ); 
				strcpy( (char*)pDest+nTmp, TEXT_ENTAL_SYSTEM_CN );
				nTmp +=strlen(TEXT_ENTAL_SYSTEM_CN);
				nResult = nTmp;				
			}
			else{
				sprintf((char*)pDest, "%s %s %s:%s", TEXT_ANTISTEAL, TEXT_ALARM,TEXT_INPUT,TEXT_ENTAL_SYSTEM );
				nResult = strlen((char*)pDest);
			}
		}
		TE_SetNeedWriteFlash();
		break;		
	case SMS_REINPUT:	//�����Զ���ߵ͵�ƽ	
		nResult = PLS_SetReInput( pDest, pSrc, TEXT_COLON, nLangCN );
		TE_SetNeedWriteFlash( );			
		break;
	default:
		break;	
	}
	return 	nResult;												  	
}				

/////////////////////////////////////////////////////////////////////////////
// �绰����
unsigned char PLS_CarListen( unsigned char  *pDest,	unsigned char  *pSrc, unsigned char nLangCN ){

	unsigned char		*pTmp = NULL;
			
	pTmp = (unsigned char*)strchr( (char*)pSrc, '*');	
	
	if ( pTmp == NULL ){
		if( nLangCN ){			
			strcpy( (char*)pDest, TEXT_PARA_ERROR_CN );		
		}
		else{
			strcpy( (char*)pDest, TEXT_PARA_ERROR );				
		}
	}
	else{ 
		*pTmp = 0x00;
		pTmp++;
		if( strcmp((char*)pSrc, (char*)g_stGSMSetup.m_strPassword) == 0x00 ){
			
			CMM_DialNumber( pTmp );	
		}
		else{
			if( nLangCN ){			
				strcpy( (char*)pDest, TEXT_PSD_ERROR_CN );		
			}
			else{
				strcpy( (char*)pDest, TEXT_PSD_ERROR );				
			}	
		}				
	}
	return strlen((char*)pDest);
} 

/////////////////////////////////////////////////////////////////////////////
//	�������غ���  *h*1234*13480924411#
unsigned char PLS_SetupMasterNo( unsigned char  *pDest, unsigned char  *pSrc, unsigned char nLangCN ){

	unsigned char  nLen = 0;
	unsigned char  *pTmp = NULL;
	unsigned char	 nResult = 0;
		
	pTmp = (unsigned char*)strchr( (char*)pSrc, '*' );
	if ( pTmp == NULL ){
		if( nLangCN){			
			strcpy( (char*)pDest, TEXT_PARA_ERROR_CN );		
		}
		else{
			strcpy( (char*)pDest, TEXT_PARA_ERROR );				
		}
		nResult = strlen( (char*)pDest );
	}
	else{
		*pTmp = 0x00;	
		pTmp++;	
		if( strcmp((char*)pSrc, (char*)g_stGSMSetup.m_strPassword) == 0x00 ){
			
			nLen = strlen( (char*)pTmp );
			if( nLen >= TELNUMBER_LENGTH ){
				nLen = TELNUMBER_LENGTH;
			}
			strncpy( (char*)g_stGSMSetup.m_strMaterNo, (char*)pTmp, nLen );
			g_stGSMSetup.m_strMaterNo[nLen] = 0x00;
						
			if( nLangCN ){		
				sprintf( (char*)pDest, "%s%s", TEXT_MASTER_CN, TEXT_NO_CN );	
				nResult = strlen( (char*)pDest );
				nResult += unicodecpy( pDest+nResult, (unsigned char*)":" );
				nResult += unicodecpy( pDest+nResult, g_stGSMSetup.m_strMaterNo );
			}
			else{
				sprintf( (char*)pDest, "%s %s:%s", TEXT_MASTER, TEXT_NO, g_stGSMSetup.m_strMaterNo );					
				nResult = strlen( (char*)pDest );
			}				
			TE_SetNeedWriteFlash( );	
		}
		else{
			if( nLangCN ){			
				strcpy( (char*)pDest, TEXT_PSD_ERROR_CN );		
			}
			else{
				strcpy( (char*)pDest, TEXT_PSD_ERROR );				
			}		
			nResult = strlen( (char*)pDest );			
		}				
	}
	return nResult;
}

#endif	 	//SW_SMSPROTOCOLA


