#include "system.h"
#include "SysTick.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "beep.h"
#include "ds18b20.h"
#include "esp8266_drive.h"
#include "sta_tcpclent_test.h"



int main()
{  	
	u8 i;
	Systick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	USART1_Init(115200);
	LED_Init();
	KEY_Init();
	DS18B20_Init();

	printf("普中科技ESP8266 WIFI模块STA TCPClent透传测试\r\n");
	ESP8266_Init(115200);
	
	ESP8266_STA_TCPClient_Test();

	while(1)
	{
		i++;
		if(i%20==0)
		{
			led0=!led0;
		}
		delay_ms(10);		
	}
}



// 打开串口
void RCC_Configuration(void)

{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA | 
  RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
}








