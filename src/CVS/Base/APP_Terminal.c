///////////////////////////////////////////////////////////////////////////
//   file name: app_Terminal.h
// description: 协议头文件接口
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


////////////////////////////////////////////////////////////////////////////
//	报警参数
STALARMSETUP	g_stAlarmSetup = {	
							  #if JORGE_AUTOFUEL
							  1,				 //扩展参数报警设置             	                
						   	  #else
						   	  0,	
						      #endif                     
							  3,				 //ACC关时间多久为设防 单位分钟
							  100,				 //非法开门设置 单位分钟								  
							  240,		   		 //疲劳驾驶设置
							  0,			     //超速报警 	   单位海里		
							  0,			   	 //停车设置 	   单位分钟
							  #if ( HW_ET201B || HW_ET201B_2 || HW_ET201B_3 )
							  0xB041,			 //报警开关	
							  0xF077F,	 		 //终端支持硬件功能
							  0xDBFFF,			 //终端支持报警功能
							  #else
							  	#if (JENNY_FRED||FRANK_FRED)
							  		0x01,		 //报警开关
							  	#else
							  		0x41,		 //报警开关
							  	#endif
							  0xF077F,	 		 //终端支持硬件功能
							  0xD3FFF,			 //终端支持报警功能
							  #endif								
							  0,				 //记录车辆运行时间
							  0,				 //记录车辆停车时间
							  
							  0,				 //ACC关空闲时间 单位分钟	
							  0,				 //GPS未定位时间 单位分钟
							  
							  0,				 //总里程
							  0,				 //当前里程
							  0,				 //最大里程
							  
							  0								  
							  };	
							  
////////////////////////////////////////////////////////////////////////////
//	报警参数
/*
#ifdef HW_ET007B
STALARMSETUP	g_stAlarmSetup = {	
								  0,				 //扩展参数报警设置             
								  3,				 //ACC关时间多久为设防 单位分钟
								  100,				 //非法开门设置 单位分钟								  
								  240,		   		 //疲劳驾驶设置
								  0,			     //超速报警 	   单位海里		
								  0,			   	 //停车设置 	   单位分钟
								  // 关闭断电报警,和低电压报警,关闭门检报
								  0xC1,			 	//ET101:0x104C1 ET201:0x194C1	报警开关	
								  
								  0xF077F,	 		 //终端支持硬件功能
								  0xD3FFF,			 //终端支持报警功能
								  
								  0,				 //记录车辆运行时间
								  0,				 //记录车辆停车时间
								  
								  0,				 //ACC关空闲时间 单位分钟	
								  0,				 //GPS未定位时间 单位分钟
								  
								  0,				 //总里程
								  0,				 //当前里程
								  0,				 //最大里程
								  
								  0								  								  
								  };	
#endif

#ifdef HW_ET007B_2
STALARMSETUP	g_stAlarmSetup = {	
								  0,				 //扩展参数报警设置                             
								  3,				 //ACC关时间多久为设防 单位分钟
								  100,				 //非法开门设置 单位分钟								  
								  240,		   		 //疲劳驾驶设置
								  0,			     //超速报警 	   单位海里		
								  0,			   	 //停车设置 	   单位分钟
								  // 关闭断电报警,和低电压报警,关闭门检报
								  0x41,//0xC1,			 	//ET101:0x104C1 ET201:0x194C1	报警开关	
								  
								  0xF077F,	 		 //终端支持硬件功能
								  0xD3FFF,			 //终端支持报警功能
								
								  0,				 //记录车辆运行时间
								  0,				 //记录车辆停车时间
								  
								  0,				 //ACC关空闲时间 单位分钟	
								  0,				 //GPS未定位时间 单位分钟
								  
								  0,				 //总里程
								  0,				 //当前里程
								  0,				 //最大里程
								  
								  0								  
								  };	
#endif	//HW_ET007B_2


#ifdef HW_ET101A
STALARMSETUP	g_stAlarmSetup = {	
							      0,				 //扩展参数报警设置             		                
								  3,				 //ACC关时间多久为设防 单位分钟
								  100,				 //非法开门设置 单位分钟								  
								  240,		   		 //疲劳驾驶设置
								  0,			     //超速报警 	   单位海里		
								  0,			   	 //停车设置 	   单位分钟
								  // 关闭断电报警,和低电压报警,关闭门检报
								  0xC1,				 //ET101:0x104C1 ET201:0x194C1	报警开关	
								  
								  0xF077F,	 		 //终端支持硬件功能
								  0xD3FFF,			 //终端支持报警功能
								 
								  0,				 //记录车辆运行时间
								  0,				 //记录车辆停车时间
								  
								  0,				 //ACC关空闲时间 单位分钟	
								  0,				 //GPS未定位时间 单位分钟
								  
								  0,				 //总里程
								  0,				 //当前里程
								  0,				 //最大里程
								  
								  0								  
								  };	
#endif	//HW_ET101A

#ifdef HW_ET101B	
	#ifdef FRANK_FRED
	STALARMSETUP	g_stAlarmSetup = {	 
									  0,				 //扩展参数报警设置             	               
									  3,				 //ACC关时间多久为设防 单位分钟
									  100,				 //非法开门设置 单位分钟								  
									  240,		   		 //疲劳驾驶设置
									  0,			     //超速报警 	   单位海里		
									  0,			   	 //停车设置 	   单位分钟
									  // 关闭断电报警,和低电压报警,关闭门检报
									  0x1,			 	//ET101:0x104C1 ET201:0x194C1	报警开关	
									  
									  0xF077F,	 		 //终端支持硬件功能
									  0xD3FFF,			 //终端支持报警功能
									 
									  0,				 //记录车辆运行时间
									  0,				 //记录车辆停车时间
									  
									  0,				 //ACC关空闲时间 单位分钟	
									  0,				 //GPS未定位时间 单位分钟
									  
									  0,				 //总里程
								  	  0,				 //当前里程
								  	  0,				 //最大里程
								  
									  0									  
									  };
	#else 
		#ifdef	JENNY_FRED
		STALARMSETUP	g_stAlarmSetup = {	
								  0,				 //扩展参数报警设置                             
								  3,				 //ACC关时间多久为设防 单位分钟
								  100,				 //非法开门设置 单位分钟								  
								  240,		   		 //疲劳驾驶设置
								  0,			     //超速报警 	   单位海里		
								  0,			   	 //停车设置 	   单位分钟
								  // 关闭断电报警,和低电压报警,关闭门检报
								  0x1,			 	//ET101:0x104C1 ET201:0x194C1	报警开关	
								  
								  0xF077F,	 		 //终端支持硬件功能
								  0xD3FFF,			 //终端支持报警功能
								 
								  0,				 //记录车辆运行时间
								  0,				 //记录车辆停车时间
								  
								  0,				 //ACC关空闲时间 单位分钟	
								  0,				 //GPS未定位时间 单位分钟
								  
								  0,				 //总里程
								  0,				 //当前里程
								  0,				 //最大里程
								  
								  0								  
								  };	
		#else
		STALARMSETUP	g_stAlarmSetup = {
								  0,				 //扩展参数报警设置             		                
								  3,				 //ACC关时间多久为设防 单位分钟
								  100,				 //非法开门设置 单位分钟								  
								  240,		   		 //疲劳驾驶设置
								  0,			     //超速报警 	   单位海里		
								  0,			   	 //停车设置 	   单位分钟
								  // 关闭断电报警,和低电压报警,关闭门检报
								  0xC1,			 	//ET101:0x104C1 ET201:0x194C1	报警开关	
								  
								  0xF077F,	 		 //终端支持硬件功能
								  0xD3FFF,			 //终端支持报警功能
								  
								  0,				 //记录车辆运行时间
								  0,				 //记录车辆停车时间
								  
								  0,				 //ACC关空闲时间 单位分钟	
								  0,				 //GPS未定位时间 单位分钟
								  
								  0,				 //总里程
								  0,				 //当前里程
								  0,				 //最大里程
								  
								  0								  
								  };	
		#endif	//JENNY_FRED
	#endif //FRANK_FRED	
#endif	//HW_ET101B

#ifdef HW_ET102B	
	#ifdef FRANK_FRED
	STALARMSETUP	g_stAlarmSetup = {	
									  0,				 //扩展参数报警设置                             
									  3,				 //ACC关时间多久为设防 单位分钟
									  100,				 //非法开门设置 单位分钟								  
									  240,		   		 //疲劳驾驶设置
									  0,			     //超速报警 	   单位海里		
									  0,			   	 //停车设置 	   单位分钟
									  // 关闭断电报警,和低电压报警,关闭门检报
									  0x1,			 	//ET101:0x104C1 ET201:0x194C1	报警开关	
									  
									  0xF077F,	 		 //终端支持硬件功能
									  0xD3FFF,			 //终端支持报警功能
									  
									  0,				 //记录车辆运行时间
									  0,				 //记录车辆停车时间
									  
									  0,				 //ACC关空闲时间 单位分钟	
									  0,				 //GPS未定位时间 单位分钟
									  
									  0,				 //总里程
								 	  0,				 //当前里程
								 	  0,				 //最大里程
									  
									  0
									  };
	#else 
		#ifdef	JENNY_FRED
		STALARMSETUP	g_stAlarmSetup = {	
								  0,				 //扩展参数报警设置                             
								  3,				 //ACC关时间多久为设防 单位分钟
								  100,				 //非法开门设置 单位分钟								  
								  240,		   		 //疲劳驾驶设置
								  0,			     //超速报警 	   单位海里		
								  0,			   	 //停车设置 	   单位分钟
								  // 关闭断电报警,和低电压报警,关闭门检报
								  0x1,				 //ET101:0x104C1 ET201:0x194C1	报警开关	
								  
								  0xF077F,	 		 //终端支持硬件功能
								  0xD3FFF,			 //终端支持报警功能
								  
								  0,				 //记录车辆运行时间
								  0,				 //记录车辆停车时间
								  
								  0,				 //ACC关空闲时间 单位分钟	
								  0,				 //GPS未定位时间 单位分钟
								  
								  0,				 //总里程
								  0,				 //当前里程
								  0,				 //最大里程
								  								  
								  0								  
								  };	
		#else
		STALARMSETUP	g_stAlarmSetup = {	                
								  0,				 //扩展参数报警设置             
								  3,				 //ACC关时间多久为设防 单位分钟
								  100,				 //非法开门设置 单位分钟								  
								  240,		   		 //疲劳驾驶设置
								  0,			     //超速报警 	   单位海里		
								  0,			   	 //停车设置 	   单位分钟
								  // 关闭断电报警,和低电压报警,关闭门检报
								  0xC1,			 	//ET101:0x104C1 ET201:0x194C1	报警开关	警	
								  
								  0xF077F,	 		 //终端支持硬件功能
								  0xD3FFF,			 //终端支持报警功能
								  
								  0,				 //记录车辆运行时间
								  0,				 //记录车辆停车时间
								  
								  0,				 //ACC关空闲时间 单位分钟	
								  0,				 //GPS未定位时间 单位分钟
								  
								  0,				 //总里程
								  0,				 //当前里程
								  0,				 //最大里程
								  
								  0								  
								  };	
		#endif	//JENNY_FRED
	#endif //FRANK_FRED	
#endif	//HW_ET102B

#ifdef HW_ETC101B
STALARMSETUP	g_stAlarmSetup = {	  
								  0,				 //扩展参数报警设置                           
								  3,				 //ACC关时间多久为设防 单位分钟
								  100,				 //非法开门设置 单位分钟								  
								  240,		   		 //疲劳驾驶设置
								  0,			     //超速报警 	   单位海里		
								  0,			   	 //停车设置 	   单位分钟
								  // 关闭断电报警,和低电压报警,关闭门检报
								  0xC1, 			 //ET101:0x104C1 ET201:0x194C1	报警开关		
								  
								  0xF077F,	 		 //终端支持硬件功能
								  0xD3FFF,			 //终端支持报警功能
								 
								  0,				 //记录车辆运行时间
								  0,				 //记录车辆停车时间
								  
								  0,				 //ACC关空闲时间 单位分钟	
								  0,				 //GPS未定位时间 单位分钟
								  
								  0,				 //总里程
								  0,				 //当前里程
								  0,				 //最大里程
								  
								  0
								  };	
#endif	//HW_ETC101B 

#ifdef HW_ET201B
STALARMSETUP	g_stAlarmSetup = {
								  0,				 //扩展参数报警设置              	                
								  3,				 //ACC关时间多久为设防 单位分钟
								  100,				 //非法开门设置 单位分钟								  
								  240,		   		 //疲劳驾驶设置
								  0,			     //超速报警 	   单位海里		
								  0,			   	 //停车设置 	   单位分钟
								  0xB0C1, 			 //ET101:0x104C1 ET201:0x194C1	报警开关,关闭门检报警	
								  0xF077F,	 		 //终端支持硬件功能
								  0xDBFFF,//0xD3FFF, //终端支持报警功能
								  
								  0,				 //记录车辆运行时间
								  0,				 //记录车辆停车时间
								  
								  0,				 //ACC关空闲时间 单位分钟	
								  0,				 //GPS未定位时间 单位分钟
								  
								  0,				 //总里程
								  0,				 //当前里程
								  0,				 //最大里程
								  
								  0
								  };
#endif	//HW_ET201B	
						  
#ifdef HW_ET201B_2
STALARMSETUP	g_stAlarmSetup = { 	
								  0,				 //扩展参数报警设置                             
								  3,				 //ACC关时间多久为设防 单位分钟
								  100,				 //非法开门设置 单位分钟								  
								  240,		   		 //疲劳驾驶设置
								  0,			     //超速报警 	   单位海里		
								  0,			   	 //停车设置 	   单位分钟
								  0xB0C1,			 //ET101:0x104C1 ET201:0x194C1	报警开关	
								  0xF077F,	 		 //终端支持硬件功能
								  0xDBFFF,//0xD3FFF, //终端支持报警功能
								  
								  0,				 //记录车辆运行时间
								  0,				 //记录车辆停车时间
								  
								  0,				 //ACC关空闲时间 单位分钟	
								  0,				 //GPS未定位时间 单位分钟
								  
								  0,				 //总里程
								  0,				 //当前里程
								  0,				 //最大里程
								  
								  0
								  };
#endif // HW_ET201B_2	

#ifdef HW_ET201B_3
STALARMSETUP	g_stAlarmSetup = { 
								  0,				 //扩展参数报警设置             	                
								  3,				 //ACC关时间多久为设防 单位分钟
								  100,				 //非法开门设置 单位分钟								  
								  240,		   		 //疲劳驾驶设置
								  0,			     //超速报警 	   单位海里		
								  0,			   	 //停车设置 	   单位分钟
								  0xB0C1,			 //0xB4C1,			 //ET101:0x104C1 ET201:0x194C1	报警开关	
								  0xF077F,	 		 //终端支持硬件功能
								  0xDBFFF,//0xD3FFF, //终端支持报警功能
								  
								  0,				 //记录车辆运行时间
								  0,				 //记录车辆停车时间
								  
								  0,				 //ACC关空闲时间 单位分钟	
								  0,				 //GPS未定位时间 单位分钟
								  
								  0,				 //总里程
								  0,				 //当前里程
								  0,				 //最大里程
								  
								  0
								  };
#endif //HW_ET201B_3
*/

STCUSTOMSETUP    g_stCustomSetup = {
									// bit0:  设置电平自定义线1
									// bit1:  设置电平自定义线2
									// bit2:  设置电平自定义线3
									// bit3:  设置电平自定义线4
									// bit4:  设置防盗电平状态 0:门检  1:原车防盗
									// bit5:  设置ACC空闲模式下不上传GPRS数据 0: 上传 1:不上传
									//ET201:  门检为高电平触发		
									0,	
									0,
									"Custom1",		 //自定义名1
									"Custom2",		 //自定义名2
									"Custom3",		 //自定义名3
									"Custom4",		 //自定义名4 
									0,	 		 	 //进距离报警
									0,		 	 	 //出距离报警	
									0,
									0,
									0,
									0
								   };		
				   
//  终端设备状态							  
STDEVICESTATE	g_stDeviceState = {
								   0,				//GPRS是否登陆成功
	       	 				 	   0,				//执行状态							
								   16,				//硬件状态 Bit4 为1 油路正常
								   0,				//报警状态
								   0				//电池电压
								   };								   


/////////////////////////////////////////////////////////////////////
// 写ALARM数据结构
void WriteAlarmSetupData(void)	{		
	
	#ifdef  SETUP_DATA_IN_EXTERNL_FLASH
		WriteDataToFlash2( FLASH_ALARMSETUP_ADDR_START, (unsigned char*)&g_stAlarmSetup, sizeof(STALARMSETUP));
		WriteDataToFlash2( FLASH_CUSTOMSETUP_ADDR_START, (unsigned char*)&g_stCustomSetup, sizeof(STCUSTOMSETUP));
	#else
		WriteDataToFlash((char*)FLASH_ALARMSETUP_ADDR_START, (char*)&g_stAlarmSetup, sizeof(STALARMSETUP));
		WriteDataToFlash((char*)FLASH_CUSTOMSETUP_ADDR_START, (char*)&g_stCustomSetup, sizeof(STCUSTOMSETUP));
	#endif
	
	return ;
}
/////////////////////////////////////////////////////////////////////
// 读取ALARM数据结
void ReadAlarmSetupData(void)	{
	
	#ifdef  SETUP_DATA_IN_EXTERNL_FLASH
		ReadDataFromFlash( (unsigned char*)&g_stAlarmSetup, FLASH_ALARMSETUP_ADDR_START, sizeof(STALARMSETUP) );
		ReadDataFromFlash( (unsigned char*)&g_stCustomSetup, FLASH_CUSTOMSETUP_ADDR_START, sizeof(STCUSTOMSETUP) );
	#else	
		memcpy((char*)&g_stAlarmSetup, (char*)FLASH_ALARMSETUP_ADDR_START, sizeof(STALARMSETUP));
		memcpy((char*)&g_stCustomSetup, (char*)FLASH_CUSTOMSETUP_ADDR_START, sizeof(STCUSTOMSETUP));
	#endif
	
	return ;
}

/////////////////////////////////////////////////////////////////////
//	初始化电子围栏
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
// 初始化FLASH
void		  InitializeGSMFlash( void ){
	
	g_stGSMSetup.m_nCommMode = 0;		//短信方式
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
// 初始化ALARM FLASH
void		  InitializeAlarmFlash( void ){  
	
	g_stAlarmSetup.m_nDefence      		= 3;			//ACC关时间多久为设防 单位分钟
	#ifdef HW_ET201B		
	g_stAlarmSetup.m_nAlarmEnable    	= 0xB0C1;		//报警开关	
	#else
	g_stAlarmSetup.m_nAlarmEnable    	= 0x1;		//报警开关
	#endif	
	#ifdef HW_ET201B_2
	g_stAlarmSetup.m_nAlarmEnable    	= 0xB0C1;		//报警开关	
	#endif
	#ifdef HW_ET201B_3
	g_stAlarmSetup.m_nAlarmEnable    	= 0xB0C1;		//报警开关	
	#endif
	g_stAlarmSetup.m_nILLDoorOpen		= 100;			//非法开门设置 单位分钟
	g_stAlarmSetup.m_nOverspeed 		= 0;			//超速报警 单位公里		
	g_stAlarmSetup.m_nTireDriver		= 240;			//疲劳驾驶设置
	g_stAlarmSetup.m_nParking			= 0;			//停车设置 		单位分钟
	// 初始化电子围栏
	InitializtFence();
	// 初始化自定义线	
	memset(&g_stCustomSetup, 0x00, sizeof(STCUSTOMSETUP) );
	g_stCustomSetup.m_nCustomPos = 0x00;	
	strcpy( (char*)g_stCustomSetup.m_nCustom1, "Custom1");
	strcpy( (char*)g_stCustomSetup.m_nCustom2, "Custom2");
	strcpy( (char*)g_stCustomSetup.m_nCustom3, "Custom3");
	strcpy( (char*)g_stCustomSetup.m_nCustom4, "Custom4");		
}
/////////////////////////////////////////////////////////////
// 字符串号码比较
// 参数: 比较后几位相同就OK
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
// 字符串比较(不分大小写)
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
// 是否授权号码
unsigned char   IsAuthorizationNo(unsigned char	*pPhone ){
	
	return( strcmp_back(g_stGSMSetup.m_strMaterNo, pPhone, PHONE_COMPARE_BIT) ||
			strcmp_back(g_stGSMSetup.m_strAuthNo1, pPhone, PHONE_COMPARE_BIT) ||
	    	strcmp_back(g_stGSMSetup.m_strAuthNo2, pPhone, PHONE_COMPARE_BIT) || 
	    	strcmp_back(g_stGSMSetup.m_strAuthNo3, pPhone, PHONE_COMPARE_BIT) ||
	    	strcmp_back(g_stGSMSetup.m_strAuthNo4, pPhone, PHONE_COMPARE_BIT) ||
	    	strcmp_back(g_stGSMSetup.m_strAuthNo5, pPhone, PHONE_COMPARE_BIT) );
}
/////////////////////////////////////////////////////////////
// 是否监听号码
unsigned char  IsListenNo( unsigned char	*pPhone ){

	return strcmp_back(g_stGSMSetup.m_strAuthNo4, pPhone, PHONE_COMPARE_BIT);
}

/////////////////////////////////////////////////////////////
// 是否授权号码
unsigned char   IsMasterNo( unsigned char	*pPhone ){
	
	return strcmp_back( g_stGSMSetup.m_strMaterNo, pPhone, PHONE_COMPARE_BIT);
}
/////////////////////////////////////////////////////////////
//  是否转发号码
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






