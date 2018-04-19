///////////////////////////////////////////////////////////////////////////
//   file name: ProtocolData.h
// description: GSM/GPRSЭ��ͷ�ļ�����
//      author:
//        date: 2009-06-01
//     reamark: 

#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>
#include  "..\hdr\includes.h"
#include  "..\hdr\ProtocolA.h"
#include  "..\hdr\ProtocolB.h"
#include  "..\hdr\ProtocolASICO.h"
#include  "..\hdr\APP_LoopDetect.h" 
#include  "..\hdr\APP_Terminal.h"
#include  "..\hdr\HW_GPIO.h"
#include  "..\hdr\APP_GSM.h"
#include  "..\hdr\APP_GTM900.h"
#include  "..\hdr\APP_BQM26.h"
#include  "..\hdr\APP_EM200.h"
#include  "..\hdr\APP_SIMCOM.h"
#include  "..\hdr\APP_GPS.h"
#include  "..\hdr\SMSProtocolA.h"
#include  "..\hdr\SMSProtocolB.h"
#include  "..\hdr\SMSProtocolC.h"


#ifdef	SW_GPRSPROTOCOL_ASICO

/////////////////////////////////////////////////////////////////////////////
// ��֯DEUID���� 13480924411
void	PL_ComposeDEUID( unsigned char	*pDest ){
	
	unsigned char	nResult = 0;
	unsigned char	nCnt = 0;
	unsigned char	nTmp;
	unsigned char	nLen = 0;
	
	memset( pDest, 0xFF,  ASICO_DEUID_LEN);
	nLen = strlen((char*)g_stGSMSetup.m_strUID);
	if( nLen > 14){
		nLen = 14;
	}
	for( nCnt = 0; nCnt < nLen; nCnt++ ){

		nTmp = g_stGSMSetup.m_strUID[nCnt];
		if( nTmp >= '0' && nTmp <='9' ){
			nTmp -= '0';

			if( (nCnt%2) == 0 ){
				*(pDest+nResult) = nTmp<<4;	
				*(pDest+nResult) |= 0x0F;									
			}
			else{
				*(pDest+nResult) &= 0xF0;		
				*(pDest+nResult) |= nTmp;
				nResult++;
			}
		}
	}
}
/////////////////////////////////////////////////////////////////////////////
//  ��֯ ��ͷ(2B)|����(2B)|DEUID(7B)|������(2B) 
void	PL_ComposeHeadData( unsigned char	*pDest, 
							unsigned char	nDataLen,
							unsigned short	nCmdCode ){
		
	pDest[WHERE_MESSAGE_HEAD1] = SYN_TEHEADERCODE1;
	pDest[WHERE_MESSAGE_HEAD2] = SYN_TEHEADERCODE2;
	pDest[WHERE_DATALEN_HIGHT] = 0x00;
	pDest[WHERE_DATALEN_LOW] = nDataLen;
	PL_ComposeDEUID(pDest+WHERE_MESSAGE_DEUID);	
	pDest[WHERE_CMD_HIGHT] = (nCmdCode >> 8) & 0xff;
	pDest[WHERE_CMD_LOW] = nCmdCode & 0xff;											
}
/////////////////////////////////////////////////////////////////////////////
//   ����״̬��Ϣ
void			PL_UPDataStatus( void ){
	
	unsigned int  			nTmp = 0;
	
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
}
/////////////////////////////////////////////////////////////////////////////
//  ��֯GPS���� ��ͷ(2B)|����(2B)|DEUID(7B)|������(2B)|��������(XB)|У����(2B)|������(2B)
//  ��������: GPRMC|ˮƽ��=1(1B)|�߶�(0��)(1B)|״̬(0)(1B)|0,0 (3B)
unsigned char 	PL_ComposeGPSData( unsigned char  *pDest ){

	unsigned char			nDataLen = 0;
	unsigned char			nGpsLen = 0;
		
		
	memset( pDest, 0x00, 128);	
	//  ����״̬��Ϣ	
	PL_UPDataStatus();	
	// GPRSģʽ����
	if( TE_HaveAlarm() ){ 		
		// 14 = ��ͷ(2B)|����(2B)|DEUID(7B)|������(2B)|����״̬(4)
		nDataLen = 13+4;		
		// 10 = |ˮƽ��=1(1B)|�߶�(0��)(1B)|״̬(0xFFFF FFFF)(8)|0,0 (3B)
		nDataLen += 17;	
		// У����(2B)|������(2B)
		nDataLen += 4;				
		memcpy( &pDest[WHERE_MESSAGE_DATA], &g_stDeviceState.m_nALState, 4);
		// ��֯GPS����
		nGpsLen = GPS_GetGPRMCData( &pDest[WHERE_MESSAGE_DATA+4] );	
		nDataLen += nGpsLen;	
		// ��֯��ͷ	
		PL_ComposeHeadData(pDest, nDataLen, CC_TE_ALARM_CMD );			
		
		// ��֯��������
		sprintf( (char*)pDest+17+nGpsLen, "|1|%d|%02X%02X%02X%02X|0,0", 0, 
																		g_stDeviceState.m_nHWState&0xFF,
																		(g_stDeviceState.m_nHWState>>8)&0xFF,
																		(g_stDeviceState.m_nHWState>>16)&0xFF,
																		(g_stDeviceState.m_nHWState>>24)&0xFF );	
																		
		
	}
	else{
		// 13 = ��ͷ(2B)|����(2B)|DEUID(7B)|������(2B)|
		nDataLen = 13;			
		// 10 = |ˮƽ��=1(1B)|�߶�(0��)(1B)|״̬(0xFFFF FFFF)(8B)|0,0 (3B)
		nDataLen += 17;	
		// У����(2B)|������(2B)
		nDataLen += 4;		
		// ��֯GPS����
		nGpsLen = GPS_GetGPRMCData( &pDest[WHERE_MESSAGE_DATA] );	
		nDataLen += nGpsLen;		
		// ��֯��ͷ
		PL_ComposeHeadData(pDest, nDataLen, CC_TE_SINGLE_LOCATION_REPORT );			
		// ��֯��������
		sprintf( (char*)pDest+13+nGpsLen, "|1|%d|%02X%02X%02X%02X|0,0", 0,
																		g_stDeviceState.m_nHWState&0xFF,
																		(g_stDeviceState.m_nHWState>>8)&0xFF,
																		(g_stDeviceState.m_nHWState>>16)&0xFF,
																		(g_stDeviceState.m_nHWState>>24)&0xFF );
	}	
	// ����У��� 
	PL_CountVerifySum(pDest, nDataLen-4);
	// ������
	pDest[nDataLen-2] = SYN_R_ENDCODE;  
	pDest[nDataLen-1] = SYN_N_ENDCODE;
	
	return nDataLen;
}
/////////////////////////////////////////////////////////////////////////////
//	û�ж�λ��ʱ��ظ�
unsigned char	PL_ComposeNoValidData( unsigned char	*pDest ){

	unsigned char			nDataLen = 0;
	unsigned char			nGpsLen = 0;
	
	
	memset( pDest, 0x00, 128 );
	// ����״̬��Ϣ
	PL_UPDataStatus();
	// 13 = ��ͷ(2B)|����(2B)|DEUID(7B)|������(2B)|��ʶ��(1Byte) 
	nDataLen = 14;	
	// 17 = |ˮƽ��=1(1B)|�߶�(0��)(1B)|״̬(0xFFFF FFFF)(8B)|0,0 (3B)
	nDataLen += 17;	
	// У����(2B)|������(2B)
	nDataLen += 4;	
	// ��֯GPS����
	pDest[WHERE_MESSAGE_DATA] = 0x66;
	nGpsLen = GPS_GetGPRMCData( &pDest[WHERE_MESSAGE_DATA+1] );	
	nDataLen += nGpsLen;	
	// ��֯��ͷ
	PL_ComposeHeadData( pDest, nDataLen, CC_TE_SINGLE_LOCATION_REPORT );	
	
	// ����У��� 
	PL_CountVerifySum(pDest, nDataLen-4);
	// ��֯��������
	sprintf( (char*)pDest+14+nGpsLen, "|1|%d|%02X%02X%02X%02X|0,0", 0, 
																	g_stDeviceState.m_nHWState&0xFF,
																	(g_stDeviceState.m_nHWState>>8)&0xFF,
																	(g_stDeviceState.m_nHWState>>16)&0xFF,
																	(g_stDeviceState.m_nHWState>>24)&0xFF );
	// ������
	pDest[nDataLen-2] = SYN_R_ENDCODE;  
	pDest[nDataLen-1] = SYN_N_ENDCODE;	
	return nDataLen;
}
/////////////////////////////////////////////////////////////////////////////
// ֯֯������Ϣ	
unsigned char 	PL_ComposeHSData( unsigned char	*pDest ){

	unsigned char			nLen  = 17;
		
	PL_ComposeHeadData(pDest, nLen, CC_TE_HS_DATA);
	PL_CountVerifySum(pDest, nLen-4);
	//������
	pDest[nLen-2] = SYN_R_ENDCODE;  
	pDest[nLen-1] = SYN_N_ENDCODE;
	return nLen;
}
/////////////////////////////////////////////////////////////////////////////
//
unsigned char	PL_ComposeLogin( unsigned char  *pDest ){

	unsigned char			nLen  = 17;
	
	PL_ComposeHeadData(pDest, nLen, CC_TE_LOGIN);
	PL_CountVerifySum(pDest, nLen-4);	
	//������
	pDest[nLen-2] = SYN_R_ENDCODE;  
	pDest[nLen-1] = SYN_N_ENDCODE;
	return nLen;
}
/////////////////////////////////////////////////////////////////////////////
//	���ø���ʱ�� 
unsigned char  PL_ComposeTrackTime( unsigned char	*pDest,
								 	unsigned short	nCmdCode,
								 	unsigned char	nStuts ){
									
	unsigned char		  nLen;	
	if( nStuts == 0 ){
		nLen = 21;
		PL_ComposeHeadData(pDest, nLen, nCmdCode);
		pDest[WHERE_MESSAGE_DATA]   = 0x01;
		pDest[WHERE_MESSAGE_DATA+1]   = g_stGSMSetup.m_nCBMode;
		pDest[WHERE_MESSAGE_DATA+2] = (g_stGSMSetup.m_nCBSpace>>8)&0xFF;
		pDest[WHERE_MESSAGE_DATA+3] = g_stGSMSetup.m_nCBSpace&0xFF;
		PL_CountVerifySum(pDest, nLen-4);
	}
	else{
		nLen = 20;
		PL_ComposeHeadData(pDest, nLen, nCmdCode);
		pDest[WHERE_MESSAGE_DATA]   = g_stGSMSetup.m_nCBMode;
		pDest[WHERE_MESSAGE_DATA+1] = (g_stGSMSetup.m_nCBSpace>>8)&0xFF;
		pDest[WHERE_MESSAGE_DATA+2] = g_stGSMSetup.m_nCBSpace&0xFF;
		PL_CountVerifySum(pDest, nLen-4);
	}
	//������
	pDest[nLen-2] = SYN_R_ENDCODE;  
	pDest[nLen-1] = SYN_N_ENDCODE;
	
	return  nLen;									
}
//////////////////////////////////////////////////////////////////////////
//	��������Ӧ������
unsigned char	PL_ComposeACKData( unsigned char	*pDest,
								   unsigned char	nFlag,	
								   unsigned short	nCmdCode ){
	unsigned char		nLen = 18;
	
	PL_ComposeHeadData(pDest, nLen, nCmdCode);
	pDest[WHERE_MESSAGE_DATA]   = nFlag;
	PL_CountVerifySum(pDest, nLen-4);	
	//������
	pDest[nLen-2] = SYN_R_ENDCODE;  
	pDest[nLen-1] = SYN_N_ENDCODE;
	
	return nLen;								   
}
//////////////////////////////////////////////////////////////////////////
//  ����������״̬
unsigned char	PL_ComposeReadAlarm( unsigned char	*pDest, 
									 unsigned short	nCmdCode ){
									 
	unsigned char		nLen = 21;
	
	PL_ComposeHeadData(pDest, nLen, nCmdCode);
	memcpy( &pDest[WHERE_MESSAGE_DATA], &g_stAlarmSetup.m_nALValid, 4);
	PL_CountVerifySum(pDest, nLen-4);	
	//������
	pDest[nLen-2] = SYN_R_ENDCODE;  
	pDest[nLen-1] = SYN_N_ENDCODE;
	
	return nLen;								 
}
//////////////////////////////////////////////////////////////////////////
//  ��ȡ��Ȩ����  17+16+16;
unsigned char PL_ComposeReadNumber( unsigned char	*pDest, 
									unsigned char	nPhoneType,
									unsigned char	*pPhone ){
	unsigned char		nLen= 17+1+16; 
	
	PL_ComposeHeadData(pDest, nLen, CC_TE_READ_NUMBER);
	memset( &pDest[WHERE_MESSAGE_DATA], 0x00, 17);
	pDest[WHERE_MESSAGE_DATA] = nPhoneType;	
	memcpy( &pDest[WHERE_MESSAGE_DATA+1], pPhone, 16);
	PL_CountVerifySum(pDest, nLen-4);	
	//������
	pDest[nLen-2] = SYN_R_ENDCODE;  
	pDest[nLen-1] = SYN_N_ENDCODE;
	return nLen;								
}
//////////////////////////////////////////////////////////////////////////
//	BCDת��Ϊʮ������
unsigned char BCDToHex( unsigned char	nData){

	return (((nData >> 4 ) * 16) + (nData & 0x0f));
}
//////////////////////////////////////////////////////////////////////////
//	�ֽ��͵�ʮ������
unsigned short ByteToInt(unsigned char* pSrc){  //16bit

	  return ((pSrc[WHERE_CMD_HIGHT] << 8) | pSrc[WHERE_CMD_LOW]);
}
//////////////////////////////////////////////////////////////////////////
//�ó�DEUID
void   PL_ParseDEUID( unsigned char	*pDest, unsigned char *pSrc ){
	
	unsigned char	nResult = 0;			
	unsigned char	nHexData = 0;	
	unsigned char	nCnt = 0;	
	
	for( nCnt = 0; nCnt < ASICO_DEUID_LEN; nCnt++){	
		
		nHexData = BCDToHex( pSrc[nCnt] ); 
		if( (nHexData&0xF0) == 0xF0){ //���ֽ�
			break;
		}
		else{ 				
			pDest[nResult++] = (nHexData/16) + 0x30;
		}
		if( (nHexData&0x0f) == 0x0F){ //���ֽ�
			break;
		}
		else{			
			pDest[nResult++] = (nHexData%16) + 0x30;
		}
	}
	pDest[nResult]= 0x00;
	return;
}
///////////////////////////////////////////////////////////////
// 	����Э������
unsigned char	PL_ParseData(unsigned char *pDest, unsigned char *pSrc, unsigned char nLen ){

	unsigned char	 	nResultLen = 0;
	unsigned char		nDataLen = 0;	
	unsigned int		nCmdCode = 0;
	unsigned char		nDEUID[DEUID_LENGTH+1];
			
	// ͬ����		
	if( pSrc[WHERE_MESSAGE_HEAD1] != SYN_SVEREHEADERCODE1 ||
		pSrc[WHERE_MESSAGE_HEAD2] != SYN_SVEREHEADERCODE2 ){
		return 0;
	}
	// ���ݳ���	
	nDataLen  = ByteToInt( pSrc );
	// �յ�DEUID����
	PL_ParseDEUID(nDEUID, pSrc+WHERE_MESSAGE_DEUID );
	if( memcmp( g_stGSMSetup.m_strUID, nDEUID, strlen((char*)g_stGSMSetup.m_strUID)) != 0){
		return 0;
	}
	// ������
	nCmdCode = ByteToInt( pSrc );	
	if( nCmdCode == CC_TE_LOGIN_ACK ){
		if(pSrc[WHERE_MESSAGE_DATA] == CMD_SUCCEED ){
			TE_SetLoginSuccess( );	
			UDP_ClrHandShakeCnt( );
			TCP_ClrRCntionCnt();
			IP_ClrConnectFailedCnt( );
		}		
	}
	else if( nCmdCode == CC_TE_HS_DATA ){
		UDP_ClrHandShakeCnt( );
		TCP_ClrRCntionCnt();
		IP_ClrConnectFailedCnt( );
	}
	else{
		//nDataLen: ��ͷ(2Byte)|����(2Byte)|DEUID(7Byte)|������(2Byte)|��������(X Byte)|У����(2Byte)|������(2Byte)|
		nResultLen = PL_ParseDelivery( pDest, pSrc, nDataLen-ASICO_DEUID_LEN-2-2-2-4, nCmdCode );
	}	
	return nResultLen;
}

////////////////////////////////////////////////////////////////////////////
//   �����ն�������Ϣ  MT_DELIVERY
unsigned char	PL_ParseDelivery( unsigned char  *pDest, 
								  unsigned char  *pSrc,
								  unsigned char	 nDataLen,
								  unsigned int	 nCmdCode ){
		
	unsigned char			nLen  = 0;
	unsigned char			nFlag = 0;
	unsigned char			*pTmp = NULL;
	unsigned char 			nCaller[TELNUMBER_LENGTH+1];
	unsigned int			nTmp = 0;
	
	switch(nCmdCode){
	case	CC_TE_CALLONCE:		 			 //��������
	case    CC_TE_EXPAND_CALLONCE:			 //��������
		if( GPS_IsPosValid() ){
			nLen = PL_ComposeGPSData( pDest );
		}
		else{
			nLen = PL_ComposeNoValidData( pDest );
		}
		break;
	case	CC_TE_SETUP_TRACKTIME:			 //���ö�λʱ��			
		nFlag = pSrc[WHERE_MESSAGE_DATA];		
		nTmp = (pSrc[WHERE_MESSAGE_DATA+1]<<8) & 0xFF00;
		nTmp += pSrc[WHERE_MESSAGE_DATA+2];
		if( nFlag == 0x00){
			g_stGSMSetup.m_nCBMode = CB_STOP;
			g_stGSMSetup.m_nCBSpace = nTmp;
		}
		else if(nFlag == 0x01){
			if( nTmp < 10){
				nTmp = 10;
			}
			g_stGSMSetup.m_nCBMode = CB_BYTIME;
			g_stGSMSetup.m_nCBSpace = nTmp;
		}
		else if( nFlag == 0x02 ){
			if( nTmp < 300){
				nTmp = 300;
			}
			g_stGSMSetup.m_nCBMode = CB_BYDISTANCE;
			g_stGSMSetup.m_nCBSpace = nTmp;
		}		
		nLen = PL_ComposeTrackTime( pDest, CC_TE_ACK_TRACKTIME, 0  );		
		TE_SetNeedWriteFlash( ); 
		
		break;
	case	CC_TE_SETUP_NUMBER:				 //������Ȩ����
		nFlag = PLS_SetupGpsMasterNo( pSrc );	
		nLen = PL_ComposeACKData( pDest, nFlag, nCmdCode);			
		TE_SetNeedWriteFlash( );	
		break;
	
	case	CC_TE_SETUP_OVERSPEED:			 //���ó��ٱ���
		nFlag = pSrc[WHERE_MESSAGE_DATA];
		if( nFlag == 0 ){
			AL_DisableOverSpeed( );	
		}
		else{
			AL_EnableOverSpeed( );
			g_stAlarmSetup.m_nOverspeed = nFlag*10;
		}
		nLen = PL_ComposeACKData( pDest, 1, nCmdCode);	
		TE_SetNeedWriteFlash( );
		break;
	case	CC_TE_SETUP_OUTGEO:				 //������λ����
		nFlag = PLS_SetupGpsOutGeo(pSrc);
		nLen = PL_ComposeACKData( pDest, nFlag, nCmdCode );
		TE_SetNeedWriteFlash( );
		break;
	case	CC_TE_SETUP_FENCE:				 //���õ���դ��
	case    CC_TE_SETUP_FENCE2:
	/*	nFlag = PLS_SetupFence( pSrc, nCmdCode );
		if( nFlag ){
			if( nCmdCode == CC_TE_SETUP_FENCE ){
				
				g_stAlarmSetup.m_oStGFence[0].m_nFlag &= ~0x80;
				//AL_DisableLeaveGEOFence();				
			}
			else if( nCmdCode == CC_TE_SETUP_FENCE2 ){
			
				g_stAlarmSetup.m_oStGFence[0].m_nFlag |= 0x80;
				//AL_DisableEnterGEOFence();				
			}
		}
		else{
			//AL_DisableLeaveGEOFence();
			//AL_DisableEnterGEOFence();
			g_stAlarmSetup.m_oStGFence[0].m_nFlag = 0;
		}		
		nLen = PL_ComposeACKData( pDest, nFlag, nCmdCode );		
		TE_SetNeedWriteFlash( );
	*/
		break;	
	case	CC_TE_RESTORE_FACTORY:			 //��㹳�������
		TE_SetNeedWriteFlash( );
		InitializeAlarmFlash();
		InitializeGSMFlash();
		nLen = PL_ComposeACKData( pDest, 1, nCmdCode );	
		TE_SetNeedWriteFlash();
		break;	
	case	CC_TE_READ_TRACKTIME:	 		 //��ȡ��λʱ��
		nLen = PL_ComposeTrackTime( pDest, nCmdCode , 1);
		break;
	case	CC_TE_READ_NUMBER:				 //��ȡ��Ȩ����
		nLen = pSrc[WHERE_MESSAGE_DATA];
		if( nLen == 0 ){
			pTmp = g_stGSMSetup.m_strMaterNo;
		}
		else if( nLen == 1){
			pTmp = g_stGSMSetup.m_strAuthNo1;
		}
		else if(nLen == 2){
			pTmp = g_stGSMSetup.m_strAuthNo2;
		}
		else if(nLen == 3){
			pTmp = g_stGSMSetup.m_strAuthNo3;
		}
		nLen = PL_ComposeReadNumber( pDest, nLen, pTmp );	
		break;
	
	case	CC_TE_READ_OVERSPEED:			 //��ȡ���ٱ���
		nLen = PL_ComposeACKData(pDest,  g_stAlarmSetup.m_nOverspeed/10, CC_TE_READ_OVERSPEED );
		break;
	case	CC_TE_READ_OUTGEO:				 //��ȡ��λ����
		nTmp = 0;
		switch( g_stCustomSetup.m_nDistance ){
		case 0:
			nTmp = 0;
			break;
		case 100:
			nTmp = 3;
			break;
		case 200:
			nTmp = 4;
			break;
		case 300:
			nTmp = 5;
			break;
		case 500:
			nTmp = 6;
			break;
		case 1000:
			nTmp = 7;
			break;
		case 2000:
			nTmp = 8;
			break;
		}
		nLen = PL_ComposeACKData( pDest, nTmp&0xFF, nCmdCode );
		break;
	case	CC_TE_READ_FENCE:				 //��ȡ����դ��
		nLen = PL_ComposeGPSData( pDest );
		break;	
	
	case	CC_TE_SINGLE_LOCATION_REPORT:	 //��һ�ϴ�	
		nLen = PL_ComposeGPSData( pDest );	
		break;	
	case	CC_TE_LISTEN_CAR:					 //���ͳ��ؼ���
		memcpy(nCaller, &pSrc[WHERE_MESSAGE_DATA], 16);
		if( strlen((char*)nCaller) > 3 ){			
			CMM_DialNumber( nCaller );
			#ifdef HW_ET201B
			HW_PORT_SPKMUTE_CLOE();				//�������� 
			#endif
			#ifdef HW_ET201B_2
			HW_PORT_SPKMUTE_CLOE();				//�������� 
			#endif
			#ifdef HW_ET201B_3
			HW_PORT_SPKMUTE_CLOE();				//�������� 
			#endif
		}
		else{
			nLen = PL_ComposeACKData( pDest, 0, nCmdCode );
		}
		break;
	case	CC_TE_TWO_WAY_CALL:					 //����˫��ͨ��
		memcpy(nCaller, &pSrc[WHERE_MESSAGE_DATA], 16);
		if( strlen((char*)nCaller) > 3 ){			
			CMM_DialNumber( nCaller );
			#ifdef HW_ET201B
			HW_PORT_SPKMUTE_OPEN();  			//�����ر�
			#endif
			#ifdef HW_ET201B_2
			HW_PORT_SPKMUTE_OPEN();  			//�����ر�
			#endif
			#ifdef HW_ET201B_3
			HW_PORT_SPKMUTE_OPEN();  			//�����ر�
			#endif
		}
		else{
			nLen = PL_ComposeACKData( pDest, 0, nCmdCode );
		}		
		break;
	case	CC_TE_FUEL_CONTROL:				 //������·����
		if( pSrc[WHERE_MESSAGE_DATA] == 1){    //����
			HW_CTRLOIL_CUTOFF();
			HW_SetOilState();	
			GSM_SetOilState();
		}
		else if(pSrc[WHERE_MESSAGE_DATA] == 0){ //�ָ�		
			HW_CTRLOIL_RESUME();
			HW_ClrOilState( );			
			GSM_ClrOilState();
		} 
		nLen = PL_ComposeACKData( pDest, 1, nCmdCode );
		break;
	case	CC_TE_CENTER_LOCK_CONTROL:		 	 //�����п���	
		if( pSrc[WHERE_MESSAGE_DATA] == 1){      //��
			HW_ClrDoorState( );
			TE_SetNeedLockFlag();
		}
		else if(pSrc[WHERE_MESSAGE_DATA] == 0){  //����
			HW_SetDoorState( );
			TE_SetNeedUnLockFlag();
		}
		nLen = PL_ComposeACKData( pDest, 1, nCmdCode ); 
		break;	
	case CC_TE_ALARM_CMD:						 
		nLen = PL_ComposeGPSData( pDest );				
		break;
	case CC_TE_ALARM_SETUP:						 //����״̬������
		nLen = pSrc[WHERE_MESSAGE_DATA];
		memcpy( &nTmp, &pSrc[WHERE_MESSAGE_DATA+1], 4);	
		if( nLen == 0xFF ){	//�رձ���״̬
			AL_CancelAlarm( nTmp );
			GPRS_ClrNumberSMSMode( nTmp );
		}
		else if( nLen == 0x00 ){
			TE_SetNeedWriteFlash();
			g_stAlarmSetup.m_nAlarmEnable = nTmp;			
		}				
		nLen = PL_ComposeACKData( pDest, 1, nCmdCode );
		break;
	case CC_TE_READ_ALARM_STATUS:				//������״̬
		PL_ComposeReadAlarm( pDest, nCmdCode );
		break;
	case SETUP_ALARM_PARKING:					//����ͣ������
		nTmp = (pSrc[WHERE_MESSAGE_DATA]<<8) & 0xFF00;
		nTmp += pSrc[WHERE_MESSAGE_DATA+1];
		if( nTmp == 0 ){
			AL_DisableParking( );
		}
		else{
			g_stAlarmSetup.m_nParking = (short)nTmp;
			AL_EnableParking();
		}
		nLen = PL_ComposeACKData( pDest, 1, nCmdCode ); 
		TE_SetNeedWriteFlash( ); 
		break;
	case SETUP_ALARM_ANTISTEAL:				// �Ƿ����ű���
		if ( pSrc[WHERE_MESSAGE_DATA] == 0 ){
			AL_DisableNotDoor( );
		}
		else{
			g_stAlarmSetup.m_nILLDoorOpen = pSrc[WHERE_MESSAGE_DATA];	
			AL_EnableNotDoor( );
		}
		nLen = PL_ComposeACKData( pDest, 1, nCmdCode ); 
		TE_SetNeedWriteFlash( ); 
		break;
	case SETUP_ALARM_TIREDNESS:				// ƣ�ͼ�ʻ����	
		if ( pSrc[WHERE_MESSAGE_DATA]  == 0 ){
			AL_DisableTireDrive();
		}
		else{
			g_stAlarmSetup.m_nTireDriver = pSrc[WHERE_MESSAGE_DATA] ;
			AL_EnableTireDrive();
		}
		nLen = PL_ComposeACKData( pDest, 1, nCmdCode ); 
		TE_SetNeedWriteFlash( ); 
		break;
	case CC_TE_MULTI_SETUP:				  //��·����
		nFlag = PL_ComposeMultiSetup( pSrc );
		nLen = PL_ComposeACKData( pDest, nFlag, nCmdCode ); 
		break;
	}
	return nLen;
}
////////////////////////////////////////////////////////////////////////////////
//   ��·����
//   һ�����ֽ�
//  1 Byte: ������·  0: �ָ���·	1: ����	  2: ������
//  2 Byte: ��������  0: ����		1: ����   2: ������
unsigned char	PL_ComposeMultiSetup( unsigned char	*pSrc ){

	unsigned char			nResult = 1;
	
	// ��·����
	switch( pSrc[WHERE_MESSAGE_DATA] ){
	case TE_CTRL_ENABLE:
        HW_ClrOilState( );
		HW_CTRLOIL_RESUME();
		GSM_ClrOilState();
		break;
	case TE_CTRL_DISABLE:
		HW_CTRLOIL_CUTOFF();
		HW_SetOilState();	
		GSM_SetOilState();
		break;	
	}
	// ������
	switch( pSrc[WHERE_MESSAGE_DATA+1] ){
	case TE_CTRL_ENABLE:			//����
		HW_SetDoorState( );
		TE_SetNeedUnLockFlag();		//����		
		break;
	case TE_CTRL_DISABLE:			//��
		HW_ClrDoorState( );
        TE_SetNeedLockFlag();		
		break;	
	}
	return nResult;
}
////////////////////////////////////////////////////////////////////////////////
//  ���ú���
unsigned char	PLS_SetupGpsMasterNo( unsigned char	*pSrc ){

	unsigned 	char		nResult = 1;
	unsigned    char		nPNumber = 0;
	unsigned    char		nPhone[20];
	
	memset( nPhone, 0x00, 20);
	nPNumber = pSrc[WHERE_MESSAGE_DATA];
	memcpy( nPhone, &pSrc[WHERE_MESSAGE_DATA+1], 16);
	switch( nPNumber ){
	case 0:
		strncpy( (char*)g_stGSMSetup.m_strMaterNo, (char*)nPhone, TELNUMBER_LENGTH);
		break;
	case 1:
		strncpy( (char*)g_stGSMSetup.m_strAuthNo1, (char*)nPhone, TELNUMBER_LENGTH);
		break;
	case 2:
		strncpy( (char*)g_stGSMSetup.m_strAuthNo2, (char*)nPhone, TELNUMBER_LENGTH);
		break;
	case 3:
		strncpy( (char*)g_stGSMSetup.m_strAuthNo3, (char*)nPhone, TELNUMBER_LENGTH);
		break;
	default:
		nResult = 0;
		break;
	}
	return nResult;
}
///////////////////////////////////////////////////////////////////
//	����������λ����
unsigned char	PLS_SetupGpsOutGeo( unsigned char	*pSrc ){

	unsigned  char		nResult = 1;
	unsigned  char		nFlag = 0;
	
	nFlag =	pSrc[WHERE_MESSAGE_DATA];
	switch( nFlag ){
	case 0:
		g_stCustomSetup.m_nDistance = 0;
		TE_DisableOutGeo();	
		break;
	case 1:
	case 2:
	case 3:
		g_stCustomSetup.m_nDistance = 100;
		break;
	case 4:
		g_stCustomSetup.m_nDistance = 200;
		break;
	case 5:
		g_stCustomSetup.m_nDistance = 300;
		break;
	case 6:
		g_stCustomSetup.m_nDistance = 500;
		break;
	case 7:
		g_stCustomSetup.m_nDistance = 1000; 
		break;
	case 8:
		g_stCustomSetup.m_nDistance = 2000;
		break;
	default:
		nResult = 0;
		break;
	}
	if( (nFlag > 0) && (nFlag < 9) ){
		if( GPS_IsPosValid() ){		//GPS��λ��Ч			
			g_stCustomSetup.m_nCLng = GPS_GetLongitude();	//����
			g_stCustomSetup.m_nCLat = GPS_GetLatitude();	//γ��			
			TE_EnableOutGeo();
		}		
		else{
			g_stCustomSetup.m_nDistance = 0;
			TE_DisableOutGeo();	
			nResult = 0;
		}	
	}
	return nResult;
}
/*
///////////////////////////////////////////////////////////////////
//	���õ���Χ������
unsigned char	PLS_SetupFence( unsigned char	*pSrc,
								unsigned char	nCmdCode ){

	unsigned char	nResult = 1;
	unsigned char	*pch = NULL;
	unsigned char	*pch1 = NULL;
	
	
	pch = pSrc+WHERE_MESSAGE_DATA;	
	// ����
	pch1 = (unsigned char*)strchr((char*)pch, ',');
	if( pch1 == NULL ){
	  return 0;
	}
	*pch1 = 0x00;
	g_stAlarmSetup.m_oStGFence[0].m_LLng = PLS_ExplainLongitudeData( pch );
	pch = pch1 + 1;
	if( (*pch == 'E') || (*pch == 'e') ){
		g_stAlarmSetup.m_oStGFence[0].m_LLng *= 1;
	}
	else{
		g_stAlarmSetup.m_oStGFence[0].m_LLng *= -1;
	}
	pch += 2;
	// γ��
	pch1 = (unsigned char*)strchr((char*)pch, ',');
	if( pch1 == NULL ){
	  return 0;
	}
	*pch1 = 0x00;
	g_stAlarmSetup.m_oStGFence[0].m_LLat = PLS_ExplainLatitudeData(pch);
	pch = pch1 + 1;
	if( (*pch == 'N') || (*pch=='n') ){
		g_stAlarmSetup.m_oStGFence[0].m_LLat *= 1;
	}
	else{
		g_stAlarmSetup.m_oStGFence[0].m_LLat *= -1;
	}
	pch += 2;
	// ����
	pch1 = (unsigned char*)strchr((char*)pch, ',');
	if( pch1 == NULL ){
		return 0;
	}
	*pch1 = 0x00;
	g_stAlarmSetup.m_oStGFence[0].m_RLng = PLS_ExplainLongitudeData(pch);
	pch = pch1 + 1;
	if( (*pch == 'E') || (*pch == 'e') ){
		g_stAlarmSetup.m_oStGFence[0].m_RLng *= 1;
	}
	else{
		g_stAlarmSetup.m_oStGFence[0].m_RLng *= -1;
	}
	pch += 2;
	// γ��
	pch1 = (unsigned char*)strchr((char*)pch, ',');
	if( pch1 == NULL ){
	  return 0;
	}
	*pch1 = 0x00;
	g_stAlarmSetup.m_oStGFence[0].m_RLat = PLS_ExplainLatitudeData(pch);
	pch = pch1 + 1;
	if( (*pch == 'N') || (*pch=='n') ){
		g_stAlarmSetup.m_oStGFence[0].m_RLat *= 1;
	}
	else{
		g_stAlarmSetup.m_oStGFence[0].m_RLat *= -1;
	}	
	// ��־λ
	g_stAlarmSetup.m_oStGFence[0].m_nFlag |= 0x21;
	return nResult;	
}
//========================================================================================
//����γ�Ⱥ���
//����:
//      pData �������
//========================================================================================
int 	PLS_ExplainLatitudeData(unsigned char *pData)			
{
	unsigned char  nLen = 0;	
	unsigned char  nDegree = 0;
	unsigned char  nMinute = 0;
	unsigned short nPrecision = 0;	
	int			   nResult = 0;
	char 		   *pch = NULL;
	char		   *pch1 = NULL;
		
	pch = (char *)pData;     		//��ȡ����
	pch1 = strchr(pch,'.');     	//ȡС����ǰ������
	if(pch1)
	{
		pch[pch1 - pch] = 0;
		nLen = strlen(pch);
		if (nLen == 1)	{
			nMinute = (*pch) - '0';	
		}
		else if (nLen == 2) {
			nMinute = ((*pch) - '0') * 10;	
			pch++;
			nMinute += (*pch) - '0';	
		}
		else if (nLen == 3) {
			nDegree = (*pch) - '0';
			pch++;
			nMinute = ((*pch) - '0') * 10;	
			pch++;
			nMinute += (*pch) - '0';	
		}
		else if (nLen == 4) {
			nDegree = ((*pch) - '0') * 10;
			pch++;
			nDegree += (*pch) - '0';
			pch++;
			nMinute = ((*pch) - '0') * 10;	
			pch++;
			nMinute += (*pch) - '0';
		}
		// ȡС��λ	
		pch = pch1 + 1;					
		nPrecision = atoi( (char*)pch );
		
		nResult = nDegree*3600000;
		nResult += nMinute * 60000;
		nResult += nPrecision * 6;
	}		
	return nResult;
}
//========================================================================================
// ���;��Ⱥ���
// ����:
//      pData �������24.1278 0001.23 
//========================================================================================
int  PLS_ExplainLongitudeData(unsigned char *pData)     
{
	
	unsigned char  nLen = 0;	
	unsigned char  nDegree = 0;
	unsigned char  nMinute = 0;
	unsigned short nPrecision = 0;	
	int			   nResult = 0;
	char 		   *pch = NULL;
	char		   *pch1 = NULL;
	
	
	pch = (char *)pData;     		//��ȡ����
	pch1 = strchr(pch,'.');     	//ȡС����ǰ������   
	if( pch1 ){			
		pch[pch1 - pch] = 0;
		nLen = strlen(pch);
		nDegree = 0;
		if (nLen == 1)		{
			nMinute = (*pch) - '0';	
		}
		else if (nLen == 2) {
			nMinute = ((*pch) - '0') * 10;
			pch++;
			nMinute += (*pch) - '0';	
		}
		else if (nLen == 3) {
			nDegree = (*pch) - '0';
			pch++;
			nMinute = ((*pch) - '0') * 10;
			pch++;
			nMinute += (*pch) - '0';	
		}
		else if (nLen == 4) {
			nDegree = ((*pch) - '0') * 10;
			pch++;
			nDegree += (*pch) - '0';
			pch++;
			nMinute = ((*pch) - '0') * 10;
			pch++;
			nMinute += (*pch) - '0';			
		}
		else if (nLen == 5)  {
			nDegree = ((*pch) - '0') * 100;
			pch++;
			nDegree += ((*pch) - '0') * 10;
			pch++;
			nDegree += (*pch) - '0';
			pch++;
			nMinute = ((*pch) - '0') * 10;	
			pch++;
			nMinute += (*pch) - '0';
		}
	}
	// ��ȡС��λ
	pch = pch1 + 1;      
	nPrecision = atoi( (char*)pch );
	
	nResult = nDegree*3600000;
	nResult += nMinute * 60000;
	nResult += nPrecision * 6;
	
	return nResult;	
}
*/
#endif  //SW_GPRSPROTOCOL_ASICO


