/**************************************************
������:
�����ͨ�������¶Ⱥ�ʪ���ϱ�OneNET��������ʾ��֧��һ�����䣬���Ŵ�λ������̨��
����λ1���а�������ʱ���������뱾�ض�Ӧ�Ƶ�������������Ա�������
�ڰ�һ�Σ���Ϩ�𣬷������Ͽ���Ҳ���Թرյơ����൱��֧�ֲ��˺�����̨����̨���в��˼�����
����֮�⣬��ɳ����ʼ������Сϵͳ����LED��˸����ʾϵͳ����������

��Ҫ�޸ĵط���esp8266.c��#define ESP8266_WIFI_INFO		"AT+CWJAP=\"ChinaNet-2.4G-9332\",\"1234567890\"\r\n"
��Ҫ�޸�Ϊ��Ҫ���ӵ�wifi

����˵����

DS18B20��PB13
DHT11  ��PB14
WIFIģ�飺RST���ţ�PB12
					TX��PA3
					RX��PA2
����ģ�飺PC14��PC15
��ӦLED�� PA0��PA1

��˸LED��PC13

***************************************************/


//C��
#include <string.h>

//��Ƭ��ͷ�ļ�
#include "sys.h"

//����Э���
#include "onenet.h"

//�����豸
#include "esp8266.h"

//Ӳ������
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "dht11.h"
#include "adc.h"
#include "ds18b20.h"
#include "key.h"


float temperature = 0; 
float temperature_18B20 = 0;
float humidity = 0;

//Ӳ����ʼ��
void Hardware_Init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	
	delay_init();			//��ʱ������ʼ��
	
	Usart1_Init(115200);	//����1��ʼ��Ϊ115200
	Usart2_Init(115200); 	//����2������ESP8266��
	LED_Init();				//LED��ʼ��
	DHT11_Init();					//dht11��ʼ��
	DS18B20_Init();
	KEY_Init();
	
	
}

void Net_Init()
{
	
	ESP8266_Init();					//��ʼ��ESP8266
	
	while(OneNet_DevLink())			//����OneNET
		delay_ms(500);
	LED = LED_ON;						//�����ɹ�
	
}

int main(void)
{
	unsigned char *dataPtr = NULL;
	unsigned short timeCount = 300;	//���ͼ������
  int i = 0;
	u16 adcx,adco;
	Hardware_Init();				//��ʼ����ΧӲ��
	Net_Init();						//�����ʼ��

//	Adc_Init();						//ADC��ʼ��   //PA0��PA1����ָʾ��

	OneNet_SendData();  //�ȷ��ͳ�ʼ���ݵ��������ϣ����г�ʼ��
	while(1)
	{	
		delay_ms(1);
		timeCount ++;
		i++;
		deal_key_val();
	  
		switch(timeCount)
		{
			case 1:DHT11_Read_Data(&temperature,&humidity);	break;//��ȡ��ʪ��ֵ	
			case 101:temperature_18B20 = DS18B20_Get_Temp();break;
			case 201:OneNet_SendData();	//��������										
							ESP8266_Clear();
				break;
			case 301:timeCount = 0;
				break;
			default:break;
		}
		
		dataPtr = ESP8266_GetIPD(0);
		if(dataPtr != NULL)
		{
			OneNet_RevPro(dataPtr);
		}	
		if(i == 20)
			LED = 1;
		else if(i == 40)
		{
			LED = 0;
			i = 0;
		}
	}
}





