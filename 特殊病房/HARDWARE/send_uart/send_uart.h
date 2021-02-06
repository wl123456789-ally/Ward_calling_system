#ifndef __SEND_UART_H
#define __SEND_UART_H	 
#include "sys.h"


extern char key[2];

void TJC4_init(int baud);
void send_wave(int ch0,int ch1,int ch2);
void send_data(void);
void get_key(void);



#endif


