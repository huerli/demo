#include "lcd.h"
#include "esp8266.h"
unsigned char ESP8266_Buf[128];                         //定义一个数组作为esp8266的数据缓冲区
unsigned short esp8266_cnt = 0, esp8266_cntPre = 0;     //定义两个计数值：此次和上一次
unsigned char a_esp8266_buf;



/**
  * @brief  清空缓存
  * @param  无
  * @retval 无
  */
void ESP8266_Clear(void)
{
    memset(ESP8266_Buf, 0, sizeof(ESP8266_Buf));    //将数组中的元素全部初始化为0，
}

/**
  * @brief  等待接收完成
  * @param  无
  * @retval OK：表示接收完成；OUTTIME：表示接收超时完成
  *         进行循环调用，检测接收是否完成
  */
_Bool ESP8266_WaitRecive(void)
{
    if(esp8266_cnt == 0) 							//如果当前接收计数为0 则说明没有处于接收数据中，所以直接跳出，结束函数
        return OUTTIME;

    if(esp8266_cnt == esp8266_cntPre)				//如果上一次的值和这次相同，则说明接收完毕
    {
        esp8266_cnt = 0;							//清0接收计数

        return OK;								    //返回接收完成标志
    }
    else                                            //如果不相同，则将此次赋值给上一次，并返回接收未完成标志
    {
        esp8266_cntPre = esp8266_cnt;

        return OUTTIME;
    }
}

/**
  * @brief 发送命令
  * @param cmd：表示命令；res：需要检查的返回指令
  * @retval 0：表示成功；1：表示失败
  */
_Bool ESP8266_SendCmd(char *cmd, char *res)
{

    unsigned char timeOut = 200;
    HAL_UART_Transmit(&huart2, (unsigned char *)cmd, strlen((const char *)cmd),1000);
    while(timeOut--)
    {
        if(ESP8266_WaitRecive() == OK)//如果收到数据
        {
            printf("%s",ESP8266_Buf);
            if(strstr((const char *)ESP8266_Buf, res) != NULL)//如果检索到关键词,清空缓存
            {
                ESP8266_Clear();
                return 0;
            }
        }
        HAL_Delay(10);
    }
    return 1;
}

/**
  * @brief 数据发送
  * @param data：待发送的数据；len：待发送的数据长度
  * @retval 无
  */
void ESP8266_SendData(unsigned char *data, unsigned short len)
{

    char cmdBuf[32];

    ESP8266_Clear();								//清空接收缓存
    sprintf(cmdBuf, "AT+CIPSEND=%d\r\n", len);		//发送命令,sprintf()函数用于将格式化的数据写入字符串
    if(!ESP8266_SendCmd(cmdBuf, ">"))				//收到‘>’时可以发送数据
    {
        HAL_UART_Transmit(&huart2, data, len,0xffff);		//发送设备连接请求数据
    }
}

/**
  * @brief 获取平台返回的数据
  * @param 等待的时间
  * @retval 平台返回的数据，不同网络设备返回的格式不同，需要进行调试，如：ESP8266的返回格式为："+IPD,x:yyy",x表示数据长度，yyy表示数据内容
  */
unsigned char *ESP8266_GetIPD(unsigned short timeOut)
{

    char *ptrIPD = NULL;

    do
    {
        if(ESP8266_WaitRecive() == OK)								//如果接收完成
        {
            ptrIPD = strstr((char *)ESP8266_Buf, "IPD,");				//搜索“IPD”头
            if(ptrIPD == NULL)											//如果没找到，可能是IPD头的延迟，还是需要等待一会，但不会超过设定的时间
            {
//                printf(USART_DEBUG, "\"IPD\" not found\r\n");
            }
            else
            {
                ptrIPD = strchr(ptrIPD, ':');							//找到':'
                if(ptrIPD != NULL)
                {
                    ptrIPD++;
                    return (unsigned char *)(ptrIPD);
                }
                else
                    return NULL;

            }
        }

        HAL_Delay(5);													//延时等待
    } while(timeOut--);

    return NULL;		//超时还未找到，返回空指针
}

/**
  * @brief esp8266初始化
  * @param 无
  * @retval 无
  */
void ESP8266_Init(void)
{
    ESP8266_Clear();

    printf("1. test AT start\r\n");            //AT：测试AT启动
    LCD_Fill(30,170,200,186,WHITE);
    LCD_ShowString(30,170,200,16,16,"1.test AT start");

    while(ESP8266_SendCmd("AT\r\n", "OK"))
    {

        HAL_Delay(500);
    }
    LCD_Fill(30,170,200,186,WHITE);
    LCD_ShowString(30,170,200,16,16,"OK");
    HAL_Delay(500);

    printf("2.RESTORE\r\n");
    LCD_Fill(30,170,200,186,WHITE);
    LCD_ShowString(30,170,200,16,16,"2.RESTORE");

    while(ESP8266_SendCmd("AT+RESTORE\r\n", "OK"))
    {
        HAL_Delay(500);
    }

    LCD_Fill(30,170,200,186,WHITE);
    LCD_ShowString(30,170,200,16,16,"OK");
    HAL_Delay(500);

    printf("3. config WiFi mode（CWMODE）\r\n");        //查询/设置 Wi-Fi 模式：设置WiFi模式为Station模式
    LCD_Fill(30,170,200,186,WHITE);
    LCD_ShowString(30,170,200,16,16,"3. config WiFi mode（CWMODE）");
    while(ESP8266_SendCmd("AT+CWMODE=1\r\n", "OK"))
    {
        HAL_Delay(500);
    }
    LCD_Fill(30,170,200,186,WHITE);
    LCD_ShowString(30,170,200,16,16,"OK");
    HAL_Delay(500);

    printf("4. connect WiFi CWJAP）\r\n");
    LCD_Fill(30,170,200,186,WHITE);
    LCD_ShowString(30,170,200,16,16,"4. connect WiFi CWJAP）");
    while(ESP8266_SendCmd(ESP8266_WIFI_INFO, "GOT IP"))
    {

        HAL_Delay(500);
    }

    LCD_Fill(30,170,200,186,WHITE);
    LCD_ShowString(30,170,200,16,16,"OK");
    HAL_Delay(500);
    printf("5. set TCP connect\r\n");
    LCD_Fill(30,170,200,186,WHITE);
    LCD_ShowString(30,170,200,16,16,"5. set TCP connect");
    while(ESP8266_SendCmd(ESP8266_TCP_INFO, "OK"))
    {
        HAL_Delay(500);
    }
    LCD_Fill(30,170,200,186,WHITE);
    LCD_ShowString(30,170,200,16,16,"OK");
    HAL_Delay(500);

    printf("6. ESP8266 Init OK\r\n");
    LCD_Fill(30,170,200,186,WHITE);
    LCD_ShowString(30,170,200,16,16,"6. ESP8266 Init OK");
}
