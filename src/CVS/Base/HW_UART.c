/****************************************Copyright (c)**************************************************
**描        述: μCOS-II下LPC210x的UART底层驱动 
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
** 函数名称	: WriteFull
** 功能描述	: 数据队列写满处理程序 
** 输　入		: Buf:指向数据队列
**        		Data:将要入队的数据
**        		Mod: 入队方式
** 输　出		:	TRUE  :成功
**        		FALSE:失败
** 全局变量: 无
** 调用模块: 无
********************************************************************************************************/
char  WriteFull(DataQueue *Buf, QUEUE_DATA_TYPE Data, char  Mod)
{
    unsigned short temp;
    
    Mod = Mod;
    
    temp = QueueSize((void *)Buf);
    while (temp <= QueueNData((void *)Buf))         /* 等待数据队列不满 */
    {
        OSTimeDly(2);
    }
    return QueueWrite((void *)Buf, Data);           /* 数据重新入队 */
}

/*********************************************************************************************************
** 函数名称: UART0Init
** 功能描述: 初始化UART0 
** 输　入: bps：波特率
**
** 输　出:TRUE  :成功
**        FALSE:失败
** 全局变量: Uart0Sem
** 调用模块: QueueCreate,OSSemCreate
********************************************************************************************************/
char  UART0Init(unsigned int bps)
{
    unsigned short Fdiv;
    
    PINSEL0 = (PINSEL0 & 0xfffffff0) | 0x05;    /* 选择管脚为UART0 */

    U0LCR = 0x80;                               /* 允许访问分频因子寄存器 */
    Fdiv = (Fpclk / 16) / bps;                  /* 设置波特率 */
    U0DLM = Fdiv / 256;							
	  U0DLL = Fdiv % 256;						
    U0LCR = 0x03;                               /* 禁止访问分频因子寄存器 */
                                                /* 且设置为8,1,n */
	  U0IER = 0x05;                               /* 允许接收和发送中断 */
    U0FCR = 0x87;                               /* 初始化FIFO */
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
** 函数名称: UART0Putch
** 功能描述: 发送一个字节数据
** 输　入: Data：发送的数据数据
** 输　出:无
** 全局变量: 无
** 调用模块: 无
********************************************************************************************************/
char UART0Putch(unsigned char  Data)
{
	unsigned char nResult = 0;
    char  temp;
    
    QueueWrite((void *)UART0SendBuf, Data);         /* 数据入队 */
    OS_ENTER_CRITICAL();
    if ((U0LSR & 0x00000020) != 0)
    {                                               /* UART0发送保持寄存器空 */
        QueueRead(&temp, UART0SendBuf);             /* 发送最初入队的数据 */
        U0THR = temp;
        U0IER = U0IER | 0x02;                       /* 允许发送中断 */
        nResult = 1;
    }
    return nResult;
    OS_EXIT_CRITICAL();    
} 
/*********************************************************************************************************
** 函数名称: UART0Write
** 功能描述: 发送多个字节数据
** 输　入: Data:发送数据存储位置
**        NByte:发送数据个数
** 输　出:无
** 全局变量: 无
** 调用模块: UART0Putch
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
** 函数名称: UART0Getch
** 功能描述: 接收一个字节
** 输　入: 无
** 输　出: 接收到的数据
** 全局变量: 无
** 调用模块: 无
********************************************************************************************************/
char  UART0Getch( unsigned char *err, unsigned char nTimeout )
{
	char		nRtn = 0;

    OS_ENTER_CRITICAL();
    if ((U0LSR & 0x00000001) == 0)
    {                                           /* 没有收到数据 */
        U0IER = U0IER | 0x01;                   /* 允许接收中断 */
        OSSemPend(Uart0Sem, nTimeout, err);           /* 等待接收数据 */
    }
   	nRtn = U0RBR;                            /* 读取收到的数据 */
    OS_EXIT_CRITICAL();
    return nRtn;
} 
/*********************************************************************************************************
** 函数名称: UART0_Exception
** 功能描述: UART0中断服务程序
** 输　入: 无
**
** 输　出: 无
**         
** 全局变量: 无
** 调用模块: QueueRead,OSSemPost
********************************************************************************************************/
void UART0_Exception(void)
{
    char  IIR, temp;
    
    OS_ENTER_CRITICAL();

    while(((IIR = U0IIR) & 0x01) == 0)
    {                                                   /* 有中断未处理完 */
        switch (IIR & 0x0e){
        case 0x02:                                  /* THRE中断    */
            if (QueueRead(&temp, UART0SendBuf) == QUEUE_OK)
            {
                U0THR = temp;
            }
            else
            {
                U0IER = U0IER & (~0x02);        /* 队列空，则禁止发送中断 */
            }
            break;
        case 0x04:                                  /* 接收数据可用 */
            OSSemPost(Uart0Sem);                    /* 通知接收任务 */
            U0IER = U0IER & (~0x01);                /* 禁止接收及字符超时中断 */
            break;
        case 0x06:                                  /* 接收线状态   */
            temp = U0LSR;
            break;
        case 0x0c:                                  /* 字符超时指示 */
            OSSemPost(Uart0Sem);                    /* 通知接收任务 */
            U0IER = U0IER & (~0x01);                /* 禁止接收及字符超时中断 */
            break;
        default :
            break;
        }
    } 
    VICVectAddr = 0;            // 通知中断控制器中断结束
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
** 函数名称: UART1Init
** 功能描述: 初始化UART1 
** 输　入: bps：波特率
**
** 输　出:TRUE  :成功
**        FALSE:失败
** 全局变量: Uart1Sem
** 调用模块: QueueCreate,OSSemCreate
********************************************************************************************************/
char  UART1Init(unsigned int bps)
{
	unsigned short Fdiv;
	
	PINSEL0 = (PINSEL0 & 0xfff0ffff) | 0x050000;    /* 选择管脚为UART1 */
	
	U1LCR = 0x80;                               /* 允许访问分频因子寄存器 */
	Fdiv = (Fpclk / 16) / bps;                  /* 设置波特率 */
	U1DLM = Fdiv / 256;							
	U1DLL = Fdiv % 256;						
	U1LCR = 0x03;                               /* 禁止访问分频因子寄存器 */
	                                            /* 且设置为8,1,n */
	U1IER = 0x05;                               /* 允许接收和发送中断 */
	U1FCR = 0x87;                               /* 初始化FIFO */
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
** 函数名称: UART1Putch
** 功能描述: 发送一个字节数据
** 输　入: Data：发送的数据数据
** 输　出:无
** 全局变量: 无
** 调用模块: 无
********************************************************************************************************/
char UART1Putch(unsigned char  Data)
{
	unsigned char nResult = 0;
    char  temp;
    
    QueueWrite((void *)UART1SendBuf, Data);         /* 数据入队 */
    OS_ENTER_CRITICAL();
    if ((U1LSR & 0x00000020) != 0)
    {                                               /* UART0发送保持寄存器空 */
        QueueRead(&temp, UART1SendBuf);             /* 发送最初入队的数据 */
        U1THR = temp;
        U1IER = U1IER | 0x02;                       /* 允许发送中断 */
        nResult = 1;
    }
    return nResult;
    OS_EXIT_CRITICAL();
} 

/*********************************************************************************************************
** 函数名称: UART1Write
** 功能描述: 发送多个字节数据
** 输　入: Data:发送数据存储位置
**        NByte:发送数据个数
** 输　出:无
** 全局变量: 无
** 调用模块: UART1Putch
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
** 函数名称: UART1Getch
** 功能描述: 接收一个字节
** 输　入: 无
** 输　出: 接收到的数据
** 全局变量: 无
** 调用模块: 无
********************************************************************************************************/
char  UART1Getch( unsigned char *err, unsigned char nTimeout )
{
	char		nRtn = 0;
	
	OS_ENTER_CRITICAL();
	if ((U1LSR & 0x00000001) == 0)
	{                                           /* 没有收到数据 */
	    U1IER = U1IER | 0x01;                   /* 允许接收中断 */
	    OSSemPend(Uart1Sem, nTimeout, err);     /* 等待接收数据 */
	}
	nRtn = U1RBR;                                /* 读取收到的数据 */
	OS_EXIT_CRITICAL();
		
	return nRtn;
} 

/*********************************************************************************************************
** 函数名称: UART1_Exception
** 功能描述: UART1中断服务程序
** 输　入: 无
**
** 输　出: 无
**         
** 全局变量: 无
** 调用模块: QueueRead,OSSemPost
********************************************************************************************************/
void UART1_Exception(void)
{
    char  IIR, temp;
    
    OS_ENTER_CRITICAL();

    while(((IIR = U1IIR) & 0x01) == 0)
    {                                              	/* 有中断未处理完 */
        switch (IIR & 0x0e){
        case 0x02:                                  /* THRE中断    */
            if (QueueRead(&temp, UART1SendBuf) == QUEUE_OK)
            {
                U1THR = temp;
            }
            else
            {
                U1IER = U1IER & (~0x02);        /* 队列空，则禁止发送中断 */
            }
            break;
        case 0x04:                                  /* 接收数据可用 */
            OSSemPost(Uart1Sem);                    /* 通知接收任务 */
            U1IER = U1IER & (~0x01);                /* 禁止接收及字符超时中断 */
            break;
        case 0x06:                                  /* 接收线状态   */
            temp = U1LSR;
            break;
        case 0x0c:                                  /* 字符超时指示 */
            OSSemPost(Uart1Sem);                    /* 通知接收任务 */
            U1IER = U1IER & (~0x01);                /* 禁止接收及字符超时中断 */
            break;
        default :
            break;
        }
    } 
    VICVectAddr = 0;            // 通知中断控制器中断结束
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
