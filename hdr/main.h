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

//*************************************配置修改区******************************************
//===== 内存变量存外部FLASH区 =================================
#define   EXTERNAL_FLASH                	 0		// 加载FLASH功能

#if 	   (EXTERNAL_FLASH)
#define		ATMEL_FLASH				    	 0	   //ATMEL厂家芯片
#define		WINBAND_FLASH   		    	 1	    //Winband厂家芯片
#endif 


//===== 通信模块定义区(此区定义中只能有一位为1） ==============
#define	HW_GTM900							1		// 华为900B	 	    
#define	HW_EM200 							0		// CDMA模块    					  
#define HW_BQM26							0		// 明基模块 
#define	HW_SIMCOM							0		// SIMCOM 模块   
#define	HW_MC323							0		// CDMA模块
#define HW_MG323							0		// 华为GSM四频模块 
 
// SIMCOM模块 GSM锁频 
#if (HW_SIMCOM)
#define	    SIMCOM_340						0
#define		SIMCOM_900             			1
#define		SIMCOM_840W						0
#define		SIMCOM_CBAND_9001800			0
#define		SIMCOM_CBAND_8501900    		0
#endif //HW_SIMCOM

// 华为900B-900C的启动方式不同
#if (HW_GTM900)
#define			HUAWEI_900AB				0  
#define			HUAWEI_900C  				1 
#endif //HW_GTM900

//======硬件定义区(此区定义中只能有一位为1）==================
#define	HW_ET007B       					0		// 支持BENQ M26
#define HW_ET007B_2							0		// 支持SIMCOM MG323
#define	HW_ET101A  							0
#define	HW_ET101B    						0		// 支持900C,900B,SIMCOM340D SIMCOM300D (PCB上新版叫ET101B-2)   
#define	HW_ETC101B   						0		// 支持 EM310 EM200(CDMA)
#define	HW_ET102B							0		// 支持900C,900B,SIMCOM340D SIMCOM300D (可外加FLASH)
#define	HW_ET201B							1		// 支持 EM310 CM200(CDMA) 900C|900B
#define HW_ET201B_2							0		// 支持	SIMCOM340D 900C|900B 
#define	HW_ET201B_3							0		// 支持 SIMCOM340D 增加了外部FLASH 
#define	HW_ET601							0		// 支持 华为两频和四频

//======协议定义区(此区定义中只能有一位为1）==================
#define 	SW_PROTOCOLA					0		//A协议 
#define 	SW_PROTOCOLB					1		//B协议
#define 	SW_PROTOCOLC					0		//Java协议 
#define		SW_PROTOCOL_ASICO		    	0		//

#if  	(SW_PROTOCOLA)
	// 短信指令 
	#define   SW_SMSPROTOCOLA     			1 		//A 
	#define   SW_SMSPROTOCOLB     			0 		//B 
	#define   SW_SMSPROTOCOLC     			0 		//Java
	// 协议定义 GPRS
	#define	  SW_GPRSPROTOCOLA				1
	#define   SW_GPRSPROTOCOLB     			0
	#define   SW_GPRSPROTOCOLC 				0
	#define	  SW_GPRSPROTOCOL_ASICO 		0
#endif //SW_PROTOCOLA

#if	 	(SW_PROTOCOLB)
	// 短信指令 
	#define   SW_SMSPROTOCOLA     			0 	   //A 
	#define   SW_SMSPROTOCOLB     			1 	   //B 
	#define   SW_SMSPROTOCOLC     			0 	   //Java
	// 协议定义 GPRS
	#define	  SW_GPRSPROTOCOLA				0
	#define   SW_GPRSPROTOCOLB     			1
	#define   SW_GPRSPROTOCOLC 				0
	#define	  SW_GPRSPROTOCOL_ASICO 		0
#endif //SW_PROTOCOLB 
 
#if		(SW_PROTOCOLC)
	// 短信指令 
	#define   SW_SMSPROTOCOLA     			0 	 //A 
	#define   SW_SMSPROTOCOLB     			0 	 //B 
	#define   SW_SMSPROTOCOLC     			1 	 //Java
	// 协议定义 GPRS
	#define	  SW_GPRSPROTOCOLA				0
	#define   SW_GPRSPROTOCOLB     			0
	#define   SW_GPRSPROTOCOLC 				1
	#define	  SW_GPRSPROTOCOL_ASICO 		0
#endif //SW_PROTOCOLC

#if    (SW_PROTOCOL_ASICO)
	// 短信指令 
	#define   SW_SMSPROTOCOLA     			0 	//A 
	#define   SW_SMSPROTOCOLB     			1 	//B 
	#define   SW_SMSPROTOCOLC     			0 	//Java
	// 协议定义 GPRS
	#define	  SW_GPRSPROTOCOLA				0
	#define   SW_GPRSPROTOCOLB     			0
	#define   SW_GPRSPROTOCOLC 				0
	#define	  SW_GPRSPROTOCOL_ASICO 		1
#endif //SW_PROTOCOL_ASICO

//======客户定义区(全为0表示为TrackEarth出厂版本）==================
#define			TOM   					  	0
#define	    	JORGE_AUTOFUEL			  	0	// 门检报警 自动断油
#define  		FRANK					  	0	// 没有低电压报警
#define			JENNY						0
#define			GBK_CHRIS				  	0	// 英国客户 GBK_chris 
#define			SUKE						0 	// 发短信号,打电话监听 
#define			FRANK_WU					0	// 黄贤APN配置
#define			LIUYAN						0	// 南非   刘岩 APN配置
#define			LOUDI						1	// 娄底客户 201机器功能


//======用户打电话给设备是否通话或listen区(1/0: talk/listen）========
#define			DIAL_CALL				  	0		

//======GSM模块冷启动定义区(0/1 : disable/enable）==================
#define			HW_COLDRESTART_GSM		    1

//**************************************************************************************** 
//////////////////////////////////////////////////////////////////////////
// 
extern char  g_strVer[32];  
///////////////////////////////////////////////////////////////////////////
// FLASH 地址分配 	

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

// 初始化FLASH
void InitFlash(void);
//  比较FLASH初始化是否成功
unsigned char    IsFlashNormal( unsigned char	 *pSrc, unsigned char	nLen );
#endif
