///////////////////////////////////////////////////////////////////////////
//   file name: SMSProtocolB.h
//      author:
//        date: 2009-02-23
//     reamark: 


#include "..\hdr\main.h"

#ifndef 	SMSPROTOCOLDATAB_H
#define 	SMSPROTOCOLDATAB_H

#define   TEXT_CMD_START				'*'
#define   TEXT_CMD_END				'#'
#define   TEXT_COLON					':'
#define   TEXT_EQUAL					'='
#define   TEXT_QUERY					'?'

#define		TEXT_PARA_ERROR				"Parameter Error!"
#define		TEXT_CMD_ERROR				"CMD Error!"
#define		TEXT_PSD_ERROR				"Wrong password!"
#define		TEXT_NOT_AUTHORIZED		"Not be Authorized!"
#define   TEXT_EXEC_ERROR				"Execute Error"

#define		TEXT_PARA_ERROR_CN		"\x53\xC2\x65\x70\x95\x19\x8B\xEF"															// ��������	
#define		TEXT_CMD_ERROR_CN			"\x54\x7D\x4E\xE4\x95\x19\x8B\xEF"
#define		TEXT_PSD_ERROR_CN			"\x5B\xC6\x78\x01\x95\x19\x8B\xEF"														// �������					

#define		TEXT_NOT_AUTHORIZED_CN		"\x6C\xA1\x67\x09\x63\x88\x67\x43"														// û����Ȩ	
#define		TEXT_RESET_SUCCESS_CN			"\x91\xCD\x54\x2F\x62\x10\x52\x9F"													// �����ɹ�	
#define		TEXT_RESTORE_SUCCESS_CN  	"\x5D\xF2\x60\x62\x59\x0D\x9E\xD8\x8B\xA4\x8B\xBE\x7F\x6E"								// �ѻָ�Ĭ������:


#define		TEXT_ALARM_ENABLE_CN			"\x62\xA5\x8B\x66\x6F\xC0\x6D\x3B"													// ��������
#define		TEXT_ALARM_DISABLE_CN			"\x62\xA5\x8B\x66\x51\x73\x95\xED"													// �����ر�:	
	
#define		TEXT_SN										"SN"

#define 	TEXT_POWER					"Power" 
#define		TEXT_POWER_CN				"\x75\x35\x6E\x90"
#define		TEXT_PASSWORD				"Password"
#define		TEXT_PASSWORD_CN		"\x5B\xC6\x78\x01"

#define		TEXT_LOCKED					"Locked"
#define		TEXT_UNLOCKED				"Unlocked"
#define		TEXT_LOCKED_CN			"\x5B\x9A\x4F\x4D"
#define		TEXT_UNLOCKED_CN		"\x67\x2A\x5B\x9A\x4F\x4D"




#define		TEXT_ACC					"ACC"
#define		TEXT_ON						"ON"
#define		TEXT_OFF					"OFF"

#define		TEXT_DOOR					"Door"
#define		TEXT_DOOR_CN			"\x95\xE8"							// ��
#define		TEXT_OPEN					"Open"
#define		TEXT_CLOSE				"Close"

#define		TEXT_GPS_SIGNAL				"GPS Signal"
#define		TEXT_GSM_SIGNAL				"GSM Signal"
#define  	TEXT_STRONG	  				"Strong"	
#define	 	TEXT_NORMAL	  				"Normal"
#define	 	TEXT_WEAK		  			"Weak"
#define		TEXT_GPRS					"GPRS"

#define		TEXT_GSM_SIGNAL_CN	"\x4F\xE1\x53\xF7"
#define  	TEXT_STRONG_CN	  	"\x5F\x3A"	
#define	 	TEXT_NORMAL_CN	  	"\x4E\x2D"
#define	 	TEXT_WEAK_CN	  		"\x5F\x31"
				
#define		TEXT_VERSION				"Version"

#define		TEXT_SPEED					"Speed"

#define		TEXT_DISABLE				"Disable"
#define		TEXT_ENABLE					"Enable"
#define		TEXT_DISABLE_CN				"\x51\x73\x95\xED"
#define		TEXT_ENABLE_CN				"\x6F\xC0\x6D\x3B"

#define		TEXT_ALL							"All"
#define		TEXT_ALARM						"Alarm"
#define		TEXT_ALARM_CN					"\x62\xA5\x8B\x66"

#define     TEXT_SUCCESS					"success!"	

#define 	TEXT_EVERY						"every"
#define 	TEXT_EVERY_CN					"\x6B\xCF"


#define 	TEXT_AUTOTRACK				"Auto Track"
#define		TEXT_AUTOTRACK_CN			"\x81\xEA\x52\xA8\x66\xF4\x65\xB0"


#define 	TEXT_SEC_CN						"\x79\xD2"
#define		TEXT_METER_CN					"\x7C\x73"

#define		TEXT_PARKING					"Parking"
#define		TEXT_PARKING_CN				"\x50\x5C\x8F\x66"
#define		TEXT_ANTISTEAL				"AntiSteal"
#define		TEXT_ANTISTEAL_CN				"\x96\x32\x76\xD7"
#define		TEXT_OVERSPEED				"Overspeed"
#define		TEXT_OVERSPEED_CN			"\x8D\x85\x90\x1F"
#define		TEXT_TOWING						"Towing"
#define		TEXT_TOWING_CN				"\x62\xD6\x8F\x66"
#define		TEXT_SOS							"SOS"
#define		TEXT_SOS_CN						"\x7D\x27\x60\x25\x6C\x42\x52\xA9"

#define		TEXT_GEOFENCE					"Geofence"
#define		TEXT_IN								"In"
#define		TEXT_OUT							"Out"
#define		TEXT_INGEOFENCE_CN		"\x51\x65\x75\x4C"
#define		TEXT_OUTGEOFENCE_CN		"\x51\xFA\x75\x4C"

#define		TEXT_POWEROFF					"Main Power Off"
#define		TEXT_POWEROFF_CN			"\x65\xAD\x75\x35"

#define   TEXT_LOW_POWER				"Low Voltage"
#define   TEXT_LOW_POWER_CN			"\x4F\x4E\x75\x35\x53\x8B"

#define		TEXT_CUSTOME					"Custom"
#define		TEXT_CUSTOME_CN				"\x81\xEA\x5B\x9A\x4E\x49"
#define		TEXT_TIREDDRIVE				"Tired Drive"
#define		TEXT_TIREDDRIVE_CN		"\x75\xB2\x52\xB3\x9A\x7E\x9A\x76"

#define		TEXT_WORKMODE				"Work mode"
#define		TEXT_MAPTRACK				"Maptrack"
#define		TEXT_SERVER					"Server"
#define  	TEXT_SMS						"SMS"



#define		TEXT_SMC						"SMC"
#define		TEXT_AUTHORIZED			"Authorized"
#define		TEXT_MASTER					"Master"
#define	  TEXT_NO							"No"

#define		TEXT_SMC_CN					"\x77\xED\x4F\xE1\x4E\x2D\x5F\xC3"
#define		TEXT_AUTHORIZED_CN	"\x63\x88\x67\x43"
#define		TEXT_MASTER_CN			"\x4E\x3B\x63\xA7"
#define	  TEXT_NO_CN					"\x53\xF7\x78\x01"

#ifdef 	TOM_FRED
#define		TEXT_FUEL_ON				"Engine unlock"
#define		TEXT_FUEL_OFF				"Engine locked"
#else
#define		TEXT_FUEL_ON				"Fuel On"
#define		TEXT_FUEL_OFF				"Fuel Off"
#endif

#define		TEXT_FUEL_ON_CN			"\x6C\xB9\x8D\xEF\x60\x62\x59\x0D"
#define		TEXT_FUEL_OFF_CN		"\x65\xAD\x6C\xB9\x62\x10\x52\x9F"

#define		TEXT_DEFENSIVE			"Defensive"
#define		TEXT_DEFENSIVE_CN		"\x8B\xBE\x96\x32"

#define     TEXT_NMEA					"Nmea"

#define  	TEXT_NETWORK			    "Network"
#define     TEXT_TEUID					"TEUID"
#define		TEXT_APN					"APN"
#define		TEXT_DNSR					"DNSR"
#define     TEXT_LOGIN_INFO				"Login Info"
#define     TEXT_CHANFE_TO				"Change to"	
#define 	TEXT_FORMAT					"format"

#define     TEXT_ARMED					"Armed"
#define     TEXT_ARMED_CN				"\x8B\xBE\x96\x32"
#define     TEXT_DISARMED				"Disarmed"
#define     TEXT_DISARMED_CN		"\x64\xA4\x96\x32"
											
#define     TEXT_CALARM					"calarm"
#define		TEXT_CAN_SETUP				"can't Setup"

#define		TEXT_INPUT_CN			"\x8F\x93\x51\x65"
#define     TEXT_INPUT				"input"

#define  	TEXT_ENTAL_SYSTEM_CN    "\x53\x9F\x8F\x66\x96\x32\x76\xD7"
#define  	TEXT_ENTAL_SYSTEM    	"external alarm system"

#define 	TEXT_REINPUT			"reinput"
#define     TEXT_CHECK				"check"
#define 	TEXT_LOW				"low"
#define     TEXT_HIGH				"high"
#define  	TEXT_M 				    "Meter"

#define     TEXT_GOGPS	 		    "http://www.gogps.mobi/tracker.asp"

#define		TEXT_HANDSHAKE_TIME		"Handshake time"
#define		TEXT_SECOND				"second"
#define		TEXT_ALARM_NOGPS_SIGNAL	"Alarm when no gps signal"
#define		TEXT_IDLE_AFTER_ACC_OFF "Idle after ACC off"
#define 	TEXT_MILEAGE_REMARK		"Mileage Remark"	
#define		TEXT_TOTAL_MILEAGE		"Total Mileage"
#define		TEXT_AUTOFUEL			"Auto Fuel"

#ifdef		SW_SMSPROTOCOLB
/////////////////////////////////////////////////////////////////////////////////
// SMS����ָ��
enum{
	SMS_RESTART = 0x00,			//������λ
	SMS_DEFAULT,				//�ָ���������
	SMS_SERIAL_SN,				//���
	SMS_STATE,					//�ն˻���״̬
	SMS_VERSION,				//��ȡ�汾��
	SMS_POSITION,				//��λ
	SMS_POSITION2,				//��λ2
	SMS_POSITION3,				//��λ3
	SMS_POSITION6,				//��ѯ��ַ
	SMS_ALARM_SETUP,			//���ñ�������/��ֹ
	SMS_TRACK_SETUP,			//�Զ��ϴ�
	SMS_PARKING_SETUP,			//ͣ����������
	SMS_ANTISTEAL_SETUP,		//�Ƿ����ű�������	
	SMS_ASLIS_SETUP,			//0:����Ϊ�ż����뱨�� 1:����Ϊԭ���������� 
	
	SMS_OVERSPEED_SETUP,		//���ٱ�������
	SMS_CORTROL_OIL,			//��·����
	SMS_CONTROL_LISTEN,			//�绰����
	SMS_CONTROL_TALK,			//����ͨ��
	SMS_SLAVE_SETUP1,			//���ú���
	SMS_SLAVE_SETUP2,			//���ú���
	SMS_SLAVE_SETUP3,			//���ú���
	
	SMS_SLAVE_SETUP4,			//���ú���
	SMS_SLAVE_SETUP5,			//���ú���
	
	SMS_MASTER_SETUP,			//�������غ���
	SMS_SMC_SETUP,				//���ö������ĺ���
	SMS_PASSWORD_SETUP,			//��������
	SMS_SERVER_SETUP,			//����IP��ַ
		
	SMS_APN_SETUP,				//����APN����
	SMS_NET_SETUP,				//���������û���������
	SMS_WORKMODE_SETUP,			//����ģʽ
	SMS_FORWARD,				//ת������ 	
		
	SMS_EXCUTE,
	SMS_TOW,					//�г�����
	SMS_DEFENSIVE,
	SMS_IMEI,					//��ѯIMEI
	SMS_LOCK,					//�п�������
	SMS_POWEROFF,				//������رյ�Դ
	SMS_LOWPOWER,				//������رյ͵�Դ
	SMS_SETUP,					//��������
	
	SMS_INGEO,
	SMS_OUTGEO,
	SMS_NAMEC,	
	SMS_REINPUT,				//�Զ�������	
	SMS_SERVER_DNSR,			//��������
	
	SMS_HANDSHAKE,				//���������ź�
	SMS_ACCALARM,				//����ACC����
	SMS_ACCIDLE,				//ACC��������
	SMS_NOGPSALARM,				//GPSδ��λ����		
	SMS_DATAFORMAT,				//GPS���ݸ�ʽ
	
	SMS_MILEAGE_REMARK,			//�������
	SMS_TOTAL_MILEAGE,			//��ѯ�����
	// 20100413 add
	SMS_CLOSE_OIL,				// ����
	SMS_RESUME_OIL,				// �ָ���·
	
	SMS_SOS,
	SMS_SETUP_AUTOFUEL,			//�ż챨������	
	SMS_CMD_END
};

/////////////////////////////////////////////////////////////
unsigned char PLS_GetCommandType( unsigned char  *pBuf );
unsigned char PLS_ParseCommandData(  unsigned char  *pDest, 
																		 unsigned char  *pSrc, 
																		 unsigned char  *pCaller,
																		 unsigned char  *pCodeType );
unsigned char PLS_CarListen( unsigned char  *pDest,unsigned char  *pSrc, unsigned char nLangCN );	
unsigned char PLS_SetupMasterNo( unsigned char  *pDest, unsigned char  *pSrc, unsigned char nLangCN );
//    TRACK=1000:S/D#
unsigned char PLS_SetupTrackSetup( unsigned char  *pSrc );
unsigned char PLS_SetupInGeo( unsigned char	*pDest, unsigned char	*pSrc );
unsigned char PLS_SetupOutGeo( unsigned char	*pDest, unsigned char	*pSrc, unsigned char   nLangCN );
unsigned char PLS_SetupCustomName( unsigned char  *pDest,  unsigned char  *pSrc );
/////////////////////////////////////////////////////////////////////////////
//  ���õ�ƽ������
//unsigned char PLS_SetReInput( unsigned char	*pDest, 
//							  unsigned char	*pSrc, 
//							  unsigned char nColon,
//				 			  unsigned char nLangCN );

unsigned char PLS_ComposeGPRMCPosition( unsigned char *pDest );				 			  
				 			  				 			  								   
#endif	  //SW_SMSPROTOCOLB

//////////////////////////////////////////////////////////////////////
// public
unsigned char unicodecpy( unsigned char	*pDest, unsigned char *pSrc );
unsigned char PLS_ReadState( unsigned char  *pDest, unsigned char nLangCN );
unsigned char PLS_SetupAlarmEnable(unsigned char	*pDest, unsigned char	nFlag, unsigned char nLangCN );
unsigned char PLS_SetupAlarmParking( unsigned char  *pDest, unsigned char  *pSrc, unsigned char nLangCN); 
unsigned char PLS_SetupAlarmAntisteal( unsigned char  *pDest, unsigned char  *pSrc, unsigned char nLangCN );
unsigned char PLS_SetupAlarmOverspeed( unsigned char  *pDest,unsigned char  *pSrc, unsigned char nLangCN );
unsigned char PLS_SetupAlarmTow( unsigned char	*pDest, unsigned char	*pSrc, unsigned char nLangCN  );		
unsigned char PLS_SetupDefensive( unsigned char  *pDest, unsigned char  *pSrc, unsigned char nLangCN );			
unsigned char PLS_CtrlOil( unsigned char	*pDest, unsigned char	nFlag, unsigned char nLangCN );
unsigned char PLS_SetupSlaveNo( unsigned char  *pDest, unsigned char  *pSrc, unsigned char nFlag, unsigned char nLangCN );
unsigned char PLS_SetupSMCNo( unsigned char  *pDest, unsigned char  *pSrc, unsigned char nLangCN );
unsigned char PLS_SetupPassword( unsigned char  *pDest,unsigned char  *pSrc, unsigned char nColon, unsigned char nLangCN );		
unsigned char PLS_SetupServer( unsigned char  *pDest,  unsigned char  *pSrc, unsigned char nColon, unsigned char nLangCN );		
unsigned char PLS_SetupServerDNSR( unsigned char  *pDest, unsigned char  *pSrc, unsigned char nLangCN  );
unsigned char PLS_SetupAPN( unsigned char  *pDest,unsigned char  *pSrc, unsigned char nLangCN );
unsigned char PLS_SetupAPNUserPsd( unsigned char  *pDest, unsigned char  *pSrc, unsigned char nColon, unsigned char nLangCN );
unsigned char PLS_SetupWorkMode( unsigned char  *pDest,unsigned char  *pSrc, unsigned char nLangCN );	
unsigned char PLS_ForwardMsg( unsigned char  *pCaller, unsigned char  *pDest, unsigned char  *pSrc, unsigned char nLangCN );
unsigned char PLS_ExecuteExtCmd( unsigned char  *pDest, unsigned char  *pSrc, unsigned char nLangCN );	
unsigned char PLS_ComposePosition( unsigned char *pDest, unsigned char nLangCN );
unsigned char PLS_SetupLock(  unsigned char *pDest, unsigned char *pSrc,  unsigned char nLangCN );
//  �����豸�Ļ���������Ϣ
unsigned char PLS_SetupDeviceInfo( unsigned char	*pDest, unsigned char	*pSrc );

unsigned char PLS_IsIPAddr( unsigned char *pSrc );		
unsigned char PLS_ComposeUploadData( unsigned char	*pDest, unsigned char *pCodeType );
unsigned char PLS_ComposeCLUploadData( unsigned  char *pDest, unsigned char *pCodeType );
//  ��֯�������
unsigned char PLS_ComposeMileage( unsigned char 	*pDest, unsigned char *pCodeType );
/////////////////////////////////////////////////////////////////////////////
//  ���õ�ƽ������
unsigned char PLS_SetReInput( unsigned char	*pDest, 
							  unsigned char	*pSrc, 
							  unsigned char nColon,
				 			  unsigned char nLangCN );
#endif 			//SMSPROTOCOLDATAB_H