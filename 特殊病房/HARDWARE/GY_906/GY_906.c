#include "GY_906.h"

//测红外探头温度
float GY_906NTC(void)
{
	u16 temp;
	u8 tempL,tempH;
	temp = I2C_ByRead(MLX90614_I2CADDR,MLX90614_TOBJ1);
	
	tempH = (u8)temp;	//模块输出，低位在前。I2c读取是高位在前所以要换位
	tempL = (temp>>8);

	return ((tempH * 256 + tempL) * 0.02 -273.15);
}

//测环境温度
float GY_906ambientNTC(void)
{
	u16 temp;
	u8 tempL,tempH;
	temp = I2C_ByRead(MLX90614_I2CADDR,MLX90614_TA);
	
	tempH = (u8)temp;	//模块输出，低位在前。I2c读取是高位在前所以要换位
	tempL = (temp>>8);

	return ((tempH * 256 + tempL) * 0.02 -273.15);
}

