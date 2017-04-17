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
	printf ( "\r\n��������ESP8266�����ĵȴ�...\r\n" );

	ESP8266_CH_PD_Pin_SetH;

	ESP8266_AT_Test();
	//ѡ��ģʽ 3
	while(!ESP8266_Net_Mode_Choose(STA_AP));
	//����͸��ģʽ
	//ESP8266_UnvarnishSend();
	//���������
	//�˿���8080
	while(!ESP8266_Enable_MultipleId ( ENABLE ));
	if(ESP8266_creat_Server())
	{
	printf("\r\n ����˴����ɹ� ��client��TCP��ʽ���ӣ�ip = 192.168.4.1  port = 8080  \r\n");
	}
	//�ռ�
	Receiver_Wifi_Account();

	//�����˺ź�����
	printf("�˺ź������ռ��ɹ�\r\n");


	//�����ⲿwifi
	//һ��Ҫ�ж����ӵ�׼ȷ��
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
				printf("WIFI ���ӳɹ� \n");
				break;
				}
			}
		}

	//���ó�ʱ
	while(!ESP8266_Send_AT_Cmd ( "AT+CIPSTO=2880", "OK", 0, 500 ));

	Server_8266_AP_send("������ȵ㣬 ������ES8266��ͬһ·����, ����ͻ��ˣ�\n");
	
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

	Server_8266_AP_send("���ͳɹ������Ի���ͨ��\n\n");
	
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



