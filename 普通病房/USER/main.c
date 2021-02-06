/**************************************************
程序功能:
检测普通病房的温度和湿度上报OneNET服务器显示，支持一个房间，两张床位呼叫总台，
当床位1呼叫按键按下时，服务器与本地对应灯点亮，当工作人员处理完后，
在按一次，灯熄灭，服务器上开关也可以关闭灯。（相当于支持病人呼叫总台，总台呼叫病人家属）
除此之外，完成程序初始化后，最小系统板上LED闪烁，表示系统正常工作。

需要修改地方：esp8266.c中#define ESP8266_WIFI_INFO		"AT+CWJAP=\"ChinaNet-2.4G-9332\",\"1234567890\"\r\n"
需要修改为将要链接的wifi

引脚说明：

DS18B20：PB13
DHT11  ：PB14
WIFI模块：RST引脚：PB12
					TX：PA3
					RX：PA2
按键模块：PC14、PC15
对应LED： PA0、PA1

闪烁LED：PC13

***************************************************/


//C库
#include <string.h>

//单片机头文件
#include "sys.h"

//网络协议层
#include "onenet.h"

//网络设备
#include "esp8266.h"

//硬件驱动
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

//硬件初始化
void Hardware_Init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	
	delay_init();			//延时函数初始化
	
	Usart1_Init(115200);	//串口1初始化为115200
	Usart2_Init(115200); 	//串口2，驱动ESP8266用
	LED_Init();				//LED初始化
	DHT11_Init();					//dht11初始化
	DS18B20_Init();
	KEY_Init();
	
	
}

void Net_Init()
{
	
	ESP8266_Init();					//初始化ESP8266
	
	while(OneNet_DevLink())			//接入OneNET
		delay_ms(500);
	LED = LED_ON;						//入网成功
	
}

int main(void)
{
	unsigned char *dataPtr = NULL;
	unsigned short timeCount = 300;	//发送间隔变量
  int i = 0;
	u16 adcx,adco;
	Hardware_Init();				//初始化外围硬件
	Net_Init();						//网络初始化

//	Adc_Init();						//ADC初始化   //PA0、PA1用于指示灯

	OneNet_SendData();  //先发送初始数据到服务器上，进行初始化
	while(1)
	{	
		delay_ms(1);
		timeCount ++;
		i++;
		deal_key_val();
	  
		switch(timeCount)
		{
			case 1:DHT11_Read_Data(&temperature,&humidity);	break;//读取温湿度值	
			case 101:temperature_18B20 = DS18B20_Get_Temp();break;
			case 201:OneNet_SendData();	//发送数据										
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





