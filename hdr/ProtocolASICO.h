#include "..\hdr\main.h"

#ifndef  GPRSPROTOCOL_ASICO_H
#define  GPRSPROTOCOL_ASICO_H

#if( SW_GPRSPROTOCOL_ASICO )

// 同步码  终端上传信息报头
#ifndef		SYN_TEHEADERCODE1
#define		SYN_TEHEADERCODE1				0x24  //$
#endif
#ifndef		SYN_TEHEADERCODE2
#define		SYN_TEHEADERCODE2				0x24  //$
#endif

// 同步码  服务器上传信息报头
#ifndef		SYN_SVEREHEADERCODE1
#define		SYN_SVEREHEADERCODE1			0x40  //@
#endif
#ifndef		SYN_SVEREHEADERCODE2
#define		SYN_SVEREHEADERCODE2			0x40  //@
#endif

// 结束符
#ifndef		SYN_R_ENDCODE					
#define		SYN_R_ENDCODE					0x0d
#endif
#ifndef		SYN_N_ENDCODE					
#define		SYN_N_ENDCODE					0x0A
#endif    

#define		ASICO_DEUID_LEN					0x07

// 表头
#define		WHERE_MESSAGE_HEAD1				0x00
#define		WHERE_MESSAGE_HEAD2				0x01
// 数据长度
#define		WHERE_DATALEN_HIGHT				0x02
#define		WHERE_DATALEN_LOW				0x03
// DEUID数据起始位
#define		WHERE_MESSAGE_DEUID				0x04
// 控制码
#define		WHERE_CMD_HIGHT		    		0x0B
#define		WHERE_CMD_LOW		    		0x0C
// 数据内容
#define		WHERE_MESSAGE_DATA			    0x0D

//=====登陆结果============================================================================
enum{
	CMD_ERROR = 0x00,
	CMD_SUCCEED
};

//=====硬件命令控制码定义============================================================================
enum{
	CC_TE_LOGIN = 0x5000,		 	 //终端登陆
	
	CC_TE_LOGIN_ACK = 0x4000, 		 //服务中心确认终端登陆
	
	
	CC_TE_CALLONCE = 0x4101,		 //点名跟踪
	CC_TE_SETUP_TRACKTIME,			 //设置定位时间
	CC_TE_SETUP_NUMBER,				 //设置授权号码
	CC_TE_SETUP_OVERSPEED = 0x4105,			 //设置超速报警
	CC_TE_SETUP_OUTGEO,				 //设置移位报警
	CC_TE_SETUP_FENCE,				 //设置电子栅栏
	CC_TE_RESTORE_FACTORY = 0x4110,	 //恢愎出厂设置
	CC_TE_MULTI_SETUP =	0x4115,	//自定义控制	
	CC_TE_SETUP_FENCE2 = 0x4127,	 //设置电子栅栏
	
	CC_TE_EXPAND_CALLONCE = 0x5101,	 //点名跟踪
	CC_TE_ACK_TRACKTIME = 0x5100,	 //应答设置时间间隔	
	
	CC_TE_HS_DATA = 0x8001,			 //握手信号
	CC_TE_LISTEN_CAR = 0x8002,		 //发送车载监听
	CC_TE_TWO_WAY_CALL,				 //设置双向通话
	CC_TE_FUEL_CONTROL,				 //设置油路控制
	CC_TE_CENTER_LOCK_CONTROL,		 //设置中控锁	
	CC_TE_ALARM_SETUP,				 //报警设置
	CC_TE_READ_ALARM_STATUS,		 //读报警状态
	SETUP_ALARM_PARKING,		 	 //设置停车报警	
	SETUP_ALARM_ANTISTEAL,			 //非法开门报警
	SETUP_ALARM_TIREDNESS,			 //疲劳驾驶设置	
	
	CC_TE_READ_TRACKTIME =0x9002,	 //读取定位时间
	CC_TE_READ_NUMBER,				 //读取授权号码
	CC_TE_READ_OVERSPEED,			 //读取超速报警
	CC_TE_READ_OUTGEO,				 //读取移位报警
	CC_TE_READ_FENCE,				 //读取电子栅栏

	
	CC_TE_SINGLE_LOCATION_REPORT = 0x9955,	 //单一上传	
	
	CC_TE_ALARM_CMD = 0x9999		 //触发报警
};

//	BCD转换为十六进制
unsigned char   BCDToHex( unsigned char	nData);
//	字节型到十六进制
unsigned short  ByteToInt(unsigned char* pSrc);
//得出DEUID
void  			PL_ParseDEUID( unsigned char	*pDest, unsigned char *pSrc );
//  组织 表头(2B)|长度(2B)|DEUID(7B)|命令码(2B) 
void			PL_ComposeHeadData( unsigned char	*pDest, 
									unsigned char	nDataLen,
									unsigned short	nCmdCode );
unsigned char	PL_ComposeGPSData( unsigned char  *pDest );			
unsigned char 	PL_ComposeHSData( unsigned char	*pDest );	
unsigned char	PL_ComposeLogin( unsigned char  *pDest );
unsigned char	PL_ParseData(unsigned char *pDest, unsigned char *pSrc, unsigned char nLen );	
//   发送终端数据信息  MT_DELIVERY
unsigned char	PL_ParseDelivery( unsigned char  *pDest, 
								  unsigned char  *pSrc,
								  unsigned char	 nDataLen,
								  unsigned int	 nCmdCode );
//	设置跟踪时间
unsigned char   PL_ComposeTrackTime( unsigned char	*pSrc,
								 	unsigned short	nCmdCode,
								 	unsigned char	nStuts );
//  设置号码
unsigned char	PLS_SetupGpsMasterNo( unsigned char	*pSrc );							
//	设置区域移位报警
unsigned char	PLS_SetupGpsOutGeo( unsigned char	*pSrc );
//	设置距离
unsigned char	PLS_SetupFence( unsigned char	*pSrc,
								unsigned char	nCmdCode );	
//  读取授权号码  17+16+16;
unsigned char   PL_ComposeReadNumber( unsigned char	*pDest, 
									  unsigned char	nPhoneType,	
									  unsigned char	*pPhone );																
//									  
int 			PLS_ExplainLatitudeData(unsigned char *pData);	
int  			PLS_ExplainLongitudeData(unsigned char *pData);							
//	设置命令应答数据
unsigned char	PL_ComposeACKData( unsigned char	*pDest,
								   unsigned char	nFlag,	
								   unsigned short	nCmdCode );

						//  读报警功能状态
unsigned char	PL_ComposeReadAlarm( unsigned char	*pDest, 
									 unsigned short	nCmdCode );	   
////////////////////////////////////////////////////////////////////////////////
//   多路控制
//   一共四字节
//  1 Byte: 控制油路  0: 恢复油路	1: 断油	  2: 不动作
//  2 Byte: 控制上锁  0: 开锁		1: 上锁 	  2: 不动作
unsigned char	PL_ComposeMultiSetup( unsigned char	*pSrc );
//   更新状态信息
void			PL_UPDataStatus( void );
//	没有定位的时候回复
unsigned char	PL_ComposeNoValidData( unsigned char	*pDest );

#endif //SW_GPRSPROTOCOL_ASICO

#endif //GPRSPROTOCOL_ASICO_H 

