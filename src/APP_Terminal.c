///////////////////////////////////////////////////////////////////////////
//   file name: app_Terminal.h
// description: Э��ͷ�ļ��ӿ�
//      author:
//        date: 2009-02-15
//     reamark: 
#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>
#include  <ctype.h>
#include "..\hdr\main.h"
#include "..\hdr\APP_GSM.h"
#include "..\hdr\HW_flash.h"
#include "..\hdr\APP_Terminal.h"
#include "..\hdr\HW_SPI.h"

#if (JORGE_AUTOFUEL) 
	#define		C_Para					0x01
	#define		C_Defence	  			1			   //ACC��ʱ����Ϊ��� ��λ����
	#define		C_ILLDoorOpen	  		5			   //�Ƿ��������� ��λ����	
	#define		C_CustomPos 			0x10		   //ԭ��������  	
	#define		C_GeoState				0x04		   //ԭ����������
#else
	#define		C_Para					0
	#define		C_Defence	  			3			   //ACC��ʱ����Ϊ��� ��λ����
	#define		C_ILLDoorOpen	  		100			   //�Ƿ��������� ��λ����	
	#define		C_CustomPos				0			   //	
	#define		C_GeoState				0			   //ԭ����������
#endif


#define		C_TireDriver   				240			   //ƣ�ͼ�ʻ����

#if ( HW_ET201B || HW_ET201B_2 || HW_ET201B_3 )
	#if ( JORGE_AUTOFUEL )
		#define		C_AlarmEnable			  	0xB441	 //�������� �ż쿪��
	#else
		#define		C_AlarmEnable				0xB041 	 //��������			
	#endif 
	
	#define   	C_HWValid						0xF077F  //�ն�֧��Ӳ������
	#define		C_ALValid						0xDBFFF  //�ն�֧�ֱ�������
#else
	#if (HW_ET601 )  //ײ����������
		#if (JENNY || FRANK) 
			#define		C_AlarmEnable			0x200001 //��������
		#else				
			#define		C_AlarmEnable			0x200041 //��������
		#endif
		#define   	C_HWValid					0xF077F  //�ն�֧��Ӳ������
		#define		C_ALValid					0x2D3FFF //�ն�֧�ֱ�������
	#else
		#if (JENNY || FRANK)
			#define		C_AlarmEnable			0x01	 //��������
		#else				
			#define		C_AlarmEnable			0x41	 //��������
		#endif
		#define   	C_HWValid					0xF077F  //�ն�֧��Ӳ������
		#define		C_ALValid					0xD3FFF  //�ն�֧�ֱ�������
	#endif
	
#endif


////////////////////////////////////////////////////////////////////////////
//	��������
STALARMSETUP	g_stAlarmSetup = {	
							  C_Para,				 		//��չ������������             	                
							  C_Defence,				//ACC��ʱ����Ϊ��� ��λ����
							  C_ILLDoorOpen,		//�Ƿ��������� ��λ����								  
							  C_TireDriver,		  //ƣ�ͼ�ʻ����
							  0,			     		  //���ٱ��� 	   ��λ����		
							  0,			   	 	    //ͣ������ 	   ��λ����
							  
							  C_AlarmEnable, 	  //��������
							  C_HWValid,			  //�ն�֧��Ӳ������ 
							  C_ALValid,			  //�ն�֧�ֱ�������
							 				
							  0,				 //��¼��������ʱ��
							  0,				 //��¼����ͣ��ʱ��
							  
							  0,				 //ACC�ؿ���ʱ�� ��λ����	
							  0,				 //GPSδ��λʱ�� ��λ����
							  
							  0,				 //�����
							  0,				 //��ǰ���
							  0,				 //������
							  
							  0								  
							  };								  

STCUSTOMSETUP    g_stCustomSetup = {
									// bit0:  ���õ�ƽ�Զ�����1
									// bit1:  ���õ�ƽ�Զ�����2
									// bit2:  ���õ�ƽ�Զ�����3
									// bit3:  ���õ�ƽ�Զ�����4
									// bit4:  ���÷�����ƽ״̬ 0:�ż�  1:ԭ������
									// bit5:  ����ACC����ģʽ�²��ϴ�GPRS���� 0: �ϴ� 1:���ϴ�
									//ET201:  �ż�Ϊ�ߵ�ƽ����		
									C_CustomPos,	
									C_GeoState,
									"Custom1",		 //�Զ�����1
									"Custom2",		 //�Զ�����2
									"Custom3",		 //�Զ�����3
									"Custom4",		 //�Զ�����4 
									0,	 		 	 //�����뱨��
									0,		 	 	 //�����뱨��	
									0,
									0,
									0,
									0
								   };		
				   
//  �ն��豸״̬							  
STDEVICESTATE	g_stDeviceState = {
								   0,				//GPRS�Ƿ��½�ɹ�
	       	 				 	   0,				//ִ��״̬							
								   16,				//Ӳ��״̬ Bit4 Ϊ1 ��·����
								   0,				//����״̬
								   0				//��ص�ѹ
								   };								   


/////////////////////////////////////////////////////////////////////
// дALARM���ݽṹ
void WriteAlarmSetupData(void)	{		
	
	#if (EXTERNAL_FLASH )
		WriteDataToFlash2( FLASH_ALARMSETUP_ADDR_START, (unsigned char*)&g_stAlarmSetup, sizeof(STALARMSETUP));
		WriteDataToFlash2( FLASH_CUSTOMSETUP_ADDR_START, (unsigned char*)&g_stCustomSetup, sizeof(STCUSTOMSETUP));
	#else
		WriteDataToFlash((char*)FLASH_ALARMSETUP_ADDR_START, (char*)&g_stAlarmSetup, sizeof(STALARMSETUP));
		WriteDataToFlash((char*)FLASH_CUSTOMSETUP_ADDR_START, (char*)&g_stCustomSetup, sizeof(STCUSTOMSETUP));
	#endif
	
	return ;
}
/////////////////////////////////////////////////////////////////////
// ��ȡALARM���ݽ�
void ReadAlarmSetupData(void)	{
	
	#if (EXTERNAL_FLASH )
		ReadDataFromFlash( (unsigned char*)&g_stAlarmSetup, FLASH_ALARMSETUP_ADDR_START, sizeof(STALARMSETUP) );
		ReadDataFromFlash( (unsigned char*)&g_stCustomSetup, FLASH_CUSTOMSETUP_ADDR_START, sizeof(STCUSTOMSETUP) );
	#else	
		memcpy((char*)&g_stAlarmSetup, (char*)FLASH_ALARMSETUP_ADDR_START, sizeof(STALARMSETUP));
		memcpy((char*)&g_stCustomSetup, (char*)FLASH_CUSTOMSETUP_ADDR_START, sizeof(STCUSTOMSETUP));
	#endif
	
	return ;
}

/////////////////////////////////////////////////////////////////////
//	��ʼ������Χ��
void  	InitializtFence( void ){ 

	unsigned char	nCnt = 0;
	
	for( nCnt=0; nCnt < 2; nCnt++ ){
		g_stAlarmSetup.m_oStGFence[nCnt].m_nFlag = 0;
		g_stAlarmSetup.m_oStGFence[nCnt].m_RLat = 0;
		g_stAlarmSetup.m_oStGFence[nCnt].m_RLng = 0;
		g_stAlarmSetup.m_oStGFence[nCnt].m_LLat = 0;
		g_stAlarmSetup.m_oStGFence[nCnt].m_LLng = 0;
	}	
}
/////////////////////////////////////////////////////////////
// ��ʼ��FLASH
void		  InitializeGSMFlash( void ){
	
	g_stGSMSetup.m_nCommMode = 0;		//���ŷ�ʽ
	g_stGSMSetup.m_nCBMode   = CB_STOP;
	g_stGSMSetup.m_nCBSpace  = 30;
	strcpy( (char*)g_stGSMSetup.m_strPassword,  DEFAULT_PSD );
	strcpy( (char*)g_stGSMSetup.m_strMaterNo, 	DEFAULT_PHONE_EMPTY);
	strcpy( (char*)g_stGSMSetup.m_strAuthNo1, 	DEFAULT_PHONE_EMPTY);
	strcpy( (char*)g_stGSMSetup.m_strAuthNo2, 	DEFAULT_PHONE_EMPTY);
	strcpy( (char*)g_stGSMSetup.m_strAuthNo3, 	DEFAULT_PHONE_EMPTY);
	strcpy( (char*)g_stGSMSetup.m_strAuthNo4, 	DEFAULT_PHONE_EMPTY);
	strcpy( (char*)g_stGSMSetup.m_strAuthNo5, 	DEFAULT_PHONE_EMPTY);
	strcpy( (char*)g_stGSMSetup.m_strSMSAddrNo,	DEFAULT_PHONE_EMPTY);
	strcpy( (char*)g_stGSMState.m_strFWDNo,		DEFAULT_PHONE_EMPTY);		
}
/////////////////////////////////////////////////////////////
// ��ʼ��ALARM FLASH
void		  InitializeAlarmFlash( void ){  
	
	g_stAlarmSetup.m_nDefence      		= C_Defence;			  //ACC��ʱ����Ϊ��� ��λ����
	g_stAlarmSetup.m_nAlarmEnable    	= C_AlarmEnable;		//��������		
	g_stAlarmSetup.m_nILLDoorOpen		  = C_ILLDoorOpen;		//�Ƿ��������� ��λ����	
	g_stAlarmSetup.m_nTireDriver		  = C_TireDriver;			//ƣ�ͼ�ʻ����
	g_stAlarmSetup.m_nOverspeed 		  = 0;						    //���ٱ��� ��λ����		
	g_stAlarmSetup.m_nParking			    = 0;						    //ͣ������ 		��λ����
	// ��ʼ������Χ��
	InitializtFence();
	// ��ʼ���Զ�����	
	memset(&g_stCustomSetup, 0x00, sizeof(STCUSTOMSETUP) );
	g_stCustomSetup.m_nCustomPos = 0x00;	
	strcpy( (char*)g_stCustomSetup.m_nCustom1, "Custom1");
	strcpy( (char*)g_stCustomSetup.m_nCustom2, "Custom2");
	strcpy( (char*)g_stCustomSetup.m_nCustom3, "Custom3");
	strcpy( (char*)g_stCustomSetup.m_nCustom4, "Custom4");		
}
/////////////////////////////////////////////////////////////
// �ַ�������Ƚ�
// ����: �ȽϺ�λ��ͬ��OK
unsigned char strcmp_back(unsigned char *p1, unsigned char *p2, unsigned char nLen ){
	
	unsigned char			nLen1,nLen2;
	unsigned char			nCnt;
	
	nLen1 = strlen( (char*)p1 );
	nLen2 = strlen( (char*)p2 );
	
	if( nLen1 < nLen || nLen2 < nLen ){
		return 0;
	}
	nLen1--;
	nLen2--;
	
	for( nCnt = 0; nCnt < nLen; nCnt++ ){
		if( *(p1+nLen1) != *(p2+nLen2) ){
			break;
		}
		nLen1--;
		nLen2--;		
	}
	if( nCnt < nLen ){
		return 0;
	}
	else{
		return 1;
	}
}
/////////////////////////////////////////////////////////////
// �ַ����Ƚ�(���ִ�Сд)
//
unsigned char strcmpnc(unsigned char *p1, unsigned char *p2 ){

	unsigned char			nResult = 1;

	if( strlen((char*)p1) != strlen((char*)p2) ){		
		nResult = 0;		
	}
	else{
		while( *p1 ){
			if( tolower(*p1) != tolower(*p2) ){
				nResult = 0;
				break;
			}
			p1++;
			p2++;			
		}		
	}
	return nResult;
}
/////////////////////////////////////////////////////////////
// �Ƿ���Ȩ����
unsigned char   IsAuthorizationNo(unsigned char	*pPhone ){
	
	return( strcmp_back(g_stGSMSetup.m_strMaterNo, pPhone, PHONE_COMPARE_BIT) ||
			strcmp_back(g_stGSMSetup.m_strAuthNo1, pPhone, PHONE_COMPARE_BIT) ||
	    	strcmp_back(g_stGSMSetup.m_strAuthNo2, pPhone, PHONE_COMPARE_BIT) || 
	    	strcmp_back(g_stGSMSetup.m_strAuthNo3, pPhone, PHONE_COMPARE_BIT) ||
	    	strcmp_back(g_stGSMSetup.m_strAuthNo4, pPhone, PHONE_COMPARE_BIT) ||
	    	strcmp_back(g_stGSMSetup.m_strAuthNo5, pPhone, PHONE_COMPARE_BIT) );
}
/////////////////////////////////////////////////////////////
// �Ƿ��������
unsigned char  IsListenNo( unsigned char	*pPhone ){

	return strcmp_back(g_stGSMSetup.m_strAuthNo4, pPhone, PHONE_COMPARE_BIT);
}

/////////////////////////////////////////////////////////////
// �Ƿ���Ȩ����
unsigned char   IsMasterNo( unsigned char	*pPhone ){
	
	return strcmp_back( g_stGSMSetup.m_strMaterNo, pPhone, PHONE_COMPARE_BIT);
}
/////////////////////////////////////////////////////////////
//  �Ƿ�ת������
unsigned char 	IsForwardNo( unsigned char *pCaller ){
	
	unsigned char	nResult = 0;
	unsigned char	nPhoneLen = 0;
	
	nPhoneLen = strlen( (char*)g_stGSMState.m_strFWDNo );
	if ( nPhoneLen >= PHONE_COMPARE_BIT ){	
		nPhoneLen = PHONE_COMPARE_BIT;
	}
	else{
		if ( nPhoneLen <= 3 ){
			nPhoneLen = 3;
		}
	}
	
	if ( strcmp_back(g_stGSMState.m_strFWDNo, pCaller, nPhoneLen )){
		
		if( strlen( (char*)g_stGSMState.m_strForward ) ){
			strcpy( (char*)pCaller, (char*)g_stGSMState.m_strForward );
		}
		else{
			strcpy( (char*)pCaller, (char*)g_stGSMSetup.m_strMaterNo );			
		}
		nResult = 1;
	}
	return nResult;
}
/////////////////////////////////////////////////////////////
//
unsigned char 	IsAddrServerNo( unsigned char *pCaller ){
	
	unsigned char	nResult = 0;
	unsigned char	nPhoneLen = 0;
	
	nPhoneLen = strlen( (char*)g_stGSMSetup.m_strSMSAddrNo );
	if ( nPhoneLen >= PHONE_COMPARE_BIT ){	
		nPhoneLen = PHONE_COMPARE_BIT;
	}
	else{
		if ( nPhoneLen <= 3 ){
			nPhoneLen = 3;
		}
	}
	
	if ( strcmp_back(g_stGSMSetup.m_strSMSAddrNo, pCaller, nPhoneLen )){
		
		if( strlen( (char*)g_stGSMState.m_strForward ) ){
			strcpy( (char*)pCaller, (char*)g_stGSMState.m_strForward );
		}
		else{
			strcpy( (char*)pCaller, (char*)g_stGSMSetup.m_strMaterNo );			
		}
		nResult = 1;
	}
	return nResult;
}






