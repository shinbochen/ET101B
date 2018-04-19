/****************************************Copyright (c)**************************************************
**描        述: 数据队列的中间件
********************************************************************************************************/

#ifndef QUEUE_H_2008_04_08
#define QUEUE_H_2008_04_08

#ifndef NOT_OK
#define NOT_OK              0xff                        /* 参数错误                                     */
#endif

#define QUEUE_FULL          8                           /* 队列满                                       */
#define QUEUE_EMPTY         4                           /* 无数据                                       */
#define QUEUE_OK            1                           /* 操作成功                                     */

#define Q_WRITE_MODE        1                           /* 操作成功                                     */
#define Q_WRITE_FRONT_MODE  2                           /* 操作成功                                     */

#ifndef QUEUE_DATA_TYPE 
#define QUEUE_DATA_TYPE     char 
#endif

#define QUEUE_DATA_TYPE char 

#define EN_QUEUE_WRITE		1
#define EN_QUEUE_WRITE_FRONT	0
#define EN_QUEUE_NDATA		1
#define EN_QUEUE_SIZE		1
#define EN_QUEUE_FLUSH		1

typedef struct {
    QUEUE_DATA_TYPE     *Out;                   /* 指向数据输出位置         */
    QUEUE_DATA_TYPE     *In;                    /* 指向数据输入位置         */
    QUEUE_DATA_TYPE     *End;                   /* 指向Buf的结束位置        */
    unsigned short              NData;                  /* 队列中数据个数           */
    unsigned short              MaxData;                /* 队列中允许存储的数据个数 */
    
    char                (* ReadEmpty)();        /* 读空处理函数             */
    char                (* WriteFull)();        /* 写满处理函数             */
    QUEUE_DATA_TYPE     Buf[1];                 /* 存储数据的空间           */
} DataQueue;


#ifndef IN_QUEUE
        char  QueueCreate(void *Buf,
                          unsigned int SizeOfBuf,
                          char  (* ReadEmpty)(),
                          char  (* WriteFull)()
                          );
/*********************************************************************************************************
** 函数名称: QueueCreate
** 功能描述: 初始化数据队列
** 输　入: Buf      ：为队列分配的存储空间地址
**         SizeOfBuf：为队列分配的存储空间大小（字节）
**         ReadEmpty：为队列读空时处理程序
**         WriteFull：为队列写满时处理程序
** 输　出: NOT_OK  :参数错误
**         QUEUE_OK:成功
** 全局变量: 无
** 调用模块: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
********************************************************************************************************/

extern char  QueueRead(QUEUE_DATA_TYPE *Ret, void *Buf);
/*********************************************************************************************************
** 函数名称: QueueRead
** 功能描述: 获取队列中的数据
** 输　入: Ret:存储返回的消息的地址
**         Buf:指向队列的指针
** 输　出: NOT_OK     ：参数错误
**         QUEUE_OK   ：收到消息
**         QUEUE_EMPTY：队列空
** 全局变量: 无
** 调用模块: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
********************************************************************************************************/

extern char  QueueWrite(void *Buf, QUEUE_DATA_TYPE Data);
/*********************************************************************************************************
** 函数名称: QueueWrite
** 功能描述: FIFO方式发送数据
** 输　入: Buf :指向队列的指针
**         Data:发送的数据
** 输　出: NOT_OK   ：参数错误
**         QUEUE_FULL:队列满
**         QUEUE_OK  :发送成功
** 全局变量: 无
** 调用模块: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
********************************************************************************************************/

extern char  QueueWriteFront(void *Buf, QUEUE_DATA_TYPE Data);
/*********************************************************************************************************
** 函数名称: QueueWrite
** 功能描述: LIFO方式发送数据
** 输　入: Buf:指向队列的指针
**         Data:消息数据
** 输　出: QUEUE_FULL:队列满
**         QUEUE_OK:发送成功
** 全局变量: 无
** 调用模块: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
********************************************************************************************************/

extern unsigned short QueueNData(void *Buf);
/*********************************************************************************************************
** 函数名称: QueueNData
** 功能描述: 取得队列中数据数
** 输　入: Buf:指向队列的指针
** 输　出: 消息数
** 全局变量: 无
** 调用模块: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
********************************************************************************************************/

extern unsigned short QueueSize(void *Buf);
/*********************************************************************************************************
** 函数名称: QueueSize
** 功能描述: 取得队列总容量
** 输　入: Buf:指向队列的指针
** 输　出: 队列总容量
** 全局变量: 无
** 调用模块: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
********************************************************************************************************/

extern void QueueFlush(void *Buf);
/*********************************************************************************************************
** 函数名称: OSQFlush
** 功能描述: 清空队列
** 输　入: Buf:指向队列的指针
** 输　出: 无
** 全局变量: 无
** 调用模块: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
********************************************************************************************************/
/*********************************************************************************************************
** 函数名称: QueueRollBack
** 功能描述: In指针回滚几位
** 输　入: Buf:指向队列的指针
** 输　出: 无
** 全局变量: 无
** 调用模块: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
********************************************************************************************************/
extern void QueueRollBack(void *Buf, unsigned char nCnt );
#endif
#endif

//#define EN_QUEUE_WRITE            0     /* 禁止(0)或允许(1)FIFO发送数据       */
//#define EN_QUEUE_WRITE_FRONT      0     /* 禁止(0)或允许(1)LIFO发送数据       */
//#define EN_QUEUE_NDATA            0     /* 禁止(0)或允许(1)取得队列数据数目   */
//#define EN_QUEUE_SIZE             0     /* 禁止(0)或允许(1)取得队列数据总容量 */
//#define EN_QUEUE_FLUSH            0     /* 禁止(0)或允许(1)清空队列           */

/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/
