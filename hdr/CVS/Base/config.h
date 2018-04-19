/****************************************Copyright (c)**************************************************
**��        ��: �û������ļ�
********************************************************************************************************/

#ifndef CONFIG_H
#define CONFIG_H

#ifndef TRUE
#define TRUE  1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef NULL
#define NULL 0
#endif 

#define BIT0			0x1
#define BIT1			0x2
#define BIT2			0x4
#define BIT3			0x8
#define BIT4			0x10
#define BIT5			0x20
#define BIT6			0x40
#define BIT7			0x80
#define BIT8			0x100
#define BIT9			0x200
#define BIT10			0x400
#define BIT11			0x800
#define BIT12			0x1000
#define BIT13			0x2000
#define BIT14			0x4000
#define BIT15			0x8000
#define BIT16			0x10000
#define BIT17			0x20000
#define BIT18			0x40000
#define BIT19			0x80000
#define BIT20			0x100000
#define BIT21			0x200000
#define BIT22			0x400000
#define BIT23			0x800000
#define BIT24			0x1000000
#define BIT25			0x2000000
#define BIT26			0x4000000
#define BIT27			0x8000000
#define BIT28			0x10000000
#define BIT29			0x20000000
#define BIT30			0x40000000
#define BIT31			0x80000000

#ifndef bool
typedef unsigned char  bool;
#endif

/********************************/
/*      uC/OS-II���������      */
/* �û�ģʽ(0x10),ARM����       */
/* ֻ����0x10,0x30,0x1f,0x3f֮һ*/
/********************************/
#define  USER_USING_MODE    0x10                    
#include "..\hdr\Includes.h"
/********************************/
/*      ARM���������           */
/********************************/
#include    "LPC2294.h"
/********************************/
/*     Ӧ�ó�������             */
/********************************/
#include    <stdio.h>
#include    <ctype.h>
#include    <stdlib.h>
#include    <math.h>
#include    <setjmp.h>
#include    <rt_misc.h>
#include	<time.h>
#include	"queue.h"
#include    "target.h"

/* ϵͳ����, Fosc��Fcclk��Fcco��Fpclk���붨��*/
#define Fosc            11059200                    //����Ƶ��,10MHz~25MHz��Ӧ����ʵ��һ��
#define Fcclk           (Fosc * 4)                  //ϵͳƵ�ʣ�����ΪFosc��������(1~32)����<=60MHZ
#define Fcco            (Fcclk * 4)                 //CCOƵ�ʣ�����ΪFcclk��2��4��8��16������ΧΪ156MHz~320MHz
#define Fpclk           (Fcclk / 4) * 1             //VPBʱ��Ƶ�ʣ�ֻ��Ϊ(Fcclk / 4)��1 ~ 4��

#endif
/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/
