/**
  ******************************************************************************
  * File Name          : I2C.h
  * Description        : This file provides code for the configuration
  *                      of the I2C instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __i2c_H
#define __i2c_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern I2C_HandleTypeDef hi2c1;

/* USER CODE BEGIN Private defines */
#define LOCK_I2C_RESOURCE()          HAL_NVIC_DisableIRQ(ALERT_A_EXTI_IRQn);
#define UNLOCK_I2C_RESOURCE()         HAL_NVIC_EnableIRQ(ALERT_A_EXTI_IRQn);
/* USER CODE END Private defines */

extern void _Error_Handler(char *, int);

void MX_I2C1_Init(void);

/* USER CODE BEGIN Prototypes */
HAL_StatusTypeDef I2C_Read_USB_PD(uint8_t Port,uint16_t I2cDeviceID_7bit ,uint16_t Address ,void *DataR ,uint16_t Length);
HAL_StatusTypeDef I2C_Write_USB_PD(uint8_t Port,uint16_t I2cDeviceID_7bit ,uint16_t Address ,uint8_t *DataW ,uint16_t Length);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ i2c_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
