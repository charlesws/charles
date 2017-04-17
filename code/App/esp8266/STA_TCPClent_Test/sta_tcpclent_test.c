#include "sta_tcpclent_test.h"
#include "SysTick.h"
#include "usart.h"
#include "esp8266_drive.h"
#include "ds18b20.h"

volatile u8 TcpClosedFlag = 0;
extern u8 Flag_wifi = 0;
extern Wifi_Account WIFI1;
extern char PC_PORT[5];	
extern char PC_IP[16];
extern ENUM_ID_NO_TypeDef ucId;


void ESP8266_STA_TCPClient_Test(void)
{
	printf ( "\r\n正在配置ESP8266请耐心等待...\r\n" );

	ESP8266_CH_PD_Pin_SetH;

	ESP8266_AT_Test();
	//选择模式 3
	while(!ESP8266_Net_Mode_Choose(STA_AP));
	//开启透传模式
	//ESP8266_UnvarnishSend();
	//创建服务端
	//端口是8080
	while(!ESP8266_Enable_MultipleId ( ENABLE ));
	if(ESP8266_creat_Server())
	{
	printf("\r\n 服务端创建成功 请client端TCP方式连接，ip = 192.168.4.1  port = 8080  \r\n");
	}
	//收集
	Receiver_Wifi_Account();

	//接收账号和密码
	printf("账号和密码收集成功\r\n");


	//连接外部wifi
	//一定要判断连接的准确性
	while(1)
		{
		if(ESP8266_JoinAP(WIFI1.SSID, WIFI1.PWD))
			{
			if(ESP8266_Send_AT_Cmd("AT+CIFSR", "ERROR", NULL, 500))
				{
				delay_ms(500);
				}
			else
				{
				printf("WIFI 连接成功 \n");
				break;
				}
			}
		}

	//设置超时
	while(!ESP8266_Send_AT_Cmd ( "AT+CIPSTO=2880", "OK", 0, 500 ));

	Server_8266_AP_send("请更换热点， 保持与ES8266在同一路由上, 创造客户端，\n");
	
	ESP8266_Send_AT_Cmd("AT+CIPSTATUS", "OK" , NULL, 500);
	while(1)
		{
		 if(CONNECT_ID())
		 	{
		 	break;
		 	}
		 else
		 	{
		 	ESP8266_Fram_Record_Struct .InfBit .FramLength = 0;
		 	delay_ms(1000);
			ESP8266_Fram_Record_Struct.Data_RX_BUF[ESP8266_Fram_Record_Struct.InfBit.FramLength ] = '\0';
		 	}
		}

	Server_8266_AP_send("发送成功，可以互相通信\n\n");
	
	while(1)
		{
			if(Flag_wifi)
				{
			ESP8266_Fram_Record_Struct.Data_RX_BUF[ESP8266_Fram_Record_Struct.InfBit.FramLength ] = '\0';
			 Server_8266_AP_send(ESP8266_Fram_Record_Struct.Data_RX_BUF);
			 Flag_wifi = 0;
				}
			delay_ms(1000);
		}
}



