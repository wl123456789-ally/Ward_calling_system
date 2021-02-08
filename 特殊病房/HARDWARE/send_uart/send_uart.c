/**************************************
程序功能：
1、发送数据到TJC4024T032屏幕显示波形数据，和四个变量的数据
2、接收TJC4024T032下发的两个按键命令到key[2]

***********************************************/


#include "stm32f10x.h"
#include "send_uart.h"
#include "sys.h" 
#include "delay.h"
#include "usart.h"
#include "string.h"
#include "bsp_usart.h"
 
 
//按理说可以不用在次引用
extern u8  USART_RX_BUF3[200]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA3;         		//接收状态标记	

extern float temperature; 
extern float environment_temp ;
extern float human_temp;
extern float humidity;


char key[2];   //用于装按键标志位0：未按下，1：按下


char hex_END[]={0xff,0xff,0xff};   //发送到串口屏的结束串

//

void USARTx_Send(USART_TypeDef* USARTx, u8 *Data, u8 Len)  
{  
    u8 i = 0;  
    for(i = 0; i < Len; i++)  
    {  
        while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) != SET);     
        USART_SendData(USARTx, Data[i]);                                                     
    }  
}

/*******************************
函数功能：
发送三个波形到屏幕，不用则参数给0即可
ch0：粉色
ch1：黄色
ch2：蓝色    
注：尽量只开一个通道，多个通道太耗费时间了
*******************************/
void send_wave(int ch0,int ch1,int ch2)
{
  char str1[50];
	int i = 0,temp;
	static int last_val = 0;
	for(i = 0;i<10;i++)         //本次数据与上次数据插入9个点的数据，拉开x轴的间距
	{
		temp = (ch0 - last_val)/10.0;
		
		sprintf(str1,"add 1,0,%d",temp*i + last_val);
		USARTx_Send(USART3,str1,strlen(str1));
		USARTx_Send(USART3,hex_END,3);   
		

	}
	
	last_val = ch0;
//	  USARTx_Send(USART3,str1,strlen(str1));
//  USARTx_Send(USART3,hex_END,3);   //多发几次，使x间隔数据变大

//	  USARTx_Send(USART3,str1,strlen(str1));
//  USARTx_Send(USART3,hex_END,3);   //多发几次，使x间隔数据变大

//	  USARTx_Send(USART3,str1,strlen(str1));
//  USARTx_Send(USART3,hex_END,3);   //多发几次，使x间隔数据变大

//  USARTx_Send(USART3,hex_END,3);
//  sprintf(str1,"add 1,1,%d",ch1);
//  USARTx_Send(USART2,str1,strlen(str1));
//  USARTx_Send(USART2,hex_END,3);
//  
//  sprintf(str1,"add 1,2,%d",ch2);
//  USARTx_Send(USART3,str1,strlen(str1));
//  USARTx_Send(USART3,hex_END,3);
}



/*******************************
函数功能：
发送四个不同数据到屏幕显示

*******************************/

void send_data(void)
{
  char str1[50]; 
  static int state = 0;
  switch(state)              //处理一个数据太长，分成四次处理
  {
    case 0:
      sprintf(str1,"t4.txt=\"%.1lf\"",environment_temp);
      USARTx_Send(USART3,str1,strlen(str1));
      USARTx_Send(USART3,hex_END,3);
    break;
    case 1:
      sprintf(str1,"t5.txt=\"%.0lf\"",humidity);
      USARTx_Send(USART3,str1,strlen(str1));
      USARTx_Send(USART3,hex_END,3);
    break;
		case 2:
      sprintf(str1,"t6.txt=\"%.1lf\"",human_temp);
      USARTx_Send(USART3,str1,strlen(str1));
      USARTx_Send(USART3,hex_END,3);
    break;
  }
  state++;
  state %= 3;
 
}
//void send_data(float F,float PR,float QRS,float ST)
//{
//  char str1[60]; 
//  sprintf(str1,"t0.txt=\"F:%.0fBPM PR:%.1fms QRS:%.1fms ST:%.1fms\"",F,PR,QRS,ST);
//  USARTx_Send(USART2,str1,strlen(str1));
//  USARTx_Send(USART2,hex_END,3);

// 
//}
/********************
函数功能：查找str1中是否有str2的字符串，放回第一个相同字符的地址


**********************/
int find_str(char* str1,char* str2)
{
    int i,j,flag=-1;
    for(i=0,j=0;str1[i]!=NULL;i++)
    {
        while(str1[i]==str2[j]&&str1[i]&&str2[j])
        {
            i++;
            j++;
            if(str2[j]==NULL)
            {
                flag=i-j;
                return flag;
            }
        }
        j=0;
    }
    return flag;
}
/********************
函数功能：解析串口接收到数据


注：上位机发送的是KEYx*p
其中x为按键号，对应存入key[x]的数组，“*p”为发送的数据的针尾
占时只写了两个

**********************/
void get_key(void)
{
   int i = 0;
   int flag = 0;
   if(USART_RX_STA3&0x8000)
   {
     flag = find_str(USART_RX_BUF3,"KEY");
     if(flag != -1)
     {
        key[USART_RX_BUF3[flag+3] - 0x31] = 1;
     }
     USART_RX_STA3 = 0;
     memset(USART_RX_BUF3,0,strlen(USART_RX_BUF3));
   }
}






