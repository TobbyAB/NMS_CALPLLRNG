
#ifndef __UART_H_
#define __UART_H_

typedef struct
{
  unsigned char start;
  unsigned char send_num;
  unsigned int time;
  
  unsigned char buf_len;
  unsigned char buf[32];
  unsigned char send_buf[32];
  unsigned char send_i;
}SUART_STRUCT;

extern SUART_STRUCT SUART;

void UartInit(void);
void UartDeInit(void);
unsigned char UartWork(void);

void UartSendStr(unsigned char * buf,unsigned char len);//PLLRNG打印打印字符串函数声明

#endif




