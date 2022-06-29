
//////////////SYSDATA/////////////

#include "sysdata.h"

SYSDATA_STRUCT sysdata;
SYSFLAG_STRUCT sysflag;

void Delay(__IO uint16_t nCount)
{
    /* Decrement nCount value */
    while (nCount--);
}

void WriteRTCWakeUp(uint32_t Alarm)
{
  //RTC_AlarmCmd(DISABLE);
  RTC_WakeUpCmd(DISABLE);
  RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div16);//32.768K/16=2.048k=0.488ms
  //RTC_WakeUpClockConfig(RTC_WakeUpClock_CK_SPRE_16bits);//32.768K/16=2.048k=0.488ms
  RTC_ITConfig(RTC_IT_WUT, ENABLE);//�����ж�
  //RTC_SetWakeUpCounter(120);//120S
  RTC_SetWakeUpCounter(Alarm);//2040/60=34Min
  RTC_ITConfig(RTC_IT_WUT, ENABLE);//���Ѷ�ʱ���ж�ʹ��
  RTC_WakeUpCmd(ENABLE);//RTC����ʹ��
}

void WriteRTCAlarm(uint32_t Alarm)
{
  //RTC_WakeUpCmd(DISABLE);
  RTC_AlarmCmd(DISABLE);
  RTC_AlarmTypeDef RTC_AlarmType;
  RTC_AlarmStructInit(&RTC_AlarmType);
  RTC_AlarmType.RTC_AlarmTime.RTC_Hours = Alarm / 3600;
  RTC_AlarmType.RTC_AlarmTime.RTC_Minutes = Alarm / 60 % 60;
  RTC_AlarmType.RTC_AlarmTime.RTC_Seconds = Alarm % 60;
  RTC_AlarmType.RTC_AlarmTime.RTC_H12 = RTC_H12_AM;
  RTC_AlarmType.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay;// | RTC_AlarmMask_Hours| RTC_AlarmMask_Minutes;
  RTC_AlarmType.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_Date;
  RTC_AlarmType.RTC_AlarmDateWeekDay = 2;
  RTC_SetAlarm(RTC_Format_BIN,&RTC_AlarmType);
  RTC_AlarmCmd(ENABLE);
}

void WriteRTCTime(uint32_t time)
{
  RTC_TimeTypeDef RTC_TimeType;
  RTC_TimeStructInit(&RTC_TimeType);
  RTC_TimeType.RTC_Hours = time / 3600;
  RTC_TimeType.RTC_Minutes = time / 60 % 60;
  RTC_TimeType.RTC_Seconds = time % 60;
  RTC_TimeType.RTC_H12 = RTC_H12_AM;
  RTC_SetTime(RTC_Format_BIN,&RTC_TimeType);
}

uint32_t ReadRTCTime(void)
{
  uint32_t time = 0;
  RTC_TimeTypeDef RTC_TimeRead;
  RTC_GetTime(RTC_Format_BIN,&RTC_TimeRead);
  
  time = RTC_TimeRead.RTC_Hours;
  time *= 3600;
  //time += RTC_TimeRead.RTC_Hours * 3600;
  time += RTC_TimeRead.RTC_Minutes * 60;
  time += RTC_TimeRead.RTC_Seconds;
  //time = ((RTC_TimeRead.RTC_Hours * 60 + RTC_TimeRead.RTC_Minutes) * 60) + RTC_TimeRead.RTC_Seconds;

  return time;
}

u8 write_eeprom(uint32_t address,uint8_t * data,u8 size)
{
    u8 check = 0;
    u8 i;
    if(IS_FLASH_DATA_EEPROM_ADDRESS(address))
    {
        FLASH_Unlock(FLASH_MemType_Data);
        for(i = 0; i < size ; i ++)
        {
                FLASH_ProgramByte(address + i,data[i]);
                check += data[i];
                while(!(FLASH->IAPSR & 0x04));
        }
        FLASH_ProgramByte(address + i,check);
        while(!(FLASH->IAPSR & 0x04));
        FLASH_Lock(FLASH_MemType_Data);
    }
    return 0;
}

u8 read_eeprom(uint32_t address,uint8_t * data,u8 size)
{
    u8 check = 0;
    u8 i;
    if(IS_FLASH_DATA_EEPROM_ADDRESS(address))
    {
        //FLASH_Unlock(FLASH_MemType_Data);
        for(i = 0; i < size ; i ++)
        {
            //data[i] = *(__IO uint8_t*)(address + i);
            data[i] = FLASH_ReadByte(address + i);
            check += data[i];
        }
        //FLASH_Lock(FLASH_MemType_Data);
        
        if(check == FLASH_ReadByte(address + i) && data[0] == 0xaa) return 0;
        else return 1;
    }
    return 1;
}

void PVDInit(void)                                        //PWR��ʼ��
{ 
  PWR_PVDLevelConfig(PWR_PVDLevel_2V26);                  //PVD����2.26V
  PWR_PVDCmd(ENABLE);                                     //PVDʹ��
  //PWR_PVDClearITPendingBit();
  //PWR_PVDITConfig(ENABLE);                                //PVD�ж�ʹ��
}

void PVDDeInit(void)                                        //~PWR��ʼ��
{ 
  PWR_PVDLevelConfig(PWR_PVDLevel_2V26);                  //PVD����2.26V
  PWR_PVDCmd(DISABLE);                                     //PVDʹ��
  //PWR_PVDITConfig(DISABLE);                                //PVD�ж�ʹ��
  //PWR_PVDClearITPendingBit();
}

//////////////////////////////User/////////////////////////////////

u8 SysdataRead(void);

void SysdataInit(void)
{
  if(SysdataRead())     //�����ݴ���
  {
    //��ʼ������
    sysdata.State = 0xaa;
    sysdata.Versions = 2;
    sysdata.Tamper = 0;
    sysdata.Battery = 0;
    sysdata.MSID = 0x1E240;
    sysdata.MSV = 0;
    
    write_eeprom(EEPROM_ADDR_BANK1,&(sysdata.State),16);
    write_eeprom(EEPROM_ADDR_BANK2,&(sysdata.State),16);
    SysdataSave();              //�����дһ��
  }
  /*
  read_eeprom(EEPROM_ADDR,&(sysdata.State),16);
  if(sysdata.State != 0xaa)
  {
    sysdata.State = 0xaa;
    sysdata.Versions = 2;
    sysdata.Tamper = 0;
    sysdata.Battery = 0;
    sysdata.MSID = 0x1E240;
    sysdata.MSV = 0;
    write_eeprom(EEPROM_ADDR,&(sysdata.State),16);
    
    if(read_eeprom(EEPROM_ADDR,&(sysdata.State),16)) while(1);
    //read_eeprom(EEPROM_ADDR,&(sysdata.State),16);
    //if(sysdata.State != 0xaa) while(1);
  }
  */
  //WriteRTCAlarm(0);
}

u8 SysdataRead(void)
{
  if(read_eeprom(EEPROM_ADDR_BANK1,&(sysdata.State),16))              //���ݴ���
  {
    if(read_eeprom(EEPROM_ADDR_BANK2,&(sysdata.State),16)) return 1;              //���ݴ���
  }
  return 0;
}

void SysdataSave(void)
{
  u8 buf1[16];
  u8 buf2[16];
  u8 res1;
  u8 res2;
  u8 i;
  u8 bank2 = 0;
  
  res1 = read_eeprom(EEPROM_ADDR_BANK1,buf1,16);
  res2 = read_eeprom(EEPROM_ADDR_BANK2,buf2,16);
  if(res1 == 0)         //������ȷ
  {
    if(res2 == 0)       //����������ȷ
    {
      for(i = 0 ; i < 16 ; i ++)        //�ȶ�ͬ��
      {
        if(buf1[i] != buf2[i]) break;
      }
    }
    else i = 0;
    
    if(i != 16)         //���ݲ�ͬ����ֱ�Ӹ�����д���µ�����
    {
      write_eeprom(EEPROM_ADDR_BANK2,&(sysdata.State),16);
      if(read_eeprom(EEPROM_ADDR_BANK2,buf2,16)) while(1);          //����д���󣬵ȴ����Ź���λ
      
      bank2 = 1;        //������������
    }
    
  }
  else if(res2) while(1);        //���ݶ����󣬵ȴ���ʼ����
  
  //������ȷ

  //����д����
  write_eeprom(EEPROM_ADDR_BANK1,&(sysdata.State),16);
  if(read_eeprom(EEPROM_ADDR_BANK1,buf1,16)) while(1);          //����д���󣬵ȴ����Ź���λ
  
  //�����´�д��ʱ�򱸷ݲ�ͬ������д���µı���
  if(bank2 == 0)          //������ȷ�����ݲ������£�׼��д����
  {
//    write_eeprom(EEPROM_ADDR_BANK2,&(sysdata.State),16);
//    if(read_eeprom(EEPROM_ADDR_BANK2,buf2,16)) while(1);          //����д���󣬵ȴ����Ź���λ
  }
  
}

void SysdataWriteVersions(uint32_t data)
{
  sysdata.Versions = data;
  SysdataSave();
}
/*
void SysdataWriteTamper(uint32_t data)
{
  sysdata.Tamper = data;
  SysdataSave();
}
void SysdataWriteBattery(uint32_t data)
{
  sysdata.Battery = data;
  SysdataSave();
}
*/
void SysdataWriteMSID(uint32_t id)
{
  sysdata.MSID = id;
  SysdataSave();
}
void SysdataWriteMSV(uint32_t data)
{
  sysdata.MSV = data;
  SysdataSave();
}
void SysdataUpMSV(void)
{
  if(sysdata.VTTime != 0) sysdata.VTFlag = 1;
  else sysdata.VTTime = 3;
  //int32_t data;
  //data = ReadRTCTime() - sysdata.S1TIME;
  //if(!(data > -5 && data < 5))
  //{
  //  sysdata.S1TIME = ReadRTCTime();
  //  sysdata.MSV ++;
  //  if(sysdata.MSV == 1000000) sysdata.MSV = 0;
  //  //SysdataSave();
  //}
}


/*void SysdataReadBattery(void)
{
  u16 i = 300;
  PVDInit();
  Delay(6000);
  //if(PWR_GetFlagStatus(PWR_FLAG_PVDOF) == 0) sysdata.Battery = 0;
  //else sysdata.Battery = 1;
  while(i--)
  {
    if(PWR_GetFlagStatus(PWR_FLAG_PVDOF) == 0) sysdata.Battery = 0;
    else sysdata.Battery = 1;
    Delay(10);
  }
  PVDDeInit();
}*/


void SysdataReadBattery(void)
{
  sysdata.PVDBatteryFlag = 0;
  PWR_PVDLevelConfig(PWR_PVDLevel_2V26);                  //20180228 pvd�޸�Ϊ2.26V    PVD����2.26V
  PWR_PVDCmd(ENABLE);                                     //PVDʹ��
  PWR_PVDClearITPendingBit();
  PWR_PVDITConfig(ENABLE);                                //PVD�ж�ʹ��
}

void SysdataUpdateTamper(void)
{
  if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1) == 0) sysdata.Tamper = 1;
  else sysdata.Tamper = 0;
}






