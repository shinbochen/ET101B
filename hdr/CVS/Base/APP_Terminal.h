///////////////////////////////////////////////////////////////////////////
//   file name: app_Terminal.h
// description: Э��ͷ�ļ��ӿ�
//      author:
//        date: 2009-02-15
//     reamark: "

#ifndef APP_TERMINAL_H
#define APP_TERMINAL_H

#include "..\hdr\APP_GSM.h"

//=======================�궨����================================================================
#ifdef  JENNY_FRED
	#define   DEFAULT_PSD 					"8491"
#else
	#define   DEFAULT_PSD 					"1234"
#endif
#define	  DEFAULT_PHONE_EMPTY			""
#define   TEXT_POINT					'.'

//=====������ʼֵ����=============================================================================
#define 	DEFAULT_OVERSPEED		80		// ���ٱ�������ֵ
#define   	MIN_OVERSPEED			20		// �����ٶ�
#define		MINGEO_SETUP  			200		// ������С��

#define		PHONE_COMPARE_BIT		6

#define	    SHORT_MAXVALUE			32766

//====�ն˿���״̬=========================================================================================
enum{
	TE_CTRL_ENABLE = 0x00,
	TE_CTRL_DISABLE	
};

//=====У��ͱ�־==================================================================================
enum{
	CHKSUM_OK     = 0x00,
	CHKSUM_ERROR
};

////////////////////////////////////////////////////////////////////////////////////////
//  ���ر�־
enum{
	RESULT_ERROR = 0x00,
	RESULT_OK,
	RESULT_EMPTY,
	RESULT_NOT_AUTHORIZATION
};

////////////////////////////////////////////////////////////////////////////////////////
// �����Զ�������
enum{
	SETUP_CUSTOM1 = 0x01,
	SETUP_CUSTOM2 = 0x02,
	SETUP_CUSTOM3 = 0x04,
	SETUP_CUSTOM4 = 0x08,
	SETUP_ANTISTEAL = 0x10,  //0:�ż����� 1:ԭ�������� 
	SETUP_ACC_IDLE  = 0x20	 //ACC����״̬���ϴ�����
};

////////////////////////////////////////////////////////////////////////////////////////
// ��չ��������
enum{

	HW_AUTOFUEL = 0x01
};

/////////////////////////////////////////////////////////////////////////////////////////////
//  ����Χ��
typedef struct _STFENCESETUP{
	// bit7: 0/1	���/����
	// bit6: 0/1    ɾ��/���� 
    // bit5: 0/1	(Ϊ��J����ǰ�İ汾)
	unsigned char		m_nFlag;
	// 2010-04-07
	unsigned char		m_nGEOFence; //�Ƿ�Խ�籨��
	int					m_RLat;      //���Ͻ�
	int					m_RLng;      //���Ͻ�
	int					m_LLat;      //���½�
	int					m_LLng;      //���½�
}STFENCESETUP,*PSTFENCESETUP;
     
////////////////////////////////////////////////////////////////////////////////////////
//	��������
typedef struct _STALARMSETUP {

	// bit0:	0/1   �ż����/���ż����
	unsigned char		m_nPara;		   	  //��չ������������
	unsigned char		m_nDefence;			  //ACC��ʱ����Ϊ��� ��λ����	 
	unsigned char		m_nILLDoorOpen;		  //�Ƿ��������� ��λ����	
	unsigned char 		m_nTireDriver;		  //ƣ�ͼ�ʻ����
	unsigned short		m_nOverspeed;		  //���ٱ��� ��λ����		
	unsigned short		m_nParking;			  //ͣ������ 		��λ����			
	unsigned int		m_nAlarmEnable;		  //�������� 	
	unsigned int		m_nHWValid;			  //�ն�֧��Ӳ������
	unsigned int		m_nALValid;			  //�ն�֧�ֱ�������

	unsigned int		m_nDriveTime;		  //��¼��������ʱ��
	unsigned int 		m_nParkingCnt;		  //��¼����ͣ��ʱ��
	
	unsigned int		m_nACCIdle;			  //ACC�ؿ���ʱ�� ��λ����			
	unsigned int		m_nNoGPS;			  //GPSδ��λʱ�� ��λ����
	
	//
	unsigned int		m_nTotalMileage;	  //�����
	unsigned int		m_nCurMileage;		  //��ǰ���
	unsigned int		m_nMaxMileage;		  //������
	
	// ����Χ������
	STFENCESETUP		m_oStGFence[2];	       
}STALARMSETUP,*PSTALARMSETUP;


////////////////////////////////////////////////////////////////////////////////////////
// �Զ���������
typedef struct _STCUSTOMSETUP{
	//bit0:  ���õ�ƽ�Զ�����1
	//bit1:  ���õ�ƽ�Զ�����2
	//bit2:  ���õ�ƽ�Զ�����3
	//bit3:  ���õ�ƽ�Զ�����4
	//bit4:  ���÷�����ƽ״̬ 0:�ż�  1:ԭ������
	//bit5:  ����ACC����ģʽ�²��ϴ�GPRS���� 0: �ϴ� 1:���ϴ�
	unsigned char		m_nCustomPos;    	  				  //�Զ���λ
	//bit0:  �����򱨾�
	//bit1:  �����򱨾�
	//bit2:  0:�ż����� 1:ԭ����������
	unsigned char		m_nGeoState;						  //״̬λ 	
	unsigned char		m_nCustom1[TELNUMBER_LENGTH+1];		  //�Զ�����1	
	unsigned char		m_nCustom2[TELNUMBER_LENGTH+1];		  //�Զ�����2
	unsigned char		m_nCustom3[TELNUMBER_LENGTH+1];       //�Զ�����3
	unsigned char		m_nCustom4[TELNUMBER_LENGTH+1];       //�Զ�����4
	unsigned short		m_nInGeo;							  //�����뱨��	
	unsigned short		m_nDistance;						  //�����뱨��	
	int					m_nInLog;			//����
	int					m_nInLag;			//γ��
	int					m_nCLng;			//����	
	int					m_nCLat;			//γ��
}STCUSTOMSETUP,*PSTCUSTOMSETUP;

////////////////////////////////////////////////////////////////////////////////////////
//  �ն��豸״̬
#define		GPRS_LOGIN_FLAG				0x01
#define		NEED_WRITE_FLASH			0x02
#define		NEED_RESET_FLAG				0x04
#define		NEED_UPLOADDATA				0x08
#define     NEED_LOCK_FLAG				0x10
#define     NEED_UNLOCK_FLAG			0x20
#define		NEED_RESULT_GG_ADDR			0x40
#define		NEED_QUERY_GG_ADDR			0x80
 
////////////////////////////////////////////////////////////////////////////////////////
//  ����״̬
enum{
	//==byte 1===============================================================
	//  ��������
	AL_SOS 		 		 = 0x01,
	//  ���ٱ���
	AL_OVERSPEED 		 = 0x02,
	//  ͣ������
	AL_PARKING     	 	 = 0x04,
	//	�г�����
	AL_TOW			 	 = 0x08,
	//	��籨��
	AL_ENT_FENCING		 = 0x10,
	//	���籨��
	AL_LEAVOMG_FENCING 	 = 0x20,
	//  ����Դ�ϵ籨��
	AL_POWER_OFF		 = 0x40,
	//	�͵�ѹ����
	AL_LOW_POWER		 = 0x80,
	//==byte 2===============================================================
	//  GPS��·����
	AL_GPS_OPEN			 = 0x100,
	//  GPS��·����
	AL_GPS_SHORT		 = 0x200,
	//  �Ƿ����ű���(�ڲ���֮���ų�Ϊ�Ƿ����� Ҳ�е���)	
	AL_NOT_DOOR		 	 = 0x400,
	//  �Ƿ���𱨾�(�ڲ���֮�����Ϊ�Ƿ����)
	AL_NOT_IGNITION		 = 0x800,
	//  �Զ��屨��1
	AL_CUSTOM1			 = 0x1000,
	//  �Զ��屨��2
	AL_CUSTOM2			 = 0x2000,
	//  �Զ��屨��3
	AL_CUSTOM3			 = 0x4000,
	//  �Զ��屨��4
	AL_CUSTOM4			 = 0x8000,
	//==byte 3===============================================================
	//  ƣ�ͼ�ʻ����
	AL_TIRE_DRIVE		 = 0x10000,
	//  ��·����
	AL_GAS				 = 0x20000,
	//  ACC�ر���
	AL_ACC_ALARM		 = 0x40000,
	//  GPSδ��λ����
	AL_NOGPS_ALARM		 = 0x80000
};

//  Ӳ��״̬
enum{
	//==byte 1===============================================================
	//  1/0 ACC��/��
	HW_ACC_STATE   		 = 0x01,
	//  1/0 ���ſ�/��
	HW_DOOR_STATE		 = 0x02,
	//  1/0 ����Դ����/��ع���
	HW_POWER_STATE		 = 0x04,
	//  1/0 ���õ�Դ�͵�ѹ/���õ�Դ����
	HW_BRK_POWER_STATE	 = 0x08,
	//  1/0 ��·��ͨ/�Ͽ�
	HW_OIL_STATE		 = 0x10,
	//  1/0 �������ذ���/���������ɿ�
	HW_SOS_STATE		 = 0x20,
	//  1/0 �Ѳ���/δ����
	HW_ANTI_STEAL_STATE  = 0x40,
	//  1/0 ����������/������δ����
	HW_ENGINE_STATE		= 0x80,
	//==byte 2===============================================================
	//  �Զ��屨���߸ߵ�ƽ/�Զ��屨���ߵ͵�ƽ
	HW_CUSTOM1_STATE   	= 0x100,
	//  �Զ��屨���߸ߵ�ƽ/�Զ��屨���ߵ͵�ƽ
	HW_CUSTOM2_STATE    = 0x200,
	//  �Զ��屨���߸ߵ�ƽ/�Զ��屨���ߵ͵�ƽ
	HW_CUSTOM3_STATE    = 0x400,
	//  �Զ��屨���߸ߵ�ƽ/�Զ��屨���ߵ͵�ƽ
	HW_CUSTOM4_STATE    = 0x800,
	//  1/0  �س�/�ճ�
	HW_EMTPYOROVERLOAD_STATE = 0x1000,
	// 1/0 ��Ӫ״̬/����Ӫ״̬
	HW_ONDUTY_STATE		= 0x2000,
	// 1/0 ���������/������
	HW_LEDPANEL_STATE	= 0x4000,
	// 1/0 �ֱ�����/������
	HW_HANDSET_STATE	= 0x8000,
	//==byte 3===============================================================
	// BIT0~1:	GPS�����ź�ǿ�� 0~3����0��Ϊ���ź�
	HW_GPS_SIGNAL1	   = 0x10000,
	HW_GPS_SIGNAL2     = 0x20000,
	// BIT2~3   GSM�����ź�ǿ��0~3����0��Ϊ���ź� 
	HW_GSM_SIGNAL1     = 0x40000,
	HW_GSM_SIGNAL2     = 0x80000
};

typedef struct _STDEVICESTATE{
	unsigned char	 	m_nFlag;			//GPRS�Ƿ��½�ɹ�
	unsigned char	 	m_nCodeState;		//ִ��״̬
	unsigned int    	m_nHWState;			//Ӳ��״̬
	unsigned int	 	m_nALState;			//����״̬
	unsigned int		m_nBattery;			//��ص�ѹ
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
//  ���п���
#define     TE_IsNeedLockFlag()         (g_stDeviceState.m_nFlag & NEED_LOCK_FLAG)
#define     TE_SetNeedLockFlag()        (g_stDeviceState.m_nFlag |= NEED_LOCK_FLAG)
#define     TE_ClrNeedLockFlag()        (g_stDeviceState.m_nFlag &= ~NEED_LOCK_FLAG)
//////////////////////////////////////////////////////////////////////////////////////
//  �����п���
#define     TE_IsNeedUnLockFlag()       (g_stDeviceState.m_nFlag & NEED_UNLOCK_FLAG)
#define     TE_SetNeedUnLockFlag()      (g_stDeviceState.m_nFlag |= NEED_UNLOCK_FLAG)
#define     TE_ClrNeedUnLockFlag()      (g_stDeviceState.m_nFlag &= ~NEED_UNLOCK_FLAG)

////////////////////////////////////////////////////////////////////////////////////////
//  ���ص�ַ
#define		TE_IsResultGGAddrFlag()		(g_stDeviceState.m_nFlag & NEED_RESULT_GG_ADDR)
#define		TE_SetResultGGAddrFlag()	(g_stDeviceState.m_nFlag |= NEED_RESULT_GG_ADDR)
#define		TE_ClrResultGGAddrFlag() 	(g_stDeviceState.m_nFlag &= ~NEED_RESULT_GG_ADDR)

////////////////////////////////////////////////////////////////////////////////////////
//  ��ѯ��ַ
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
//  ���ٱ���
#define		AL_IsOverspeed( )		 (g_stDeviceState.m_nALState &   AL_OVERSPEED  )
#define		AL_SetOverspeed( )		 (g_stDeviceState.m_nALState |=  AL_OVERSPEED )
#define		AL_ClrOverspeed( )		 (g_stDeviceState.m_nALState &= ~AL_OVERSPEED )
//  ͣ������
#define		AL_IsParking( )			 (g_stDeviceState.m_nALState &  AL_PARKING ) 
#define		AL_SetParking( )		 (g_stDeviceState.m_nALState |= AL_PARKING ) 	   
#define		AL_ClrParking( )		 (g_stDeviceState.m_nALState &=~AL_PARKING ) 
//	�г�����
#define     AL_IsTow( )			 	 (g_stDeviceState.m_nALState &  AL_TOW ) 
#define		AL_SetTow( )			 (g_stDeviceState.m_nALState |= AL_TOW ) 
#define		AL_ClrTow( )			 (g_stDeviceState.m_nALState &=~AL_TOW ) 
//	��籨�� 
#define		AL_IsEnterGEOFence( )		 	 (g_stDeviceState.m_nALState &  AL_ENT_FENCING ) 
#define		AL_SetEnterGEOFence( )		 (g_stDeviceState.m_nALState |= AL_ENT_FENCING ) 	
#define		AL_ClrEnterGEOFence( )		 (g_stDeviceState.m_nALState &=~AL_ENT_FENCING )
//	���籨��
#define		AL_IsLeaveGEOFence( )   (g_stDeviceState.m_nALState  &  AL_LEAVOMG_FENCING ) 
#define		AL_SetLeaveGEOFence( )  (g_stDeviceState.m_nALState  |= AL_LEAVOMG_FENCING ) 
#define		AL_ClrLeaveGEOFence( )  (g_stDeviceState.m_nALState  &=~AL_LEAVOMG_FENCING ) 
//  ����Դ�ϵ籨��
#define		AL_IsPowerOff( )		 (g_stDeviceState.m_nALState  &   AL_POWER_OFF )	
#define		AL_SetPowerOff( )		 (g_stDeviceState.m_nALState  |=  AL_POWER_OFF )
#define		AL_ClrPowerOff( )		 (g_stDeviceState.m_nALState  &= ~AL_POWER_OFF )
//	�͵�ѹ����
#define		AL_IsLowPower( )		 (g_stDeviceState.m_nALState  &   AL_LOW_POWER )	
#define		AL_SetLowPower( )		 (g_stDeviceState.m_nALState  |=  AL_LOW_POWER )	
#define		AL_ClrLowPower( )		 (g_stDeviceState.m_nALState  &= ~AL_LOW_POWER )	
//==byte 2===============================================================
//  GPS��·����
#define		AL_IsGpsOpen( )			 (g_stDeviceState.m_nALState  &   AL_GPS_OPEN )			
#define	    AL_SetGpsOpen( )		 (g_stDeviceState.m_nALState  |=  AL_GPS_OPEN )			
#define	    AL_ClrGpsOpen( )		 (g_stDeviceState.m_nALState  &= ~AL_GPS_OPEN )
//  GPS��·����
#define		AL_IsGpsShort( )	     (g_stDeviceState.m_nALState  &   AL_GPS_SHORT )	
#define		AL_SetGpsShort( )	     (g_stDeviceState.m_nALState  |=  AL_GPS_SHORT )	
#define		AL_ClrGpsShort( )	     (g_stDeviceState.m_nALState  &= ~AL_GPS_SHORT )	
//  �Ƿ����ű���(�ڲ���֮���ų�Ϊ�Ƿ����� Ҳ�е���)	
#define		AL_IsNotDoor( )		     (g_stDeviceState.m_nALState  &   AL_NOT_DOOR )		
#define		AL_SetNotDoor( )		 (g_stDeviceState.m_nALState  |=  AL_NOT_DOOR )		
#define		AL_ClrNotDoor( )		 (g_stDeviceState.m_nALState  &= ~AL_NOT_DOOR )		
//  �Ƿ���𱨾�(�ڲ���֮�����Ϊ�Ƿ����)
#define		AL_IsNotIgnition( )		 (g_stDeviceState.m_nALState  &   AL_NOT_IGNITION )	
#define		AL_SetNotIgnition( )	 (g_stDeviceState.m_nALState  |=  AL_NOT_IGNITION )	
#define		AL_ClrNotIgnition( )	 (g_stDeviceState.m_nALState  &= ~AL_NOT_IGNITION )	
//  �Զ��屨��1
#define		AL_IsCustom1( )			 (g_stDeviceState.m_nALState  &   AL_CUSTOM1 )			
#define		AL_SetCustom1( )		 (g_stDeviceState.m_nALState  |=  AL_CUSTOM1 )
#define		AL_ClrCustom1( )		 (g_stDeviceState.m_nALState  &= ~AL_CUSTOM1 )
//  �Զ��屨��2
#define		AL_IsCustom2( )			 (g_stDeviceState.m_nALState  &   AL_CUSTOM2 )			
#define		AL_SetCustom2( )		 (g_stDeviceState.m_nALState  |=  AL_CUSTOM2 )
#define		AL_ClrCustom2( )		 (g_stDeviceState.m_nALState  &= ~AL_CUSTOM2 )
//  �Զ��屨��3
#define		AL_IsCustom3( )			 (g_stDeviceState.m_nALState  &   AL_CUSTOM3 )			
#define		AL_SetCustom3( )		 (g_stDeviceState.m_nALState  |=  AL_CUSTOM3 )
#define		AL_ClrCustom3( )		 (g_stDeviceState.m_nALState  &= ~AL_CUSTOM3 )
//  �Զ��屨��4
#define		AL_IsCustom4( )			 (g_stDeviceState.m_nALState  &   AL_CUSTOM4 )			
#define		AL_SetCustom4( )		 (g_stDeviceState.m_nALState  |=  AL_CUSTOM4 )
#define		AL_ClrCustom4( )		 (g_stDeviceState.m_nALState  &= ~AL_CUSTOM4 )	
//==byte 3===============================================================
//  ƣ�ͼ�ʻ����
#define		AL_IsTireDrive( )		 (g_stDeviceState.m_nALState  &   AL_TIRE_DRIVE )					
#define		AL_SetTireDrive( )		 (g_stDeviceState.m_nALState  |=  AL_TIRE_DRIVE )					
#define		AL_ClrTireDrive( )		 (g_stDeviceState.m_nALState  &= ~AL_TIRE_DRIVE )
//  ACC�ر���
#define		AL_IsACCAlarm()		 	 (g_stDeviceState.m_nALState &   AL_ACC_ALARM )	
#define		AL_SetACCAlarm( )	 	 (g_stDeviceState.m_nALState |=  AL_ACC_ALARM )
#define		AL_ClrACCAlarm( )	 	 (g_stDeviceState.m_nALState &= ~AL_ACC_ALARM )	
//  GPSδ��λ����
#define		AL_IsNoGPSAlarm()	 	 (g_stDeviceState.m_nALState &   AL_NOGPS_ALARM )	
#define		AL_SetNoGPSAlarm( )	 	 (g_stDeviceState.m_nALState |=  AL_NOGPS_ALARM )
#define		AL_ClrNoGPSAlarm( )	 	 (g_stDeviceState.m_nALState &= ~AL_NOGPS_ALARM )	
					

//////////////////////////////////////////////////////////////////////////////
//  ����λ�Ƿ���
//  ��������
#define  	AL_IsEnableSOS( )			(g_stAlarmSetup.m_nAlarmEnable &  AL_SOS )
#define		AL_EnableSOS( )			 	(g_stAlarmSetup.m_nAlarmEnable |= AL_SOS )
#define		AL_DisableSOS( )			(g_stAlarmSetup.m_nAlarmEnable &=~AL_SOS )
//  ���ٱ���
#define		AL_IsEnableOverSpeed( )		(g_stAlarmSetup.m_nAlarmEnable &  AL_OVERSPEED )  	
#define		AL_EnableOverSpeed( )	 	(g_stAlarmSetup.m_nAlarmEnable |= AL_OVERSPEED )  
#define		AL_DisableOverSpeed( )	 	(g_stAlarmSetup.m_nAlarmEnable &=~AL_OVERSPEED )  
//  ͣ������
#define		AL_IsEnableParking( )		(g_stAlarmSetup.m_nAlarmEnable &  AL_PARKING ) 		
#define		AL_EnableParking( )		 	(g_stAlarmSetup.m_nAlarmEnable |= AL_PARKING )
#define		AL_DisableParking( )		(g_stAlarmSetup.m_nAlarmEnable &=~AL_PARKING )
//	�г�����
#define		AL_IsEnableTow( )		 	(g_stAlarmSetup.m_nAlarmEnable &  AL_TOW )
#define		AL_EnableTow( )		 		(g_stAlarmSetup.m_nAlarmEnable |= AL_TOW )
#define		AL_DisableEnTow( )		 	(g_stAlarmSetup.m_nAlarmEnable &=~AL_TOW )
//  ����Դ�ϵ籨��
#define		AL_IsEnablePowerOff( )		 (g_stAlarmSetup.m_nAlarmEnable &  AL_POWER_OFF ) 
#define		AL_EnablePowerOff( )		 (g_stAlarmSetup.m_nAlarmEnable |= AL_POWER_OFF ) 
#define		AL_DisablePowerOff( )		 (g_stAlarmSetup.m_nAlarmEnable &=~AL_POWER_OFF ) 
//	�͵�ѹ����
#define		AL_IsEnableLowPower( )		 (g_stAlarmSetup.m_nAlarmEnable &  AL_LOW_POWER )
#define		AL_EnableLowPower( )		 (g_stAlarmSetup.m_nAlarmEnable |= AL_LOW_POWER )
#define		AL_DisableLowPower( )		 (g_stAlarmSetup.m_nAlarmEnable &=~AL_LOW_POWER )
//==byte 2===============================================================
//  GPS��·����
#define		AL_IsEnableGpsOpen( )		 (g_stAlarmSetup.m_nAlarmEnable &  AL_GPS_OPEN )
#define		AL_EnableGpsOpen( )		 	 (g_stAlarmSetup.m_nAlarmEnable |= AL_GPS_OPEN )
#define		AL_DisableGpsOpen( )		 (g_stAlarmSetup.m_nAlarmEnable &=~AL_GPS_OPEN )
//  GPS��·����
#define		AL_IsEnableGpsShort( )	 	 (g_stAlarmSetup.m_nAlarmEnable &  AL_GPS_SHORT )
#define		AL_EnableGpsShort( )	 	 (g_stAlarmSetup.m_nAlarmEnable |= AL_GPS_SHORT )
#define		AL_DisableGpsShort( )	 	 (g_stAlarmSetup.m_nAlarmEnable &=~AL_GPS_SHORT )
//  �Ƿ����ű���(�ڲ���֮���ų�Ϊ�Ƿ����� Ҳ�е���)	
#define		AL_IsEnableNotDoor( )		 (g_stAlarmSetup.m_nAlarmEnable &  AL_NOT_DOOR )
#define		AL_EnableNotDoor( )		 	 (g_stAlarmSetup.m_nAlarmEnable |= AL_NOT_DOOR )
#define		AL_DisableNotDoor( )		 (g_stAlarmSetup.m_nAlarmEnable &=~AL_NOT_DOOR )
//  �Ƿ���𱨾�(�ڲ���֮�����Ϊ�Ƿ����)
#define		AL_IsEnableNotIgnition( ) 	 (g_stAlarmSetup.m_nAlarmEnable &  AL_NOT_IGNITION )
#define		AL_EnableNotIgnition( ) 	 (g_stAlarmSetup.m_nAlarmEnable |= AL_NOT_IGNITION )
#define		AL_DisableNotIgnition( ) 	 (g_stAlarmSetup.m_nAlarmEnable &=~AL_NOT_IGNITION )
//  �Զ��屨��1
#define		AL_IsEnableCustom1( )		 (g_stAlarmSetup.m_nAlarmEnable &  AL_CUSTOM1 )	
#define		AL_EnableCustom1( )		 	 (g_stAlarmSetup.m_nAlarmEnable |= AL_CUSTOM1 )	
#define		AL_DisableCustom1( )		 (g_stAlarmSetup.m_nAlarmEnable &=~AL_CUSTOM1 )	
//  �Զ��屨��2
#define		AL_IsEnableCustom2( )		 (g_stAlarmSetup.m_nAlarmEnable &  AL_CUSTOM2 )	
#define		AL_EnableCustom2( )		 	 (g_stAlarmSetup.m_nAlarmEnable |= AL_CUSTOM2 )	
#define		AL_DisableCustom2( )		 (g_stAlarmSetup.m_nAlarmEnable &=~AL_CUSTOM2 )	
//  �Զ��屨��3
#define		AL_IsEnableCustom3( )		 (g_stAlarmSetup.m_nAlarmEnable &  AL_CUSTOM3 )	
#define		AL_EnableCustom3( )		 	 (g_stAlarmSetup.m_nAlarmEnable |= AL_CUSTOM3 )	
#define		AL_DisableCustom3( )		 (g_stAlarmSetup.m_nAlarmEnable &=~AL_CUSTOM3 )	
//  �Զ��屨��4
#define		AL_IsEnableCustom4( )		 (g_stAlarmSetup.m_nAlarmEnable &  AL_CUSTOM4 )	
#define		AL_EnableCustom4( )		 	 (g_stAlarmSetup.m_nAlarmEnable |= AL_CUSTOM4 )	
#define		AL_DisableCustom4( )		 (g_stAlarmSetup.m_nAlarmEnable &=~AL_CUSTOM4 )	
//==byte 3===============================================================
//  ƣ�ͼ�ʻ����
#define		AL_IsEnableTireDrive( )		 (g_stAlarmSetup.m_nAlarmEnable &  AL_TIRE_DRIVE )	
#define		AL_EnableTireDrive( )	 	 (g_stAlarmSetup.m_nAlarmEnable |= AL_TIRE_DRIVE )
#define		AL_DisableTireDrive( )	 	 (g_stAlarmSetup.m_nAlarmEnable &=~AL_TIRE_DRIVE )
//  ACC�ر���
#define		AL_IsEnableACCAlarm()		 (g_stAlarmSetup.m_nAlarmEnable &  AL_ACC_ALARM )	
#define		AL_EnableACCAlarm( )	 	 (g_stAlarmSetup.m_nAlarmEnable |= AL_ACC_ALARM )
#define		AL_DisableACCAlarm( )	 	 (g_stAlarmSetup.m_nAlarmEnable &=~AL_ACC_ALARM )	
//  GPSδ��λ����
#define		AL_IsEnableNoGPSAlarm()		 (g_stAlarmSetup.m_nAlarmEnable &  AL_NOGPS_ALARM )	
#define		AL_EnableNoGPSAlarm( )	 	 (g_stAlarmSetup.m_nAlarmEnable |= AL_NOGPS_ALARM )
#define		AL_DisableNoGPSAlarm( )	 	 (g_stAlarmSetup.m_nAlarmEnable &=~AL_NOGPS_ALARM )	

//  �ż촥������,�Ƿ����
#define		HW_IsEnableAutoFuel()		 ( g_stAlarmSetup.m_nPara & HW_AUTOFUEL )
#define		HW_EnableAutoFuel()          (g_stAlarmSetup.m_nPara |= HW_AUTOFUEL )
#define		HW_DisableAutoFuel()		 (g_stAlarmSetup.m_nPara &= ~HW_AUTOFUEL )


///////////////////�ն�Ӳ��״̬////////////////////////////////////////
//  1/0 ACC��/��
#define  HW_IsAccState( )		 ( g_stDeviceState.m_nHWState &   HW_ACC_STATE )
#define  HW_SetAccState( )		 ( g_stDeviceState.m_nHWState |=  HW_ACC_STATE )  
#define  HW_ClrAccState( )		 ( g_stDeviceState.m_nHWState &= ~HW_ACC_STATE )
//  1/0 ���ſ�/��
#define  HW_IsDoorState( )		 ( g_stDeviceState.m_nHWState &   HW_DOOR_STATE )	
#define  HW_SetDoorState( )		 ( g_stDeviceState.m_nHWState |=  HW_DOOR_STATE )
#define  HW_ClrDoorState( )		 ( g_stDeviceState.m_nHWState &= ~HW_DOOR_STATE )
//  1/0 ����Դ����/��ع���
#define  HW_IsPowerState( )		 ( g_stDeviceState.m_nHWState &   HW_POWER_STATE )	
#define  HW_SetPowerState( )	 ( g_stDeviceState.m_nHWState |=  HW_POWER_STATE )
#define  HW_ClrPowerState( )	 ( g_stDeviceState.m_nHWState &= ~HW_POWER_STATE )
//  1/0 ���õ�Դ�͵�ѹ/���õ�Դ����
#define  HW_IsBrkPowerState( )   ( g_stDeviceState.m_nHWState &   HW_BRK_POWER_STATE )		
#define  HW_SetBrkPowerState( )  ( g_stDeviceState.m_nHWState |=  HW_BRK_POWER_STATE )
#define  HW_ClrBrkPowerState( )  ( g_stDeviceState.m_nHWState &= ~HW_BRK_POWER_STATE )
//  1/0 ��·��ͨ/�Ͽ�
#define  HW_IsOilState( )		 ( g_stDeviceState.m_nHWState &   HW_OIL_STATE )	
#define  HW_SetOilState( )		 ( g_stDeviceState.m_nHWState |=  HW_OIL_STATE )	
#define  HW_ClrOilState( )		 ( g_stDeviceState.m_nHWState &= ~HW_OIL_STATE )	
//  1/0 �������ذ���/���������ɿ�
#define  HW_IsSosState( )		 ( g_stDeviceState.m_nHWState &   HW_SOS_STATE )		
#define  HW_SetSosState( )		 ( g_stDeviceState.m_nHWState |=  HW_SOS_STATE )
#define  HW_ClrSosState( )		 ( g_stDeviceState.m_nHWState &= ~HW_SOS_STATE )
//  1/0 �Ѳ���/δ����
#define  HW_IsAntiStealState( )  ( g_stDeviceState.m_nHWState &   HW_ANTI_STEAL_STATE )		  
#define  HW_SetAntiStealState( ) ( g_stDeviceState.m_nHWState |=  HW_ANTI_STEAL_STATE )
#define  HW_ClrAntiStealState( ) ( g_stDeviceState.m_nHWState &= ~HW_ANTI_STEAL_STATE )
//  1/0 ����������/������δ����
#define  HW_IsEngineState( )     ( g_stDeviceState.m_nHWState &   HW_ENGINE_STATE )	
#define  HW_SetEngineState( )    ( g_stDeviceState.m_nHWState |=  HW_ENGINE_STATE )
#define  HW_ClrEngineState( )    ( g_stDeviceState.m_nHWState &= ~HW_ENGINE_STATE )
//  �Զ��屨���߸ߵ�ƽ/�Զ��屨���ߵ͵�ƽ
#define	 HW_IsCustom1State( )	 ( g_stDeviceState.m_nHWState &   HW_CUSTOM1_STATE )			
#define	 HW_SetCustom1State( )	 ( g_stDeviceState.m_nHWState |=  HW_CUSTOM1_STATE )
#define	 HW_ClrCustom1State( )	 ( g_stDeviceState.m_nHWState &= ~HW_CUSTOM1_STATE )
//  �Զ��屨���߸ߵ�ƽ/�Զ��屨���ߵ͵�ƽ
#define	 HW_IsCustom2State( )	 ( g_stDeviceState.m_nHWState &   HW_CUSTOM2_STATE )			
#define	 HW_SetCustom2State( )	 ( g_stDeviceState.m_nHWState |=  HW_CUSTOM2_STATE )
#define	 HW_ClrCustom21State( )	 ( g_stDeviceState.m_nHWState &= ~HW_CUSTOM2_STATE )
//  �Զ��屨���߸ߵ�ƽ/�Զ��屨���ߵ͵�ƽ
#define	 HW_IsCustom2State( )	 ( g_stDeviceState.m_nHWState &   HW_CUSTOM2_STATE )			
#define	 HW_SetCustom2State( )	 ( g_stDeviceState.m_nHWState |=  HW_CUSTOM2_STATE )
#define	 HW_ClrCustom2State( )	 ( g_stDeviceState.m_nHWState &= ~HW_CUSTOM2_STATE )
//  �Զ��屨���߸ߵ�ƽ/�Զ��屨���ߵ͵�ƽ
#define	 HW_IsCustom3State( )	 ( g_stDeviceState.m_nHWState &   HW_CUSTOM3_STATE )			
#define	 HW_SetCustom3State( )	 ( g_stDeviceState.m_nHWState |=  HW_CUSTOM3_STATE )
#define	 HW_ClrCustom3State( )	 ( g_stDeviceState.m_nHWState &= ~HW_CUSTOM3_STATE )
//  �Զ��屨���߸ߵ�ƽ/�Զ��屨���ߵ͵�ƽ
#define	 HW_IsCustom4State( )	 ( g_stDeviceState.m_nHWState &   HW_CUSTOM4_STATE )			
#define	 HW_SetCustom4State( )	 ( g_stDeviceState.m_nHWState |=  HW_CUSTOM4_STATE )
#define	 HW_ClrCustom4State( )	 ( g_stDeviceState.m_nHWState &= ~HW_CUSTOM4_STATE )
//  1/0  �س�/�ճ�
#define	 HW_IsEmptyOrOverload( ) ( g_stDeviceState.m_nHWState &   HW_EMTPYOROVERLOAD_STATE )		
#define	 HW_SetEmptyOrOverload( )( g_stDeviceState.m_nHWState |=  HW_EMTPYOROVERLOAD_STATE )		
#define	 HW_ClrEmptyOrOverload( )( g_stDeviceState.m_nHWState &= ~HW_EMTPYOROVERLOAD_STATE )		
// 1/0 ��Ӫ״̬/����Ӫ״̬
#define	 HW_IsOndutyState( )	 ( g_stDeviceState.m_nHWState &   HW_ONDUTY_STATE )		
#define	 HW_SetOndutyState( )	 ( g_stDeviceState.m_nHWState |=  HW_ONDUTY_STATE )		
#define	 HW_ClrOndutyState( )	 ( g_stDeviceState.m_nHWState &= ~HW_ONDUTY_STATE )		
// 1/0 ���������/������
#define	 HW_IsLedpanelState( )   ( g_stDeviceState.m_nHWState &   HW_LEDPANEL_STATE )		
#define	 HW_SetLedpanelState( )  ( g_stDeviceState.m_nHWState |=  HW_LEDPANEL_STATE )
#define	 HW_ClrLedpanelState( )  ( g_stDeviceState.m_nHWState &= ~HW_LEDPANEL_STATE )
// 1/0 �ֱ�����/������
#define	 HW_IsHandsetState( )	 ( g_stDeviceState.m_nHWState &   HW_HANDSET_STATE )		
#define	 HW_SetHandsetState( )	 ( g_stDeviceState.m_nHWState |=  HW_HANDSET_STATE )
#define	 HW_ClrHandsetState( )	 ( g_stDeviceState.m_nHWState &= ~HW_HANDSET_STATE )

//==============����������===============================================================
extern STALARMSETUP		g_stAlarmSetup;
extern STDEVICESTATE	g_stDeviceState;
extern STCUSTOMSETUP    g_stCustomSetup;

////////////////////////////////////////////////////////////////////////////////////////
// �����򱨾�
#define  TE_IsEnableOutGeo( )	(g_stCustomSetup.m_nGeoState &  SETUP_CUSTOM1)
#define  TE_EnableOutGeo( ) 	(g_stCustomSetup.m_nGeoState |= SETUP_CUSTOM1) 	
#define  TE_DisableOutGeo( )	(g_stCustomSetup.m_nGeoState &= ~SETUP_CUSTOM1)
 	
// �����򱨾�
#define  TE_IsEnableInGeo( )	(g_stCustomSetup.m_nGeoState  & SETUP_CUSTOM2) 	
#define	 TE_EnableInGeo( )   	(g_stCustomSetup.m_nGeoState |= SETUP_CUSTOM2) 	
#define	 TE_DisableInGeo( )  	(g_stCustomSetup.m_nGeoState &= ~SETUP_CUSTOM2) 	

// 0:�ż����� 1:ԭ����������
#define  TE_IsAntiSteal( )      (g_stCustomSetup.m_nGeoState  & SETUP_CUSTOM3)
#define  TE_SetAntiSteal( )     (g_stCustomSetup.m_nGeoState |= SETUP_CUSTOM3) 	
#define  TE_ClrAntiSteal( )     (g_stCustomSetup.m_nGeoState &= ~SETUP_CUSTOM3) 	


////////////////////////////////////////////////////////////////////////////////////////
//  ���õ�ƽ���
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
// ���÷������  0:�ż�����  1:ԭ����������
#define  TE_SetASIHTrigger( ) ( g_stCustomSetup.m_nCustomPos |= SETUP_ANTISTEAL ) 
#define  TE_SetASILTrigger( ) ( g_stCustomSetup.m_nCustomPos &= ~SETUP_ANTISTEAL ) 
#define  TE_GetAntisteal( )   ((g_stCustomSetup.m_nCustomPos & SETUP_ANTISTEAL)?1:0 )

// ����ACC����ģʽ�²��ϴ�GPRS���� 0: �ϴ� 1:���ϴ�
#define  TE_IsIdleGPRS( )   ( g_stCustomSetup.m_nCustomPos & SETUP_ACC_IDLE )
#define  TE_SetIdleGPRS( )  ( g_stCustomSetup.m_nCustomPos |= SETUP_ACC_IDLE ) 
#define  TE_ClrIdleGPRS( )  ( g_stCustomSetup.m_nCustomPos &= ~SETUP_ACC_IDLE ) 


// ��¼���������(��)
#define	 AddTotalMileage( nMeter )  (g_stAlarmSetup.m_nTotalMileage += (nMeter) )
#define	 GetTotalMileage()  		 (g_stAlarmSetup.m_nTotalMileage)

// ���õ�ǰ���(��)
#define	 AddCurrentMileage( nMeter ) (g_stAlarmSetup.m_nCurMileage += (nMeter) )
#define	 GetCurrentMileage()		 (g_stAlarmSetup.m_nCurMileage )
#define	 ClearCurrentMileage() 	     (g_stAlarmSetup.m_nCurMileage = 0 )

// ���ñ���������(��)
#define	 SetMaxMileage( nMeter ) 	 (g_stAlarmSetup.m_nMaxMileage = (nMeter) )
#define	 GetMaxMileage()		     (g_stAlarmSetup.m_nMaxMileage )


//===============================================================
//	ʵʼ������Χ��
void  			InitializtFence( void );
//==============����������===============================================================
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
// �Ƿ��������
unsigned char   IsListenNo( unsigned char	*pPhone );
					 				 
#endif //APP_TERMINAL_H