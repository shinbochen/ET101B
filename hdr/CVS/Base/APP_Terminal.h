///////////////////////////////////////////////////////////////////////////
//   file name: app_Terminal.h
// description: 协议头文件接口
//      author:
//        date: 2009-02-15
//     reamark: "

#ifndef APP_TERMINAL_H
#define APP_TERMINAL_H

#include "..\hdr\APP_GSM.h"

//=======================宏定义区================================================================
#ifdef  JENNY_FRED
	#define   DEFAULT_PSD 					"8491"
#else
	#define   DEFAULT_PSD 					"1234"
#endif
#define	  DEFAULT_PHONE_EMPTY			""
#define   TEXT_POINT					'.'

//=====出厂初始值设置=============================================================================
#define 	DEFAULT_OVERSPEED		80		// 超速报警出厂值
#define   	MIN_OVERSPEED			20		// 最少速度
#define		MINGEO_SETUP  			200		// 设置最小米

#define		PHONE_COMPARE_BIT		6

#define	    SHORT_MAXVALUE			32766

//====终端控制状态=========================================================================================
enum{
	TE_CTRL_ENABLE = 0x00,
	TE_CTRL_DISABLE	
};

//=====校验和标志==================================================================================
enum{
	CHKSUM_OK     = 0x00,
	CHKSUM_ERROR
};

////////////////////////////////////////////////////////////////////////////////////////
//  返回标志
enum{
	RESULT_ERROR = 0x00,
	RESULT_OK,
	RESULT_EMPTY,
	RESULT_NOT_AUTHORIZATION
};

////////////////////////////////////////////////////////////////////////////////////////
// 设置自定义命令
enum{
	SETUP_CUSTOM1 = 0x01,
	SETUP_CUSTOM2 = 0x02,
	SETUP_CUSTOM3 = 0x04,
	SETUP_CUSTOM4 = 0x08,
	SETUP_ANTISTEAL = 0x10,  //0:门检输入 1:原车防盗器 
	SETUP_ACC_IDLE  = 0x20	 //ACC空闲状态下上传数据
};

////////////////////////////////////////////////////////////////////////////////////////
// 扩展参数设置
enum{

	HW_AUTOFUEL = 0x01
};

/////////////////////////////////////////////////////////////////////////////////////////////
//  电子围栏
typedef struct _STFENCESETUP{
	// bit7: 0/1	入界/出界
	// bit6: 0/1    删除/增加 
    // bit5: 0/1	(为了J容以前的版本)
	unsigned char		m_nFlag;
	// 2010-04-07
	unsigned char		m_nGEOFence; //是否越界报警
	int					m_RLat;      //右上角
	int					m_RLng;      //右上角
	int					m_LLat;      //左下角
	int					m_LLng;      //左下角
}STFENCESETUP,*PSTFENCESETUP;
     
////////////////////////////////////////////////////////////////////////////////////////
//	报警参数
typedef struct _STALARMSETUP {

	// bit0:	0/1   门检断油/不门检断油
	unsigned char		m_nPara;		   	  //扩展参数报警设置
	unsigned char		m_nDefence;			  //ACC关时间多久为设防 单位分钟	 
	unsigned char		m_nILLDoorOpen;		  //非法开门设置 单位分钟	
	unsigned char 		m_nTireDriver;		  //疲劳驾驶设置
	unsigned short		m_nOverspeed;		  //超速报警 单位海里		
	unsigned short		m_nParking;			  //停车设置 		单位分钟			
	unsigned int		m_nAlarmEnable;		  //报警开关 	
	unsigned int		m_nHWValid;			  //终端支持硬件功能
	unsigned int		m_nALValid;			  //终端支持报警功能

	unsigned int		m_nDriveTime;		  //记录车辆运行时间
	unsigned int 		m_nParkingCnt;		  //记录车辆停车时间
	
	unsigned int		m_nACCIdle;			  //ACC关空闲时间 单位分钟			
	unsigned int		m_nNoGPS;			  //GPS未定位时间 单位分钟
	
	//
	unsigned int		m_nTotalMileage;	  //总里程
	unsigned int		m_nCurMileage;		  //当前里程
	unsigned int		m_nMaxMileage;		  //最大里程
	
	// 电子围栏数组
	STFENCESETUP		m_oStGFence[2];	       
}STALARMSETUP,*PSTALARMSETUP;


////////////////////////////////////////////////////////////////////////////////////////
// 自定义线命名
typedef struct _STCUSTOMSETUP{
	//bit0:  设置电平自定义线1
	//bit1:  设置电平自定义线2
	//bit2:  设置电平自定义线3
	//bit3:  设置电平自定义线4
	//bit4:  设置防盗电平状态 0:门检  1:原车防盗
	//bit5:  设置ACC空闲模式下不上传GPRS数据 0: 上传 1:不上传
	unsigned char		m_nCustomPos;    	  				  //自定义位
	//bit0:  出区域报警
	//bit1:  入区域报警
	//bit2:  0:门检输入 1:原车防盗输入
	unsigned char		m_nGeoState;						  //状态位 	
	unsigned char		m_nCustom1[TELNUMBER_LENGTH+1];		  //自定义名1	
	unsigned char		m_nCustom2[TELNUMBER_LENGTH+1];		  //自定义名2
	unsigned char		m_nCustom3[TELNUMBER_LENGTH+1];       //自定义名3
	unsigned char		m_nCustom4[TELNUMBER_LENGTH+1];       //自定义名4
	unsigned short		m_nInGeo;							  //进距离报警	
	unsigned short		m_nDistance;						  //出距离报警	
	int					m_nInLog;			//经度
	int					m_nInLag;			//纬度
	int					m_nCLng;			//经度	
	int					m_nCLat;			//纬度
}STCUSTOMSETUP,*PSTCUSTOMSETUP;

////////////////////////////////////////////////////////////////////////////////////////
//  终端设备状态
#define		GPRS_LOGIN_FLAG				0x01
#define		NEED_WRITE_FLASH			0x02
#define		NEED_RESET_FLAG				0x04
#define		NEED_UPLOADDATA				0x08
#define     NEED_LOCK_FLAG				0x10
#define     NEED_UNLOCK_FLAG			0x20
#define		NEED_RESULT_GG_ADDR			0x40
#define		NEED_QUERY_GG_ADDR			0x80
 
////////////////////////////////////////////////////////////////////////////////////////
//  报警状态
enum{
	//==byte 1===============================================================
	//  紧急报警
	AL_SOS 		 		 = 0x01,
	//  超速报警
	AL_OVERSPEED 		 = 0x02,
	//  停车报警
	AL_PARKING     	 	 = 0x04,
	//	托车报警
	AL_TOW			 	 = 0x08,
	//	入界报警
	AL_ENT_FENCING		 = 0x10,
	//	出界报警
	AL_LEAVOMG_FENCING 	 = 0x20,
	//  主电源断电报警
	AL_POWER_OFF		 = 0x40,
	//	低电压报警
	AL_LOW_POWER		 = 0x80,
	//==byte 2===============================================================
	//  GPS开路报警
	AL_GPS_OPEN			 = 0x100,
	//  GPS短路报警
	AL_GPS_SHORT		 = 0x200,
	//  非法开门报警(在布防之后开门称为非法开门 也叫盗警)	
	AL_NOT_DOOR		 	 = 0x400,
	//  非法点火报警(在布防之后点火称为非法点火)
	AL_NOT_IGNITION		 = 0x800,
	//  自定义报警1
	AL_CUSTOM1			 = 0x1000,
	//  自定义报警2
	AL_CUSTOM2			 = 0x2000,
	//  自定义报警3
	AL_CUSTOM3			 = 0x4000,
	//  自定义报警4
	AL_CUSTOM4			 = 0x8000,
	//==byte 3===============================================================
	//  疲劳驾驶报警
	AL_TIRE_DRIVE		 = 0x10000,
	//  油路报警
	AL_GAS				 = 0x20000,
	//  ACC关报警
	AL_ACC_ALARM		 = 0x40000,
	//  GPS未定位报警
	AL_NOGPS_ALARM		 = 0x80000
};

//  硬件状态
enum{
	//==byte 1===============================================================
	//  1/0 ACC开/关
	HW_ACC_STATE   		 = 0x01,
	//  1/0 车门开/关
	HW_DOOR_STATE		 = 0x02,
	//  1/0 主电源供电/电池供电
	HW_POWER_STATE		 = 0x04,
	//  1/0 备用电源低电压/备用电源正常
	HW_BRK_POWER_STATE	 = 0x08,
	//  1/0 油路连通/断开
	HW_OIL_STATE		 = 0x10,
	//  1/0 紧急开关按下/紧急开关松开
	HW_SOS_STATE		 = 0x20,
	//  1/0 已布防/未布防
	HW_ANTI_STEAL_STATE  = 0x40,
	//  1/0 发动机启动/发动机未启动
	HW_ENGINE_STATE		= 0x80,
	//==byte 2===============================================================
	//  自定义报警线高电平/自定义报警线低电平
	HW_CUSTOM1_STATE   	= 0x100,
	//  自定义报警线高电平/自定义报警线低电平
	HW_CUSTOM2_STATE    = 0x200,
	//  自定义报警线高电平/自定义报警线低电平
	HW_CUSTOM3_STATE    = 0x400,
	//  自定义报警线高电平/自定义报警线低电平
	HW_CUSTOM4_STATE    = 0x800,
	//  1/0  重车/空车
	HW_EMTPYOROVERLOAD_STATE = 0x1000,
	// 1/0 运营状态/非运营状态
	HW_ONDUTY_STATE		= 0x2000,
	// 1/0 广告屏正常/不正常
	HW_LEDPANEL_STATE	= 0x4000,
	// 1/0 手柄正常/不正常
	HW_HANDSET_STATE	= 0x8000,
	//==byte 3===============================================================
	// BIT0~1:	GPS天线信号强度 0~3级，0级为无信号
	HW_GPS_SIGNAL1	   = 0x10000,
	HW_GPS_SIGNAL2     = 0x20000,
	// BIT2~3   GSM天线信号强度0~3级，0级为无信号 
	HW_GSM_SIGNAL1     = 0x40000,
	HW_GSM_SIGNAL2     = 0x80000
};

typedef struct _STDEVICESTATE{
	unsigned char	 	m_nFlag;			//GPRS是否登陆成功
	unsigned char	 	m_nCodeState;		//执行状态
	unsigned int    	m_nHWState;			//硬件状态
	unsigned int	 	m_nALState;			//报警状态
	unsigned int		m_nBattery;			//电池电压
}STDEVICESTATE,*PSTDEVICESTATE;

//////////////////////////////////////////////////////////////////////////////////////
// 
#define     TE_SetLoginSuccess( )   ( g_stDeviceState.m_nFlag  |= GPRS_LOGIN_FLAG )
#define		TE_ClrLoginSuccess( )   ( g_stDeviceState.m_nFlag  &= ~GPRS_LOGIN_FLAG)
#define 	TE_IsLoginSuccess( )    ( g_stDeviceState.m_nFlag  &  GPRS_LOGIN_FLAG )

#define		TE_SetNeedWriteFlash( ) ( g_stDeviceState.m_nFlag  |= NEED_WRITE_FLASH) 
#define		TE_ClrNeedWriteFlash( ) ( g_stDeviceState.m_nFlag  &= ~NEED_WRITE_FLASH)
#define		TE_IsNeedWriteFlash( )  ( g_stDeviceState.m_nFlag  &  NEED_WRITE_FLASH)

#define		TE_SetNeedTEReset( ) 	( g_stDeviceState.m_nFlag  |= NEED_RESET_FLAG) 
#define		TE_ClrNeedTEReset( ) 	( g_stDeviceState.m_nFlag  &= ~NEED_RESET_FLAG)
#define		TE_IsNeedTEReset( )  	( g_stDeviceState.m_nFlag  &  NEED_RESET_FLAG)

#define		TE_SetNeedUploadData( )	(g_stDeviceState.m_nFlag |= NEED_UPLOADDATA)
#define		TE_ClrNeedUploadData( )	(g_stDeviceState.m_nFlag &= ~NEED_UPLOADDATA)
#define		TE_IsNeedUploadData( )	(g_stDeviceState.m_nFlag & NEED_UPLOADDATA )

//////////////////////////////////////////////////////////////////////////////////////
//  锁中控锁
#define     TE_IsNeedLockFlag()         (g_stDeviceState.m_nFlag & NEED_LOCK_FLAG)
#define     TE_SetNeedLockFlag()        (g_stDeviceState.m_nFlag |= NEED_LOCK_FLAG)
#define     TE_ClrNeedLockFlag()        (g_stDeviceState.m_nFlag &= ~NEED_LOCK_FLAG)
//////////////////////////////////////////////////////////////////////////////////////
//  开锁中控锁
#define     TE_IsNeedUnLockFlag()       (g_stDeviceState.m_nFlag & NEED_UNLOCK_FLAG)
#define     TE_SetNeedUnLockFlag()      (g_stDeviceState.m_nFlag |= NEED_UNLOCK_FLAG)
#define     TE_ClrNeedUnLockFlag()      (g_stDeviceState.m_nFlag &= ~NEED_UNLOCK_FLAG)

////////////////////////////////////////////////////////////////////////////////////////
//  返回地址
#define		TE_IsResultGGAddrFlag()		(g_stDeviceState.m_nFlag & NEED_RESULT_GG_ADDR)
#define		TE_SetResultGGAddrFlag()	(g_stDeviceState.m_nFlag |= NEED_RESULT_GG_ADDR)
#define		TE_ClrResultGGAddrFlag() 	(g_stDeviceState.m_nFlag &= ~NEED_RESULT_GG_ADDR)

////////////////////////////////////////////////////////////////////////////////////////
//  查询地址
#define		TE_IsQueryGGAddrFlag()		(g_stDeviceState.m_nFlag & NEED_QUERY_GG_ADDR)
#define		TE_SetQueryGGAddrFlag()		(g_stDeviceState.m_nFlag |= NEED_QUERY_GG_ADDR)
#define		TE_ClrQueryGGAddrFlag() 	(g_stDeviceState.m_nFlag &= ~NEED_QUERY_GG_ADDR)

////////////////////////////////////////////////////////////////////////////
//
#define     TE_HaveAlarm( )         ( g_stDeviceState.m_nALState )
#define     TE_ClearAlarmState()    ( g_stDeviceState.m_nALState = 0 )


#define		AL_CancelAlarm( nState ) (g_stDeviceState.m_nALState &= ~(nState) )

#define		AL_IsSOS( ) 			 (g_stDeviceState.m_nALState &   AL_SOS )
#define		AL_SetSOS( ) 			 (g_stDeviceState.m_nALState |=  AL_SOS )
#define     AL_ClrSOS( )			 (g_stDeviceState.m_nALState &= ~AL_SOS )
//  超速报警
#define		AL_IsOverspeed( )		 (g_stDeviceState.m_nALState &   AL_OVERSPEED  )
#define		AL_SetOverspeed( )		 (g_stDeviceState.m_nALState |=  AL_OVERSPEED )
#define		AL_ClrOverspeed( )		 (g_stDeviceState.m_nALState &= ~AL_OVERSPEED )
//  停车报警
#define		AL_IsParking( )			 (g_stDeviceState.m_nALState &  AL_PARKING ) 
#define		AL_SetParking( )		 (g_stDeviceState.m_nALState |= AL_PARKING ) 	   
#define		AL_ClrParking( )		 (g_stDeviceState.m_nALState &=~AL_PARKING ) 
//	托车报警
#define     AL_IsTow( )			 	 (g_stDeviceState.m_nALState &  AL_TOW ) 
#define		AL_SetTow( )			 (g_stDeviceState.m_nALState |= AL_TOW ) 
#define		AL_ClrTow( )			 (g_stDeviceState.m_nALState &=~AL_TOW ) 
//	入界报警 
#define		AL_IsEnterGEOFence( )		 	 (g_stDeviceState.m_nALState &  AL_ENT_FENCING ) 
#define		AL_SetEnterGEOFence( )		 (g_stDeviceState.m_nALState |= AL_ENT_FENCING ) 	
#define		AL_ClrEnterGEOFence( )		 (g_stDeviceState.m_nALState &=~AL_ENT_FENCING )
//	出界报警
#define		AL_IsLeaveGEOFence( )   (g_stDeviceState.m_nALState  &  AL_LEAVOMG_FENCING ) 
#define		AL_SetLeaveGEOFence( )  (g_stDeviceState.m_nALState  |= AL_LEAVOMG_FENCING ) 
#define		AL_ClrLeaveGEOFence( )  (g_stDeviceState.m_nALState  &=~AL_LEAVOMG_FENCING ) 
//  主电源断电报警
#define		AL_IsPowerOff( )		 (g_stDeviceState.m_nALState  &   AL_POWER_OFF )	
#define		AL_SetPowerOff( )		 (g_stDeviceState.m_nALState  |=  AL_POWER_OFF )
#define		AL_ClrPowerOff( )		 (g_stDeviceState.m_nALState  &= ~AL_POWER_OFF )
//	低电压报警
#define		AL_IsLowPower( )		 (g_stDeviceState.m_nALState  &   AL_LOW_POWER )	
#define		AL_SetLowPower( )		 (g_stDeviceState.m_nALState  |=  AL_LOW_POWER )	
#define		AL_ClrLowPower( )		 (g_stDeviceState.m_nALState  &= ~AL_LOW_POWER )	
//==byte 2===============================================================
//  GPS开路报警
#define		AL_IsGpsOpen( )			 (g_stDeviceState.m_nALState  &   AL_GPS_OPEN )			
#define	    AL_SetGpsOpen( )		 (g_stDeviceState.m_nALState  |=  AL_GPS_OPEN )			
#define	    AL_ClrGpsOpen( )		 (g_stDeviceState.m_nALState  &= ~AL_GPS_OPEN )
//  GPS短路报警
#define		AL_IsGpsShort( )	     (g_stDeviceState.m_nALState  &   AL_GPS_SHORT )	
#define		AL_SetGpsShort( )	     (g_stDeviceState.m_nALState  |=  AL_GPS_SHORT )	
#define		AL_ClrGpsShort( )	     (g_stDeviceState.m_nALState  &= ~AL_GPS_SHORT )	
//  非法开门报警(在布防之后开门称为非法开门 也叫盗警)	
#define		AL_IsNotDoor( )		     (g_stDeviceState.m_nALState  &   AL_NOT_DOOR )		
#define		AL_SetNotDoor( )		 (g_stDeviceState.m_nALState  |=  AL_NOT_DOOR )		
#define		AL_ClrNotDoor( )		 (g_stDeviceState.m_nALState  &= ~AL_NOT_DOOR )		
//  非法点火报警(在布防之后点火称为非法点火)
#define		AL_IsNotIgnition( )		 (g_stDeviceState.m_nALState  &   AL_NOT_IGNITION )	
#define		AL_SetNotIgnition( )	 (g_stDeviceState.m_nALState  |=  AL_NOT_IGNITION )	
#define		AL_ClrNotIgnition( )	 (g_stDeviceState.m_nALState  &= ~AL_NOT_IGNITION )	
//  自定义报警1
#define		AL_IsCustom1( )			 (g_stDeviceState.m_nALState  &   AL_CUSTOM1 )			
#define		AL_SetCustom1( )		 (g_stDeviceState.m_nALState  |=  AL_CUSTOM1 )
#define		AL_ClrCustom1( )		 (g_stDeviceState.m_nALState  &= ~AL_CUSTOM1 )
//  自定义报警2
#define		AL_IsCustom2( )			 (g_stDeviceState.m_nALState  &   AL_CUSTOM2 )			
#define		AL_SetCustom2( )		 (g_stDeviceState.m_nALState  |=  AL_CUSTOM2 )
#define		AL_ClrCustom2( )		 (g_stDeviceState.m_nALState  &= ~AL_CUSTOM2 )
//  自定义报警3
#define		AL_IsCustom3( )			 (g_stDeviceState.m_nALState  &   AL_CUSTOM3 )			
#define		AL_SetCustom3( )		 (g_stDeviceState.m_nALState  |=  AL_CUSTOM3 )
#define		AL_ClrCustom3( )		 (g_stDeviceState.m_nALState  &= ~AL_CUSTOM3 )
//  自定义报警4
#define		AL_IsCustom4( )			 (g_stDeviceState.m_nALState  &   AL_CUSTOM4 )			
#define		AL_SetCustom4( )		 (g_stDeviceState.m_nALState  |=  AL_CUSTOM4 )
#define		AL_ClrCustom4( )		 (g_stDeviceState.m_nALState  &= ~AL_CUSTOM4 )	
//==byte 3===============================================================
//  疲劳驾驶报警
#define		AL_IsTireDrive( )		 (g_stDeviceState.m_nALState  &   AL_TIRE_DRIVE )					
#define		AL_SetTireDrive( )		 (g_stDeviceState.m_nALState  |=  AL_TIRE_DRIVE )					
#define		AL_ClrTireDrive( )		 (g_stDeviceState.m_nALState  &= ~AL_TIRE_DRIVE )
//  ACC关报警
#define		AL_IsACCAlarm()		 	 (g_stDeviceState.m_nALState &   AL_ACC_ALARM )	
#define		AL_SetACCAlarm( )	 	 (g_stDeviceState.m_nALState |=  AL_ACC_ALARM )
#define		AL_ClrACCAlarm( )	 	 (g_stDeviceState.m_nALState &= ~AL_ACC_ALARM )	
//  GPS未定位报警
#define		AL_IsNoGPSAlarm()	 	 (g_stDeviceState.m_nALState &   AL_NOGPS_ALARM )	
#define		AL_SetNoGPSAlarm( )	 	 (g_stDeviceState.m_nALState |=  AL_NOGPS_ALARM )
#define		AL_ClrNoGPSAlarm( )	 	 (g_stDeviceState.m_nALState &= ~AL_NOGPS_ALARM )	
					

//////////////////////////////////////////////////////////////////////////////
//  报警位是否开启
//  紧急报警
#define  	AL_IsEnableSOS( )			(g_stAlarmSetup.m_nAlarmEnable &  AL_SOS )
#define		AL_EnableSOS( )			 	(g_stAlarmSetup.m_nAlarmEnable |= AL_SOS )
#define		AL_DisableSOS( )			(g_stAlarmSetup.m_nAlarmEnable &=~AL_SOS )
//  超速报警
#define		AL_IsEnableOverSpeed( )		(g_stAlarmSetup.m_nAlarmEnable &  AL_OVERSPEED )  	
#define		AL_EnableOverSpeed( )	 	(g_stAlarmSetup.m_nAlarmEnable |= AL_OVERSPEED )  
#define		AL_DisableOverSpeed( )	 	(g_stAlarmSetup.m_nAlarmEnable &=~AL_OVERSPEED )  
//  停车报警
#define		AL_IsEnableParking( )		(g_stAlarmSetup.m_nAlarmEnable &  AL_PARKING ) 		
#define		AL_EnableParking( )		 	(g_stAlarmSetup.m_nAlarmEnable |= AL_PARKING )
#define		AL_DisableParking( )		(g_stAlarmSetup.m_nAlarmEnable &=~AL_PARKING )
//	托车报警
#define		AL_IsEnableTow( )		 	(g_stAlarmSetup.m_nAlarmEnable &  AL_TOW )
#define		AL_EnableTow( )		 		(g_stAlarmSetup.m_nAlarmEnable |= AL_TOW )
#define		AL_DisableEnTow( )		 	(g_stAlarmSetup.m_nAlarmEnable &=~AL_TOW )
//  主电源断电报警
#define		AL_IsEnablePowerOff( )		 (g_stAlarmSetup.m_nAlarmEnable &  AL_POWER_OFF ) 
#define		AL_EnablePowerOff( )		 (g_stAlarmSetup.m_nAlarmEnable |= AL_POWER_OFF ) 
#define		AL_DisablePowerOff( )		 (g_stAlarmSetup.m_nAlarmEnable &=~AL_POWER_OFF ) 
//	低电压报警
#define		AL_IsEnableLowPower( )		 (g_stAlarmSetup.m_nAlarmEnable &  AL_LOW_POWER )
#define		AL_EnableLowPower( )		 (g_stAlarmSetup.m_nAlarmEnable |= AL_LOW_POWER )
#define		AL_DisableLowPower( )		 (g_stAlarmSetup.m_nAlarmEnable &=~AL_LOW_POWER )
//==byte 2===============================================================
//  GPS开路报警
#define		AL_IsEnableGpsOpen( )		 (g_stAlarmSetup.m_nAlarmEnable &  AL_GPS_OPEN )
#define		AL_EnableGpsOpen( )		 	 (g_stAlarmSetup.m_nAlarmEnable |= AL_GPS_OPEN )
#define		AL_DisableGpsOpen( )		 (g_stAlarmSetup.m_nAlarmEnable &=~AL_GPS_OPEN )
//  GPS短路报警
#define		AL_IsEnableGpsShort( )	 	 (g_stAlarmSetup.m_nAlarmEnable &  AL_GPS_SHORT )
#define		AL_EnableGpsShort( )	 	 (g_stAlarmSetup.m_nAlarmEnable |= AL_GPS_SHORT )
#define		AL_DisableGpsShort( )	 	 (g_stAlarmSetup.m_nAlarmEnable &=~AL_GPS_SHORT )
//  非法开门报警(在布防之后开门称为非法开门 也叫盗警)	
#define		AL_IsEnableNotDoor( )		 (g_stAlarmSetup.m_nAlarmEnable &  AL_NOT_DOOR )
#define		AL_EnableNotDoor( )		 	 (g_stAlarmSetup.m_nAlarmEnable |= AL_NOT_DOOR )
#define		AL_DisableNotDoor( )		 (g_stAlarmSetup.m_nAlarmEnable &=~AL_NOT_DOOR )
//  非法点火报警(在布防之后点火称为非法点火)
#define		AL_IsEnableNotIgnition( ) 	 (g_stAlarmSetup.m_nAlarmEnable &  AL_NOT_IGNITION )
#define		AL_EnableNotIgnition( ) 	 (g_stAlarmSetup.m_nAlarmEnable |= AL_NOT_IGNITION )
#define		AL_DisableNotIgnition( ) 	 (g_stAlarmSetup.m_nAlarmEnable &=~AL_NOT_IGNITION )
//  自定义报警1
#define		AL_IsEnableCustom1( )		 (g_stAlarmSetup.m_nAlarmEnable &  AL_CUSTOM1 )	
#define		AL_EnableCustom1( )		 	 (g_stAlarmSetup.m_nAlarmEnable |= AL_CUSTOM1 )	
#define		AL_DisableCustom1( )		 (g_stAlarmSetup.m_nAlarmEnable &=~AL_CUSTOM1 )	
//  自定义报警2
#define		AL_IsEnableCustom2( )		 (g_stAlarmSetup.m_nAlarmEnable &  AL_CUSTOM2 )	
#define		AL_EnableCustom2( )		 	 (g_stAlarmSetup.m_nAlarmEnable |= AL_CUSTOM2 )	
#define		AL_DisableCustom2( )		 (g_stAlarmSetup.m_nAlarmEnable &=~AL_CUSTOM2 )	
//  自定义报警3
#define		AL_IsEnableCustom3( )		 (g_stAlarmSetup.m_nAlarmEnable &  AL_CUSTOM3 )	
#define		AL_EnableCustom3( )		 	 (g_stAlarmSetup.m_nAlarmEnable |= AL_CUSTOM3 )	
#define		AL_DisableCustom3( )		 (g_stAlarmSetup.m_nAlarmEnable &=~AL_CUSTOM3 )	
//  自定义报警4
#define		AL_IsEnableCustom4( )		 (g_stAlarmSetup.m_nAlarmEnable &  AL_CUSTOM4 )	
#define		AL_EnableCustom4( )		 	 (g_stAlarmSetup.m_nAlarmEnable |= AL_CUSTOM4 )	
#define		AL_DisableCustom4( )		 (g_stAlarmSetup.m_nAlarmEnable &=~AL_CUSTOM4 )	
//==byte 3===============================================================
//  疲劳驾驶报警
#define		AL_IsEnableTireDrive( )		 (g_stAlarmSetup.m_nAlarmEnable &  AL_TIRE_DRIVE )	
#define		AL_EnableTireDrive( )	 	 (g_stAlarmSetup.m_nAlarmEnable |= AL_TIRE_DRIVE )
#define		AL_DisableTireDrive( )	 	 (g_stAlarmSetup.m_nAlarmEnable &=~AL_TIRE_DRIVE )
//  ACC关报警
#define		AL_IsEnableACCAlarm()		 (g_stAlarmSetup.m_nAlarmEnable &  AL_ACC_ALARM )	
#define		AL_EnableACCAlarm( )	 	 (g_stAlarmSetup.m_nAlarmEnable |= AL_ACC_ALARM )
#define		AL_DisableACCAlarm( )	 	 (g_stAlarmSetup.m_nAlarmEnable &=~AL_ACC_ALARM )	
//  GPS未定位报警
#define		AL_IsEnableNoGPSAlarm()		 (g_stAlarmSetup.m_nAlarmEnable &  AL_NOGPS_ALARM )	
#define		AL_EnableNoGPSAlarm( )	 	 (g_stAlarmSetup.m_nAlarmEnable |= AL_NOGPS_ALARM )
#define		AL_DisableNoGPSAlarm( )	 	 (g_stAlarmSetup.m_nAlarmEnable &=~AL_NOGPS_ALARM )	

//  门检触发报警,是否断油
#define		HW_IsEnableAutoFuel()		 ( g_stAlarmSetup.m_nPara & HW_AUTOFUEL )
#define		HW_EnableAutoFuel()          (g_stAlarmSetup.m_nPara |= HW_AUTOFUEL )
#define		HW_DisableAutoFuel()		 (g_stAlarmSetup.m_nPara &= ~HW_AUTOFUEL )


///////////////////终端硬件状态////////////////////////////////////////
//  1/0 ACC开/关
#define  HW_IsAccState( )		 ( g_stDeviceState.m_nHWState &   HW_ACC_STATE )
#define  HW_SetAccState( )		 ( g_stDeviceState.m_nHWState |=  HW_ACC_STATE )  
#define  HW_ClrAccState( )		 ( g_stDeviceState.m_nHWState &= ~HW_ACC_STATE )
//  1/0 车门开/关
#define  HW_IsDoorState( )		 ( g_stDeviceState.m_nHWState &   HW_DOOR_STATE )	
#define  HW_SetDoorState( )		 ( g_stDeviceState.m_nHWState |=  HW_DOOR_STATE )
#define  HW_ClrDoorState( )		 ( g_stDeviceState.m_nHWState &= ~HW_DOOR_STATE )
//  1/0 主电源供电/电池供电
#define  HW_IsPowerState( )		 ( g_stDeviceState.m_nHWState &   HW_POWER_STATE )	
#define  HW_SetPowerState( )	 ( g_stDeviceState.m_nHWState |=  HW_POWER_STATE )
#define  HW_ClrPowerState( )	 ( g_stDeviceState.m_nHWState &= ~HW_POWER_STATE )
//  1/0 备用电源低电压/备用电源正常
#define  HW_IsBrkPowerState( )   ( g_stDeviceState.m_nHWState &   HW_BRK_POWER_STATE )		
#define  HW_SetBrkPowerState( )  ( g_stDeviceState.m_nHWState |=  HW_BRK_POWER_STATE )
#define  HW_ClrBrkPowerState( )  ( g_stDeviceState.m_nHWState &= ~HW_BRK_POWER_STATE )
//  1/0 油路连通/断开
#define  HW_IsOilState( )		 ( g_stDeviceState.m_nHWState &   HW_OIL_STATE )	
#define  HW_SetOilState( )		 ( g_stDeviceState.m_nHWState |=  HW_OIL_STATE )	
#define  HW_ClrOilState( )		 ( g_stDeviceState.m_nHWState &= ~HW_OIL_STATE )	
//  1/0 紧急开关按下/紧急开关松开
#define  HW_IsSosState( )		 ( g_stDeviceState.m_nHWState &   HW_SOS_STATE )		
#define  HW_SetSosState( )		 ( g_stDeviceState.m_nHWState |=  HW_SOS_STATE )
#define  HW_ClrSosState( )		 ( g_stDeviceState.m_nHWState &= ~HW_SOS_STATE )
//  1/0 已布防/未布防
#define  HW_IsAntiStealState( )  ( g_stDeviceState.m_nHWState &   HW_ANTI_STEAL_STATE )		  
#define  HW_SetAntiStealState( ) ( g_stDeviceState.m_nHWState |=  HW_ANTI_STEAL_STATE )
#define  HW_ClrAntiStealState( ) ( g_stDeviceState.m_nHWState &= ~HW_ANTI_STEAL_STATE )
//  1/0 发动机启动/发动机未启动
#define  HW_IsEngineState( )     ( g_stDeviceState.m_nHWState &   HW_ENGINE_STATE )	
#define  HW_SetEngineState( )    ( g_stDeviceState.m_nHWState |=  HW_ENGINE_STATE )
#define  HW_ClrEngineState( )    ( g_stDeviceState.m_nHWState &= ~HW_ENGINE_STATE )
//  自定义报警线高电平/自定义报警线低电平
#define	 HW_IsCustom1State( )	 ( g_stDeviceState.m_nHWState &   HW_CUSTOM1_STATE )			
#define	 HW_SetCustom1State( )	 ( g_stDeviceState.m_nHWState |=  HW_CUSTOM1_STATE )
#define	 HW_ClrCustom1State( )	 ( g_stDeviceState.m_nHWState &= ~HW_CUSTOM1_STATE )
//  自定义报警线高电平/自定义报警线低电平
#define	 HW_IsCustom2State( )	 ( g_stDeviceState.m_nHWState &   HW_CUSTOM2_STATE )			
#define	 HW_SetCustom2State( )	 ( g_stDeviceState.m_nHWState |=  HW_CUSTOM2_STATE )
#define	 HW_ClrCustom21State( )	 ( g_stDeviceState.m_nHWState &= ~HW_CUSTOM2_STATE )
//  自定义报警线高电平/自定义报警线低电平
#define	 HW_IsCustom2State( )	 ( g_stDeviceState.m_nHWState &   HW_CUSTOM2_STATE )			
#define	 HW_SetCustom2State( )	 ( g_stDeviceState.m_nHWState |=  HW_CUSTOM2_STATE )
#define	 HW_ClrCustom2State( )	 ( g_stDeviceState.m_nHWState &= ~HW_CUSTOM2_STATE )
//  自定义报警线高电平/自定义报警线低电平
#define	 HW_IsCustom3State( )	 ( g_stDeviceState.m_nHWState &   HW_CUSTOM3_STATE )			
#define	 HW_SetCustom3State( )	 ( g_stDeviceState.m_nHWState |=  HW_CUSTOM3_STATE )
#define	 HW_ClrCustom3State( )	 ( g_stDeviceState.m_nHWState &= ~HW_CUSTOM3_STATE )
//  自定义报警线高电平/自定义报警线低电平
#define	 HW_IsCustom4State( )	 ( g_stDeviceState.m_nHWState &   HW_CUSTOM4_STATE )			
#define	 HW_SetCustom4State( )	 ( g_stDeviceState.m_nHWState |=  HW_CUSTOM4_STATE )
#define	 HW_ClrCustom4State( )	 ( g_stDeviceState.m_nHWState &= ~HW_CUSTOM4_STATE )
//  1/0  重车/空车
#define	 HW_IsEmptyOrOverload( ) ( g_stDeviceState.m_nHWState &   HW_EMTPYOROVERLOAD_STATE )		
#define	 HW_SetEmptyOrOverload( )( g_stDeviceState.m_nHWState |=  HW_EMTPYOROVERLOAD_STATE )		
#define	 HW_ClrEmptyOrOverload( )( g_stDeviceState.m_nHWState &= ~HW_EMTPYOROVERLOAD_STATE )		
// 1/0 运营状态/非运营状态
#define	 HW_IsOndutyState( )	 ( g_stDeviceState.m_nHWState &   HW_ONDUTY_STATE )		
#define	 HW_SetOndutyState( )	 ( g_stDeviceState.m_nHWState |=  HW_ONDUTY_STATE )		
#define	 HW_ClrOndutyState( )	 ( g_stDeviceState.m_nHWState &= ~HW_ONDUTY_STATE )		
// 1/0 广告屏正常/不正常
#define	 HW_IsLedpanelState( )   ( g_stDeviceState.m_nHWState &   HW_LEDPANEL_STATE )		
#define	 HW_SetLedpanelState( )  ( g_stDeviceState.m_nHWState |=  HW_LEDPANEL_STATE )
#define	 HW_ClrLedpanelState( )  ( g_stDeviceState.m_nHWState &= ~HW_LEDPANEL_STATE )
// 1/0 手柄正常/不正常
#define	 HW_IsHandsetState( )	 ( g_stDeviceState.m_nHWState &   HW_HANDSET_STATE )		
#define	 HW_SetHandsetState( )	 ( g_stDeviceState.m_nHWState |=  HW_HANDSET_STATE )
#define	 HW_ClrHandsetState( )	 ( g_stDeviceState.m_nHWState &= ~HW_HANDSET_STATE )

//==============变量定义区===============================================================
extern STALARMSETUP		g_stAlarmSetup;
extern STDEVICESTATE	g_stDeviceState;
extern STCUSTOMSETUP    g_stCustomSetup;

////////////////////////////////////////////////////////////////////////////////////////
// 出区域报警
#define  TE_IsEnableOutGeo( )	(g_stCustomSetup.m_nGeoState &  SETUP_CUSTOM1)
#define  TE_EnableOutGeo( ) 	(g_stCustomSetup.m_nGeoState |= SETUP_CUSTOM1) 	
#define  TE_DisableOutGeo( )	(g_stCustomSetup.m_nGeoState &= ~SETUP_CUSTOM1)
 	
// 入区域报警
#define  TE_IsEnableInGeo( )	(g_stCustomSetup.m_nGeoState  & SETUP_CUSTOM2) 	
#define	 TE_EnableInGeo( )   	(g_stCustomSetup.m_nGeoState |= SETUP_CUSTOM2) 	
#define	 TE_DisableInGeo( )  	(g_stCustomSetup.m_nGeoState &= ~SETUP_CUSTOM2) 	

// 0:门检输入 1:原车防盗输入
#define  TE_IsAntiSteal( )      (g_stCustomSetup.m_nGeoState  & SETUP_CUSTOM3)
#define  TE_SetAntiSteal( )     (g_stCustomSetup.m_nGeoState |= SETUP_CUSTOM3) 	
#define  TE_ClrAntiSteal( )     (g_stCustomSetup.m_nGeoState &= ~SETUP_CUSTOM3) 	


////////////////////////////////////////////////////////////////////////////////////////
//  设置电平检测
#define  TE_SetC1HTrigger( )  ( g_stCustomSetup.m_nCustomPos |= SETUP_CUSTOM1 )
#define  TE_SetC1LTrigger( )  ( g_stCustomSetup.m_nCustomPos &= ~SETUP_CUSTOM1 )
#define  TE_GetCustom1( )  	  ((g_stCustomSetup.m_nCustomPos & SETUP_CUSTOM1)?1:0 )

#define  TE_SetC2HTrigger( )  ( g_stCustomSetup.m_nCustomPos |= SETUP_CUSTOM2 )
#define  TE_SetC2LTrigger( )  ( g_stCustomSetup.m_nCustomPos &= ~SETUP_CUSTOM2 )
#define  TE_GetCustom2( )  	  ((g_stCustomSetup.m_nCustomPos & SETUP_CUSTOM2)?1:0 )

#define  TE_SetC3HTrigger( )  ( g_stCustomSetup.m_nCustomPos |= SETUP_CUSTOM3 )
#define  TE_SetC3LTrigger( )  ( g_stCustomSetup.m_nCustomPos &= ~SETUP_CUSTOM3 )
#define  TE_GetCustom3( )     ((g_stCustomSetup.m_nCustomPos & SETUP_CUSTOM3)?1:0 )

#define  TE_SetC4HTrigger( )  ( g_stCustomSetup.m_nCustomPos |= SETUP_CUSTOM4 )
#define  TE_SetC4LTrigger( )  ( g_stCustomSetup.m_nCustomPos &= ~SETUP_CUSTOM4 )
#define  TE_GetCustom4( )     ((g_stCustomSetup.m_nCustomPos & SETUP_CUSTOM4)?1:0 )
// 设置防盗检测  0:门检输入  1:原车防盗输入
#define  TE_SetASIHTrigger( ) ( g_stCustomSetup.m_nCustomPos |= SETUP_ANTISTEAL ) 
#define  TE_SetASILTrigger( ) ( g_stCustomSetup.m_nCustomPos &= ~SETUP_ANTISTEAL ) 
#define  TE_GetAntisteal( )   ((g_stCustomSetup.m_nCustomPos & SETUP_ANTISTEAL)?1:0 )

// 设置ACC空闲模式下不上传GPRS数据 0: 上传 1:不上传
#define  TE_IsIdleGPRS( )   ( g_stCustomSetup.m_nCustomPos & SETUP_ACC_IDLE )
#define  TE_SetIdleGPRS( )  ( g_stCustomSetup.m_nCustomPos |= SETUP_ACC_IDLE ) 
#define  TE_ClrIdleGPRS( )  ( g_stCustomSetup.m_nCustomPos &= ~SETUP_ACC_IDLE ) 


// 计录车辆总里程(米)
#define	 AddTotalMileage( nMeter )  (g_stAlarmSetup.m_nTotalMileage += (nMeter) )
#define	 GetTotalMileage()  		 (g_stAlarmSetup.m_nTotalMileage)

// 设置当前里程(米)
#define	 AddCurrentMileage( nMeter ) (g_stAlarmSetup.m_nCurMileage += (nMeter) )
#define	 GetCurrentMileage()		 (g_stAlarmSetup.m_nCurMileage )
#define	 ClearCurrentMileage() 	     (g_stAlarmSetup.m_nCurMileage = 0 )

// 设置报警最大里程(米)
#define	 SetMaxMileage( nMeter ) 	 (g_stAlarmSetup.m_nMaxMileage = (nMeter) )
#define	 GetMaxMileage()		     (g_stAlarmSetup.m_nMaxMileage )


//===============================================================
//	实始化电子围栏
void  			InitializtFence( void );
//==============函数定义区===============================================================
void 			WriteAlarmSetupData(void);
void 			ReadAlarmSetupData(void);
void		    InitializeGSMFlash( void );
void		    InitializeAlarmFlash( void );
unsigned char   strcmp_back( unsigned char *p1, unsigned char *p2, unsigned char nLen);
unsigned char   strcmpnc( unsigned char	 *p1, unsigned char	*p2 );
unsigned char   IsAuthorizationNo( unsigned char *pPhone );
unsigned char   IsMasterNo( unsigned char	*pPhone );
unsigned char 	IsForwardNo( unsigned char *pCaller );
unsigned char 	IsAddrServerNo( unsigned char *pCaller );
// 是否监听号码
unsigned char   IsListenNo( unsigned char	*pPhone );
					 				 
#endif //APP_TERMINAL_H