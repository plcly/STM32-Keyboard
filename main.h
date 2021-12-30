/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
struct PressedKeyChars
{
	uint8_t keyChar[18];
};
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
uint8_t CheckReportChanged(void);
void SetReport(uint8_t rowNum, uint32_t columnNum);
void KEY_SCAN(void);
void ColumnScan(uint8_t rowNum);
struct PressedKeyChars GetKeyChar(uint8_t rowNum, uint32_t columnNum);
uint32_t GetColumn(void);
uint8_t IsModifierKey(uint8_t keyChar);
void SendFNReport(void);
void SendFNReportDetail(uint8_t reportF,uint8_t reportFModifier);

struct PressedKeyChars GetRow1(uint32_t columnNum);
struct PressedKeyChars GetRow2(uint32_t columnNum);
struct PressedKeyChars GetRow3(uint32_t columnNum);
struct PressedKeyChars GetRow4(uint32_t columnNum);
struct PressedKeyChars GetRow5(uint32_t columnNum);
struct PressedKeyChars GetRow6(uint32_t columnNum);

//char KEY_ROW_SCAN(void);
//void HW_KEY_FUNCTION(void);

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN Private defines */
#define KeyBoard_Columns 18

#define SET_ROW1_DOWN HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_RESET)
#define SET_ROW2_DOWN HAL_GPIO_WritePin(GPIOB,GPIO_PIN_11,GPIO_PIN_RESET)
#define SET_ROW3_DOWN HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_RESET)
#define SET_ROW4_DOWN HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,GPIO_PIN_RESET)
#define SET_ROW5_DOWN HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_RESET)
#define SET_ROW6_DOWN HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,GPIO_PIN_RESET)

#define SET_ROW1_UP HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_SET)
#define SET_ROW2_UP HAL_GPIO_WritePin(GPIOB,GPIO_PIN_11,GPIO_PIN_SET)
#define SET_ROW3_UP HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET)
#define SET_ROW4_UP HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,GPIO_PIN_SET)
#define SET_ROW5_UP HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_SET)
#define SET_ROW6_UP HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,GPIO_PIN_SET)

#define GET_COLUMN1  HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1)
#define GET_COLUMN2  HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0)
#define GET_COLUMN3  HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7)
#define GET_COLUMN4  HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6)
#define GET_COLUMN5  HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5)
#define GET_COLUMN6  HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4)
#define GET_COLUMN7  HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5)
#define GET_COLUMN8  HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6)
#define GET_COLUMN9  HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7)
#define GET_COLUMN10 HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_8)
#define GET_COLUMN11 HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9)
#define GET_COLUMN12 HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2)
#define GET_COLUMN13 HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3)
#define GET_COLUMN14 HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2)
#define GET_COLUMN15 HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1)
#define GET_COLUMN16 HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9)
#define GET_COLUMN17 HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10)
#define GET_COLUMN18 HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13)


#define Key_None    								0x00
#define Key_A   										0x04
#define Key_B   										0x05
#define Key_C   										0x06
#define Key_D   										0x07
#define Key_E   										0x08
#define Key_F   										0x09
#define Key_G   										0x0A
#define Key_H   										0x0B
#define Key_I   										0x0C
#define Key_J   										0x0D
#define Key_K   										0x0E
#define Key_L   										0x0F
#define Key_M   										0x10
#define Key_N   										0x11
#define Key_O   										0x12
#define Key_P   										0x13
#define Key_Q   										0x14
#define Key_R   										0x15
#define Key_S   										0x16
#define Key_T   										0x17
#define Key_U   										0x18
#define Key_V   										0x19
#define Key_W   										0x1A
#define Key_X   										0x1B
#define Key_Y   										0x1C
#define Key_Z   										0x1D
#define Key_wave      							0x35//`~ 
#define Key_1   										0x1E//1!
#define Key_2   										0x1F//2@
#define Key_3   										0x20//3#
#define Key_4   										0x21//4$
#define Key_5   										0x22//5%
#define Key_6   										0x23//6^
#define Key_7   										0x24//7&
#define Key_8   										0x25//8*
#define Key_9   										0x26//9(
#define Key_0   										0x27//0)
#define Key_Minus     							0x2D//-_  
#define Key_Equal     							0x2E//=+ 
#define Key_Semi 										0x33// 	;:
#define Key_Comma 									0x36// 	,<
#define Key_Dot 										0x37// 	.>
#define Key_Question 								0x38// 	/?
#define Key_Quote										0x34// 	'"
#define Key_LBracket								0x2F// 	[{
#define Key_RBracket								0x30// 	]}
#define Key_Slash										0x31// 	\|
#define Key_F1  										0x3A
#define Key_F2  										0x3B
#define Key_F3  										0x3C
#define Key_F4  										0x3D
#define Key_F5  										0x3E
#define Key_F6  										0x3F
#define Key_F7  										0x40
#define Key_F8  										0x41
#define Key_F9  										0x42 
#define Key_F10 										0x43 
#define Key_F11 										0x44 
#define Key_F12 										0x45 
#define Key_ESC 										0x29 
#define Key_KeyPad_1    						0x59 
#define Key_KeyPad_2    						0x5A 
#define Key_KeyPad_3	   						0x5B 
#define Key_KeyPad_4    						0x5C 
#define Key_KeyPad_5    						0x5D 
#define Key_KeyPad_6    						0x5E 
#define Key_KeyPad_7    						0x5F 
#define Key_KeyPad_8    						0x60 
#define Key_KeyPad_9    						0x61 
#define Key_KeyPad_0    						0x62 
#define Key_KeyPad_Dot    					0x63// KeyPad .
#define Key_KeyPad_Sprit  					0x54// KeyPad /  
#define Key_KeyPad_Star   					0x55// KeyPad *
#define Key_KeyPad_Minus  					0x56// KeyPad - 
#define Key_KeyPad_Plus 						0x57// KeyPad +
#define Key_KeyPad_Enter 						0x58// KeyPad Enter

#define Key_Print     							0x46
#define Key_Return      						0x28 
#define Key_Home          					0x4A 
#define Key_End         						0x4D 
#define Key_Insert 									0x49
#define Key_Delete 									0x4C
#define Key_PageUp        					0x4B
#define Key_PageDown    						0x4E
#define Key_Tab  										0x2B
#define Key_Backspace     					0x2A 
#define Key_NumberLock    					0x53
#define Key_CapsLock    						0x39
#define Key_Space    								0x2C

#define Key_UpArrow   							0x52
#define Key_DownArrow   						0x51
#define Key_LeftArrow    						0x50
#define Key_RightArrow    					0x4F

#define Key_LControl		 						0xE0
#define Key_LShift		 							0xE1
#define Key_LAlt 										0xE2
#define Key_LWin 										0xE3
#define Key_RControl 								0xE4
#define Key_RShift		 							0xE5
#define Key_RAlt 		 								0xE6
#define Key_RWin 										0xE7










/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
