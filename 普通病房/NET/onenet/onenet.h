#ifndef _ONENET_H_
#define _ONENET_H_
#include "stm32f10x.h"





_Bool OneNet_DevLink(void);

void OneNet_SendData(void);
void OneNet_SendCmd(void);
void OneNet_RevPro(unsigned char *cmd);
void  temp_rede(u8 temp ,u8 humi,u8 outadc);

#endif
