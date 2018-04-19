//////////////////////////////////////////////////////////////////////////
// file name: main.h
// Author:wyb
// date: 20080903

/*
// 写注释时间: 2009-09-10
GPRS的设置:
	1 :  GSM工作模式
	2A:  TCP模式
	2B:  UDP模式

ET101B 一些报警参数的设置:
	  紧急报警:   开启
	  超速报警:   关闭
	  停车报警:   关闭
	  拖车报警:   关闭
	  入界报警:   关闭
	  出界报警:   关闭
主电源断电报警:   开启
	低电压报警:   开启
   GPS开路报警:	  关闭	
   GPS短路报警:	  关闭
  非法开门报警:	  开启
  非法点火报警:	  关闭
   自定义报警1:   关闭
   自定义报警2:   关闭
   自定义报警3:   关闭
   自定义报警4:   关闭
  疲劳驾驶报警:   关闭
  
ET201B 一些报警参数的设置:  
     紧急报警:    开启
	  超速报警:   关闭
	  停车报警:   关闭
	  拖车报警:   关闭
	  入界报警:   关闭
	  出界报警:   关闭
主电源断电报警:   开启
	低电压报警:   开启
   GPS开路报警:	  关闭	
   GPS短路报警:	  关闭
  非法开门报警:	  开启
  非法点火报警:	  关闭
   自定义报警1:   开启
   自定义报警2:   开启
   自定义报警3:   关闭
   自定义报警4:   开启
  疲劳驾驶报警:   关闭
*/ 

#ifndef MAIN_H_20080403 
#define MAIN_H_20080403  

// 内存变量存外部FLASH
//#define		SETUP_DATA_IN_EXTERNL_FLASH
#define		FLASH_MAX_NORMAL					0x06

//////////////////////////////////////////////////////////////////////////
// 
extern char  g_strVer[32];  
///////////////////////////////////////////////////////////////////////////
// FLASH 地址分配 	
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
  

// 通信模块定义
#define	HW_GTM900		// 华为900B	 	    
//#define	HW_EM200 	// CDMA模块    					  
//#define   HW_BQM26	// 明基模块 
//#define	HW_SIMCOM	//SIMCOM 模块  
//#define		HW_MC323	//CDMA模块

 
// SIMCOM模块 GSM锁频 
// EGSM_DCS_MODE   	900/1800
// GSM850_PCS_MODE  850/1900
//#define		SIMCOM_CBAND_9001800	
//#define		SIMCOM_CBAND_8501900  


// 华为900B-900C的启动方式不同
//#define			HUAWEI_900AB
#define			HUAWEI_900C  

// SIMCOM300-900的AT指令的不同 
//#define			SIMCOM_340
//#define			SIMCOM_900
  
// 硬件定义 

// 老板
//#define	HW_ET101A  		1
            
// 支持900C,900B,SIMCOM340D SIMCOM300D (PCB上新版叫ET101B-2)   
//#define	HW_ET101B    	1

// 支持 EM310 CM200(CDMA)  
//#define	HW_ETC101B   	1

// 支持 EM310 CM200(CDMA) 900C|900B
//#define	HW_ET201B		1
// EM310 EM200(CDMA)改进的新板,UART串口调换
//#define		HW_ET201_UART

// 支持	SIMCOM340D 900C|900B 
//#define 	HW_ET201B_2		1

// 支持 SIMCOM340D 增加了外部FLASH 
#define		HW_ET201B_3		1
 
// 支持BENQ M26
//#define	HW_ET007B     	1

// 支持SIMCOM
//#define 	HW_ET007B_2		1
 
// 支持900C,900B,SIMCOM340D SIMCOM300D (可外加FLASH)
//#define	HW_ET102B		1

// 协议 
//#define 	SW_PROTOCOLA		//老协议 
#define 	SW_PROTOCOLB		//新协议	
//#define 	SW_PROTOCOLC		//Java协议
//#define	SW_PROTOCOL_ASICO	//亚美迅协议    
 
 
// 协议定义 SMS
//#define   SW_SMSPROTOCOLA     
#define   SW_SMSPROTOCOLB    
//#define     SW_SMSPROTOCOLC  

// 协议定义 GPRS
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


// 用户定义
#define		TOM_FRED   
// 没有低电压报警
//#define  		FRANK_FRED
// Jenny
//#define		JENNY_FRED
// 英国客户 GBK_chris 
//#define		GBK_CHRIS
// 加载FLASH功能
//#define		EXTERNAL_FLASH
//ATMEL厂家芯片
//#define		ATMEL_FLASH		
//Winband厂家芯片
//#define		WINBAND_FLASH   
// 201打电话通话
//#define		DIAL_CALL
// SIMCOM/900C模块断电源,冷启动
#define			HW_COLDRESTART_GSM
// 发短信号,打电话监听 
//#define		SUKE_FRED
// 触发门检报警断油
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

// 初始化FLASH
void InitFlash(void);
//  比较FLASH初始化是否成功
unsigned char    IsFlashNormal( unsigned char	 *pSrc, unsigned char	nLen );
#endif
