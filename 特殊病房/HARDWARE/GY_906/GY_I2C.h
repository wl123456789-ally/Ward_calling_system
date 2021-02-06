#ifndef NTC_I2C_H
#define NTC_I2C_H
#include "stm32f10x.h"


#define I2C_Speed 40000	//40K
#define NTC_I2Cx	I2C1

void I2C_GPIO_Config(void);
void I2C_ByWrite(uint8_t ModuleAdd, uint8_t RegAdd, uint8_t RegDate);	
u16 I2C_ByRead(uint8_t ModuleAdd, uint8_t RegAdd);


#endif
