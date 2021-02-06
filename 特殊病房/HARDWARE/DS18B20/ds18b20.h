
#ifndef __DS18B20_H
#define __DS18B20_H 

#include "stm32f10x.h"


////IO��������   Ҳ����������
//#define DS18B20_IO_IN()  {GPIOC->CRL&=0x000000F0;GPIOC->CRL|=0x00000040;} //pC1	  ��������ģʽ
//#define DS18B20_IO_OUT() {GPIOC->CRL&=0x000000F0;GPIOC->CRL|=0x00000030;} //PC1 ͨ�����������50MHZ



//IO��������	 
#define DS18B20_DQ_High    GPIO_SetBits(GPIOB,GPIO_Pin_13)  //PC1   DQ=1	  
#define DS18B20_DQ_Low     GPIO_ResetBits(GPIOB,GPIO_Pin_13)//PC1   DQ=0
#define DS18B20_DQ_IN     GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13)  //����DQ
   	
u8 DS18B20_Init(void);//��ʼ��DS18B20
short DS18B20_Get_Temp(void);//��ȡ�¶�
void DS18B20_Start(void);//��ʼ�¶�ת��
void DS18B20_Write_Byte(u8 dat);//д��һ���ֽ�
u8 DS18B20_Read_Byte(void);//����һ���ֽ�
u8 DS18B20_Read_Bit(void);//����һ��λ
u8 DS18B20_Check(void);//����Ƿ����DS18B20
void DS18B20_Rst(void);//��λDS18B20   
void DS18B20_test(void);


void DS18B20_IO_IN(void);
void DS18B20_IO_OUT(void);

#endif


















