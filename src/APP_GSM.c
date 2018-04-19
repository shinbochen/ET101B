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
#include "..\hdr\APP_EM200.h" 
#include "..\hdr\APP_GPS.h"
#include "..\hdr\HW_UART.h"
#include "..\hdr\HW_flash.h"
#include "..\hdr\APP_LoopDetect.h"
#include "..\hdr\APP_Terminal.h"
#include "..\hdr\HW_GPIO.h"
#include "..\hdr\APP_GTM900.h"
#include "..\hdr\APP_BQM26.h"
#include "..\hdr\APP_SIMCOM.h"
#include "..\hdr\APP_EM200.h"
#include "..\hdr\APP_MC323.h"
#include "..\hdr\APP_MG323.h"
#include "..\hdr\ProtocolA.h"
#include "..\hdr\ProtocolB.h"
#include "..\hdr\ProtocolC.h"
#include "..\hdr\SMSProtocolA.h"
#include "..\hdr\SMSProtocolB.h"
#include "..\hdr\SMSProtocolC.h"

#include "..\hdr\HW_Storage.h" 
#include "..\hdr\HW_SPI.h"
#include "..\hdr\target.h" 


///////////////////////////////////////////////////////////
#if( HW_GTM900 )
#define				CMM_OK								GSM_OK
#define				CMM_ERROR							GSM_ERROR
#define				CMM_CONNECT							GSM_CONNECT

#endif	//HW_GTM900

#if( HW_EM200 )
#define				CMM_OK								CDMA_OK
// ����EM200�ڳ�ERROR�󻹳���OK�ַ�.���Բ��ܽ�CME��Ϊ��������
// ��:
//		+CME ERROR:socket not connected
//		OK
// ����				CMM_ERROR	= CDMA_OK����CDMA_CME
//#define				CMM_ERROR							CDMA_CME
#define				CMM_ERROR							CMM_CMD_UNKNOW
#define				CMM_CONNECT		 				CDMA_OK
#endif	// HW_EM200

#if( HW_BQM26 )
#define				CMM_OK								GSM_OK
#define				CMM_ERROR							GSM_ERROR
#define				CMM_CONNECT							GSM_OK
#endif //HW_BQM26

#if( HW_SIMCOM )
#define				CMM_OK								GSM_OK
#define				CMM_ERROR							GSM_ERROR 
//#define				CMM_CONNECT							GSM_CONNECT_OK
#define				CMM_CONNECT							GSM_CME_ERROR

#endif //HW_SIMCOM

#if( HW_MC323 )
#define				CMM_OK								CDMA_OK
// ����MC323�ڳ�ERROR�󻹳���OK�ַ�.���Բ��ܽ�CME��Ϊ��������
// ��:
//		+CME ERROR:socket not connected
//		OK
// ����				CMM_ERROR	= CDMA_OK����CDMA_CME
//#define				CMM_ERROR							CDMA_CME
#define				CMM_ERROR							CMM_CMD_UNKNOW
#define				CMM_CONNECT		 					CDMA_OK
#endif	// HW_MC323


#if( HW_MG323 )
#define				CMM_OK								GSM_OK
#define				CMM_ERROR							GSM_ERROR
#define				CMM_CONNECT							GSM_OK

#endif	//HW_MG323


unsigned char	*lpLargeBuf1 = (unsigned char*)&g_RamBuffer[0];
unsigned char	*lpLargeBuf2 = (unsigned char*)&g_RamBuffer[1024];
unsigned char *lpLargeBuf3 = (unsigned char*)&g_RamBuffer[2048];
unsigned char *lpLargeBuf4 = (unsigned char*)&g_RamBuffer[3072];				
				
				
///////////////////////////////////////////////////////////
// ����GSM���ݴ洢��
unsigned int 	nGSMRecvBuf[(GSM_RECMAXBUFFER + sizeof(int) - 1) / sizeof(int)];

OS_EVENT 			*pGSMReceiveSem = NULL;  
OS_STK				TaskGSMDataRecvStk[TASK_GSMDATA_RECEIVE_STK_SIZE];			
OS_STK				TaskGSMAppStk[TASK_GSMAPP_STK_SIZE];	

#define				MAX_NEWMSG		20
unsigned char		nNewMsgBuf[MAX_NEWMSG];
unsigned char 		nReadPtNewMsg = 0;
unsigned char 		nWritePtNewMsg = 0;
unsigned char		nNewMsgLen = 0;   



///////////////////////////////////////////////////////////
//  FLASH GSM����״̬ 
STGSMSETUP			g_stGSMSetup = {     
										C_CommMode,  //0: SMS 1:GPRS 
													 //0x01: �����ϴ���ʱ�� 
													 //0x02: �����ϴ�������
													 //0x03: �����ϴ���ʱ��
													 //0x04: �����ϴ������� 
													 //0x05: ֹͣ�ϴ� 
										C_CBMode,
										30,		 //ʱ����  
										
										DEFAULT_PSD,	 //����
										"",		 	     //���ĺ��� 
										C_MaterNo,	   // ���ƺ��� 669475
										"",		 			 //��Ȩ����1 
										"",		 		   //��Ȩ����2
										"",				   //��Ȩ����3
										
										"",		 //��Ȩ����4
										"",		 //��Ȩ����5
										"",		 //��ַ�������
										
										"",
										C_ServerIP,		//IP��ַ 
										8868,					//�˿ں�
										C_APN,				//����
										C_APNUser,	 	//SIM GPRS�û��� card
										C_APNPsd,			//SIM GPRS���� card
										"",						//UID
										
										60,						//����ʱ��					  
										0,						//��·״̬ 
										""									
										};										
										
///////////////////////////////////////////////////////////
//  GSM״̬
STGSMSTATE			g_stGSMState = {
									0,		//״̬��־
									0,		//�ź���
									0,		//���ִ���
									0,		//��¼��������	
									0,		//TCP���Ӳ��ɹ�,����
									0,		//��ѯGPRS�����ź�
									0,		//��绰���λض���
									"",		//��Ϣת��
									"",	    //����ת������
									"",		//��绰�ض���
									0
									};

/////////////////////////////////////////////////////////////////////
// дGSM���ݽṹ
void GSM_WriteSetupData(void)	{

	#if (EXTERNAL_FLASH)
		WriteDataToFlash2( FLASH_GSMSETUP_ADDR_START, (unsigned char*)&g_stGSMSetup, sizeof(STGSMSETUP));		
	#else
		WriteDataToFlash((char*)FLASH_GSMSETUP_ADDR_START, (char*)&g_stGSMSetup, sizeof(STGSMSETUP));
	#endif
	
	return ;
}

// ��ȡGSM���ݽ�
void GSM_ReadSetupData(void)	{	

	#if (EXTERNAL_FLASH)
		ReadDataFromFlash( (unsigned char*)&g_stGSMSetup, FLASH_GSMSETUP_ADDR_START, sizeof(STGSMSETUP) );
	#else	
		memcpy((char*)&g_stGSMSetup, (char*)FLASH_GSMSETUP_ADDR_START, sizeof(STGSMSETUP));
	#endif
	
	return ;
}

/////////////////////////////////////////////////////////////////////////
//GSM ATָ�
unsigned char CMM_SendATCmd(unsigned char nCmdType, 
														unsigned char nLFRN,
														unsigned char *pMsg){
			
	GSMUARTSendStr( (unsigned char*)TEXT_AT );
	if( nCmdType != CMM_CMD_END ){
		GSMUARTSendStr( (unsigned char*)s_strCMMCMD[nCmdType] );
		if( pMsg ){
			GSMUARTSendStr( pMsg );
		}		
	}
	if ( nLFRN == 0 ){
		GSMUARTSendStr( (unsigned char*)TEXT_LF_RN );
	}
	else{
		if( nLFRN == 2 ){
			GSMUARTSendStr( (unsigned char*)TEXT_CR );
		}
		else{
			GSMUARTSendStr( (unsigned char*)TEXT_LF );
		}
	}
	
	while( !GSMUARTIsSendBufEmpty( ) ){ 
		OSTimeDly( 2 );	
	}
	return GSM_SUCCEED;
}
						 
////////////////////////////////////////////////////////////////////////
// ִ��GSM ATָ��
// nCmdType 	: ��������
// pPara			: �������
// pReturnMsg	: ��Ϊ�ձ�ʾ��Ҫ�õ����ص�����( �����������ַ����� )
//						: Ϊ�� ֻҪ�ȵ�OK
unsigned char CMM_ExecATCmdN( unsigned char  nCmdType, 	
							  unsigned short nPara,
							  unsigned short nTimeout,
							  unsigned char  nOKType,		
							  unsigned char  nNGType,
							  unsigned char  *pReturnMsg ){
		unsigned char nBuf[20];
		
		sprintf( (char*)nBuf, "=%d", nPara );
		return CMM_ExecATCmd( nCmdType, nBuf, nTimeout, nOKType, nNGType, pReturnMsg );
}
														 
////////////////////////////////////////////////////////////////////////
// ͨ��ִ��GSM ATָ��
// nCmdType 	: ��������
// pPara		: �������
// pReturnMsg	: ��Ϊ�ձ�ʾ��Ҫ�õ����ص�����( �����������ַ����� )
//				: Ϊ�� ֻҪ�ȵ�OK
// CGSN�ر���
unsigned char CMM_ExecATCmd( unsigned char nCmdType, 	
							 unsigned char *pPara,
							 unsigned short nTimeout,	
							 unsigned char nOKType,		
							 unsigned char nNGType,
							 unsigned char *pReturnMsg ){
														 	
	
	unsigned char 	nResult = GSM_NG;
	unsigned char	nCnt = 0;
	unsigned char 	nError = 0;
	unsigned char 	nType = 0;
	unsigned short 	nLen = 0;
	unsigned char	*lpLargeBuf = NULL;
	
	lpLargeBuf	= lpLargeBuf4;
	//2011-11-16 �޸�
	#if ( HW_EM200 == 0 &&  HW_MC323 == 0 && HW_MG323 == 0 )
		if ( nCmdType == GMS_CUSD ){
			sprintf( (char*)lpLargeBuf, "=1,\"%s\",15", pPara );
			CMM_SendATCmd( nCmdType, 0, lpLargeBuf );
		}
		else{	
			CMM_SendATCmd( nCmdType, 0, pPara );		
		}
	#else
		CMM_SendATCmd( nCmdType, 0, pPara );
	#endif
	
	while ( 1 ){
		
		OSSemPend(pGSMReceiveSem, nTimeout, &nError);
		if( nError == OS_TIMEOUT ){
			if ( nCnt++ > 4 ){
				break;
			}
		}
		else{ 
			nLen = CMM_ReadCmd( lpLargeBuf , GSM_RECMAXBUFFER-1 );
			//2011-11-16 �޸�
			#if ( HW_EM200 == 0 && HW_MC323 == 0 )
				// GSM_ATD �����IMEI��ŵ�ǰ�� 			
				// ��ѯSN:������������,���ر���
				if( nCmdType == GSM_CGSN ){	
					if( (strlen((char*)lpLargeBuf) > 10 ) &&
						(strchr( (char*)lpLargeBuf, ' ') == NULL ) &&
					    (strchr( (char*)lpLargeBuf, '"') == NULL ) &&
					    (strchr( (char*)lpLargeBuf, ',') == NULL ) &&
					    (strchr( (char*)lpLargeBuf, ':') == NULL ) &&
					    (strchr( (char*)lpLargeBuf, '+') == NULL ) &&
					    (strchr( (char*)lpLargeBuf, '?') == NULL ) ){
					    
					    nResult = GSM_SUCCEED;
					    if( pReturnMsg ){
					    	strcpy( (char*)pReturnMsg, (char*)lpLargeBuf );	
					    }				    
					}					   
				}
				else if( nCmdType == GSM_ATD ){
					if( strlen((char*)lpLargeBuf) > 10 ){
						nResult = GSM_SUCCEED;
						if( pReturnMsg ){
							strcpy( (char*)pReturnMsg, (char*)lpLargeBuf );					
						}
					}
				}
			#endif 
			
			
			#if( HW_MC323 )
				nType = CMM_GetCmdType(lpLargeBuf, nLen);	
			#else
				nType = CMM_GetCmdType(lpLargeBuf);			
			#endif //HW_MC323
			
			// 1: �յ�OK��ERROR��CONNECT һ��Ҫ��quit
			// 2: ��������ָ��������������ͬʱ����ظ�ֵ
			
			if( nType == CMM_OK || nType == CMM_ERROR || nType == CMM_CONNECT ){
				if( nType == nOKType ){
					nResult = GSM_SUCCEED;
				}
				if( nType == nNGType ){
					nResult = GSM_NG; 
				}			
				break;
			}
			// ����Ҫ��OK��ERROR����quit
			else if( nType == nOKType || nType == nNGType ){
					
				nTimeout = GSM_LOWAIT_TIME;
				if( nType == nOKType ){
					nResult = GSM_SUCCEED;
				}
				else{
					nResult = GSM_NG;
				}
				if( pReturnMsg  && (strlen((char*)lpLargeBuf) > 0 )){					
					//2011-11-16 �޸�
					#if( HW_EM200 == 0 && HW_MC323 == 0 )
						if( (nCmdType != GSM_CGSN) && (nCmdType != GSM_ATD ) ){		
							strcpy( (char*)pReturnMsg, (char*)lpLargeBuf );	
						}	
					#else
						strcpy( (char*)pReturnMsg, (char*)lpLargeBuf );
					#endif
				}
			}
			else{
				CMM_ParseMasterCmd(lpLargeBuf, nType);				
			}	
		}				
	}
	return nResult;						 	
}

// ��ʼ��GSM state
void CMM_InitState( void ){	
	memset( &g_stGSMState, 0, sizeof( STGSMSTATE ) );
	g_stGSMState.m_nGPRSAlarmSMSState = g_stDeviceState.m_nALState;
	memset( nNewMsgBuf, 0 , MAX_NEWMSG );
	nReadPtNewMsg = 0;
	nWritePtNewMsg = 0;
	nNewMsgLen = 0;
}

/////////////////////////////////////////////////
void InsertNewMsg( unsigned char nCnt ){
	
	nNewMsgBuf[nWritePtNewMsg] = nCnt;
	nWritePtNewMsg++;
	nNewMsgLen++;
	if( nWritePtNewMsg == MAX_NEWMSG ){
		nWritePtNewMsg = 0;
	}
	return;
	
}
unsigned char GetNewMsg( ){

	unsigned char nResult = 255;
	
	if( nNewMsgLen ){		
		nResult = nNewMsgBuf[nReadPtNewMsg];
		nReadPtNewMsg++;
		if( nReadPtNewMsg == MAX_NEWMSG ){
			nReadPtNewMsg = 0;
		}
		nNewMsgLen--;
	}	
	return nResult;
	
}
/////////////////////////////////////////////////
// GSM���߳̿�
void GSM_Task_Creat( void ){	 	
	OSTaskCreate(GSM_TaskApp, (void *)0, &TaskGSMAppStk[TASK_GSMAPP_STK_SIZE - 1], 5); 	
	return;
}


////////////////////////////////////////////////////////////
// ���ڽ�������
// GSM
void GSM_TaskDataReceive(void * pData){ 

	unsigned char 	 nTemp = 0;
	unsigned char	 nTemp2= 0;
	unsigned char	 nPre	= 0;	
	unsigned char 	 nError = 0;
		
	pData = pData;
	
	QueueCreate(nGSMRecvBuf, sizeof(nGSMRecvBuf), NULL, NULL);	
	QueueFlush(nGSMRecvBuf); 
	
	while (1){
		OS_ENTER_CRITICAL(); 	
		#if( HW_SIMCOM == 0 )
		nPre  = nTemp;		
		nTemp = GSMUARTGetch( &nError, 0 );	
		//ΪSIMCOM��GPRSģʽ�½���GPRS����û��\r\n�ַ�,���������⴦��	
		#else
		nError = 0;
		nTemp2 = GSMUARTGetch( &nError, 100 );		
		// 10ms
		if( nError == OS_TIMEOUT ){
			if ((nPre != 0x0D) || (nTemp != 0x0A)){	
					
				nPre = 0x0D;
				nTemp = 0x0A;
				QueueWrite(nGSMRecvBuf, 0x0D);
				QueueWrite(nGSMRecvBuf, 0x0A);
				if( pGSMReceiveSem ){
					OSSemPost(pGSMReceiveSem); 
				} 				 		
			}	
			OS_EXIT_CRITICAL();
			continue;
		}
		else{
			nPre  = nTemp;	
			nTemp = nTemp2;	
		}
		#endif	
		if (QueueWrite(nGSMRecvBuf, nTemp) == QUEUE_OK)	{ 	 			
			if ((nPre == 0x0D) && (nTemp == 0x0A)){				
				if( pGSMReceiveSem ){
					OSSemPost(pGSMReceiveSem); 
				}  				
			}	
			else if( nTemp == '>'){   		
				CMM_SetSendSMSReady( );
			}				
			//else if((nPre == '>') && (nTemp ==' ')){   
			//	QueueRollBack( nGSMRecvBuf, 2 );
			//	CMM_SetSendSMSReady( );
			//	if( pGSMReceiveSem ){
			//		OSSemPost(pGSMReceiveSem);
			//	}
			//}   

		}
		OS_EXIT_CRITICAL();
	}
	return;
}
/////////////////////////////////////////////////////////////////////////////
//�������ݽ��պ�Ĵ�������
void GSM_TaskApp(void *pData)	{ 			
	
	unsigned char 			err = 0;	
	unsigned char			nLen = 0;
	unsigned char 			nCmdType = 0;	
	unsigned char			*lpLargeBuf = NULL;
	unsigned char			nCodeType = 0;
	unsigned short			nShortLen = 0;
//	unsigned char			nResetFlag = 0;			
	static unsigned char	nSMSMode = 0;
	
	lpLargeBuf = lpLargeBuf1;	
	pData = pData;	
			
	// ��ʼ��Ӳ������ 
	InitHardware();	
	// ��ʼ��FLASH
	InitFlash();
	// ��·����Ƿ�Ҫ����
	if( GSM_IsOilState() ){
		
		HW_CTRLOIL_CUTOFF();						
		GSM_ClrOilState();
		TE_SetNeedWriteFlash( ); 
	} 

	OSTaskCreate(GSM_TaskDataReceive, (void *)0,&TaskGSMDataRecvStk[TASK_GSMDATA_RECEIVE_STK_SIZE - 1], 4);	 
	
					
	GSM_SetNeedReset( );	
	while (1){		
		
		if( GSM_IsNeedReset( ) ){	 		
			GSM_SetUnNeedReset( );
			CMM_InitState( );
			
			CMM_ReStart();
			
			// �ر�ι��
			TE_SetNeedTEReset();
			CMM_Init( );
			// ����ι��
			TE_ClrNeedTEReset();
			
			if( GSM_IsGPRSCommMode() ){
			
				if( CMM_IPInit( g_stGSMSetup.m_strGateway, 
								g_stGSMSetup.m_strUser, 
								g_stGSMSetup.m_strPsd ) == GSM_SUCCEED ){
					// ������IP��ַ
					if( strlen( (char*)g_stGSMSetup.m_strServerDomain) > 0 ){
					
						if( GSM_ParseDNS( g_stGSMSetup.m_strServerDomain, 
										  lpLargeBuf ) == GSM_SUCCEED ){
							nLen = strlen( (char*)lpLargeBuf ); 			  
							if( nLen > SERVERDOMAIN_LENGTH ){
								nLen = SERVERDOMAIN_LENGTH;
							} 
							memcpy( g_stGSMSetup.m_strServerIP,lpLargeBuf, nLen);
							g_stGSMSetup.m_strServerIP[nLen] = 0x00;
						}
					}						
					if( CMM_IPConnect( IP_IsTCPCommMode(), 
					  				 g_stGSMSetup.m_strServerIP, 
					  				 g_stGSMSetup.m_nServerPort ) == GSM_SUCCEED ){
					  
					  IP_SetConnectOK( );
					 }
				}
				else{
					IP_SetConnectNG( );
				} 
			}
		} 
		// 100*5 = 500 ms
		OSSemPend(pGSMReceiveSem, 100, &err);         	 	// �ȴ��������� 
		if( err != OS_TIMEOUT ){			
			while ( 1 ){ 								
				nShortLen = CMM_ReadCmd(lpLargeBuf, GSM_RECMAXBUFFER-1);	
				#if( HW_MC323 )
					nCmdType =  CMM_GetCmdType( lpLargeBuf, nShortLen );
				#else
					nCmdType =  CMM_GetCmdType( lpLargeBuf );
				#endif //HW_MC323
				CMM_ParseMasterCmd(lpLargeBuf, nCmdType);
				// �������� 
				OSSemPend(pGSMReceiveSem, GSM_LOWAIT_TIME, &err);
				if( err == OS_TIMEOUT ){
					break;	
				} 
			}
		}		
		// GPRS Mode
		if( GSM_IsGPRSCommMode( ) ){
			if (TE_IsNeedUploadData()){ 	
				
				// ACC�غ�Ŀ���ģʽ
				if( TE_IsIdleGPRS() == 0 ){	
				
					if( TE_IsLoginSuccess( ) ){				
						nLen = PL_ComposeGPSData( lpLargeBuf );
						if ( IP_SendData(IP_IsTCPCommMode(), lpLargeBuf, nLen) != GSM_SUCCEED ){						
									
							CMM_SaveGPSToFlash();
							IP_SetConnectNG( );
						}
						TE_ClrNeedUploadData( );
					}
					else{ 				
						CMM_SaveGPSToFlash();
						TE_ClrNeedUploadData( );
					}
				}
				if( TE_HaveAlarm() ){
					if ( g_stGSMState.m_nGPRSAlarmSMSState != g_stDeviceState.m_nALState ){
					
						g_stGSMState.m_nGPRSAlarmSMSState = g_stDeviceState.m_nALState;
						CMM_SMSSendAlarmData( lpLargeBuf );
					}						
					// ����Զ���1����
					AL_ClrCustom1();
					// ����Զ���2����
					AL_ClrCustom2();				
					// ����񶯱���
					AL_ClrCustom4();
					// ���ACC����
					AL_ClrACCAlarm();	
					
					GPRS_ClrNumberSMSMode( AL_CUSTOM1 );
					GPRS_ClrNumberSMSMode( AL_CUSTOM2 );
					GPRS_ClrNumberSMSMode( AL_CUSTOM4 );
					GPRS_ClrNumberSMSMode( AL_ACC_ALARM );
				}
			}
			// ACC�غ�Ŀ���ģʽ
			if( TE_IsIdleGPRS() == 0 ){
			
				CMM_IPMAINTENANCE( (unsigned char*)lpLargeBuf);	
			}
			// �����ά��Flash������			
			if( TE_IsLoginSuccess( ) ){		
				CMM_FlashManagement( (unsigned char*)lpLargeBuf);	
			}
		
		}	
		// GSM Mode	
		else{
			if( TE_IsNeedUploadData( ) ){				
				TE_ClrNeedUploadData();
				if( TE_HaveAlarm( ) ){
					CMM_SMSSendAlarmData( lpLargeBuf  );
					TE_ClearAlarmState();
				}
				else{
					if( COM_IsSMSFeedbackUseJave() ){
						nLen = PLS_ComposeCLUploadData( lpLargeBuf, &nCodeType );
					} 	
					else{	
						if( COMM_IsSMSFeedbackUseProtocol() ){
							nLen = PL_ComposeGPSData( lpLargeBuf );
							nCodeType = 1; 			//8bit��ʽ����
						}	
						else{		
							nLen = PLS_ComposeUploadData( lpLargeBuf, &nCodeType );																								 				
						}
					}
					CMM_SendSMSData( g_stGSMSetup.m_strMaterNo, lpLargeBuf, nLen, nCodeType );
				}
			}		
		}
		// дFLASH��־
		if ( TE_IsNeedWriteFlash() ){			
			GSM_WriteSetupData();
			WriteAlarmSetupData();
			TE_ClrNeedWriteFlash(); 
		}		
		
		// process new msg 
		while( 1 ){		
			nLen = GetNewMsg();				
			if( nLen != 255 ){
				CMM_ParseNewMsg( nLen ); 
				OSTimeDly(100);			
			}
			else{
				break;
			}
		}	
		// �ڶ���ģʽ��,�Ѳ���������Ϣ�ϴ�
		if( !GSM_IsGPRSCommMode() ){
			if( COMM_IsSMSFeedbackUseProtocol() && ( nSMSMode == 0) ){
			
				nLen = PL_ComposeLogin( lpLargeBuf );				
				#if( HW_EM200 )
					CMM_SendSMSData( g_stGSMSetup.m_strMaterNo, lpLargeBuf, nLen, 0 );
				#else
					#if( HW_MC323 )
						 CMM_SendSMSData( g_stGSMSetup.m_strMaterNo, lpLargeBuf, nLen, 2 );
					#else					
						 CMM_SendSMSData( g_stGSMSetup.m_strMaterNo, lpLargeBuf, nLen, 1 );
					#endif
				#endif				
								 						
				nSMSMode = 1;
			}
		}
		// ��������ģʽ
		if( !GSM_IsSetupConfig() ){	
			CMM_SignalTest();
			CMM_ATTest( );	
		}		
		// ����绰�ض���
		if ( g_stGSMState.m_nDialLatLng > 0 ){
					
			if( GSM_ReadCaller( lpLargeBuf ) == 0 ){
										
				g_stGSMState.m_nDialLatLng = 0;
				OSTimeDlyHMSM(0,0,2,0);							
				nLen = PLS_ComposeUploadData( lpLargeBuf, &nCodeType );
				CMM_SendSMSData( g_stGSMState.m_strDialNo, lpLargeBuf, nLen, nCodeType );
			}
		}
		// ���������̱���
		if( GetMaxMileage() > 0 ){			
		
			if( GetCurrentMileage() > GetMaxMileage() ){
			
				nLen = PLS_ComposeMileage( lpLargeBuf, &nCodeType );
				CMM_SendSMSData( g_stGSMSetup.m_strMaterNo, lpLargeBuf, nLen, nCodeType );
				ClearCurrentMileage();
			}	
		}		
	}
	return;
}
///////////////////////////////////////////////////////////
// 	��֯���ŷ��� (2010-05-12) ��Ϸ�����,��ʡ����ռ�
//	    pPhone:	 ���ͺ���
//	lpLargeBuf:  ��������
//		  nLen:  ���ݳ���
//	 nCodeType:  ���͸�ʽ
void CMM_SendSMSData( unsigned char	*pPhone,unsigned char	*lpLargeBuf,unsigned char	nLen,unsigned char	nCodeType ){
					  	
	unsigned char	nCnt = 0;						  	
	unsigned char	nResultLen = 0;
	
	#if( HW_EM200 || HW_MC323  )
		if( strlen( (char*)pPhone) ){
			while( nCnt++ < 3 ){
				switch( nCodeType ){
				case 0:
					nResultLen = CMM_SendASCIIData( pPhone, lpLargeBuf, nLen);
					break;
				case 1:
					nResultLen = CMM_SendOctetData( pPhone, lpLargeBuf, nLen );
					break;
				case 2:
				default:
					nResultLen = CMM_SendUnicodeData( pPhone,lpLargeBuf,nLen);
					break;
				}
				if( nResultLen == GSM_SUCCEED ){
					break;
				}
				OSTimeDlyHMSM(0,0,2,0);
			}
		}		
	#else	
		if( strlen( (char*)pPhone) ){
			switch( nCodeType ){
			case 0:	//7bit ���뷢��												
                nLen = GSM_ComposeSend7GSMData( lpLargeBuf,
												&nResultLen, 
												
												g_stGSMSetup.m_strSMCNo, 
												pPhone,
												lpLargeBuf);												
				break;
			case 1:	//8bit ���뷢��
				nLen = GSM_ComposeSend8GSMData( lpLargeBuf, 
												&nResultLen,
												 	 
									 			g_stGSMSetup.m_strSMCNo,  
									 			pPhone,
								 				lpLargeBuf,
								 				nLen );
				break;
			case 2:
			default:
				nLen = GSM_ComposeSend16GSMData( lpLargeBuf,
												 &nResultLen, 
												
												 g_stGSMSetup.m_strSMCNo, 
												 pPhone,
												 lpLargeBuf,
												 nLen );
				break;
			}			
			if( nLen ){
				nCnt = 0;
				while( nCnt++ < 3 ){
					if(GSM_SendPDUSMS( lpLargeBuf, nResultLen, nLen ) == GSM_SUCCEED ){
						break;
					}
					OSTimeDlyHMSM(0,0,2,0);
				}	
			}
		}
	#endif	
}
///////////////////////////////////////////////////////////
//  ���ͱ���������������  
void  CMM_SMSSendAlarmData( unsigned char *lpLargeBuf ){
	
	unsigned char		 	nLen = 0;	
	unsigned char			nCodeType;
	unsigned char			nNumber = 0;
	// 1:	GBK_CHRIS���ݷ�ʽ����(Ӣ���ͻ�)	
	// 2:	�����뷽ʽ����
	// 3:	���غ��뷢������,���������뷽ʽ����
	unsigned char			nCmdType = 0;
	unsigned char			*pPhone = NULL;
	
	
	#if( HW_EM200 )		
		#if( GBK_CHRIS )
			nCmdType = 1;
		#else
			nCmdType = 2;
		#endif
	#else
		// Ӣ���ͻ��ı���״̬
		#if( GBK_CHRIS )  
			nCmdType = 1;
		#else
			nCmdType = 3; 
		#endif
	#endif
	
	switch( nCmdType ){
	case 1:	//GBK_CHRIS���ݷ�ʽ����(Ӣ���ͻ�)	
		for(nNumber = 0; nNumber < 2; nNumber++ ){
				
			nLen = PLS_ComposeUploadData( lpLargeBuf, &nCodeType ); 
			if( AL_IsSOS() ){
				pPhone = g_stGSMSetup.m_strAuthNo3;
			}
			else{
				if( nNumber == 0 ){
					pPhone = g_stGSMSetup.m_strAuthNo1;
				}
				else if( nNumber == 1){
					pPhone = g_stGSMSetup.m_strAuthNo2;
				}	
			}
			CMM_SendSMSData( pPhone, lpLargeBuf, nLen, nCodeType );	
			// ��������,����Ȩ����3
			if( AL_IsSOS() ){
				break;
			} 	
		}
		break;
	case 2:	//�����뷽ʽ����
		for(nNumber = 0; nNumber < 6; nNumber++ ){
				
			nLen = PLS_ComposeUploadData( lpLargeBuf, &nCodeType ); 
			switch( nNumber ){
			case 0:
				pPhone = g_stGSMSetup.m_strMaterNo;
				break;
			case 1:
				pPhone = g_stGSMSetup.m_strAuthNo1;
				break;
			case 2:
				pPhone = g_stGSMSetup.m_strAuthNo2;
				break;
			case 3:
				pPhone = g_stGSMSetup.m_strAuthNo3;
				break;
			case 4:
				pPhone = g_stGSMSetup.m_strAuthNo4;
				break;
			case 5:
				pPhone = g_stGSMSetup.m_strAuthNo5;
				break;
			}	
			CMM_SendSMSData( pPhone, lpLargeBuf, nLen, nCodeType );
		}
		break;
	case 3://���غ��뷢������,���������뷽ʽ����
		for(nNumber = 0; nNumber < 6; nNumber++ ){
					
			if( (!GSM_IsGPRSCommMode()) && COMM_IsSMSFeedbackUseProtocol() && ( nNumber == 0) ){
				nLen = PL_ComposeGPSData( lpLargeBuf );
				nCodeType = 1; 
			}
			else{
				nLen = PLS_ComposeUploadData( lpLargeBuf, &nCodeType );
			}				
			switch( nNumber ){
			case 0:
				pPhone = g_stGSMSetup.m_strMaterNo;
				break;
			case 1:
				pPhone = g_stGSMSetup.m_strAuthNo1;
				break;
			case 2:
				pPhone = g_stGSMSetup.m_strAuthNo2;
				break;
			case 3:
				pPhone = g_stGSMSetup.m_strAuthNo3;
				break;
			case 4:
				pPhone = g_stGSMSetup.m_strAuthNo4;
				break;
			case 5:
				pPhone = g_stGSMSetup.m_strAuthNo5; 
				break;
			}				
			CMM_SendSMSData( pPhone, lpLargeBuf, nLen, nCodeType );						
		}	
		break;
	default:
		break;
	}		
}
///////////////////////////////////////////////////////////
//��ȡGSM ATָ�����
int CMM_ReadCmd(unsigned char *pBuf, unsigned short nDataLen) {
	
	unsigned char 	nPre = 0;
	unsigned char 	nTemp = 0;	
	unsigned char 	bFlag = 0;
	unsigned short	nLen = 0;
	
	OS_ENTER_CRITICAL();		
	while( 1 ){
		nPre = nTemp;
		if( QueueRead((char*)&nTemp, nGSMRecvBuf) == QUEUE_OK) {
			
	 		*(pBuf+nLen) = nTemp;
	 		nLen++;
	 		
	 		if( (nTemp == 0x0A) && (nPre == 0x0D) )	 {
	 			// ȥ��0x0d,0x0a
	 			nLen -= 2;
 				bFlag = 1;
 				break;
	 		} 
		 	
		 	if( nLen >= nDataLen ){
		 		bFlag = 1;
		 		break;		 	
		 	} 	
		}
		else {
			bFlag = 0;
			break;
		}
	}
	if( bFlag == 0 ){
		*pBuf = NULL;
	}
	else{
		*(pBuf+nLen) = NULL;		
	}
	OS_EXIT_CRITICAL();
	return nLen;
}
////////////////////////////////////////////////////////////////////////////
// 
void CMM_ParseNewMsg( unsigned char nCnt ){
	
	unsigned char		nCnt1 = 0;	
	unsigned short		nLen;
	unsigned char 		nCaller[TELNUMBER_LENGTH+5];
	unsigned char		nDCSType;	
	unsigned char		nCodeType;
	unsigned char		nTPUDL;
	unsigned char		*lpSMCNo = NULL;
	unsigned char 		*pMsg = lpLargeBuf1;	
	
	
	#if( HW_EM200 || HW_MC323 )			// CDMAģ��
	nLen = CMM_ReadSMS( nCnt, nCaller, pMsg, &nDCSType );
	if( nLen > 0 ){
		if( IsForwardNo( nCaller ) ){
					
			CMM_SendSMSData( nCaller, pMsg, nLen, nDCSType );				
		}
		else{
			nCodeType = 0;
			// nCodeType = 0: �ı��������� ��ASCII��ʽ����
			// nCodeType = 1: ����Э������ ��OCTET��ʽ����
			// nCodeType = 2: ������������ ��unicode��ʽ����
			nLen = PL_ParseSMSData( pMsg, nLen, nCaller, (nDCSType==2)? DCS_UNICODE:DCS_8BIT, nLen, &nCodeType );				
			
			if( nLen > 0 ){
				CMM_SendSMSData( nCaller, pMsg, nLen, nCodeType );
			}			
		}
	}
	
	#else		
	/*	#if( HW_MC323 )
			nLen = CMM_ReadSMS( nCnt, nCaller, pMsg, &nDCSType );
			if( nLen > 0 ){
				if( IsForwardNo( nCaller ) ){
							
					CMM_SendSMSData( nCaller, pMsg, nLen, nDCSType );				
				}
				else{
					nCodeType = 0;
					// nCodeType = 0: �ı��������� ��ASCII��ʽ����
					// nCodeType = 1: ����Э������ ��OCTET��ʽ����
					// nCodeType = 2: ������������ ��unicode��ʽ����
					nLen = PL_ParseSMSData( pMsg, nLen, nCaller, (nDCSType==2)? DCS_UNICODE:DCS_8BIT, nLen, &nCodeType );				
					
					if( nLen > 0 ){
						CMM_SendSMSData( nCaller, pMsg, nLen, nCodeType );
					}			
				}
			}
		#else*/
			lpSMCNo = g_stGSMSetup.m_strSMCNo;
			// tempory not discuss
			// 1: default must delete
			// 2: if send failed( such as no money, this will cause server send a msg to this) 
			//    so, in delete prog. it will cause the read msg( master process)
			//   it maybe cause loop program.
			//  ��ȡ�������δ��󷵻�
			nCnt1 = 0;
			while( nCnt1++ < 3 ){
				nLen = GSM_ReadPDUSMS( nCnt, pMsg ); 
				if( nLen > 0 ){
					break;
				}
				OSTimeDlyHMSM(0,0,2,0);
			}	 
			if( nLen > 0 ){			
				nLen = GSM_ParsePDUSMS( pMsg, lpSMCNo, nCaller, &nTPUDL, &nDCSType,  pMsg  ); 	
				if ( nLen > 0 ){	
					// 1: ����>0��ʾ�а���Э�����ݷ���
					// 2: ����0, ��pMsg���ı����ݡ���ʾ��������Ϣ����
					// 3: ����0,��pMsgΪ�ա���ʾ����ش�����
					
					// nCodeType = 0: 7λ���ݡ��ı���������
					// nCodeType = 1: 8λ���뷢��������Э������
					// nCodeType = 2: unicode���롣 ��������
					// ����Ϊ0�����跢����
					nCodeType = 0;
					nLen = PL_ParseSMSData( pMsg, nLen, nCaller, GSM_GetDCSType(nDCSType), nTPUDL, &nCodeType );				
					if( nLen ){													
						CMM_SendSMSData( nCaller, pMsg, nLen, nCodeType );
					}
				}				
			}
	//	#endif	
	#endif	
	//OSTimeDly(800);
	CMM_DeleteSMS( nCnt );
	return;
}
///////////////////////////////////////////////////////////////////
//  ��ʱ���GSMģ��
void CMM_SignalTest( void ){
	
	unsigned char			nSignal = 0;
	static unsigned int		nCBand = 0;			//SIM300��Ƶ
	static unsigned int		nPrevTime = 0 ;
	static unsigned short	nLowSignalCnt = 0;
	
	if( OSTimeGet() - nPrevTime >= GSM_CHECK_SIGNAL_TIMEOUT ){
	
		nPrevTime = OSTimeGet();
		nSignal = CMM_QuerySignal( );
		GSM_SetSignal(nSignal);

		if ( nSignal <= GSM_SIGNAL_LOWBOUND ){
		
			if( nCBand++ >= GSM_LOW_SINGLE_MAXCNT/2 ){
			
				nCBand = 0;				
				CMM_ConfigCBand();	//  ת��SIMCOM��Ƶ
			}
			if ( nLowSignalCnt++ >=  GSM_LOW_SINGLE_MAXCNT){		
				// ����		
				GSM_SetNeedReset( );
				nLowSignalCnt = 0;
			}
		}
		else{
			nCBand = 0;
			nLowSignalCnt = 0;
		}
	}
	return;
}
//////////////////////////////////////////////////
// ά��UDP ����ͨ�� 
unsigned char CMM_IPMAINTENANCE( unsigned char	*lpLargeBuf )
{	
	unsigned char 				nResult = GSM_NG;
	unsigned char				nLen = 0;
	static unsigned int			nPrevTime = 0;
	
	if( nPrevTime == 0 ){
		nPrevTime = OSTimeGet( );
	}	
	if( IP_IsConnected() ){		
		if ( TE_IsLoginSuccess() ) {	
			if( OSTimeGet() - nPrevTime >= GSM_GetMaxHandShakeTime( ) ){			
				nPrevTime = OSTimeGet( );
			//  �ĳ�TCP/UDPͨ�Ŷ�Ҫ�������ź�
			//	if( !IP_IsTCPCommMode( ) ){
					nLen = PL_ComposeHSData( lpLargeBuf  );	
					if ( nLen > 0 ){			
						IP_SendData(IP_IsTCPCommMode(),lpLargeBuf, nLen);					
					}
					UDP_IncHandShakeCnt( );
			//	}
			} 
		}
		else{	//��½ָ��ά��
			if( OSTimeGet() - nPrevTime >= GSM_LOGIN_SPACE ){	
						
				nPrevTime = OSTimeGet( );
				nLen = PL_ComposeLogin( lpLargeBuf );
				if ( nLen > 0 ){
					IP_SendData(IP_IsTCPCommMode(), lpLargeBuf, nLen);					
				}
				UDP_IncHandShakeCnt( );				
			}
		}
		// ��������	 	
		if ( UDP_GetHandShakeCnt() >= UDP_HANDSHAKE_CNT ){				
			UDP_ClrHandShakeCnt();
			IP_SetConnectNG();  					// ������ӱ�־
			IP_IncConnectFailedCnt( );				
			// ������������5��,����ģ��
			if ( IP_GetConnectFailedCnt() >= IP_FAIL_CONNECT_MAX ){  			
				GSM_SetNeedReset();	
				IP_ClrConnectFailedCnt( );
			}		
		}
	}
	else{							
		if ( OSTimeGet() - nPrevTime > GSM_CONNECT_DEGREE ){	
			nPrevTime = OSTimeGet();
			TE_ClrLoginSuccess( );
			if( CMM_QueryIPNetworkStatus() ){					
				//TE_ClrLoginSuccess( );	
				CMM_IPClose( );
				// ������IP��ַ
				if( strlen( (char*)g_stGSMSetup.m_strServerDomain) > 0 ){

					if( GSM_ParseDNS( g_stGSMSetup.m_strServerDomain, 
									  lpLargeBuf ) == GSM_SUCCEED ){
						nLen = strlen( (char*)lpLargeBuf ); 			  
						if( nLen > SERVERDOMAIN_LENGTH ){
							nLen = SERVERDOMAIN_LENGTH;
						} 
						memcpy( g_stGSMSetup.m_strServerIP,lpLargeBuf, nLen);
						g_stGSMSetup.m_strServerIP[nLen] = 0x00;
					}
				}
				if( CMM_IPConnect( 	IP_IsTCPCommMode(), 
									g_stGSMSetup.m_strServerIP, 
									g_stGSMSetup.m_nServerPort ) == GSM_SUCCEED ){
					IP_SetConnectOK( );
				}
				else{		
					CMM_IPInit( g_stGSMSetup.m_strGateway,
								g_stGSMSetup.m_strUser, 
								g_stGSMSetup.m_strPsd );
					IP_SetConnectNG( );
											
					//GPRS_SetGateWay( g_stGSMSetup.m_strGateway );
					//GPRS_SetUserPsd( g_stGSMSetup.m_strUser, g_stGSMSetup.m_strPsd);															
					//IP_SetConnectNG( );
				}
				TCP_IncRCntionCnt();
				// �����ѯ����
				GSM_ClrCGREGCnt();
			}
			else{				
				// ��ѯGPRS�����ź�, 20����û���ź�����ģ��, 20��1�� 
				if( GSM_GetCGREGCnt() >= GSM_CGREG_MAXCNT ){
					GSM_SetNeedReset();
					GSM_ClrCGREGCnt();	
				}
				else{	//ע�������ź�
					CMM_ActivationPDP();
				}
				GSM_IncCGREGCnt();
			}
		}	
		// TCPģʽ ������������5��,����ģ�� 
		if( IP_IsTCPCommMode() ){
			if( TCP_GetRCntionCnt() >= IP_FAIL_CONNECT_MAX ){					
				GSM_SetNeedReset();	
				TCP_ClrRCntionCnt();
			}	
		}
	}			
	return nResult;
}
//////////////////////////////////////////////
// 	����Flash���ݵķ���
unsigned char	CMM_FlashManagement( unsigned char *lpLargeBuf ){	

	unsigned char				nLen = 0;
	unsigned short				nPackage;
	static unsigned char		nQueryAddCnt = 0;
	static unsigned int			nQueryAddrTime = 0;	  
	
	#if	(EXTERNAL_FLASH)		
	static unsigned int			nPrevTime = 0;	
	unsigned char				nPubBuf256[256];
	unsigned char				*lpData = NULL;

	//lpData = (unsigned char*)&lpLargeBuf3[0];			
	lpData = (unsigned char*)&nPubBuf256[0];
	
	if( nPrevTime == 0 ){		
		
		nPrevTime = OSTimeGet();
	}	
	// ��ʱ���ä�������ϴ�	(20���Ӽ��һ��) fortest
	if( (OSTimeGet() - nPrevTime ) >= CHECK_GPSDATA_FLASH ){
		
		// ȡ��GPS����
		nLen =	PopGPSData( lpData, GSM_SENDFLASH_MAXDATA  );
		if( nLen == 0 ){
			nPrevTime = OSTimeGet();
		}
		else{
			nPackage = nLen / GPSDATASIZE;				
			nLen = PL_ComposeGPSBlindData( lpLargeBuf, lpData, nPackage );
			IP_SendData( IP_IsTCPCommMode(), lpLargeBuf, nLen );			
		}		
	}		
	#endif //EXTERNAL_FLASH
	
	#if( SW_PROTOCOLB )
	if( nQueryAddrTime == 0 ){		
		nQueryAddrTime = OSTimeGet();
	}	
	// ��ʱ��ѯ��ַ
	if( TE_IsQueryGGAddrFlag() ){
		
		// 10�����²�ѯһ�ε�ַ
		if( ( (OSTimeGet() - nQueryAddrTime) >= GSM_LOGIN_SPACE ) || 
			( nQueryAddCnt == 0 ) ){ 
			
			nQueryAddCnt++;	
			
			if( GSM_IsLangState() ){
				nLen = PL_ComposeGGGPSData( lpLargeBuf, CC_TE_QUERY_GG_ADDR_CN );
			}
			else{
				nLen = PL_ComposeGGGPSData( lpLargeBuf, CC_TE_QUERY_GG_ADDR_EN );
			}
			// ֪ͨ��������
			IP_SendData( IP_IsTCPCommMode(), lpLargeBuf, nLen );
			
			nQueryAddrTime = OSTimeGet();
		}
		//  ���ͳ�ʱ,�����������	
		if( nQueryAddCnt >= 3 ){
		
			nPackage = PLS_ComposeUploadData( lpLargeBuf, &nLen );			
			CMM_SendSMSData( g_stGSMState.m_strForward, lpLargeBuf, nPackage, nLen );
								
			nQueryAddCnt = 0;
			TE_ClrQueryGGAddrFlag();
		}		
	}	
	else{		
		nQueryAddrTime = OSTimeGet();
		nQueryAddCnt = 0;		
	}
	#endif //SW_PROTOCOLB
	
	return 0;
}
//////////////////////////////////////////////
// ����GPS����
// ������ʱ����(6)|��λ��־(1)|����(4)|γ��(4)|����(2)|�ٶ�(1)|Ӳ��״̬(4)|����״̬(4)|ACC����ʱ��(2)|������(1)
unsigned char	CMM_SaveGPSToFlash( void ){
	
	#if	(EXTERNAL_FLASH)
	unsigned char		nGPSData[GPSDATASIZE];
	
	if( GPS_IsPosValid() ){		
	
		memset( nGPSData, 0x00, GPSDATASIZE );	
		// 18�ֽ�
		PL_AddGPSData( &nGPSData[0] );
		// 11�ֽ�
		PL_AddDEState( &nGPSData[18] );
		// ��GPS��FLASH����
		PushGPSData( nGPSData, GPSDATASIZE );	
	}
	#endif //EXTERNAL_FLASH	
	return  0;
}
/// ///////////////////////////////////////////
// 
void CMM_ATTest(void){
	
	static unsigned int	nPrevTime = 0;
	if ( OSTimeGet()-nPrevTime >= GSM_AT_CHECK_TIMEOUT ){
		nPrevTime = OSTimeGet( );
		if( CMM_AT( ) == GSM_NG ){ 
			GSM_SetNeedReset();
		}	
	} 
	return; 
}
///////////////////////////////////////////////////////////
//���ű� �뷽ʽ
unsigned char GSM_GetDCSType( unsigned char nType ){	
	unsigned char	nResult = DCS_7BIT;
	if( (nType & 0xC0) == 0x00 ){
		
		switch( nType&0x0C ){
		case 0x00:
			nResult = DCS_7BIT;
			break;
		case 0x04:
			nResult = DCS_8BIT;
			break;
		case 0x08:
			nResult = DCS_UNICODE;
			break;	
		default:
			break;	
		}	
	}
	else if( (nType & 0xF0) == 0xF0 ){
		if( nType & 0x04 ){
			nResult = DCS_8BIT;		
		}
		else{
			nResult = DCS_7BIT;		
		}	
	}
	return nResult;
}
/*2012-03-29(wyb���ٴ���ռ�)
//////////////////////////////////////////////////////////////////////////
// ����: "1BE08602"
// ���: "abcd"
unsigned char	GSM_ParseTPUD( unsigned char *lpDest, unsigned char* lpSrc,  unsigned char nLen, unsigned char nDCSType){

	unsigned char	nCnt = 0;

	if( nDCSType == DCS_7BIT ){
		nLen = P_GSMToASCII( (char*)lpDest, (char*)lpSrc, nLen );
		*(lpDest+nLen) = 0x00;	
	}
	else if( nDCSType == DCS_8BIT ){
		memcpy( lpDest, lpSrc, nLen );
		*(lpDest+nLen) = 0x00;
	}
	else{
		nLen /= 2;
		for( nCnt = 0; nCnt < nLen; nCnt++ ){
			*(lpDest+nCnt) = *(lpSrc+nCnt*2);
		}
		*(lpDest+nCnt) = 0x00;
	}
	return nLen;
}
*/
//////////////////////////////////////////////////////////////////////////
// ����: 0x68,0x31,0x08,0x00,0x75,0x80,0xF5
// ���: "8613800057085"
unsigned char GSM_ParseTPOA(  unsigned char *lpDest, unsigned char* lpSrc, unsigned char nLen){

	unsigned char		nCnt = 0;
	unsigned char		nTmp;
	
	for( nCnt = 0; nCnt < nLen; nCnt++){
		nTmp = *(lpSrc+nCnt);
		*(lpDest+nCnt*2) = (nTmp&0x0F) +'0';
		
		nTmp >>= 4;
		nTmp &= 0x0F;
		if( nTmp == 0x0F ){
			*(lpDest+nCnt*2+1) = 0x00;
		}
		else{
			*(lpDest+nCnt*2+1) = nTmp + '0';
		}			
	}
	*(lpDest+nCnt*2) = 0x00;
	return 0;
}
//////////////////////////////////////////////////////////////////////////
// ����:0x08,0x91,0x68
//      from"0891683108705505F0040D91683135804115F9000080508241947523041BE08602"
//      
// ���:	
//       pBuf: 			TP_UD
//       pSMC: 			SMSC
//       pCallerNo: TP_OA
//			 pDCSType:  DCS type
// ����: ���ݳ���
unsigned char GSM_ParsePDUSMS( unsigned char	*pDest,
															 unsigned char	*pSMC,
															 unsigned char 	*pCallerNo,
															 unsigned char	*pTPUDL,
															 unsigned char 	*pDCSType,
															 unsigned char	*pSrc  ){

	unsigned short	nLen;
	unsigned char		nTmp = 0;
	
	
			
	// ===============================================
	// TP_SMSC
	// ===============================================
	nTmp = pSrc[0];
	// SMSC��ַ��ʽ(TON/NPI) 
	// �����ʽ�����ʾ��0x91 = 10010001 
	// Bit7 = 1
	// ��ֵ���ͣ�Type of Number����000��δ֪��001�����ʣ�010������,111��������չ�� 
	// �������Numbering plan identification��:0000��δ֪��  
	// 0001��ISDN/�绰����(E.164/E.163)
	// TON Value
	// Definition	 
	//	0	 Unknown		 
	//	1	 International Number		 
	//	2	 National Number		 
	//	3	 Network Specific Number		 
	//	4	 Subscriber Number		 
	//	6	 Abbreviated Number		    
	//	7	 Reserved
	if( strlen( (char*)pSMC ) <= 5 ){			
		if( (pSrc[1]&0x70) == 0x10 ){			
			pSMC[0] = '+';
			GSM_ParseTPOA( pSMC + 1, pSrc+2, nTmp-1);		
		}	  
		else{
			GSM_ParseTPOA( pSMC, pSrc+2,  nTmp-1);		
		}
	}	
	if( (pSMC[0] == '0') && (pSMC[1] == '0') ){
		pSMC[0] = '+';
		strcpy( (char*)&pSMC[1], (char*)&pSMC[2] );	
	}	
	pSrc += (nTmp+1);
 
	// ===============================================
	// TP_MTI
	// ===============================================
	// BIT No.  7  6  5  4  3  2  1  0  
	// Name  TP-RP  TP-UDHI  TP-SPR  TP-VFP  TP-RD  TP-MTI  
	// Value  0  0  0  1  0  0  0  1  
	pSrc++;

	// ===============================================
	// TP-OA
	// ===============================================	
	nTmp = *pSrc++;
	nLen = nTmp/2;
	if( nTmp%2 ){
		nLen++;
	}
	nTmp = *pSrc++;
	if( (nTmp & 0x70 ) == 0x10 ){
		pCallerNo[0] = '+';
		GSM_ParseTPOA(pCallerNo+1, pSrc, nLen );
	}
	else{
		GSM_ParseTPOA(pCallerNo, pSrc, nLen );
	}
	if( (pCallerNo[0] == '0') &&
			(pCallerNo[1] == '0') ){
		pCallerNo[0] = '+';
		strcpy( (char*)&pCallerNo[1],(char*)&pCallerNo[2] );	
	}
	pSrc += nLen;

	// ===============================================
	//TP_PID����
	// ===============================================
	pSrc++;
	
	// ===============================================
	// TP-DCS���뷽ʽ
	// ===============================================
	*pDCSType = *pSrc++;

	// ===============================================
	// TP-SCTS������
	// ===============================================
	pSrc += 7;

	// ===============================================
	// TP_DATA����
	// ===============================================
	nLen = *pSrc++;
	*pTPUDL = nLen;
	
	if( GSM_GetDCSType( *pDCSType) == DCS_7BIT ){
		nLen *= 7;
		if( nLen % 8 ){
			nLen /= 8;
			nLen++;			
		}
		else{
			nLen /= 8;
		}
	}
	memcpy( (char*)pDest, (char*)pSrc, nLen );		
	return (unsigned char)nLen;
}

//////////////////////////////////////////////////////////////////////////
// ����TPUDʵ�ʵĳ���.AT+CMGSʱ�õ�
// Ϊ�򵥣�����һ�ɲ���7λ����(160���ֽ�)
unsigned char	GSM_ComposeSend7GSMData( unsigned char	*lpDest,
										 unsigned char	*pResultLen,
										 unsigned char	*lpSMCNo, 
										 unsigned char	*lpRecvNo,
										 unsigned char	*lpMsg  ){
	
	unsigned char 	nResult = 0;
	unsigned char		nTPUDL = 0;
	unsigned char 	nLen = 0; 
	
	nLen = strlen( (char*)lpMsg );
	if( nLen > 160 ){
		nLen = 160;
	}		
	// TP_UDL
	nTPUDL = nLen;
	// TP_UD
	nLen  = P_Code8BitTo7bit( lpMsg, lpMsg, nLen );
	nResult = GSM_ComposeSendGSMData( lpDest, 
									  pResultLen,
									
									lpSMCNo,
									lpRecvNo,
									lpMsg,
									nLen,
									nTPUDL,
									0 );
	return nResult;		
}
//////////////////////////////////////////////////////////////////////////
// ����TPUDʵ�ʵĳ���.AT+CMGSʱ�õ�
// ���140
unsigned char	GSM_ComposeSend8GSMData( unsigned char	*lpDest,
										 unsigned char	*pResultLen,
										 
										 unsigned char	*lpSMCNo, 
										 unsigned char	*lpRecvNo,
										 unsigned char	*lpMsg, 
										 unsigned char 	nLen  ){
	
	unsigned char   nResult = 0;
	unsigned char	nTPUDL = 0;
	
	if( nLen > 140 ){
		nLen = 140;
	}		
	// TP_UDL
	nTPUDL = nLen;																				
	nResult = GSM_ComposeSendGSMData(  lpDest, 
									   pResultLen,
									   
									   lpSMCNo,
									   lpRecvNo,
									   lpMsg,// lpBuf,
									   nLen,
									   nTPUDL,
									   0x04 );
	return nResult;		
}

//////////////////////////////////////////////////////////////////////////
// ����TPUDʵ�ʵĳ���.AT+CMGSʱ�õ�
// ���140
unsigned char	GSM_ComposeSend16GSMData( unsigned char	*lpDest,
										 unsigned char	*pResultLen,
										 
										 unsigned char	*lpSMCNo, 
										 unsigned char	*lpRecvNo,
										 unsigned char	*lpMsg, 
										 unsigned char 	nLen  ){
	
	unsigned char   nResult = 0;
	unsigned char	nTPUDL = 0;
	
	if( nLen > 140 ){
		nLen = 140;
	}		
	// TP_UDL
	nTPUDL = nLen;																				
	nResult = GSM_ComposeSendGSMData(  lpDest, 
									   pResultLen,
									   
									   lpSMCNo,
									   lpRecvNo,
									   lpMsg,// lpBuf,
									   nLen,
									   nTPUDL,
									   0x08 );
	return nResult;		
}
//////////////////////////////////////////////////////////////////////////
// ����: TPUDʵ�ʵĳ���.AT+CMGSʱ�õ�
// input		: nDCS :TP-DCS
// lpTPUDBuf: TP_UDL + TP_UD
// nLen			: length of lpBuf
unsigned char	GSM_ComposeSendGSMData( unsigned char *lpDest,
										unsigned char *pResultLen,
										
							 			unsigned char *lpSMCNo, 
							 			unsigned char *lpRecvNo, 
							 			unsigned char *lpTPUDBuf,
							 			unsigned char nLen, 
							 			unsigned char nTPUDL,
										unsigned char nDCS  ){

	static unsigned char	s_nMRCnt = 0;
	unsigned char			nTmp = 0;
	unsigned char			nCnt = 0;
	unsigned char			nBack = 0;
	unsigned char			nResult = 0;
	unsigned char			*pData = NULL;
	
	
	pData = lpLargeBuf2;
	memset( pData, 0x00, nLen + 1);
	memcpy( pData, lpTPUDBuf, nLen );
	
	// SMC_NO
	if( *lpSMCNo == '+' ){
		lpSMCNo++;
	}
	
	nTmp = strlen((char*)lpSMCNo);       
	if(nTmp%2){
		nCnt = nTmp/2 + 1;
	}
	else{
		nCnt = nTmp/2;
	}
	nCnt++; 
	
	// �������뷽ʽ����
	nResult = 0;
	*(lpDest+nResult++) = nCnt;
	*(lpDest+nResult++) = (unsigned char)0x91;
	nTmp = strlen( (char*)lpSMCNo );
	nResult += P_StrToBCD( &lpDest[nResult], lpSMCNo, nTmp );	
	
	nBack = nResult;
	// TP_MTI
	*(lpDest+nResult++) = 0x11;
	// TP_MR
	*(lpDest+nResult++) = s_nMRCnt++;
	
	// TP_DA	
	if( *lpRecvNo == '+'){
	
		lpRecvNo++;
		nTmp = strlen((char*)lpRecvNo);
		*(lpDest+nResult++) = nTmp;	
		*(lpDest+nResult++) = (unsigned char)0x91;
	}
	else if( (*lpRecvNo == '0') 	&& 
			     (*(lpRecvNo+1) == '0') ){
		lpRecvNo += 2;
		nTmp = strlen((char*)lpRecvNo);
		*(lpDest+nResult++) = nTmp;	
		*(lpDest+nResult++) = (unsigned char)0x91;
			 	
	}
	else{
		nTmp = strlen((char*)lpRecvNo);
		*(lpDest+nResult++) = nTmp;	
		*(lpDest+nResult++) = (unsigned char)0x81;
	}
	
	nResult += P_StrToBCD( &lpDest[nResult], lpRecvNo, nTmp );
	// TP_PID = 0x00
	*(lpDest+nResult++) = 0x00;	
	// TP_DCS = 0x00	
	*(lpDest+nResult++) = nDCS;	
	// TP_VP	
	*(lpDest+nResult++) = 0x00;	
	// TP_UDL
	*(lpDest+nResult++) = nTPUDL;
	// TP_UD
	memcpy( &lpDest[nResult], pData, nLen );	
	nResult += nLen;	
	*pResultLen = nResult;
	pData = NULL;
	return nResult-nBack;
}

//////////////////////////////////////////////////////////////////////////
// ����:"089124"
// ���:0x08,0x91,0x24 ������3
unsigned short P_StrToHex( unsigned char* lpDestBuf, unsigned char* lpSrcBuf, unsigned short nLen )
{
	
	unsigned short	nResult = 0;
	unsigned short	nCnt = 0;	
	char			*pch = NULL;	
	unsigned char	nBuf[3];
		
	memset( nBuf, 0, 3);
	for( nCnt = 0; nCnt < nLen; ){
		memcpy( (char*)nBuf, (char*)(lpSrcBuf+nCnt), 2 );
		*(lpDestBuf+nResult ) = (unsigned char)strtol( (char*)nBuf, &pch, 16);   
		nResult++;
		nCnt+=2;
	}
	return nResult;
}
//////////////////////////////////////////////////////////////////////////
// ����:  "1358008"
// ���:	0x31, 0x85, 0x00, 0xF8
unsigned char P_StrToBCD( unsigned char* lpDestBuf, unsigned char* lpSrcBuf, unsigned char nLen )
{
	
	unsigned char	nResult = 0;
	unsigned char	nCnt = 0;
	unsigned char	nTmp;
	
	
	for( nCnt = 0; nCnt < nLen; nCnt++ ){

		nTmp = *(lpSrcBuf+nCnt);
		if( nTmp >= '0' && nTmp <='9' ){
			nTmp -= '0';

			if( (nCnt%2) == 0 ){
				*(lpDestBuf+nResult) = 0xF0|nTmp;
			}
			else{
				nTmp <<= 4;
				*(lpDestBuf+nResult) &= 0x0F;
				*(lpDestBuf+nResult) |= nTmp;
				nResult++;
			}
		}
	}
	if( nLen%2 ){
		nResult++;
	}
	return nResult;
}
///////////////////////////////////////////////////////////////////////////
// 7-bit����
// pSrc:		Դ�ַ���ָ��
// pDst:		Ŀ����봮ָ��
// nSrcLength:  Դ�ַ�������
// ����:		Ŀ����봮����
unsigned char P_Code8BitTo7bit(unsigned char* pDest, unsigned char* pSrc, unsigned char nSrcLength)
{
    unsigned char nSrc;				// Դ�ַ����ļ���ֵ
    unsigned char nDst;				// Ŀ����봮�ļ���ֵ
    unsigned char nChar;			// ��ǰ���ڴ���������ַ��ֽڵ���ţ���Χ��0-7
    unsigned char nLeft;    		// ��һ�ֽڲ��������
    
    unsigned char nData = 0;
    
    nSrc = 0;
    nDst = 0;
    
    while(nSrc<=nSrcLength){
		
		nData = *pSrc;
		
		switch( nData ){
		case '@':
			nData = 0x00;
			break;
		case '$':
			nData = 0x02;
			break;
		default:
			break;
		}		
		
        nChar = nSrc & 7;    
        if(nChar == 0){
            nLeft = nData;
        }
        else{			
            *pDest++ = (nData << (8-nChar)) | nLeft;    
            nLeft = nData >> nChar;
            nDst++; 
        }         
        pSrc++; 
		nSrc++;
    }    
    return nDst; 
}     
    
///////////////////////////////////////////////////////////////////////////
// 7-bit����
// pSrc:		Դ���봮ָ��
// pDest:		Ŀ���ַ���ָ��
// nSrcLength:  Դ���봮����
// ����:		Ŀ���ַ�������
unsigned char P_Decode7bitTo8Bit(unsigned char* pDest, unsigned char* pSrc, unsigned char nSrcLength)
{
    unsigned char nSrc = 0;			// Դ�ַ����ļ���ֵ
    unsigned char nDst = 0;			// Ŀ����봮�ļ���ֵ
    unsigned char nByte= 0;			// ��ǰ���ڴ���������ֽڵ���ţ���Χ��0-6
    unsigned char nLeft;    		// ��һ�ֽڲ��������
    
    nSrc = 0;
    nDst = 0;
    
    nByte = 0;
    nLeft = 0;
    
    while(nSrc<nSrcLength){		
	    *pDest = ((*pSrc << nByte) | nLeft) & 0x7f;
	    nLeft = *pSrc >> (7-nByte);
	    pDest++;
	    nDst++;    
	    nByte++;    
	    if(nByte == 7){
        *pDest = nLeft;    
        pDest++;
        nDst++;    
        nByte = 0;
        nLeft = 0;
	    }    
	    pSrc++;
	    nSrc++;
    }    
    *pDest = 0;    
    return nDst;
}
//////////////////////////////////////////////////////////////////////////
//
//unsigned char P_ASCIIToGSM( char *lpSrcBuf, char* lpDestBuf, unsigned char nLen ){
//}
//////////////////////////////////////////////////////////////////////////
// GSM����ASCII��
unsigned char P_GSMToASCII(  char* lpDestBuf, char *lpSrcBuf, unsigned char nLen )
{
	
	int				nCnt    = 0;
	int				nResult = 0;
	unsigned char	nChar   = 0;

	for( nCnt = 0; nCnt < nLen; nCnt++ ){
		
		nChar = (unsigned char)*(lpSrcBuf+nCnt);
		if( nChar == 0x1b){
			nCnt++;
			nChar = (unsigned char)*(lpSrcBuf+nCnt);
			switch( nChar ){
			case 0x14:
				nChar = '^';
				break;
			case 0x28:
				nChar = '{';
				break;
			case 0x29:
				nChar = '}';
				break;
			case 0x2F:
				nChar = '\\';
				break;
			case 0x3C:
				nChar = '[';
				break;
			case 0x3D:
				nChar = '~';
				break;
			case 0x3E:
				nChar = ']';
				break;
			case 0x40:
				nChar = '|';
				break;
			default:
				nChar = ' ';
				break;
			}
		}
		else{
			// ת���������õ�
			if( nChar < 0x20 ){
				switch( nChar ){
				case 0:
					nChar ='@';
					break;
				case 2:
					nChar ='$';
					break;
				case 0x0D:
				case 0x0A:	
					break;
				default:
					nChar = ' ';
					break;
				}
			}	
			else{
				// ��O�ֲ�֧��
				if( nChar ){
					switch( nChar){
					case 0x24:
					case 0x40:
					case 0x60:	
					case 0x5B:		
					case 0x5C:	
					case 0x5D:	
					case 0x5E:	
					case 0x5F:	
					case 0x7B:		
					case 0x7C:	
					case 0x7D:	
					case 0x7E:	
					case 0x7F:
						nChar = ' ';
					default:
						break;
					}
				}
			}
		}
		*(lpDestBuf+nResult) = nChar; 
		nResult++;
	}
	*(lpDestBuf+nResult) = 0x00;
	return nResult;
}
//////////////////////////////////////////////////////////////////////////
//
void P_UnicodeToASCII( unsigned char	*pDest, unsigned char	*pSrc , unsigned char nLen ){

	unsigned char	nResult = 0;
	unsigned char	nCnt = 0;
	
	for ( nCnt=0; nCnt <= nLen; nCnt += 2){
	
		*(pDest + nResult) = pSrc[nCnt+1];		
		nResult++; 
	}
	pDest[nResult] = 0x00;
}





