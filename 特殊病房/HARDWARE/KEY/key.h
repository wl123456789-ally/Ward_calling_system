#ifndef __KEY_H
#define __KEY_H	 

#include "sys.h"







#define KEY0  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8)//读取按键0
#define KEY1  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9)//读取按键1

 

#define KEY0_PRES 1	//KEY0按下
#define KEY1_PRES	2	//KEY1按下

extern int key_val;
extern int key_val_1,key_val_2;



void KEY_Init(void);//IO初始化
void get_jz(char flag);
void deal_key_val(void);

#endif
