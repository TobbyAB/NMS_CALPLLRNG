#ifndef _SPI_H_
#define _SPI_H_

#include "stm8l15x.h"

#define nSEL_Dir_out()         GPIO_Init(GPIOB, GPIO_Pin_3, GPIO_Mode_Out_PP_High_Fast)
#define SpiCLKOut()            GPIO_Init(GPIOB, GPIO_Pin_5, GPIO_Mode_Out_PP_Low_Fast)
#define SpiSDOOut()            GPIO_Init(GPIOB, GPIO_Pin_6, GPIO_Mode_Out_PP_High_Fast)
#define SpiSDIIn()             GPIO_Init(GPIOB, GPIO_Pin_7, GPIO_Mode_In_FL_No_IT)

#define SDI              (GPIOB->IDR & (uint8_t)GPIO_Pin_7)
#define SDO(n)           GPIO_WriteBit(GPIOB,GPIO_Pin_6,(BitAction)n)

#define SCK(n)              GPIO_WriteBit(GPIOB,GPIO_Pin_5,(BitAction)n)
#define CSN(n)              GPIO_WriteBit(GPIOB,GPIO_Pin_3,(BitAction)n)


void SPI_init(void);

void SpiWriteSingleAddressRegister(unsigned char Addr, unsigned char Data);
void SpiWriteLongAddressRegister(unsigned int Addr, unsigned char Data);
unsigned char SpiReadSingleAddressRegister(unsigned char Addr);  
unsigned char SpiReadLongAddressRegister(unsigned int Addr);


unsigned char SpiReadAutoAddressRegister(unsigned int Addr);
void SpiWriteAutoAddressRegister(unsigned int Addr,unsigned char Data);



#endif




