#include "GY_906.h"

//�����̽ͷ�¶�
float GY_906NTC(void)
{
	u16 temp;
	u8 tempL,tempH;
	temp = I2C_ByRead(MLX90614_I2CADDR,MLX90614_TOBJ1);
	
	tempH = (u8)temp;	//ģ���������λ��ǰ��I2c��ȡ�Ǹ�λ��ǰ����Ҫ��λ
	tempL = (temp>>8);

	return ((tempH * 256 + tempL) * 0.02 -273.15);
}

//�⻷���¶�
float GY_906ambientNTC(void)
{
	u16 temp;
	u8 tempL,tempH;
	temp = I2C_ByRead(MLX90614_I2CADDR,MLX90614_TA);
	
	tempH = (u8)temp;	//ģ���������λ��ǰ��I2c��ȡ�Ǹ�λ��ǰ����Ҫ��λ
	tempL = (temp>>8);

	return ((tempH * 256 + tempL) * 0.02 -273.15);
}

