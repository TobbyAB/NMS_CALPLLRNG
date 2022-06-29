
/*******************************************************************************/
/*Software Version：SUS0010–010010*/
#include "stm8l15x.h"
#include "stdio.h"
#include "string.h"

#include "spi.h"

#include "sysdata.h"
#include "uart.h"

#include "ax5043.h"


void PVDInit(void);
void PVDDeInit(void);
void RTC_Config(void);
void CLK_init(void);
void FastLowPowerInit(void);
void LowPowerInit(void);
void IWDGInit(void);


void UpDate(void);

/*uint8_t send_buf[32]=
{
  0xa1,0x3E,0x55,0x55,0x55,0x55,0x60,
  0x61,0x38,0x55,0x5A,
  0xe1,0x13,0x39,
  0x6a,0xa9,0xa9,0x95,0x66,0x69,
  0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,
  0xa9,0xa9,0xa6,0x56,0xa6,0x56,
};*/
uint8_t send_buf[32]=
{

  0xe1,0x18,0x03,0X55,0X55,0X55,0X55,0X5A,
  //0x6a,0xa9,0xa9,0x95,0x66,0x69,
  //0xa6,0xa9,0xaa,0xaa,0xaa,0xaa,
  //0xa9,0xa9,0xaa,0x55,0xaa,0x55,
};
/*uint8_t send_buf[]=
{
  0X55,0X55,0X55,0X55,0X55,0X5A,
  0x99,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,
  0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,
};*/
uint8_t send_01[256]=
{
  0xe1,201,0x03,
};

void main_send2min(void)
{
  GPIO_Init(GPIOC, GPIO_Pin_4, GPIO_Mode_Out_PP_High_Fast);
  SPI_init();
  Delay(6000);
  
  AX5043_Reset();
  AX5043_set_registers();
  simple_autorange_pll();
    
  send2min();
  
  SpiWriteSingleAddressRegister(0x01, 0x01);
  SpiWriteSingleAddressRegister(0x02, 0x00);
  
  GPIOC->ODR &= ~GPIO_Pin_4;
  
  Delay(300);
  SpiWriteSingleAddressRegister(0x02, 0x01);
}

int main(void)
{
  Delay(6000);          //延时，等待系统电源稳定
  
  CLK_init();           //内部高速时钟初始化
  
  SysdataReadBattery();         //PVD工作一次，初始化PVD，打开电量测量功能，读取点量状态
  
  SysdataInit();                //系统数据初始化，从eeprom读取，或设置默认值
  
  RTC_Config();         //外部32.768K始终初始化 + RTC实时时钟配置
  
  WriteRTCAlarm(sysdata.MSID % 1024 * 84);              //设置RTC闹钟时间，就是发送时间，1024个为一组，每个设置发送间隔时间为84秒，一共约1天
  
  LowPowerInit();               //低功耗设置 + IO口配置
  
  
  
 // GPIO_Init(GPIOB, GPIO_Pin_0, GPIO_Mode_In_PU_No_IT);
  
  
/*******************5043初始化，进入低功耗**************************/
  GPIO_Init(GPIOC, GPIO_Pin_4, GPIO_Mode_Out_PP_High_Fast);             
  SPI_init();           //SPI初始化
  Delay(6000);
  
  SpiWriteSingleAddressRegister(0x01, 0x01);
  SpiWriteSingleAddressRegister(0x02, 0x00);
  
  GPIOC->ODR &= ~GPIO_Pin_4;
  
  Delay(300);
  SpiWriteSingleAddressRegister(0x02, 0x01);
    
  Delay(6000);
  Delay(6000);
  Delay(6000);

  
//  GPIO_Init(GPIOB, GPIO_Pin_0, GPIO_Mode_In_PU_IT);
  sysflag.send = 0;
  //GPIO_Init(GPIOC, GPIO_Pin_4, GPIO_Mode_Out_PP_Low_Fast);
    
  //LowPowerInit();
/*******************5043初始化结束****************************/
    
/*****************关电量检测功能******************/
  PWR_PVDCmd(DISABLE);                                     //PVD使能
  PWR_PVDITConfig(DISABLE);                                //PVD中断使能
  PWR_PVDClearITPendingBit();
/*************************************************/
  
  WriteRTCWakeUp(2048);         //RTC定时唤醒，2048 = 1秒唤醒一次
  
  UartInit(); //打印PLLRNG串口初始化
  
  IWDGInit();           //看门狗初始化

  while (1)
  {
    IWDG_ReloadCounter();               //喂狗
    
    if(sysflag.uart == 0 && sysflag.send == 0 && sysflag.send2min == 0)         //串口，发送一次，连续发送，都不需要执行
    {
      //LowPowerInit();
      FastLowPowerInit();               //快速低功耗函数
      do
      {
        sysflag.FastSleep = 0;          //快速睡眠标志清零
        
        //halt();         //进入低功耗睡眠
        
        IWDG->KR = IWDG_KEY_REFRESH;    //唤醒后喂狗
        
        
        sysdata.SaveNum ++;             //计时40000秒，保存数据一次。
        if(sysdata.SaveNum >= 40000)
        {
          sysdata.SaveNum = 0;
          
          SysdataSave();                //保存数据
        }
      }
      while(sysflag.FastSleep);         //快速睡眠标志，如果不需要处理别的事情唤醒后直接睡眠，flag = 1；
      
      CLK_init();               //开启内部高速时钟
    }
    else if(sysflag.uart)               //需要执行串口服务
    {
      if(UartWork()) sysflag.uart = 0;          //执行串口服务，返回1表示，执行完成，清标志位
      Delay(6000);
    }
    else if(sysflag.send)               //需要执行发送一次服务
    {
      SysdataReadBattery();             //开启电量检测功能
  
 //     SysdataSave();
      //disableInterrupts();
      
      IWDG_ReloadCounter();             //喂狗
      
      UpDate();         //打包数据到发送缓存

      SPI_init();       //SPI初始化
      
      GPIO_Init(GPIOC, GPIO_Pin_4, GPIO_Mode_Out_PP_High_Fast);         //开晶振
      Delay(6000);
  //      IWDG_ReloadCounter();
  //    AX5043_Reset();
  //      IWDG_ReloadCounter();
      AX5043_set_registers();           //配置发射芯片
  //      IWDG_ReloadCounter();
      simple_autorange_pll();           //发送芯片自动锁频
  //      IWDG_ReloadCounter();
      simple_transmit(send_buf,26);             //装填发送数据，并发送
  //      IWDG_ReloadCounter();
      SpiWriteSingleAddressRegister(0x01, 0x01);
      SpiWriteSingleAddressRegister(0x02, 0x00);
      
      //GPIO_Init(GPIOC, GPIO_Pin_4, GPIO_Mode_Out_PP_Low_Fast);
      GPIOC->ODR &= ~GPIO_Pin_4;
      
      Delay(300);
      SpiWriteSingleAddressRegister(0x02, 0x01);                //发送芯片进入低功耗
      
  //      IWDG_ReloadCounter();
    
      sysflag.send = 0;         //清发送标志
      
  /*****************关电量检测功能******************/
  PWR_PVDCmd(DISABLE);                                     //PVD使能
  PWR_PVDITConfig(DISABLE);                                //PVD中断使能
  PWR_PVDClearITPendingBit();
  /*************************************************/

  if(sysdata.PVDBatteryFlag) sysdata.Battery = 1;         //如果没电，就置位没电标志位
  else sysdata.Battery = 0;             //否则等于0，复位没电标志位
      
      //enableInterrupts();
      SysdataUpdateTamper();            //读取干扰位
      
      SysdataSave();            //每发送一次保存一次数据
      
    }
    else if(sysflag.send2min)           //如果需要执行连续发送服务
    {
      sysflag.send2min = 0;             //清标志位
      
      main_send2min();          //执行连续发送服务函数
    }
  }
}

#define DATA_START 8

#define THSTART         0

#define THVERSIONS_LEN  2

#define THTAMPER_LEN    1

#define THBATTERY_LEN   1

#define THMSID_LEN      20

#define THMSV_LEN       20


unsigned char data_buf[9] = 
{
  0,
};

void UpDataToSendBuf(void)
{
  unsigned char addr = DATA_START;
  unsigned char addr_i = 0;
  for(unsigned char n = 0 ; n < 9 ; n ++)
  {
    unsigned char data = data_buf[n];
    for(unsigned char i = 0 ; i < 8 ; i ++)
    {
      send_buf[addr] <<= 2;
      if(data & 0x80) send_buf[addr] |= 1;
      else send_buf[addr] |= 2;
      
      data <<= 1;
      
      addr_i ++;
      if(addr_i == 4)
      {
        addr_i = 0;
        addr ++;
      }
    }
  }
}

void DataCheck(void)
{
  for(unsigned char n = 0 ; n < 7 ; n ++)
  {
    unsigned char buf_data = data_buf[n];
    unsigned char data = 0;
    for(unsigned char i = 0 ; i < 7 ; i ++)
    {
      if(buf_data & 0x80) data ++;
      buf_data <<= 1;
    }
    if((data % 2) != 0) data_buf[n] |= 0x01;
  }
  
  data_buf[7] = 0;
  unsigned char wei_data = 0x01;
  for(unsigned char n = 0 ; n < 8 ; n ++)
  {
    unsigned char data = 0;
    for(unsigned char i = 0 ; i < 7 ; i ++)
    {
      if(data_buf[i] & wei_data) data ++;
    }
    if((data % 2) != 0) data_buf[7] |= wei_data;
    
    wei_data <<= 1;
  }
  
  data_buf[8] = data_buf[7];
  
  UpDataToSendBuf();
}

void UpDate(void)
{
  signed char i;
  uint32_t data;
  unsigned char len;
  unsigned char addr = 0;
  unsigned char addr_i = 0;
  /*
  sysdata.Versions = 2;
  sysdata.Tamper = 0;
  sysdata.Battery = 1;
  sysdata.MSID = 456789;
  sysdata.MSV = 999999;
  */
  for(unsigned char n = 0 ; n < 6 ; n ++)
  {
    if(n == 0)
    {
      data = sysdata.Versions;
      len = THVERSIONS_LEN;
    }
    else if(n == 1)
    {
      data = sysdata.Tamper;
      len = THTAMPER_LEN;
    }
    else if(n == 2)
    {
      data = sysdata.Battery;
      len = THBATTERY_LEN;
    }
    else if(n == 3)
    {
      data = sysdata.MSID;
      len = THMSID_LEN;
    }
    else if(n == 4)
    {
      data = sysdata.MSV;
      len = THMSV_LEN;
    }
    else
    {
      data = 8;
      len = 5;
    }
    data <<= (32 - len);
    for(i = 0 ; i < len ; i ++)
    {
      data_buf[addr] <<= 1;
      if(data & 0x80000000)
      {
        data_buf[addr] |= 1;
      }
      data <<= 1;
      
      addr_i ++;
      if(addr_i == 7)
      {
        data_buf[addr] <<= 1;
        addr_i = 0;
        addr ++;
      }
    }
  }
  DataCheck();
}

void RTC_Config(void)
{
        CLK_LSEConfig(CLK_LSE_ON);//使能LSE
        CLK_RTCClockConfig(CLK_RTCCLKSource_LSE, CLK_RTCCLKDiv_1);//RTC时钟源LSE，1分频=32.768K
        while (CLK_GetFlagStatus(CLK_FLAG_LSERDY) == RESET);//等待LSE就绪
  
        CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE);//RTC时钟门控使能

        
        RTC_WaitForSynchro();
        
        
        RTC_ClearITPendingBit(RTC_IT_ALRA);
        RTC_ITConfig(RTC_IT_ALRA,ENABLE);//开启中断
        
        
        //ITC_SetSoftwarePriority(RTC_CSSLSE_IRQn, ITC_PriorityLevel_3);//优先级
        
        //enableInterrupts();
}

void CLK_init(void)
{
  CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSI);
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_8);         // 16M内部RC经8分频后系统时钟为2M 
}

void IWDGInit(void)
{
  /*if (RST_GetFlagStatus(RST_FLAG_IWDGF) != RESET)
  {
    RST_ClearFlag(RST_FLAG_IWDGF);
  }*/
  
  IWDG_Enable();
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
  IWDG_SetPrescaler(IWDG_Prescaler_256);
  IWDG_SetReload(255);
  IWDG_ReloadCounter();
  
}

void LowPowerInit(void)
{
  //disableInterrupts();
    //PWR_INC();
  
    PWR_FastWakeUpCmd(ENABLE);  //快速唤醒使能
    //CLK_HaltConfig(CLK_Halt_FastWakeup, ENABLE); 
    
    //所有IO输出低电平
    GPIO_Init(GPIOA, GPIO_Pin_All & ~(GPIO_Pin_3), GPIO_Mode_Out_PP_Low_Slow);
    GPIO_Init(GPIOB, GPIO_Pin_All & ~(GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2), GPIO_Mode_Out_PP_Low_Slow);
    GPIO_Init(GPIOC, GPIO_Pin_All & ~GPIO_Pin_4, GPIO_Mode_Out_PP_Low_Slow);
    GPIO_Init(GPIOD, GPIO_Pin_All, GPIO_Mode_Out_PP_Low_Slow);
    
    GPIO_Init(GPIOC, GPIO_Pin_4, GPIO_Mode_Out_PP_High_Slow);//Power
    //GPIO_Init(GPIOA, GPIO_Pin_2, GPIO_Mode_Out_PP_High_Slow);//UART-TX
  
    //GPIO_Init(GPIOA, GPIO_Pin_3, GPIO_Mode_In_PU_IT);//初始化按键，上拉输入，带中断         //串口高电平
    GPIO_Init(GPIOA, GPIO_Pin_3, GPIO_Mode_In_FL_IT);//初始化按键，上拉输入，带中断           //串口低电平
    
    GPIO_Init(GPIOB, GPIO_Pin_0, GPIO_Mode_In_PU_IT);//初始化按键，上拉输入，带中断
    //GPIO_Init(GPIOB, GPIO_Pin_3, GPIO_Mode_In_PU_IT);//初始化按键，上拉输入，带中断
    GPIO_Init(GPIOB, GPIO_Pin_1, GPIO_Mode_In_FL_IT);//初始化按键，上拉输入，带中断
    GPIO_Init(GPIOB, GPIO_Pin_2, GPIO_Mode_In_FL_IT);//初始化按键，上拉输入，带中断
    EXTI_DeInit (); //恢复中断的所有设置 
    EXTI_SetPinSensitivity (EXTI_Pin_0,EXTI_Trigger_Rising_Falling);//外部中断1，下降沿触发，向量号9
    EXTI_SetPinSensitivity (EXTI_Pin_1,EXTI_Trigger_Falling);//外部中断1，下降沿触发，向量号9
    EXTI_SetPinSensitivity (EXTI_Pin_2,EXTI_Trigger_Falling);//外部中断1，下降沿触发，向量号9

    //EXTI_SetPinSensitivity (EXTI_Pin_3,EXTI_Trigger_Falling);//外部中断1，下降沿触发，向量号9         //串口高电平
    EXTI_SetPinSensitivity (EXTI_Pin_3,EXTI_Trigger_Rising);//外部中断1，下降沿触发，向量号9        //串口低电平
  
    PWR_UltraLowPowerCmd(ENABLE);//超低功耗
    
    //RTC_ITConfig(RTC_IT_WUT, ENABLE);//唤醒定时器中断使能
    //RTC_WakeUpCmd(ENABLE);//RTC唤醒使能
    
  //EXTI_ClearITPendingBit (EXTI_IT_Pin0);//清除中断标志
  EXTI_ClearITPendingBit (EXTI_IT_Pin1);//清除中断标志
  EXTI_ClearITPendingBit (EXTI_IT_Pin2);//清除中断标志
  //EXTI_ClearITPendingBit (EXTI_IT_Pin3);//清除中断标志
  
    
    //RTC_AlarmCmd(ENABLE);
    enableInterrupts();
}


void FastLowPowerInit(void)
{
    PWR_FastWakeUpCmd(ENABLE);  //快速唤醒使能
    //GPIO_Init(GPIOB, GPIO_Pin_3 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7, GPIO_Mode_Out_PP_High_Slow);
    
    GPIOB->ODR |= GPIO_Pin_3 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    
    //GPIO_Init(GPIOB, GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7, GPIO_Mode_In_FL_No_IT);
    //GPIOB->ODR |= GPIO_Pin_3;
    
    PWR_UltraLowPowerCmd(ENABLE);//超低功耗
    //EXTI_SetPinSensitivity (EXTI_Pin_3,EXTI_Trigger_Falling);//外部中断1，下降沿触发，向量号9
}





