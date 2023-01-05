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
#include "can.h"
#include "i2c.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
extern CAN_TxHeaderTypeDef   TxHeader;
extern uint8_t               TxData[8];
extern uint8_t               RxData[8];

//only Received element
uint8_t Temp;//FeedBack Temp of motor 
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

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
  MX_CAN_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
	
	int16_t Speed=0;//Speed of motor 1 

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	Motor_Calibration();
	while (1)
	{
		LED1OFF;
		LED2OFF;
		LED3OFF;
		LED4OFF;
		switch(KEY_Scan())
		{
			case up:
			{
				LED3ON;
				Speed++;
				break;
			}
			case down:
			{
				LED4ON;
				Speed--;
				break;
			}
			case enter:
			{
				LED2ON;
				Motor_Drive(Speed,1);
				break;
			}
			case back:
			{
				LED1ON;
				Speed=0;
				Motor_Drive(Speed,1);
				break;
			}
			default:break;
		}
	}
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
 
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
}

/* USER CODE BEGIN 4 */
/**
	*	@name		Motor_Set_FeedBack
  * @brief  This function is used to set the feedback mode of the motor 
  * @param  FeedBack If it is greater than 0x80, you need to obtain the 
						relevant value through the query function,Less than 0x40, 
						the motor actively reports fixed information, and the value
						determines the reporting time interval (default).ID determines
						the data storage location of the data frame.
  * @retval FBFeedback
  */
uint8_t Motor_Set_FeedBack(unsigned char FeedBack,uint8_t ID)
{
	uint16_t i=0;
	while(i<7)
	{
		TxData[i]=0; 
		i++;
	}
	TxData[ID-1]=FeedBack;
	TxHeader.StdId=0x106;
	USER_CAN_Send();
	return RxData[0];
}


/**
	*	@name		Motor_Calibration
  * @brief  This function is used to Motor calibration before operation 
  * @param  None
  * @retval None
  */
void Motor_Calibration(void)
{
	uint16_t i=0;
	while(i<8)
	{
		TxData[i++]=0;
	}
	TxHeader.StdId=0x104;
	USER_CAN_Send();
}

/**
	*	@name		Ck_Check
  * @brief  This function is used to The feedback method is to query and 
						specify the values of three objects on the premise of query. 
						0x01:Query speed 0x02:Query torque current 0x03:Query temperature
						0x04:Query location 0x05:Query fault code 0x06:Query mode.
  * @param  ID Check1 Check2 Check3 reporter
  * @retval None
  */ 
void Ck_Check(uint8_t ID,uint8_t Check1,uint8_t Check2,uint8_t Check3,reporter* reporter)
{
	TxData[0]=ID;
	TxData[1]=Check1;
	TxData[2]=Check2;
	TxData[3]=Check3;
	TxData[4]=0xAA;
	TxData[5]=0;
	TxData[6]=0;
	TxData[7]=0;
	TxHeader.StdId=0x107;
	USER_CAN_Send();
	
	switch(Check1)
	{
		case 1:
		{
			reporter->FBSpeed=(RxData[0]<<8)+RxData[1];
			break;
		}
		case 2:
		{
			reporter->ECurru=(RxData[0]<<8)+RxData[1];
			break;
		}
		case 3:
		{
			Temp=(RxData[0]<<8)+RxData[1];
			break;
		}
		case 4:
		{
			reporter->Position=(RxData[0]<<8)+RxData[1];
			break;
		}
		case 5:
		{
			reporter->ErrCode=(RxData[0]<<8)+RxData[1];
			break;
		}
		case 6:
		{
			reporter->FBMode=(RxData[0]<<8)+RxData[1];
			break;
		}
	}
	
	switch(Check2)
	{
		case 1:
		{
			reporter->FBSpeed=(RxData[2]<<8)+RxData[3];
			break;
		}
		case 2:
		{
			reporter->ECurru=(RxData[2]<<8)+RxData[3];
			break;
		}
		case 3:
		{
			Temp=(RxData[2]<<8)+RxData[3];
			break;
		}
		case 4:
		{
			reporter->Position=(RxData[2]<<8)+RxData[3];
			break;
		}
		case 5:
		{
			reporter->ErrCode=(RxData[2]<<8)+RxData[3];
			break;
		}
		case 6:
		{
			reporter->FBMode=(RxData[2]<<8)+RxData[3];
			break;
		}
	}
	
	switch(Check3)
	{
		case 1:
		{
			reporter->FBSpeed=(RxData[4]<<8)+RxData[5];
			break;
		}
		case 2:
		{
			reporter->ECurru=(RxData[4]<<8)+RxData[5];
			break;
		}
		case 3:
		{
			Temp=(RxData[4]<<8)+RxData[5];
			break;
		}
		case 4:
		{
			reporter->Position=(RxData[4]<<8)+RxData[5];
			break;
		}
		case 5:
		{
			reporter->ErrCode=(RxData[4]<<8)+RxData[5];
			break;
		}
		case 6:
		{
			reporter->FBMode=(RxData[4]<<8)+RxData[5];
			break;
		}
	
	}
}
/**
	*	@name		Obtain_Motor_Report
  * @brief  This function is used to Obtain the feedback
						information actively reported by the motor 
  * @param  report
  * @retval None
  */
void Obtain_Motor_Report(reporter*report)
{
	report->ECurru=(RxData[2]<<8)+RxData[3];
	report->FBSpeed=(RxData[0]<<8)+RxData[1];
	report->Position=(RxData[4]<<8)+RxData[5];
	report->ErrCode=RxData[6];
	report->FBMode=RxData[7];
}
/**
	*	@name		ID_Set
  * @brief  This function is used to Set motor ID 
  * @param  None
  * @retval FeedBack ID
  */
uint8_t ID_Set(uint8_t ID)
{
	uint16_t i=0;
	while(i<8)
	{
		TxData[i++]=0;
	}
	TxHeader.StdId=0x108;
	TxData[0]=ID;
	USER_CAN_Send();
	return RxData[0];
}

/**
	*	@name		Motor_SetMode
  * @brief  This function is used to Set motor mode
						mode 0x00 open loop 0x01 Current loop
						0x02 Velocity loop 0x03 Position ring
  * @param  Mode
  * @retval FeedBack Mode
  */
uint8_t Motor_SetMode(unsigned char Mode)
{
	uint16_t i=0;
	while(i<8)
	{
		TxData[i++]=0;
	}
	TxData[0]=Mode;
	TxHeader.StdId=0x105;
	USER_CAN_Send();
	return RxData[0];
}
/**
	*	@name		Send
  * @brief  This function is used to The driving motor changes the motion state
						If id = 2, the second data frame of speed or current position value
						is raised by eight bits and the third is lowered by eight bits. ID <= 8
						ID determines the data storage location of the data frame.
  * @param  Speed ID
  * @retval None
  */
void Motor_Drive(uint16_t Speed,uint8_t ID)
{
	uint16_t i=0;
	while(i<8)
	{
		TxData[i++]=0;
	}
	TxData[ID*2-2]=Speed>>8;
	TxData[ID*2-1]=Speed&0x00ff;
	TxHeader.StdId=0x32;
	USER_CAN_Send();			
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

