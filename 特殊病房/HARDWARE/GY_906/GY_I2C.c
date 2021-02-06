#include "GY_I2C.h"
#include "delay.h"
//#include "bsp_SysTick.h"


void I2C_GPIO_Config(void)
{
	/* 使能与 I2C 有关的时钟 */
	RCC_APB1PeriphClockCmd ( RCC_APB1Periph_I2C1, ENABLE );
	RCC_APB2PeriphClockCmd ( RCC_APB2Periph_GPIOB, ENABLE );
	
  GPIO_InitTypeDef  GPIO_InitStructure;  
  /* I2C_SCL、I2C_SDA*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;	       // 开漏输出
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;	       // 开漏输出
  GPIO_Init(GPIOB, &GPIO_InitStructure);

	I2C_InitTypeDef  I2C_InitStructure; 
	
	I2C_DeInit(NTC_I2Cx);
  /* I2C 配置 */
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	
	/* 高电平数据稳定，低电平数据变化 SCL 时钟线的占空比 */
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	
	//主设备地址
  I2C_InitStructure.I2C_OwnAddress1 =0X0A; 		
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable ;
	 
	/* I2C的寻址模式 */
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	
	/* 通信速率 */
  I2C_InitStructure.I2C_ClockSpeed = I2C_Speed;
  
	/* I2C 初始化 */
  I2C_Init(NTC_I2Cx, &I2C_InitStructure);
	
	/*允许应答模式*/
	I2C_AcknowledgeConfig(NTC_I2Cx, ENABLE);
  
	/* 使能 I2C */
  I2C_Cmd(NTC_I2Cx, ENABLE);   	
 
}

//对模块写一个字节
void I2C_ByWrite(uint8_t ModuleAdd, uint8_t RegAdd, uint8_t RegDate)
{
	//I2C_1启动信号
	I2C_GenerateSTART(NTC_I2Cx,ENABLE);
	while(!I2C_CheckEvent(NTC_I2Cx,I2C_EVENT_MASTER_MODE_SELECT));//检测	EV5事件
	
	//发送模块地址+写信号
	I2C_Send7bitAddress(NTC_I2Cx,ModuleAdd,I2C_Direction_Transmitter);
	while(!I2C_CheckEvent(NTC_I2Cx,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)); //检测EV6

	
	//发送模块中寄存器地址
	I2C_SendData(NTC_I2Cx, RegAdd);
  while(!I2C_CheckEvent(NTC_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) //检测EV8
	
//	//发送数据
//	I2C_SendData(NTC_I2Cx, RegDate);
//	//NTC_I2Cx->DR = RegDate;
//	while(I2C_GetFlagStatus(NTC_I2Cx,I2C_FLAG_AF));	//是否有答应
//  while(!I2C_CheckEvent(NTC_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) //检测EV8
	
	delay_ms(10);
	//发送数据
	I2C_SendData(NTC_I2Cx, RegDate);
  while(!I2C_CheckEvent(NTC_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) //检测EV8
	
	//停止信号
	I2C_GenerateSTOP(NTC_I2Cx,ENABLE);
	while(I2C_GetFlagStatus(NTC_I2Cx,I2C_FLAG_ARLO));	//主机是否失去对总线的控制
}

//对模块读取数值
u16 I2C_ByRead(uint8_t ModuleAdd, uint8_t RegAdd)
{
	u8	H_data =0,L_data = 0;
	u16 temp = 0;
	
	while(I2C_GetFlagStatus(NTC_I2Cx,I2C_FLAG_BUSY));	//判忙
	
	//I2C_1启动信号
	I2C_GenerateSTART(NTC_I2Cx,ENABLE);
	while(!I2C_CheckEvent(NTC_I2Cx,I2C_EVENT_MASTER_MODE_SELECT));//检测	EV5事件
	
	//发送模块地址+写信号
	I2C_Send7bitAddress(NTC_I2Cx,ModuleAdd,I2C_Direction_Transmitter);
	while(!I2C_CheckEvent(NTC_I2Cx,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)); //检测EV6

	//发送模块中寄存器地址

	I2C_SendData(NTC_I2Cx, RegAdd);
  while(!I2C_CheckEvent(NTC_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) //检测EV8
	
	//I2C_Cmd(NTC_I2Cx,ENABLE);
	//I2C_1启动信号
	I2C_GenerateSTART(NTC_I2Cx,ENABLE);
	while(!I2C_CheckEvent(NTC_I2Cx,I2C_EVENT_MASTER_MODE_SELECT));//检测	EV5事件
	
	//发送模块地址+读信号
	I2C_Send7bitAddress(NTC_I2Cx,ModuleAdd,I2C_Direction_Receiver);
	while(!I2C_CheckEvent(NTC_I2Cx,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)); //检测EV6
	
	//接受寄存器非空
	while(!(I2C_CheckEvent(NTC_I2Cx,I2C_EVENT_MASTER_BYTE_RECEIVED)));
	H_data=I2C_ReceiveData(NTC_I2Cx);//读出寄存器数据
	
	//禁止自动应答
	I2C_AcknowledgeConfig(NTC_I2Cx,DISABLE);
	//停止信号
	I2C_GenerateSTOP(NTC_I2Cx,ENABLE);
	
	//接受寄存器非空
	while(!(I2C_CheckEvent(NTC_I2Cx,I2C_EVENT_MASTER_BYTE_RECEIVED)));
	L_data=I2C_ReceiveData(NTC_I2Cx);//读出寄存器数据
	//启动自动应答
	I2C_AcknowledgeConfig(NTC_I2Cx,ENABLE);
	
	temp = H_data;
	temp <<= 8;
	temp |= L_data;
	
	return temp;
}

