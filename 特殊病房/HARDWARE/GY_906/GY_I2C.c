#include "GY_I2C.h"
#include "delay.h"
//#include "bsp_SysTick.h"


void I2C_GPIO_Config(void)
{
	/* ʹ���� I2C �йص�ʱ�� */
	RCC_APB1PeriphClockCmd ( RCC_APB1Periph_I2C1, ENABLE );
	RCC_APB2PeriphClockCmd ( RCC_APB2Periph_GPIOB, ENABLE );
	
  GPIO_InitTypeDef  GPIO_InitStructure;  
  /* I2C_SCL��I2C_SDA*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;	       // ��©���
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;	       // ��©���
  GPIO_Init(GPIOB, &GPIO_InitStructure);

	I2C_InitTypeDef  I2C_InitStructure; 
	
	I2C_DeInit(NTC_I2Cx);
  /* I2C ���� */
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	
	/* �ߵ�ƽ�����ȶ����͵�ƽ���ݱ仯 SCL ʱ���ߵ�ռ�ձ� */
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	
	//���豸��ַ
  I2C_InitStructure.I2C_OwnAddress1 =0X0A; 		
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable ;
	 
	/* I2C��Ѱַģʽ */
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	
	/* ͨ������ */
  I2C_InitStructure.I2C_ClockSpeed = I2C_Speed;
  
	/* I2C ��ʼ�� */
  I2C_Init(NTC_I2Cx, &I2C_InitStructure);
	
	/*����Ӧ��ģʽ*/
	I2C_AcknowledgeConfig(NTC_I2Cx, ENABLE);
  
	/* ʹ�� I2C */
  I2C_Cmd(NTC_I2Cx, ENABLE);   	
 
}

//��ģ��дһ���ֽ�
void I2C_ByWrite(uint8_t ModuleAdd, uint8_t RegAdd, uint8_t RegDate)
{
	//I2C_1�����ź�
	I2C_GenerateSTART(NTC_I2Cx,ENABLE);
	while(!I2C_CheckEvent(NTC_I2Cx,I2C_EVENT_MASTER_MODE_SELECT));//���	EV5�¼�
	
	//����ģ���ַ+д�ź�
	I2C_Send7bitAddress(NTC_I2Cx,ModuleAdd,I2C_Direction_Transmitter);
	while(!I2C_CheckEvent(NTC_I2Cx,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)); //���EV6

	
	//����ģ���мĴ�����ַ
	I2C_SendData(NTC_I2Cx, RegAdd);
  while(!I2C_CheckEvent(NTC_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) //���EV8
	
//	//��������
//	I2C_SendData(NTC_I2Cx, RegDate);
//	//NTC_I2Cx->DR = RegDate;
//	while(I2C_GetFlagStatus(NTC_I2Cx,I2C_FLAG_AF));	//�Ƿ��д�Ӧ
//  while(!I2C_CheckEvent(NTC_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) //���EV8
	
	delay_ms(10);
	//��������
	I2C_SendData(NTC_I2Cx, RegDate);
  while(!I2C_CheckEvent(NTC_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) //���EV8
	
	//ֹͣ�ź�
	I2C_GenerateSTOP(NTC_I2Cx,ENABLE);
	while(I2C_GetFlagStatus(NTC_I2Cx,I2C_FLAG_ARLO));	//�����Ƿ�ʧȥ�����ߵĿ���
}

//��ģ���ȡ��ֵ
u16 I2C_ByRead(uint8_t ModuleAdd, uint8_t RegAdd)
{
	u8	H_data =0,L_data = 0;
	u16 temp = 0;
	
	while(I2C_GetFlagStatus(NTC_I2Cx,I2C_FLAG_BUSY));	//��æ
	
	//I2C_1�����ź�
	I2C_GenerateSTART(NTC_I2Cx,ENABLE);
	while(!I2C_CheckEvent(NTC_I2Cx,I2C_EVENT_MASTER_MODE_SELECT));//���	EV5�¼�
	
	//����ģ���ַ+д�ź�
	I2C_Send7bitAddress(NTC_I2Cx,ModuleAdd,I2C_Direction_Transmitter);
	while(!I2C_CheckEvent(NTC_I2Cx,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)); //���EV6

	//����ģ���мĴ�����ַ

	I2C_SendData(NTC_I2Cx, RegAdd);
  while(!I2C_CheckEvent(NTC_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) //���EV8
	
	//I2C_Cmd(NTC_I2Cx,ENABLE);
	//I2C_1�����ź�
	I2C_GenerateSTART(NTC_I2Cx,ENABLE);
	while(!I2C_CheckEvent(NTC_I2Cx,I2C_EVENT_MASTER_MODE_SELECT));//���	EV5�¼�
	
	//����ģ���ַ+���ź�
	I2C_Send7bitAddress(NTC_I2Cx,ModuleAdd,I2C_Direction_Receiver);
	while(!I2C_CheckEvent(NTC_I2Cx,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)); //���EV6
	
	//���ܼĴ����ǿ�
	while(!(I2C_CheckEvent(NTC_I2Cx,I2C_EVENT_MASTER_BYTE_RECEIVED)));
	H_data=I2C_ReceiveData(NTC_I2Cx);//�����Ĵ�������
	
	//��ֹ�Զ�Ӧ��
	I2C_AcknowledgeConfig(NTC_I2Cx,DISABLE);
	//ֹͣ�ź�
	I2C_GenerateSTOP(NTC_I2Cx,ENABLE);
	
	//���ܼĴ����ǿ�
	while(!(I2C_CheckEvent(NTC_I2Cx,I2C_EVENT_MASTER_BYTE_RECEIVED)));
	L_data=I2C_ReceiveData(NTC_I2Cx);//�����Ĵ�������
	//�����Զ�Ӧ��
	I2C_AcknowledgeConfig(NTC_I2Cx,ENABLE);
	
	temp = H_data;
	temp <<= 8;
	temp |= L_data;
	
	return temp;
}

