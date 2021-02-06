/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DHT11_H
#define __DHT11_H

#include "stm32f10x.h"

#define DHT11_IO_IN()  DHT11_INMode()
#define DHT11_IO_OUT() DHT11_OUTMode()

#define DHT11_IO_H()   GPIO_SetBits(GPIOB, GPIO_Pin_14 );  	       
#define DHT11_IO_L() 	 GPIO_ResetBits(GPIOB, GPIO_Pin_14 ); 	   


#define DHT11_DQ_IN 			GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)/////////////////


#define u8 unsigned char


u8 DHT11_Init(void);//初始化DHT11
u8 DHT11_Read_Data(float *temp,float *humi);//读取温湿度
u8 DHT11_Read_Byte(void);//读出一个字节
u8 DHT11_Read_Bit(void);//读出一个位
u8 DHT11_Check(void);//检测是否存在DHT11
void DHT11_Rst(void);//复位DHT11    

void DHT11_OUTMode(void);
void DHT11_INMode(void);     
	 
	 
	 
#endif

