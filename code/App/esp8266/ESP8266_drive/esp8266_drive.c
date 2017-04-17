#include "esp8266_drive.h"
#include "SysTick.h"
#include <stdio.h>  
#include <string.h>  
#include <stdbool.h>
#include "sta_tcpclent_test.h"
#include "esp8266_public.h"

#include<string.h>
#include <stdlib.h>
//#include <stdlib.h>
extern u8 Flag_wifi;

struct STRUCT_USART_Fram ESP8266_Fram_Record_Struct = { 0 };
Wifi_Account WIFI1 = { 0 };
char PC_PORT[5];	
char PC_IP[16];
u8 Flag_ip = 0;
u8 Flag_port = 0;
ENUM_ID_NO_TypeDef ucId;





void USART3_Init(u32 bound)
{

  	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	

  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB10
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  	GPIO_Init(GPIOB, &GPIO_InitStructure);
   

  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PB11
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOB 11  
  
	//Usart3 NVIC ����
  	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//�����ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

   	//USART3 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  	USART_Init(USART3, &USART_InitStructure); //��ʼ������3

  	USART_ITConfig(USART3, USART_IT_RXNE|USART_IT_IDLE, ENABLE);//�������ڽ��ܺ����߿����ж�
  	
	USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ���3 	
}

void USART3_IRQHandler( void )
{	
	u8 ucCh;
	
	if(USART_GetITStatus( USART3, USART_IT_RXNE ) != RESET )
	{
		ucCh  = USART_ReceiveData( USART3 );
		
		if(ESP8266_Fram_Record_Struct .InfBit .FramLength < ( RX_BUF_MAX_LEN - 1 ) ) 
		{
			//Ԥ��1���ֽ�д������
			ESP8266_Fram_Record_Struct .Data_RX_BUF[ ESP8266_Fram_Record_Struct .InfBit .FramLength ++ ]  = ucCh;	
		}                      
	}
	 	 
	if( USART_GetITStatus( USART3, USART_IT_IDLE ) == SET )                                         //����֡�������
	{
    	ESP8266_Fram_Record_Struct .InfBit .FramFinishFlag = 1;
		
		ucCh = USART_ReceiveData( USART3 );                                                              //�������������жϱ�־λ(�ȶ�USART_SR��Ȼ���USART_DR)
	
		TcpClosedFlag = strstr ( ESP8266_Fram_Record_Struct .Data_RX_BUF, "CLOSED\r\n" ) ? 1 : 0;
		
  	}	

}



void USART2_Init(u32 bound)
{

  	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	

  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA2  TXD
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  	GPIO_Init(GPIOA, &GPIO_InitStructure);
   

  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA3  RXD
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA3  
  
	//Usart2 NVIC ����
  	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//�����ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

   	//USART2 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  	USART_Init(USART2, &USART_InitStructure); //��ʼ������2

  	USART_ITConfig(USART2, USART_IT_RXNE|USART_IT_IDLE, ENABLE);//�������ڽ��ܺ����߿����ж�
  	
	USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ���2 	
}

void USART2_IRQHandler( void )
{	
	u8 ucCh;
	
	if(USART_GetITStatus( USART2, USART_IT_RXNE ) != RESET )
	{
		ucCh  = USART_ReceiveData( USART2 );
		
		if(ESP8266_Fram_Record_Struct .InfBit .FramLength < ( RX_BUF_MAX_LEN - 1 ) ) 
		{
			//Ԥ��1���ֽ�д������
			ESP8266_Fram_Record_Struct .Data_RX_BUF[ ESP8266_Fram_Record_Struct .InfBit .FramLength ++ ]  = ucCh;	
		}                      
	}
	 	 
	if( USART_GetITStatus( USART2, USART_IT_IDLE ) == SET )                                         //����֡�������
	{
    	ESP8266_Fram_Record_Struct .InfBit .FramFinishFlag = 1;
		
		ucCh = USART_ReceiveData( USART2 );                                                              //�������������жϱ�־λ(�ȶ�USART_SR��Ȼ���USART_DR)
	
		TcpClosedFlag = strstr ( ESP8266_Fram_Record_Struct .Data_RX_BUF, "CLOSED\r\n" ) ? 1 : 0;
		
  	}	

	Flag_wifi = 1;
	Flag_port = 1;
	Flag_ip = 1;

}

void ESP8266_Init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(ESP8266_RST_Pin_Periph_Clock|ESP8266_CH_PD_Pin_Periph_Clock, ENABLE);

	GPIO_InitStructure.GPIO_Pin = ESP8266_RST_Pin;			   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //IO���ٶ�Ϊ50MHz
	GPIO_Init(ESP8266_RST_Pin_Port, &GPIO_InitStructure);

   	GPIO_InitStructure.GPIO_Pin = ESP8266_CH_PD_Pin;			   
	GPIO_Init(ESP8266_CH_PD_Pin_Port, &GPIO_InitStructure);
	
	USART2_Init(bound);
	
	ESP8266_RST_Pin_SetH;
	ESP8266_CH_PD_Pin_SetL;	
}

//�ж�����ID
bool CONNECT_ID(void)
{
if(strstr( ESP8266_Fram_Record_Struct .Data_RX_BUF, "0,CONNECT" ))
{
	ucId = Multiple_ID_0;
	return TRUE;

}
if(strstr( ESP8266_Fram_Record_Struct .Data_RX_BUF, "1,CONNECT" ))
{
	ucId = Multiple_ID_1;
	return TRUE;
}
if(strstr( ESP8266_Fram_Record_Struct .Data_RX_BUF, "2,CONNECT" ))
{
	ucId = Multiple_ID_2;

	return TRUE;
}
if(strstr( ESP8266_Fram_Record_Struct .Data_RX_BUF, "3,CONNECT" ))
{
	ucId = Multiple_ID_3;

	return TRUE;
}
if(strstr( ESP8266_Fram_Record_Struct .Data_RX_BUF, "4,CONNECT" ))
{
	ucId = Multiple_ID_4;
	return TRUE;
}
return FALSE;

}





//��ESP8266ģ�鷢��ATָ��
// cmd�������͵�ָ��
// ack1��ack2���ڴ�����Ӧ��ΪNULL������Ӧ������Ϊ���߼���ϵ
// time���ȴ���Ӧ��ʱ��
//����1�����ͳɹ� 0��ʧ��
bool ESP8266_Send_AT_Cmd(char *cmd,char *ack1,char *ack2,u32 time)
{
	ESP8266_Fram_Record_Struct .InfBit .FramLength = 0;	//���¿�ʼ�����µ����ݰ�
	ESP8266_USART("%s\r\n", cmd);
	if(ack1==0&&ack2==0)	 //����Ҫ��������
	{
		return true;
	}
	delay_ms(time);	  //��ʱtimeʱ��

	ESP8266_Fram_Record_Struct.Data_RX_BUF[ESP8266_Fram_Record_Struct.InfBit.FramLength ] = '\0';

	PC_USART("%s", ESP8266_Fram_Record_Struct .Data_RX_BUF);

	if(ack1!=0&&ack2!=0)
	{
		return ( ( bool ) strstr ( ESP8266_Fram_Record_Struct .Data_RX_BUF, ack1 ) || 
						 ( bool ) strstr ( ESP8266_Fram_Record_Struct .Data_RX_BUF, ack2 ) );
	}
	else if( ack1 != 0 )
		return ( ( bool ) strstr ( ESP8266_Fram_Record_Struct .Data_RX_BUF, ack1 ) );
	
	else
		return ( ( bool ) strstr ( ESP8266_Fram_Record_Struct .Data_RX_BUF, ack2 ) );
			
}


//����WF-ESP8266ģ��
void ESP8266_Rst(void)
{
	ESP8266_RST_Pin_SetL;
	delay_ms(500); 
	ESP8266_RST_Pin_SetH;
}


//��ESP8266ģ�����AT��������
void ESP8266_AT_Test(void)
{
	char count=0;
	
	ESP8266_RST_Pin_SetH;
	delay_ms(1000);	
	while(count < 10)
	{
		if(ESP8266_Send_AT_Cmd("AT","OK",NULL,500)) 
			return;
		ESP8266_Rst();
		++ count;
	}
}


//ѡ��ESP8266ģ��Ĺ���ģʽ
// enumMode������ģʽ
//����1��ѡ��ɹ� 0��ѡ��ʧ��
bool ESP8266_Net_Mode_Choose(ENUM_Net_ModeTypeDef enumMode)
{
	switch ( enumMode )
	{
		case STA:
			return ESP8266_Send_AT_Cmd ( "AT+CWMODE=1", "OK", "no change", 2500 ); 
		
	  	case AP:
		  	return ESP8266_Send_AT_Cmd ( "AT+CWMODE=2", "OK", "no change", 2500 ); 
		
		case STA_AP:
		  	return ESP8266_Send_AT_Cmd ( "AT+CWMODE=3", "OK", "no change", 2500 ); 
		
	  	default:
		  return false;
	}		
}


//ESP8266ģ�������ⲿWiFi
//pSSID��WiFi�����ַ���
//pPassWord��WiFi�����ַ���
//����1�����ӳɹ� 0������ʧ��
bool ESP8266_JoinAP( char * pSSID, char * pPassWord )
{
	char cCmd [120];

	sprintf ( cCmd, "AT+CWJAP=\"%s\",\"%s\"", pSSID, pPassWord );
	
	return ESP8266_Send_AT_Cmd( cCmd, "OK", NULL, 5000 );
	
}


//ESP8266ģ������������
//enumEnUnvarnishTx�������Ƿ������
//����1�����óɹ� 0������ʧ��
bool ESP8266_Enable_MultipleId (FunctionalState enumEnUnvarnishTx )
{
	char cStr [20];
	
	sprintf ( cStr, "AT+CIPMUX=%d", ( enumEnUnvarnishTx ? 1 : 0 ) );
	
	return ESP8266_Send_AT_Cmd ( cStr, "OK", 0, 1000 );
	
}

//ESP8266ģ�鴴��TCP�����
bool ESP8266_creat_Server(void)
{
	char cStr [40];
		
	sprintf ( cStr, "AT+CIPSERVER=1,8080");
		
	while(ESP8266_Send_AT_Cmd ( cStr, "OK", 0, 500 ) == FALSE)
		{
		
		ESP8266_Rst();

		delay_ms(3000);
	
		}

		//���ó�ʱ
		ESP8266_USART("%s\r\n", "AT+CIPSTO=2880");

	
	return !FALSE;

}

//ESP8266��Server�ˣ�������Ϣ��client��
bool ESP8266_Server_Send(char * str_to_client)
{
	char* str;
	char chd[68] = {0};
	
	u8 t;
	
	delay_ms(15000);

	ESP8266_Fram_Record_Struct .InfBit .FramLength = 0;
	
	ESP8266_USART("%s\r\n", str_to_client);

	delay_ms(500);

	ESP8266_Fram_Record_Struct.Data_RX_BUF[ESP8266_Fram_Record_Struct.InfBit.FramLength ] = '\0';

	PC_USART("%s", ESP8266_Fram_Record_Struct .Data_RX_BUF);

	str = strstr(ESP8266_Fram_Record_Struct .Data_RX_BUF, "+CIPSTATUS:");

	t = (u8)(str[11] - '0');

	sprintf(chd, "%s%d%c%d", "AT+CIPSEND=",t,',',64);

	ESP8266_Send_AT_Cmd(chd,"OK",0,500);

	ESP8266_USART("%s\r\n", str_to_client);
	
	return !FALSE;
}


//Esp8266�������
//ֻ�� IDΪ0�����ӷ�����Ϣ
// �����Ҫ���������ӷ���Ϣ����Ҫ�޸�Multiple_ID_0�Ă�
void Server_8266_AP_send(char* string)
{
	char str[100];
	char *str1;
	u8 i = 0;
	memset(str, '\0', sizeof(str));
	sprintf(str, "%s\r\n", string);
	str1 = strstr(str,"+IPD");
	if(str1)
		{
		while(1)
			{
			if(str1[i] == ':')
				{
				break;
				}
			i++;
			}
		str1 = &str1[i+1];
		while(!ESP8266_SendString(DISABLE, str1, (u32)(strlen(str1)), ucId));
		}
	else
		{
		while(!ESP8266_SendString(DISABLE, str, (u32)(strlen(str)), ucId));
		}
	ESP8266_Fram_Record_Struct .InfBit .FramLength = 0;

}

// �ȴ�WIFI�˺�����
void Receiver_Wifi_Account(void)
{

	
	printf("\r\n������ WiFi �˻�������	SSID:#####, PWD:#######  \r\n");

	
	Flag_wifi = 0;

	memset(&WIFI1, '\0', sizeof(Wifi_Account));
		
	ESP8266_Fram_Record_Struct .InfBit .FramLength = 0; //���¿�ʼ�����µ����ݰ�

	
	while(1)
			{
			//printf("Data_RX_BUF = %s\n\n", ESP8266_Fram_Record_Struct .Data_RX_BUF);
			 if(CONNECT_ID())
				{
				ESP8266_Fram_Record_Struct .InfBit .FramLength = 0;
				Server_8266_AP_send("..................������ WiFi �˻�������	SSID:#####, PWD:#######\r\n");
				Flag_wifi = 0;
				break;
				}
			 else
				{
				ESP8266_Fram_Record_Struct .InfBit .FramLength = 0;
				delay_ms(1000);
				ESP8266_Fram_Record_Struct.Data_RX_BUF[ESP8266_Fram_Record_Struct.InfBit.FramLength ] = '\0';
				}
			}
		
	while(1)
		{
		if(Flag_wifi == 1)
			{
				ESP8266_Fram_Record_Struct.Data_RX_BUF[ESP8266_Fram_Record_Struct.InfBit.FramLength ] = '\0';
				printf("%s\r\n", ESP8266_Fram_Record_Struct.Data_RX_BUF);
				Wifi_Resolve(); 
	
				if('\0' != *WIFI1.SSID)
					{
					Flag_wifi = 0;
					printf("\r\n�˺ź������ȡ�ɹ� \r\n"); 
					Server_8266_AP_send("\r\n�˺ź������ȡ�ɹ� \r\n");
					//Server_8266_AP_send("\r\n ׼������wifi��............\r\n");
					
					break;
					}
			
			}
		delay_ms( 1000 );
		}
				

}


// wifi�˺Ž���	
	void  Wifi_Resolve(void)
{
		char *str1;
		char *str2;
		u8 i = 0;
	
		str1 = strstr(ESP8266_Fram_Record_Struct .Data_RX_BUF, "SSID");
		str2 = strstr(ESP8266_Fram_Record_Struct .Data_RX_BUF, "PWD");
		
		if((NULL == str1) || (NULL == str2))
			{
				printf("%s", ESP8266_Fram_Record_Struct.Data_RX_BUF);
				printf("\r\n ��������˺Ÿ�ʽ������� ���������� SSID:#####, PWD:####### \r\n");
				Flag_wifi = 0;
				ESP8266_Fram_Record_Struct .InfBit .FramLength = 0;
				return;
			}

			//��ȡ�˺�
			while(str1[5+i] != ',')
				{
				WIFI1.SSID[i] = (str1[5 + i]);
				i++;
				}
				WIFI1.SSID[i] = '\0';
			
				i = 0;
			printf("\r\n WIFI1.SSID = %s \r\n ", WIFI1.SSID);
		
			//��ȡ����
			//memcpy(WIFI1.PWD, &str2[4], strlen(&str2[4]));
			while((str2[4 + i] != '\r') && (str2[4 + i] != '\n') && (str2[4 + i] != ' '))
				{
					WIFI1.PWD[i] = str2[4 + i];
					i++;
				}
				WIFI1.PWD[i] = '\0';
				
			printf("\r\n WIFI1.PWD = %s \r\n ", WIFI1.PWD);
			Server_8266_AP_send(ESP8266_Fram_Record_Struct.Data_RX_BUF);

			//�ж� ���� �����˺ų����ǲ��ǳ���
			if((strlen(WIFI1.PWD) > 32) || (strlen(WIFI1.SSID) > 32))
				{
				printf(" \r\n ���� ���� �˺� ���ȳ��� \r\n");
				memset(&WIFI1, '\0', sizeof(Wifi_Account));
				return;
				}
				
			
			
}

void Rec_Port(void)
{
	char* str;
	u8 i = 0;
	Server_8266_AP_send("������pc�˿ں�\n");
	ESP8266_Fram_Record_Struct .InfBit .FramLength = 0;
	Flag_port = 0;
	while(1)
		{
	while(Flag_port)
		{
		ESP8266_Fram_Record_Struct.Data_RX_BUF[ESP8266_Fram_Record_Struct.InfBit.FramLength ] = '\0';
		
		
		str = ESP8266_Fram_Record_Struct.Data_RX_BUF;
	
		if(':' == str[i])
			{
				printf("str[i] = %s", &str[i+1]);
				
				sprintf(PC_PORT, "%s", &str[i+1]);
				printf("pc_port = %s\n", PC_PORT);
				
				Server_8266_AP_send(PC_PORT);
				Server_8266_AP_send("\n ok \n");
				
				return;
			}

		i++;
		}
		delay_ms(1000);
		}
}

void Rec_IP(void)
{
	char* str;
	u8 i = 0 ;
	Server_8266_AP_send("������	pc��IP��\n");
	ESP8266_Fram_Record_Struct .InfBit .FramLength = 0;
	Flag_ip = 0;
	while(1)
		{
	while(Flag_ip)
		{
		
		ESP8266_Fram_Record_Struct.Data_RX_BUF[ESP8266_Fram_Record_Struct.InfBit.FramLength ] = '\0';
		str = ESP8266_Fram_Record_Struct.Data_RX_BUF;
		if(':' == str[i])
			{
				sprintf(PC_IP, "%s", &str[i+1]);
				Server_8266_AP_send(PC_IP);
				Server_8266_AP_send("\n ok \n");
				return;
			}

		i++;
		}
	delay_ms(1000);
		}
		
}


//ESP8266ģ��ر�TCP�����
bool ESP8266_Shut_Server(void)
{
	char cStr [40];

	ESP8266_Rst();

	sprintf ( cStr, "AT+CIPSERVER=0");

	return ESP8266_Send_AT_Cmd ( cStr, "OK", 0, 500 );
}



//ESP8266ģ�������ⲿ������
//enumE������Э��
//ip��������IP�ַ���
//ComNum���������˿��ַ���
//id��ģ�����ӷ�������ID
//����1�����ӳɹ� 0������ʧ��
bool ESP8266_Link_Server(ENUM_NetPro_TypeDef enumE, char * ip, char * ComNum, ENUM_ID_NO_TypeDef id)
{
	char cStr [100] = { 0 }, cCmd [120];

  	switch (  enumE )
  	{
		case enumTCP:
		  sprintf ( cStr, "\"%s\",\"%s\",%s", "TCP", ip, ComNum );
		  break;
		
		case enumUDP:
		  sprintf ( cStr, "\"%s\",\"%s\",%s", "UDP", ip, ComNum );
		  break;
		
		default:
			break;
  	}

  	if ( id < 5 )
    	sprintf ( cCmd, "AT+CIPSTART=%d,%s", id, cStr);

  	else
	  	sprintf ( cCmd, "AT+CIPSTART=%s", cStr );

	return ESP8266_Send_AT_Cmd ( cCmd, "OK", "ALREAY CONNECT", 4000 );
	
}


//����ESP8266ģ�����͸������
//����1�����óɹ� 0������ʧ��
bool ESP8266_UnvarnishSend ( void )
{
	if (!ESP8266_Send_AT_Cmd ( "AT+CIPMODE=1", "OK", 0, 500 ))
		return false;
	
	return 
	  	ESP8266_Send_AT_Cmd( "AT+CIPSEND", "OK", ">", 500 );
	
}


//ESP8266ģ�鷢���ַ���
//enumEnUnvarnishTx�������Ƿ���ʹ����͸��ģʽ
//pStr��Ҫ���͵��ַ���
//ulStrLength��Ҫ���͵��ַ������ֽ���
//ucId���ĸ�ID���͵��ַ���
//����1�����ͳɹ� 0������ʧ��
bool ESP8266_SendString(FunctionalState enumEnUnvarnishTx, char * pStr, u32 ulStrLength, ENUM_ID_NO_TypeDef ucId )
{
	char cStr [20];
	bool bRet = false;
	
		
	if ( enumEnUnvarnishTx )
	{
		ESP8266_USART ( "%s", pStr );
		
		bRet = true;
		
	}

	else
	{
		if ( ucId < 5 )
			sprintf ( cStr, "AT+CIPSEND=%d,%d", ucId, ulStrLength + 2 );

		else
			sprintf ( cStr, "AT+CIPSEND=%d", ulStrLength + 2 );
		
		ESP8266_Send_AT_Cmd ( cStr, "> ", 0, 1000 );

		bRet = ESP8266_Send_AT_Cmd ( pStr, "SEND OK", 0, 1000 );
  }
	
	return bRet;

}


//ESP8266ģ���˳�͸��ģʽ
void ESP8266_ExitUnvarnishSend ( void )
{
	delay_ms(1000);
	ESP8266_USART( "+++" );
	delay_ms( 500 ); 	
}


//ESP8266 ������״̬�����ʺϵ��˿�ʱʹ��
//����0����ȡ״̬ʧ��
//����2�����ip
//����3���������� 
//����4��ʧȥ���� 
u8 ESP8266_Get_LinkStatus ( void )
{
	if (ESP8266_Send_AT_Cmd( "AT+CIPSTATUS", "OK", 0, 500 ) )
	{
		if ( strstr ( ESP8266_Fram_Record_Struct .Data_RX_BUF, "STATUS:2\r\n" ) )
			return 2;
		
		else if ( strstr ( ESP8266_Fram_Record_Struct .Data_RX_BUF, "STATUS:3\r\n" ) )
			return 3;
		
		else if ( strstr ( ESP8266_Fram_Record_Struct .Data_RX_BUF, "STATUS:4\r\n" ) )
			return 4;		

	}
	
	return 0;
	
}

