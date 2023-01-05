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

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
//Get the return value of motor Report
typedef struct report
{
	uint8_t ErrCode;//FeedBack ErrorCode of motor
	uint16_t FBSpeed;//FeedBack Speed of motor
	uint16_t ECurru;//FeedBack electric current of motor
	uint16_t Position;//FeedBack Position of motor
	uint8_t FBMode;//FeedBack Mode of motor
}reporter;
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void Motor_Drive(uint16_t Speed,uint8_t ID);
void Obtain_Motor_Report(reporter*report);
uint8_t Motor_Set_FeedBack(unsigned char FeedBack,uint8_t ID);
void Motor_Calibration(void);
void Ck_Check(uint8_t ID,uint8_t Check1,uint8_t Check2,uint8_t Check3,reporter* reporter);
uint8_t ID_Set(uint8_t ID);
uint8_t Motor_SetMode(unsigned char Mode);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
