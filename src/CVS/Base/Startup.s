;/****************************************Copyright (c)**************************************************
;**描        述: lpc22xx的启动代码，包含异常向量入口、初始化堆栈的代码等
;**              每个工程应当有独立的这个文件的拷贝，并进行相应的修改   
;********************************************************************************************************/

;定义堆栈的大小
FIQ_STACK_LEGTH         EQU         0
IRQ_STACK_LEGTH         EQU         9*8             ;每层嵌套需要9个字堆栈，允许8层嵌套
ABT_STACK_LEGTH         EQU         0
UND_STACK_LEGTH         EQU         0

NoInt       EQU 0x80

USR32Mode   EQU 0x10
SVC32Mode   EQU 0x13
SYS32Mode   EQU 0x1f
IRQ32Mode   EQU 0x12
FIQ32Mode   EQU 0x11

PINSEL2     EQU 0xE002C014

BCFG0       EQU 0xFFE00000
BCFG1       EQU 0xFFE00004
BCFG2       EQU 0xFFE00008
BCFG3       EQU 0xFFE0000C


;引入的外部标号在这声明
    IMPORT  FIQ_Exception                   ;快速中断异常处理程序
    IMPORT  __main                          ;C语言主程序入口 
    IMPORT  TargetResetInit                 ;目标板基本初始化
    IMPORT  StackUsr
    IMPORT  bottom_of_heap
    IMPORT  SoftwareInterrupt

;给外部使用的标号在这声明
    EXPORT  Reset
    EXPORT  __rt_div0
    EXPORT  __user_initial_stackheap

    CODE32

    AREA    vectors,CODE,READONLY
        ENTRY
;中断向量表
Reset
        LDR     PC, ResetAddr
        LDR     PC, UndefinedAddr
        LDR     PC, SWI_Addr
        LDR     PC, PrefetchAddr
        LDR     PC, DataAbortAddr
        DCD     0xb9205f80
        LDR     PC, [PC, #-0xff0]
        LDR     PC, FIQ_Addr

ResetAddr           DCD     ResetInit
UndefinedAddr       DCD     Undefined
SWI_Addr            DCD     SoftwareInterrupt
PrefetchAddr        DCD     PrefetchAbort
DataAbortAddr       DCD     DataAbort
Nouse               DCD     0
IRQ_Addr            DCD     0
FIQ_Addr            DCD     FIQ_Handler

;未定义指令
Undefined
        B       Undefined

;取指令中止
PrefetchAbort
        B       PrefetchAbort

;取数据中止
DataAbort
        B       DataAbort

;快速中断
FIQ_Handler
        STMFD   SP!, {R0-R3, LR}
        BL      FIQ_Exception
        LDMFD   SP!, {R0-R3, LR}
        SUBS    PC,  LR,  #4

;/*********************************************************************************************************
;** 函数名称: InitStack
;** 功能描述: 初始化堆栈
;** 输　入:   无
;** 输　出 :  无
;** 全局变量: 无
;** 调用模块: 无
;********************************************************************************************************/
InitStack    
        MOV     R0, LR

;设置中断模式堆栈
        MSR     CPSR_c, #0xd2
        LDR     SP, StackIrq
;设置快速中断模式堆栈
        MSR     CPSR_c, #0xd1
        LDR     SP, StackFiq
;设置中止模式堆栈
        MSR     CPSR_c, #0xd7
        LDR     SP, StackAbt
;设置未定义模式堆栈
        MSR     CPSR_c, #0xdb
        LDR     SP, StackUnd
;设置系统模式堆栈
        MSR     CPSR_c, #0xdf
        LDR     SP, =StackUsr

        MOV     PC, R0

;/*********************************************************************************************************
;** 函数名称: ResetInit
;** 功能描述: 复位入口
;** 
;** 输　入: 无
;**
;** 输　出: 无
;**         
;** 全局变量: 无
;** 调用模块: 无
;********************************************************************************************************/
ResetInit
;初始化外部总线控制器，根据目标板决定配置

        LDR     R0, =PINSEL2
    IF :DEF: EN_CRP
        LDR     R1, =0x0f814910
    ELSE
        LDR     R1, =0x0f814914
    ENDIF
        STR     R1, [R0]

        BL      InitStack               ;初始化堆栈
        BL      TargetResetInit         ;目标板基本初始化
                                        ;跳转到c语言入口
        B       __main


;/*********************************************************************************************************
;** 函数名称: __user_initial_stackheap 
;** 功能描述: 库函数初始化堆和栈，不能删除
;** 
;** 输　入: 参考库函数手册
;**
;** 输　出: 参考库函数手册
;**         
;** 全局变量: 无
;** 调用模块: 无
;********************************************************************************************************/
__user_initial_stackheap    
;shinboAddTEST
	LDR   r0, =bottom_of_heap
	LDR	  r1, =StackUsr
    MOV   pc,lr


;/*********************************************************************************************************
;** 函数名称: __rt_div0
;** 功能描述: 整数除法除数为0错误处理函数，替代原始的__rt_div0减少目标代码大小
;** 
;** 输　入: 参考库函数手册
;**
;** 输　出: 无
;**         
;** 全局变量: 无
;** 调用模块: 无
;********************************************************************************************************/
__rt_div0
        B       __rt_div0

StackIrq           DCD     IrqStackSpace + (IRQ_STACK_LEGTH - 1)* 4
StackFiq           DCD     FiqStackSpace + (FIQ_STACK_LEGTH - 1)* 4
StackAbt           DCD     AbtStackSpace + (ABT_STACK_LEGTH - 1)* 4
StackUnd           DCD     UndtStackSpace + (UND_STACK_LEGTH - 1)* 4

;/*********************************************************************************************************
;** 函数名称: CrpData
;** 功能描述: 支持芯片加密功能
;** 
;** 输　入: 无
;**
;** 输　出: 无
;**         
;** 全局变量: 无
;** 调用模块: 无
;********************************************************************************************************/
    IF :DEF: EN_CRP
        IF  . >= 0x1fc
        INFO    1,"\nThe data at 0x000001fc must be 0x87654321.\nPlease delete some source before this line."
        ENDIF
CrpData
    WHILE . < 0x1fc
    NOP
    WEND
CrpData1
    DCD     0x87654321          ;/* 当此数为0x87654321时，用户程序被保护 */
    ENDIF

;/* 分配堆栈空间 */
        AREA    MyStacks, DATA, NOINIT, ALIGN=2
IrqStackSpace           SPACE   IRQ_STACK_LEGTH * 4  ;中断模式堆栈空间
FiqStackSpace           SPACE   FIQ_STACK_LEGTH * 4  ;快速中断模式堆栈空间
AbtStackSpace           SPACE   ABT_STACK_LEGTH * 4  ;中止义模式堆栈空间
UndtStackSpace          SPACE   UND_STACK_LEGTH * 4  ;未定义模式堆栈
    END
;/*********************************************************************************************************
;**                            End Of File
;********************************************************************************************************/
