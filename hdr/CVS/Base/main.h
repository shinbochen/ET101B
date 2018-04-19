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

// �ڴ�������ⲿFLASH
//#define		SETUP_DATA_IN_EXTERNL_FLASH
#define		FLASH_MAX_NORMAL					0x06

//////////////////////////////////////////////////////////////////////////
// 
extern char  g_strVer[32];  
///////////////////////////////////////////////////////////////////////////
// FLASH ��ַ���� 	
#ifdef 	SETUP_DATA_IN_EXTERNL_FLASH
	// 0x00 -0x7FF
	#define		HW_FLASH_SETUP_START					0x00
	#define		FLASH_DATA_ADDR_START  					HW_FLASH_SETUP_START 
#else
	#define		FLASH_DATA_ADDR_START  					0x18000 
#endif //

#define		FLASH_VER_ADDR_START					FLASH_DATA_ADDR_START
#define		FLASH_VER_ADDR_SIZE						32 

#define		FLASH_GSMSETUP_ADDR_START				FLASH_VER_ADDR_START+FLASH_VER_ADDR_SIZE
#define		FLASH_GSMSETUP_ADDR_SIZE				370//320

#define		FLASH_ALARMSETUP_ADDR_START				FLASH_GSMSETUP_ADDR_START+FLASH_GSMSETUP_ADDR_SIZE
#define		FLASH_ALARMSETUP_ADDR_SIZE 				128//96

#define     FLASH_CUSTOMSETUP_ADDR_START			FLASH_ALARMSETUP_ADDR_START+FLASH_ALARMSETUP_ADDR_SIZE
#define     FLASH_CUSTOMSETUP_ADDR_SIZE				128   
  

// ͨ��ģ�鶨��
#define	HW_GTM900		// ��Ϊ900B	 	    
//#define	HW_EM200 	// CDMAģ��    					  
//#define   HW_BQM26	// ����ģ�� 
//#define	HW_SIMCOM	//SIMCOM ģ��  
//#define		HW_MC323	//CDMAģ��

 
// SIMCOMģ�� GSM��Ƶ 
// EGSM_DCS_MODE   	900/1800
// GSM850_PCS_MODE  850/1900
//#define		SIMCOM_CBAND_9001800	
//#define		SIMCOM_CBAND_8501900  


// ��Ϊ900B-900C��������ʽ��ͬ
//#define			HUAWEI_900AB
#define			HUAWEI_900C  

// SIMCOM300-900��ATָ��Ĳ�ͬ 
//#define			SIMCOM_340
//#define			SIMCOM_900
  
// Ӳ������ 

// �ϰ�
//#define	HW_ET101A  		1
            
// ֧��900C,900B,SIMCOM340D SIMCOM300D (PCB���°��ET101B-2)   
//#define	HW_ET101B    	1

// ֧�� EM310 CM200(CDMA)  
//#define	HW_ETC101B   	1

// ֧�� EM310 CM200(CDMA) 900C|900B
//#define	HW_ET201B		1
// EM310 EM200(CDMA)�Ľ����°�,UART���ڵ���
//#define		HW_ET201_UART

// ֧��	SIMCOM340D 900C|900B 
//#define 	HW_ET201B_2		1

// ֧�� SIMCOM340D �������ⲿFLASH 
#define		HW_ET201B_3		1
 
// ֧��BENQ M26
//#define	HW_ET007B     	1

// ֧��SIMCOM
//#define 	HW_ET007B_2		1
 
// ֧��900C,900B,SIMCOM340D SIMCOM300D (�����FLASH)
//#define	HW_ET102B		1

// Э�� 
//#define 	SW_PROTOCOLA		//��Э�� 
#define 	SW_PROTOCOLB		//��Э��	
//#define 	SW_PROTOCOLC		//JavaЭ��
//#define	SW_PROTOCOL_ASICO	//����ѸЭ��    
 
 
// Э�鶨�� SMS
//#define   SW_SMSPROTOCOLA     
#define   SW_SMSPROTOCOLB    
//#define     SW_SMSPROTOCOLC  

// Э�鶨�� GPRS
//#define	SW_GPRSPROTOCOLA
#define   SW_GPRSPROTOCOLB     
//#define  	SW_GPRSPROTOCOLC 
//#define	SW_GPRSPROTOCOL_ASICO 


#ifdef  	SW_PROTOCOLA
#define		SW_SMSPROTOCOLA		 
#define 	SW_GPRSPROTOCOLA
#endif //SW_PROTOCOLA

#ifdef	 	SW_PROTOCOLB
#define   	SW_SMSPROTOCOLB
#define   	SW_GPRSPROTOCOLB 
#endif //SW_PROTOCOLB 
 
#ifdef		SW_PROTOCOLC
#define 	SW_SMSPROTOCOLC
#define  	SW_GPRSPROTOCOLC
#endif //SW_PROTOCOLC

#ifdef      SW_PROTOCOL_ASICO
#define	 	SW_SMSPROTOCOLB
#define	 	SW_GPRSPROTOCOL_ASICO   
#endif //SW_PROTOCOL_ASICO  


// �û�����
#define		TOM_FRED   
// û�е͵�ѹ����
//#define  		FRANK_FRED
// Jenny
//#define		JENNY_FRED
// Ӣ���ͻ� GBK_chris 
//#define		GBK_CHRIS
// ����FLASH����
//#define		EXTERNAL_FLASH
//ATMEL����оƬ
//#define		ATMEL_FLASH		
//Winband����оƬ
//#define		WINBAND_FLASH   
// 201��绰ͨ��
//#define		DIAL_CALL
// SIMCOM/900Cģ��ϵ�Դ,������
#define			HW_COLDRESTART_GSM
// �����ź�,��绰���� 
//#define		SUKE_FRED
// �����ż챨������
#define	    JORGE_AUTOFUEL			1

//
#ifdef HW_ET101A
	#define		GPSUARTInit						UART1Init
	#define		GPSUARTGetch					UART1Getch

	#define		GSMUARTInit						UART0Init
	#define		GSMUARTGetch					UART0Getch
	#define		GSMUARTSendStr					UART0SendStr
	#define		GSMUARTIsSendBufEmpty			IsUART0SendBufEmpty
	#define		GSMUARTPutch					UART0Putch
	#define		GSMUARTGetch					UART0Getch
	#define		GSMUARTSendData					UART0Write
#endif //HW_ET101A

#ifdef HW_ET101B
	#define		GPSUARTInit						UART1Init
	#define		GPSUARTGetch					UART1Getch

	#define		GSMUARTInit						UART0Init
	#define		GSMUARTGetch					UART0Getch
	#define		GSMUARTSendStr					UART0SendStr
	#define		GSMUARTIsSendBufEmpty			IsUART0SendBufEmpty
	#define		GSMUARTPutch					UART0Putch
	#define		GSMUARTGetch					UART0Getch
	#define		GSMUARTSendData					UART0Write
#endif //HW_ET101B

#ifdef 	HW_ET102B
	#define		GPSUARTInit						UART1Init
	#define		GPSUARTGetch					UART1Getch

	#define		GSMUARTInit						UART0Init
	#define		GSMUARTGetch					UART0Getch
	#define		GSMUARTSendStr					UART0SendStr
	#define		GSMUARTIsSendBufEmpty			IsUART0SendBufEmpty
	#define		GSMUARTPutch					UART0Putch
	#define		GSMUARTGetch					UART0Getch
	#define		GSMUARTSendData					UART0Write
#endif //HW_ET102B


#ifdef HW_ETC101B
	#define		GPSUARTInit						UART0Init
	#define		GPSUARTGetch					UART0Getch

	#define		GSMUARTInit						UART1Init
	#define		GSMUARTGetch					UART1Getch
	#define		GSMUARTSendStr					UART1SendStr
	#define		GSMUARTIsSendBufEmpty			IsUART1SendBufEmpty
	#define		GSMUARTPutch					UART1Putch
	#define		GSMUARTGetch					UART1Getch
	#define		GSMUARTSendData					UART1Write
#endif	//HW_ETC101B	

#ifdef HW_ET201B
	#ifdef HW_ET201_UART
		#define		GPSUARTInit						UART1Init
		#define		GPSUARTGetch					UART1Getch 

		#define		GSMUARTInit						UART0Init
		#define		GSMUARTGetch					UART0Getch 
		
		#define		GSMUARTSendStr					UART0SendStr
		#define		GSMUARTIsSendBufEmpty			IsUART0SendBufEmpty
		#define		GSMUARTPutch					UART0Putch
		#define		GSMUARTGetch					UART0Getch
		#define		GSMUARTSendData					UART0Write 
	#else
		#define		GPSUARTInit						UART0Init
		#define		GPSUARTGetch					UART0Getch 

		#define		GSMUARTInit						UART1Init
		#define		GSMUARTGetch					UART1Getch 	
		
		#define		GSMUARTSendStr					UART1SendStr
		#define		GSMUARTIsSendBufEmpty			IsUART1SendBufEmpty
		#define		GSMUARTPutch					UART1Putch
		#define		GSMUARTGetch					UART1Getch
		#define		GSMUARTSendData					UART1Write 
	#endif	
#endif //HW_ET201B

#ifdef HW_ET201B_2
	#define		GPSUARTInit						UART1Init
	#define		GPSUARTGetch					UART1Getch

	#define		GSMUARTInit						UART0Init
	#define		GSMUARTGetch					UART0Getch
	#define		GSMUARTSendStr					UART0SendStr
	#define		GSMUARTIsSendBufEmpty			IsUART0SendBufEmpty
	#define		GSMUARTPutch					UART0Putch
	#define		GSMUARTGetch					UART0Getch
	#define		GSMUARTSendData					UART0Write
#endif //HW_ET201B_2

#ifdef HW_ET201B_3
	#define		GPSUARTInit						UART1Init
	#define		GPSUARTGetch					UART1Getch

	#define		GSMUARTInit						UART0Init
	#define		GSMUARTGetch					UART0Getch
	#define		GSMUARTSendStr					UART0SendStr
	#define		GSMUARTIsSendBufEmpty			IsUART0SendBufEmpty
	#define		GSMUARTPutch					UART0Putch
	#define		GSMUARTGetch					UART0Getch
	#define		GSMUARTSendData					UART0Write
#endif //HW_ET201B_3
	
#ifdef HW_ET007B
	#define		GPSUARTInit						UART0Init
	#define		GPSUARTGetch					UART0Getch

	#define		GSMUARTInit						UART1Init
	#define		GSMUARTGetch					UART1Getch
	#define		GSMUARTSendStr					UART1SendStr
	#define		GSMUARTIsSendBufEmpty			IsUART1SendBufEmpty
	#define		GSMUARTPutch					UART1Putch
	#define		GSMUARTGetch					UART1Getch
	#define		GSMUARTSendData					UART1Write
#endif	//HW_ET007B

#ifdef  HW_ET007B_2
	#define		GPSUARTInit						UART1Init
	#define		GPSUARTGetch					UART1Getch

	#define		GSMUARTInit						UART0Init
	#define		GSMUARTGetch					UART0Getch
	#define		GSMUARTSendStr					UART0SendStr
	#define		GSMUARTIsSendBufEmpty			IsUART0SendBufEmpty
	#define		GSMUARTPutch					UART0Putch
	#define		GSMUARTGetch					UART0Getch
	#define		GSMUARTSendData					UART0Write
#endif //HW_ET007B_2

// ��ʼ��FLASH
void InitFlash(void);
//  �Ƚ�FLASH��ʼ���Ƿ�ɹ�
unsigned char    IsFlashNormal( unsigned char	 *pSrc, unsigned char	nLen );
#endif
