/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.h
  * @brief   This file contains all the function prototypes for
  *          the gpio.c file
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
#ifndef __GPIO_H__
#define __GPIO_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */
/* red LED1  green LED2 white LED3 blue LED4  */
#define LED1ON HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_RESET)	
#define LED2ON HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,GPIO_PIN_RESET)	
#define LED3ON HAL_GPIO_WritePin(GPIOC,GPIO_PIN_14,GPIO_PIN_RESET)	
#define LED4ON HAL_GPIO_WritePin(GPIOC,GPIO_PIN_15,GPIO_PIN_RESET)	

#define LED1OFF HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_SET)	
#define LED2OFF HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,GPIO_PIN_SET)	
#define LED3OFF HAL_GPIO_WritePin(GPIOC,GPIO_PIN_14,GPIO_PIN_SET)	
#define LED4OFF HAL_GPIO_WritePin(GPIOC,GPIO_PIN_15,GPIO_PIN_SET)

#define KEY0  HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_12)//+
#define KEY1  HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_13)//-
#define KEY2  HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_14)//ENTERN
#define KEY3  HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_15)//back
 
#define up	1		//KEY0  
#define down	2		//KEY1 
#define enter	3		//KEY3  
#define back	4		//KEY4 


/* USER CODE END Private defines */

void MX_GPIO_Init(void);

/* USER CODE BEGIN Prototypes */
uint16_t KEY_Scan(void);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ GPIO_H__ */

