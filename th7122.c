
#include "stm8l15x.h"

#include "sysdata.h"


////////////////Interface//////////////

#define THSCK_PORT  GPIOC
#define THSCK_PIN  GPIO_Pin_4

#define THSDA_PORT  GPIOD
#define THSDA_PIN  GPIO_Pin_0

#define THSEN_PORT  GPIOB
#define THSEN_PIN  GPIO_Pin_0

#define THLOCK_PORT  GPIOB
#define THLOCK_PIN  GPIO_Pin_7

#define THPOW_PORT  GPIOB
#define THPOW_PIN  GPIO_Pin_6

#define THDATA_PORT  GPIOB
#define THDATA_PIN  GPIO_Pin_1

void TH7122DelayUs(__IO uint16_t nCount)
{
    /* Decrement nCount value */
  //uint16_t i = nCount;
  //i = 333;
  //nCount = i;
    while (nCount--);
}

//////////////Driver///////////////
void TH7122PWMInit(void)
{
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM3, ENABLE);
  TIM3_DeInit();
  TIM3_TimeBaseInit(TIM3_Prescaler_16,TIM3_CounterMode_Up, 600);

  TIM3_OC1Init(TIM3_OCMode_PWM1,TIM3_OutputState_Enable,300, TIM3_OCPolarity_Low,TIM3_OCIdleState_Set);
  //TIM3_OC1PreloadConfig(ENABLE);
  
  //TIM3_ARRPreloadConfig(ENABLE);

  TIM3_Cmd(ENABLE);
  
  //TIM3_CtrlPWMOutputs(ENABLE);
  TIM3_ClearFlag(TIM3_FLAG_Update);
  /* Enable update interrupt */
  TIM3_ITConfig(TIM3_IT_Update, ENABLE);
  
  enableInterrupts();

}

void TH7122ITOn(void)
{
  //TIM3_ITConfig(TIM3_IT_Update, ENABLE);
  TIM3_Cmd(ENABLE);
}

void TH7122ITOff(void)
{
  //TIM3_ITConfig(TIM3_IT_Update, DISABLE);
  TIM3_Cmd(DISABLE);
}

void TH7122PWMOn(void)
{
  TIM3_CtrlPWMOutputs(ENABLE);
}

void TH7122PWMOff(void)
{
  TIM3_CtrlPWMOutputs(DISABLE);
}

void TH7122PWMSend(void)
{
  TIM3_GenerateEvent(TIM3_EventSource_Update);
}

void TH7122Init(void)
{
  /*GPIO_SetBits(THPOW_PORT,THPOW_PIN);
  GPIO_SetBits(THSEN_PORT,THSEN_PIN);
  GPIO_ResetBits(THSCK_PORT,THSCK_PIN);
  GPIO_ResetBits(THSDA_PORT,THSDA_PIN);
  GPIO_ResetBits(THDATA_PORT,THDATA_PIN);*/
  
  GPIO_Init(THSCK_PORT, THSCK_PIN, GPIO_Mode_Out_PP_Low_Fast);
  GPIO_Init(THSDA_PORT, THSDA_PIN, GPIO_Mode_Out_PP_Low_Fast);
  GPIO_Init(THSEN_PORT, THSEN_PIN, GPIO_Mode_Out_PP_High_Fast);
  GPIO_Init(THPOW_PORT, THPOW_PIN, GPIO_Mode_Out_PP_High_Fast);
  GPIO_Init(THDATA_PORT, THDATA_PIN, GPIO_Mode_Out_PP_Low_Fast);
  
  GPIO_Init(THLOCK_PORT, THLOCK_PIN, GPIO_Mode_In_PU_No_IT);
  
  //TH7122PWMInit();
}

void TH7122DeInit(void)
{
  /*
  GPIO_SetBits(THPOW_PORT,THPOW_PIN);
  GPIO_ResetBits(THSEN_PORT,THSEN_PIN);
  GPIO_ResetBits(THSCK_PORT,THSCK_PIN);
  GPIO_ResetBits(THSDA_PORT,THSDA_PIN);
  GPIO_ResetBits(THDATA_PORT,THDATA_PIN);
  */
  
  THPOW_PORT->ODR |= THPOW_PIN;
  THSEN_PORT->ODR &= (uint8_t)(~THSEN_PIN);
  THSCK_PORT->ODR &= (uint8_t)(~THSCK_PIN);
  THSDA_PORT->ODR &= (uint8_t)(~THSDA_PIN);
  THDATA_PORT->ODR &= (uint8_t)(~THDATA_PIN);
      
  
  //TH7122PWMInit();
}

void TH7122Write24Bit(unsigned char * buf)
{
  GPIO_ResetBits(THSEN_PORT,THSEN_PIN);
  GPIO_ResetBits(THSEN_PORT,THSEN_PIN);
  for(unsigned char i = 0 ; i < 24 ; i ++)
  {
    if(buf[i]) GPIO_SetBits(THSDA_PORT,THSDA_PIN);
    else GPIO_ResetBits(THSDA_PORT,THSDA_PIN);
    
    GPIO_ResetBits(THSCK_PORT,THSCK_PIN);
    GPIO_SetBits(THSCK_PORT,THSCK_PIN);
    GPIO_SetBits(THSCK_PORT,THSCK_PIN);
    GPIO_ResetBits(THSCK_PORT,THSCK_PIN);
    GPIO_ResetBits(THSCK_PORT,THSCK_PIN);
  }
  GPIO_SetBits(THSEN_PORT,THSEN_PIN);
  GPIO_SetBits(THSEN_PORT,THSEN_PIN);
}

void TH7122PowerOn(void)
{
  //GPIO_ResetBits(THPOW_PORT,THPOW_PIN);
  
  THPOW_PORT->ODR &= (uint8_t)(~THPOW_PIN);
  
  Delay(60000);//Delay(60000);Delay(60000);Delay(60000);Delay(60000);
}

void TH7122PowerOff(void)
{
  //GPIO_SetBits(THPOW_PORT,THPOW_PIN);
  
  THPOW_PORT->ODR |= THPOW_PIN;
}

void TH7122WaitBusy(void)
{
  sysflag.WhileTime = 0;
  while(GPIO_ReadInputDataBit(THLOCK_PORT,THLOCK_PIN) == 0)
  {
    
    sysflag.WhileTime ++;
    if(sysflag.WhileTime == 5000)
    {
      sysflag.WhileTime = 0;
      IWDG->KR = IWDG_KEY_REFRESH;
      sysflag.WhileNum ++;
      if(sysflag.WhileNum == 10)
      {
        sysflag.WhileNum = 0;
        break;
      }
    }
  }
}

void TH7122SendData(unsigned char data)
{
  if(data == 1)
  {
    //GPIO_ResetBits(THDATA_PORT,THDATA_PIN);
    THDATA_PORT->ODR &= (uint8_t)(~THDATA_PIN);
    TH7122DelayUs(335);
    //GPIO_SetBits(THDATA_PORT,THDATA_PIN);
    THDATA_PORT->ODR |= (uint8_t)(THDATA_PIN);
    TH7122DelayUs(335);
  }
  else if(data == 0) 
  {
    //GPIO_SetBits(THDATA_PORT,THDATA_PIN);
    THDATA_PORT->ODR |= (uint8_t)(THDATA_PIN);
    TH7122DelayUs(335);
    //GPIO_ResetBits(THDATA_PORT,THDATA_PIN);
    THDATA_PORT->ODR &= (uint8_t)(~THDATA_PIN);
    TH7122DelayUs(335);
  }
  else
  {
    //GPIO_ResetBits(THDATA_PORT,THDATA_PIN);
    THDATA_PORT->ODR &= (uint8_t)(~THDATA_PIN);
    TH7122DelayUs(335);
    //GPIO_ResetBits(THDATA_PORT,THDATA_PIN);
    THDATA_PORT->ODR &= (uint8_t)(~THDATA_PIN);
    TH7122DelayUs(335);
  }
}

////////////////User///////////////
#include "th7122.h"
#include "sysdata.h"

//TH7122_STRUCT TH7122;

//起始配置文件

/*	MY SET NOW DEMO*/
unsigned char PACKET_A[24]={0,0,0,0,1,0,1,1,1,0,0,0,1,0,1,1,0,0,1,0,0,0,0,0};
unsigned char PACKET_B[24]={0,1,0,1,1,0,0,0,1,1,1,0,1,0,0,1,0,0,1,0,1,1,0,0};
unsigned char PACKET_C[24]={1,0,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,1,1,1};
unsigned char PACKET_D[24]={1,1,1,1,0,1,1,0,0,0,0,0,1,0,1,1,1,1,1,0,1,0,1,1};


//结束配置文件
unsigned char PACKET_A1[24]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
unsigned char PACKET_B1[24]={0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
unsigned char PACKET_C1[24]={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
unsigned char PACKET_D1[24]={1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};



#define THSENDTABLESTART    48      //发送数组起始位置

#define THSTART         0

#define THVERSIONS      THSTART
#define THVERSIONS_LEN  2

#define THTAMPER        THVERSIONS + THVERSIONS_LEN
#define THTAMPER_LEN    1

#define THBATTERY       THTAMPER + THTAMPER_LEN
#define THBATTERY_LEN   1

#define THMSID          THBATTERY + THBATTERY_LEN
#define THMSID_LEN      20

#define THMSV           THMSID + THMSID_LEN
#define THMSV_LEN       20

#define THCHECK         THSENDTABLESTART + THMSV + THMSV_LEN + 5 + 7

unsigned char PACKETSDATATable[49];

unsigned char PACKETSTable[128] = 
{
  2,2,2,2,2,2,2,2,
  2,2,2,2,2,2,2,2,
  2,2,2,2,2,2,2,1,
  1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,0,0,
  
  1,0,0,0,0,0,0,1,
  0,0,0,1,0,1,1,1,
  1,0,1,0,1,0,0,1,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,1,0,0,0,1,
  
  0,0,1,0,1,1,1,0,
  0,0,1,0,1,1,1,0,
  
  2,2,2,2,2,2,2,2,
  //0,0,0,0,0,0,0,0,
};

void TH7122Start(void)
{
  TH7122PowerOn();
  TH7122Write24Bit(PACKET_A);
  TH7122Write24Bit(PACKET_B);
  TH7122Write24Bit(PACKET_C);
  TH7122Write24Bit(PACKET_D);
  TH7122WaitBusy();
}

void TH7122End(void)
{
  //TH7122Write24Bit(PACKET_A1);
  //TH7122Write24Bit(PACKET_B1);
  //TH7122Write24Bit(PACKET_C1);
  //TH7122Write24Bit(PACKET_D1);
  TH7122PowerOff();
}

void THDataCheck(void)
{
  unsigned char Check;
  unsigned char i,j,k;
  unsigned char data_i;
  data_i = 0;
  k = THSENDTABLESTART;
  
  for(j = 0 ; j < 8 ; j ++)
  {
    for(i = 0 ; i < 7 ; i ++)
    {
      PACKETSTable[k] = PACKETSDATATable[data_i ++];
      Check += PACKETSTable[k ++];
    }
    PACKETSTable[k ++] = Check & 0x01;
    Check = 0;
  }
  
  k = THSENDTABLESTART;
  for(j = 0 ; j < 8 ; j ++)
  {
    for(i = 0 ; i < 7 ; i ++)
    {
      Check += PACKETSTable[k];
      k += 8;
    }
    PACKETSTable[k] = Check & 0x01;
    Check = 0;
    k -= 7*8;
    k ++;
  }
  
  for(i = 0 ; i < 8 ; i ++)
  {
    PACKETSTable[THCHECK + 8 + i] = PACKETSTable[THCHECK + i];
  }
  
}

void THUpDate(void)
{
  signed char i;
  uint32_t data;
  unsigned char len;
  unsigned char addr;
  
  for(unsigned char n = 0 ; n < 6 ; n ++)
  {
    if(n == 0)
    {
      data = sysdata.Versions;
      len = THVERSIONS_LEN;
      addr = THVERSIONS;
    }
    else if(n == 1)
    {
      data = sysdata.Tamper;
      len = THTAMPER_LEN;
      addr = THTAMPER;
    }
    else if(n == 2)
    {
      data = sysdata.Battery;
      len = THBATTERY_LEN;
      addr = THBATTERY;
    }
    else if(n == 3)
    {
      data = sysdata.MSID;
      len = THMSID_LEN;
      addr = THMSID;
    }
    else if(n == 4)
    {
      data = sysdata.MSV;
      len = THMSV_LEN;
      addr = THMSV;
    }
    else
    {
      data = 8;
      len = 5;
      addr = THMSV + THMSV_LEN;
    }
    for(i = len - 1 ; i >= 0 ; i --) PACKETSDATATable[addr + i] = data & 0x1,data >>= 1;
  }
  /*
  data = sysdata.Versions;
  len = THVERSIONS_LEN;
  addr = THVERSIONS;
  for(i = len - 1 ; i >= 0 ; i --) PACKETSDATATable[addr + i] = data & 0x1,data >>= 1;
  
  data = sysdata.Tamper;
  len = THTAMPER_LEN;
  addr = THTAMPER;
  for(i = len - 1 ; i >= 0 ; i --) PACKETSDATATable[addr + i] = data & 0x1,data >>= 1;
  
  data = sysdata.Battery;
  len = THBATTERY_LEN;
  addr = THBATTERY;
  for(i = len - 1 ; i >= 0 ; i --) PACKETSDATATable[addr + i] = data & 0x1,data >>= 1;
  
  data = sysdata.MSID;
  len = THMSID_LEN;
  addr = THMSID;
  for(i = len - 1 ; i >= 0 ; i --) PACKETSDATATable[addr + i] = data & 0x1,data >>= 1;
  
  data = sysdata.MSV;
  len = THMSV_LEN;
  addr = THMSV;
  for(i = len - 1 ; i >= 0 ; i --) PACKETSDATATable[addr + i] = data & 0x1,data >>= 1;
  
  data = 8;
  len = 5;
  addr = THMSV + THMSV_LEN;
  for(i = len - 1 ; i >= 0 ; i --) PACKETSDATATable[addr + i] = data & 0x1,data >>= 1;
  */
  THDataCheck();
}

void TH7122Send(void)
{
  THUpDate();
  
  TH7122Start();
  
  /*TH7122ITOff();
  TH7122.len = 104;
  TH7122.work = 1;
  TH7122PWMSend();
  TH7122ITOn();
  
  while(TH7122.finish == 0);
  TH7122.finish = 0;*/
  IWDG_ReloadCounter();
  SysdataReadBattery();
  Delay(60000);
  IWDG_ReloadCounter();
  
  
  unsigned char k;
  
  //while(1)
  for(k=0;k<128;k++)     
  {
    TH7122SendData(PACKETSTable[k]);
  }
  
  TH7122End();
}






