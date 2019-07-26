/*********************************************************************************
 * �� �� ����main.c
 * ��    ��������MCU stm32f030c8t6,��Ȩ�ؾ�
 * ��    �ߣ�https://item.taobao.com/item.htm?spm=a1z38n.10677092.0.0.79311debo4Xft6&id=566321791377
 * ��    ����1.0
**********************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"
#include <string.h>
#include "time.h"
#include "Hal_motor.h"
#include "gpio_driver.h"
#include "usart_driver.h"
#include "delay.h"
#include "hal_key.h"

#include "bsp_dht11.h"
#include "Hal_rgb_led.h"
#include "gizwits_product.h"
uint16_t test=0;
DHT11_Data_TypeDef DHT11_Data;
/** �û�����ǰ�豸״̬�ṹ��*/
dataPoint_t currentDataPoint;
/**@name ������ض��� 
* @{
*/
#define GPIO_KEY_NUM 2                                  ///< ���尴����Ա����
keyTypedef_t singleKey[GPIO_KEY_NUM];                   ///< ���嵥��������Ա����ָ��
keysTypedef_t keys;                                     ///< �����ܵİ���ģ��ṹ��ָ��
/**@} */
/**
* �û���ʼ������

* �ڸú��������������������ʼ���Լ��û�������ݵĳ�ʼ
* @param none
* @return none
* @note �����߿��ڴ˺���������µ�������ʼ��״̬��ʼ��
*/
void userInit(void)
{
  
  
    rgbLedInit();
   rgbKeyGpioInit();
    motorInit();
//    dht11Init();
//    irInit();
//    watchdogInit(2);    //5,625���Ź���λʱ��2s
    memset((uint8_t*)&currentDataPoint, 0, sizeof(dataPoint_t));
    
		motorStatus(MOTOR_SPEED_DEFAULT);

//	
}
RCC_ClocksTypeDef RCC_ClockFreq;
static void RCC_Configuration(void)
{   
//RCC���� 
    RCC_DeInit();                                /* RCC system reset(for debug purpose) */
    RCC_HCLKConfig(RCC_SYSCLK_Div1);            /* HCLK = SYSCLK */
    RCC_PCLKConfig(RCC_HCLK_Div1);                /* PCLK2 = HCLK */
    RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_12);    /* PLLCLK = 8MHz /2 * 12 = 48 MHz */
    RCC_PLLCmd(ENABLE);                            /* Enable PLL */
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET) {}    /* Wait till PLL is ready */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);    /* Select PLL as system clock source */
    while(RCC_GetSYSCLKSource() != 0x08) {}        /* Wait till PLL is used as system clock source */		
    RCC_GetClocksFreq(&RCC_ClockFreq);             // This function fills a RCC_ClocksTypeDef structure with the current frequencies of different on chip clocks (for debug purpose)
}
/**
* �û����ݻ�ȡ

* �˴���Ҫ�û�ʵ�ֳ���д���ݵ�֮�����д��������ݵĲɼ�,�����ж���ɼ�Ƶ�ʺ�������ݹ����㷨
* @param none
* @return none
*/
void userHandle(void)
{
    uint8_t ret  = 0;
    static uint32_t thLastTimer = 0;
    
//    currentDataPoint.valueInfrared = irHandle();
		

    if((gizGetTimerCount()-thLastTimer) > SAMPLING_TIME_MAX) 
    {
			
	/*����DHT11_Read_TempAndHumidity��ȡ��ʪ�ȣ����ɹ����������Ϣ*/
	if( DHT11_Read_TempAndHumidity ( & DHT11_Data ) == SUCCESS)
	{
		printf("\r\n��ȡDHT11�ɹ�!\r\n\r\nʪ��Ϊ%d.%d ��RH ���¶�Ϊ %d.%d�� \r\n",\
		DHT11_Data.humi_int,DHT11_Data.humi_deci,DHT11_Data.temp_int,DHT11_Data.temp_deci);
	}
	
	else
		printf("Read DHT11 ERROR!\r\n");	
  currentDataPoint.valueTemperature = DHT11_Data.temp_int;
	currentDataPoint.valueHumidity=DHT11_Data.humi_int ;	
  thLastTimer = gizGetTimerCount();
    }
}

/**
* key1�����̰�����
* @param none
* @return none
*/
void key1ShortPress(void)
{
    printf("KEY1 PRESS ,Production Mode\n");
    gizwitsSetMode(WIFI_PRODUCTION_TEST);
}

/**
* key1������������
* @param none
* @return none
*/
void key1LongPress(void)
{
    printf("KEY1 PRESS LONG ,Wifi Reset\n");
    gizwitsSetMode(WIFI_RESET_MODE);
}

/**
* key2�����̰�����
* @param none
* @return none
*/
void key2ShortPress(void)
{
    printf("KEY2 PRESS ,Soft AP mode\n");

    //Soft AP mode, RGB red
    ledRgbControl(250, 0, 0);
    gizwitsSetMode(WIFI_SOFTAP_MODE);
}

/**
* key2������������
* @param none
* @return none
*/
void key2LongPress(void)
{
    printf("KEY2 PRESS LONG ,AirLink mode\n");

    //AirLink mode, RGB Green
    ledRgbControl(0, 250, 0);
    gizwitsSetMode(WIFI_AIRLINK_MODE);
}

/**
* ������ʼ��
* @param none
* @return none
*/
void keyInit(void)
{
    singleKey[0] = keyInitOne(RCC_AHBPeriph_GPIOB, GPIOB, GPIO_Pin_10, key1ShortPress, key1LongPress);
    singleKey[1] = keyInitOne(RCC_AHBPeriph_GPIOA, GPIOA, GPIO_Pin_8, key2ShortPress, key2LongPress);
    keys.singleKey = (keyTypedef_t *)&singleKey;
    keyParaInit(&keys); 
}
	
/*******������,ϵͳʱ��Ϊ48M*******/
int main(void)
{

	RCC_Configuration();
	__set_PRIMASK(1);    
	delay_init();	
	userInit();
	USART1_Config();
	keyInit();

	gizwitsInit();
	__set_PRIMASK(0);
	printf("MCU Init Success \n");	
	
	/*��ʼ��DTT11������*/
	DHT11_Init ();
	motorStatus(2);
	while(1)
	{	
		test++;
		userHandle();
		gizwitsHandle((dataPoint_t *)&currentDataPoint);		
		
	}
 
}

