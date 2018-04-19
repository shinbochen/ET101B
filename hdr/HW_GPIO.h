//////////////////////////////////////////////////////////////////////////
// file name: HW_GPIO.h
// Date: 2009-02-04
// Author: real

#include		"main.h"

#ifndef GPIO_20090204_H
#define GPIO_20090204_H

/////////////////////////////////////////////////////////////////////////
// hardware IO port config

#define		HW_PORT_OUTPUT				1
#define		HW_PORT_INPUT				0 


// output port 
#define		HW_PORT_WARMPOWER			18
#define		HW_PORT_GSMRESET			123
#define		HW_PORT_CTRLOIL				23																																																																																						
#define		HW_PORT_GPSLED				16
#define   HW_PORT_UNLOCK 				116	  //���� 116
#define   HW_PORT_LOCK					13	  //���� 
#define		HW_PORT_UART_DTR			10    //DTR����

#define		HW_PORT_COLDPOWER			19    //ֱ�Ӷϵ�Դ

#if 		(HW_ET201B_3 || HW_ET201B || HW_ET201B_2 )
#define  	HW_PORT_SPK_MUTE			12	  //����	
#else
#define  	HW_PORT_SPK_MUTE			5	  //����	
#endif //HW_ET201B_3

// input port
#define		HW_PORT_ACC_I				17
#define		HW_PORT_SOS_I				21
#define		HW_PORT_DOOR_I				120																																				
#define  	HW_PORT_CUSTOM1      		30    //�Զ���
#define  	HW_PORT_CUSTOM2      		117   //�Զ���1
#define		HW_PORT_SHOCKING			25    //�񶯱���

#if ( HW_ET601 )
#define	    HW_G_SELECT					12	 //���ٸ�Ӧ
#define		HW_G_DETECT				    13

#define		HW_G_SELECT_H				1
#define		HW_G_SELECT_L				0

#define		HW_G_DETECT_H				1
#define		HW_G_DETECT_L				0


#define		HW_G_SELECT_HIGH()			GPIO_Set( HW_G_SELECT, HW_G_SELECT_H )
#define		HW_G_SELECT_LOW()			GPIO_Set( HW_G_SELECT, HW_G_SELECT_L )

#define		HW_G_DETECT_HIGH()			GPIO_Set( HW_G_DETECT, HW_G_DETECT_H )
#define		HW_G_DETECT_LOW()			GPIO_Set( HW_G_DETECT, HW_G_DETECT_L )
#endif

//////////////////////////////////////////////////////
// ��ģ���Դ 1
#if ( HW_ET007B_2 || HW_ET101B || HW_ET102B || HW_ETC101B || HW_ET201B || HW_ET201B_2 || HW_ET201B_3 || HW_ET601 )
	#define		HW_PORT_WARMPOWER_ON			1
	#define		HW_PORT_WARMPOWER_OFF			0	
#else // HW_ET007B HW_ET101A
	#define		HW_PORT_WARMPOWER_ON			0
	#define		HW_PORT_WARMPOWER_OFF			1
#endif

//////////////////////////////////////////////////////
// GSM Reset��ƽ 2 
#if ( HW_ET101A || HW_ET101B || HW_ET102B || HW_ETC101B || HW_ET201B || HW_ET201B_2 || HW_ET201B_3 || HW_ET601 )
	#define		HW_PORT_GSMRESET_H			1
	#define		HW_PORT_GSMRESET_L			0
#else // HW_ET007B HW_ET007B_2
	#define		HW_PORT_GSMRESET_H			0
	#define		HW_PORT_GSMRESET_L			1	
#endif

//////////////////////////////////////////////////////
// ��·��ƽ 3
#if ( HW_ET007B || HW_ET007B_2 || HW_ET101B || HW_ET102B || HW_ETC101B || HW_ET201B || HW_ET201B_2 || HW_ET201B_3 || HW_ET601)
	#define		HW_PORT_CTRLOIL_CUTOFF		1
	#define		HW_PORT_CTRLOIL_RESUME		0 
#else // HW_ET101A
	#define		HW_PORT_CTRLOIL_CUTOFF		0
	#define		HW_PORT_CTRLOIL_RESUME		1 
#endif



#define     HW_PORT_UART_DTR_H			1
#define 	HW_PORT_UART_DTR_L			0

#define 	HW_PORT_CENTER_H			1
#define		HW_PORT_CENTER_L			0
#define  	HW_PORT_SPK_MUTE_H			1
#define		HW_PORT_SPK_MUTE_L			0

#define		HW_PORT_GPSLED_ON			1
#define		HW_PORT_GPSLED_OFF			0

#define		HW_PORT_COLDPOWER_OFF 		0
#define		HW_PORT_COLDPOWER_ON		1 

///////////////////////////////////////////////////////////////////////// 

/////////////////////////////////////////////////////////////////////////
// ʹ��IO�� p0.1Ϊ1,p1.1Ϊ101,p2.1Ϊ201
void 					GPIO_Enable(unsigned char nPortNo);
void 					GPIO_SetDirection(unsigned char nPortNo, unsigned char nDir);
unsigned char GPIO_Get(unsigned char nPortNo);
void 					GPIO_Set(unsigned char nPortNo, unsigned char nLevel);
/////////////////////////////////////////////////////////////////////////
#define HW_GSM_WARM_POWEROFF()   	GPIO_Set(HW_PORT_WARMPOWER,HW_PORT_WARMPOWER_OFF)
#define HW_GSM_WARM_POWERON()    	GPIO_Set(HW_PORT_WARMPOWER,HW_PORT_WARMPOWER_ON)

#define HW_GSM_RESET_HIGH()		GPIO_Set(HW_PORT_GSMRESET, HW_PORT_GSMRESET_H)
#define HW_GSM_RESET_LOW()      GPIO_Set(HW_PORT_GSMRESET, HW_PORT_GSMRESET_L)

// ����ָ��
#define	HW_GSM_COLDPOWEROFF()	GPIO_Set(HW_PORT_COLDPOWER,HW_PORT_COLDPOWER_OFF)
#define	HW_GSM_COLDPOWERON()	GPIO_Set(HW_PORT_COLDPOWER,HW_PORT_COLDPOWER_ON)

#define HW_CTRLOIL_CUTOFF()		GPIO_Set(HW_PORT_CTRLOIL,HW_PORT_CTRLOIL_CUTOFF)
#define HW_CTRLOIL_RESUME()		GPIO_Set(HW_PORT_CTRLOIL,HW_PORT_CTRLOIL_RESUME)

#define	HW_GPSLED_ON()			GPIO_Set(HW_PORT_GPSLED, HW_PORT_GPSLED_ON )
#define	HW_GPSLED_OFF()			GPIO_Set(HW_PORT_GPSLED, HW_PORT_GPSLED_OFF )
#define	HW_GPSLED_BLINK()		GPIO_Set(HW_PORT_GPSLED, !GPIO_Get(HW_PORT_GPSLED) )

#define	HW_GET_ACC_STATE()		GPIO_Get( HW_PORT_ACC_I )
#define	HW_GET_SOS_STATE()		GPIO_Get( HW_PORT_SOS_I )


// �ż�� 101A��101B����PNP�ܣ���ͬ��
// �������ֲ���NPN���Ƿ���


#if(HW_ET101A||HW_ET101B )
#define	HW_GET_DOOR_STATE()		GPIO_Get( HW_PORT_DOOR_I )
#else // HW_ET007B HW_ET007B_2 HW_ET102B HW_ETC101B HW_ET201B HW_ET201B_2 HW_ET201B_3  HW_ET601
#define	HW_GET_DOOR_STATE()		(!GPIO_Get( HW_PORT_DOOR_I ))
#endif


#define HW_GET_CUSTOM1()		(!GPIO_Get (HW_PORT_CUSTOM1 ))	 //�Զ���1
#define HW_GET_CUSTOM2()		(!GPIO_Get (HW_PORT_CUSTOM2 ))	 //�Զ���2
#define HW_GET_SHOCKING()		GPIO_Get( HW_PORT_SHOCKING ) 	 //�񶯱���

#define HW_PORT_UART_DTR_HIGH() GPIO_Set( HW_PORT_UART_DTR, HW_PORT_UART_DTR_H)
#define HW_PORT_UART_DTR_LOW()  GPIO_Set( HW_PORT_UART_DTR, HW_PORT_UART_DTR_L)

// ���� ACTION 
#define HW_CENTER_UNLOOK_NACTION()  GPIO_Set( HW_PORT_UNLOCK, HW_PORT_CENTER_H) 
#define HW_CENTER_UNLOOK_ACTION()  	GPIO_Set( HW_PORT_UNLOCK, HW_PORT_CENTER_L)
// ����
#define HW_CENTER_LOOK_NACTION() 	GPIO_Set( HW_PORT_LOCK, HW_PORT_CENTER_H) 
#define HW_CENTER_LOOK_ACTION() 	GPIO_Set( HW_PORT_LOCK, HW_PORT_CENTER_L)
// ����
#define HW_PORT_SPKMUTE_OPEN()  	GPIO_Set( HW_PORT_SPK_MUTE, HW_PORT_SPK_MUTE_H) 
#define HW_PORT_SPKMUTE_CLOE()  	GPIO_Set( HW_PORT_SPK_MUTE, HW_PORT_SPK_MUTE_L)


#endif	// GPIO_20090204_H
