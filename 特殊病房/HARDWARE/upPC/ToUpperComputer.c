#include "ToUpperComputer.h"
#include "string.h"

#include "usart.h"

/*And relevant code instead of the following two sentences 
	if user want to send data via other channel.*/
//#define  Transimit_to_upper_computer   do{\
//	uart_putchar(USART_0, transbuf[lp]) /*transmit data*/;\
//}while(0)
//#define  Transimit_to_upper_computer   do{\
//	nrf_send_packet(transbuf) /*transmit data*/;\
//}while(0)
/*Transimit user's data to computer.*/
/*Parameter "ch" is function word of upper computer,can be set 0xf1~0xfa*/
/*Parameter "dat" is the data to be viewed.*/
//void TransmitUserCurve_s32(uint8_t ch,float dat)
//{
//	
// uint8_t lp, transbuf[10];
//	uint16_t sum = 0;
//	transbuf[0] = 0xaa;
//	transbuf[1] = 0xaa;
//	transbuf[2] = ch;
//	transbuf[3] = 4;
//	transbuf[4] = dataBYTE3(dat);
//	transbuf[5] = dataBYTE2(dat);
//	transbuf[6] = dataBYTE1(dat);
//	transbuf[7] = dataBYTE0(dat);
//	for(lp=0;lp<8;lp++)
//	{
//		sum += transbuf[lp];
//	}
//	transbuf[8] = dataBYTE0(sum);
//  for(lp = 0;lp <9;lp++)
//  {
//    while((USART1->SR&0X40)==0);
//    USART1->DR = transbuf[lp];
//  }
//	//nrf_send_packet(transbuf);     
//}
//上位机记着设置为int32类型的数据，否则接受到是乱码

//发送模板：	TransmitUserCurve_s32(0xf1,(int)(temp*100)); 
void TransmitUserCurve_s32(uint8_t ch,int32_t dat)
{
	
 uint8_t lp, transbuf[10];
	uint16_t sum = 0;
	transbuf[0] = 0xaa;
	transbuf[1] = 0xaa;
	transbuf[2] = ch;
	transbuf[3] = 4;
	transbuf[4] = dataBYTE3(dat);
	transbuf[5] = dataBYTE2(dat);
	transbuf[6] = dataBYTE1(dat);
	transbuf[7] = dataBYTE0(dat);
	for(lp=0;lp<8;lp++)
	{
		sum += transbuf[lp];
	}
	transbuf[8] = dataBYTE0(sum);
  for(lp = 0;lp <9;lp++)
  {
    while(USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET);     
      USART_SendData(USART1, transbuf[lp]);    
  }


}


