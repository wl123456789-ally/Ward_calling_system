#ifndef __LED_H
#define __LED_H	 
#include "sys.h"

#define LED PCout(13)	// LED接口	

#define LED_1 PBout(5)	// LED接口	
#define LED_2 PAout(0)	// LED接口	

#define LED_OFF		1
#define LED_ON		0

void LED_Init(void);//初始化
			    
#endif
