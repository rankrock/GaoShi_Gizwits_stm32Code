/**
********************************************************
*
* @file      Hal_motor.c
*
*********************************************************/
#include <math.h>
#include "Hal_motor.h"


/*******************************************************************************
* Function Name  : tim3GpioConfig
* Description    : 配置TIM3复用输出PWM的IO
* Input          : 无
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void tim3GpioConfig(void) 
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  /* 使能GPIOB时钟 */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE); 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE); 
	
  /* 配置相应引脚*/
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
//  GPIO_Init(GPIOB, &GPIO_InitStructure);
//	GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_1);
//	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_1);
	
	//PB10、PB11 端口输出配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_2); // TIM2_CH3
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_2); // TIM2_CH4
}

/*******************************************************************************
* Function Name  : tim3ModeConfig
* Description    : 配置TIM3输出时的PWM信号的模式,如周期、极性、占空比
* Input          : 无
* Output         : None
* Return         : None
* Attention		   : None
*******************************************************************************/

void tim3ModeConfig(void)
{
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
	
		/* PWM信号电平跳变值 */
    uint16_t ccr1Val = 500;
    uint16_t ccr2Val = 500;
	
		
/* -----------------------------------------------------------------------
    TIM3 Configuration: generate 4 PWM signals with 4 different duty cycles:
    TIM3CLK = 72 MHz, Prescaler = 0x0, TIM3 counter clock = 72 MHz
    TIM3 ARR Register = 999 => TIM3 Frequency = TIM3 counter clock/(ARR + 1)
    TIM3 Frequency = 72 KHz.
    TIM3 Channel1 duty cycle = (TIM3_CCR1/ TIM3_ARR)* 100 = 50%
    TIM3 Channel2 duty cycle = (TIM3_CCR2/ TIM3_ARR)* 100 = 37.5%
    TIM3 Channel3 duty cycle = (TIM3_CCR3/ TIM3_ARR)* 100 = 25%
    TIM3 Channel4 duty cycle = (TIM3_CCR4/ TIM3_ARR)* 100 = 12.5%
  ----------------------------------------------------------------------- */
		//SystemCoreClock = 48MHz , 设置1KHz 的定时器
		//TIM_TimeBaseStructure.TIM_Period = 36-1;
//		TIM_TimeBaseStructure.TIM_Period = SystemCoreClock/1000; //定时时钟 1KHz 
//		TIM_TimeBaseStructure.TIM_Period = 100-1; //10Hz 定时器
//		TIM_TimeBaseStructure.TIM_ClockDivision = 1;
//		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
// 
//		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //配置PWM模式1
//		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
//    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//    //TIM_OCInitStructure.TIM_Pulse = 0;	  //ccr2Val 设置通道2的电平跳变值,输出另外一个占空比的PWM
//		TIM_OC1Init(TIM3, &TIM_OCInitStructure);	 //使能通道1
//    TIM_OC2Init(TIM3, &TIM_OCInitStructure);	  //使能通道2
		
		
		//TIM2 PWM 输出配置
		TIM_TimeBaseStructure.TIM_Period = SystemCoreClock/1000; //定时时钟 1KHz 
		TIM_TimeBaseStructure.TIM_Period = 100-1; //10Hz 定时器
		TIM_TimeBaseStructure.TIM_ClockDivision = 1;
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
 
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //配置PWM模式1
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //当计数器计数值小于ccr1Val时为高电平
    TIM_OC3Init(TIM2, &TIM_OCInitStructure);	 //使能通道1
    TIM_OC4Init(TIM2, &TIM_OCInitStructure);	  //使能通道2
		
		
		//使能定时器预装载寄存器
		//TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
    //TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
		TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
    TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);

		TIM_ARRPreloadConfig(TIM3,ENABLE);
		TIM_Cmd(TIM3, ENABLE);                   //使能定时器3	
		
		TIM_ARRPreloadConfig(TIM2,ENABLE);
		TIM_Cmd(TIM2, ENABLE);                   //使能定时器2	
}

void motorInit(void)
{
    tim3GpioConfig();
    tim3ModeConfig();
    motorStatus(0);
	
		// tim3 ch1/ch2端口输出波形不对，使用TIM3的ch3/ch4输出PWM波形控制电机旋转。
	  //TIM_SetCompare1(TIM3,0);
	  //TIM_SetCompare2(TIM3,0);
	
	TIM_SetCompare3(TIM2,0);
	TIM_SetCompare4(TIM2,0);
}


void motorControl(uint8_t m1,uint8_t m2)
{
    //uint16_t temp = (MOTOR_ARR+1) / MOTOR_MAX;
    //TIM_SetCompare1(TIM3,m1*temp);	//m1
    //TIM_SetCompare2(TIM3,m2*temp);	//m4
	
		TIM_SetCompare3(TIM2,m1);
		TIM_SetCompare4(TIM2,m2);
}

void motorStatus(motor_t status)
{
    if(0 == status)
    {
        motorControl(0,0);
    }
    else
    {
        if(status > 0)
        {
           //motorControl((abs(status)+5)*25, 0);
						motorControl((abs(status))*10, 0);
        }
        else
        {
            //motorControl(0, (abs(status)+5)*25);
						motorControl(0, (abs(status))*10);
        }
    }  
}


