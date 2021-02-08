/**************************************************
������:
�����ͨ�������¶Ⱥ�ʪ���ϱ�OneNET��������ʾ��һ������һ����λ
�����˺��а�������ʱ���������뱾�ض�Ӧ�Ƶ�������������Ա�������
�ڰ�һ�Σ���Ϩ�𣬷������Ͽ���Ҳ���Թرյơ����൱��֧�ֲ��˺�����̨����̨���в��˼�����
����֮�⣬��ɳ����ʼ������Сϵͳ����LED��˸����ʾϵͳ����������

����֮�⣬���ⲡ��������GY906�������ģ�飬�ȿ��Բ��������¶ȣ�Ҳ�������ú���̽ͷ�޽Ӵ������������£�
����AD8232ģ��ɼ��ĵ�ͼ�Σ���HMI����������ʾ�ĵ粨�Ρ������¶ȡ�ʪ��
���²����������ұ��Ǹ����������������£���ʾ��HMI�������ϣ�2s���Զ�������ݣ��ȴ��´β�����

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
#include "bsp_adc.h"
#include "ds18b20.h"
#include "key.h"
#include "GY_906.h"
#include "ToUpperComputer.h"
#include "math.h"
#include "send_uart.h"
#include "bsp_usart.h"

float temperature = 0; 
float environment_temp = 0;
float human_temp = 0;

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
	ADCx_Init();
	USART_Config();    //usart3��ʼ������������������
}

void Net_Init()
{
	
	ESP8266_Init();					//��ʼ��ESP8266
	
	while(OneNet_DevLink())			//����OneNET
		delay_ms(500);
	LED = LED_ON;						//�����ɹ�
	
}

/**********************
�������ܣ�
��ʼ��������������HMI��������ʾ����

***********************/
void interface_init(void)
{
	
	OneNet_SendData(); //���ͳ�ʼ״̬����	
}

int main(void)
{
	
	unsigned char *dataPtr = NULL;
	unsigned short timeCount = 0;	//���ͼ������
  int j = 0,i = 0;
	u16 adcx,adco;
	Hardware_Init();				//��ʼ����ΧӲ��
	Net_Init();						//�����ʼ��

	I2C_GPIO_Config();
	
	interface_init();
	
	while(1)
	{	
		send_data();         //�����¶�ʪ�����ݵ�HMI������
		if(i%5 == 0)     //���ͷ��Ͳ������ݵ��ٶ�
			send_wave(ADC_ConvertedValue[0]/21,0,0);   //�����ĵ粨�ε�������
		
		
//	TransmitUserCurve_s32(0xf1,ADC_ConvertedValue[0]);
		deal_key_val();   //����������		
		
		timeCount ++;
		switch(timeCount)
		{
			case 1:DHT11_Read_Data(&temperature,&humidity);	break;//��ȡ��ʪ��ֵ	
			case 101:environment_temp = GY_906ambientNTC();
					break;
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
		i++;
		if(i == 15)
			LED = 1;
		else if(i == 30)
		{
			LED = 0;
			i = 0;
		}
		delay_ms(20);
	}
}





