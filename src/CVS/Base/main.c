/////////////////////////////////////////////////////////////////////////
// file name: main.c
// remark: opti.. by shinbo on 2008-04-02
// 版本号:V2.07 更改了GPRS模式,登陆的长度改成了0x09
// 版本号:V2.44-20091229 增回了域名解释,转成SMS模式,关闭GPRS网络

// GTM900 GPRS的工作状态 2010-03-17
// 1: APN没有设置正确,红灯不会快闪
// 2: APN设置正确, TCP模式AT%IPOPEN时红灯快决, UDP模式AT%IPOPEN红灯不快闪,只有发数据时,红灯快闪
// 版本:
// V2.67-100529  增加了五个授权号码
// V2.69-100626  解决了SN乱码问题
// V2.70-100715  解决了SPI SEEL0 脚初始化为高电平
// V2.73-100907  增加了101B 华为模块增加了冷启动功能
// V2.74-101018  修改了DEUID出现+,?等字符
// V2.78-110201  修改接原车防盗的情况下,开启防盗报警
// V2.79-110305  1:修改接原车防盗的情况下,开启防盗报警
//               2:原车防盗时二次报警
//               3:改原车防盗为负触发

//V2.80-110326	 1:修改了CDMA,UART口的调换.
//				 			 2:修改了CDMA的冷启动功能
//V2.81-110413   1:增加cut与on做为断油断电的简要指令
//V2.82-110531   更改了按距离回传,速度要大于十公里才计算
//V2.83-110610	 增加了,关闭SOS报警开关
//V2.84-110620   增加了MC323 模块功能
//V2.85-110730	 修改了回传间隔大于32768,出现出掉高位
//V2.86-110815   修改了CDMA模块,AT%HSMSSS 关闭短信提示报告状态
//V2.87-110901   增加了触发门检报警后,断油路
/**协议文件说明************************************************
***************************************************************/
// SA = J  Java
// SA = 8  ASICO
// SA = 2  JENNY
// SB = 2  JENNY
// SA = 3  GBK_CHRIS
// SB = 3  GBK_CHRIS

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
// 初始化FLASH
//void InitFlash(void);

/////////////////////////////////////////////////////////////////////////
// variable declare  
// 版本号
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
	
	#ifdef HW_ET101A	
		#ifdef HW_COLDRESTART_GSM
		strcpy(g_strVer , "101A-HR"); 
		#else
		strcpy(g_strVer , "101-HA");
		#endif
	#endif 
	
	#ifdef HW_ET101B
		#ifdef HW_COLDRESTART_GSM
		strcpy(g_strVer , "101-HR");   		
		#else
		strcpy(g_strVer , "101-HB");   		
		#endif
	#endif	
	
	#ifdef HW_ET102B
		#ifdef SETUP_DATA_IN_EXTERNL_FLASH
			#ifdef HW_COLDRESTART_GSM
				strcpy(g_strVer , "102X-HR");   		 
			#else
				strcpy(g_strVer , "102X-HB"); 
			#endif
		#else
			#ifdef HW_COLDRESTART_GSM
				strcpy(g_strVer , "102-HR"); 
			#else
				strcpy(g_strVer , "102-HB");   		 
			#endif
		#endif //SETUP_DATA_IN_EXTERNL_FLASH
	#endif
			 
	#ifdef HW_ETC101B 
		#ifdef HW_EM200	
		strcpy(g_strVer, "C101-HC");
		#else
		strcpy(g_strVer, "101-HC");	
		#endif
	#endif //HW_ETC101B
	
	#ifdef HW_ET201B 
		#ifdef HW_EM200 
			#ifdef HW_ET201_UART
				strcpy(g_strVer , "C201-HR"); 
			#else
				strcpy(g_strVer , "C201-H1"); 
			#endif
		#else
			#ifdef HW_COLDRESTART_GSM							
				strcpy(g_strVer , "201-HR");   
			#else
				strcpy(g_strVer , "201-H1");   
			#endif
		#endif
	#endif
	
	#ifdef HW_ET201B_2
		strcpy(g_strVer , "201B-H2");
	#endif
	
	#ifdef HW_ET201B_3
		#ifdef SETUP_DATA_IN_EXTERNL_FLASH
			#ifdef HW_COLDRESTART_GSM
				strcpy(g_strVer , "201X-HR");
			#else
				strcpy(g_strVer , "201X-H3");
			#endif
		#else
			#ifdef HW_COLDRESTART_GSM
				strcpy(g_strVer , "201B-HR"); 
			#else			
				strcpy(g_strVer , "201B-H3");
			#endif
		#endif //SETUP_DATA_IN_EXTERNL_FLASH
	#endif
	
	#ifdef HW_ET007B
		strcpy(g_strVer, "007-H1");
	#endif
	
	#ifdef HW_ET007B_2
		#ifdef HW_COLDRESTART_GSM
		strcpy(g_strVer , "007-HR");
		#else
		strcpy(g_strVer , "007-H2");
		#endif
	#endif 
		
	#ifdef  HW_GTM900	
		#ifdef HUAWEI_900AB
		strcat(g_strVer , "-6"); 	
		#else
		strcat(g_strVer , "-1"); 	
		#endif
	#endif
	
	#ifdef  HW_SIMCOM
	 #ifdef SIMCOM_340
		strcat(g_strVer , "-2");  
	 #else
	 	strcat(g_strVer , "-5"); 
	 #endif	
	#endif	
	
	#ifdef  HW_BQM26
	strcat(g_strVer , "-3"); 	
	#endif 
	
	#ifdef  HW_EM200
	strcat(g_strVer , "-4"); 	
	#endif 	
	
	#ifdef HW_MC323
	strcat(g_strVer , "-7"); 
	#endif
	
	#ifdef  SIMCOM_CBAND_9001800
	strcat(g_strVer , "-B1"); 	
	#endif
	
	#ifdef  SIMCOM_CBAND_8501900
	strcat(g_strVer , "-B2"); 	
	#endif
	
		
	#ifdef	SW_PROTOCOLA  
		#ifdef TOM_FRED 
			strcat(g_strVer , "-SAT"); 	    
		#else
			#ifdef JENNY_FRED 
				strcat(g_strVer , "-SA2"); 
			#else
				#ifdef GBK_CHRIS 
					strcat(g_strVer , "-SA3"); 
				#else
					strcat(g_strVer , "-SA");	
				#endif
			#endif // JENNY_FRED
		#endif	// TOM_FRED
	#endif	// SW_PROTOCOLA
	
	#ifdef	SW_PROTOCOLB
		#ifdef JENNY_FRED 
			strcat(g_strVer , "-SB2");	
		#else
			#ifdef GBK_CHRIS
				strcat(g_strVer , "-SB3"); 
			#else
				#ifdef TOM_FRED 
					strcat(g_strVer , "-SBT");	 
				#else
					strcat(g_strVer , "-SB");	
				#endif
			#endif
		#endif   
	#endif
	
	#ifdef	SW_PROTOCOLC
	strcat(g_strVer , "-SAJ");  
	#endif	  
	
	#ifdef	SW_PROTOCOL_ASICO
	strcat(g_strVer , "-SB8"); 	  
	#endif
		
	#ifdef DIAL_CALL 
	strcat( g_strVer,"-C");
	#endif
	
	#ifdef EXTERNAL_FLASH
		#ifdef ATMEL_FLASH
			strcat( g_strVer,"-A");
		#else
			strcat( g_strVer,"-W");
		#endif
	#endif	
		
	strcat(g_strVer , "-V2.87-110901");	     
} 
///////////////////////////////////////////////////////////////////////////////////
//  比较FLASH初始化是否成功 
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
// 初始化FLASH
void InitFlash(void){
	
	unsigned char		 nLen = 0;
	unsigned char		 strVer[32];	
	#ifdef	EXTERNAL_FLASH
	STGPSINDEX			 stGPSIndex;
	#endif //EXTERNAL_FLASH
	
		
	GetVersion();	
	nLen = strlen( (char *)g_strVer );
	memset( strVer, 0x00, 32 );	
	#ifdef  SETUP_DATA_IN_EXTERNL_FLASH
		ReadDataFromFlash( strVer, FLASH_VER_ADDR_START, nLen );
	#else
		memcpy( strVer, (char *)FLASH_VER_ADDR_START, nLen );
	#endif//SETUP_DATA_IN_EXTERNL_FLASH		

	if( memcmp( g_strVer, strVer, nLen ) == 0 ){
	
		// 读取索引号
		#ifdef EXTERNAL_FLASH		
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
	
		#ifdef EXTERNAL_FLASH
			InitGPSStorageIndex();
		#endif	//EXTERNAL_FLASH
		
		GSM_WriteSetupData( );	
		WriteAlarmSetupData( );
		
		#ifdef  SETUP_DATA_IN_EXTERNL_FLASH
			WriteDataToFlash2( FLASH_VER_ADDR_START, (unsigned char*)g_strVer, nLen );
		#else
			WriteDataToFlash( (char *)FLASH_VER_ADDR_START, (char *)g_strVer, nLen );
		#endif //SETUP_DATA_IN_EXTERNL_FLASH
	}			
			
	/*									
    GetVersion();															// 启动看门狗
	nLen = strlen( (char *)g_strVer );
	// read data from flash
	if( memcmp( g_strVer, (char *)FLASH_VER_ADDR_START, nLen ) == 0 ){		
		GSM_ReadSetupData( );
		ReadAlarmSetupData( );		
	}
	// write data from flash
	else{		
		#ifdef EXTERNAL_FLASH
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