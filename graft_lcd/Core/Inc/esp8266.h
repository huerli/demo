#ifndef _ESP8266_H_
#define _ESP8266_H_

#include "main.h"
#include "usart.h"
#include<string.h>
#include<stdio.h>
#include<stdbool.h>

#define     ESP8266_WIFI_INFO		"AT+CWJAP=\"Cduestc203\",\"123456789\"\r\n"          //连接上自己的wifi热点：WiFi名和密码
#define     ESP8266_TCP_INFO		"AT+CIPSTART=\"TCP\",\"192.168.1.129\",8080\r\n"
//#define     ESP8266_ONENET_INFO		"AT+CIPSTART=\"TCP\",\"183.230.40.39\",6002\r\n" //连接上OneNet的MQTT
#define     ESP8266_yuanziyun_INFO	"AT+ATKCLDSTA=\"14351237793832593198\",\"12345678\"\r\n"

#define     OK		        0	    //接收完成标志
#define     OUTTIME	        1	    //接收未完成标志

extern unsigned char ESP8266_Buf[128];                         //定义一个数组作为esp8266的数据缓冲区
extern unsigned short esp8266_cnt , esp8266_cntPre;     //定义两个计数值：此次和上一次
extern unsigned char a_esp8266_buf;

void ESP8266_Clear(void);           //清空缓存

void ESP8266_Init(void);            //esp8266初始化

_Bool ESP8266_SendCmd(char *cmd, char *res);//发送数据
_Bool ESP8266_WaitRecive(void);
unsigned char *ESP8266_GetIPD(unsigned short timeOut);

#endif


