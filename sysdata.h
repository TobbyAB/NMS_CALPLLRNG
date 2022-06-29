
#ifndef __SYSDATA_H_
#define __SYSDATA_H_

#include "stm8l15x.h"

#define EEPROM_ADDR 0x1020
#define EEPROM_ADDR_BANK1 EEPROM_ADDR
#define EEPROM_ADDR_BANK2 EEPROM_ADDR + 20

typedef struct
{
  unsigned char State;
  unsigned char Versions;
  unsigned char Tamper;
  unsigned char Battery;
  uint32_t MSID;
  uint32_t MSV;
  uint32_t MST;
  
  uint32_t S1TIME;
  
  uint32_t SaveNum;
  
  unsigned char PVDBatteryFlag;
  
  unsigned char SendMode;
  
  //unsigned char MSVBit;
  unsigned char VTTime;
  unsigned char VTFlag;
  unsigned char TTTime;
  unsigned char TTFlag;
  
  unsigned char KeyTime;
}SYSDATA_STRUCT;

typedef struct
{
  unsigned char send;
  unsigned char uart;
  
  unsigned char FastSleep;
  
  unsigned char send2min;
  
  uint32_t WhileTime;
  uint32_t WhileNum;
  
}SYSFLAG_STRUCT;

extern SYSDATA_STRUCT sysdata;
extern SYSFLAG_STRUCT sysflag;

void Delay(__IO uint16_t nCount);

void WriteRTCWakeUp(uint32_t Alarm);
void WriteRTCAlarm(uint32_t Alarm);
void WriteRTCTime(uint32_t time);
uint32_t ReadRTCTime(void);
u8 write_eeprom(uint32_t address,uint8_t * data,u8 size);
u8 read_eeprom(uint32_t address,uint8_t * data,u8 size);
void SysdataInit(void);
void SysdataSave(void);
void SysdataWriteVersions(uint32_t data);
void SysdataWriteTamper(uint32_t data);
void SysdataWriteBattery(uint32_t data);
void SysdataWriteMSID(uint32_t id);
void SysdataWriteMSV(uint32_t data);
void SysdataUpMSV(void);

void SysdataReadBattery(void);
void SysdataUpdateTamper(void);

#endif




