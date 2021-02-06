#include "dht11.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//INT8U DHT11_temperature,DHT11_humidity;

//��λDHT11
void DHT11_Rst(void)	   
{                 
	DHT11_IO_OUT(); 	//SET OUTPUT
    DHT11_IO_L(); 	//����DQ
    delay_ms(20);    	//��������18ms
    DHT11_IO_H(); 	//DQ=1 
	delay_us(30);     	//��������20~40us
}
//�ȴ�DHT11�Ļ�Ӧ
//����1:δ��⵽DHT11�Ĵ���
//����0:����
u8 DHT11_Check(void) 	   
{   
	u8 retry=0;
	DHT11_IO_IN();//SET INPUT	 
    while (DHT11_DQ_IN&&retry<100)//DHT11������40~80us
	{
		retry++;
		delay_us(1);
	};	 
	if(retry>=100)return 1;
	else retry=0;
    while (!DHT11_DQ_IN&&retry<100)//DHT11���ͺ���ٴ�����40~80us
	{
		retry++;
		delay_us(1);
	};
	if(retry>=100)return 1;	    
	return 0;
}
//��DHT11��ȡһ��λ
//����ֵ��1/0
u8 DHT11_Read_Bit(void) 			 
{
 	u8 retry=0;
	while(DHT11_DQ_IN&&retry<100)//�ȴ���Ϊ�͵�ƽ
	{
		retry++;
		delay_us(1);
	}
	retry=0;
	while(!DHT11_DQ_IN&&retry<100)//�ȴ���ߵ�ƽ
	{
		retry++;
		delay_us(1);
	}
	delay_us(40);//�ȴ�40us
	if(DHT11_DQ_IN)return 1;
	else return 0;		   
}
//��DHT11��ȡһ���ֽ�
//����ֵ������������
u8 DHT11_Read_Byte(void)    
{        
    u8 i,dat;
    dat=0;
	for (i=0;i<8;i++) 
	{
   		dat<<=1; 
	    dat|=DHT11_Read_Bit();
    }						    
    return dat;
}
//��DHT11��ȡһ������
//temp:�¶�ֵ(��Χ:0~50��)
//humi:ʪ��ֵ(��Χ:20%~90%)
//����ֵ��0,����;1,��ȡʧ��
u8 DHT11_Read_Data(float *temp,float *humi)    
{        
 	u8 buf[5];
	u8 i;
	DHT11_Rst();
	if(DHT11_Check()==0)
	{
		for(i=0;i<5;i++)//��ȡ40λ����
		{
			buf[i]=DHT11_Read_Byte();
		}
		if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])
		{
			*humi=buf[0]+buf[1]*0.1;
			*temp=buf[2]+buf[3]*0.1;
		}
	}else return 1;
	return 0;	    
}

void DHT11_OUTMode(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	
  /* ʹ�ܸ��˿�ʱ�� */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE); 
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 ;				            
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);		
}

void DHT11_INMode(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;    

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE); 

	/*  */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_14;		    
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;                                               
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void  DHT11_LowLevel_DeInit (void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	
  /* ʹ�ܸ��˿�ʱ�� */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE); 
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 ;				            
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);		
	
}
//��ʼ��DHT11��IO�� DQ ͬʱ���DHT11�Ĵ���
//����1:������
//����0:����    	 
u8 DHT11_Init(void)
{
//	delay_init(72);	   	 	//��ʱ��ʼ�� 

	//	RCC->APB2ENR|=1<<8;    //ʹ��PORTG��ʱ�� 
//	GPIOG->CRH&=0XFFFF0FFF;//PORTG.11 �������
//	GPIOG->CRH|=0X00003000;
	DHT11_LowLevel_DeInit();
//	GPIOG->ODR|=1<<11;      //���1				    
	DHT11_IO_H();
	DHT11_Rst();
	return DHT11_Check();
}


	 


