///////////////////////////////////////////////////////////////////////////
// file name: app_Terminal.h
// description: GPRS/GSM Terminal Check Function
// author:  wyb
// date: 2009-3-02
///////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////// 
// include file declare
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "..\hdr\includes.h"
#include "..\hdr\APP_GSM.h"
#include "..\hdr\APP_LoopDetect.h"
#include "..\hdr\APP_GSM.h"
#include "..\hdr\HW_Battery.h"
#include "..\hdr\APP_GPS.h"
#include "..\hdr\APP_Terminal.h"
#include "..\hdr\HW_GPIO.h"
#include "..\hdr\target.h"


///////////////////////////////////////////////////////////////// 
// variable declare
OS_STK      	TaskLoopDetectStk[TASK_LOOP_DETECT_STK_SIZE];
/////////////////////////////////////////////////////////////////
// 计算距离
STCALCUDISTANCE  g_stCalcuDistance = {
									 0,
									 0,
									 };

/////////////////////////////////////////////////////////////////
//创建邮箱信息
void HW_LoopDetectTask_Creat( void ){
	OSTaskCreate(HW_TaskLoopDetect, (void *)0,	&TaskLoopDetectStk[TASK_LOOP_DETECT_STK_SIZE - 1], 9);	
	return;
}

unsigned char		m_nLouDiFlag = 0;

///////////////////////////////////////////////////////////////
// 电池检测
unsigned char LP_ChkBattery(void){
	
	static  unsigned char 	bFlag = 0;	
	unsigned char 			nResult = 0;
	unsigned char			nCnt = 0;	
	
	g_stDeviceState.m_nBattery = GetBatteryVoltage();	
	
	if( g_stDeviceState.m_nBattery < LP_BROKEN_BATTERYCHECK){
		if ( bFlag == 0){		
			
			for( nCnt = 0; nCnt < 8; nCnt++ ){
				// 5ms*4
				OSTimeDly(4);	
				if( GetBatteryVoltage() > LP_BROKEN_BATTERYCHECK ){
					break;				
				}			
			}
			if( nCnt == 8 ){
				bFlag = 1;
				if ( AL_IsEnablePowerOff() ){
					nResult = 1;
					AL_SetPowerOff();
				}
				HW_ClrPowerState();	
				HW_SetBrkPowerState();					
			}
		}
	}
	else{
		bFlag = 0;
		GPRS_ClrNumberSMSMode( AL_POWER_OFF );
		AL_ClrPowerOff();	
		HW_SetPowerState();
		HW_SetBrkPowerState();
	}		
	return nResult;
}

//////////////////////////////////////////////////////////////////
//获取低电压检测
unsigned char LP_ChkLowBattery(void){     
	
	unsigned char 			nResult = 0; 
	static unsigned char	nCnt = 0;
	static unsigned char 	bFlag = 0;	 	
		
							    
	if ( (g_stDeviceState.m_nBattery < LP_BOUND_BATTERYCHECK) && 
		 (g_stDeviceState.m_nBattery > LP_BROKEN_BATTERYCHECK) ){			
		if( nCnt++ > 20 ){
			if ( bFlag == 0){				
				bFlag = 1;
				if ( AL_IsEnableLowPower( ) ){	
					AL_SetLowPower();
					nResult = 1;
				}
				HW_SetPowerState();
				
			}
			nCnt = 0;
		}
	}	
	else if( g_stDeviceState.m_nBattery > LP_BOUND_LOWBATTERYCHECK ){
		
		bFlag = 0;
		nCnt = 0;
		GPRS_ClrNumberSMSMode( AL_LOW_POWER );
		AL_ClrLowPower();
		HW_SetBrkPowerState();			
	}
	return nResult;
} 

///////////////////////////////////////////////////////////////////
//超速报警检测
unsigned char LP_ChkOverspeedAlarm(void){     
	
	unsigned char  		  nResult = 0;
	static unsigned char  nSpeedCheckCnt = 0; 
	//  采样次数  
	static unsigned char  nSampling = 0;	 	 
	//  超速次数
	static unsigned char  nCnt = 0;
	static unsigned char  bFlag = 0;
	
		
	if ( g_stAlarmSetup.m_nOverspeed > 0 ){ 
		//	两秒检测一次
		if ( nSpeedCheckCnt++ < 4 ){		   
			return nResult;
		}						
		nSpeedCheckCnt = 0;
		if ( GPS_GetMilimetre() > g_stAlarmSetup.m_nOverspeed ){
			if ( nCnt == 0 ){
				nSampling = 0;  
			}
			nCnt++;		
		}	
		if ( nSampling++ >= 6 ){ 	
			// 大于或等于6次			
			if ( nCnt >= 4 ){  			
				if ( bFlag == 0 ){	
					bFlag = 1;
					if ( AL_IsEnableOverSpeed() ){
						nResult = 1;						
						AL_SetOverspeed(); 
					}
				}					
			}				
			else{
				GPRS_ClrNumberSMSMode( AL_OVERSPEED );
				AL_ClrOverspeed();
				bFlag = 0;					
			}				
			nCnt = 0;	
			nSampling = 0;				
		}
	}	
	return nResult;
}

/////////////////////////////////////////////////////////////
//紧急报警按键检测
unsigned char LP_ChkSOSAlarm(void){
	
	static unsigned char 	bFlag = 0;
	static unsigned char 	nCnt = 0;
	unsigned char   		nResult = 0;
			
	//GPRS模式紧急报警	
	if( !HW_GET_SOS_STATE() ){	 	
		if( nCnt++ >= 3 ){ 
			if( bFlag == 0 ){					
				
				HW_ClrSosState();			
				bFlag = 1;
				if( AL_IsEnableSOS( ) ){
					nResult = 1;
					AL_SetSOS( );
				}
			}
		}		
	}
	else{
		bFlag = 0;
		nCnt = 0;
		HW_SetSosState();
	}	
	return nResult;
}
/////////////////////////////////////////////////////////////
//ACC检测
unsigned char LP_ChkAcc(void){
	
	unsigned char  			nResult = 0;
	static unsigned int		nCnt = 0;
	static unsigned int		nIdleCnt = 0;
	static unsigned char 	bAccFlag = 0;	
	static unsigned char	bAccOpen = 0;
	static unsigned char	bAccClose = 0;
	static unsigned char	bIdleFlag = 0; 
	
	if( HW_GET_ACC_STATE() ){				//ACC关		
		HW_ClrAccState( );		
		HW_ClrEngineState( );
		#if( LOUDI )
		if( bAccFlag == 0 ){
			bAccFlag = 1;
		}
		#else
		if( bAccFlag == 0 ){				 
			if( (g_stAlarmSetup.m_nDefence * 120 ) < nCnt++ ){ 
				TE_SetNeedLockFlag();
				HW_SetAntiStealState();  	// 已布防 
				bAccFlag = 1;
			}
		}
		AL_ClrTireDrive( );					 // ACC关, 清除疲驾驶
		GPRS_ClrNumberSMSMode( AL_TIRE_DRIVE );
		#endif						
		
		//==ACC开报警==============================================
		bAccOpen = 1; 						//清除状态
		if( AL_IsEnableACCAlarm() && (bAccClose == 1) ){
		
			bAccClose = 0;	
			nResult = 1;
			AL_SetACCAlarm();			
		}
		//==ACC关后空闲状态下======================================
		if( g_stAlarmSetup.m_nACCIdle > 0 ){
			
			if( bIdleFlag == 0 ){
				// 关闭GPRS模式
				if( (g_stAlarmSetup.m_nACCIdle * 120) < nIdleCnt++ ){
				
					bIdleFlag = 1;
					TE_SetIdleGPRS();					
					TE_SetNeedWriteFlash();					
				}
			}
		}
	}
	else{
		if( bAccFlag || ( HW_IsAccState() == 0x00)){
			nCnt = 0;	
			bAccFlag = 0;			
			#if( LOUDI == 0 )
			AL_ClrTow( );				 // ACC开,关闭托车报警
			AL_ClrParking( ); 			 // ACC开,停车报警关闭
			AL_ClrNotDoor( ); 			 // ACC开,门检撤防	
			HW_ClrAntiStealState( );
			GPRS_ClrNumberSMSMode( AL_PARKING|AL_NOT_DOOR|AL_TOW );	
			#endif 
			HW_SetEngineState( );
			HW_SetAccState();					
		}										 //ACC开		
		//==ACC开报警=======================================================
		bAccClose = 1;
		if( AL_IsEnableACCAlarm() && (bAccOpen == 1) ){
		
			bAccOpen = 0;	
			nResult = 1;
			AL_SetACCAlarm();			
		}
		//==ACC空闲=======================================================
		nIdleCnt = 0;
		if( (g_stAlarmSetup.m_nACCIdle > 0) && (bIdleFlag == 1) ){
						
			bIdleFlag = 0;			
			IP_SetConnectNG();
			TE_ClrLoginSuccess();		// 重连GPRS网络
			TE_ClrIdleGPRS();		
			TE_SetNeedWriteFlash(); 	// 保存状态
		}
	}		
	return nResult;
}

/////////////////////////////////////////////////////////////
//  ACC关后计算托车报警
unsigned char LP_ChkTowCarAlarm(void){  

	unsigned char 		 		nResult = 0;
	static unsigned char 		bFlag = 0;
	static unsigned char 		nCnt = 0; 
	
	if ( HW_IsAntiStealState() ){ 
		if( GSP_GetSeaMile() >= 40){							
			if ( bFlag == 0 ){
				if( nCnt++ >= 60){						
					if( AL_IsEnableTow() ){							
						AL_SetTow( );
						nResult = 1;
					}
					bFlag = 1;																
				}
			}
		}
		else{					
			AL_ClrTow( );					 
			nCnt = 0;
			bFlag = 0;
		}
	}
	return  nResult;	
}

/////////////////////////////////////////////////////////////
//	防盗报警检测
unsigned char LP_ChkAntiStealAlarm(void){
	
	unsigned char 				nResult = 0;
		
	if( TE_IsAntiSteal() ){  //原车防盗
		nResult = LP_AntiStealAlarm();
	}
	else{					 //门检测	
		nResult = LP_DoorOpenAlarm();
	}
	return nResult;
}
///////////////////////////////////////////////////////////
//  原车报警检测
unsigned char	LP_AntiStealAlarm( void ){ 
	
	unsigned char			nResult = 0;
	static unsigned char	nCnt = 0;	
	static unsigned char	nALPer = 0;	
	static unsigned short	nALCnt = 0;	
	
	// 门打开	
	if( HW_GET_DOOR_STATE() == TE_GetAntisteal() ){	
	    HW_SetDoorState( );	
		if( HW_IsAntiStealState( ) ){	
			if( nALPer == 0 ){		   			
			   if( nCnt++ > g_stAlarmSetup.m_nILLDoorOpen * 2 ){			
			   		nCnt = 0;
					nALCnt = 0;
					nALPer = 1;
					if( AL_IsEnableNotDoor() ){
						nResult = 1;
						AL_SetNotDoor(); 
						// 触发报警后断油
						if( HW_IsEnableAutoFuel() ){
						
							HW_CTRLOIL_CUTOFF();
							HW_SetOilState();
							GSM_SetOilState();
						}
					}
				}			
			}
		}	
	}
	else{		
		HW_ClrDoorState();
	}
	if( !HW_IsAntiStealState( ) ){ 
		nALPer = 0;
		nALCnt = 0;
		nCnt = 0;
	}
	if( nALPer == 1 ){	
	  if( nALCnt++ >= 5 * 120 ){
		nCnt = 0;
		nALCnt = 0;
		nALPer = 0;
	  }			
	}
	return nResult;
}
///////////////////////////////////////////////////////////
//	车门报警检测
unsigned char	LP_DoorOpenAlarm( void ){

	unsigned char 				nResult = 0;
	static unsigned char 		bFlag = 0;
	static unsigned char		bDoorFlag = 0;		//0:1 正常/准备时间到报警
	static unsigned int  		nCnt = 0;
	
	// 门打开	
	if( HW_GET_DOOR_STATE() == TE_GetAntisteal() ){	
		HW_SetDoorState( );		
		if( AL_IsEnableNotDoor() ){
			if( HW_IsAntiStealState( ) ){ 
				if( bFlag == 0 ) {				
					bDoorFlag = 1;	
					#if( LOUDI )
						//在LOUDI客户不需要时间
						bFlag = 1;
						nResult = 1;
						bDoorFlag = 0;														
						AL_SetNotDoor();
					#else
						//car door open alarm  秒钟为单位  
						if( nCnt++ > g_stAlarmSetup.m_nILLDoorOpen * 2 ){
							
							nCnt = 0;
							bFlag = 1;
							nResult = 1;
							bDoorFlag = 0;														
							AL_SetNotDoor();
							// 触发报警后断油
							if( HW_IsEnableAutoFuel() ){ 
							
								HW_CTRLOIL_CUTOFF();
								HW_SetOilState();
								GSM_SetOilState();
							}											
						}
					#endif
				} 
			}	
			else{//ACC开撤防
				nCnt = 0;
				bDoorFlag = 0;
				bFlag = 0;				
			}	
		}			
	}
	else{
		HW_ClrDoorState(); 		
		bFlag = 0;
	}	
	// 客户信息
	#if( LOUDI == 0 )
	//记录车门打开后多久报警 车门关的时间
	if( bDoorFlag == 1 ){
		// 门打开	
		if( (HW_GET_DOOR_STATE() != TE_GetAntisteal()) && HW_IsAntiStealState() ){
			if( nCnt++ > g_stAlarmSetup.m_nILLDoorOpen * 2 ){ 
				bDoorFlag = 0;		
				nCnt = 0;
				nResult = 1;
				AL_SetNotDoor(); 
				// 触发报警后断油
				if( HW_IsEnableAutoFuel() ){
					HW_CTRLOIL_CUTOFF();
					HW_SetOilState();
				}					
			}	
		}
	}	
	#endif
	return nResult;
}
///////////////////////////////////////////////////////////
// 停车报警
unsigned char LP_ChkParkingAlarm(void){  

	unsigned char        nResult = 0;
	static unsigned char nFlag = 0;
	
	//ACC关
	if( HW_GET_ACC_STATE() && AL_IsEnableParking() ){	
		if( g_stAlarmSetup.m_nParking > 0){
			// 开始记录停车报警时间  2*60=120
			if( g_stAlarmSetup.m_nParkingCnt++ >= (g_stAlarmSetup.m_nParking * 120)){		
				if( nFlag == 0){					
					nFlag = 1;
					nResult = 1;
					AL_SetParking();
				}
			}
		}
	}
	else{
		nFlag = 0;
		g_stAlarmSetup.m_nParkingCnt = 0; 
	}	
	return nResult;
}
///////////////////////////////////////////////////////////
//   疲累驾驶报警
unsigned char	LP_ChkTireDriver( void ){
	
	unsigned char			nResult = 0;
	static  unsigned char	nFlag = 0;
	
	if( !HW_GET_ACC_STATE() ){ 		// ACC开			
		if(  g_stAlarmSetup.m_nDriveTime++ > (g_stAlarmSetup.m_nTireDriver * 120) ){
			if( AL_IsEnableTireDrive() ){
				if( nFlag == 0 ){
					AL_SetTireDrive();
					nFlag = 1;
				}
			} 
		}
	}	
	else{						   // ACC关									
		nFlag = 0;
		g_stAlarmSetup.m_nDriveTime = 0;
	}
	return nResult;
}
///////////////////////////////////////////////////////////
//	GPS在指定时间内没有定位,报警
unsigned char	LP_ChkNoGPSAlarm( void ){

	unsigned char			nResult = 0;
	static unsigned char	nFlag  = 0;
	static unsigned int		nCnt = 0;
	
	if( AL_IsEnableNoGPSAlarm() ){
		
		if( GPS_IsPosValid()  ){
		
			nCnt = 0;
			nFlag = 0;
			AL_ClrNoGPSAlarm();
			GPRS_ClrNumberSMSMode(AL_NOGPS_ALARM);
		}
		else{
			if( nFlag == 0 ){
				if( (g_stAlarmSetup.m_nNoGPS * 120) < nCnt++ ){
					
					nFlag = 1;
					nResult = 1;
					AL_SetNoGPSAlarm();				
				}
			}	
		}
	}
	return nResult;
}
///////////////////////////////////////////////////////////
//  加速感应报警
unsigned char   LP_ChkMMAAlarm( void ){
	
	unsigned char		nResult = 0;
	
#if (HW_ET601)
	unsigned char 		nCnt = 0;
	unsigned short		nXAxis = 0;
	unsigned short		nYAxis = 0;
	unsigned short		nZAxis = 0;
	char				chBuf[50];
		
	sprintf( chBuf, "B:%d\r\n", g_stDeviceState.m_nBattery );
	UART1SendStr(chBuf);	
	nXAxis = MMA_GetVoltage( MMA_XOUT );
	sprintf( chBuf, "X:%d\r\n", nXAxis );
	UART1SendStr(chBuf);

	nYAxis = MMA_GetVoltage( MMA_YOUT );
	sprintf( chBuf, "Y:%d\r\n", nYAxis );
	UART1SendStr(chBuf);
	nZAxis = MMA_GetVoltage( MMA_ZOUT );
	sprintf( chBuf, "Z:%d\r\n", nZAxis );
	UART1SendStr(chBuf);
	
	if( (nXAxis > X_Y_Z_AXIS_MAX) || 
		(nXAxis < X_Y_Z_AXIS_MIN) ||
		(nYAxis > X_Y_Z_AXIS_MAX) ||
		(nYAxis < X_Y_Z_AXIS_MIN) ||
		(nZAxis > X_Y_Z_AXIS_MAX) ||
		(nZAxis < X_Y_Z_AXIS_MIN) ){
		
	/*	for( nCnt = 0; nCnt < 10; nCnt++ ){					
			GPIO_Set(16, 1);
			OSTimeDlyHMSM(0,0,0,25);				
			GPIO_Set(16, 0);	
			OSTimeDlyHMSM(0,0,0,25);				
		}
		GPIO_Set(16, 0);	
	*/			
		nResult = 1;
	}		
				
#endif		
	return nResult;
}
///////////////////////////////////////////////////////////////
// 娄底客户要求
#if( LOUDI )
///////////////////////////////////////////////////////////
//	车载布防 
unsigned char	LP_ChkCustom1( void ){  
	
	unsigned char			nResult = 0;	
		
	if( HW_GET_CUSTOM1() == TE_GetCustom1() ){
	
		if( m_nLouDiFlag == 0 ){		
			m_nLouDiFlag = 1;		
			HW_SetAntiStealState();  	// 已布防	
			TE_SetNeedLockFlag();	
			AL_ClrTireDrive( );			// ACC关, 清除疲驾驶
			GPRS_ClrNumberSMSMode( AL_TIRE_DRIVE );
		}
	}
	if( HW_GET_CUSTOM1() ){
		HW_SetCustom1State();
	}
	else{
		HW_ClrCustom1State();
	} 		
	return  nResult;
}
///////////////////////////////////////////////////////////
//	车载取消布防 
unsigned char	LP_ChkCustom2( void ){
	
	unsigned char			nResult = 0;
	
	
	if( HW_GET_CUSTOM2() == TE_GetCustom2() ){	
	
		if( m_nLouDiFlag == 1 ){
			m_nLouDiFlag = 0;				
			AL_ClrTow( );				 // ACC开,关闭托车报警
			AL_ClrParking( ); 			 // ACC开,停车报警关闭
			AL_ClrNotDoor( ); 			 // ACC开,门检撤防
			HW_ClrAntiStealState();				
			GPRS_ClrNumberSMSMode( AL_PARKING|AL_NOT_DOOR|AL_TOW );	
		}
	}
		
	if( HW_GET_CUSTOM2() ){
		HW_SetCustom2State();
	}
	else{
		HW_ClrCustom2State(); 
	}
	return  nResult;
}
#else
///////////////////////////////////////////////////////////
//	自定义线1 
unsigned char	LP_ChkCustom1( void ){ 
	
	unsigned char			nResult = 0;
	
	#if( HW_ET201B || HW_ET201B_2 || HW_ET201B_3 )	
	static unsigned char	nFlag = 0;	
	
	if( HW_GET_CUSTOM1() == TE_GetCustom1() ){
		if( nFlag == 0 ){
			nFlag = 1;
			if( AL_IsEnableCustom1() ){ 
				nResult = 1;
				AL_SetCustom1();
			}
		}
	}
	else{			
		nFlag = 0;
	}
	if( HW_GET_CUSTOM1() ){
		HW_SetCustom1State();
	}
	else{
		HW_ClrCustom1State();
	} 
	#endif
	
	return  nResult;
}
///////////////////////////////////////////////////////////
//	自定义线2 
unsigned char	LP_ChkCustom2( void ){
	
	unsigned char			nResult = 0;
	
	#if( HW_ET201B || HW_ET201B_2 || HW_ET201B_3 )	
	static unsigned char	nFlag = 0;	
	
	
	if( HW_GET_CUSTOM2() == TE_GetCustom2() ){	
		// 三秒后报警
		if( nFlag == 0 ){
			
			nFlag = 1;
			if( AL_IsEnableCustom2() ){
				nResult = 1;
				AL_SetCustom2();
			}
		}
	}
	else{			
		nFlag = 0;
	}	
	if( HW_GET_CUSTOM2() ){
		HW_SetCustom2State();
	}
	else{
		HW_ClrCustom2State(); 
	}
	#endif
	return  nResult;
}
#endif //


///////////////////////////////////////////////////////////
//	检测振动报警
unsigned char	LP_ChkShocking( void ){

	unsigned char			nResult = 0;	
	
	#if( HW_ET201B || HW_ET201B_2 || HW_ET201B_3 )	
	static unsigned char	nALPer = 0;
	static unsigned short	nALCnt = 0;
	
	// 振动报警是否开启
	if( HW_IsAntiStealState() ){						
		// 触发报警
		if( HW_GET_SHOCKING() == TE_GetCustom4() ){
			HW_SetCustom4State();
			if( nALPer == 0 ){
				nALCnt = 0;
				nALPer = 1;
				if( AL_IsEnableCustom4() ){
					nResult = 1;
					AL_SetCustom4();				
				}
			}
		}
		else{
			HW_ClrCustom4State();
		}
		if( nALPer == 1 ){
			//  五分钟过后,再检测报警  
			if( nALCnt++ >= 5 * 120 ){
				nALPer = 0;
				nALCnt = 0;	
			}			
		}
	}
	if( !HW_IsAntiStealState( ) ){ 
		nALPer = 0;
		nALCnt = 0;	
	}
	#endif
	return nResult;
}
///////////////////////////////////////////////////////////
//  检测电子围栏检测
unsigned char  LP_ChkGEOFence( void ){ 
	
	// 5秒检测一次
	static unsigned char			nCnt = 0;
	unsigned char					nResult = 0;
	unsigned char					nPos = 0;
	unsigned long					lLat = 0;
	unsigned long					lLng = 0;  
	
	
	if( nCnt++ >= 10 ){
		nCnt = 0;
		if( GPS_IsPosValid() ){
			lLat = GPS_GetLatitude();
			lLng = GPS_GetLongitude();
			for( nPos = 0; nPos < 2; nPos++ ){
				if( (g_stAlarmSetup.m_oStGFence[nPos].m_nFlag & 0x20) == 0x00 ){					
					continue;
				}
				// Enter GEOFence
				if( g_stAlarmSetup.m_oStGFence[nPos].m_nFlag & 0x80 ){ 
					if( AL_IsEnableEnterGEOFence( ) ){
						if( g_stAlarmSetup.m_oStGFence[nPos].m_LLat < lLat &&
							  g_stAlarmSetup.m_oStGFence[nPos].m_RLat > lLat &&
							  g_stAlarmSetup.m_oStGFence[nPos].m_LLng < lLng &&
							  g_stAlarmSetup.m_oStGFence[nPos].m_RLng > lLng ){		
							  //if( !AL_IsEnterGEOFence( ) ){ 
							  if( g_stAlarmSetup.m_oStGFence[nCnt].m_nGEOFence == 0 ){
							  	AL_SetEnterGEOFence( );
							  	g_stAlarmSetup.m_oStGFence[nCnt].m_nGEOFence = 1;

							  	nResult = 1;
							  }				  	
						}	
						else{
							g_stAlarmSetup.m_oStGFence[nCnt].m_nGEOFence = 0;
							AL_ClrEnterGEOFence( );							
						}					
					}						
				}
				// Leave GEOFence
				else{						
					if( AL_IsEnableLeaveGEOFence( ) ){
						if( g_stAlarmSetup.m_oStGFence[nPos].m_LLat > lLat ||
							  g_stAlarmSetup.m_oStGFence[nPos].m_RLat < lLat ||
							  g_stAlarmSetup.m_oStGFence[nPos].m_LLng > lLng ||
							  g_stAlarmSetup.m_oStGFence[nPos].m_RLng < lLng ){			
							
							  if( g_stAlarmSetup.m_oStGFence[nCnt].m_nGEOFence == 0 ){
							  	AL_SetLeaveGEOFence( );
							  	g_stAlarmSetup.m_oStGFence[nCnt].m_nGEOFence = 1;
							  	nResult = 1;
							  }				  	
						}
						else{
							g_stAlarmSetup.m_oStGFence[nCnt].m_nGEOFence = 0;
							AL_ClrLeaveGEOFence( );
						}	
					}						
				}							
			}			
		}
	}
	
	return  nResult;
}
///////////////////////////////////////////////////////////
//	检测出区域报警
unsigned char	LP_ChkLeaveRGEOFence( void ){

	unsigned char			nResult = 0;
	static unsigned char	nCnt = 0;
	static unsigned char	nFlag = 0;
	static unsigned char	nRecordCnt = 0;
	
	if (  TE_IsEnableOutGeo() &&
		 ( g_stCustomSetup.m_nDistance >= MINGEO_SETUP) ){
		// 5秒计算一次
		if( nCnt++ >= 10 ){	 
			nCnt = 0;			
			
			if( CalculateDistance( g_stCustomSetup.m_nCLat,
								   g_stCustomSetup.m_nCLng,
								   GPS_GetLatitude(), 
								   GPS_GetLongitude()) > g_stCustomSetup.m_nDistance ){
				// 20秒一直入界就报警								   
				if( nFlag == 0 && nRecordCnt++ >= 4 ){ 
					nFlag = 1;		
					nRecordCnt = 0;
					AL_SetLeaveGEOFence( );				
					nResult = 1;
				}		  
			}
			else{
				nFlag = 0;
				AL_ClrLeaveGEOFence( );	
				nRecordCnt = 0;
			}
		}
		
	}		
	return 	nResult;
}
/////////////////////////////////////////////////////////// 
// GSM 自动回传间隔
unsigned char	LP_ChkAlwaysUpload(void){ 
	
	unsigned char        nResult = 0;
	static unsigned int  nCnt = 0;
		
	if( g_stGSMSetup.m_nCBMode == CB_BYTIME ){		    // 按时间回传达		
		if( g_stGSMSetup.m_nCBSpace >= 5 ){
			if( nCnt++ >= 	g_stGSMSetup.m_nCBSpace * 2 ){ 
				nResult = 1;
				nCnt = 0;
			}
		} 
	}
	else if( g_stGSMSetup.m_nCBMode == CB_BYDISTANCE ){  //按距离回传	
		
		if( (g_stGSMSetup.m_nCBSpace >= MINGEO_SETUP ) &&
			( nCnt++ >= 10) ){
			
			if( GPS_GetMilimetre() >= MIN_SPEED ){
			
				if( (g_stCalcuDistance.m_nPerLag == 0) ||
					(g_stCalcuDistance.m_nPerLog == 0) ){
					
					g_stCalcuDistance.m_nPerLag = GPS_GetLatitude();
					g_stCalcuDistance.m_nPerLog = GPS_GetLongitude();
				}
				else{
					//按距离回传			
					if( CalculateDistance( g_stCalcuDistance.m_nPerLag,
										   g_stCalcuDistance.m_nPerLog,
										   GPS_GetLatitude(), 
										   GPS_GetLongitude() ) >= g_stGSMSetup.m_nCBSpace ){
										  
						g_stCalcuDistance.m_nPerLag = GPS_GetLatitude();
						g_stCalcuDistance.m_nPerLog = GPS_GetLongitude();				  
						nResult = 1;
					}
				}	
			}			
			nCnt = 0;
		}
	}
	
	return nResult;	
}
/////////////////////////////////////////////////////////// 
//  计算里程数
void	LP_ChkCalculateMileage(void){

	static unsigned char		nCnt = 0;
	unsigned short				nMeter = 0;
	static int					nPerLog = 0;
	static int					nPerLag = 0; 
	
	
	if( GPS_IsPosValid() && (nCnt++ >= 20) ){
		
		nCnt = 0;		
		if( HW_IsAccState() ){
				
			if( (nPerLag == 0) || (nPerLog == 0) ){
				
				nPerLag = GPS_GetLatitude();
				nPerLog = GPS_GetLongitude();
			}
			else{						
				nMeter = CalculateDistance( nPerLag, nPerLog, GPS_GetLatitude(), GPS_GetLongitude() );							
				
				nPerLag = GPS_GetLatitude();
				nPerLog = GPS_GetLongitude();
				// 10秒小于500米,为正确数据
				if( nMeter < 500 ){
				
					AddTotalMileage( nMeter );
					AddCurrentMileage( nMeter );
				}
			}
		}
		else{
			nPerLag = 0;
			nPerLog = 0;
		}				
	}	
	return ;
}
////////////////////////////////////////////////////////////
//
double RadPIData(double dbData ){
	
	return dbData * PI / 180.0;
}
///////////////////////////////////////////////////////////////////////////
// 计算距离
//CalculateDistance(411919200,82972200,411923400,82961400) 距离:189	
unsigned int	CalculateDistance( int	nOldLag,  int	nOldLog,
								   int	nNewLag,  int	nNewLog ){
	double			dbData   = 0.0;
	double			dbOldLag = 0.0;
	double			dbOldLog = 0.0;
	double			dbNewLag = 0.0;
	double			dbNewLog = 0.0;
	
	// 是否定位
	if( GPS_IsPosValid() == 0){	 	
		return 0;
	}
		
	dbOldLag = RadPIData(nOldLag);
	dbOldLag /= 3600000; 
	
	dbNewLag = RadPIData(nNewLag);
	dbNewLag /= 3600000;
	
	dbOldLog = RadPIData(nOldLog);
	dbOldLog /= 3600000;
	
	dbNewLog = RadPIData(nNewLog);
	dbNewLog /= 3600000;
	
	dbData = dbOldLag - dbNewLag;
	dbOldLog = dbOldLog - dbNewLog;
	
	// dbData代做临时变量,存距离	
	dbData = 2 * asin( sqrt(pow(sin(dbData/2),2) +  
					   cos(dbOldLag) *
					   cos(dbNewLag) *
					   pow(sin(dbOldLog/2),2)));

	dbData = dbData * EARTH_RADIUS;	
	
	return dbData * 10000;

//	return 800;
}
/////////////////////////////////////////////////////////////
//	中控锁检测 
void LP_CheckCentralLocking( void ){

	#if( HW_ET201B || HW_ET201B_2 || HW_ET201B_3 )	
	static unsigned char	nLock = 0;
	static unsigned char	nUnLock = 0;	
	
	// 锁
	if( TE_IsNeedLockFlag() ){
		if( nLock++ == 0 ){
			HW_CENTER_LOOK_ACTION();
		}
		else{
			nLock = 0;
			HW_CENTER_LOOK_NACTION();
			TE_ClrNeedLockFlag();			
		}
	}
	//开锁
	if( TE_IsNeedUnLockFlag() ) {
		if( nUnLock++ == 0 ){
			HW_CENTER_UNLOOK_ACTION();  	
		}
		else{
			nUnLock = 0;
			TE_ClrNeedUnLockFlag();
			HW_CENTER_UNLOOK_NACTION();
		}
	}
	#endif	
}

/////////////////////////////////////////////////////////////
// 任务入口
// 系统时间 5ms为一步长
void HW_TaskLoopDetect(void *pData){
	
	unsigned char			nCnt = 0;
	unsigned char			nLang = 0;
	unsigned int			n24HRTimeCnt = 0;	
				
	TE_ClrNeedTEReset();	
		
	while( 1 ){
		OSTimeDlyHMSM(0,0,0,500);
				
		// 24小时软件硬件重启	
		if( ( n24HRTimeCnt < TIMEOUT24HR) && 
			( TE_IsNeedTEReset() == 0 ) ){		
			nLang = 1;	 	
			n24HRTimeCnt++;
			// 软件Watchdog
			SW_Watchdog( );			
		}
		else{
			// 24小时过后,主机会保存FLASH
			if( nLang == 1 ){
				nLang = 0;
				TE_SetNeedWriteFlash( );				
			}
		}
		//电压检测 
		#if( SW_PROTOCOLB )
			if( LP_ChkBattery() ||
				//底电压检测
				LP_ChkLowBattery() ||
				//超速报警检测
				LP_ChkOverspeedAlarm() ||
				//紧急报警按键检测
				LP_ChkSOSAlarm() ||
				//ACC检测
				LP_ChkAcc() ||
				// 托车报警
				LP_ChkTowCarAlarm()||
				// 防盗报警
				LP_ChkAntiStealAlarm() ||
				// 自动回传间隔
				LP_ChkAlwaysUpload() || 
				//  停车报警
				LP_ChkParkingAlarm() ||
				//  疲累驾驶
				LP_ChkTireDriver() ||
				//  电子围栏检测
				LP_ChkGEOFence() ||
				//  自定义1报警
				LP_ChkCustom1() ||
				//  自定义2报警
				LP_ChkCustom2() ||
				//  圆形出界报警
				LP_ChkLeaveRGEOFence() ||
				//  振动报警
				LP_ChkShocking() ||
				LP_ChkNoGPSAlarm() ||
				LP_ChkMMAAlarm()  ){
					TE_SetNeedUploadData();			
			}
		#else
			#if( SW_PROTOCOL_ASICO )
				if( LP_ChkBattery() ||
					//底电压检测
					LP_ChkLowBattery() ||
					//超速报警检测
					LP_ChkOverspeedAlarm() ||
					//紧急报警按键检测
					LP_ChkSOSAlarm() ||
					//ACC检测
					LP_ChkAcc() ||
					// 托车报警
					LP_ChkTowCarAlarm()||
					// 防盗报警
					LP_ChkAntiStealAlarm() ||
					// 自动回传间隔
					LP_ChkAlwaysUpload() || 
					//  停车报警
					LP_ChkParkingAlarm() ||
					//  疲累驾驶
					LP_ChkTireDriver() ||
					//  电子围栏检测
					LP_ChkGEOFence() ||
					//  自定义1报警
					LP_ChkCustom1() ||
					//  自定义2报警
					LP_ChkCustom2() ||
					//  圆形出界报警
					LP_ChkLeaveRGEOFence() ||
					//  振动报警
					LP_ChkShocking() ){
						TE_SetNeedUploadData();			
				}
			#else
				if( LP_ChkBattery() ||
					//底电压检测
					LP_ChkLowBattery() ||
					//超速报警检测
					LP_ChkOverspeedAlarm() ||
					//紧急报警按键检测
					LP_ChkSOSAlarm() ||
					//ACC检测
					LP_ChkAcc() ||
					// 托车报警
					LP_ChkTowCarAlarm()||
					// 防盗报警
					LP_ChkAntiStealAlarm() || 
					// 自动回传间隔
					LP_ChkAlwaysUpload() || 
					//  停车报警
					LP_ChkParkingAlarm() ){
						TE_SetNeedUploadData();			
				}	
			#endif
		#endif	
		// 油路检测
		if( GSM_IsOilState() ){
			HW_SetOilState();
		}
		else{
			HW_ClrOilState();
		}
		// 控制中控状态
		LP_CheckCentralLocking();
		// 计算里程	
		LP_ChkCalculateMileage();	
		// GPS定位后,三秒亮一次
		if(GPS_IsPosValid()){
	
			if( nCnt++ >= 6){				
				HW_GPSLED_ON();
				OSTimeDly(20);
				HW_GPSLED_OFF();
				nCnt = 0;
			}							
		}
		else{
			#if( HW_ET007B ) //ET007 GPS 模块长亮
			HW_GPSLED_ON();
			#else
			HW_GPSLED_OFF();
			#endif
		}
	}
	return;	
}


