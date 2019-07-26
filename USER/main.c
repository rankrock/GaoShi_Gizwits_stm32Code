/*********************************************************************************
 * 文 件 名：main.c
 * 描    述：主控MCU stm32f030c8t6,侵权必究
 * 作    者：https://item.taobao.com/item.htm?spm=a1z38n.10677092.0.0.79311debo4Xft6&id=566321791377
 * 版    本：1.0
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
/** 用户区当前设备状态结构体*/
dataPoint_t currentDataPoint;
/**@name 按键相关定义 
* @{
*/
#define GPIO_KEY_NUM 2                                  ///< 定义按键成员总数
keyTypedef_t singleKey[GPIO_KEY_NUM];                   ///< 定义单个按键成员数组指针
keysTypedef_t keys;                                     ///< 定义总的按键模块结构体指针
/**@} */
/**
* 用户初始化函数

* 在该函数中完成了外设驱动初始化以及用户相关数据的初始
* @param none
* @return none
* @note 开发者可在此函数内添加新的驱动初始及状态初始化
*/
void userInit(void)
{
  
  
    rgbLedInit();
   rgbKeyGpioInit();
    motorInit();
//    dht11Init();
//    irInit();
//    watchdogInit(2);    //5,625看门狗复位时间2s
    memset((uint8_t*)&currentDataPoint, 0, sizeof(dataPoint_t));
    
		motorStatus(MOTOR_SPEED_DEFAULT);

//	
}
RCC_ClocksTypeDef RCC_ClockFreq;
static void RCC_Configuration(void)
{   
//RCC配置 
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
* 用户数据获取

* 此处需要用户实现除可写数据点之外所有传感器数据的采集,可自行定义采集频率和设计数据过滤算法
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
			
	/*调用DHT11_Read_TempAndHumidity读取温湿度，若成功则输出该信息*/
	if( DHT11_Read_TempAndHumidity ( & DHT11_Data ) == SUCCESS)
	{
		printf("\r\n读取DHT11成功!\r\n\r\n湿度为%d.%d ％RH ，温度为 %d.%d℃ \r\n",\
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
* key1按键短按处理
* @param none
* @return none
*/
void key1ShortPress(void)
{
    printf("KEY1 PRESS ,Production Mode\n");
    gizwitsSetMode(WIFI_PRODUCTION_TEST);
}

/**
* key1按键长按处理
* @param none
* @return none
*/
void key1LongPress(void)
{
    printf("KEY1 PRESS LONG ,Wifi Reset\n");
    gizwitsSetMode(WIFI_RESET_MODE);
}

/**
* key2按键短按处理
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
* key2按键长按处理
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
* 按键初始化
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
	
/*******主程序,系统时钟为48M*******/
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
	
	/*初始化DTT11的引脚*/
	DHT11_Init ();
	motorStatus(2);
	while(1)
	{	
		test++;
		userHandle();
		gizwitsHandle((dataPoint_t *)&currentDataPoint);		
		
	}
 
}

