/**
  ******************************************************************************
  * @file    Project/STM8L15x_StdPeriph_Template/stm8l15x_it.c
  * @author  MCD Application Team
  * @version V1.6.1
  * @date    30-September-2014
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm8l15x_it.h"
    
#include "sysdata.h"

/** @addtogroup STM8L15x_StdPeriph_Template
  * @{
  */
	
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/

#ifdef _COSMIC_
/**
  * @brief Dummy interrupt routine
  * @par Parameters:
  * None
  * @retval 
  * None
*/
INTERRUPT_HANDLER(NonHandledInterrupt,0)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
#endif

/**
  * @brief TRAP interrupt routine
  * @par Parameters:
  * None
  * @retval 
  * None
*/
INTERRUPT_HANDLER_TRAP(TRAP_IRQHandler)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
  * @brief FLASH Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(FLASH_IRQHandler,1)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
  * @brief DMA1 channel0 and channel1 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(DMA1_CHANNEL0_1_IRQHandler,2)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
  * @brief DMA1 channel2 and channel3 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(DMA1_CHANNEL2_3_IRQHandler,3)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
  * @brief RTC / CSS_LSE Interrupt routine.
  * @param  None
  * @retval None
  */
#include "sysdata.h"
INTERRUPT_HANDLER(RTC_CSSLSE_IRQHandler,4)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
   //RTC_WakeUpCmd(DISABLE);
   ////Delay(0x002F);
   //GPIO_ToggleBits(GPIOD,GPIO_Pin_0);
   //RTC_ClearITPendingBit(RTC_IT_WUT);  
   //RTC_WakeUpCmd(ENABLE);

  /*
  GPIO_ToggleBits(GPIOD,GPIO_Pin_0);
  
  RTC_WaitForSynchro();
  
  RTC_AlarmTypeDef RTC_AlarmRead;
  RTC_GetAlarm(RTC_Format_BIN,&RTC_AlarmRead);
  RTC_TimeTypeDef RTC_TimeRead;
  RTC_GetTime(RTC_Format_BIN,&RTC_TimeRead);
  
        //RTC_AlarmCmd(DISABLE);
  RTC_TimeTypeDef RTC_TimeType;
  RTC_TimeStructInit(&RTC_TimeType);
  RTC_TimeType.RTC_Hours = 11;
  RTC_TimeType.RTC_Minutes = 10;
  RTC_TimeType.RTC_Seconds = 0;
  RTC_TimeType.RTC_H12 = RTC_H12_AM;
  RTC_SetTime(RTC_Format_BIN,&RTC_TimeType);
  
        //RTC_AlarmCmd(ENABLE);
  */
  
  if(RTC_GetITStatus(RTC_IT_WUT) != RESET)
  {
    RTC_ClearITPendingBit(RTC_IT_WUT);
    sysflag.FastSleep = 1;
    //sysflag.send = 1;
    //sysflag.FastSleep = 0;
    
        if(sysdata.VTTime)
        {
          sysdata.VTTime --;
          if(sysdata.VTTime == 0)
          {
            if(!sysdata.VTFlag)
            {
                /*if(sysdata.MSVBit)
                {
                  sysdata.MSVBit = 0;*/
                  
                  sysdata.MSV ++;
                  if(sysdata.MSV == 1000000) sysdata.MSV = 0;
                /*}
                else sysdata.MSVBit = 1;*/
              
            }
            sysdata.VTFlag = 0;
          }
        }
        
        if(sysdata.TTTime)
        {
          sysdata.TTTime --;
          if(sysdata.TTTime == 0)
          {
            if(!sysdata.TTFlag)
            {
              sysdata.Tamper = 1;
            }
            sysdata.TTFlag = 0;
          }
        }
        
        if(sysdata.KeyTime != 0 && sysdata.KeyTime < 10)
        {
          sysdata.KeyTime ++;
          if(sysdata.KeyTime >= 10)
          {
            sysflag.send2min = 1;
            sysflag.FastSleep = 0;
          }
        }
  }
  
  if(RTC_GetITStatus(RTC_IT_ALRA) != RESET)
  {
    RTC_ClearITPendingBit(RTC_IT_ALRA);
    sysflag.send = 1;
    sysflag.FastSleep = 0;
  }
}
/**
  * @brief External IT PORTE/F and PVD Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTIE_F_PVD_IRQHandler,5)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
  if(PWR_GetFlagStatus(PWR_FLAG_PVDIF) != RESET)
  {
    PWR_PVDClearITPendingBit();
    sysdata.PVDBatteryFlag = 1;
  }
}

/**
  * @brief External IT PORTB / PORTG Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTIB_G_IRQHandler,6)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
  * @brief External IT PORTD /PORTH Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTID_H_IRQHandler,7)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
  * @brief External IT PIN0 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI0_IRQHandler,8)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
  EXTI_ClearITPendingBit (EXTI_IT_Pin0);//清除中断标志
  
  sysflag.FastSleep = 1;
  
  if((GPIOB->IDR & 0x01) == 0)
  {
    //if(sysdata.KeyTime == 0)
    sysdata.KeyTime = 1;
  }
  else
  {
    if(sysdata.KeyTime <= 4)      //s + 2
    {
      sysflag.send = 1;
      sysflag.FastSleep = 0;
    }
    /*else if(sysdata.KeyTime >= 10)      //s
    {
      sysflag.send2min = 1;
      sysflag.FastSleep = 0;
    }*/
    sysdata.KeyTime = 0;
  }
  
}

/**
  * @brief External IT PIN1 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI1_IRQHandler,9)           //ganrao
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
  EXTI_ClearITPendingBit (EXTI_IT_Pin1);//清除中断标志
  //GPIO_SetBits(GPIOD,GPIO_Pin_0);
  
  //sysdata.Tamper = 1;
  if(sysdata.TTTime != 0) sysdata.TTFlag = 1;
  sysdata.TTTime = 3;
  
  sysflag.FastSleep = 1;
}

/**
  * @brief External IT PIN2 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI2_IRQHandler,10)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
  EXTI_ClearITPendingBit (EXTI_IT_Pin2);//清除中断标志
  //GPIO_SetBits(GPIOD,GPIO_Pin_0);
  //sysflag.send = 1;
  
  sysflag.FastSleep = 1;
  SysdataUpMSV();
  
}

/**
  * @brief External IT PIN3 Interrupt routine.
  * @param  None
  * @retval None
  */
#include "uart.h"
INTERRUPT_HANDLER(EXTI3_IRQHandler,11)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
  UartInit();
  EXTI_ClearITPendingBit (EXTI_IT_Pin3);//清除中断标志
  sysflag.uart = 1;
  SUART.start = 1;
  //GPIO_SetBits(GPIOD,GPIO_Pin_0);
}

/**
  * @brief External IT PIN4 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI4_IRQHandler,12)          //干扰
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
  EXTI_ClearITPendingBit (EXTI_IT_Pin4);//清除中断标志
  
  //sysdata.Tamper = 1;
  
  //sysflag.FastSleep = 1;
  
}

/**
  * @brief External IT PIN5 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI5_IRQHandler,13)          //数据
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
  EXTI_ClearITPendingBit (EXTI_IT_Pin5);//清除中断标志
  
  //sysflag.FastSleep = 1;
  //SysdataUpMSV();
  
}

/**
  * @brief External IT PIN6 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI6_IRQHandler,14)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
  * @brief External IT PIN7 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI7_IRQHandler,15)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
  * @brief LCD /AES Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(LCD_AES_IRQHandler,16)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
  * @brief CLK switch/CSS/TIM1 break Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(SWITCH_CSS_BREAK_DAC_IRQHandler,17)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
  * @brief ADC1/Comparator Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(ADC1_COMP_IRQHandler,18)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
  * @brief TIM2 Update/Overflow/Trigger/Break /USART2 TX Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM2_UPD_OVF_TRG_BRK_USART2_TX_IRQHandler,19)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
  * @brief Timer2 Capture/Compare / USART2 RX Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM2_CC_USART2_RX_IRQHandler,20)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}


/**
  * @brief Timer3 Update/Overflow/Trigger/Break Interrupt routine.
  * @param  None
  * @retval None
  */
//#include "th7122.h"
//extern unsigned char PACKETSPWM[104];
INTERRUPT_HANDLER(TIM3_UPD_OVF_TRG_BRK_USART3_TX_IRQHandler,21)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
/*  TH7122ITOff();
  if(TIM3_GetITStatus(TIM3_IT_Update) != RESET)
  {
    TIM3_ClearITPendingBit(TIM3_IT_Update);
    if(TH7122.work)
    {
      if(TH7122.start == 0)
      {
        TH7122.pwmlen = 0;
        TH7122.start = 1;
        if(PACKETSPWM[TH7122.pwmlen]) TIM3_OC1PolarityConfig(TIM3_OCPolarity_High);
        else TIM3_OC1PolarityConfig(TIM3_OCPolarity_Low);
        TH7122.pwmlen ++;
        TH7122PWMOn();
      }
      else
      {
        if(TH7122.pwmlen == TH7122.len)
        {
          TH7122PWMOff();
          TH7122.finish = 1;
          TH7122.work = 0;
        }
        else
        {
          if(PACKETSPWM[TH7122.pwmlen]) TIM3_OC1PolarityConfig(TIM3_OCPolarity_High);
          else TIM3_OC1PolarityConfig(TIM3_OCPolarity_Low);
          TH7122.pwmlen ++;
        }
      }
    }
  }
  TH7122ITOn();*/
}
/**
  * @brief Timer3 Capture/Compare /USART3 RX Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM3_CC_USART3_RX_IRQHandler,22)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
  * @brief TIM1 Update/Overflow/Trigger/Commutation Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM1_UPD_OVF_TRG_COM_IRQHandler,23)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
  * @brief TIM1 Capture/Compare Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM1_CC_IRQHandler,24)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
  * @brief TIM4 Update/Overflow/Trigger Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM4_UPD_OVF_TRG_IRQHandler,25)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
  * @brief SPI1 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(SPI1_IRQHandler,26)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */		
}

/**
  * @brief USART1 TX / TIM5 Update/Overflow/Trigger/Break Interrupt  routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(USART1_TX_TIM5_UPD_OVF_TRG_BRK_IRQHandler,27)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
  * @brief USART1 RX / Timer5 Capture/Compare Interrupt routine.
  * @param  None
  * @retval None
  */
#include "uart.h"
INTERRUPT_HANDLER(USART1_RX_TIM5_CC_IRQHandler,28)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
   USART_ClearITPendingBit (USART1,USART_IT_RXNE);//清中断标志
   
   unsigned char data = USART_ReceiveData8 (USART1);
   
   if(SUART.buf_len == 0)
   {
     if(data == 0x40)
     {
      SUART.buf[SUART.buf_len ++] = data;
     }
   }
   else
   {
     if(SUART.buf_len < 12)
     {
      SUART.buf[SUART.buf_len ++] = data;
     }
     else SUART.buf_len = 0;
   }
   //USART_SendData8 (USART1,USART_ReceiveData8 (USART1));//将接收到的数据发送出去
}

/**
  * @brief I2C1 / SPI2 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(I2C1_SPI2_IRQHandler,29)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/