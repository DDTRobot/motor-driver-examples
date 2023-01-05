# M0601Instructions

## 0. Receive data

```
typedef struct Received
{
	uint8_t Temp;//Temp of motor 
	uint16_t ECurru;//electric current of motor 
	int16_t BSpeed;//FeedBack Speed of motor 
	uint16_t ErrCode;//Fault code of motor 
	uint8_t BMode;//Feedback Mode of motor
	uint16_t Position;//Feedback Position of motor
}Receiver;
```

The structure is used to save the received motor state.

```
uint8_t Tx[10];
uint8_t Rx[10];
```

Receive and send data cache.

## 1.Send Data to Motor

```
/**
	*	@name		Send_Motor
  * @brief  This function is used to Send data to motor.
  * @param  None
  * @retval None
  */
void Send_Motor(void)
{
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_SET);
	uint16_t err=HAL_UART_Transmit(&huart1,Tx,10,0xffff);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_RESET);
	HAL_Delay(10);
}
```

The motor is based on RS485 communication. This function sends data to the motor.

## 2.Calculate CRC

```
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
```

Get CRC8 check value.CRC verification is required for data frames sent by most function functions.

## 3.Control Motor

```
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
	Tx[3]=Speed&0x00FF;
	Tx[4]=0;
	Tx[5]=0;
	Tx[6]=Acce;
	Tx[7]=Brake_P;
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
```

## 4.Get other feedback from the motor

```
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
```

The difference between this function and the data frame obtained after transmitting the drive motor function lies in the feedback temperature.

## 5.Set Motor Mode

```
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
```

## 6.Set Motor ID

```
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
```

## 7.Check Motor

```
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
```