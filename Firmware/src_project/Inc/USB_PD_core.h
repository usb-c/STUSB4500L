/**
******************************************************************************
* File Name          : USB_PD_core.h
* Date               : 03/09/2019 10:51:46
* Description        : This file contains all the functions prototypes for 
*                      the USB_PD_core  
******************************************************************************
*
* COPYRIGHT(c) 2019 STMicroelectronics
*
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
*   1. Redistributions of source code must retain the above copyright notice,
*      this list of conditions and the following disclaimer.
*   2. Redistributions in binary form must reproduce the above copyright notice,
*      this list of conditions and the following disclaimer in the documentation
*      and/or other materials provided with the distribution.
*   3. Neither the name of STMicroelectronics nor the names of its contributors
*      may be used to endorse or promote products derived from this software
*      without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
******************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_PD_CORE_H
#define __USB_PD_CORE_H
#ifdef __cplusplus
extern "C" {
#endif
  
  /* Includes ------------------------------------------------------------------*/
#if defined (STM32L476xx)
#include "stm32l4xx_hal.h"
#elif defined(STM32F072xB)
#include "stm32f0xx_hal.h"
#elif defined(STM32F401xE)
#include "stm32f4xx_hal.h"
  
#endif


#include "USB_PD_defines_STUSB-GEN1S.h"
  
#define USBPORT_MAX 1
#define I2CBUS_MAX 1
  
#define true 1
#define false 0    
  
#define LE16(addr) (((uint16_t)(*((uint8_t *)(addr))))\
  + (((uint16_t)(*(((uint8_t *)(addr)) + 1))) << 8))

#define LE32(addr) ((((uint32_t)(*(((uint8_t *)(addr)) + 0))) + \
(((uint32_t)(*(((uint8_t *)(addr)) + 1))) << 8) + \
  (((uint32_t)(*(((uint8_t *)(addr)) + 2))) << 16) + \
    (((uint32_t)(*(((uint8_t *)(addr)) + 3))) << 24)))


typedef uint8_t bool;


  typedef struct  
    {
      uint8_t Usb_Port;
      uint8_t I2cBus ;
      uint8_t I2cDeviceID_7bit;
      uint8_t Dev_Cut;
      uint8_t Alert_GPIO_Pin;
      uint8_t Alert_GPIO_Bank;
    }USB_PD_I2C_PORT;


typedef struct  
  {
    uint8_t                                         Reserved0 ; 
    STUSB_GEN1S_CC_DETECTION_STATUS_RegTypeDef      Port_Status;/*!< Specifies the Port status register */
    uint8_t TypeC;
    STUSB_GEN1S_CC_STATUS_RegTypeDef                CC_status;
    STUSB_GEN1S_MONITORING_STATUS_RegTypeDef        Monitoring_status;         /*!< Specifies the  */
    STUSB_GEN1S_HW_FAULT_STATUS_RegTypeDef          HWFault_status;
    uint8_t Reserved1  ;    /*!< Specifies t */
    uint8_t Reserved2  ;    /*!< Specifies t */
  }USB_PD_StatusTypeDef;


void HW_Reset_state(uint8_t Usb_Port);
void SW_reset_by_Reg(uint8_t Usb_Port);
void usb_pd_init(uint8_t Usb_Port);
void ALARM_MANAGEMENT(uint8_t Usb_Port);
void Print_Type_C_Only_Status(uint8_t Usb_Port);



#ifdef __cplusplus
}
#endif

#endif /*usbpd core header */

/**
* @}
*/

/**
* @}
*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
