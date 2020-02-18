/* USER CODE BEGIN Header */
/**
******************************************************************************
* @file           : main.c
* @brief          : Main program body
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
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "i2c.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include "USB_PD_core.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
I2C_HandleTypeDef *hi2c[I2CBUS_MAX];
unsigned int Address;
unsigned int AddressSize = I2C_MEMADD_SIZE_8BIT;
uint8_t Buffer[12];
USB_PD_I2C_PORT STUSB45DeviceConf[USBPORT_MAX];
extern uint32_t ConnectionStamp[];
uint32_t timer_cnt = 0;
int Flag_count = 0;
int PB_press=0;
int Time_elapse=1;
extern uint8_t Cut[USBPORT_MAX];
uint8_t USB_PD_Interupt_Flag[USBPORT_MAX] ;
uint8_t USB_PD_Interupt_PostponedFlag[USBPORT_MAX] ; 
uint8_t push_button_Action_Flag[USBPORT_MAX];
uint8_t Timer_Action_Flag[USBPORT_MAX];
uint8_t flag_once=1;
extern USB_PD_StatusTypeDef PD_status[USBPORT_MAX] ;


/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
void push_button_Action(void);
extern void nvm_flash(uint8_t Usb_Port);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
* @brief  The application entry point.
* @retval int
*/
int main(void)
{
  /* USER CODE BEGIN 1 */
  
  int Usb_Port = 0;
  int Status;
  /* USER CODE END 1 */
  
  
  /* MCU Configuration--------------------------------------------------------*/
  
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
  
  /* USER CODE BEGIN Init */
  
  /* USER CODE END Init */
  
  /* Configure the system clock */
  SystemClock_Config();
  
  /* USER CODE BEGIN SysInit */
  
  /* USER CODE END SysInit */
  
  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_I2C1_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  hi2c[0]= &hi2c1;
  STUSB45DeviceConf[Usb_Port].I2cBus = Usb_Port;
  STUSB45DeviceConf[Usb_Port].I2cDeviceID_7bit = 0x28;
  AddressSize = I2C_MEMADD_SIZE_8BIT; 
  
  USB_PD_Interupt_PostponedFlag[0]= 0; /* this flag is 1 if I2C is busy when Alert signal raise */
  USB_PD_Interupt_Flag[Usb_Port] = 1;
  
  usb_pd_init(Usb_Port);   // after this USBPD alert line must be high 
  
  push_button_Action_Flag[Usb_Port]=0;
  flag_once =1;
  /* USER CODE END 2 */
  
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
    /* USER CODE BEGIN 3 */
    if( push_button_Action_Flag[Usb_Port] == 1  )
      push_button_Action();  
    if( Timer_Action_Flag[Usb_Port] == 1  )
      Timer_Action();          
    if(USB_PD_Interupt_Flag[Usb_Port] != 1)
    {
      
      if ( flag_once == 1)
      {  
        
        if(PD_status[Usb_Port].Port_Status.b.CC_ATTACH_STATE  !=0)
        {
          int timeout = HAL_GetTick( );
          if ( (timeout - ConnectionStamp[Usb_Port]) > 16 )  /* the CC status is correctly updated after Tcc debounce */
          {
          Print_Type_C_Only_Status(Usb_Port);
          flag_once = 0 ; 
          }
        }
        else 
        {
#ifdef PRINTF    
    printf(" \r\n---- Usb_Port #%i: CONNECTION STATUS   ----------",Usb_Port);
    printf(" \r\n - DETACHED  ");
#endif    
          flag_once = 0 ;
        }
      }
    }
  }
  /* USER CODE END 3 */
}

/**
* @brief System Clock Configuration
* @retval None
*/
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
  
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
    |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2|RCC_PERIPHCLK_I2C1;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
/**
* @brief  EXTI line detection callback.
* @param  GPIO_Pin Specifies the port pin connected to corresponding EXTI line.
* @retval None
*/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  int Usb_Port = 0;
  switch(GPIO_Pin)
  {
    
  case ( ALERT_A_Pin)://B1_Pin 
    { 
      
      
      if((hi2c[0])->Lock == HAL_UNLOCKED)   
      {                                    
        ALARM_MANAGEMENT(Usb_Port);
      }
      else USB_PD_Interupt_Flag[Usb_Port] = 1;         
    } 
    break;
    
    // SELECT HERE WHICH function must be attached to the BLUE push button
  case ( B1_Pin)://B1_Pin 
    { 
      push_button_Action_Flag[Usb_Port] = 1 ;
      PB_press ++ ;
    } 
    break;
  }
}
void push_button_Action(void)
{
  uint8_t Usb_Port = 0;
#ifdef PRINTF
  printf("\r\n - Push button ");
#endif
  switch (PB_press%2)  /*  */
  {
  case 0:     /* */
    flag_once = 1 ; /* to print CC status*/
    break ;
  default :
    
    break;
  }
  push_button_Action_Flag[Usb_Port] = 0 ;
}

void Timer_Action(void)
{
  int Usb_Port = 0;
  
  if (PB_press == 0 )      
  {
    Time_elapse ++ ;        
  } 
  else  if(PB_press == 1 )  
  {
#ifdef PRINTF    
    printf("\r\n--- Timer Action ---\r\n");	
#endif   
    
    switch(Time_elapse )
    {
    case 1 :
      { 
        SW_reset_by_Reg(0);
#ifdef PRINTF          
        printf(" STUSB4500L  Reset by register \r\n");
#endif
        if (Flag_count)
          Time_elapse ++ ;
        else 
          Time_elapse -- ;  
      } break; 
      case 0 :
        {
        HW_Reset_state(0);
        /*update PDO num to 1 then Reset */
#ifdef PRINTF          
        printf(" STUSB4500L  Reset by GPIO \r\n");
#endif
        if (Flag_count)
          Time_elapse ++ ;
        else 
          Time_elapse -- ;  
      } 
    break;           
    default :
      Time_elapse = 1;
      break;            
    }
    
    
    if ( Time_elapse == 1)
      Flag_count = 1 ;
    if ( Time_elapse == 1)
      Flag_count = 0 ;
  }
  Timer_Action_Flag[Usb_Port] = 0 ;
}



/* USER CODE END 4 */

/**
* @brief  Period elapsed callback in non blocking mode
* @note   This function is called  when TIM1 interrupt took place, inside
* HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
* a global variable "uwTick" used as application time base.
* @param  htim : TIM handle
* @retval None
*/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */
  int Usb_Port=0;
  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */
  if (PD_status[Usb_Port].Port_Status.b.CC_ATTACH_STATE != 0)
  {
    timer_cnt ++ ;
    
    if (timer_cnt == 5000)
    {
      Timer_Action_Flag[0] = 1;
      timer_cnt = 0; 
    }
  }
  /* USER CODE END Callback 1 */
}

/**
* @brief  This function is executed in case of error occurrence.
* @retval None
*/
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
* @brief  Reports the name of the source file and the source line number
*         where the assert_param error has occurred.
* @param  file: pointer to the source file name
* @param  line: assert_param error line source number
* @retval None
*/
void assert_failed(char *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
  tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
