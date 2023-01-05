/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
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
#include "usart.h"

/* USER CODE BEGIN 0 */
Receiver Receiv;
/* USER CODE END 0 */

UART_HandleTypeDef huart1;

/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

/**
	*	@name		Control_Motor
  * @brief  This function is used to Set the speed,acceleration
						 or braking of motor.The end of the data
						frame shall be CRC8 verified.
						(the default speed is 0, which can also be braked)
  * @param   Speed, ID, Acce, Brake_P, Receiver
  * @retval None
  */
void Control_Motor(uint16_t Speed,uint8_t ID,uint8_t Acce,uint8_t Brake_P,Receiver* Receiver)
{
	Tx[0]=ID;
	Tx[1]=0x64;
	Tx[2]=Speed>>8;
	Tx[3]=Speed&0x00ff;
	Tx[4]=0;
	Tx[5]=0;
	Tx[6]=Acce;
	Tx[7]=Brake_P;
	Tx[8]=0;
	Tx[9]=CRC8_Table(Tx,9);
	Send_Motor();
	
	HAL_Delay(1);
	Receiver->BMode=Rx[1];
	Receiver->ECurru=(Rx[2]<<8)+Rx[3];
	Receiver->BSpeed=(Rx[4]<<8)+Rx[5];
	Receiver->Position=(Rx[6]<<8)+Rx[7];
	Receiver->ErrCode=Rx[8];
}

/**
	*	@name		Get_Motor
  * @brief  This function is used Get other feedback from the motor
  * @param  ID,Receiver
  * @retval None
  */
void Get_Motor(uint8_t ID,Receiver*Receiver)
{
	Tx[0]=ID;
	Tx[1]=0x74;
	Tx[2]=0;
	Tx[3]=0;
	Tx[4]=0;
	Tx[5]=0;
	Tx[6]=0;
	Tx[7]=0;
	Tx[8]=0;
	Tx[9]=CRC8_Table(Tx,9);
	Send_Motor();
	
	HAL_Delay(10);
	Receiver->BMode=Rx[1];
	Receiver->ECurru=(Rx[2]<<8)+Rx[3];
	Receiver->BSpeed=(Rx[4]<<8)+Rx[5];
	Receiver->Temp=Rx[6];
	Receiver->Position=Rx[7];
	Receiver->ErrCode=Rx[8];
}

/**
	*	@name		Set_MotorMode
  * @brief  This function is used Set Mode of motor
  * @param  ID Mode
  * @retval None
  */
void Set_MotorMode(uint8_t Mode,uint8_t ID)
{
	Tx[0]=ID;
	Tx[1]=0xA0;
	Tx[2]=0;
	Tx[3]=0;
	Tx[4]=0;
	Tx[5]=0;
	Tx[6]=0;
	Tx[7]=0;
	Tx[8]=0;
	Tx[9]=Mode;
	Send_Motor();
}
/**
	*	@name		Set_MotorID
  * @brief  This function is used Set Mode of motor
  * @param  ID
  * @retval None
  */
void Set_MotorID(uint8_t ID)
{
	Tx[0]=0xAA;
	Tx[1]=0x55;
	Tx[2]=0x53;
	Tx[3]=ID;
	Tx[4]=0;
	Tx[5]=0;
	Tx[6]=0;
	Tx[7]=0;
	Tx[8]=0;
	Tx[9]=0;
	Send_Motor();
}

/**
	*	@name		Check_Motor
  * @brief  This function is used to Query motor.
  * @param  Receiver
  * @retval None
  */
void Check_Motor(Receiver*Receiver)
{
	Tx[0]=0xc8;
	Tx[1]=0x64;
	Tx[2]=0;
	Tx[3]=0;
	Tx[4]=0;
	Tx[5]=0;
	Tx[6]=0;
	Tx[7]=0;
	Tx[8]=0;
	Tx[9]=CRC8_Table(Tx,9);
	Send_Motor();
	
	HAL_Delay(10);
	Receiver->BMode=Rx[1];
	Receiver->ECurru=(Rx[2]<<8)+Rx[3];
	Receiver->BSpeed=(Rx[4]<<8)+Rx[5];
	Receiver->Position=(Rx[6]<<8)+Rx[7];
	Receiver->ErrCode=Rx[8];
}
/**
	*	@name		Send_Motor
  * @brief  This function is used to Send data to motor.
  * @param  None
  * @retval None
  */
void Send_Motor(void)
{
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_SET);
	HAL_UART_Transmit(&huart1,Tx,10,0xffff);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_RESET);
	HAL_Delay(10);
}
/**
	*	@name		CRC8_Table
  * @brief  This function is used to Get CRC8 check value.
	* @param  p :Array pointer. counter :Number of array elements.
  * @retval crc8
  */
unsigned char CRC8_Table(unsigned char *p, int counter)  
{  
    unsigned char crc8 = 0;  
  
    for( ; counter > 0; counter--)
	  {  
     crc8 = CRC8Table[crc8^*p];  
     p++;  
    }  
    return(crc8);  
}
/**
	*	@name		HAL_UART_RxCpltCallback
  * @brief  This function is used to Trigger the interrupt system
						to call automatically, and obtain the return value 
						through the receiving function.
	* @param  huart.
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    HAL_UART_Receive_IT(huart,(uint8_t *)Rx,10);
}

/* USER CODE END 1 */
