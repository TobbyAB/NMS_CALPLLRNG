
/*******************************************************************************/
/*Software Version��SUS0010�C010010*/
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
  Delay(6000);          //��ʱ���ȴ�ϵͳ��Դ�ȶ�
  
  CLK_init();           //�ڲ�����ʱ�ӳ�ʼ��
  
  SysdataReadBattery();         //PVD����һ�Σ���ʼ��PVD���򿪵����������ܣ���ȡ����״̬
  
  SysdataInit();                //ϵͳ���ݳ�ʼ������eeprom��ȡ��������Ĭ��ֵ
  
  RTC_Config();         //�ⲿ32.768Kʼ�ճ�ʼ�� + RTCʵʱʱ������
  
  WriteRTCAlarm(sysdata.MSID % 1024 * 84);              //����RTC����ʱ�䣬���Ƿ���ʱ�䣬1024��Ϊһ�飬ÿ�����÷��ͼ��ʱ��Ϊ84�룬һ��Լ1��
  
  LowPowerInit();               //�͹������� + IO������
  
  
  
 // GPIO_Init(GPIOB, GPIO_Pin_0, GPIO_Mode_In_PU_No_IT);
  
  
/*******************5043��ʼ��������͹���**************************/
  GPIO_Init(GPIOC, GPIO_Pin_4, GPIO_Mode_Out_PP_High_Fast);             
  SPI_init();           //SPI��ʼ��
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
/*******************5043��ʼ������****************************/
    
/*****************�ص�����⹦��******************/
  PWR_PVDCmd(DISABLE);                                     //PVDʹ��
  PWR_PVDITConfig(DISABLE);                                //PVD�ж�ʹ��
  PWR_PVDClearITPendingBit();
/*************************************************/
  
  WriteRTCWakeUp(2048);         //RTC��ʱ���ѣ�2048 = 1�뻽��һ��
  
  UartInit(); //��ӡPLLRNG���ڳ�ʼ��
  
  IWDGInit();           //���Ź���ʼ��

  while (1)
  {
    IWDG_ReloadCounter();               //ι��
    
    if(sysflag.uart == 0 && sysflag.send == 0 && sysflag.send2min == 0)         //���ڣ�����һ�Σ��������ͣ�������Ҫִ��
    {
      //LowPowerInit();
      FastLowPowerInit();               //���ٵ͹��ĺ���
      do
      {
        sysflag.FastSleep = 0;          //����˯�߱�־����
        
        //halt();         //����͹���˯��
        
        IWDG->KR = IWDG_KEY_REFRESH;    //���Ѻ�ι��
        
        
        sysdata.SaveNum ++;             //��ʱ40000�룬��������һ�Ρ�
        if(sysdata.SaveNum >= 40000)
        {
          sysdata.SaveNum = 0;
          
          SysdataSave();                //��������
        }
      }
      while(sysflag.FastSleep);         //����˯�߱�־���������Ҫ���������黽�Ѻ�ֱ��˯�ߣ�flag = 1��
      
      CLK_init();               //�����ڲ�����ʱ��
    }
    else if(sysflag.uart)               //��Ҫִ�д��ڷ���
    {
      if(UartWork()) sysflag.uart = 0;          //ִ�д��ڷ��񣬷���1��ʾ��ִ����ɣ����־λ
      Delay(6000);
    }
    else if(sysflag.send)               //��Ҫִ�з���һ�η���
    {
      SysdataReadBattery();             //����������⹦��
  
 //     SysdataSave();
      //disableInterrupts();
      
      IWDG_ReloadCounter();             //ι��
      
      UpDate();         //������ݵ����ͻ���

      SPI_init();       //SPI��ʼ��
      
      GPIO_Init(GPIOC, GPIO_Pin_4, GPIO_Mode_Out_PP_High_Fast);         //������
      Delay(6000);
  //      IWDG_ReloadCounter();
  //    AX5043_Reset();
  //      IWDG_ReloadCounter();
      AX5043_set_registers();           //���÷���оƬ
  //      IWDG_ReloadCounter();
      simple_autorange_pll();           //����оƬ�Զ���Ƶ
  //      IWDG_ReloadCounter();
      simple_transmit(send_buf,26);             //װ������ݣ�������
  //      IWDG_ReloadCounter();
      SpiWriteSingleAddressRegister(0x01, 0x01);
      SpiWriteSingleAddressRegister(0x02, 0x00);
      
      //GPIO_Init(GPIOC, GPIO_Pin_4, GPIO_Mode_Out_PP_Low_Fast);
      GPIOC->ODR &= ~GPIO_Pin_4;
      
      Delay(300);
      SpiWriteSingleAddressRegister(0x02, 0x01);                //����оƬ����͹���
      
  //      IWDG_ReloadCounter();
    
      sysflag.send = 0;         //�巢�ͱ�־
      
  /*****************�ص�����⹦��******************/
  PWR_PVDCmd(DISABLE);                                     //PVDʹ��
  PWR_PVDITConfig(DISABLE);                                //PVD�ж�ʹ��
  PWR_PVDClearITPendingBit();
  /*************************************************/

  if(sysdata.PVDBatteryFlag) sysdata.Battery = 1;         //���û�磬����λû���־λ
  else sysdata.Battery = 0;             //�������0����λû���־λ
      
      //enableInterrupts();
      SysdataUpdateTamper();            //��ȡ����λ
      
      SysdataSave();            //ÿ����һ�α���һ������
      
    }
    else if(sysflag.send2min)           //�����Ҫִ���������ͷ���
    {
      sysflag.send2min = 0;             //���־λ
      
      main_send2min();          //ִ���������ͷ�����
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
        CLK_LSEConfig(CLK_LSE_ON);//ʹ��LSE
        CLK_RTCClockConfig(CLK_RTCCLKSource_LSE, CLK_RTCCLKDiv_1);//RTCʱ��ԴLSE��1��Ƶ=32.768K
        while (CLK_GetFlagStatus(CLK_FLAG_LSERDY) == RESET);//�ȴ�LSE����
  
        CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE);//RTCʱ���ſ�ʹ��

        
        RTC_WaitForSynchro();
        
        
        RTC_ClearITPendingBit(RTC_IT_ALRA);
        RTC_ITConfig(RTC_IT_ALRA,ENABLE);//�����ж�
        
        
        //ITC_SetSoftwarePriority(RTC_CSSLSE_IRQn, ITC_PriorityLevel_3);//���ȼ�
        
        //enableInterrupts();
}

void CLK_init(void)
{
  CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSI);
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_8);         // 16M�ڲ�RC��8��Ƶ��ϵͳʱ��Ϊ2M 
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
  
    PWR_FastWakeUpCmd(ENABLE);  //���ٻ���ʹ��
    //CLK_HaltConfig(CLK_Halt_FastWakeup, ENABLE); 
    
    //����IO����͵�ƽ
    GPIO_Init(GPIOA, GPIO_Pin_All & ~(GPIO_Pin_3), GPIO_Mode_Out_PP_Low_Slow);
    GPIO_Init(GPIOB, GPIO_Pin_All & ~(GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2), GPIO_Mode_Out_PP_Low_Slow);
    GPIO_Init(GPIOC, GPIO_Pin_All & ~GPIO_Pin_4, GPIO_Mode_Out_PP_Low_Slow);
    GPIO_Init(GPIOD, GPIO_Pin_All, GPIO_Mode_Out_PP_Low_Slow);
    
    GPIO_Init(GPIOC, GPIO_Pin_4, GPIO_Mode_Out_PP_High_Slow);//Power
    //GPIO_Init(GPIOA, GPIO_Pin_2, GPIO_Mode_Out_PP_High_Slow);//UART-TX
  
    //GPIO_Init(GPIOA, GPIO_Pin_3, GPIO_Mode_In_PU_IT);//��ʼ���������������룬���ж�         //���ڸߵ�ƽ
    GPIO_Init(GPIOA, GPIO_Pin_3, GPIO_Mode_In_FL_IT);//��ʼ���������������룬���ж�           //���ڵ͵�ƽ
    
    GPIO_Init(GPIOB, GPIO_Pin_0, GPIO_Mode_In_PU_IT);//��ʼ���������������룬���ж�
    //GPIO_Init(GPIOB, GPIO_Pin_3, GPIO_Mode_In_PU_IT);//��ʼ���������������룬���ж�
    GPIO_Init(GPIOB, GPIO_Pin_1, GPIO_Mode_In_FL_IT);//��ʼ���������������룬���ж�
    GPIO_Init(GPIOB, GPIO_Pin_2, GPIO_Mode_In_FL_IT);//��ʼ���������������룬���ж�
    EXTI_DeInit (); //�ָ��жϵ��������� 
    EXTI_SetPinSensitivity (EXTI_Pin_0,EXTI_Trigger_Rising_Falling);//�ⲿ�ж�1���½��ش�����������9
    EXTI_SetPinSensitivity (EXTI_Pin_1,EXTI_Trigger_Falling);//�ⲿ�ж�1���½��ش�����������9
    EXTI_SetPinSensitivity (EXTI_Pin_2,EXTI_Trigger_Falling);//�ⲿ�ж�1���½��ش�����������9

    //EXTI_SetPinSensitivity (EXTI_Pin_3,EXTI_Trigger_Falling);//�ⲿ�ж�1���½��ش�����������9         //���ڸߵ�ƽ
    EXTI_SetPinSensitivity (EXTI_Pin_3,EXTI_Trigger_Rising);//�ⲿ�ж�1���½��ش�����������9        //���ڵ͵�ƽ
  
    PWR_UltraLowPowerCmd(ENABLE);//���͹���
    
    //RTC_ITConfig(RTC_IT_WUT, ENABLE);//���Ѷ�ʱ���ж�ʹ��
    //RTC_WakeUpCmd(ENABLE);//RTC����ʹ��
    
  //EXTI_ClearITPendingBit (EXTI_IT_Pin0);//����жϱ�־
  EXTI_ClearITPendingBit (EXTI_IT_Pin1);//����жϱ�־
  EXTI_ClearITPendingBit (EXTI_IT_Pin2);//����жϱ�־
  //EXTI_ClearITPendingBit (EXTI_IT_Pin3);//����жϱ�־
  
    
    //RTC_AlarmCmd(ENABLE);
    enableInterrupts();
}


void FastLowPowerInit(void)
{
    PWR_FastWakeUpCmd(ENABLE);  //���ٻ���ʹ��
    //GPIO_Init(GPIOB, GPIO_Pin_3 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7, GPIO_Mode_Out_PP_High_Slow);
    
    GPIOB->ODR |= GPIO_Pin_3 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    
    //GPIO_Init(GPIOB, GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7, GPIO_Mode_In_FL_No_IT);
    //GPIOB->ODR |= GPIO_Pin_3;
    
    PWR_UltraLowPowerCmd(ENABLE);//���͹���
    //EXTI_SetPinSensitivity (EXTI_Pin_3,EXTI_Trigger_Falling);//�ⲿ�ж�1���½��ش�����������9
}





