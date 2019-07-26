#ifndef _TIME_H_
#define _TIME_H_

#include "stm32f0xx.h"


//#define TIM_1   
#define TIM_2     //作为基础定时器1ms
#define TIM_3     //作为基础定时器

extern uint16_t lamp_light;

void TIM_Config(void);
void TIM2_Interrupt_Entry(void);
//void TIM3_Interrupt_Entry(void);

#endif  /* _TIME_H_ */
