#include "USB_PD_core.h"

#include "i2c.h"


extern I2C_HandleTypeDef *hi2c[I2CBUS_MAX];	
extern unsigned int Address;

extern int PB_press ;
extern uint8_t flag_once;
uint8_t Cut[USBPORT_MAX];
USB_PD_StatusTypeDef PD_status[USBPORT_MAX] ;

uint32_t ConnectionStamp[USBPORT_MAX] = {0} ;
uint8_t TypeC_Only_status[USBPORT_MAX] = {0};


extern uint8_t  USB_PD_Interupt_Flag[USBPORT_MAX] ;
extern uint8_t USB_PD_Status_change_flag[USBPORT_MAX] ;

extern USB_PD_I2C_PORT STUSB45DeviceConf[USBPORT_MAX];


/**
* @brief  asserts and de-asserts the STUSB4500L Hardware reset pin.
* @param  I2C Port used (I2C1 or I2C2).
* @param  none
* @retval none
*/

/************************   HW_Reset_state(uint8_t Port)  ***************************
This function asserts and de-asserts the STUSB4500L Hardware reset pin.  
After reset, STUSB4500L behave according to Non Volatile Memory defaults settings. 
************************************************************************************/
void HW_Reset_state(uint8_t Usb_Port)
{

  LOCK_I2C_RESOURCE();
  HAL_GPIO_WritePin(Reset_GPIO_Port,Reset_Pin,GPIO_PIN_SET);
  HAL_Delay(15);  /*time to be dedected by the source */
  HAL_GPIO_WritePin(Reset_GPIO_Port,Reset_Pin,GPIO_PIN_RESET);
  HAL_Delay(15); /* this to left time to Device to load NVM*/
  usb_pd_init(0);
  UNLOCK_I2C_RESOURCE();
 
}

/************************   SW_reset_by_Reg (uint8_t Port)  *************************
This function resets STUSB4500L type-C and USB PD state machines. It also clears any
ALERT. By initialisating Type-C pull-down termination, it forces electrical USB type-C
disconnection (both on SOURCE and SINK sides). 
************************************************************************************/

void SW_reset_by_Reg(uint8_t Usb_Port)
{
  int Status;
  uint8_t Buffer[12];
  Buffer[0] = 1;
  LOCK_I2C_RESOURCE();
  Status = I2C_Write_USB_PD(STUSB45DeviceConf[Usb_Port].I2cBus,STUSB45DeviceConf[Usb_Port].I2cDeviceID_7bit ,STUSB_GEN1S_RESET_CTRL_REG,&Buffer[0],1 );
 
  if ( Status == HAL_OK)
  {
    Status = I2C_Read_USB_PD(STUSB45DeviceConf[Usb_Port].I2cBus,STUSB45DeviceConf[Usb_Port].I2cDeviceID_7bit ,ALERT_STATUS_1 ,&Buffer[0], 12 );  // clear ALERT Status
    HAL_Delay(27); // on source , the debounce time is more than 15ms error recovery < at 25ms 
    Buffer[0] = 0; 
    Status = I2C_Write_USB_PD(STUSB45DeviceConf[Usb_Port].I2cBus,STUSB45DeviceConf[Usb_Port].I2cDeviceID_7bit ,STUSB_GEN1S_RESET_CTRL_REG,&Buffer[0],1 ); 
  }
  UNLOCK_I2C_RESOURCE();
}



/***************************   usb_pd_init(uint8_t Port)  ***************************
this function clears all interrupts and unmask the usefull interrupt

************************************************************************************/

void usb_pd_init(uint8_t Usb_Port)
{
  STUSB_GEN1S_ALERT_STATUS_MASK_RegTypeDef Alert_Mask;
  int Status = HAL_OK ;
  static unsigned char DataRW[13];	
  DataRW[0]= 0;
  uint8_t ID_OK=0;
  do /* wait for NVM to be reloaded */
  {
    Status = I2C_Read_USB_PD(STUSB45DeviceConf[Usb_Port].I2cBus,STUSB45DeviceConf[Usb_Port].I2cDeviceID_7bit,DEVICE_ID ,&Cut[Usb_Port], 1 );
      
    if (Cut[Usb_Port] == (uint8_t)0x21)  ID_OK = 1;  // ST eval board
    if (Cut[Usb_Port] == (uint8_t)0x25)  ID_OK = 1;  // Product 
   } while( ID_OK == 0); 
  I2C_Read_USB_PD(STUSB45DeviceConf[Usb_Port].I2cBus,STUSB45DeviceConf[Usb_Port].I2cDeviceID_7bit,DEVICE_ID ,&Cut[Usb_Port], 1 );

#ifdef PRINTF
  if ((Cut[Usb_Port] == 0x21 )|| (Cut[Usb_Port] == 0x25) )
  { 
    printf("\r\n------------------------------------------------");
    printf("\r\n---- INIT: STUSB4500L  detected             ----");
    printf("\r\n- Port #%i                                   ----",Usb_Port);
    printf("\r\n- Device ID: 0x%02x                           ----",Cut[Usb_Port]);
    printf("\r\n------------------------------------------------\r\n");
  }
  else printf(" STUSB4500L Not detected\r\n");
#endif
  
  Alert_Mask.d8 = 0xFF;  
  Alert_Mask.b.CC_DETECTION_STATUS_AL_MASK = 0;
  DataRW[0]= Alert_Mask.d8; 
  if ( Status == HAL_OK)
  {
    Status = I2C_Write_USB_PD(STUSB45DeviceConf[Usb_Port].I2cBus,STUSB45DeviceConf[Usb_Port].I2cDeviceID_7bit ,ALERT_STATUS_MASK ,&DataRW[0], 1 ); // unmask port status alarm 
  }
  /* clear ALERT Status */
  Status = I2C_Read_USB_PD(STUSB45DeviceConf[Usb_Port].I2cBus,STUSB45DeviceConf[Usb_Port].I2cDeviceID_7bit ,ALERT_STATUS_1 ,&DataRW[0], 12 ); 
  
  USB_PD_Interupt_Flag[Usb_Port] =0;
  PD_status[Usb_Port].Port_Status.d8 = DataRW[ 3 ] ;
  PD_status[Usb_Port].CC_status.d8 = DataRW[6];
  PD_status[Usb_Port].HWFault_status.d8 = DataRW[8];
  PD_status[Usb_Port].Monitoring_status.d8=DataRW[5];
  TypeC_Only_status[Usb_Port] = 0;
  
  return;
}

/**********************   ALARM_MANAGEMENT(uint8_t Port)  ***************************
device interrupt Handler

************************************************************************************/

void ALARM_MANAGEMENT(uint8_t Usb_Port)   
{
  
  int Status = HAL_OK;
  STUSB_GEN1S_ALERT_STATUS_RegTypeDef Alert_Status;
  STUSB_GEN1S_ALERT_STATUS_MASK_RegTypeDef Alert_Mask;
  static unsigned char DataRW[40];
  
  
  if ( HAL_GPIO_ReadPin(ALERT_A_GPIO_Port,ALERT_A_Pin)== GPIO_PIN_RESET)
  {
    Address = ALERT_STATUS_1; 
    Status = I2C_Read_USB_PD(STUSB45DeviceConf[Usb_Port].I2cBus,STUSB45DeviceConf[Usb_Port].I2cDeviceID_7bit ,Address ,&DataRW[0], 2 );
    Alert_Mask.d8 = DataRW[1]; 
    Alert_Status.d8 = DataRW[0] & ~Alert_Mask.d8;
    if (Alert_Status.d8 != 0)
    {     
      
      if (Alert_Status.b.CC_DETECTION_STATUS_AL !=0)
      {
//        if (Status == HAL_OK) 
        flag_once = 1;
        Status = I2C_Read_USB_PD(STUSB45DeviceConf[Usb_Port].I2cBus,STUSB45DeviceConf[Usb_Port].I2cDeviceID_7bit ,PORT_STATUS_TRANS ,&DataRW[0], 2 );
        PD_status[Usb_Port].Port_Status.d8= DataRW[ 1 ]; 
        if(PD_status[Usb_Port].Port_Status.b.CC_ATTACH_STATE  != 0)
        {
          ConnectionStamp[Usb_Port] = HAL_GetTick() ;
          Status = I2C_Read_USB_PD(STUSB45DeviceConf[Usb_Port].I2cBus,STUSB45DeviceConf[Usb_Port].I2cDeviceID_7bit ,CC_STATUS ,&PD_status[Usb_Port].CC_status.d8, 1 );
        }
        else /* Detached detected */
        {
          
          ConnectionStamp[Usb_Port] = 0;
        }
      }
      if (Alert_Status.b.MONITORING_STATUS_AL !=0)
      {
        Status = I2C_Read_USB_PD(STUSB45DeviceConf[Usb_Port].I2cBus,STUSB45DeviceConf[Usb_Port].I2cDeviceID_7bit ,TYPEC_MONITORING_STATUS_0 ,&DataRW[0], 2 );
        PD_status[Usb_Port].Monitoring_status.d8 = DataRW[ 1 ];
      }
      Status = I2C_Read_USB_PD(STUSB45DeviceConf[Usb_Port].I2cBus,STUSB45DeviceConf[Usb_Port].I2cDeviceID_7bit ,CC_STATUS ,&DataRW[0], 1);
      PD_status[Usb_Port].CC_status.d8 = DataRW[ 0];
      
      if (Alert_Status.b.HW_FAULT_STATUS_AL !=0)
      {
        Status = I2C_Read_USB_PD(STUSB45DeviceConf[Usb_Port].I2cBus,STUSB45DeviceConf[Usb_Port].I2cDeviceID_7bit ,CC_HW_FAULT_STATUS_0 ,&DataRW[0], 2 );
        PD_status[Usb_Port].HWFault_status.d8 = DataRW[ 1 ]; 
      }
      
    }
    if(HAL_GPIO_ReadPin (GPIOA, ALERT_A_Pin  ) == GPIO_PIN_RESET)    
      USB_PD_Interupt_Flag[Usb_Port] = 1;
    else USB_PD_Interupt_Flag[Usb_Port] = 0;
    
  }
  
}



void Print_Type_C_Only_Status(uint8_t Usb_Port) 
{
  int Status;

#ifdef PRINTF    
    printf(" \r\n---- Usb_Port #%i: CONNECTION STATUS   ----------",Usb_Port);
    printf(" \r\n - ATTACHED to USB-C only device");
#endif    
    Status = I2C_Read_USB_PD(STUSB45DeviceConf[Usb_Port].I2cBus,STUSB45DeviceConf[Usb_Port].I2cDeviceID_7bit ,CC_STATUS ,&PD_status[Usb_Port].CC_status.d8, 1 );
    UNUSED(Status); /* to remove warning */
    ConnectionStamp[Usb_Port] = 0;
    if ( PD_status[Usb_Port].CC_status.b.CONNECT_RESULT)
    {
      TypeC_Only_status[Usb_Port] = 1;
#ifdef PRINTF
      switch( ( PD_status[Usb_Port].CC_status.b.CC1_STATE ) | ( PD_status[Usb_Port].CC_status.b.CC2_STATE << 2 ) )
      {
      case 0x1:
        printf(" \r\n - PIN   : CC1");
        printf(" \r\n - Power : Rp = USB Default");
        break;
      case 0x4:
        printf(" \r\n - PIN   : CC2");
        printf(" \r\n - Power : Rp = USB Default");
        break ;
      case 0x2:
        printf(" \r\n - PIN   : CC1");
        printf(" \r\n - Power : Rp = 1.5A");
        break;
      case 0x8:
        printf(" \r\n - PIN   : CC2");
        printf(" \r\n - Power : Rp = 1.5A");
        break ;
      case 0x3:
        printf(" \r\n - PIN   : CC1");
        printf(" \r\n - Power : Rp = 3A");
        break;
      case 0xC:
        printf(" \r\n - PIN   : CC2");
        printf(" \r\n - Power : Rp = 3A");
        break ;
      default :
        printf (" \r\n - DEVICE not supported");
        break ;
      }
#else 
      __NOP();    
#endif
    }
     
}