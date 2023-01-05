/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.h
  * @brief   This file contains all the function prototypes for
  *          the usart.c file
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
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern UART_HandleTypeDef huart1;

/* USER CODE BEGIN Private defines */
#define REC_LENGTH  10
#define MAX_REC_LENGTH  10
extern uint8_t Tx[10];
extern uint8_t Rx[10];

// Model:CRC-8/MAXIM 
//polynomial x8 + x5 + x4 + 1
extern const char CRC8Table[]; 
/*Received element*/
typedef struct Received
{
	uint8_t Temp;//Temp of motor 
	uint16_t ECurru;//electric current of motor 
	int16_t BSpeed;//FeedBack Speed of motor 
	uint16_t ErrCode;//Fault code of motor 
	uint8_t BMode;//Feedback Mode of motor
	uint16_t Position;//Feedback Position of motor
}Receiver;
/* USER CODE END Private defines */

void MX_USART1_UART_Init(void);

/* USER CODE BEGIN Prototypes */
extern Receiver Receiv;

void Control_Motor(uint16_t Speed,uint8_t ID,uint8_t Acce,uint8_t Brake_P,Receiver*Receiver);
unsigned char CRC8_Table(unsigned char *p, int counter);
void Get_Motor(uint8_t ID,Receiver*Receiver);
void Set_MotorMode(uint8_t Mode,uint8_t ID);
void Set_MotorID(uint8_t ID);
void Check_Motor(Receiver*Receiver);
void Send_Motor(void);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */

