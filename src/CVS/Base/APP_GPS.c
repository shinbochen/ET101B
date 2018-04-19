/////////////////////////////////////////////////////////////
//file name:  APP_GPS.C
//     Date:  2008-9-02
//	 Author:	
/////////////////////////////////////////////////////////////
#include "..\hdr\includes.h"
#include "..\hdr\APP_GPS.h"
#include "..\hdr\HW_UART.h"
#include "..\hdr\queue.h"
#include    <stdlib.h>
#include "..\hdr\main.h"
#include "..\hdr\HW_GPIO.h"
#include  <stdio.h> 


////////////////////////////////////////////////////////////
// variable declare

#define   GPS_RECEMAXBUFFER					160


//GPS���������¼�
OS_EVENT 	*pGPSReceiveSem = NULL;
// ���������ջ�ռ�
OS_STK     TaskGPSReceiveStk[TASK_GPS_RECEIVE_STK_SIZE];								
// �������ݽ��պ�Ĵ��������
OS_STK     TaskGPSAppStk[TASK_GPS_APP_STK_SIZE];				
//GPS���ݽṹ
STGPSINFO  g_stGPSInfo;
//���������Ӵ�����
static int nGPSReceiveBuf[(GPS_RECEMAXBUFFER + sizeof(int) - 1) / sizeof(int)];

//========================================================================================
//  GPS��������
char *s_strGPSCMD[]={
	"$GPRMC,",
	"$GPGGA,"
};

//========================================================================================
//����GPS����
//========================================================================================
void GPS_Task_Creat( void ){
		
	
	OSTaskCreate(GPS_TaskGPSApp, (void *)0,
		&TaskGPSAppStk[TASK_GPS_APP_STK_SIZE-1], 7);
		
	return;
}
//========================================================================================
// ���ڽ�������
// ����������GPS
//========================================================================================
void GPS_TaskGPSDataReceive(void *pData){
	
	unsigned char nTemp = 0, nPre = 0;
	unsigned char nError = 0;
		
	pData = pData;
	
	QueueCreate((void*)nGPSReceiveBuf, sizeof(nGPSReceiveBuf),NULL, NULL); 	
	
	QueueFlush((void*)nGPSReceiveBuf); 
	
	
	while ( 1 ){	
		OS_ENTER_CRITICAL();		
		nPre = nTemp;
		nTemp = GPSUARTGetch(&nError, 0);			
		QueueWrite((void *)nGPSReceiveBuf, nTemp); 		
		if ((nTemp == 0x0A) && (nPre ==0x0D) ){
			if( pGPSReceiveSem ){
				OSSemPost( pGPSReceiveSem );		  	
			}
		}
		OS_EXIT_CRITICAL();			
	}
	return;
}
//========================================================================================
// ��ȡGPS����
// ����ֵ: 0: �ɹ�  1: ʧ��
//========================================================================================
void GPS_ReadCmd( unsigned char *pBuf ){
	
	unsigned char   nTemp = 0;
	unsigned char   nPre = 0;
	unsigned char	nLen = 0;
	unsigned char 	bFlag = 0;
	
	OS_ENTER_CRITICAL();		  
	while (nLen <= 89)	{				//!QueueIsEmpty( )
		nPre = nTemp;
		if (QueueRead((char*)&nTemp, nGPSReceiveBuf) == QUEUE_OK)
		{
			 *(pBuf+nLen) = nTemp;
			 nLen++;
			 if ( (nTemp == 0x0A) && (nPre == 0x0D) )	{	
				 bFlag = 1;
				 break;
			}
		}
		else{
			bFlag = 0;
			break;		
		}	
	}
	if( !bFlag ){
		*pBuf = NULL;
	}
	else{
		*(pBuf+nLen) = NULL;
	}
	OS_EXIT_CRITICAL();
	return;
}
//========================================================================================
// ���ڽ�������
// �������ݽ��պ�(GPS)�Ĵ�������
//========================================================================================
void GPS_TaskGPSApp(void *pData){ 
	
	unsigned char 	nCmdBuf[90];	
	unsigned char   err = 0;
	unsigned char	nCmdType = 0;
	
	pData = pData;
	

	OSTimeDly(200);	
	HW_GPSLED_OFF();
	// ��ʼֵ
	memset( &g_stGPSInfo, 0x00, sizeof(STGPSINFO) );
	//����GPS�����߳�
	OSTaskCreate(GPS_TaskGPSDataReceive, (void *)0,
		&TaskGPSReceiveStk[TASK_GPS_RECEIVE_STK_SIZE - 1], 8);	
		
	while( 1 ){		
		
		OSSemPend(pGPSReceiveSem, 500, &err );
		
		if( err == OS_NO_ERR ){
			GPS_ReadCmd( nCmdBuf );	
			
			nCmdType = GPS_GetCmdType( nCmdBuf );
			switch( nCmdType ){
			case GPS_GPRMC:
				GPS_ParseGPRMCCmd( nCmdBuf );
				break;
			case GPS_GPGGA:
				GPS_ParseGPGGACmd( nCmdBuf );
				break;
			}	
		}		
	}
	return;
}
//========================================================================================
//��ȡ����һ�����ݸ�ʽ
//GPS����ȥ��: ��ͷ����	 
//========================================================================================
unsigned char GPS_GetCmdType(unsigned char *pMsg)
{
	unsigned char 	nCnt = 0;
	unsigned char	nLen = 0;

	for( nCnt = 0; nCnt < GPS_CMD_END; nCnt++ ){
		nLen = strlen( (char*)s_strGPSCMD[nCnt] );
		if( memcmp( pMsg, s_strGPSCMD[nCnt], nLen ) == 0 ){
			strcpy( (char*)pMsg, (char*)pMsg+nLen );
			break;		 		
		}	 	
	}
	return nCnt;
}
//========================================================================================
//����GPS��$GPRMC���ݸ�ʽ
// $GPRMC,054113.000,A,2235.5730,N,11352.2119,E,0.00,,190408,,,A*7E
// input:054113.000,A,2235.5730,N,11352.2119,E,0.00,,190408,,,A*7E
//========================================================================================
void  GPS_ParseGPRMCCmd(unsigned char *pData)
{
	unsigned char  	*pTmp  = NULL;
	unsigned char	*pTmp1 = NULL;
	unsigned char 	nCnt = 0;
	unsigned char 	nLen = 0;
		
	while( 1 ){
		
		pTmp = (unsigned char*)strchr((char *)pData, ',' );   
		
		if( pTmp == NULL ){
			break;
		}			
		*pTmp = 0x00;
		switch( nCnt ){
		// ʱ��
		case 0:
			g_stGPSInfo.m_nHour 	=  ((*pData++) - '0')*10;
			g_stGPSInfo.m_nHour 	+= ((*pData++) - '0');
			g_stGPSInfo.m_nMinute =  ((*pData++) - '0')*10;
			g_stGPSInfo.m_nMinute += ((*pData++) - '0');
			g_stGPSInfo.m_nSecond =  ((*pData++) - '0')*10;
			g_stGPSInfo.m_nSecond += ((*pData++) - '0');	
			break;	
		// ��λ
		case 1:
			if (*pData == 'A' || *pData == 'a' ){
				g_stGPSInfo.m_bPosValid = 1;
			}
			else{
				g_stGPSInfo.m_bPosValid = 0;
			}
			break;	
		//γ�Ȳ���
		case 2:
			if( GPS_IsPosValid( ) ){			
				// ȡС����ǰ������
				pTmp1 = (unsigned char*)strchr( (char*)pData, '.' ); 
				if( pTmp1 ){
							
					*pTmp1 = 0x00;
					nLen = strlen( (char*)pData );				

					g_stGPSInfo.m_nLATIDegree = 0;
					g_stGPSInfo.m_nLATIMinute = 0;
					g_stGPSInfo.m_nLATIPrecision = 0;

					switch( nLen ){
					case 1:
						g_stGPSInfo.m_nLATIMinute = (*pData) - '0';	
						break;
						
					case 2:
						g_stGPSInfo.m_nLATIMinute =  ((*pData++) - '0')*10;	
						g_stGPSInfo.m_nLATIMinute += (*pData++) - '0';	
						break;
						
					case 3:
						g_stGPSInfo.m_nLATIDegree += (*pData++) - '0';
						
						g_stGPSInfo.m_nLATIMinute =  ((*pData++) - '0')*10;	
						g_stGPSInfo.m_nLATIMinute += (*pData++) - '0';	
						break;
						
					case 4:
					default:	
						g_stGPSInfo.m_nLATIDegree =  ((*pData++) - '0')*10;
						g_stGPSInfo.m_nLATIDegree += (*pData++) - '0';
						
						g_stGPSInfo.m_nLATIMinute =  ((*pData++) - '0')*10;	
						g_stGPSInfo.m_nLATIMinute += (*pData++) - '0';				
						break;			
					}				
					g_stGPSInfo.m_nLATIPrecision = atoi( (char*)pTmp1+1 );
				}
			}	
			break;
		//γ�ȱ��
		case 3:
			if( GPS_IsPosValid( ) ){
				if ( *pData == 'N' || *pData == 'n' ){
			 		g_stGPSInfo.m_nLLFlag |= N_LATITUDE;
			 	}
			 	else{
			 		g_stGPSInfo.m_nLLFlag &= ~N_LATITUDE;		 		
			 	}
			 }
			break;
		//���Ȳ���
		case 4:
			if( GPS_IsPosValid( ) ){
				// ȡС����ǰ������
				pTmp1 = (unsigned char*)strchr( (char*)pData, '.' ); 
				if( pTmp1 ){			
					*pTmp1 = 0x00;
					
					nLen = strlen( (char*)pData );
					g_stGPSInfo.m_nLONGDegree = 0;
					g_stGPSInfo.m_nLONGMinute = 0;
					g_stGPSInfo.m_nLONGPrecision = 0;

					switch( nLen ){
					case 1:
						g_stGPSInfo.m_nLONGMinute = (*pData) - '0';	
						break;
						
					case 2:
						g_stGPSInfo.m_nLONGMinute =  ((*pData++) - '0')*10;	
						g_stGPSInfo.m_nLONGMinute += (*pData++) - '0';	
						break;
						
					case 3:
						g_stGPSInfo.m_nLONGDegree += (*pData++) - '0';
						
						g_stGPSInfo.m_nLONGMinute =  ((*pData++) - '0')*10;	
						g_stGPSInfo.m_nLONGMinute += (*pData++) - '0';	
						break;
						
					case 4:
						g_stGPSInfo.m_nLONGDegree =  ((*pData++) - '0')*10;
						g_stGPSInfo.m_nLONGDegree += (*pData++) - '0';
						
						g_stGPSInfo.m_nLONGMinute =  ((*pData++) - '0')*10;	
						g_stGPSInfo.m_nLONGMinute += (*pData++) - '0';				
						break;
						
					case 5:
						g_stGPSInfo.m_nLONGDegree =  ((*pData++) - '0')*100;
						g_stGPSInfo.m_nLONGDegree += ((*pData++) - '0')*10;
						g_stGPSInfo.m_nLONGDegree += (*pData++) - '0';
						
						g_stGPSInfo.m_nLONGMinute =  ((*pData++) - '0')*10;	
						g_stGPSInfo.m_nLONGMinute += (*pData++) - '0';	
						break;
						
					default:
						break;			
					}		
					g_stGPSInfo.m_nLONGPrecision = atoi( (char*)pTmp1+1 );
				}
			}		  
			break;
		//���ȱ��
		case 5:
			if( GPS_IsPosValid( ) ){
				if ( *pData == 'E' || *pData == 'e' ){
					g_stGPSInfo.m_nLLFlag |= E_LONGITUDE;
			 	}
			 	else{
			 		g_stGPSInfo.m_nLLFlag &= ~E_LONGITUDE;		 		
			 	}
			 }
			break;
		//speed
		case 6:
			if( GPS_IsPosValid( ) ){
				g_stGPSInfo.m_nCurSpeed = atoi( (char*)pData )*10;
				pTmp1 = (unsigned char*)strchr((char*)pData,'.');     		  //ȡС����ǰ������
				if (pTmp1 != NULL){
					pTmp1++;
					g_stGPSInfo.m_nCurSpeed += (*pTmp1) - '0';
				}
			}
			else{
				g_stGPSInfo.m_nCurSpeed = 0;
			}
			break;
		//����
		case 7:
			if( GPS_IsPosValid( ) ){
				g_stGPSInfo.m_nCurDirection = atoi( (char*)pData );
			}
			break;
		// �������ں���
		// 190408
		case 8:
			g_stGPSInfo.m_nDay   =  ((*pData++) - '0') * 10;
			g_stGPSInfo.m_nDay   += (*pData++) - '0';
			g_stGPSInfo.m_nMonth =  ((*pData++) - '0') * 10;
			g_stGPSInfo.m_nMonth += (*pData++) - '0';
			g_stGPSInfo.m_nYear  =  ((*pData++) - '0') * 10;
			g_stGPSInfo.m_nYear  += (*pData++) - '0';
			break;			
		}
		pData = pTmp+1; 
		nCnt++;
	}	
	return;	
}
//========================================================================================
//	����
//========================================================================================
int GPS_GetLongitude( void ){
		
	int			nResult;
	
	nResult =  g_stGPSInfo.m_nLONGDegree*3600000;
	nResult += g_stGPSInfo.m_nLONGMinute * 60000;
	nResult += g_stGPSInfo.m_nLONGPrecision * 6;
	if ( g_stGPSInfo.m_nLLFlag & E_LONGITUDE  ){
		nResult *= 1;
	}
	else{
		nResult *= -1;
	}
	return nResult;
}
//========================================================================================
//	����-4*4+γ��-16*19
int	 GPS_CalculateData( void ){
	
	int				nTmp;
	int				nResult;
	
	nTmp = (int)(GPS_GetLongitude()/3600000);
	nTmp = (nTmp-4)*4;
	
	nResult = (int)(GPS_GetLatitude()/3600000);
	nResult = (nResult-16)*19;	
	nResult += nTmp;
	
	return nResult;
}

//========================================================================================
//	γ��
//========================================================================================
int GPS_GetLatitude( void ){	
	
	int			nResult = 0;
	
	nResult = g_stGPSInfo.m_nLATIDegree*3600000;
	nResult += g_stGPSInfo.m_nLATIMinute * 60000;
	nResult += g_stGPSInfo.m_nLATIPrecision * 6;
	if ( g_stGPSInfo.m_nLLFlag & N_LATITUDE ){
		nResult *= 1;
	}
	else{
		nResult *= -1;
	}
	return nResult;
}
//========================================================================================
//	�����ٶ� milimetre
//========================================================================================
unsigned short	GPS_GetMilimetre( void ){ 

	unsigned short	nResult = 0;
	
	nResult =  g_stGPSInfo.m_nCurSpeed * 1.852;
	nResult /= 10;
	
	return nResult;	
}
//========================================================================================
//	��ѯGPS�ź�
//  $GPGGA,025849.000,2234.4271,N,11351.2681,E,1,10,1.0,108.2,M,-3.0,M,,0000*46
void  GPS_ParseGPGGACmd( unsigned char	*pMsg ){
	
	unsigned char	nCnt = 0;
	unsigned char	*pch = NULL;
	
	while( nCnt <= 8 ){	
		pch = (unsigned char*)strchr((char*)pMsg, ',');
		if ( pch == NULL ){
			break;
		}
		if ( nCnt == 6 ){
			*pch = 0;
			g_stGPSInfo.m_nGPSSignal = atoi((char*)pMsg);
			break;
		}
		pMsg = pch + 1;
		nCnt++;
	}
}
//========================================================================================
//$GPRMC,054113.000,A,2235.5730,N,11352.2119,E,0.00,,190408,,,A*7E
unsigned char	GPS_GetGPRMCData( unsigned char	*pDest ){
	
	unsigned char 	nResult = 0;
	unsigned char	nCheck = 0;
	unsigned char	nBuf[3];
	
	
	// ��ȡ��λ��־
	if( GPS_IsPosValid() ){ 
		nBuf[0] = 'A';
	}
	else{
		nBuf[0] = 'V';
	}
	//γ�ȱ��
	
	if( g_stGPSInfo.m_nLLFlag & N_LATITUDE){
		nBuf[1] = 'N';
	}
	else{
		nBuf[1] = 'S';
	}
	//���ȱ��
	if ( g_stGPSInfo.m_nLLFlag & E_LONGITUDE  ){
		nBuf[2] = 'E';
	}
	else{
		nBuf[2] = 'W';
	}
	// GPS ����
	sprintf( (char*)pDest, 	"$GPRMC,%02d%02d%02d.000,%c,%02d%02d.%04d,%c,%03d%02d.%04d,%c,%.2f,%d,%02d%02d%02d,,,A", 
														g_stGPSInfo.m_nHour,
												   	    g_stGPSInfo.m_nMinute,
												   	    g_stGPSInfo.m_nSecond,
												   		nBuf[0],
												   		g_stGPSInfo.m_nLATIDegree,
												   		g_stGPSInfo.m_nLATIMinute,
												   		g_stGPSInfo.m_nLATIPrecision,
												   		nBuf[1],
												   		g_stGPSInfo.m_nLONGDegree,
												   		g_stGPSInfo.m_nLONGMinute,
												   		g_stGPSInfo.m_nLONGPrecision,
												   		nBuf[2],
												   		(float)g_stGPSInfo.m_nCurSpeed/10,
												   		g_stGPSInfo.m_nCurDirection,
												   	    g_stGPSInfo.m_nDay,
												   	    g_stGPSInfo.m_nMonth,
												   	    g_stGPSInfo.m_nYear );
	nResult = strlen( (char*)pDest );
	
	nCheck = GPS_CountVerifySum( pDest+1, nResult-1 );
	
	sprintf( (char*)pDest+nResult, "*%02X\r\n", nCheck ); 
	
	nResult = strlen( (char*)pDest );
	
	return nResult;
}
/////////////////////////////////////////////////////////////////////////
//	����GPS���ݼ����
unsigned char  GPS_CountVerifySum( unsigned char	*pSrc, unsigned char	nLen ){
	
	unsigned char	nCnt = 0;
	unsigned char	nCheckData=0;
	
	for ( nCnt = 0; nCnt < nLen; nCnt++ ){
		nCheckData ^=pSrc[nCnt];
	}
	return nCheckData;
}
