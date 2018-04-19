///////////////////////////////////////////////////////////////////////////
//   file name: ProtocolData.h
// description: GSM/GPRSЭ��ͷ�ļ�����
//      author:
//        date: 2009-02-15
//     reamark: 
#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>
#include  "..\hdr\includes.h"
#include  "..\hdr\ProtocolA.h"
#include  "..\hdr\ProtocolB.h"
#include  "..\hdr\ProtocolC.h"
#include  "..\hdr\ProtocolASICO.h"
#include  "..\hdr\APP_LoopDetect.h" 
#include  "..\hdr\APP_Terminal.h"
#include  "..\hdr\HW_GPIO.h"
#include  "..\hdr\APP_GSM.h"
#include  "..\hdr\APP_GTM900.h"
#include  "..\hdr\APP_BQM26.h"
#include  "..\hdr\APP_EM200.h"
#include  "..\hdr\APP_MC323.h"
#include  "..\hdr\APP_SIMCOM.h"
#include  "..\hdr\APP_GPS.h"
#include  "..\hdr\SMSProtocolA.h"
#include  "..\hdr\SMSProtocolB.h"
#include  "..\hdr\SMSProtocolC.h"

#ifdef	SW_GPRSPROTOCOLB
static unsigned char		s_sequnce = 0;
/////////////////////////////////////////////////////////////////////////////
//  ���ӱ�ͷ
void  PL_AddHead( unsigned char	*pBuf ){
	
	*pBuf++ = SYN_HEADERCODE1;
	*pBuf = SYN_HEADERCODE2;	
}
/////////////////////////////////////////////////////////////////////////////
//  ����GPS����
void  PL_AddGPSData( unsigned char *pDataBuf ){
	
	int			nTmpData = 0;
		
	//ʱ��
	pDataBuf[0] = g_stGPSInfo.m_nYear;
	pDataBuf[1] = g_stGPSInfo.m_nMonth;
	pDataBuf[2] = g_stGPSInfo.m_nDay;
	pDataBuf[3] = g_stGPSInfo.m_nHour;
	pDataBuf[4] = g_stGPSInfo.m_nMinute;
	pDataBuf[5] = g_stGPSInfo.m_nSecond;
	//��λ��־
	pDataBuf[6] = g_stGPSInfo.m_bPosValid;
	//����	
	nTmpData = GPS_GetLongitude();	
	memcpy( &pDataBuf[7], &nTmpData, 4);
	//γ��
	nTmpData = GPS_GetLatitude();	
	memcpy( &pDataBuf[11], &nTmpData, 4);
	//����
	memcpy( &pDataBuf[15],  &g_stGPSInfo.m_nCurDirection, 2 );
	//�ٶ�	
	nTmpData = GPS_GetMilimetre();	
	pDataBuf[17] =  nTmpData & 0xff;	
	
}
/////////////////////////////////////////////////////////////////////////////
//  ��ȡӲ��״̬/��ȡ����״̬/����ʱ��/״̬��
void  PL_AddDEState( unsigned char *pDataBuf ){

	unsigned int  	nTmp = 0;
		
	// GPS�ź�
	if( (g_stGPSInfo.m_nGPSSignal >= 1) &&
	    (g_stGPSInfo.m_nGPSSignal <= 3) ){		//�źŲ�
	    g_stDeviceState.m_nHWState |= HW_GPS_SIGNAL1;
	    g_stDeviceState.m_nHWState &= ~HW_GPS_SIGNAL2;
	}
	else if( (g_stGPSInfo.m_nGPSSignal >= 4) &&	    
	    	 (g_stGPSInfo.m_nGPSSignal <= 5) ){ //�ź�һ��
	    g_stDeviceState.m_nHWState &= ~HW_GPS_SIGNAL1;	 
	    g_stDeviceState.m_nHWState |= HW_GPS_SIGNAL2;
	}
	else if ( g_stGPSInfo.m_nGPSSignal > 5 ){   //�ź�ǿ
		g_stDeviceState.m_nHWState |= HW_GPS_SIGNAL1;
		g_stDeviceState.m_nHWState |= HW_GPS_SIGNAL2;
	}
	else{										//û���ź�	
		g_stDeviceState.m_nHWState &= ~HW_GPS_SIGNAL1;
		g_stDeviceState.m_nHWState &= ~HW_GPS_SIGNAL2;
	}
	// GSM�ź�	
	if( (g_stGSMState.m_nGSMSignal >= 8) &&  
	    (g_stGSMState.m_nGSMSignal <= 15) ){	 //�źŲ�
	    g_stDeviceState.m_nHWState |= HW_GSM_SIGNAL1;
	    g_stDeviceState.m_nHWState &= ~HW_GSM_SIGNAL2;
	}
	else if( (g_stGSMState.m_nGSMSignal >= 16) && 
	    (g_stGSMState.m_nGSMSignal <= 23) ){ 	//�ź�һ��
	    g_stDeviceState.m_nHWState &= ~HW_GSM_SIGNAL1;
	    g_stDeviceState.m_nHWState |= HW_GSM_SIGNAL2; 	    
	}
	else if( (g_stGSMState.m_nGSMSignal >= 24) &&
	    	 (g_stGSMState.m_nGSMSignal <= 31) ){ //�ź�ǿ
	    g_stDeviceState.m_nHWState |= HW_GSM_SIGNAL1;
	    g_stDeviceState.m_nHWState |= HW_GSM_SIGNAL2; 
	}
	else{										  //û���ź�
		g_stDeviceState.m_nHWState &= ~HW_GSM_SIGNAL1;
	    g_stDeviceState.m_nHWState &= ~HW_GSM_SIGNAL2;
	}
	// ��Դ��ѹ
	nTmp = g_stDeviceState.m_nBattery*170/20; 
	nTmp /= 1000; 
	nTmp &= 0x1F;
	nTmp = nTmp<<24;
	g_stDeviceState.m_nHWState &= ~0x1F000000;
	g_stDeviceState.m_nHWState |= nTmp;	
	memcpy( &pDataBuf[0], &g_stDeviceState.m_nHWState, 4);
	memcpy( &pDataBuf[4], &g_stDeviceState.m_nALState, 4);
	// ACC����ʱ�����
	nTmp = g_stAlarmSetup.m_nDriveTime / 120;
	memcpy( &pDataBuf[8], &nTmp, 2);
	memcpy( &pDataBuf[10],&g_stDeviceState.m_nCodeState, 1);
}
/////////////////////////////////////////////////////////////////////////////
// 
void  PL_AddIpAddrN( unsigned char	*pDataBuf ){
	
	unsigned char	*pData  = NULL;
	unsigned char	*pData1 = NULL;
	
	pData = g_stGSMSetup.m_strServerIP;
	
	// 1
	pData1 = (unsigned char*)strchr((char*)pData, TEXT_POINT );
	if ( pData1 == NULL ){
		return;
	}
	pDataBuf[0] = (unsigned char)atoi( (char*)pData );
	pData = pData1 + 1;
	
	// 2	
	pData1 = (unsigned char*)strchr( (char*)pData, TEXT_POINT );
	if ( pData1 == NULL ){
		return ;
	}
	pDataBuf[1] = (unsigned char)atoi((char*)pData );
	pData = pData1 + 1;
	
	// 3
	pData1 = (unsigned char*)strchr( (char*)pData, TEXT_POINT );
	if ( pData1 == NULL ){
		return ;
	}	
	pDataBuf[2] = (unsigned char)atoi( (char*)pData );
	pData = pData1 + 1;
	// 4
	pDataBuf[3] = (unsigned char)atoi( (char*)pData );
}

/////////////////////////////////////////////////////////////////////////////
//  ��֯GPS����
unsigned char 	 PL_ComposeGPSData( unsigned char  *pDest ){	

	unsigned char			nLen  = 0;
									 	
	//  ��ͷ(2)
	PL_AddHead( pDest );
	//  ��Ϣ����(1)
	pDest[WHERE_MESSAGE_TYPE] = MT_SUBMIT;	 
	//  �ܳ���(1)
	pDest[WHERE_MESSAGE_TOTAL_LENGHT] = 40;
	//  �ܰ���(1)
	pDest[WHERE_MESSAGE_SEQNUM] = s_sequnce++;	
	//  DEUID����(1)+DEUID����(n)
	PL_AddTEUID( &pDest[5] );
	//  ��������(1)
	pDest[WHERE_MESSAGE_DATA_TYPE] = CC_GPS_STATE;	
	//  GPS���ݳ���+Ӳ��״̬
	pDest[15] = 29;	
	//  GPS����(18)
	PL_AddGPSData( &pDest[16] );
	//  Ӳ��״̬(11)
	PL_AddDEState( &pDest[34] );
	//  У���
	pDest[45] = PL_CountVerifySum(pDest, 45);
	nLen = 46;
	
	return nLen;
}
/////////////////////////////////////////////////////////////////////////////
//  ��֯googleGPS����
unsigned char 	 PL_ComposeGGGPSData( unsigned char  *pDest, unsigned char	nCmdType ){	

	unsigned char			nLen  = 0;
									 	
	//  ��ͷ(2)
	PL_AddHead( pDest );
	//  ��Ϣ����(1)
	pDest[WHERE_MESSAGE_TYPE] = MT_SUBMIT;	 
	//  �ܳ���(1)
	pDest[WHERE_MESSAGE_TOTAL_LENGHT] = 40;
	//  �ܰ���(1)
	pDest[WHERE_MESSAGE_SEQNUM] = s_sequnce++;	
	//  DEUID����(1)+DEUID����(n)
	PL_AddTEUID( &pDest[5] );
	//  ��������(1)
	pDest[WHERE_MESSAGE_DATA_TYPE] = nCmdType;	
	//  GPS���ݳ���+Ӳ��״̬
	pDest[15] = 29;	
	//  GPS����(18)
	PL_AddGPSData( &pDest[16] );
	//  Ӳ��״̬(11)
	PL_AddDEState( &pDest[34] );
	//  У���
	pDest[45] = PL_CountVerifySum(pDest, 45);
	nLen = 46;
	
	return nLen;
}

#ifdef EXTERNAL_FLASH
///////////////////////////////////////////////////////////////////////////////////
//	 ��֯ä�������ϴ�
//		pDest :	  �������
//		pSrc  :   �������
//nPackageSize:   �����ݴ�С
// ��������   :   �������ݳ���
unsigned char	PL_ComposeGPSBlindData( unsigned char	*pDest, 
										unsigned char	*pSrc,  
										unsigned char	nPackageSize ){

	unsigned char	nCnt;
	unsigned char	nLen;
    unsigned char	nMessageType = 0;
	
	//  ��ͷ(2)
	PL_AddHead( pDest );
	//  ��Ϣ����(1)
	nMessageType = MT_SUBMIT;
	if( nPackageSize > 1 ){		
		nMessageType |= 0x80;
	}		
	pDest[WHERE_MESSAGE_TYPE] = nMessageType;	 
	//  �ܳ���(1)
	pDest[WHERE_MESSAGE_TOTAL_LENGHT] = 0x00;
	//  �ܰ���(1)
	pDest[WHERE_MESSAGE_SEQNUM] = s_sequnce++;	
	//  DEUID����(1)+DEUID����(n)
	PL_AddTEUID( &pDest[5] );
	
	if( nMessageType & 0x80 ){
		// �ܰ���
		pDest[WHERE_MESSAGE_DATA_TYPE] = nPackageSize;
		
		nLen = WHERE_MESSAGE_DATA_TYPE+1;
		for( nCnt = 0; nCnt < nPackageSize; nCnt++ ){
			
			pDest[nLen+nCnt*31] = CC_BLIND_GPS_STATE;
			pDest[nLen+1+nCnt*31] = 29;
			memcpy( &pDest[nLen+2+nCnt*31], &pSrc[nCnt*32], 29 );			
		}
		// GPS���� 29+������+����
		nLen = nPackageSize*31;	
		// ��ͷ(2)+������(1)+�ܳ���(1)+�ۼӺ�(1)+DE����(1)+DE����(8)+����(1)
		nLen += 14+1;					
	}
	else{
		pDest[WHERE_MESSAGE_DATA_TYPE] = CC_BLIND_GPS_STATE;
		pDest[15] = 29;
		memcpy( &pDest[16], pSrc, 29 );
		nLen = 45;
	}
	//  �ܳ���(1) ��ͷ(2)+������(1)+�ܳ���(1)+���(1)+Data����+����(1)
	pDest[WHERE_MESSAGE_TOTAL_LENGHT] = nLen+1-6;
	//  У���	
	pDest[nLen] = PL_CountVerifySum(pDest, nLen );
	//  �ܳ���
	nLen += 1;
	
	return nLen;
}
#endif //EXTERNAL_FLASH

/////////////////////////////////////////////////////////////////////////////
//	��֯GSM������Ϣ
unsigned char	PL_ComposeGSMData( unsigned char	*pDest ){

	unsigned char			nLen = 0;
	int						nLac = 0;
	int						nID = 0;

	//  ��ѯС���ŵĵ�ַ
	CMM_QueryNetworkLac( &nLac, &nID );
	//  ��ͷ(2)
	PL_AddHead( pDest );
	//  ��Ϣ����(1)
	pDest[WHERE_MESSAGE_TYPE] = MT_SUBMIT;	 
	//  �ܳ���(1)
	pDest[WHERE_MESSAGE_TOTAL_LENGHT] = 32;
	//  �ܰ���(1)
	pDest[WHERE_MESSAGE_SEQNUM] = s_sequnce++;	
	//  DEUID����(1)+DEUID����(n)
	PL_AddTEUID( &pDest[5] );
	//  ��������(1)
	pDest[WHERE_MESSAGE_DATA_TYPE] = CC_GSM_STATE;	
	//  GSM���ݳ���+Ӳ��״̬
	pDest[15] = 21;		
	//  GPSʱ��
	pDest[16] = g_stGPSInfo.m_nYear;
	pDest[17] = g_stGPSInfo.m_nMonth;
	pDest[18] = g_stGPSInfo.m_nDay;
	pDest[19] = g_stGPSInfo.m_nHour;
	pDest[20] = g_stGPSInfo.m_nMinute;
	pDest[21] = g_stGPSInfo.m_nSecond;
	//  nLac С�� 2 Byte 
	memcpy( &pDest[22], &nLac, 2 );
	//  nID ��ַ  2 Byte
	memcpy( &pDest[24], &nID, 2 );
	//  ״̬��Ϣ
	PL_AddDEState( &pDest[26] );
	//  У���
	pDest[37] = PL_CountVerifySum(pDest, 37);
	
	nLen = 38;	
	return nLen;
}
/////////////////////////////////////////////////////////////////////////////
// ֯֯������Ϣ
unsigned char 	 PL_ComposeHSData( unsigned char	*pDest ){
	
	unsigned  char	nResultLen = 0;
		
	//  ��ͷ(2)
	PL_AddHead( pDest );
	//  ��Ϣ����(1)
	pDest[WHERE_MESSAGE_TYPE] = MT_HANDSHAKE;	
	//  �ܳ���(1)
	pDest[WHERE_MESSAGE_TOTAL_LENGHT] = 10;
	//  ��Ű���(1)
	pDest[WHERE_MESSAGE_SEQNUM] = s_sequnce++;	
	//  DEUID����(1)+DEUID����(n)
	PL_AddTEUID( &pDest[5] );
	//  ��������(1)
	pDest[WHERE_MESSAGE_DATA_TYPE] = 0;	
	//  У���
	pDest[15] = PL_CountVerifySum(pDest, 15 );	
	nResultLen = 16;
	
	return nResultLen;
}
/////////////////////////////////////////////////////////////////////////////
//
unsigned char	PL_ComposeLogin( unsigned char  *pDest ){ 
	return PL_ComposeHWSWSetup( MT_LOGIN, pDest );	
}
/////////////////////////////////////////////////////////////////////////////
//���������� 
//��ͷ(2)+��Ϣ����(1)+�ܳ���(1)+�ܰ���(1)+DEUID����(1)+DEUID����(n)+��������(1)+���ݳ���(1)+��������(n)+У���(1)
unsigned char	PL_ComposeHWSWSetup( unsigned char nMessageType ,unsigned char  *pDest ){
	
	unsigned short	nTmp = 0;
	
	
	memset( pDest, 0x00, 133 );
	//  ��ͷ(2)
	PL_AddHead( pDest );
	//  ��Ϣ����(1)
	pDest[WHERE_MESSAGE_TYPE] = nMessageType;	
	//  �ܳ���(1)
	pDest[WHERE_MESSAGE_TOTAL_LENGHT] =  127;
	//  ��Ű���(1)
	pDest[WHERE_MESSAGE_SEQNUM] = s_sequnce++;
	//  DEUID����(1)+DEUID����(n)
	PL_AddTEUID( &pDest[5] );
	//  ��������(1)
	pDest[WHERE_MESSAGE_DATA_TYPE] = CC_SW_SETUP;
	
	//	����״̬(4)
	memcpy( &pDest[WHERE_MESSAGE_DATA_ALSTATE], &g_stAlarmSetup.m_nALValid, 4 );
	//  Ӳ��״̬(4)
	memcpy( &pDest[WHERE_MESSAGE_DATA_HWSTATE], &g_stAlarmSetup.m_nHWValid, 4 );
		
	//  ��������(4)
	memcpy( &pDest[WHERE_MESSAGE_ALARM_ENABLE], &g_stAlarmSetup.m_nAlarmEnable, 4 );	
	//  ���ٱ���(1)
	pDest[WHERE_MESSAGE_OVERSPEED] = g_stAlarmSetup.m_nOverspeed;	
	//  ƣ�ͼ�ʻ(1)
	pDest[WHERE_MESSAGE_TIREDNESS] = g_stAlarmSetup.m_nTireDriver;
	//  �Ƿ����ű�������(1)
	pDest[WHERE_MESSAGE_ANTISTERAL] = g_stAlarmSetup.m_nILLDoorOpen;
	//  ͣ������(1)
	memcpy( &pDest[WHERE_MESSAGE_PARKING], &g_stAlarmSetup.m_nParking, 2);
	
	//  ����Χ��1	�����־+����Χ��+�Ƿ����
	memset( &pDest[WHERE_MESSAGE_AREA1], 0x00, 18);	
	//  ����Χ��2   �����־+����Χ��+�Ƿ����
	memset( &pDest[WHERE_MESSAGE_AREA2], 0x00, 18);
		
	//  �������˿ں�(2)
	memcpy( &pDest[WHERE_MESSAGE_SERVERPORT], &g_stGSMSetup.m_nServerPort , 2);
	//  ������IP��ַ(4)
	PL_AddIpAddrN( &pDest[WHERE_MESSAGE_SERVERIP] );	
	//  �ش�ģʽ(3)
	pDest[WHERE_MESSAGE_CBMODE] = g_stGSMSetup.m_nCBMode;
	memcpy(  &pDest[WHERE_MESSAGE_CBMODE+1], &g_stGSMSetup.m_nCBSpace, 2 );
			
	//  ���غ��� 1:���볤��  10:��������
	nTmp = strlen( (char*)g_stGSMSetup.m_strMaterNo );		
	nTmp = P_StrToBCD(&pDest[WHERE_MESSAGE_MATERNO+1], g_stGSMSetup.m_strMaterNo, nTmp ); 
	pDest[WHERE_MESSAGE_MATERNO] = nTmp;
	
	//   ����Ϣ���ĺ��� 1:���볤��  10:��������
	nTmp = strlen( (char*)g_stGSMSetup.m_strSMCNo );		
	nTmp = P_StrToBCD( &pDest[WHERE_MESSAGE_SMCNO+1], g_stGSMSetup.m_strSMCNo, nTmp ); 
	pDest[WHERE_MESSAGE_SMCNO] = nTmp;
	
	//   ��Ȩ����1 1:���볤��  10:��������
	nTmp = strlen( (char*)g_stGSMSetup.m_strAuthNo1 );		
	nTmp = P_StrToBCD( &pDest[WHERE_MESSAGE_AUTHNO1+1], g_stGSMSetup.m_strAuthNo1, nTmp ); 
	pDest[WHERE_MESSAGE_AUTHNO1] = nTmp;
	
	//   ��Ȩ����2 1:���볤��  10:��������
	nTmp = strlen( (char*)g_stGSMSetup.m_strAuthNo2 );		
	nTmp = P_StrToBCD( &pDest[WHERE_MESSAGE_AUTHNO2+1], g_stGSMSetup.m_strAuthNo2, nTmp ); 
	pDest[WHERE_MESSAGE_AUTHNO2] = nTmp;
	
	//   ��Ȩ����3 1:���볤��  10:��������
	nTmp = strlen( (char*)g_stGSMSetup.m_strAuthNo3 );		
	nTmp = P_StrToBCD( &pDest[WHERE_MESSAGE_AUTHNO3+1], g_stGSMSetup.m_strAuthNo3, nTmp ); 
	pDest[WHERE_MESSAGE_AUTHNO3] = nTmp;
	pDest[WHERE_MESSAGE_END] = PL_CountVerifySum( pDest, WHERE_MESSAGE_END);
	
	return WHERE_MESSAGE_END+1;
}

///////////////////////////////////////////////////////////////
// 	����Э������
unsigned char	PL_ParseData(unsigned char *pDest, unsigned char *pSrc, unsigned char nLen ){
									 	
	unsigned char	 	nResultLen = 0;
	unsigned char		nSequnce = 0;
	unsigned char		nDEUID[DEUID_LENGTH+1];
			
	if( pSrc[WHERE_MESSAGE_HEAD1] != SYN_HEADERCODE1 ||
			pSrc[WHERE_MESSAGE_HEAD2] != SYN_HEADERCODE2 ){
		return 0;
	}		
	
	nSequnce = pSrc[WHERE_MESSAGE_SEQNUM];
	
	s_sequnce = nSequnce;
	
	// DEUID compare
	nLen = pSrc[WHERE_MESSAGE_DEUIDLEN];
	if( nLen > DEUID_LENGTH ){
		nLen = DEUID_LENGTH;
	}
	memcpy( (char*)nDEUID, (char*)&pSrc[WHERE_MESSAGE_DEUID], nLen );
	nDEUID[nLen] = 0x00;	
	if( !strcmp_back( g_stGSMSetup.m_strUID, nDEUID, nLen ) ){
		return 0;
	}
			
	// ��������		
	switch( pSrc[WHERE_MESSAGE_TYPE] ){			
	case MT_DELIVERY:
		
		nResultLen = PL_ParseDelivery( pDest, pSrc+WHERE_MESSAGE_DATA_TYPE );						
		break;
	
	// ����Ϊ��½�ɹ�
	case MT_LOGIN_ACK:		
		TE_SetLoginSuccess( );	
		UDP_ClrHandShakeCnt( );
		TCP_ClrRCntionCnt();
		IP_ClrConnectFailedCnt( );
		break;
		
	// �յ�����Ӧ������
	case MT_ACK:				
		UDP_ClrHandShakeCnt( );	
		TCP_ClrRCntionCnt();
		IP_ClrConnectFailedCnt( );
		break;	
	}
	return nResultLen;									 	
}
////////////////////////////////////////////////////////////////////////////
//   �����ն�������Ϣ  MT_DELIVERY
unsigned char	PL_ParseDelivery( unsigned char *pDest, unsigned char *pSrc ){								     	
	
	unsigned char		nFlag	= 0;
	unsigned char		nLen    = 0;
	unsigned char		nGSMState = 0;
	unsigned char		nCodeState = 0;
	unsigned char 		nCaller[TELNUMBER_LENGTH+1];

	nCodeState = *pSrc++;	
	switch( nCodeState ){
	case  CC_TE_QUERY_SW_SETUP:									// 6.	��ȡ���������Ϣ
		nLen = PL_ComposeHWSWSetup( MT_SUBMIT ,pDest );	
		g_stDeviceState.m_nCodeState = 0;
		nFlag = 1;	
		break;
		
	case  CC_TE_CTRLOIL:										// 7.	�ر�/�����·��Ϣ
		pSrc++;   												// ����
		if( *pSrc == TE_CTRL_DISABLE ){							// ����	
			HW_CTRLOIL_CUTOFF();
			HW_SetOilState();	
			GSM_SetOilState();
			g_stDeviceState.m_nCodeState = ACK_DISABLE_OIL;
		}
		else{																			// �ָ���·
			HW_ClrOilState( );
			HW_CTRLOIL_RESUME();
			GSM_ClrOilState();
			g_stDeviceState.m_nCodeState = ACK_ENABLE_OIL;
		}	
		TE_SetNeedWriteFlash( );	
		nFlag = 1;
		break;
		
	case  CC_TE_CTRLDOOR:										//8.	��/�� ����
		pSrc++;
		if ( *pSrc == TE_CTRL_DISABLE ){						//�Ź�
			HW_ClrDoorState( );
			TE_SetNeedLockFlag();		//��
			g_stDeviceState.m_nCodeState = ACK_CLOSE_DOOR;
		}
		else{																			//�ſ�
			HW_SetDoorState( );
			TE_SetNeedUnLockFlag();		//����	
			g_stDeviceState.m_nCodeState = ACK_OPEN_DOOR;	
		}	
		nFlag = 1;
		break;
	
	case  CC_TE_CALLONCE:										//9.	��������: (�����ش�������Ϣ)			
		g_stDeviceState.m_nCodeState = ACK_LOCATION;
		nFlag = 1;
		break;		
	case CC_GSM_STATE:		// GSM��λ��Ϣ
		g_stDeviceState.m_nCodeState = ACK_LOCATION;
		nFlag = 1;	
		nGSMState = 1;
		break;	
	case  CC_TE_LISTEN:						//10.	�绰����: (�򿪳����ն˻�Ͳ��Զ�̼������ڶ���)
		nLen = *pSrc++;
		if( nLen > TELNUMBER_LENGTH ){
			nLen = TELNUMBER_LENGTH;
		}
		if( nLen == 0 ){
			nFlag = 0;			
		}
		else{
			memcpy( (char*)nCaller, pSrc, nLen );
			nCaller[nLen] = 0x00;
			#ifdef HW_EM200	
			CMM_IPClose();
			CMM_DTUPPPClose();
			IP_SetConnectNG( );	
			#endif	
			#ifdef HW_MC323
			CMM_IPClose();
			CMM_DTUPPPClose();
			IP_SetConnectNG( );	
			#endif		
			if( CMM_DialNumber( nCaller ) == GSM_SUCCEED ){
				nLen = 0;
				nFlag = 1;
			}
			else{				
				nLen = 0;
				nFlag = 0;
			}
		}		
		g_stDeviceState.m_nCodeState = ACK_LISTEN;
		break;
	// �ݲ�֧��
	case  CC_TE_TALK:						//11.	�绰ͨ��: (�򿪳����ն˻�Ͳ��Զ�̺���)
		nLen = *pSrc++;
		if( nLen > TELNUMBER_LENGTH ){
			nLen = TELNUMBER_LENGTH;
		}
		if( nLen == 0 ){
			nFlag = 0;			
		}
		else{
			nFlag = 1;
			memcpy( (char*)nCaller, pSrc, nLen );			
			nCaller[nLen] = 0x00;
			#ifdef HW_EM200
			CMM_IPClose();
			CMM_DTUPPPClose();
			IP_SetConnectNG( );	
			#endif			
			#ifdef HW_MC323
			CMM_IPClose();
			CMM_DTUPPPClose();
			IP_SetConnectNG( );	
			#endif
			if( CMM_DialNumber( nCaller ) == GSM_SUCCEED ){
				nLen = 0;
				nFlag = 1;
			}
			else{				
				nLen = 0;
				nFlag = 0;
			}
			nCaller[nLen] = 0x00;		
			#ifdef HW_ET201B
			HW_PORT_SPKMUTE_OPEN();  //�������� 
			#endif		
			#ifdef HW_ET201B_2
			HW_PORT_SPKMUTE_OPEN();  //��������
			#endif
			#ifdef HW_ET201B_3
			HW_PORT_SPKMUTE_OPEN();  //��������
			#endif
		}
		g_stDeviceState.m_nCodeState = ACK_TALK;
		break;
	case  CC_TE_SETUP_UPLOADMODE: 		    //12.	���������ϴ�ģʽ
		nFlag = PL_TESetupCBMode( pSrc );
		if( nFlag ){
			TE_SetNeedWriteFlash( );
		}
		g_stDeviceState.m_nCodeState = ACK_SETUP_MODE;
		break;
		
	case  CC_TE_SETUP_NUMBER:				//13.	���ø��ֺ���
		nFlag = PL_TESetupNumber( pSrc );
		if( nFlag ){
			TE_SetNeedWriteFlash( );
		}
		g_stDeviceState.m_nCodeState = ACK_SETUP_PHONE;
		break;
	case  CC_TE_SETUP_SERVER:				//14.	���÷����� 
		nFlag = PL_TESetupServer( pSrc );
		if( nFlag ){
			TE_SetNeedWriteFlash( );
		}
		g_stDeviceState.m_nCodeState = ACK_SETUP_SERVER;
		break;
	case  CC_TE_SETUP_ALARM:				//15.	��������
		nFlag = PL_TESetupAlarm( pSrc );
		if( nFlag ){
			TE_SetNeedWriteFlash( );
		}
		g_stDeviceState.m_nCodeState = ACK_SETUP_ALARM;
		break;
	case  CC_TE_RESET:						//16.	�ն˸�λ
		TE_SetNeedTEReset( );
		g_stDeviceState.m_nCodeState = ACK_SETUP_RESET;
		nFlag = 1;
		break;
	case  CC_TE_RESTORE_FACTORY:			//17.	��㹳�������	
		InitializeAlarmFlash();
		InitializeGSMFlash();	
		TE_SetNeedWriteFlash();
		nFlag = 1;
		g_stDeviceState.m_nCodeState = ACK_RESTORE_FACTORY;
		break;	
	case CC_TE_FENCE:					    //18.   ���õ���Χ�� 		
		nLen = 0;
		nFlag = 1;
		PL_TESetupFence( pSrc );
		TE_SetNeedWriteFlash();
		g_stDeviceState.m_nCodeState = ACK_SETUP_FENCE; 
		break;
	//  �õ�GG��ַ	
	case CC_TE_RESULT_GG_ADDR:	
		nLen = *pSrc++;
		nLen -= 1;
		// ����ģʽ 0:���� 1:Ӣ��
	    if( *pSrc++ == 0 ){
	    	GSM_SetLangState();
	    }
	    else{
	    	GSM_ClrLangState();
	    }	    
	    memcpy( pDest,pSrc, nLen );	
	    pDest[nLen] = 0x00;
	    TE_SetResultGGAddrFlag();
		break;
	}		
	
	if( nCodeState != CC_TE_RESULT_GG_ADDR ){
	
		if ( nLen == 0 ){
			if ( nFlag ){
				g_stDeviceState.m_nCodeState |= 0x80;
			}
			else{
				g_stDeviceState.m_nCodeState &= ~0x80;
			}
			if( nGSMState == 1){
				nLen = PL_ComposeGSMData( pDest ); 
			}
			else{
				nLen = PL_ComposeGPSData( pDest );
			}
			g_stDeviceState.m_nCodeState = 0;
		}
		else{
			nFlag = 0;
			nLen = 0;
		}
	}	
	return nLen;			     	
}
/////////////////////////////////////////////////////////////////////////////
//	���õ���Χ�� 
unsigned char	PL_TESetupFence( unsigned char	*pDataMsg ){

	unsigned char	nFence = 0;
	unsigned char	nCnt   = 0;
	unsigned char	nLen = 0;
	
	nLen = *pDataMsg++;
	nFence = *pDataMsg++;
	nCnt = nFence & 0x1F;
	if( nCnt > 2 ){
		nCnt = 0;
	}	 
	else{
		nCnt -= 1;
	}
	g_stAlarmSetup.m_oStGFence[nCnt].m_nGEOFence = 0;
	g_stAlarmSetup.m_oStGFence[nCnt].m_nFlag = nFence;	
	// ���ӻ�ɾ��
	if( (nFence & 0x40) == 0x00 ){
		g_stAlarmSetup.m_oStGFence[nCnt].m_RLat = 0;
		g_stAlarmSetup.m_oStGFence[nCnt].m_RLng = 0;
		g_stAlarmSetup.m_oStGFence[nCnt].m_LLat = 0;
		g_stAlarmSetup.m_oStGFence[nCnt].m_LLng = 0;
//		if( nFence & 0x80 ){
//			AL_DisableEnterGEOFence();
//		}
//		else{
//			AL_DisableLeaveGEOFence();
//		}
	}
//	else{
//		if( nFence & 0x80 ){
//			AL_EnableEnterGEOFence();
//		}
//		else{
//			AL_EnableLeaveGEOFence();
//		}
//	}
	else{
		// ����Χ��
		memcpy( &g_stAlarmSetup.m_oStGFence[nCnt].m_RLat, pDataMsg, 4);
		pDataMsg += 4;
		memcpy( &g_stAlarmSetup.m_oStGFence[nCnt].m_RLng, pDataMsg, 4);
		pDataMsg += 4;
		memcpy( &g_stAlarmSetup.m_oStGFence[nCnt].m_LLat, pDataMsg, 4);
		pDataMsg += 4;
		memcpy( &g_stAlarmSetup.m_oStGFence[nCnt].m_LLng, pDataMsg, 4);
	}	
	
	return 1;
}
/////////////////////////////////////////////////////////////////////////////
//	���������ϴ�ģʽ 	
unsigned char	PL_TESetupCBMode( unsigned char	*pDataMsg ){
	
	unsigned short  nTmp = 0;

	if ( *pDataMsg++ != 5 ){
		return 0;
	}				
	g_stGSMSetup.m_nCBMode = *pDataMsg++;
	pDataMsg += 2;	
	memcpy( &nTmp, pDataMsg, 2);					
	g_stGSMSetup.m_nCBSpace = nTmp;
	if ( g_stGSMSetup.m_nCBSpace <= 5 ){
		g_stGSMSetup.m_nCBSpace = 5;
	}
	
	return 1;
}
/////////////////////////////////////////////////////////////////////////////
//	���ø��ֺ���	
unsigned char	PL_TESetupNumber( unsigned char	*pDataMsg ){
	
	unsigned char 	nResult = 1;
	unsigned char 	nLen = 0;
	unsigned char 	nSaveMode = 0;
	
	nLen = *pDataMsg++;	
	nSaveMode = *pDataMsg++;		
	
	nLen--;
	
	if( nLen > TELNUMBER_LENGTH ){
		nLen = TELNUMBER_LENGTH;
	}
		
	switch( nSaveMode ){
	case PHONE_MASTER:			//���غ���		
		memcpy( (char*)g_stGSMSetup.m_strMaterNo, (char*)pDataMsg, nLen );
		g_stGSMSetup.m_strMaterNo[nLen] = 0x00;
		break;
	case PHONE_AUTHORIZATION1:	//��Ȩ����
		memcpy( (char*)g_stGSMSetup.m_strAuthNo1, (char*)pDataMsg, nLen );
		g_stGSMSetup.m_strAuthNo1[nLen] = 0x00;
		break;
	case PHONE_AUTHORIZATION2:  //��Ȩ����
		memcpy( (char*)g_stGSMSetup.m_strAuthNo2, (char*)pDataMsg, nLen );
		g_stGSMSetup.m_strAuthNo2[nLen] = 0x00;
		break;
	case PHONE_AUTHORIZATION3:  //��Ȩ����
		memcpy( (char*)g_stGSMSetup.m_strAuthNo3, (char*)pDataMsg, nLen );
		g_stGSMSetup.m_strAuthNo3[nLen] = 0x00;
		break;
	case PHONE_SMSADDR:					//��ַ�������ĺ���
		memcpy( (char*)g_stGSMSetup.m_strSMSAddrNo, (char*)pDataMsg, nLen );
		g_stGSMSetup.m_strSMSAddrNo[nLen] = 0x00;
		break;
	case PHONE_SMC:				//SMC���ĺ���
		memcpy( (char*)g_stGSMSetup.m_strSMCNo, (char*)pDataMsg, nLen );
		g_stGSMSetup.m_strSMCNo[nLen] = 0x00;
		break;
	default:
		nResult = 0;
		break;
	}	
	return nResult;
}
/////////////////////////////////////////////////////////////////////////////
//	���÷�����   
unsigned char	PL_TESetupServer( unsigned char	*pDataMsg ){
	
	
	if ( *pDataMsg++ != 6 ){
		return 0;
	}
	memcpy( (char*)&g_stGSMSetup.m_nServerPort, (char*)pDataMsg, 2);
	pDataMsg += 2;	
	sprintf( (char*)g_stGSMSetup.m_strServerIP, "%03d.%03d.%03d.%03d",
				 		pDataMsg[0], 
				 		pDataMsg[1], 
				 		pDataMsg[2], 
				 		pDataMsg[3] );	
	IP_SetConnectNG( );		
	TE_ClrLoginSuccess();		 			 		
	return 1;
}
/////////////////////////////////////////////////////////////////////////////
//	��������	
unsigned char	PL_TESetupAlarm( unsigned char	*pDataMsg ){
	
	unsigned char 	nResult = 1;
	unsigned char 	nDataType = 0;
	unsigned char	nTmp = 0;
	unsigned int	iTmp = 0;
	
	// CC_PARALEN
	pDataMsg++;
	// CC_PARADATA TYPE
	nDataType = *pDataMsg++;
	switch( nDataType ){
	case SETUP_ALARM_SET: 		  	    									//������������
		memcpy( &iTmp, pDataMsg, 4 );
		g_stAlarmSetup.m_nAlarmEnable = iTmp;
		break;
	
	case SETUP_ALARM_OVERSPEED:
		nTmp = *pDataMsg;
		if ( nTmp < MIN_OVERSPEED ){
			AL_DisableOverSpeed( );
		}
		else{
			g_stAlarmSetup.m_nOverspeed = nTmp;
			AL_EnableOverSpeed();
		}
		break;
	
	case SETUP_ALARM_PARKING:
		memcpy( &iTmp, (char*)pDataMsg, sizeof(short));	
		if ( iTmp == 0 ){
			AL_DisableParking( );
		}
		else{			
			g_stAlarmSetup.m_nParking = (short)iTmp;
			AL_EnableParking();
		}
		break;
	// �Ƿ����ű���	
	case SETUP_ALARM_ANTISTEAL:		
		if ( *pDataMsg == 0 ){
			AL_DisableNotDoor( );
		}
		else{
			g_stAlarmSetup.m_nILLDoorOpen = *pDataMsg;	
			AL_EnableNotDoor( );
		}
		break;
	// ƣ�ͼ�ʻ����	
	case SETUP_ALARM_TIREDNESS:
		if ( *pDataMsg == 0 ){
			AL_DisableTireDrive();
		}
		else{
			g_stAlarmSetup.m_nTireDriver = *pDataMsg;
			AL_EnableTireDrive();
		}
		break;
    //  Բ�ξ��뱨��
	case SETUP_ALARM_DISTANCE:
		memcpy( &iTmp, (char*)pDataMsg, sizeof(short));	
		if( iTmp == 0 ){
			g_stCustomSetup.m_nDistance = 0;
			TE_DisableOutGeo();
		}
		else{
			if( GPS_IsPosValid() ){		//GPS��λ��Ч
				if( nTmp < MINGEO_SETUP ){
					nTmp = MINGEO_SETUP;
				}			
				g_stCustomSetup.m_nCLng = GPS_GetLongitude();	//����
				g_stCustomSetup.m_nCLat = GPS_GetLatitude();		//γ��
				g_stCustomSetup.m_nDistance = nTmp;
				TE_EnableOutGeo();
			}
		}
		break;
	// ȡ������
	case SETUP_ALARM_CANCEL:
		memcpy( (char*)&iTmp, (char*)pDataMsg, sizeof(int) );
		TE_ClearAlarmState( );
		GPRS_ClrAlarmSMSMode();
		break;	
	default:
		nResult = 0;
		break;		
	}
	return nResult;
}		    

#endif		//SW_GPRSPROTOCOLB

/////////////////////////////////////////////////////////////////////////////
//  �������к�  ȡ��8λ
unsigned char  PL_AddTEUID( unsigned char	 *pBuf ){
	
	unsigned char 	nLen = 0;
	unsigned char	nPos = 0;
	
	nLen = strlen((char*)g_stGSMSetup.m_strUID);
	if ( nLen > DEUID_LENGTH ){
		nPos = nLen - DEUID_LENGTH;
		nLen = DEUID_LENGTH;
	}
	pBuf[0] = nLen;
	memcpy( &pBuf[1], &g_stGSMSetup.m_strUID[nPos], nLen);
	return nLen + 1; 
}
/////////////////////////////////////////////////////////////
//   ���У���   
unsigned char PL_VerifyCheckSum(unsigned char *lpDataBuf, int nDataLen ){
		
	unsigned char 	nCnt = 0;	
	unsigned char 	nSumCheck = CHKSUM_ERROR;
		
	// CRCУ�鷨
	#ifdef  SW_PROTOCOL_ASICO	
		unsigned char	nBit = 0;
		unsigned char	hHig = 0;
		unsigned char	hLow = 0;
		unsigned short	nCRC = 0xffff; 

		if( nDataLen < 6){
			return nSumCheck;
		}		
		// ͬ����,���������ͬ������Ϊ��CUP���ٶ�, ��ֹ�ڶ���ģʽ�µ�����		
		if( lpDataBuf[WHERE_MESSAGE_HEAD1] != SYN_SVEREHEADERCODE1 ||
			lpDataBuf[WHERE_MESSAGE_HEAD2] != SYN_SVEREHEADERCODE2 ){
			return nSumCheck;
		}
		nSumCheck = 0;
		// �õ����ݳ���		
		nDataLen = (lpDataBuf[WHERE_DATALEN_HIGHT]<<8) & 0xff00; 
		nDataLen = lpDataBuf[WHERE_DATALEN_LOW];
		// У����(2B)|������(2B)
		for( nCnt = 0; nCnt< nDataLen-4; nCnt++ ){
			for( nBit = 0; nBit < 8; nBit++){						
				nSumCheck = ((lpDataBuf[nCnt]<<nBit)&0x80) ^ ((nCRC&0x8000)>>8);			
				nCRC <<= 1;				
				if(nSumCheck != 0){  
					nCRC^=0x1021;
				}
			}
		}
		hHig = (nCRC >> 8)&0xff;
		hLow = nCRC & 0xff;
		if( (lpDataBuf[nCnt] == hHig) &&
			(lpDataBuf[nCnt+1] == hLow) ){ 
			nSumCheck = CHKSUM_OK;
		}
		else{
			nSumCheck = CHKSUM_ERROR;
		}		
	#else		
		nSumCheck = 0;
		for( nCnt = 0; nCnt < nDataLen; nCnt++){
			nSumCheck += lpDataBuf[nCnt];
		}
	#endif
	return nSumCheck;
}
//////////////////////////////////////////////////////////////////////////
//   ����У���
#ifdef  SW_PROTOCOL_ASICO
unsigned short  PL_CountVerifySum(unsigned char *lpDataBuf, int nDataLen){
	
	unsigned char  nCnt = 0;
	unsigned char  nBit = 0;
	unsigned char  nFlag = 0;
	unsigned short nSumCheck = 0xffff;	
		
	for( nCnt = 0; nCnt< nDataLen; nCnt++ ){
		for( nBit = 0; nBit < 8; nBit++){						
			nFlag = ((lpDataBuf[nCnt]<<nBit)&0x80) ^ ((nSumCheck&0x8000)>>8);			
			nSumCheck <<= 1;				
			if(nFlag != 0){  
				nSumCheck^=0x1021;
			}
		}
	}
	// У���
	lpDataBuf[nCnt] = (nSumCheck >> 8) &0xff;
	lpDataBuf[nCnt+1] = nSumCheck & 0xff;
		
	return nSumCheck;
}
#else
unsigned char  PL_CountVerifySum(unsigned char *lpDataBuf, int nDataLen){
	
	unsigned char nSumCheck = 0;
	unsigned char nCnt = 0;
	
	for ( nCnt = 0; nCnt < nDataLen; nCnt++){
		
		nSumCheck -= lpDataBuf[nCnt];
	}
	return nSumCheck;
}
#endif  //SW_PROTOCOL_ASICO 

/////////////////////////////////////////////////////////////
//������
// ��������
// nCaller	:  ���պ���
// pMsg	  	:  ��������
// nTPUDL 	:  ���ݳ���
// nDCSType :  DCS 

// 1: ����>0��ʾ�а���Э�����ݷ���							x
// 2: ����0, ��pMsg���ı����ݡ���ʾ��������Ϣ����			x
// 3: ����0,��pMsgΪ�ա���ʾ����ش�����					x


// nCodeType = 0: 7λ���ݡ��ı���������
// nCodeType = 1: 8λ���뷢��������Э������
// nCodeType = 2: unicode���롣 ��������
// ����Ϊ0�����跢����

unsigned char 	PL_ParseSMSData( unsigned char    *pDataBuf,
								 unsigned char     nDataLen,	
								 unsigned char	  *pCaller,    				                 
								 unsigned char     nDCS,
								 unsigned char	   nTPUDL,
								 unsigned char 	  *pCodeType ){
 
	unsigned char   nResultLen = 0; 			 	
	unsigned char		*pSrc 	   = NULL;
									 	
	if ( PL_VerifyCheckSum( pDataBuf, nDataLen ) != CHKSUM_OK ){

		pSrc = lpLargeBuf2;	
		
		if( IsAddrServerNo( pCaller ) || IsForwardNo(pCaller) ){
			if ( nDCS == DCS_7BIT){	 
				P_Decode7bitTo8Bit( pSrc, pDataBuf, nDataLen);						
				nResultLen = P_GSMToASCII( (char*)pDataBuf, (char*)pSrc, nTPUDL); 
				*pCodeType = 0;
			}
			else if ( nDCS == DCS_8BIT ){
				nResultLen = nDataLen;
				*pCodeType = 1;
			}
			else{
				nResultLen = nDataLen;
				*pCodeType = 2;			
				
			}			
		}
		else{		
			if ( nDCS == DCS_7BIT){	 
				P_Decode7bitTo8Bit( pSrc, pDataBuf, nDataLen);						
				P_GSMToASCII( (char*)pSrc, (char*)pSrc, nTPUDL); 
			}
			else if ( nDCS == DCS_8BIT ){
				memcpy(pSrc, pDataBuf, nDataLen);
				pSrc[ nDataLen ] = 0x00;
			}
			else if ( nDCS == DCS_UNICODE ){ 
				P_UnicodeToASCII( pSrc, pDataBuf,  nDataLen );
			}		
			else{
				memset( pSrc, 0, 1);
			}	
			nResultLen = PLS_ParseCommandData( pDataBuf, pSrc, pCaller, pCodeType );
		}	
		pSrc = NULL;										 	
	}
	else{  
		nResultLen = PL_ParseData( pDataBuf, pDataBuf, nDataLen );
		*pCodeType = 2;
	}					  
	return  nResultLen;							 
}
///////////////////////////////////////////////////////////////////
//	���Ͳ�������
//////////////////////////////////////////////////////////////////////////
//������Чλ(8),��������(8), ��������(2),ƣ�ͼ�ʻ(2),�Ƿ����ű�������(2),
//ͣ����������(2),������IP(8), �������˿�(4),����ģʽ(1),�ش�ģʽ(3),
//���غ���(4~21),��Ȩ����1(4~21), ��Ȩ����2, (4~21) ��Ȩ����3(4~21),
//APN(?),APNUSER,APNPSD
unsigned  char	PL_ParseConfig( unsigned char *pDest ,unsigned char	*pSrc ){ 

	
	unsigned char   nFlag = 0;
	unsigned char   bExit = 0;
	char			*pTmp = NULL; 
	unsigned char 	*pBuf = NULL;
	

	nFlag = 0; 
	bExit = 0;

	pBuf = (unsigned char*)strchr( (char*)pSrc, ',' );
	if( pBuf == NULL ){
		return 0;
	}
	pSrc = pBuf+1;	
	while( bExit == 0 ){
		pBuf = (unsigned char*)strchr( (char*)pSrc, ',' );
		if( pBuf != NULL ){
			*pBuf = 0;
		}
		else{			
			bExit = 1;
		}
		// ��������
		switch( nFlag ){
		// ��������
		case 0:
			g_stAlarmSetup.m_nAlarmEnable = strtol( (char*)pSrc, &pTmp, 16 );
		break;
		// �ٶ�
		case 1:
			g_stAlarmSetup.m_nOverspeed = (unsigned char)strtol( (char*)pSrc, &pTmp, 16 );
			break;
		// ƣ�ͼ�ʻ 	
		case 2:
			g_stAlarmSetup.m_nTireDriver = (unsigned char)strtol( (char*)pSrc, &pTmp, 16 );
			break;
		// �Ƿ����ű���
		case 3:
			g_stAlarmSetup.m_nILLDoorOpen = (unsigned char)strtol( (char*)pSrc, &pTmp, 16 );
			break;
		// ͣ����������(2)
		case 4:
			g_stAlarmSetup.m_nParking = (short)strtol( (char*)pSrc, &pTmp, 16 );
			break;
		// ������IP��ַ
		case 5:
			memset(g_stGSMSetup.m_strServerIP,0x00, SERVERIP_LENGTH+1 );
			strncpy((char*)g_stGSMSetup.m_strServerIP, (char*)pSrc, SERVERIP_LENGTH );
			break;
		//  �������˿ں�
		case 6:
			g_stGSMSetup.m_nServerPort = (short)strtol( (char*)pSrc, &pTmp, 16 );
			break;
		// GPRS����ģʽ
		case 7:
			g_stGSMSetup.m_nCommMode = (short)strtol( (char*)pSrc, &pTmp, 16 );
			break;
		//  ����ģʽ
		case 8:
			g_stGSMSetup.m_nCBMode = (unsigned char)strtol( (char*)pSrc, &pTmp, 16);
			break;
		// �ش�ģʽ
		case 9:
			g_stGSMSetup.m_nCBSpace = (short)strtol( (char*)pSrc, &pTmp, 16 );
			break;
		// ���غ���
		case 10:
			memset( g_stGSMSetup.m_strMaterNo, 0x00, TELNUMBER_LENGTH+1 );
			strncpy( (char*)g_stGSMSetup.m_strMaterNo, (char*)pSrc,  TELNUMBER_LENGTH );
			break;
		// ��Ȩ����1
		case 11:
			memset( g_stGSMSetup.m_strAuthNo1, 0x00, TELNUMBER_LENGTH+1 );
			strncpy( (char*)g_stGSMSetup.m_strAuthNo1, (char*)pSrc,  TELNUMBER_LENGTH );
			break;
		// ��Ȩ����2
		case 12:
			memset( g_stGSMSetup.m_strAuthNo2, 0x00, TELNUMBER_LENGTH+1 );
			strncpy( (char*)g_stGSMSetup.m_strAuthNo2, (char*)pSrc, TELNUMBER_LENGTH );
			break;
		// ��Ȩ����3
		case 13:
			memset( g_stGSMSetup.m_strAuthNo3, 0x00, TELNUMBER_LENGTH+1 );
			strncpy( (char*)g_stGSMSetup.m_strAuthNo3, (char*)pSrc,  TELNUMBER_LENGTH );
			break;
		// APN
		case 14:
			memset( g_stGSMSetup.m_strGateway, 0x00, SERVERDOMAIN_LENGTH+1 );
			strncpy( (char*)g_stGSMSetup.m_strGateway, (char*)pSrc, SERVERDOMAIN_LENGTH);
			break;
		// User name
		case 15:
			memset( g_stGSMSetup.m_strUser, 0x00, TELNUMBER_LENGTH+1 );
			strncpy( (char*)g_stGSMSetup.m_strUser, (char*)pSrc, TELNUMBER_LENGTH);
			break;
		// user Psd	
		case 16:	
			memset( g_stGSMSetup.m_strPsd, 0x00, TELNUMBER_LENGTH+1 );
			strncpy( (char*)g_stGSMSetup.m_strPsd, (char*)pSrc, TELNUMBER_LENGTH);		
			break;
		// dnsr
		case 17:
			memset( g_stGSMSetup.m_strServerDomain, 0x00, SERVERDOMAIN_LENGTH+1 );
			strncpy( (char*)g_stGSMSetup.m_strServerDomain, (char*)pSrc, SERVERDOMAIN_LENGTH);
			break;
		}	
		nFlag++;	
		if( pBuf != NULL ){ 
			pSrc = pBuf+1; 
		}
	}
	return PL_ReadConfig( pDest );
}
///////////////////////////////////////////////////////////////////
//	 ��ȡ��������
//////////////////////////////////////////////////////////////////////////
//DEUID(20),������Чλ(8),��������(8), ��������(2),ƣ�ͼ�ʻ(2),�Ƿ����ű�������(2),
//ͣ����������(2),������IP(8), �������˿�(4),����ģʽ(1),�ش�ģʽ(3),
//���غ���(4~21),��Ȩ����1(4~21), ��Ȩ����2, (4~21) ��Ȩ����3(4~21),
//APN(?),APNUSER,APNPSD
unsigned char	PL_ReadConfig( unsigned char	*pDest ){

	unsigned char	nLen = 0;
	
	sprintf((char*)pDest, "%08X,%08X,%02X,%02X,%02X,%02X,%s,%04X,%d,%d,%03X,%s,%s,%s,%s,%s,%s,%s,%s,%s",								
								g_stAlarmSetup.m_nALValid,
								g_stAlarmSetup.m_nAlarmEnable,
								g_stAlarmSetup.m_nOverspeed,
								g_stAlarmSetup.m_nTireDriver,
								g_stAlarmSetup.m_nILLDoorOpen,
								g_stAlarmSetup.m_nParking,
								g_stGSMSetup.m_strServerIP,
								g_stGSMSetup.m_nServerPort,
								g_stGSMSetup.m_nCommMode,
								g_stGSMSetup.m_nCBMode,
								g_stGSMSetup.m_nCBSpace,
								g_stGSMSetup.m_strMaterNo,
								g_stGSMSetup.m_strAuthNo1,
								g_stGSMSetup.m_strAuthNo2,
								g_stGSMSetup.m_strAuthNo3,
								g_stGSMSetup.m_strGateway,
								g_stGSMSetup.m_strUser,
								g_stGSMSetup.m_strPsd,
								g_stGSMSetup.m_strUID,
								g_stGSMSetup.m_strServerDomain );
	
	nLen = strlen( (char*)pDest );								
	return nLen;
}


