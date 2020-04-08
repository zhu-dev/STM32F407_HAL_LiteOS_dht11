#ifndef __DHT11_H
#define __DHT11_H 
 
#include "sys.h"  

//IO方向设置
#define DHT11_IO_IN()  {GPIOG->MODER&=~(3<<(9*2));GPIOG->MODER|=0<<9*2;}	//PG9输入模式
#define DHT11_IO_OUT() {GPIOG->MODER&=~(3<<(9*2));GPIOG->MODER|=1<<9*2;} 	//PG9输出模式 
////IO操作函数											   
#define	DHT11_DQ_OUT PGout(9) //数据端口	PG9 
#define	DHT11_DQ_IN  PGin(9)  //数据端口	PG9 

uint8_t DHT11_Init(void);//初始化DHT11
uint8_t DHT11_Read_Data(uint8_t *temp,uint8_t *humi);//读取温湿度
uint8_t DHT11_Read_Byte(void);//读出一个字节
uint8_t DHT11_Read_Bit(void);//读出一个位
uint8_t DHT11_Check(void);//检测是否存在DHT11
void DHT11_Rst(void);//复位DHT11    
#endif

















