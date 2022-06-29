#ifndef TH7122_H_
#define TH7122_H_


//void TH7122PWMOn(void);
//void TH7122PWMOff(void);

////////////////User///////////////
/*typedef struct
{
  unsigned char work;
  unsigned char start;
  unsigned char finish;
  unsigned char len;
  unsigned char pwmlen;
}TH7122_STRUCT;

extern TH7122_STRUCT TH7122;

void TH7122Start(void);
void TH7122End(void);
*/
void TH7122Init(void);
void TH7122DeInit(void);
void TH7122Send(void);

void TH7122Start(void);
void TH7122SendData(unsigned char data);


#endif


