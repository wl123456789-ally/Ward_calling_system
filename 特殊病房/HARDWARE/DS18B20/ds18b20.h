
#ifndef __DS18B20_H
#define __DS18B20_H 

#include "stm32f10x.h"


////IO方向设置   也可以这样用
//#define DS18B20_IO_IN()  {GPIOC->CRL&=0x000000F0;GPIOC->CRL|=0x00000040;} //pC1	  浮空输入模式
//#define DS18B20_IO_OUT() {GPIOC->CRL&=0x000000F0;GPIOC->CRL|=0x00000030;} //PC1 通用推挽输出，50MHZ



//IO操作函数	 
#define DS18B20_DQ_High    GPIO_SetBits(GPIOB,GPIO_Pin_13)  //PC1   DQ=1	  
#define DS18B20_DQ_Low     GPIO_ResetBits(GPIOB,GPIO_Pin_13)//PC1   DQ=0
#define DS18B20_DQ_IN     GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13)  //输入DQ
   	
u8 DS18B20_Init(void);//初始化DS18B20
short DS18B20_Get_Temp(void);//获取温度
void DS18B20_Start(void);//开始温度转换
void DS18B20_Write_Byte(u8 dat);//写入一个字节
u8 DS18B20_Read_Byte(void);//读出一个字节
u8 DS18B20_Read_Bit(void);//读出一个位
u8 DS18B20_Check(void);//检测是否存在DS18B20
void DS18B20_Rst(void);//复位DS18B20   
void DS18B20_test(void);


void DS18B20_IO_IN(void);
void DS18B20_IO_OUT(void);

#endif


















