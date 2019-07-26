/**
************************************************************
* @file         gizwits_product.h
* @brief        对应gizwits_product.c的头文件(包含产品软硬件版本定义)
* @author       Gizwits
* @date         2016-12-01
* @version      V03010101
* @copyright    Gizwits
* 
* @note         机智云.只为智能硬件而生
*               Gizwits Smart Cloud  for Smart Products
*               链接|增值ֵ|开放|中立|安全|自有|自由|生态
*               www.gizwits.com
*
***********************************************************/
#ifndef _GIZWITS_PRODUCT_H
#define _GIZWITS_PRODUCT_H

#include <stdint.h>
#include "stm32f0xx.h"
#include "gizwits_protocol.h"

/**
* MCU软件版本号
*/
#define SOFTWARE_VERSION "03010102"
/**
* MCU硬件版本号
*/
#define HARDWARE_VERSION "03010100"

#ifndef SOFTWARE_VERSION
    #error "no define SOFTWARE_VERSION"
#endif

#ifndef HARDWARE_VERSION
    #error "no define HARDWARE_VERSION"
#endif

/**@name TIM3相关宏定义
* @{
*/
#define TIMER 					TIM3
#define TIMER_IRQ 				TIM3_IRQn
#define TIMER_RCC 				RCC_APB1Periph_TIM3
#define TIMER_IRQ_FUN 			TIM3_IRQHandler
/**@} */

/**@name USART相关宏定义
* @{
*/
#define UART_BAUDRATE 			9600
#define UART_PORT     			2
#define UART          			USART2
#define UART_IRQ      			USART2_IRQn
#define UART_IRQ_FUN  			USART2_IRQHandler


/**@} */

/**@name 设备状态数据上报间隔时间宏定义
* @{
*/
#define REPORT_TIME_MAX 6000   //6S
/**@} */

/**@name 采集间隔时间宏定义
* @{
*/
#define SAMPLING_TIME_MAX 2000 //2S
/**@} */


void timerInit(void);
void uartInit(void);
void mcuRestart(void);
int32_t uartWrite(uint8_t *buf, uint32_t len);

/**@name Gizwits 用户API接口
* @{
*/
int8_t gizwitsEventProcess(eventInfo_t *info, uint8_t *data, uint32_t len);
/**@} */

#endif
