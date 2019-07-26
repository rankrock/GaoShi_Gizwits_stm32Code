#ifndef __GPIO_DRIVER_H
#define	__GPIO_DRIVER_H

#include "stm32f0xx.h"

/* 定义ON/OFF*/

#define ON  1 /*亮*/
#define OFF 0 /*灭*/

#define LED1(a)  if (a)    \
                    GPIO_SetBits(GPIOC,GPIO_Pin_1);\
                 else        \
                    GPIO_ResetBits(GPIOC,GPIO_Pin_1)

#define LED2(a)  if (a)    \
                    GPIO_SetBits(GPIOB,GPIO_Pin_10);\
                 else        \
                    GPIO_ResetBits(GPIOB,GPIO_Pin_10)

#define LED3(a)  if (a)    \
                    GPIO_SetBits(GPIOB,GPIO_Pin_11);\
                 else        \
                    GPIO_ResetBits(GPIOB,GPIO_Pin_11)

/*指定IO翻转*/

#define CS1_EN_4052      GPIO_ResetBits(GPIOB,GPIO_Pin_12)
#define CS2_EN_4052      GPIO_ResetBits(GPIOB,GPIO_Pin_13)
#define CS1_DIS_4052     GPIO_SetBits(GPIOB,GPIO_Pin_12)
#define CS2_DIS_4052     GPIO_SetBits(GPIOB,GPIO_Pin_13)
#define A_HIGH_4052      GPIO_SetBits(GPIOB,GPIO_Pin_14)
#define B_HIGH_4052      GPIO_SetBits(GPIOB,GPIO_Pin_15)
#define A_LOW_4052       GPIO_ResetBits(GPIOB,GPIO_Pin_14)
#define B_LOW_4052       GPIO_ResetBits(GPIOB,GPIO_Pin_15)





                 
#define LED_GPIOA11_RVL  GPIO_WriteBit(GPIOA, GPIO_Pin_11, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_11)))
                 
void GPIO_Config(void);

#endif /* __LED_H */
