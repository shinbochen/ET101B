//////////////////////////////////////////////////////////////////////////
// file name: HW_UART.h
// Date: 2009-02-04
// Author: real optimize
#ifndef _HW_UART_H_
#define _HW_UART_H_

#define UART0_SEND_QUEUE_LENGTH 	60
#define UART1_SEND_QUEUE_LENGTH 	60

char UART0Init(unsigned int bps);
char UART0Putch(unsigned char  Data);
void UART0Write(unsigned char *Data, unsigned short NByte);
void UART0SendStr(unsigned char * pCp);
//char UART0Getch(void);
char UART0Getch( unsigned char *err, unsigned char nTimeout );
char IsUART0SendBufEmpty( void );
void UART0_Exception(void);

char UART1Init(unsigned int 	bps);
char UART1Putch(unsigned char  Data);
void UART1Write(unsigned char *Data, unsigned short NByte);
void UART1SendStr(unsigned char *pStr);
//char UART1Getch(void);      
char UART1Getch( unsigned char *err, unsigned char nTimeout );
void UART1_Exception(void);
char IsUART1SendBufEmpty( void );

#endif
