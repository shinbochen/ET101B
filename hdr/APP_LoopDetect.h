///////////////////////////////////////////////////////////////////////////
// file name: app_Terminal.h
// description: GPRS/GSM Terminal Check Function
// author: wyb
// date: 2009-02-02
/////////////////////////////////////////////////////////////////////////// 

  
#ifndef LOOPDETECT_H
#define LOOPDETECT_H
 
// ѭ��������״̬������(512 * 4�ֽڰ����Ĵ���)
#define  		TASK_LOOP_DETECT_STK_SIZE  			90        //80
// 500ms����	
#define			TIMEOUT24HR							(24*60*60*2)
#define	  	    LP_BOUND_BATTERYCHECK		    	1000//1080
#define  	 	LP_BROKEN_BATTERYCHECK    			422

#define         LP_BOUND_LOWBATTERYCHECK			1117 

#define			PI									3.1415926
#define			EARTH_RADIUS						637.8137
#define			MIN_SPEED							10	//(����)

// ��ѹ����
#define  		X_Y_Z_AXIS_MAX					    3000
#define  		X_Y_Z_AXIS_MIN					    200
///////////////////////////////////////////////////////////////////////////
// �������
typedef struct __STCALCUDISTANCE{
	
	int				m_nPerLog;
	int				m_nPerLag;
}STCALCUDISTANCE,*PSTCALCUDISTANCE;

extern STCALCUDISTANCE		g_stCalcuDistance;

//////////////////////////////////////////////////////////////////////////////
// �����߳�
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
//   ƣ�ۼ�ʻ����
unsigned char	LP_ChkTireDriver( void );
double 			RadPIData(double dbData);
unsigned int	CalculateDistance( int	nOldLag,  int	nOldLog,
								   int	nNewLag,  int	nNewLog );
//  ������Χ�����
unsigned char   LP_ChkGEOFence( void );
unsigned char	LP_ChkCustom1( void );
unsigned char	LP_ChkCustom2( void );
unsigned char	LP_ChkLeaveRGEOFence( void );
//	����񶯱���
unsigned char	LP_ChkShocking( void );
//	���ű������
unsigned char	LP_DoorOpenAlarm( void ); 
//  ԭ���������
unsigned char	LP_AntiStealAlarm( void );  
//	�п������
void 			LP_CheckCentralLocking( void );
//	GPS��ָ��ʱ����û�ж�λ,����
unsigned char	LP_ChkNoGPSAlarm( void );
//  ���ٸ�Ӧ����
unsigned char   LP_ChkMMAAlarm( void );
//  ���������
void			LP_ChkCalculateMileage(void);
#endif

