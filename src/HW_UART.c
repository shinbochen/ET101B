/****************************************Copyright (c)**************************************************
**��        ��: ��COS-II��LPC210x��UART�ײ����� 
********************************************************************************************************/
//////////////////////////////////////////////////////////////////
// include file declare

#include "..\hdr\config.h"
#include "..\hdr\HW_UART.h"

//////////////////////////////////////////////////////////////////
// function declare

static int UART0SendBuf[(UART0_SEND_QUEUE_LENGTH + sizeof(int) - 1) / sizeof(int)];
static int UART1SendBuf[(UART1_SEND_QUEUE_LENGTH + sizeof(int) - 1) / sizeof(int)];

static OS_EVENT *Uart0Sem;
static OS_EVENT *Uart1Sem;


/*********************************************************************************************************
** ��������	: WriteFull
** ��������	: ���ݶ���д��������� 
** �䡡��		: Buf:ָ�����ݶ���
**        		Data:��Ҫ��ӵ�����
**        		Mod: ��ӷ�ʽ
** �䡡��		:	TRUE  :�ɹ�
**        		FALSE:ʧ��
** ȫ�ֱ���: ��
** ����ģ��: ��
********************************************************************************************************/
char  WriteFull(DataQueue *Buf, QUEUE_DATA_TYPE Data, char  Mod)
{
    unsigned short temp;
    
    Mod = Mod;
    
    temp = QueueSize((void *)Buf);
    while (temp <= QueueNData((void *)Buf))         /* �ȴ����ݶ��в��� */
    {
        OSTimeDly(2);
    }
    return QueueWrite((void *)Buf, Data);           /* ����������� */
}

/*********************************************************************************************************
** ��������: UART0Init
** ��������: ��ʼ��UART0 
** �䡡��: bps��������
**
** �䡡��:TRUE  :�ɹ�
**        FALSE:ʧ��
** ȫ�ֱ���: Uart0Sem
** ����ģ��: QueueCreate,OSSemCreate
********************************************************************************************************/
char  UART0Init(unsigned int bps)
{
    unsigned short Fdiv;
    
    PINSEL0 = (PINSEL0 & 0xfffffff0) | 0x05;    /* ѡ��ܽ�ΪUART0 */

    U0LCR = 0x80;                               /* ������ʷ�Ƶ���ӼĴ��� */
    Fdiv = (Fpclk / 16) / bps;                  /* ���ò����� */
    U0DLM = Fdiv / 256;							
	  U0DLL = Fdiv % 256;						
    U0LCR = 0x03;                               /* ��ֹ���ʷ�Ƶ���ӼĴ��� */
                                                /* ������Ϊ8,1,n */
	  U0IER = 0x05;                               /* ������պͷ����ж� */
    U0FCR = 0x87;                               /* ��ʼ��FIFO */
    if (QueueCreate((void *)UART0SendBuf,
                     sizeof(UART0SendBuf),
                     NULL,
                     (char  (*)())WriteFull)
                     == NOT_OK)
    {
        return FALSE;
    }
    Uart0Sem = OSSemCreate(0);
    if (Uart0Sem != NULL)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }

} 

/*********************************************************************************************************
** ��������: UART0Putch
** ��������: ����һ���ֽ�����
** �䡡��: Data�����͵���������
** �䡡��:��
** ȫ�ֱ���: ��
** ����ģ��: ��
********************************************************************************************************/
char UART0Putch(unsigned char  Data)
{
	unsigned char nResult = 0;
    char  temp;
    
    QueueWrite((void *)UART0SendBuf, Data);         /* ������� */
    OS_ENTER_CRITICAL();
    if ((U0LSR & 0x00000020) != 0)
    {                                               /* UART0���ͱ��ּĴ����� */
        QueueRead(&temp, UART0SendBuf);             /* ���������ӵ����� */
        U0THR = temp;
        U0IER = U0IER | 0x02;                       /* �������ж� */
        nResult = 1;
    }
    return nResult;
    OS_EXIT_CRITICAL();    
} 
/*********************************************************************************************************
** ��������: UART0Write
** ��������: ���Ͷ���ֽ�����
** �䡡��: Data:�������ݴ洢λ��
**        NByte:�������ݸ���
** �䡡��:��
** ȫ�ֱ���: ��
** ����ģ��: UART0Putch
********************************************************************************************************/
void UART0Write(unsigned char *pData, unsigned short NByte)
{
    OS_ENTER_CRITICAL();
    while (NByte > 0)
    {
        UART0Putch(*pData);
        NByte--;
        pData++;
    }
    OS_EXIT_CRITICAL();
}

/*********************************************************************************************************
** ��������: UART0Getch
** ��������: ����һ���ֽ�
** �䡡��: ��
** �䡡��: ���յ�������
** ȫ�ֱ���: ��
** ����ģ��: ��
********************************************************************************************************/
char  UART0Getch( unsigned char *err, unsigned char nTimeout )
{
	char		nRtn = 0;

    OS_ENTER_CRITICAL();
    if ((U0LSR & 0x00000001) == 0)
    {                                           /* û���յ����� */
        U0IER = U0IER | 0x01;                   /* ��������ж� */
        OSSemPend(Uart0Sem, nTimeout, err);           /* �ȴ��������� */
    }
   	nRtn = U0RBR;                            /* ��ȡ�յ������� */
    OS_EXIT_CRITICAL();
    return nRtn;
} 
/*********************************************************************************************************
** ��������: UART0_Exception
** ��������: UART0�жϷ������
** �䡡��: ��
**
** �䡡��: ��
**         
** ȫ�ֱ���: ��
** ����ģ��: QueueRead,OSSemPost
********************************************************************************************************/
void UART0_Exception(void)
{
    char  IIR, temp;
    
    OS_ENTER_CRITICAL();

    while(((IIR = U0IIR) & 0x01) == 0)
    {                                                   /* ���ж�δ������ */
        switch (IIR & 0x0e){
        case 0x02:                                  /* THRE�ж�    */
            if (QueueRead(&temp, UART0SendBuf) == QUEUE_OK)
            {
                U0THR = temp;
            }
            else
            {
                U0IER = U0IER & (~0x02);        /* ���пգ����ֹ�����ж� */
            }
            break;
        case 0x04:                                  /* �������ݿ��� */
            OSSemPost(Uart0Sem);                    /* ֪ͨ�������� */
            U0IER = U0IER & (~0x01);                /* ��ֹ���ռ��ַ���ʱ�ж� */
            break;
        case 0x06:                                  /* ������״̬   */
            temp = U0LSR;
            break;
        case 0x0c:                                  /* �ַ���ʱָʾ */
            OSSemPost(Uart0Sem);                    /* ֪ͨ�������� */
            U0IER = U0IER & (~0x01);                /* ��ֹ���ռ��ַ���ʱ�ж� */
            break;
        default :
            break;
        }
    } 
    VICVectAddr = 0;            // ֪ͨ�жϿ������жϽ���
    OS_EXIT_CRITICAL();
}

/////////////////////////////////////////////////////////////////
//
void UART0SendStr(unsigned char * pStr )
{
	OS_ENTER_CRITICAL();

	while( *pStr ){
	
		UART0Putch( *pStr );
		pStr++;		
	}	
	OS_EXIT_CRITICAL();
}
/////////////////////////////////////////////////////////////////
//
char IsUART0SendBufEmpty( void ){
	if( QueueNData( (void *)UART0SendBuf ) == 0 ){	
		return 1;
	}
	else{
		return 0;
	}
}
/*********************************************************************************************************
** ��������: UART1Init
** ��������: ��ʼ��UART1 
** �䡡��: bps��������
**
** �䡡��:TRUE  :�ɹ�
**        FALSE:ʧ��
** ȫ�ֱ���: Uart1Sem
** ����ģ��: QueueCreate,OSSemCreate
********************************************************************************************************/
char  UART1Init(unsigned int bps)
{
	unsigned short Fdiv;
	
	PINSEL0 = (PINSEL0 & 0xfff0ffff) | 0x050000;    /* ѡ��ܽ�ΪUART1 */
	
	U1LCR = 0x80;                               /* ������ʷ�Ƶ���ӼĴ��� */
	Fdiv = (Fpclk / 16) / bps;                  /* ���ò����� */
	U1DLM = Fdiv / 256;							
	U1DLL = Fdiv % 256;						
	U1LCR = 0x03;                               /* ��ֹ���ʷ�Ƶ���ӼĴ��� */
	                                            /* ������Ϊ8,1,n */
	U1IER = 0x05;                               /* ������պͷ����ж� */
	U1FCR = 0x87;                               /* ��ʼ��FIFO */
	if (QueueCreate((void *)UART1SendBuf,
	                 sizeof(UART1SendBuf),
	                 NULL,
	                 (char  (*)())WriteFull)
	                 == NOT_OK)
	{
	    return FALSE;
	}
	Uart1Sem = OSSemCreate(0);
	if (Uart1Sem != NULL)
	{
	    return TRUE;
	}
	else
	{
	    return FALSE;
	}

} 

/*********************************************************************************************************
** ��������: UART1Putch
** ��������: ����һ���ֽ�����
** �䡡��: Data�����͵���������
** �䡡��:��
** ȫ�ֱ���: ��
** ����ģ��: ��
********************************************************************************************************/
char UART1Putch(unsigned char  Data)
{
	unsigned char nResult = 0;
    char  temp;
    
    QueueWrite((void *)UART1SendBuf, Data);         /* ������� */
    OS_ENTER_CRITICAL();
    if ((U1LSR & 0x00000020) != 0)
    {                                               /* UART0���ͱ��ּĴ����� */
        QueueRead(&temp, UART1SendBuf);             /* ���������ӵ����� */
        U1THR = temp;
        U1IER = U1IER | 0x02;                       /* �������ж� */
        nResult = 1;
    }
    return nResult;
    OS_EXIT_CRITICAL();
} 

/*********************************************************************************************************
** ��������: UART1Write
** ��������: ���Ͷ���ֽ�����
** �䡡��: Data:�������ݴ洢λ��
**        NByte:�������ݸ���
** �䡡��:��
** ȫ�ֱ���: ��
** ����ģ��: UART1Putch
********************************************************************************************************/
void UART1Write(unsigned char *pData, unsigned short NByte)
{
    OS_ENTER_CRITICAL();
    while (NByte > 0) {
        UART1Putch(*pData);
        pData++;
        NByte--;
    }
    OS_EXIT_CRITICAL();
} 


/*********************************************************************************************************
** ��������: UART1Getch
** ��������: ����һ���ֽ�
** �䡡��: ��
** �䡡��: ���յ�������
** ȫ�ֱ���: ��
** ����ģ��: ��
********************************************************************************************************/
char  UART1Getch( unsigned char *err, unsigned char nTimeout )
{
	char		nRtn = 0;
	
	OS_ENTER_CRITICAL();
	if ((U1LSR & 0x00000001) == 0)
	{                                           /* û���յ����� */
	    U1IER = U1IER | 0x01;                   /* ��������ж� */
	    OSSemPend(Uart1Sem, nTimeout, err);     /* �ȴ��������� */
	}
	nRtn = U1RBR;                                /* ��ȡ�յ������� */
	OS_EXIT_CRITICAL();
		
	return nRtn;
} 

/*********************************************************************************************************
** ��������: UART1_Exception
** ��������: UART1�жϷ������
** �䡡��: ��
**
** �䡡��: ��
**         
** ȫ�ֱ���: ��
** ����ģ��: QueueRead,OSSemPost
********************************************************************************************************/
void UART1_Exception(void)
{
    char  IIR, temp;
    
    OS_ENTER_CRITICAL();

    while(((IIR = U1IIR) & 0x01) == 0)
    {                                              	/* ���ж�δ������ */
        switch (IIR & 0x0e){
        case 0x02:                                  /* THRE�ж�    */
            if (QueueRead(&temp, UART1SendBuf) == QUEUE_OK)
            {
                U1THR = temp;
            }
            else
            {
                U1IER = U1IER & (~0x02);        /* ���пգ����ֹ�����ж� */
            }
            break;
        case 0x04:                                  /* �������ݿ��� */
            OSSemPost(Uart1Sem);                    /* ֪ͨ�������� */
            U1IER = U1IER & (~0x01);                /* ��ֹ���ռ��ַ���ʱ�ж� */
            break;
        case 0x06:                                  /* ������״̬   */
            temp = U1LSR;
            break;
        case 0x0c:                                  /* �ַ���ʱָʾ */
            OSSemPost(Uart1Sem);                    /* ֪ͨ�������� */
            U1IER = U1IER & (~0x01);                /* ��ֹ���ռ��ַ���ʱ�ж� */
            break;
        default :
            break;
        }
    } 
    VICVectAddr = 0;            // ֪ͨ�жϿ������жϽ���
    OS_EXIT_CRITICAL();
}
//////////////////////////////////////////////////////////////////////////////////
//
void UART1SendStr( unsigned char * pStr ){
	
	OS_ENTER_CRITICAL();
	while( *pStr ){	
		
		UART1Putch( *pStr );		
		pStr++;
	}	
	OS_EXIT_CRITICAL(); 
}
/////////////////////////////////////////////////////////////////
//
char IsUART1SendBufEmpty( void ){
	if( QueueNData( (void *)UART1SendBuf ) == 0 ){	
		return 1;
	}
	else{
		return 0;
	}
}
/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/
