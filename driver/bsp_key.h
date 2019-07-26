#ifndef __KEY_H
#define	__KEY_H


#include "stm32f0xx.h"

// 使用KEY1，如果要使用KEY2的话，把宏KEY1_PA0注释掉即可
#define   KEYI_PA0

#ifdef    KEYI_PA0
#define               macKEY1_GPIO_CLK                      RCC_AHBPeriph_GPIOB
#define               macKEY1_GPIO_PORT    	                GPIOB			   
#define               macKEY1_GPIO_PIN		                  GPIO_Pin_10

#else    //KEY2_PC13
#define               macKEY1_GPIO_CLK                      RCC_AHBPeriph_GPIOC
#define               macKEY1_GPIO_PORT    	                GPIOC		   
#define               macKEY1_GPIO_PIN		                  GPIO_Pin_13

#endif

 /*******
 *按键按下标置，按键K1和K2默认是低电平，按键按下的时候是高电平
 KEY_ON 0
 KEY_OFF 1
 ********/
#define KEY_ON	0
#define KEY_OFF	1

#define MAX_KEY         2
#define PRESS_KEY1      0x01
#define KEY_NO          0x00
#define KEY_DOWN        0x10  	
#define KEY_UP          0x20   
#define KEY_LONG        0x40
#define TIME_PRESS_LONG         (3 * 500)
#define TIME_PRESS_SHORT        50

static uint8_t Get_Key(void); 
void Key_GPIO_Config(void);
uint8_t Key_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin);
void Key1_ShortPress_Handle(void); 
void Key1_LongPress_Handle(void); 


#endif /* __LED_H */

