
#define ALERT_STATUS_1          0x0B // Interrupt register
/*************************************************************************************************************
  * @brief  	STUSB_GEN1S ALERT_STATUS register Structure definition
  * @Address: 	0Bh
  * @Access: 	RC
  * @Note: 	This register indicates an Alert has occurred. 
  *             (When a bit value change occurs on one of the mentioned transition register, it automatically 
  *             sets the corresponding alert bit in ALERT_STATUS register. )
  ************************************************************************************************************/
typedef union
{
  uint8_t d8;
  struct
  {
    uint8_t Reserved_0_3 			:       4; 
    uint8_t HW_FAULT_STATUS_AL 		:       1;    
    uint8_t MONITORING_STATUS_AL 	:       1;    
    uint8_t CC_DETECTION_STATUS_AL 	:       1;    
    uint8_t Reserved_7 				:       1;
  } b;
} STUSB_GEN1S_ALERT_STATUS_RegTypeDef;

#define ALERT_STATUS_MASK     0x0C // interrup MASK same mask as status shoud be used 
typedef union
{
  uint8_t d8;
  struct
  {
    uint8_t Reserved_0_3          		: 4; 
    uint8_t HW_FAULT_STATUS_AL_MASK     : 1;    
    uint8_t MONITORING_STATUS_AL_MASK   : 1;    
    uint8_t CC_DETECTION_STATUS_AL_MASK : 1;    
    uint8_t Reserved_7			        : 1;
  } b;
} STUSB_GEN1S_ALERT_STATUS_MASK_RegTypeDef;

#define PORT_STATUS_TRANS 0x0D 
/*************************************************************************************************************
  * @brief:  	STUSB_GEN1S CC_DETECTION_STATUS_TRANS register Structure definition
  * @Address: 	0Dh
  * @Access: 	RC
  * @Note: 	This register indicates a bit value change has occurred in CC_DETECTION_STATUS register.
  ************************************************************************************************************/ 
typedef union
{
  uint8_t d8;
  struct
  {
    uint8_t ATTACH_STATE_TRANS 	        :       1;
    uint8_t _Reserved_1_7		:       7;
  } b;
} STUSB_GEN1S_CC_DETECTION_STATUS_TRANS_RegTypeDef;

/*************************************************************************************************************
  * @brief:  	STUSB_GEN1S CC_DETECTION_STATUS register Structure definition
  * @Address: 	0Eh
  * @Access: 	RO
  * @Note: 	This register provides current status of the connection detection and corresponding operation modes.
  ************************************************************************************************************/ 
#define PORT_STATUS 0x0E
typedef union
{
  uint8_t d8;
  struct
  {
    uint8_t CC_ATTACH_STATE			:       1;
    uint8_t CC_VCONN_SUPPLY_STATE 		:       1;    
    uint8_t CC_DATA_ROLE 			:       1;   
    uint8_t CC_POWER_ROLE 			:       1; 
    uint8_t START_UP_POWER_MODE 		:       1;    
    uint8_t CC_ATTACH_MODE 			:       3;
  } b;
} STUSB_GEN1S_CC_DETECTION_STATUS_RegTypeDef;

/*************************************************************************************************************
  * @brief:  	STUSB_GEN1S MONITORING_STATUS_TRANS register Structure definition
  * @Address: 	0Fh
  * @Access: 	RC
  * @Note: 	This register allows to: 
  *              - Alert about any change that occurs in MONITORING_STATUS register.
  *              - Manage specific USB PD Acknowledge commands
  *              - to manage Type-C state machine Acknowledge to USB PD Requests commands
  ************************************************************************************************************/ 
#define TYPEC_MONITORING_STATUS_0 0x0F
typedef union
{
  uint8_t d8;
  struct
  {    
    uint8_t VCONN_VALID_TRANS 	:       1;
    uint8_t VBUS_VALID_SNK_TRANS	:       1;
    uint8_t VBUS_VSAFE0V_TRANS 	        :       1;
    uint8_t VBUS_READY_TRANS 	        :       1;
    uint8_t VBUS_LOW_STATUS		:       1;
    uint8_t VBUS_HIGH_STATUS		:       1;
    uint8_t Reserved6_7                 :       2;
  } b;
} STUSB_GEN1S_MONITORING_STATUS_TRANS_RegTypeDef;

/*************************************************************************************************************
  * @brief:  	STUSB_GEN1S MONITORING_STATUS register Structure definition
  * @Address: 	10h
  * @Access: 	RO
  * @Note: 	This register provides information on current status of the VBUS and VCONN voltages 
  *		monitoring done respectively on VBUS_SENSE input pin and VCONN input pin.
  ************************************************************************************************************/ 
#define TYPEC_MONITORING_STATUS_1 0x10
typedef union
{
  uint8_t d8;
  struct
  {  
    uint8_t VCONN_VALID	: 	1;
    uint8_t VBUS_VALID_SNK	: 	1;    
    uint8_t VBUS_VSAFE0V	: 	1;    
    uint8_t VBUS_READY		: 	1;   
    uint8_t _Reserved_4_7	:	4;    
  } b;
} STUSB_GEN1S_MONITORING_STATUS_RegTypeDef;

#define	CC_STATUS 0x11
typedef union
{
  uint8_t d8;
  struct
  {  
    uint8_t CC1_STATE	           : 	2;
    uint8_t CC2_STATE	           : 	2;    
    uint8_t CONNECT_RESULT	   : 	1;    
    uint8_t LOOKING_FOR_CONNECTION : 	1;   
    uint8_t _Reserved_4_7	   :	2;    
  } b;
} STUSB_GEN1S_CC_STATUS_RegTypeDef;

#define CC_HW_FAULT_STATUS_0 0x12
/************************************************************************************************************
  * @brief:  	STUSB_GEN1S HW_FAULT_STATUS_TRANS register Structure definition
  * @Address: 	12h
  * @Access: 	RC
  * @Note: 	This register indicates a bit value change has occurred in HW_FAULT_STATUS register. 
  *		It alerts also when the over temperature condition is met.
  ************************************************************************************************************/ 
typedef union
{
  uint8_t d8;
  struct
  {
        uint8_t VCONN_SW_OVP_FAULT_TRANS	: 	1;
        uint8_t VCONN_SW_OCP_FAULT_TRANS	:	1;    
        uint8_t VCONN_SW_RVP_FAULT_TRANS	: 	1;    
        uint8_t VBUS_VSRC_DISCH_FAULT_TRANS	: 	1;    
        uint8_t VPU_VALID_TRANS		        :	1;    
        uint8_t VPU_OVP_FAULT_TRANS 	        : 	1;    
        uint8_t _Reserved_6			: 	1;    
        uint8_t THERMAL_FAULT		        :	1;
  } b;
} STUSB_GEN1S_HW_FAULT_STATUS_TRANS_RegTypeDef;

#define CC_HW_FAULT_STATUS_1 0x13
/************************************************************************************************************
  * @brief:  	STUSB_GEN1S HW_FAULT_STATUS register Structure definition
  * @Address: 	13h
  * @Access: 	RO
  * @Note: 	This register provides information on hardware fault conditions related to the 
  *		internal pull-up voltage in Source power role and to the VCONN power switches
  ************************************************************************************************************/ 
typedef union
{
  uint8_t d8;
  struct
  {
        uint8_t VCONN_SW_OVP_FAULT		:	1;
        uint8_t VCONN_SW_OCP_FAULT		:	1;    
        uint8_t VCONN_SW_RVP_FAULT		:	1;    
        uint8_t VSRC_DISCH_FAULT		:	1;
        uint8_t Reserved                        :       1;
        uint8_t VBUS_DISCH_FAULT		:	1;    
        uint8_t VPU_PRESENCE			:	1;    
        uint8_t VPU_OVP_FAULT			:	1;     
  } b;
} STUSB_GEN1S_HW_FAULT_STATUS_RegTypeDef;

#define PD_TYPEC_STATUS
/************************************************************************************************************
  * @brief:  	STUSB_GEN1S PD_TYPEC_STATUS register Structure definition
  * @Address: 	14h
  * @Access: 	RO
  * @Note: 	This register provides handcheck from typeC 
  ************************************************************************************************************/ 
typedef union
{
  uint8_t d8;
  struct
  {
        uint8_t PD_TYPEC_HAND_CHECK	:	4;
        uint8_t Reserved_4_7		:	4;    
  } b;
} STUSB_GEN1S_PD_TYPEC_STATUS_RegTypeDef;

#define TYPE_C_STATUS 0x15
/************************************************************************************************************
  * @brief:  	STUSB_GEN1S TYPE_C_STATUS register Structure definition
  * @Address: 	15h
  * @Access: 	RO
  * @Note: 	This register provides information on typeC connection status
  ************************************************************************************************************/ 
typedef union
{
  uint8_t d8;
  struct
  {
        uint8_t TYPEC_FSM_STATE :	5;
        
        uint8_t PD_SNK_TX_RP 	:	1;
        uint8_t PD_SRC_TX_RP 	:	1;        
        uint8_t REVERSE		:	1;    
  } b;
} STUSB_GEN1S_TYPE_C_STATUS_RegTypeDef;


#define CC_CAPABILITY_CTRL 0x18
/************************************************************************************************************
  * @brief:  	STUSB_GEN1S CC_CAPABILITY_CTRL register Structure definition
  * @Address: 	18h
  * @Access: 	R/W
  * @Note: 	This register allows to change the advertising of the current capability and the VCONN 
  *		supply capability when operating in Source power role.
  ************************************************************************************************************/ 
typedef union
{
  uint8_t d8;
  struct
  {
        uint8_t CC_VCONN_SUPPLY_EN		:	1;
        uint8_t VCONN_SWAP_EN		        :	1;
        uint8_t PR_SWAP_EN		        :	1;
        uint8_t DR_SWAP_EN		        :	1;
        uint8_t CC_VCONN_DISCHARGE_EN	        :	1;
        uint8_t SNK_DISCONNECT_MODE		:	1;
        uint8_t CC_CURRENT_ADVERTISED	        :	2; 
  } b;
} STUSB_GEN1S_CC_CAPABILITY_CTRL_RegTypeDef;


#define 		SNK_UNATTACHED        0x00
#define 		SNK_ATTACHWAIT        0x01
#define 		SNK_ATTACHED          0x02
#define 		SNK_2_SRC_PR_SWAP     0x06
#define 		SNK_TRYWAIT           0x07
#define 		SRC_UNATTACHED        0x08
#define 		SRC_ATTACHWAIT        0x09
#define 		SRC_ATTACHED          0x0A
#define 		SRC_2_SNK_PR_SWAP     0x0B
#define 		SRC_TRY               0x0C
#define 		ACCESSORY_UNATTACHED  0x0D
#define 		ACCESSORY_ATTACHWAIT  0x0E
#define 		ACCESSORY_AUDIO       0x0F
#define 		ACCESSORY_DEBUG       0x10
#define 		ACCESSORY_POWERED     0x11
#define 		ACCESSORY_UNSUPPORTED 0x12
#define 		ERRORRECOVERY         0x13


#define STUSB_GEN1S_RESET_CTRL_REG			0x23
/************************************************************************************************************
  * @brief:  	STUSB RESET_CTRL register Structure definition
  * @Address: 	23h
  * @Access: 	R/W
  * @Note: 	This register allows to reset the device by software.
  *		The SW_RESET_EN bit acts as the hardware RESET pin but it does not command the RESET pin.
  ************************************************************************************************************/ 
typedef union
{
  uint8_t d8;
  struct
  {
        uint8_t SW_RESET_EN			:	1;
        uint8_t _Reserved_1_7		        :	7;
  } b;
} STUSB_GEN1S_RESET_CTRL_RegTypeDef;

/*0x23*/
/**
  * @brief USB PD SW RESET structures definition
  * @Address: 23h - Bit0
  * @Access: R/W
  */
typedef enum
{
  No_SW_RST                             = 0,           /*!< DEFAULT: Device reset is performed through the hardware RESET pin */
  SW_RST                                = 1            /*!< Force the device reset as long as this bit value is set */
} SW_RESET_TypeDef;

/*Identification of STUSB */
#define DEVICE_ID 0x2F
#define ID_Reg (uint8_t )0x1C 
#define GEN1S (uint8_t )8 << 2

#define DEV_CUT (uint8_t )0x03

/*NVM FLasher Registers Definition */

#define FTP_CUST_PASSWORD_REG	0x95
#define FTP_CUST_PASSWORD		0x47
#define FTP_CTRL_0              0x96
	#define FTP_CUST_PWR	0x80 
	#define FTP_CUST_RST_N	0x40
	#define FTP_CUST_REQ	0x10
	#define FTP_CUST_SECT 0x07
#define FTP_CTRL_1              0x97
	#define FTP_CUST_SER 0xF8
	#define FTP_CUST_OPCODE 0x07
#define RW_BUFFER 0x53
	
/*"000" then No Operation
"001" then Read 
"010" and FTP_ADR[2:0]="000" then Shift-In Write Bit Data (0x20-0x28). (to be done before Programming)
"010" and FTP_ADR[2:0]="001" then Shift-In Erase Sector Data (0x20). (to be done before Erasing)
"011" and FTP_ADR[2:0]="000" then Shift-Out Read Bit Data (0x20-0x28). (to be done after Reading) 
"011" and FTP_ADR[2:0]="001" then Shift-Out Erase Sector Data (0x20). (to be done after Erasing) 
"100" then Verify (to be done after Programming)
"101" then Erase
"110" then Program
"111" then Soft Programming (to be done after Erasing)*/
#define READ 0x00
#define WRITE_PL 0x01
#define WRITE_SER 0x02
#define READ_PL	0x03
#define READ_SER 0x04
#define ERASE_SECTOR 0x05
#define PROG_SECTOR 0x06
#define SOFT_PROG_SECTOR 0x07
        
    #define	SECTOR_0	0x01
	#define	SECTOR_1	0x02
	#define	SECTOR_2	0x04
	#define	SECTOR_3	0x08
	#define	SECTOR_4	0x10








