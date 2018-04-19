/////////////////////////////////////////////////////////////////////////
// file name: main.c
// remark: opti.. by shinbo on 2008-04-02
// �汾��:V2.07 ������GPRSģʽ,��½�ĳ��ȸĳ���0x09
// �汾��:V2.44-20091229 ��������������,ת��SMSģʽ,�ر�GPRS����

// GTM900 GPRS�Ĺ���״̬ 2010-03-17
// 1: APNû��������ȷ,��Ʋ������
// 2: APN������ȷ, TCPģʽAT%IPOPENʱ��ƿ��, UDPģʽAT%IPOPEN��Ʋ�����,ֻ�з�����ʱ,��ƿ���
// �汾:
// V2.67-100529  �����������Ȩ����
// V2.69-100626  �����SN��������
// V2.70-100715  �����SPI SEEL0 �ų�ʼ��Ϊ�ߵ�ƽ
// V2.73-100907  ������101B ��Ϊģ������������������
// V2.74-101018  �޸���DEUID����+,?���ַ�
// V2.78-110201  �޸Ľ�ԭ�������������,������������
// V2.79-110305  1:�޸Ľ�ԭ�������������,������������
//               2:ԭ������ʱ���α���
//               3:��ԭ������Ϊ������

//V2.80-110326	 1:�޸���CDMA,UART�ڵĵ���.
//				 			 2:�޸���CDMA������������
//V2.81-110413   1:����cut��on��Ϊ���Ͷϵ�ļ�Ҫָ��
//V2.82-110531   �����˰�����ش�,�ٶ�Ҫ����ʮ����ż���
//V2.83-110610	 ������,�ر�SOS��������
//V2.84-110620   ������MC323 ģ�鹦��
//V2.85-110730	 �޸��˻ش��������32768,���ֳ�����λ
//V2.86-110815   �޸���CDMAģ��,AT%HSMSSS �رն�����ʾ����״̬
//V2.87-110901   �����˴����ż챨����,����·
//V2.88-110902   �޸��˺궨��ķ�ʽ ��#ifdef/#ifndef �ĳ���#if
//V2.90-111201   �����˻�Ϊ MG323 ��Ƶģ��
//V2.91-111203   ET601Ӳ��������,���ٸ�Ӧ��
//V2.92-120211   �޸���CDMAģ��,������������Ը�SN��
//V2.93-120217   �޸���CMDA MC323 ���ض�����ʾ���� HSMSSS
//V2.94-120229
/**Э���ļ�˵��************************************************
***************************************************************/ 
// SA = J  Java
// SA = 8  ASICO
// SA = 2  JENNY
// SB = 2  JENNY
// SA = 3  GBK_CHRIS
// SB = 3  GBK_CHRIS
// SB = 4  JORGE_AUTOFUEL

////////////////////////////////////////////////////////////////////////
// include file declare
#include "main.h"
#include "..\hdr\includes.h"
#include "..\hdr\target.h"
#include "..\hdr\HW_Battery.h"
#include "..\hdr\HW_GPIO.h"
#include "..\hdr\HW_Flash.h"
#include "..\hdr\APP_Terminal.h"
#include "..\hdr\APP_GPS.h"
#include "..\hdr\APP_GSM.h"
#include "..\hdr\APP_LoopDetect.h"        
#include "..\hdr\HW_Storage.h"
#include "..\hdr\HW_SPI.h"


//extern  OS_EVENT 			*pGSMReceiveSem;
//extern  OS_EVENT 			*pGPSReceiveSem;

/////////////////////////////////////////////////////////////////////////
// function declare 
// ��ʼ��FLASH
//void InitFlash(void);

/////////////////////////////////////////////////////////////////////////
// variable declare  
// �汾��
char 		g_strVer[32]=""; 

	
/////////////////////////////////////////////////////////////////////////
//

int main(void){	

	SW_WatchdogEnable( );	
		
	SW_Watchdog( );		
	
	OSInit();	

	SW_Watchdog( );
	
//	InitFlash();
	
//	SW_Watchdog( );	
	
//	InitHardware();	

	MainBatterInit();	
	#if ( HW_ET601 )
	MMA_ADCInit();
	#endif 
	
			
	HW_LoopDetectTask_Creat( );     
	GPS_Task_Creat( );   
	GSM_Task_Creat( );	
		
	OSStart();
	OSTaskDel(OS_PRIO_SELF);
	return 0;
}
///////////////////////////////////////////////////////////////////////////////////
//
void GetVersion( void ){
	
	g_strVer[0] = 0x00;
	
	#if (HW_ET101A)	
		#if (HW_COLDRESTART_GSM)
		strcpy(g_strVer , "101A-HR"); 
		#else
		strcpy(g_strVer , "101-HA");
		#endif
	#endif 
	
	#if  (HW_ET101B)
		#if  (HW_COLDRESTART_GSM)
		strcpy(g_strVer , "101-HR");   		
		#else
		strcpy(g_strVer , "101-HB");   		
		#endif
	#endif	
	
	#if  (HW_ET102B)
		#if  (EXTERNAL_FLASH)
			#if  (HW_COLDRESTART_GSM)
				strcpy(g_strVer , "102X-HR");   		 
			#else
				strcpy(g_strVer , "102X-HB"); 
			#endif
		#else
			#if  (HW_COLDRESTART_GSM)
				strcpy(g_strVer , "102-HR"); 
			#else
				strcpy(g_strVer , "102-HB");   		 
			#endif
		#endif //EXTERNAL_FLASH
	#endif
			 
	#if (HW_ETC101B )
		#if (HW_EM200	)
		strcpy(g_strVer, "C101-HC");
		#else
		strcpy(g_strVer, "101-HC");	
		#endif
	#endif //HW_ETC101B
	
	#if (HW_ET201B)
		#if (HW_EM200 )
			#if (HW_COLDRESTART_GSM)
				strcpy(g_strVer , "C201-HR"); 
			#else
				strcpy(g_strVer , "C201-H1"); 
			#endif
		#else
			#if (HW_COLDRESTART_GSM)
				strcpy(g_strVer , "201-HR");   
			#else
				strcpy(g_strVer , "201-H1");   
			#endif
		#endif
	#endif
	
	#if (HW_ET201B_2)
		#if (HW_COLDRESTART_GSM)
			strcpy(g_strVer , "201B-H2R"); 
		#else
			strcpy(g_strVer , "201B-H2");
		#endif
	#endif
	
	#if (HW_ET201B_3)
		#if (EXTERNAL_FLASH)
			#if (HW_COLDRESTART_GSM)
				strcpy(g_strVer , "201X-HR");
			#else
				strcpy(g_strVer , "201X-H3"); 
			#endif
		#else
			#if (HW_COLDRESTART_GSM)
				strcpy(g_strVer , "201B-HR"); 
			#else			
				strcpy(g_strVer , "201B-H3");
			#endif
		#endif //EXTERNAL_FLASH
	#endif
	
	#if (HW_ET007B)
		strcpy(g_strVer, "007-H1");
	#endif
	
	#if (HW_ET007B_2)
		#if (HW_COLDRESTART_GSM)
		strcpy(g_strVer , "007-HR"); 
		#else
		strcpy(g_strVer , "007-H2");
		#endif
	#endif 
	
	#if (HW_ET601)
		#if (HW_COLDRESTART_GSM)
		strcpy(g_strVer , "601-HR");
		#else
		strcpy(g_strVer , "601-HB");
		#endif
	#endif 
		
	#if  (HW_GTM900	)
		#if (HUAWEI_900AB)
		strcat(g_strVer , "-6"); 	
		#else
		strcat(g_strVer , "-1"); 	
		#endif
	#endif
	
	#if  (HW_SIMCOM)
	 #if (SIMCOM_340)
		strcat(g_strVer , "-2");  
	 #else
	 	#if( SIMCOM_840W )	 	
	 		strcat(g_strVer , "-9"); 
	 	#else
		 	strcat(g_strVer , "-5"); 
		#endif
	 #endif	
	 #if  ( SIMCOM_CBAND_9001800)
		strcat(g_strVer , "B1"); 	
	 #endif
	
	 #if  (SIMCOM_CBAND_8501900)
	 	strcat(g_strVer , "B2"); 	
	 #endif	
	#endif	
	
	#if  (HW_BQM26)
	strcat(g_strVer , "-3"); 	
	#endif 
	
	#if (HW_EM200)
	strcat(g_strVer , "-4"); 	
	#endif 	
	
	#if  (HW_MC323)
	strcat(g_strVer , "-7"); 
	#endif	
	
	#if  (HW_MG323)
	strcat(g_strVer , "-8"); 
	#endif	
		
	#if	(SW_PROTOCOLA)
		#if (TOM)
			strcat(g_strVer , "-SAT"); 	    
		#else
			#if (JENNY)
				strcat(g_strVer , "-SA2"); 
			#else
				#if (GBK_CHRIS)
					strcat(g_strVer , "-SA3"); 
				#else
					strcat(g_strVer , "-SA");	
				#endif
			#endif // JENNY
		#endif	// TOM
	#endif	// SW_PROTOCOLA
	
	#if	(SW_PROTOCOLB)
		#if (JENNY )
			strcat(g_strVer , "-SB2");	
		#else
			#if (GBK_CHRIS)
				strcat(g_strVer , "-SB3"); 
			#else
				#if (TOM )
					strcat(g_strVer , "-SBT");	 
				#else
					#if (JORGE_AUTOFUEL)
						strcat(g_strVer , "-SB4");	    
					#else
						strcat(g_strVer , "-SB");	
					#endif
				#endif
			#endif
		#endif   
	#endif
	
	#if (SW_PROTOCOLC)
	strcat(g_strVer , "-SAJ");   
	#endif	  
	
	#if	(SW_PROTOCOL_ASICO)
	strcat(g_strVer , "-SB8"); 	  
	#endif
		
	#if (DIAL_CALL)
	strcat( g_strVer,"-C");
	#endif
	
	#if (EXTERNAL_FLASH) 
		#if (ATMEL_FLASH)
			strcat( g_strVer,"-A");
		#else
			strcat( g_strVer,"-W"); 
		#endif
	#endif	
		
	strcat(g_strVer , "-V2.95-130808");	      
} 
///////////////////////////////////////////////////////////////////////////////////
//  �Ƚ�FLASH��ʼ���Ƿ�ɹ� 
unsigned char    IsFlashNormal( unsigned char	 *pSrc, unsigned char	nLen ){
	
	unsigned char	nResult = 0;
	unsigned char	nCnt = 0;
	
	if( nLen > FLASH_MAX_NORMAL ){
		
		nLen = FLASH_MAX_NORMAL;
	}
	for( nCnt = 0; nCnt < nLen; nCnt++ ){
		
		if( pSrc[nCnt] != 0xFF ){
			
			nResult = 1;
			break;
		}	
	}
	return nResult;
}
///////////////////////////////////////////////////////////////////////////////////
// ��ʼ��FLASH
void InitFlash(void){
	
	unsigned char		 nLen = 0;
	unsigned char		 strVer[32];	
	#if	(EXTERNAL_FLASH)
	STGPSINDEX			 stGPSIndex;
	#endif //EXTERNAL_FLASH
	
		
	GetVersion();	
	nLen = strlen( (char *)g_strVer );
	memset( strVer, 0x00, 32 );	
	#if	(EXTERNAL_FLASH)
		ReadDataFromFlash( strVer, FLASH_VER_ADDR_START, nLen );
	#else
		memcpy( strVer, (char *)FLASH_VER_ADDR_START, nLen );
	#endif//EXTERNAL_FLASH		

	if( memcmp( g_strVer, strVer, nLen ) == 0 ){
	
		// ��ȡ������
		#if	(EXTERNAL_FLASH)		
			memset( &stGPSIndex, 0x00, sizeof(STGPSINDEX) );		
			ReadGPSIndex( &stGPSIndex );
			if( IsFlashNormal( (unsigned char*)&stGPSIndex, sizeof(STGPSINDEX) ) == 0 ){
								
				InitGPSStorageIndex();
			}
		#endif	//EXTERNAL_FLASH
		
		GSM_ReadSetupData( );
		ReadAlarmSetupData( );
	}
	else{
	
		#if	(EXTERNAL_FLASH)
			InitGPSStorageIndex();
		#endif	//EXTERNAL_FLASH
		
		GSM_WriteSetupData( );	
		WriteAlarmSetupData( );
		
		#if	(EXTERNAL_FLASH)
			WriteDataToFlash2( FLASH_VER_ADDR_START, (unsigned char*)g_strVer, nLen );
		#else
			WriteDataToFlash( (char *)FLASH_VER_ADDR_START, (char *)g_strVer, nLen );
		#endif //EXTERNAL_FLASH
	}			
			
	/*									
    GetVersion();															// �������Ź�
	nLen = strlen( (char *)g_strVer );
	// read data from flash
	if( memcmp( g_strVer, (char *)FLASH_VER_ADDR_START, nLen ) == 0 ){		
		GSM_ReadSetupData( );
		ReadAlarmSetupData( );		
	}
	// write data from flash
	else{		
		#if	(EXTERNAL_FLASH)
		InitGPSStorageIndex();
		#endif	
		InitializtFence();
		GSM_WriteSetupData( );
		WriteAlarmSetupData( );
		WriteDataToFlash( (char *)FLASH_VER_ADDR_START, (char *)g_strVer, nLen );
	}
	*/
	return;
}