
#include "stm8l15x.h"
#include "spi.h"



void SPI_init(void)
{
    nSEL_Dir_out();
    nSEL_Dir_out();
    SpiCLKOut();
    SpiSDOOut();
    SpiSDIIn();	
}

unsigned char SpiWriteReadByte(unsigned char ubByte)
{
    unsigned char ubi;
    unsigned char redata;

    SCK(0);
    for (ubi = 0; ubi < 8; ubi++)
    {
        redata <<= 1;
        if ((ubByte&0x80) == 0x80)
          SDO(1);
        else
          SDO(0);
        SCK(1);
        ubByte = ubByte << 1;
        if (SDI)
            redata |= 0x01;
        SCK(0);
    }
    
    asm("NOP");
    SDO(0);
    
    return redata;
}



void SpiWriteSingleAddressRegister(unsigned char Addr, unsigned char Data)
{
  Addr = Addr|0x80 ;  //write common  bit7=1
    
  CSN(0);
  SpiWriteReadByte(Addr);
  SpiWriteReadByte(Data);
  CSN(1);
}
void SpiWriteLongAddressRegister(unsigned int Addr, unsigned char Data)
{
  Addr = Addr|0xF000; //long address bit[14:12]=0b111 ,write common bit15=1;
  
  CSN(0);
  SpiWriteReadByte(Addr >> 8);
  SpiWriteReadByte(Addr & 0xff);
  SpiWriteReadByte(Data);
  CSN(1);
}
unsigned char SpiReadSingleAddressRegister(unsigned char Addr)
{
  unsigned char Data;
  Addr = Addr&0x7F ;//read common bit7=0
  
  CSN(0);
  SpiWriteReadByte(Addr);
  Data = SpiWriteReadByte(0);
  CSN(1);
  return Data;
}
unsigned char SpiReadLongAddressRegister(unsigned int Addr)
{
  unsigned char Data;
  Addr = Addr&0x7FFF; // read   bit15=0
  Addr = Addr|0x7000; // read long addr bit[14:12]=0b111
  
  CSN(0);
  SpiWriteReadByte(Addr >> 8);
  SpiWriteReadByte(Addr & 0xff);
  Data = SpiWriteReadByte(0);
  CSN(1);
  return Data;
}


unsigned char SpiReadAutoAddressRegister(unsigned int Addr)
{
  if(Addr < 0x70) return SpiReadSingleAddressRegister(Addr);
  else return SpiReadLongAddressRegister(Addr);
}

void SpiWriteAutoAddressRegister(unsigned int Addr,unsigned char Data)
{
  if(Addr < 0x70) SpiWriteSingleAddressRegister(Addr,Data);
  else SpiWriteLongAddressRegister(Addr,Data);
}




