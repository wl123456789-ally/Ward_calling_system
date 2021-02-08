/**************************************
�����ܣ�
1���������ݵ�TJC4024T032��Ļ��ʾ�������ݣ����ĸ�����������
2������TJC4024T032�·��������������key[2]

***********************************************/


#include "stm32f10x.h"
#include "send_uart.h"
#include "sys.h" 
#include "delay.h"
#include "usart.h"
#include "string.h"
#include "bsp_usart.h"
 
 
//����˵���Բ����ڴ�����
extern u8  USART_RX_BUF3[200]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA3;         		//����״̬���	

extern float temperature; 
extern float environment_temp ;
extern float human_temp;
extern float humidity;


char key[2];   //����װ������־λ0��δ���£�1������


char hex_END[]={0xff,0xff,0xff};   //���͵��������Ľ�����

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
�������ܣ�
�����������ε���Ļ�������������0����
ch0����ɫ
ch1����ɫ
ch2����ɫ    
ע������ֻ��һ��ͨ�������ͨ��̫�ķ�ʱ����
*******************************/
void send_wave(int ch0,int ch1,int ch2)
{
  char str1[50];
	int i = 0,temp;
	static int last_val = 0;
	for(i = 0;i<10;i++)         //�����������ϴ����ݲ���9��������ݣ�����x��ļ��
	{
		temp = (ch0 - last_val)/10.0;
		
		sprintf(str1,"add 1,0,%d",temp*i + last_val);
		USARTx_Send(USART3,str1,strlen(str1));
		USARTx_Send(USART3,hex_END,3);   
		

	}
	
	last_val = ch0;
//	  USARTx_Send(USART3,str1,strlen(str1));
//  USARTx_Send(USART3,hex_END,3);   //�෢���Σ�ʹx������ݱ��

//	  USARTx_Send(USART3,str1,strlen(str1));
//  USARTx_Send(USART3,hex_END,3);   //�෢���Σ�ʹx������ݱ��

//	  USARTx_Send(USART3,str1,strlen(str1));
//  USARTx_Send(USART3,hex_END,3);   //�෢���Σ�ʹx������ݱ��

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
�������ܣ�
�����ĸ���ͬ���ݵ���Ļ��ʾ

*******************************/

void send_data(void)
{
  char str1[50]; 
  static int state = 0;
  switch(state)              //����һ������̫�����ֳ��Ĵδ���
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
�������ܣ�����str1���Ƿ���str2���ַ������Żص�һ����ͬ�ַ��ĵ�ַ


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
�������ܣ��������ڽ��յ�����


ע����λ�����͵���KEYx*p
����xΪ�����ţ���Ӧ����key[x]�����飬��*p��Ϊ���͵����ݵ���β
ռʱֻд������

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






