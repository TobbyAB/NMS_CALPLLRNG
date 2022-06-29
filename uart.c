

/*******************************************************************************/
#include "stm8l15x.h"

#include "uart.h"

SUART_STRUCT SUART;

void UartInit(void)
{
  GPIO_Init(GPIOA, GPIO_Pin_3, GPIO_Mode_In_PU_No_IT);
  GPIO_Init(GPIOA, GPIO_Pin_2, GPIO_Mode_Out_PP_High_Slow);//UART-TX
  CLK_PeripheralClockConfig (CLK_Peripheral_USART1,ENABLE);//开启USART时钟
  SYSCFG_REMAPPinConfig(REMAP_Pin_USART1TxRxPortA,ENABLE);//端口重映射，去掉注释之后USART1为PA2-TX、PA3-RX；注释之后USART1为TX-PC5、RX-PC6；复位之后USART会自动恢复至PC5、PC6
  
  USART_Init(USART1,115200,USART_WordLength_8b,USART_StopBits_1,USART_Parity_No,(USART_Mode_TypeDef)(USART_Mode_Tx|USART_Mode_Rx));//设置USART参数600，8N1，接收/发送
  
  USART_ITConfig (USART1,USART_IT_RXNE,ENABLE);//使能接收中断
  USART_Cmd (USART1,ENABLE);//使能USART
  
  USART_ClearITPendingBit (USART1,USART_IT_RXNE);//清中断标志
  USART_ReceiveData8 (USART1);
  
  enableInterrupts(); 
}

void UartDeInit(void)
{
  //USART_ITConfig (USART1,USART_IT_RXNE,ENABLE);//~使能接收中断
  USART_Cmd (USART1,DISABLE);//~使能USART
  CLK_PeripheralClockConfig (CLK_Peripheral_USART1,DISABLE);//~开启USART时钟
  SYSCFG_REMAPPinConfig(REMAP_Pin_USART1TxRxPortA,DISABLE);
  
  //GPIO_Init(GPIOA, GPIO_Pin_3, GPIO_Mode_In_PU_IT);//初始化按键，上拉输入，带中断             //串口高电平
  GPIO_Init(GPIOA, GPIO_Pin_3, GPIO_Mode_In_FL_IT);//初始化按键，上拉输入，带中断           //串口低电平
  
  GPIOA->ODR &= ~GPIO_Pin_2;
}

void UartSendStr(unsigned char * buf,unsigned char len)
{
  for(unsigned char i = 0 ; i < len ; i ++)
  {
    USART_SendData8 (USART1,buf[i]);
    while(!USART_GetFlagStatus (USART1,USART_FLAG_TXE));//等待发送完毕
  }
}

/////////////////////////////////////////////////////
#include "sysdata.h"

/*unsigned cahr UartTable[][] = 
{
  {0x40 ,0x41 ,0x42 ,0x38 ,0x33 ,0x2A ,0x0D},
  {0x40 ,0x41 ,0x46 ,0x38 ,0x37 ,0x2A ,0x0D},
  {0x40 ,0x41 ,0x48 ,0x38 ,0x39 ,0x2A ,0x0D},
  {0x40 ,0x41 ,0x49 ,0x38 ,0x41 ,0x2A ,0x0D},
  {0x40 ,0x41 ,0x44 ,0x38 ,0x35 ,0x2A ,0x0D},
  {0x40 ,0x41 ,0x4A ,0x38 ,0x42 ,0x2A ,0x0D},
};*/

uint16_t UartGetCheck(unsigned char * buf , unsigned char len)
{
  uint16_t Chack = 0;
  unsigned char Sum = 0;
  unsigned char str = 0;
  unsigned char i;
  for(i = 0 ; i < len ; i ++) Sum += buf[i];
  
  str = Sum >> 4;
  if(str < 10) str += '0';
  else str += 'A' - 10;
  Chack = str << 8;
  
  str = Sum & 0xf;
  if(str < 10) str += '0';
  else str += 'A' - 10;
  Chack |= str;
  
  return Chack;
}

uint16_t UartWriteCheck(unsigned char * buf , unsigned char len)
{
  uint16_t Chack = UartGetCheck(buf + 1,len - 1);
  buf[len] = Chack >> 8;
  buf[len + 1] = Chack & 0xff;
  buf[len + 2] = 0x2a;
  buf[len + 3] = 0x0d;
  
  return Chack;
}

unsigned char UartCheckOk(unsigned char * buf , unsigned char len)
{
  uint16_t Chack = UartGetCheck(buf + 1,len - 1 - 4);
  
  if((Chack >> 8) == buf[len - 4])
  {
    if((Chack & 0xff) == buf[len - 3])
    {
      return 1;
    }
  }
  
  return 0;
}

unsigned char UartGetBCD(unsigned char * buf,uint32_t data,unsigned char len)
{
  unsigned char temp;
  for(signed char i = len - 1 ; i >= 0 ; i--)
  {
    temp = data & 0xf;
    if(temp < 10) buf[i] = temp + '0';
    else buf[i] = temp - 10 + 'A';
    data >>= 4;
  }
  return 1;
}

uint32_t UartGetHEX(unsigned char * buf,uint32_t * data,unsigned char len)
{
  unsigned char temp;
  uint32_t hex = 0;
  for(unsigned char i = 0 ; i < len ; i ++)
  {
    hex <<= 4;
    temp = buf[i];
    if(temp < 'A') hex += temp - '0';
    else hex += temp - 'A' + 10;
  }
  *data = hex;
  return hex;
}

unsigned char UartWork(void)
{
  SUART.time ++;
  if(SUART.start == 0)
  {
    SUART.start = 1;
    UartInit();
  }
  
  if(SUART.buf_len > 2)
  {
    if( SUART.buf[2] >= 'A' && SUART.buf[2] <= 'J' )
    {
      if(SUART.buf_len == 7 && SUART.buf[5] == 0x2a && SUART.buf[6] == 0x0d)
      {
        SUART.buf_len = 0;
        SUART.time = 0;
        if(UartCheckOk(SUART.buf,7))
        {
          SUART.send_i = 0;
          SUART.send_buf[SUART.send_i ++] = 0x40;
          SUART.send_buf[SUART.send_i ++] = SUART.buf[1];
          SUART.send_buf[SUART.send_i ++] = SUART.buf[2];
          SUART.send_buf[SUART.send_i ++] = 0x30;
          switch(SUART.buf[2])
          {
            case 'B':
              UartGetBCD(&SUART.send_buf[SUART.send_i],sysdata.MSID,5);
              SUART.send_i += 5;
              break;
            case 'F':
              UartGetBCD(&SUART.send_buf[SUART.send_i],sysdata.MSV,5);
              SUART.send_i += 5;
              break;
            case 'H':
              UartGetBCD(&SUART.send_buf[SUART.send_i],ReadRTCTime() * 2,5);
              SUART.send_i += 5;
              break;
            case 'I':
              
              //SysdataReadBattery();
              
              UartGetBCD(&SUART.send_buf[SUART.send_i],(sysdata.MSV << 2) + (sysdata.Tamper << 1) + (sysdata.Battery),1);
              SUART.send_i += 1;
              
              SysdataUpdateTamper();
              
              break;
            case 'D':
              UartGetBCD(&SUART.send_buf[SUART.send_i],sysdata.SendMode,1);
              SUART.send_i += 1;
              break;
            case 'J':
              UartGetBCD(&SUART.send_buf[SUART.send_i],0,2);
              SUART.send_i += 2;
              break;
            default:
              SUART.send_buf[SUART.send_i - 1] = 0x31;
              break;
          }
          UartWriteCheck(SUART.send_buf,SUART.send_i);
          SUART.send_i += 4;
          UartSendStr(SUART.send_buf,SUART.send_i);
        }
      }
      else if(SUART.buf_len == 12 && SUART.buf[10] == 0x2a && SUART.buf[11] == 0x0d)
      {
        SUART.buf_len = 0;
        SUART.time = 0;
        if(UartCheckOk(SUART.buf,12))
        {
          SUART.send_i = 0;
          SUART.send_buf[SUART.send_i ++] = 0x40;
          SUART.send_buf[SUART.send_i ++] = SUART.buf[1];
          SUART.send_buf[SUART.send_i ++] = SUART.buf[2];
          SUART.send_buf[SUART.send_i ++] = 0x30;
          switch(SUART.buf[2])
          {
            case 'A'://msid
              UartGetHEX(&(SUART.buf[3]),&(sysdata.MSID),5);
              if(sysdata.SendMode == 0) WriteRTCAlarm(sysdata.MSID % 1024 * 84);
              
              SysdataSave();
              break;
            case 'E'://msv
              UartGetHEX(&(SUART.buf[3]),&(sysdata.MSV),5);
              
              SysdataSave();
              break;
            case 'G'://mst
              UartGetHEX(&(SUART.buf[3]),&(sysdata.MST),5);
              WriteRTCTime(sysdata.MST / 2);
              
              SysdataSave();
              break;
            default:
              SUART.send_buf[SUART.send_i - 1] = 0x31;
              break;
          }
          UartWriteCheck(SUART.send_buf,SUART.send_i);
          SUART.send_i += 4;
          UartSendStr(SUART.send_buf,SUART.send_i);
        }
      }
      /*else if(SUART.buf_len == 8 && SUART.buf[6] == 0x2a && SUART.buf[7] == 0x0d)
      {
        SUART.buf_len = 0;
        SUART.time = 0;
        if(UartCheckOk(SUART.buf,8))
        {
          SUART.send_i = 0;
          SUART.send_buf[SUART.send_i ++] = 0x40;
          SUART.send_buf[SUART.send_i ++] = SUART.buf[1];
          SUART.send_buf[SUART.send_i ++] = SUART.buf[2];
          SUART.send_buf[SUART.send_i ++] = 0x30;
          switch(SUART.buf[2])
          {
            case 'C'://mswm
              if(SUART.buf[3] == '0')
              {
                sysdata.SendMode = 0;
                WriteRTCAlarm(sysdata.MSID % 1024 * 84);
              }
              else if(SUART.buf[3] == '1')
              {
                sysdata.SendMode = 1;
                WriteRTCWakeUp(2040);
              }
              
              SysdataSave();
              break;
            default:
              SUART.send_buf[SUART.send_i - 1] = 0x31;
              break;
          }
          UartWriteCheck(SUART.send_buf,SUART.send_i);
          SUART.send_i += 4;
          UartSendStr(SUART.send_buf,SUART.send_i);
        }
      }*/
    }
    else SUART.buf_len = 0;
  }
  
  if(SUART.time == 200)
  {
    SUART.time = 0;
    SUART.start = 0;
    
    UartDeInit();
    
    return 1;
  }
  
  return 0;
}




