#ifndef __LED_H
#define __LED_H	 
#include "sys.h"

#define LED PCout(13)	// LED�ӿ�	

#define LED_1 PAout(1)	// LED�ӿ�	
#define LED_2 PAout(0)	// LED�ӿ�	

#define LED_OFF		1
#define LED_ON		0

void LED_Init(void);//��ʼ��
			    
#endif
