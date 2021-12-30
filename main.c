/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usb_device.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usbd_hid.h"
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
extern USBD_HandleTypeDef hUsbDeviceFS;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t report[8]={0,0,0,0,0,0,0,0};
uint8_t reportPrevious[8]={0,0,0,0,0,0,0,0};
uint8_t reportFn[8]={0,0,0,0,0,0,0,0};

uint8_t fnPressed = 0;
uint8_t fnPressedKey = 0;
uint8_t fnPressedKeyPervious = 0;
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

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
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		
		KEY_SCAN();
		if(CheckReportChanged())
		{
			if(fnPressed)
			{
				SendFNReport();
			}
			else
			{
				USBD_HID_SendReport(&hUsbDeviceFS,report,8);
				memcpy(reportPrevious,report,sizeof(report));
			}
			
			HAL_Delay(10);
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

uint8_t CheckReportChanged(void)
{
	for(uint8_t i=0;i<8;i++)
	{
		if(report[i]!=reportPrevious[i]) return 1;
	}
	return 0 || fnPressed;
}

void KEY_SCAN(void)
{
	memset(report,0,sizeof(report));
	memset(reportFn,0,sizeof(reportFn));
	fnPressedKey = 0;
	SET_ROW1_DOWN;
	ColumnScan(0x01);
	SET_ROW1_UP;
	
	SET_ROW2_DOWN;
	ColumnScan(0x02);
	SET_ROW2_UP;
	
	SET_ROW3_DOWN;
	ColumnScan(0x03);
	SET_ROW3_UP;
	
	SET_ROW4_DOWN;
	ColumnScan(0x04);
	SET_ROW4_UP;
	
	SET_ROW5_DOWN;
	ColumnScan(0x05);
	SET_ROW5_UP;
	
	SET_ROW6_DOWN;
	fnPressed = 0;//reset fn
	ColumnScan(0x06);
	SET_ROW6_UP;
}

void ColumnScan(uint8_t rowNum)
{
	uint32_t columnNum = GetColumn();
	if(columnNum>0)
	{
		HAL_Delay(10);
		columnNum= GetColumn();
		if(columnNum>0)
		{
			SetReport(rowNum, columnNum);
		}			
	}
}
	
	

uint32_t GetColumn(void)
{
	uint32_t columnPressed=0;
	
	if(GET_COLUMN1  == GPIO_PIN_RESET) columnPressed|= 0x01   ; //0b000000000000000001
	if(GET_COLUMN2  == GPIO_PIN_RESET) columnPressed|= 0x02   ; //0b000000000000000010
	if(GET_COLUMN3  == GPIO_PIN_RESET) columnPressed|= 0x04   ; //0b000000000000000100
	if(GET_COLUMN4  == GPIO_PIN_RESET) columnPressed|= 0x08   ; //0b000000000000001000
	if(GET_COLUMN5  == GPIO_PIN_RESET) columnPressed|= 0x10   ; //0b000000000000010000
	if(GET_COLUMN6  == GPIO_PIN_RESET) columnPressed|= 0x20   ; //0b000000000000100000
	if(GET_COLUMN7  == GPIO_PIN_RESET) columnPressed|= 0x40   ; //0b000000000001000000
	if(GET_COLUMN8  == GPIO_PIN_RESET) columnPressed|= 0x80   ; //0b000000000010000000
	if(GET_COLUMN9  == GPIO_PIN_RESET) columnPressed|= 0x100  ; //0b000000000100000000
	if(GET_COLUMN10 == GPIO_PIN_RESET) columnPressed|= 0x200  ; //0b000000001000000000
	if(GET_COLUMN11 == GPIO_PIN_RESET) columnPressed|= 0x400  ; //0b000000010000000000
	if(GET_COLUMN12 == GPIO_PIN_RESET) columnPressed|= 0x800  ; //0b000000100000000000
	if(GET_COLUMN13 == GPIO_PIN_RESET) columnPressed|= 0x1000 ; //0b000001000000000000
	if(GET_COLUMN14 == GPIO_PIN_RESET) columnPressed|= 0x2000 ; //0b000010000000000000
	if(GET_COLUMN15 == GPIO_PIN_RESET) columnPressed|= 0x4000 ; //0b000100000000000000
	if(GET_COLUMN16 == GPIO_PIN_RESET) columnPressed|= 0x8000 ; //0b001000000000000000
	if(GET_COLUMN17 == GPIO_PIN_RESET) columnPressed|= 0x10000; //0b010000000000000000
	if(GET_COLUMN18 == GPIO_PIN_RESET) columnPressed|= 0x20000; //0b100000000000000000
	
	return columnPressed;
		
}

void SetReport(uint8_t rowNum, uint32_t columnNum)
{
	struct PressedKeyChars keys = GetKeyChar(rowNum, columnNum);
	
	
	if(fnPressed)
	{
		for(uint8_t i=0;i<KeyBoard_Columns;i++)
		{
			uint8_t keyChar=keys.keyChar[i];
			if(keyChar>0)
			{
				fnPressedKey=keyChar;
				break;
			}
		}
	}
	else
	{
		for(uint8_t i=0;i<KeyBoard_Columns;i++)//18 columns
		{
			uint8_t keyChar=keys.keyChar[i];
			if(keyChar>0)
			{
				if(IsModifierKey(keyChar))
				{
					switch(keyChar)
					{
						case 0xE0: report[0]|= 0x01;break;//0b00000001
						case 0xE1: report[0]|= 0x02;break;//0b00000010
						case 0xE2: report[0]|= 0x04;break;//0b00000100
						case 0xE3: report[0]|= 0x08;break;//0b00001000
						case 0xE4: report[0]|= 0x10;break;//0b00010000
						case 0xE5: report[0]|= 0x20;break;//0b00100000
						case 0xE6: report[0]|= 0x40;break;//0b01000000
						case 0xE7: report[0]|= 0x80;break;//0b10000000
						default: report[0]|=0x0; break;
					}
				}
				else
				{
					for(uint8_t j =2; j<8; j++)
					{
						if(report[j]>0)continue;
						report[j]=keyChar;
						break;
					}
				}
			}
		}
	}
	
	
}

void SendFNReport(void)
{
	if(fnPressedKey == fnPressedKeyPervious) return;//FN key only set once.
	if(fnPressedKey==Key_2)					
	{
		SendFNReportDetail(Key_Z,Key_None);		
	}
	fnPressedKeyPervious=fnPressedKey;
	memset(reportFn,0,sizeof(reportFn));
}

void SendFNReportDetail(uint8_t reportF,uint8_t reportFModifier)
{
	switch(reportFModifier)
	{
		case 0xE0: reportFn[0]|= 0x01;break;//0b00000001
		case 0xE1: reportFn[0]|= 0x02;break;//0b00000010
		case 0xE2: reportFn[0]|= 0x04;break;//0b00000100
		case 0xE3: reportFn[0]|= 0x08;break;//0b00001000
		case 0xE4: reportFn[0]|= 0x10;break;//0b00010000
		case 0xE5: reportFn[0]|= 0x20;break;//0b00100000
		case 0xE6: reportFn[0]|= 0x40;break;//0b01000000
		case 0xE7: reportFn[0]|= 0x80;break;//0b10000000
		default: reportFn[0]|=0x0; break;
	}
	
	reportFn[2]=reportF;
	USBD_HID_SendReport(&hUsbDeviceFS,reportFn,8);
	
	HAL_Delay(10);
	
	reportFn[0]=0;
	reportFn[2]=0;
	USBD_HID_SendReport(&hUsbDeviceFS,reportFn,8);
	HAL_Delay(10);
}

uint8_t IsModifierKey(uint8_t keyChar)
{
	if(keyChar>=0xE0 && keyChar<= 0xE7) return 1;
	return 0;
}

struct PressedKeyChars GetKeyChar(uint8_t rowNum, uint32_t columnNum)
{
	struct PressedKeyChars keys;
	memset(keys.keyChar,0,sizeof(keys.keyChar));
	switch(rowNum)
	{
		case 0x01: keys=GetRow1(columnNum);break;
		case 0x02: keys=GetRow2(columnNum);break;
		case 0x03: keys=GetRow3(columnNum);break;
		case 0x04: keys=GetRow4(columnNum);break;
		case 0x05: keys=GetRow5(columnNum);break;
		case 0x06: keys=GetRow6(columnNum);break;
	}
	return keys;
		
}

struct PressedKeyChars GetRow1(uint32_t columnNum)
{
	struct PressedKeyChars keys;
	memset(keys.keyChar,0,sizeof(keys.keyChar));
	
	if((columnNum & 0x01)    == 0x01)    keys.keyChar[0]  = Key_ESC;     //0b000000000000000001//ESC
	if((columnNum & 0x02)    == 0x02)    keys.keyChar[1]  = Key_F5;      //0b000000000000000010//F5
	if((columnNum & 0x04)    == 0x04)    keys.keyChar[2]  = Key_F1;      //0b000000000000000100//F1
	if((columnNum & 0x08)    == 0x08)    keys.keyChar[3]  = Key_F2;      //0b000000000000001000//F2
	if((columnNum & 0x10)    == 0x10)    keys.keyChar[4]  = Key_F3;      //0b000000000000010000//F3
	if((columnNum & 0x20)    == 0x20)    keys.keyChar[5]  = Key_F4;      //0b000000000000100000//F4
	if((columnNum & 0x40)    == 0x40)    keys.keyChar[6]  = Key_F5;      //0b000000000001000000//F5
	if((columnNum & 0x80)    == 0x80)    keys.keyChar[7]  = Key_F6;      //0b000000000010000000//F6
	if((columnNum & 0x100)   == 0x100)   keys.keyChar[8]  = Key_F7;      //0b000000000100000000//F7
	if((columnNum & 0x200)   == 0x200)   keys.keyChar[9]  = Key_F8;      //0b000000001000000000//F8
	if((columnNum & 0x400)   == 0x400)   keys.keyChar[10] = Key_Minus;   //0b000000010000000000//-_
	if((columnNum & 0x800)   == 0x800)   keys.keyChar[11] = Key_Equal;   //0b000000100000000000//=+
	if((columnNum & 0x1000)  == 0x1000)  keys.keyChar[12] = Key_F9 ;     //0b000001000000000000//F9
	if((columnNum & 0x2000)  == 0x2000)  keys.keyChar[13] = Key_F10;     //0b000010000000000000//F10
	if((columnNum & 0x4000)  == 0x4000)  keys.keyChar[14] = Key_F11;     //0b000100000000000000//F11
	if((columnNum & 0x8000)  == 0x8000)  keys.keyChar[15] = Key_F12;     //0b001000000000000000//F12
	if((columnNum & 0x10000) == 0x10000) keys.keyChar[16] = Key_None;    //0b010000000000000000//
	if((columnNum & 0x20000) == 0x20000) keys.keyChar[17] = Key_Print;   //0b100000000000000000//Print
	
	return keys;
}

struct PressedKeyChars GetRow2(uint32_t columnNum)
{
	struct PressedKeyChars keys;
	memset(keys.keyChar,0,sizeof(keys.keyChar));
	
	if((columnNum & 0x01)    == 0x01)    keys.keyChar[0]  = Key_wave; 				//0b000000000000000001//`~
	if((columnNum & 0x02)    == 0x02)    keys.keyChar[1]  = Key_1; 						//0b000000000000000010//1!
	if((columnNum & 0x04)    == 0x04)    keys.keyChar[2]  = Key_2; 						//0b000000000000000100//2@
	if((columnNum & 0x08)    == 0x08)    keys.keyChar[3]  = Key_3; 						//0b000000000000001000//3#
	if((columnNum & 0x10)    == 0x10)    keys.keyChar[4]  = Key_4; 						//0b000000000000010000//4$
	if((columnNum & 0x20)    == 0x20)    keys.keyChar[5]  = Key_5; 						//0b000000000000100000//5%
	if((columnNum & 0x40)    == 0x40)    keys.keyChar[6]  = Key_6; 						//0b000000000001000000//6^
	if((columnNum & 0x80)    == 0x80)    keys.keyChar[7]  = Key_7; 						//0b000000000010000000//7&
	if((columnNum & 0x100)   == 0x100)   keys.keyChar[8]  = Key_8; 						//0b000000000100000000//8*
	if((columnNum & 0x200)   == 0x200)   keys.keyChar[9]  = Key_9; 						//0b000000001000000000//9(
	if((columnNum & 0x400)   == 0x400)   keys.keyChar[10] = Key_0; 						//0b000000010000000000//0)
	if((columnNum & 0x800)   == 0x800)   keys.keyChar[11] = Key_Backspace; 		//0b000000100000000000//Backspace
	if((columnNum & 0x1000)  == 0x1000)  keys.keyChar[12] = Key_Home; 				//0b000001000000000000//Home
	if((columnNum & 0x2000)  == 0x2000)  keys.keyChar[13] = Key_PageUp; 			//0b000010000000000000//PageUp
	if((columnNum & 0x4000)  == 0x4000)  keys.keyChar[14] = Key_NumberLock; 	//0b000100000000000000//number lock
	if((columnNum & 0x8000)  == 0x8000)  keys.keyChar[15] = Key_KeyPad_Sprit; //0b001000000000000000//keypad/
	if((columnNum & 0x10000) == 0x10000) keys.keyChar[16] = Key_KeyPad_Star; 	//0b010000000000000000//*
	if((columnNum & 0x20000) == 0x20000) keys.keyChar[17] = Key_KeyPad_Minus; //0b100000000000000000//-
	
	return keys;
}

struct PressedKeyChars GetRow3(uint32_t columnNum)
{
	struct PressedKeyChars keys;
	memset(keys.keyChar,0,sizeof(keys.keyChar));
	
	if((columnNum & 0x01)    == 0x01)    keys.keyChar[0]  = Key_Tab; 					//0b000000000000000001//Tab
	if((columnNum & 0x02)    == 0x02)    keys.keyChar[1]  = Key_Q; 						//0b000000000000000010//Q
	if((columnNum & 0x04)    == 0x04)    keys.keyChar[2]  = Key_W; 						//0b000000000000000100//W
	if((columnNum & 0x08)    == 0x08)    keys.keyChar[3]  = Key_E; 						//0b000000000000001000//E
	if((columnNum & 0x10)    == 0x10)    keys.keyChar[4]  = Key_R; 						//0b000000000000010000//R
	if((columnNum & 0x20)    == 0x20)    keys.keyChar[5]  = Key_T; 						//0b000000000000100000//T
	if((columnNum & 0x40)    == 0x40)    keys.keyChar[6]  = Key_Y; 						//0b000000000001000000//Y
	if((columnNum & 0x80)    == 0x80)    keys.keyChar[7]  = Key_U; 						//0b000000000010000000//U
	if((columnNum & 0x100)   == 0x100)   keys.keyChar[8]  = Key_I; 						//0b000000000100000000//I
	if((columnNum & 0x200)   == 0x200)   keys.keyChar[9]  = Key_O; 						//0b000000001000000000//O
	if((columnNum & 0x400)   == 0x400)   keys.keyChar[10] = Key_P; 						//0b000000010000000000//P
	if((columnNum & 0x800)   == 0x800)   keys.keyChar[11] = Key_Return; 			//0b000000100000000000//Return
	if((columnNum & 0x1000)  == 0x1000)  keys.keyChar[12] = Key_End; 					//0b000001000000000000//End
	if((columnNum & 0x2000)  == 0x2000)  keys.keyChar[13] = Key_PageDown; 		//0b000010000000000000//PageDown
	if((columnNum & 0x4000)  == 0x4000)  keys.keyChar[14] = Key_KeyPad_7; 		//0b000100000000000000//keypad7
	if((columnNum & 0x8000)  == 0x8000)  keys.keyChar[15] = Key_KeyPad_8; 		//0b001000000000000000//keypad8
	if((columnNum & 0x10000) == 0x10000) keys.keyChar[16] = Key_KeyPad_9; 		//0b010000000000000000//keypad9
	if((columnNum & 0x20000) == 0x20000) keys.keyChar[17] = Key_KeyPad_Plus; 	//0b100000000000000000//keypad+
	
	return keys;
}

struct PressedKeyChars GetRow4(uint32_t columnNum)
{
	struct PressedKeyChars keys;
	memset(keys.keyChar,0,sizeof(keys.keyChar));
	
	if((columnNum & 0x01)    == 0x01)    keys.keyChar[0]  = Key_CapsLock; 		//0b000000000000000001//Caps Lock
	if((columnNum & 0x02)    == 0x02)    keys.keyChar[1]  = Key_A; 						//0b000000000000000010//A
	if((columnNum & 0x04)    == 0x04)    keys.keyChar[2]  = Key_S; 						//0b000000000000000100//S
	if((columnNum & 0x08)    == 0x08)    keys.keyChar[3]  = Key_D; 						//0b000000000000001000//D
	if((columnNum & 0x10)    == 0x10)    keys.keyChar[4]  = Key_F; 						//0b000000000000010000//F
	if((columnNum & 0x20)    == 0x20)    keys.keyChar[5]  = Key_G; 						//0b000000000000100000//G
	if((columnNum & 0x40)    == 0x40)    keys.keyChar[6]  = Key_H; 						//0b000000000001000000//H
	if((columnNum & 0x80)    == 0x80)    keys.keyChar[7]  = Key_J; 						//0b000000000010000000//J
	if((columnNum & 0x100)   == 0x100)   keys.keyChar[8]  = Key_K; 						//0b000000000100000000//K
	if((columnNum & 0x200)   == 0x200)   keys.keyChar[9]  = Key_L; 						//0b000000001000000000//L
	if((columnNum & 0x400)   == 0x400)   keys.keyChar[10] = Key_Semi; 				//0b000000010000000000//;:
	if((columnNum & 0x800)   == 0x800)   keys.keyChar[11] = Key_Return; 			//0b000000100000000000//Return
	if((columnNum & 0x1000)  == 0x1000)  keys.keyChar[12] = Key_Insert; 			//0b000001000000000000//Insert
	if((columnNum & 0x2000)  == 0x2000)  keys.keyChar[13] = Key_None; 				//0b000010000000000000//
	if((columnNum & 0x4000)  == 0x4000)  keys.keyChar[14] = Key_KeyPad_4; 		//0b000100000000000000//keypad4
	if((columnNum & 0x8000)  == 0x8000)  keys.keyChar[15] = Key_KeyPad_5; 		//0b001000000000000000//keypad5
	if((columnNum & 0x10000) == 0x10000) keys.keyChar[16] = Key_KeyPad_6; 		//0b010000000000000000//keypad6
	if((columnNum & 0x20000) == 0x20000) keys.keyChar[17] = Key_KeyPad_Plus; 	//0b100000000000000000//keypad+
	
	return keys;
}

struct PressedKeyChars GetRow5(uint32_t columnNum)
{
	struct PressedKeyChars keys;
	memset(keys.keyChar,0,sizeof(keys.keyChar));
	
	if((columnNum & 0x01)    == 0x01)    keys.keyChar[0]  = Key_LShift; 				//0b000000000000000001//Left Shift
	if((columnNum & 0x02)    == 0x02)    keys.keyChar[1]  = Key_Z; 							//0b000000000000000010//Z
	if((columnNum & 0x04)    == 0x04)    keys.keyChar[2]  = Key_X; 							//0b000000000000000100//X
	if((columnNum & 0x08)    == 0x08)    keys.keyChar[3]  = Key_C; 							//0b000000000000001000//C
	if((columnNum & 0x10)    == 0x10)    keys.keyChar[4]  = Key_V; 							//0b000000000000010000//V
	if((columnNum & 0x20)    == 0x20)    keys.keyChar[5]  = Key_B; 							//0b000000000000100000//B
	if((columnNum & 0x40)    == 0x40)    keys.keyChar[6]  = Key_N; 							//0b000000000001000000//N
	if((columnNum & 0x80)    == 0x80)    keys.keyChar[7]  = Key_M; 							//0b000000000010000000//M
	if((columnNum & 0x100)   == 0x100)   keys.keyChar[8]  = Key_Comma; 					//0b000000000100000000//,<
	if((columnNum & 0x200)   == 0x200)   keys.keyChar[9]  = Key_Dot; 						//0b000000001000000000//.>
	if((columnNum & 0x400)   == 0x400)   keys.keyChar[10] = Key_Question; 			//0b000000010000000000///?
	if((columnNum & 0x800)   == 0x800)   keys.keyChar[11] = Key_RShift; 				//0b000000100000000000//Right Shift
	if((columnNum & 0x1000)  == 0x1000)  keys.keyChar[12] = Key_Delete; 				//0b000001000000000000//Delete
	if((columnNum & 0x2000)  == 0x2000)  keys.keyChar[13] = Key_UpArrow; 				//0b000010000000000000//Up Arrow
	if((columnNum & 0x4000)  == 0x4000)  keys.keyChar[14] = Key_KeyPad_1; 			//0b000100000000000000//keypad1
	if((columnNum & 0x8000)  == 0x8000)  keys.keyChar[15] = Key_KeyPad_2; 			//0b001000000000000000//keypad2
	if((columnNum & 0x10000) == 0x10000) keys.keyChar[16] = Key_KeyPad_3; 			//0b010000000000000000//keypad3
	if((columnNum & 0x20000) == 0x20000) keys.keyChar[17] = Key_KeyPad_Enter; 	//0b100000000000000000//keypadEnter
	
	return keys;
}

struct PressedKeyChars GetRow6(uint32_t columnNum)
{
	struct PressedKeyChars keys;
	memset(keys.keyChar,0,sizeof(keys.keyChar));
	
	
	if((columnNum & 0x01)    == 0x01)    keys.keyChar[0]  = Key_LControl; 			//0b000000000000000001//Left Control
	if((columnNum & 0x02)    == 0x02)    keys.keyChar[1]  = Key_LWin; 					//0b000000000000000010//Left GUI
	if((columnNum & 0x04)    == 0x04)    keys.keyChar[2]  = Key_LAlt; 					//0b000000000000000100//Left Alt
	if((columnNum & 0x08)    == 0x08)    keys.keyChar[3]  = Key_RControl; 			//0b000000000000001000//Right Control
	if((columnNum & 0x10)    == 0x10)    keys.keyChar[4]  = Key_Space; 					//0b000000000000010000//Space
	if((columnNum & 0x20)    == 0x20)   																				//0b000000000000100000//FN 
	{
		keys.keyChar[5]  = 0x00;
		fnPressed = 1;
	} 
	if((columnNum & 0x40)    == 0x40)																						//0b000000000001000000//
	{
		keys.keyChar[6]  = 0x00;
		fnPressed = 1;
	}
	if((columnNum & 0x80)    == 0x80)    keys.keyChar[7]  = Key_Space; 					//0b000000000010000000//Space
	if((columnNum & 0x100)   == 0x100)   keys.keyChar[8]  = Key_Quote; 					//0b000000000100000000//'"
	if((columnNum & 0x200)   == 0x200)   keys.keyChar[9]  = Key_LBracket; 			//0b000000001000000000//[{
	if((columnNum & 0x400)   == 0x400)   keys.keyChar[10] = Key_RBracket; 			//0b000000010000000000//]}
	if((columnNum & 0x800)   == 0x800)   keys.keyChar[11] = Key_Slash; 					//0b000000100000000000//\|
	if((columnNum & 0x1000)  == 0x1000)  keys.keyChar[12] = Key_LeftArrow; 			//0b000001000000000000//Left Arrow
	if((columnNum & 0x2000)  == 0x2000)  keys.keyChar[13] = Key_DownArrow; 			//0b000010000000000000//Down Arrow
	if((columnNum & 0x4000)  == 0x4000)  keys.keyChar[14] = Key_RightArrow;			//0b000100000000000000//Right Arrow
	if((columnNum & 0x8000)  == 0x8000)  keys.keyChar[15] = Key_KeyPad_0; 			//0b001000000000000000//keypad0
	if((columnNum & 0x10000) == 0x10000) keys.keyChar[16] = Key_KeyPad_Dot; 		//0b010000000000000000//keypad . 
	if((columnNum & 0x20000) == 0x20000) keys.keyChar[17] = Key_KeyPad_Enter; 	//0b100000000000000000//keypadEnter
	
	return keys;
}



/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
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
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

