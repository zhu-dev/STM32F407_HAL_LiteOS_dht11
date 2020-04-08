#ifndef __DHT11_H
#define __DHT11_H 
 
#include "sys.h"  

//IO��������
#define DHT11_IO_IN()  {GPIOG->MODER&=~(3<<(9*2));GPIOG->MODER|=0<<9*2;}	//PG9����ģʽ
#define DHT11_IO_OUT() {GPIOG->MODER&=~(3<<(9*2));GPIOG->MODER|=1<<9*2;} 	//PG9���ģʽ 
////IO��������											   
#define	DHT11_DQ_OUT PGout(9) //���ݶ˿�	PG9 
#define	DHT11_DQ_IN  PGin(9)  //���ݶ˿�	PG9 

uint8_t DHT11_Init(void);//��ʼ��DHT11
uint8_t DHT11_Read_Data(uint8_t *temp,uint8_t *humi);//��ȡ��ʪ��
uint8_t DHT11_Read_Byte(void);//����һ���ֽ�
uint8_t DHT11_Read_Bit(void);//����һ��λ
uint8_t DHT11_Check(void);//����Ƿ����DHT11
void DHT11_Rst(void);//��λDHT11    
#endif

















