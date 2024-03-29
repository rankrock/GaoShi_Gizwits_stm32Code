/**
************************************************************
* @file         hal_key.h
* @brief        hal_key.c对应头文件

*
***********************************************************/
#ifndef _HAL_KEY_H
#define _HAL_KEY_H

#include <stdio.h>
#include "stm32f0xx.h"

#define G_SET_BIT(a,b)                            (a |= (1 << b))
#define G_CLEAR_BIT(a,b)                          (a &= ~(1 << b))
#define G_IS_BIT_SET(a,b)                         (a & (1 << b))

#define KEY_TIMER_MS                            1
#define KEY_MAX_NUMBER                          12
#define DEBOUNCE_TIME                           30
#define PRESS_LONG_TIME                         1500

#define NO_KEY                                  0x0000
#define KEY_DOWN                                0x1000
#define KEY_UP                                  0x2000
#define KEY_LIAN                                0x4000
#define KEY_LONG                                0x8000

typedef void (*gokitKeyFunction)(void);

__packed typedef struct
{
    uint8_t          keyNum;
    uint32_t         keyRccPeriph;
    GPIO_TypeDef     *keyPort;
    uint32_t         keyGpio;
    gokitKeyFunction shortPress; 
    gokitKeyFunction longPress; 
}keyTypedef_t;

__packed typedef struct
{
    uint8_t      keyTotolNum;
    keyTypedef_t *singleKey; 
}keysTypedef_t; 

void keyGpioInit(void);
void keyHandle(keysTypedef_t *keys);
void keyParaInit(keysTypedef_t *keys);
uint16_t getKey(keysTypedef_t *key);
uint16_t readKeyValue(keysTypedef_t *keys);
keyTypedef_t keyInitOne(uint32_t keyRccPeriph, GPIO_TypeDef * keyPort, uint32_t keyGpio, gokitKeyFunction shortPress, gokitKeyFunction longPress);

#endif /*_HAL_KEY_H*/

