/**
************************************************************
* @file         gizwits_product.c
* @brief        Gizwits 控制协议处理,及平台相关的硬件初始化 

*
***********************************************************/

#include <stdio.h>
#include <string.h>
#include "hal_uart.h"
#include "Hal_motor.h"
#include "Hal_rgb_led.h"

//#include "Hal_infrared.h"
#include "gizwits_product.h"

/** 用户区设备状态变量 */
extern dataPoint_t currentDataPoint;

/**@name Gizwits 用户API接口
* @{
*/

/**
* @brief 事件处理接口

* 说明：

* 1.用户可以对WiFi模组状态的变化进行自定义的处理

* 2.用户可以在该函数内添加数据点事件处理逻辑，如调用相关硬件外设的操作接口

* @param[in] info : 事件队列
* @param[in] data : 协议数据
* @param[in] len  : 协议数据长度
* @return NULL
* @ref gizwits_protocol.h
*/
int8_t gizwitsEventProcess(eventInfo_t *info, uint8_t *data, uint32_t len)
{
  uint8_t i = 0;
  dataPoint_t *dataPointPtr = (dataPoint_t *)data;
  moduleStatusInfo_t *wifiData = (moduleStatusInfo_t *)data;

  protocolTime_t *ptime = (protocolTime_t *)data;

  if((NULL == info) || (NULL == data))
  {
    return -1;
  }

  for(i=0; i<info->num; i++)
  {
    switch(info->event[i])
    {
      case EVENT_LED_ONOFF:
        currentDataPoint.valueLED_OnOff = dataPointPtr->valueLED_OnOff;
        GIZWITS_LOG("Evt: EVENT_LED_ONOFF %d \n", currentDataPoint.valueLED_OnOff);
        if(0x01 == currentDataPoint.valueLED_OnOff)
        {
            ledRgbControl(255,0,0);
        }
        else
        {
            ledRgbControl(0,0,0);  
        }
        break;

      case EVENT_LED_COLOR:
        currentDataPoint.valueLED_Color = dataPointPtr->valueLED_Color;
        GIZWITS_LOG("Evt: EVENT_LED_COLOR %d\n", currentDataPoint.valueLED_Color);
        switch(currentDataPoint.valueLED_Color)
        {
          case LED_COLOR_VALUE0:
            ledRgbControl(currentDataPoint.valueLED_R,currentDataPoint.valueLED_G,currentDataPoint.valueLED_B);
						break;
          case LED_COLOR_VALUE1:
            ledRgbControl(254, 254, 0); 
            break;
          case LED_COLOR_VALUE2:
            ledRgbControl(254, 0, 70);
            break;
          case LED_COLOR_VALUE3:
            ledRgbControl(238 ,30 ,30); 
            break;
          default:
            break;
        }
        break;

      case EVENT_LED_R:
        currentDataPoint.valueLED_R = dataPointPtr->valueLED_R;
        GIZWITS_LOG("Evt:EVENT_LED_R %d\n",currentDataPoint.valueLED_R);
        ledRgbControl(currentDataPoint.valueLED_R,currentDataPoint.valueLED_G,currentDataPoint.valueLED_B);
			  //ledRgbControl(100,100,100);
				break;
      case EVENT_LED_G:
        currentDataPoint.valueLED_G = dataPointPtr->valueLED_G;
        GIZWITS_LOG("Evt:EVENT_LED_G %d\n",currentDataPoint.valueLED_G);
        ledRgbControl(currentDataPoint.valueLED_R,currentDataPoint.valueLED_G,currentDataPoint.valueLED_B);
				break;
      case EVENT_LED_B:
        currentDataPoint.valueLED_B = dataPointPtr->valueLED_B;
        GIZWITS_LOG("Evt:EVENT_LED_B %d\n",currentDataPoint.valueLED_B);
        ledRgbControl(currentDataPoint.valueLED_R,currentDataPoint.valueLED_G,currentDataPoint.valueLED_B);
				break;
      case EVENT_MOTOR_SPEED:
        currentDataPoint.valueMotor_Speed = dataPointPtr->valueMotor_Speed;
        GIZWITS_LOG("Evt:EVENT_MOTOR_SPEED %d\n",currentDataPoint.valueMotor_Speed);
        motorStatus(currentDataPoint.valueMotor_Speed);
        break;


      case WIFI_SOFTAP:
        break;
      case WIFI_AIRLINK:
        break;
      case WIFI_STATION:
        break;
      case WIFI_CON_ROUTER:
	      ledRgbControl(0, 0, 0); 
        break;
      case WIFI_DISCON_ROUTER:
        break;
      case WIFI_CON_M2M:
        break;
      case WIFI_DISCON_M2M:
        break;
      case WIFI_RSSI:
        GIZWITS_LOG("RSSI %d\n", wifiData->rssi);
        break;
      case TRANSPARENT_DATA:
        GIZWITS_LOG("TRANSPARENT_DATA \n");
        //user handle , Fetch data from [data] , size is [len]
        break;

      case WIFI_NTP:
        GIZWITS_LOG("WIFI_NTP : [%d-%d-%d %02d:%02d:%02d][%d] \n",ptime->year,ptime->month,ptime->day,ptime->hour,ptime->minute,ptime->second,ptime->ntp);
        break;
      default:
        break;
    }
  }

  return 0;
}

/**@} */

/**
* @brief MCU复位函数

* @param none
* @return none
*/
void mcuRestart(void)
{
    __set_PRIMASK(1); 
    NVIC_SystemReset();
}

/**@} */

/**
* @brief 定时器TIM3中断处理函数

* @param none
* @return none
*/
void TIMER_IRQ_FUN(void)
{
    if (TIM_GetITStatus(TIMER, TIM_IT_Update) != RESET)  
    {
        TIM_ClearITPendingBit(TIMER, TIM_IT_Update  );
        gizTimerMs();
    }
}

/**
* @brief 定时器TIM3定时器初始化

* @param none
* @return none
*/
void timerInit(void)
{
    uint16_t arr = 7199;
    uint16_t psc = 9;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(TIMER_RCC, ENABLE); //时钟使能

    //定时器TIM3初始化
    TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重载寄存器周期的值
    TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率的预分频值
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
    TIM_TimeBaseInit(TIMER, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位0

    TIM_ITConfig(TIMER, TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

    //中断优先级NVIC设置
    NVIC_InitStructure.NVIC_IRQChannel = TIMER_IRQ;  //TIM3中断
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
    NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器
    TIM_Cmd(TIMER, ENABLE);  //使能TIMx
}

/**
* @brief USART2串口中断函数

* 接收功能，用于接收与WiFi模组间的串口协议数据
* @param none
* @return none
*/
 uint8_t value = 0;
void UART_IRQ_FUN(void)
{

    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        USART_ClearITPendingBit(USART2,USART_IT_RXNE);
        value = USART_ReceiveData(USART2);

        gizPutData(&value, 1);
    }
}

/**
* @brief 串口写操作，发送数据到WiFi模组
*
* @param buf       : 数据地址
* @param len       : 数据长度
*
* @return : 正确返回有效数据长度;-1，错误返回
*/
int32_t uartWrite(uint8_t *buf, uint32_t len)
{
    uint32_t i = 0;
    
    if(NULL == buf)
    {
        return -1;
    }
#ifdef PROTOCOL_DEBUG
    GIZWITS_LOG("MCU2WiFi[%4d:%4d]: ", gizGetTimerCount(), len);
#endif
    
    for(i=0; i<len; i++)
    {
        USART_SendData(UART, buf[i]);
        while (USART_GetFlagStatus(UART, USART_FLAG_TXE) == RESET);//Loop until the end of transmission
#ifdef PROTOCOL_DEBUG
        GIZWITS_LOG("%02x ", buf[i]);
#endif
        if(i >=2 && buf[i] == 0xFF)
        {
          USART_SendData(UART,0x55);
          while (USART_GetFlagStatus(UART, USART_FLAG_TXE) == RESET);//Loop until the end of transmission
#ifdef PROTOCOL_DEBUG
        GIZWITS_LOG("%02x ", 0x55);
#endif
        }
    }
    
#ifdef PROTOCOL_DEBUG
    GIZWITS_LOG("\n");
#endif
    
    return len;
}

/**
* @brief USART串口初始化

* 用于与WiFi模组间的串口通信
* @param none
* @return none
*/
void uartInit(void)
{
   
		GPIO_InitTypeDef  GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef 	NVIC_InitStructure;
	
		RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOA, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE );						
		GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_1);
		GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_1);     											 
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3;                 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);   

		USART_InitStructure.USART_BaudRate = 9600;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		USART_Init(USART2, &USART_InitStructure);

    USART_ITConfig(UART,USART_IT_RXNE,ENABLE);
    USART_Cmd(UART, ENABLE);
    USART_ClearFlag(UART, USART_FLAG_TC); /*清空发送完成标志,Transmission Complete flag */

    /*使能串口中断,并设置优先级*/
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPriority = 2;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
    
}

