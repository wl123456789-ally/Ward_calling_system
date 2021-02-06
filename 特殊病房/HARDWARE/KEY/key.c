#include "key.h"
#include "led.h"
#include "GY_906.h"


int key_val = 0;
int key_val_1 = 0;  //��һ�ΰ��º�����̨���ڰ�һ�����


extern float human_temp;

//������ʼ������
void KEY_Init(void) //IO��ʼ��
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//ʹ��PORTA,PORTEʱ��

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8|GPIO_Pin_9;//KEY0-KEY2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOE2,3,4

}
/*****************************************
�������ܣ���ȡ��ֵ
���������0��1
          0���������ּ��
          1�������ּ��,ֻ���WK_UP���������ּ��
�����������
*****************************************/
void get_jz(char flag)
{	 
  static int key_val_temp = 0,i = 0;
  if(KEY0==0||KEY1==0)
  {
    if(i>1)    //��ʱ����
    {
      i = 0;
      
      if(KEY0==0) key_val_temp = 1;        //����
      else if(KEY1==0)key_val_temp = 2;   //�Ӽ�
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
	static int i = 0;
	get_jz(1);
	if(key_val != 0)
	{
		switch(key_val)
		{
			case 1:key_val_1++;break;
			case 2:human_temp = GY_906NTC();i = 200;break;
		}
		key_val_1 %= 2;
		key_val = 0;		
		
	}	
	if(key_val_1 == 1)
		LED_1 = 1;
	else
		LED_1 = 0; 
	if(i > 0)
		i--;
	if(i == 0)
		human_temp = 0;
}


