#include "key.h"
#include "led.h"

int key_val = 0;
int key_val_1 = 0,key_val_2 = 0;  //第一次按下呼叫总台，在按一次清楚
//按键初始化函数
void KEY_Init(void) //IO初始化
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//使能PORTA,PORTE时钟

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_14|GPIO_Pin_15;//KEY0-KEY2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIOE2,3,4

}
/*****************************************
函数功能：获取键值
输入参数：0、1
          0：不做松手检测
          1：做松手检测,只针对WK_UP按键的松手检测
输出参数：无
*****************************************/
void get_jz(char flag)
{	 
  static int key_val_temp = 0,i = 0;
  if(KEY0==0||KEY1==0)
  {
    if(i>1)    //延时消逗
    {
      i = 0;
      
      if(KEY0==0) key_val_temp = 1;        //减键
      else if(KEY1==0)key_val_temp = 2;   //加键
      if(flag == 0)
        key_val = key_val_temp;
    }
    i++;   
  } 
  if(flag == 1)
  {
    if(key_val_temp != 0 && KEY0==1 && KEY1 == 1)
    {
      key_val = key_val_temp;
      key_val_temp = 0;
    }
  }
}
void deal_key_val(void)
{
	get_jz(1);
	if(key_val != 0)
	{
		switch(key_val)
		{
			case 1:key_val_1++;break;
			case 2:key_val_2++;break;
		}
		key_val_1 %= 2;
		key_val_2 %= 2;
		key_val = 0;	
	}
	if(key_val_1 == 1)
			LED_1 = 1;
		else
			LED_1 = 0; 
		if(key_val_2 == 1)
			LED_2 = 1;
		else
			LED_2 = 0; 		
}


