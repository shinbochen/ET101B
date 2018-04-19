//////////////////////////////////////////////////////////////////////////
// file name: main.h
// Author:wyb
// date: 20080903

/*
// дע��ʱ��: 2009-09-10
GPRS������:
	1 :  GSM����ģʽ
	2A:  TCPģʽ
	2B:  UDPģʽ

ET101B һЩ��������������:
	  ��������:   ����
	  ���ٱ���:   �ر�
	  ͣ������:   �ر�
	  �ϳ�����:   �ر�
	  ��籨��:   �ر�
	  ���籨��:   �ر�
����Դ�ϵ籨��:   ����
	�͵�ѹ����:   ����
   GPS��·����:	  �ر�	
   GPS��·����:	  �ر�
  �Ƿ����ű���:	  ����
  �Ƿ���𱨾�:	  �ر�
   �Զ��屨��1:   �ر�
   �Զ��屨��2:   �ر�
   �Զ��屨��3:   �ر�
   �Զ��屨��4:   �ر�
  ƣ�ͼ�ʻ����:   �ر�
  
ET201B һЩ��������������:  
     ��������:    ����
	  ���ٱ���:   �ر�
	  ͣ������:   �ر�
	  �ϳ�����:   �ر�
	  ��籨��:   �ر�
	  ���籨��:   �ر�
����Դ�ϵ籨��:   ����
	�͵�ѹ����:   ����
   GPS��·����:	  �ر�	
   GPS��·����:	  �ر�
  �Ƿ����ű���:	  ����
  �Ƿ���𱨾�:	  �ر�
   �Զ��屨��1:   ����
   �Զ��屨��2:   ����
   �Զ��屨��3:   �ر�
   �Զ��屨��4:   ���� 
  ƣ�ͼ�ʻ����:   �ر�
*/ 

#ifndef MAIN_H_20080403 
#define MAIN_H_20080403  

//*************************************�����޸���******************************************
//===== �ڴ�������ⲿFLASH�� =================================
#define   EXTERNAL_FLASH                	 0		// ����FLASH����

#if 	   (EXTERNAL_FLASH)
#define		ATMEL_FLASH				    	 0	   //ATMEL����оƬ
#define		WINBAND_FLASH   		    	 1	    //Winband����оƬ
#endif 


//===== ͨ��ģ�鶨����(����������ֻ����һλΪ1�� ==============
#define	HW_GTM900							1		// ��Ϊ900B	 	    
#define	HW_EM200 							0		// CDMAģ��    					  
#define HW_BQM26							0		// ����ģ�� 
#define	HW_SIMCOM							0		// SIMCOM ģ��   
#define	HW_MC323							0		// CDMAģ��
#define HW_MG323							0		// ��ΪGSM��Ƶģ�� 
 
// SIMCOMģ�� GSM��Ƶ 
#if (HW_SIMCOM)
#define	    SIMCOM_340						0
#define		SIMCOM_900             			1
#define		SIMCOM_840W						0
#define		SIMCOM_CBAND_9001800			0
#define		SIMCOM_CBAND_8501900    		0
#endif //HW_SIMCOM

// ��Ϊ900B-900C��������ʽ��ͬ
#if (HW_GTM900)
#define			HUAWEI_900AB				0  
#define			HUAWEI_900C  				1 
#endif //HW_GTM900

//======Ӳ��������(����������ֻ����һλΪ1��==================
#define	HW_ET007B       					0		// ֧��BENQ M26
#define HW_ET007B_2							0		// ֧��SIMCOM MG323
#define	HW_ET101A  							0
#define	HW_ET101B    						0		// ֧��900C,900B,SIMCOM340D SIMCOM300D (PCB���°��ET101B-2)   
#define	HW_ETC101B   						0		// ֧�� EM310 EM200(CDMA)
#define	HW_ET102B							0		// ֧��900C,900B,SIMCOM340D SIMCOM300D (�����FLASH)
#define	HW_ET201B							1		// ֧�� EM310 CM200(CDMA) 900C|900B
#define HW_ET201B_2							0		// ֧��	SIMCOM340D 900C|900B 
#define	HW_ET201B_3							0		// ֧�� SIMCOM340D �������ⲿFLASH 
#define	HW_ET601							0		// ֧�� ��Ϊ��Ƶ����Ƶ

//======Э�鶨����(����������ֻ����һλΪ1��==================
#define 	SW_PROTOCOLA					0		//AЭ�� 
#define 	SW_PROTOCOLB					1		//BЭ��
#define 	SW_PROTOCOLC					0		//JavaЭ�� 
#define		SW_PROTOCOL_ASICO		    	0		//

#if  	(SW_PROTOCOLA)
	// ����ָ�� 
	#define   SW_SMSPROTOCOLA     			1 		//A 
	#define   SW_SMSPROTOCOLB     			0 		//B 
	#define   SW_SMSPROTOCOLC     			0 		//Java
	// Э�鶨�� GPRS
	#define	  SW_GPRSPROTOCOLA				1
	#define   SW_GPRSPROTOCOLB     			0
	#define   SW_GPRSPROTOCOLC 				0
	#define	  SW_GPRSPROTOCOL_ASICO 		0
#endif //SW_PROTOCOLA

#if	 	(SW_PROTOCOLB)
	// ����ָ�� 
	#define   SW_SMSPROTOCOLA     			0 	   //A 
	#define   SW_SMSPROTOCOLB     			1 	   //B 
	#define   SW_SMSPROTOCOLC     			0 	   //Java
	// Э�鶨�� GPRS
	#define	  SW_GPRSPROTOCOLA				0
	#define   SW_GPRSPROTOCOLB     			1
	#define   SW_GPRSPROTOCOLC 				0
	#define	  SW_GPRSPROTOCOL_ASICO 		0
#endif //SW_PROTOCOLB 
 
#if		(SW_PROTOCOLC)
	// ����ָ�� 
	#define   SW_SMSPROTOCOLA     			0 	 //A 
	#define   SW_SMSPROTOCOLB     			0 	 //B 
	#define   SW_SMSPROTOCOLC     			1 	 //Java
	// Э�鶨�� GPRS
	#define	  SW_GPRSPROTOCOLA				0
	#define   SW_GPRSPROTOCOLB     			0
	#define   SW_GPRSPROTOCOLC 				1
	#define	  SW_GPRSPROTOCOL_ASICO 		0
#endif //SW_PROTOCOLC

#if    (SW_PROTOCOL_ASICO)
	// ����ָ�� 
	#define   SW_SMSPROTOCOLA     			0 	//A 
	#define   SW_SMSPROTOCOLB     			1 	//B 
	#define   SW_SMSPROTOCOLC     			0 	//Java
	// Э�鶨�� GPRS
	#define	  SW_GPRSPROTOCOLA				0
	#define   SW_GPRSPROTOCOLB     			0
	#define   SW_GPRSPROTOCOLC 				0
	#define	  SW_GPRSPROTOCOL_ASICO 		1
#endif //SW_PROTOCOL_ASICO

//======�ͻ�������(ȫΪ0��ʾΪTrackEarth�����汾��==================
#define			TOM   					  	0
#define	    	JORGE_AUTOFUEL			  	0	// �ż챨�� �Զ�����
#define  		FRANK					  	0	// û�е͵�ѹ����
#define			JENNY						0
#define			GBK_CHRIS				  	0	// Ӣ���ͻ� GBK_chris 
#define			SUKE						0 	// �����ź�,��绰���� 
#define			FRANK_WU					0	// ����APN����
#define			LIUYAN						0	// �Ϸ�   ���� APN����
#define			LOUDI						1	// ¦�׿ͻ� 201��������


//======�û���绰���豸�Ƿ�ͨ����listen��(1/0: talk/listen��========
#define			DIAL_CALL				  	0		

//======GSMģ��������������(0/1 : disable/enable��==================
#define			HW_COLDRESTART_GSM		    1

//**************************************************************************************** 
//////////////////////////////////////////////////////////////////////////
// 
extern char  g_strVer[32];  
///////////////////////////////////////////////////////////////////////////
// FLASH ��ַ���� 	

#define		FLASH_MAX_NORMAL					0x06
#if	 		(EXTERNAL_FLASH)
	// 0x00 -0x7FF
	#define		HW_FLASH_SETUP_START			0x00
	#define		FLASH_DATA_ADDR_START  			HW_FLASH_SETUP_START  
#else
	#define		FLASH_DATA_ADDR_START  			0x18000  
#endif //

#define		FLASH_VER_ADDR_START					FLASH_DATA_ADDR_START
#define		FLASH_VER_ADDR_SIZE						32 

#define		FLASH_GSMSETUP_ADDR_START				FLASH_VER_ADDR_START+FLASH_VER_ADDR_SIZE
#define		FLASH_GSMSETUP_ADDR_SIZE				370//320

#define		FLASH_ALARMSETUP_ADDR_START				FLASH_GSMSETUP_ADDR_START+FLASH_GSMSETUP_ADDR_SIZE
#define		FLASH_ALARMSETUP_ADDR_SIZE 				128//96

#define     FLASH_CUSTOMSETUP_ADDR_START			FLASH_ALARMSETUP_ADDR_START+FLASH_ALARMSETUP_ADDR_SIZE
#define     FLASH_CUSTOMSETUP_ADDR_SIZE				128    
  



#if ( HW_ET101A || HW_ET101B || HW_ET102B || HW_ET201B_2 || HW_ET201B_3 || HW_ET007B_2 || HW_ET601 || HW_ET201B)
	#define		GPSUARTInit							UART1Init
	#define		GPSUARTGetch						UART1Getch

	#define		GSMUARTInit							UART0Init
	#define		GSMUARTGetch						UART0Getch
	#define		GSMUARTSendStr						UART0SendStr
	#define		GSMUARTIsSendBufEmpty				IsUART0SendBufEmpty
	#define		GSMUARTPutch						UART0Putch
	#define		GSMUARTGetch						UART0Getch
	#define		GSMUARTSendData						UART0Write
#else // HW_ETC101B  HW_ET007B
	#define		GPSUARTInit						    UART0Init
	#define		GPSUARTGetch					    UART0Getch

	#define		GSMUARTInit						    UART1Init
	#define		GSMUARTGetch					    UART1Getch
	#define		GSMUARTSendStr						UART1SendStr
	#define		GSMUARTIsSendBufEmpty			    IsUART1SendBufEmpty
	#define		GSMUARTPutch					    UART1Putch
	#define		GSMUARTGetch					    UART1Getch
	#define		GSMUARTSendData						UART1Write
#endif

// ��ʼ��FLASH
void InitFlash(void);
//  �Ƚ�FLASH��ʼ���Ƿ�ɹ�
unsigned char    IsFlashNormal( unsigned char	 *pSrc, unsigned char	nLen );
#endif
