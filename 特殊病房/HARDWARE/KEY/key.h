#ifndef __KEY_H
#define __KEY_H	 

#include "sys.h"







#define KEY0  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8)//��ȡ����0
#define KEY1  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9)//��ȡ����1

 

#define KEY0_PRES 1	//KEY0����
#define KEY1_PRES	2	//KEY1����

extern int key_val;
extern int key_val_1,key_val_2;



void KEY_Init(void);//IO��ʼ��
void get_jz(char flag);
void deal_key_val(void);

#endif
