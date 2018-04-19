/****************************************Copyright (c)**************************************************
**��        ��: lpc22xx�������ֵ�ARM��Ŀ�������Ĵ��룬�����쳣��������Ŀ����ʼ������
**              ÿ������Ӧ����������ļ��Ŀ������û����ݳ������Ҫ�޸ı��ļ���
********************************************************************************************************/
#include 	"..\hdr\config.h"
#include	"..\hdr\HW_GPIO.h"
#include	"..\hdr\HW_UART.h"
#include  "..\hdr\APP_GTM900.h"
#include  "..\hdr\APP_BQM26.h"
#include  "..\hdr\APP_EM200.h"
#include  "..\hdr\APP_MC323.h"
#include  "..\hdr\APP_MG323.h"
#include "..\hdr\APP_SIMCOM.h"
#include  "..\hdr\APP_GSM.h"
#include  "..\hdr\APP_Terminal.h"
#include  "..\hdr\HW_SPI.h"

extern  OS_EVENT 			*pGSMReceiveSem;
extern  OS_EVENT 			*pGPSReceiveSem;
/*********************************************************************************************************
** ��������: IRQ_Exception
** ��������: �ж��쳣��������û�������Ҫ�Լ��ı����
**
** �䡡��: ��
**
** �䡡��: ��
**         
** ȫ�ֱ���: ��
** ����ģ��: ��
********************************************************************************************************/
void IRQ_Exception(void)
{
	// shinboAddTEST
	VICVectAddr = 0;            // ֪ͨ�жϿ������жϽ���
    //while(1);                   // ��һ���滻Ϊ�Լ��Ĵ���
}

/*********************************************************************************************************
** ��������: FIQ_Exception
** ��������: �����ж��쳣��������û�������Ҫ�Լ��ı����
**           
** �䡡��: ��
**
** �䡡��: ��
**         
** ȫ�ֱ���: ��
** ����ģ��: ��
********************************************************************************************************/
void FIQ_Exception(void)
{
	// shinboAddTEST
	VICVectAddr = 0;            // ֪ͨ�жϿ������жϽ���
    //while(1);                   // ��һ���滻Ϊ�Լ��Ĵ���
}

/*********************************************************************************************************
** ��������: Timer0_Exception
** ��������: ��ʱ��0�жϷ������
** �䡡��: ��
**
** �䡡��: ��
**         
** ȫ�ֱ���: ��
** ����ģ��: OSTimeTick
********************************************************************************************************/
void Timer0_Exception(void)
{
    T0IR = 0x01;
    VICVectAddr = 0;            // ֪ͨ�жϿ������жϽ���
    OSTimeTick();
}

/*********************************************************************************************************
** ��������: Timer0Init
** ��������: ��ʱ��0��ʼ��
** �䡡��: ��
**
** �䡡��: ��
**         
** ȫ�ֱ���: ��
** ����ģ��: ��
********************************************************************************************************/
void Timer0Init(void)
{
    T0IR = 0xffffffff;
    T0TC = 0;
    T0TCR = 0x01;
    T0MCR = 0x03;
    T0MR0 = (Fpclk / OS_TICKS_PER_SEC);
 }

/*********************************************************************************************************
** ��������: VICInit
** ��������: �����жϿ�������ʼ��
** �䡡��: ��
**
** �䡡��: ��
**         
** ȫ�ֱ���: ��
** ����ģ��: ��
********************************************************************************************************/
void VICInit(void)
{
    extern void IRQ_Handler(void);
    extern void Timer0_Handler(void);
    extern void UART0_Handler(void);
    extern void UART1_Handler(void);

    VICIntEnClr = 0xffffffff;
    VICDefVectAddr = (unsigned int)IRQ_Handler;
    
    VICVectAddr13 = (unsigned int)UART0_Handler;
    VICVectCntl13 = (0x20 | 0x06);
    VICIntEnable = 1 << 6;

    VICVectAddr14 = (unsigned int)UART1_Handler;
    VICVectCntl14 = (0x20 | 0x07);
    VICIntEnable = 1 << 7;

    VICVectAddr0 = (unsigned int)Timer0_Handler;
    VICVectCntl0 = (0x20 | 0x04);
    VICIntEnable = 1 << 4;
 }

/*********************************************************************************************************
** ��������: InitHardware
** ��������: Ŀ����ʼ�����룬����Ҫ�ĵط����ã�������Ҫ�ı�
** �䡡��: ��
**
** �䡡��: ��
**         
** ȫ�ֱ���: ��
** ����ģ��: ��
********************************************************************************************************/
void InitHardware(void)
{
  OS_ENTER_CRITICAL();
  
  srand((unsigned int) InitHardware);
  
	pGSMReceiveSem =  OSSemCreate(0);
	pGPSReceiveSem = OSSemCreate (0); 
  
  

  
   
  VICInit();   
  Timer0Init(); 
  GPSUARTInit(4800);		// GPS ����  
  
    
 	#if	(EXTERNAL_FLASH)
  SPI0Init();
  #endif
  CMM_HWInit( );
	
	// ��·����
	GPIO_Enable(HW_PORT_CTRLOIL);			
	GPIO_SetDirection(HW_PORT_CTRLOIL, HW_PORT_OUTPUT);
	HW_CTRLOIL_RESUME( ); 

	// GPS LED����
	GPIO_Enable(HW_PORT_GPSLED);			
	GPIO_SetDirection(HW_PORT_GPSLED, HW_PORT_OUTPUT);
	HW_GPSLED_OFF();

	// ACC���
	GPIO_Enable(HW_PORT_ACC_I);			
	GPIO_SetDirection(HW_PORT_ACC_I, HW_PORT_INPUT);

	// ��������
	GPIO_Enable(HW_PORT_SOS_I);			
	GPIO_SetDirection(HW_PORT_SOS_I, HW_PORT_INPUT);	

	// �ż��
	GPIO_Enable(HW_PORT_DOOR_I);						
	GPIO_SetDirection(HW_PORT_DOOR_I, HW_PORT_INPUT);	
		
	
	#if( HW_ET201B || HW_ET201B_2 || HW_ET201B_3 )
	// ����
	GPIO_Enable(HW_PORT_UNLOCK);
	HW_CENTER_UNLOOK_NACTION();
	GPIO_SetDirection(HW_PORT_UNLOCK, HW_PORT_OUTPUT ); 
	HW_CENTER_UNLOOK_NACTION();	
	// ����
	GPIO_Enable(HW_PORT_LOCK);
	HW_CENTER_LOOK_NACTION();
	GPIO_SetDirection(HW_PORT_LOCK, HW_PORT_OUTPUT );
	HW_CENTER_LOOK_NACTION();	
	// ����
	GPIO_Enable(HW_PORT_SPK_MUTE);
	GPIO_SetDirection(HW_PORT_SPK_MUTE, HW_PORT_OUTPUT );	
	// �Զ���1
	GPIO_Enable(HW_PORT_CUSTOM1);
	GPIO_SetDirection(HW_PORT_CUSTOM1, HW_PORT_INPUT );
	// �Զ���2
	GPIO_Enable(HW_PORT_CUSTOM2);
	GPIO_SetDirection(HW_PORT_CUSTOM2, HW_PORT_INPUT );
	//�񶯱���
	GPIO_Enable(HW_PORT_SHOCKING);
	GPIO_SetDirection(HW_PORT_SHOCKING, HW_PORT_INPUT );
	#endif 
		
	// DTR����	
	#if( HW_EM200 )	
	GPIO_Enable(HW_PORT_UART_DTR);
	GPIO_SetDirection(HW_PORT_UART_DTR, HW_PORT_OUTPUT );	
	#endif	//HW_EM200	
	// 	
	HW_GPSLED_ON();
	
	#if (HW_ET601)
	GPIO_Enable(HW_G_SELECT);
	GPIO_SetDirection(HW_G_SELECT, HW_PORT_OUTPUT );	
	HW_G_SELECT_HIGH();
	
	GPIO_Enable(HW_G_DETECT);
	GPIO_SetDirection(HW_G_DETECT, HW_PORT_OUTPUT );	
	HW_G_DETECT_HIGH();
	#endif 

	OS_EXIT_CRITICAL(); 	 
}

/*********************************************************************************************************
** ��������: TargetResetInit
** ��������: ����main����ǰĿ����ʼ�����룬������Ҫ�ı䣬����ɾ��
** �䡡��: ��
**
** �䡡��: ��
**         
** ȫ�ֱ���: ��
** ����ģ��: ��
********************************************************************************************************/
void TargetResetInit(void)
{
#ifdef __DEBUG    
    MEMMAP = 0x3;                   //remap
#endif

#ifdef __OUT_CHIP    
    MEMMAP = 0x3;                   //remap
#endif

#ifdef __IN_CHIP    
    MEMMAP = 0x1;                   //remap
#endif

    PINSEL0 = (PINSEL0 & 0xFFFF0000) | 0x05 | 0x50;

/* ����ϵͳ������ʱ�� */
    PLLCON = 1;
#if (Fpclk / (Fcclk / 4)) == 1
    VPBDIV = 0;
#endif
#if (Fpclk / (Fcclk / 4)) == 2
    VPBDIV = 2;
#endif
#if (Fpclk / (Fcclk / 4)) == 4
    VPBDIV = 1;
#endif

#if (Fcco / Fcclk) == 2
    PLLCFG = ((Fcclk / Fosc) - 1) | (0 << 5);
#endif
#if (Fcco / Fcclk) == 4
    PLLCFG = ((Fcclk / Fosc) - 1) | (1 << 5);
#endif
#if (Fcco / Fcclk) == 8
    PLLCFG = ((Fcclk / Fosc) - 1) | (2 << 5);
#endif
#if (Fcco / Fcclk) == 16
    PLLCFG = ((Fcclk / Fosc) - 1) | (3 << 5);
#endif
    PLLFEED = 0xaa;
    PLLFEED = 0x55;
    while((PLLSTAT & (1 << 10)) == 0);
    PLLCON = 3;
    PLLFEED = 0xaa;
    PLLFEED = 0x55;

/* ���ô洢������ģ�� */
    MAMCR = 0;
#if Fcclk < 20000000
    MAMTIM = 1;
#else
#if Fcclk < 40000000
    MAMTIM = 2;
#else
    MAMTIM = 3;
#endif
#endif
    MAMCR = 2;

/* ����ʵʱʱ�� */
    CCR = 1;
    PREINT = Fpclk / 32768 - 1;
    PREFRAC = Fpclk - (Fpclk / 32768) * 32768;
    YEAR = 2003;
    MONTH = 6;
    DOM = 2;
    

    VICIntEnClr = 0xffffffff;
    VICVectAddr = 0;
    VICIntSelect = 0;
    T0IR = 0xffffffff;
    T0TCR = 0X02;
}
/////////////////////////////////////////////////////
//�����ڲ�оƬι��
void	SW_WatchdogEnable(void)	{
	
	WDMOD = 0x0;
	WDTC = 165888000;
	WDMOD = 0x3;
}
///////////////////////////////////////////
//ι��һ��
void SW_Watchdog(void){

	OS_ENTER_CRITICAL();
	WDFEED = 0xaa;
	WDFEED = 0x55;
	OS_EXIT_CRITICAL();
}
/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/
