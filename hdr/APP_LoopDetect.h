///////////////////////////////////////////////////////////////////////////
// file name: app_Terminal.h
// description: GPRS/GSM Terminal Check Function
// author: wyb
// date: 2009-02-02
/////////////////////////////////////////////////////////////////////////// 

  
#ifndef LOOPDETECT_H
#define LOOPDETECT_H
 
// 循环检测各种状态的任务(512 * 4字节包括寄存器)
#define  		TASK_LOOP_DETECT_STK_SIZE  			90        //80
// 500ms步长	
#define			TIMEOUT24HR							(24*60*60*2)
#define	  	    LP_BOUND_BATTERYCHECK		    	1000//1080
#define  	 	LP_BROKEN_BATTERYCHECK    			422

#define         LP_BOUND_LOWBATTERYCHECK			1117 

#define			PI									3.1415926
#define			EARTH_RADIUS						637.8137
#define			MIN_SPEED							10	//(公里)

// 电压毫伏
#define  		X_Y_Z_AXIS_MAX					    3000
#define  		X_Y_Z_AXIS_MIN					    200
///////////////////////////////////////////////////////////////////////////
// 计算距离
typedef struct __STCALCUDISTANCE{
	
	int				m_nPerLog;
	int				m_nPerLag;
}STCALCUDISTANCE,*PSTCALCUDISTANCE;

extern STCALCUDISTANCE		g_stCalcuDistance;

//////////////////////////////////////////////////////////////////////////////
// 创建线程
void  			HW_LoopDetectTask_Creat(void); 
void 			HW_TaskLoopDetect(void *pData);
unsigned char   LP_ChkBattery(void);
unsigned char   LP_ChkLowBattery(void);
unsigned char   LP_ChkOverspeedAlarm(void);
unsigned char   LP_ChkSOSAlarm(void);
unsigned char   LP_ChkAcc(void);
unsigned char   LP_ChkTowCarAlarm(void);
unsigned char   LP_ChkAntiStealAlarm(void);
unsigned char 	LP_ChkParkingAlarm(void);
unsigned char	LP_ChkAlwaysUpload(void);
//   疲累驾驶报警
unsigned char	LP_ChkTireDriver( void );
double 			RadPIData(double dbData);
unsigned int	CalculateDistance( int	nOldLag,  int	nOldLog,
								   int	nNewLag,  int	nNewLog );
//  检测电子围栏检测
unsigned char   LP_ChkGEOFence( void );
unsigned char	LP_ChkCustom1( void );
unsigned char	LP_ChkCustom2( void );
unsigned char	LP_ChkLeaveRGEOFence( void );
//	检测振动报警
unsigned char	LP_ChkShocking( void );
//	车门报警检测
unsigned char	LP_DoorOpenAlarm( void ); 
//  原车报警检测
unsigned char	LP_AntiStealAlarm( void );  
//	中控锁检测
void 			LP_CheckCentralLocking( void );
//	GPS在指定时间内没有定位,报警
unsigned char	LP_ChkNoGPSAlarm( void );
//  加速感应报警
unsigned char   LP_ChkMMAAlarm( void );
//  计算里程数
void			LP_ChkCalculateMileage(void);
#endif

