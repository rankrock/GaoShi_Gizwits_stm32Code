/*********************************************************************************
 * 
 * 描    述：主控MCU stm32f030c8t6,侵权必究
 * 作    者：https://item.taobao.com/item.htm?spm=a1z38n.10677092.0.0.79311debo4Xft6&id=566321791377
 * 版    本：1.0
**********************************************************************************/

#include "time.h"
#include "stm32f0xx.h"
/*******************************************************************************
* Function Name  : RCC_TIME_Config
* Description    : TIME的中断优先级配置(注意：如果中断优先级没有设置则中断不会响应)
* Input          : None
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
static void RCC_TIME_Config(void)
{
#ifdef  TIM_1
    RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB, ENABLE);
    /* TIM1 clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 , ENABLE);
#endif

#ifdef  TIM_2
    /* TIM2 clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
#endif

#ifdef  TIM_3
    /* TIM3 clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
#endif  
  
}
/*******************************************************************************
* Function Name  : NVIC_TIME_Config
* Description    : TIME的中断优先级配置(注意：如果中断优先级没有设置则中断不会响应)
* Input          : None
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
static void NVIC_TIME_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
#ifdef  TIM_1
    /*待添加*/
#endif

#ifdef  TIM_2
    /* Enable the TIM2 gloabal Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
#endif

#ifdef  TIM_3
    /* Enable the TIM3 gloabal Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
#endif
}

/*******************************************************************************
* Function Name  : TIM_Config
* Description    : 定时器初始化
* Input          : None
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
void TIM_Config(void)
{
//    uint32_t TimerPeriod = 0;
//    uint32_t Channel1Pulse = 0;

    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//    TIM_OCInitTypeDef  TIM_OCInitStructure;
//    GPIO_InitTypeDef GPIO_InitStructure; 
    
    RCC_TIME_Config();
    NVIC_TIME_Config();
    
#ifdef  TIM_1
    TIM_DeInit(TIM1);
    
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_2);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    TimerPeriod = ((SystemCoreClock/800) *1.004/1 ) - 1;
    /* Compute CCR1 value to generate a duty cycle at 50% for channel 1 and 1N */
    Channel1Pulse = (uint32_t) (( (TimerPeriod - 1)) / 2);

    /* Time Base configuration */
    TIM_TimeBaseStructure.TIM_Prescaler = 799;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = TimerPeriod;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 1;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
    
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
    TIM_OCInitStructure.TIM_Pulse = Channel1Pulse;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

    TIM_OC1Init(TIM1, &TIM_OCInitStructure);

    /* TIM1 counter enable */
    TIM_Cmd(TIM1, ENABLE);

    /* TIM1 Main Output Enable */
    TIM_CtrlPWMOutputs(TIM1, ENABLE);
#endif



#ifdef  TIM_2
    TIM_DeInit(TIM2);
   
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure); 
    TIM_TimeBaseStructure.TIM_Period = (360 - 1);                     //设置时钟周期 32位  100*50=5000us=5ms
    TIM_TimeBaseStructure.TIM_Prescaler = (200 - 1); 
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;               //时钟分频  16位
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   //计数器模式
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    
    TIM_Cmd(TIM2, ENABLE);                                      //启动定时器
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); 
#endif

#ifdef  TIM_3
    TIM_DeInit(TIM3);
    //1000US*(3+1)=4000US
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure); 
    TIM_TimeBaseStructure.TIM_Period = 4000-1;                        //设置时钟周期 32位  100ms
    TIM_TimeBaseStructure.TIM_Prescaler = (uint16_t) (SystemCoreClock / 20000) - 1; // 1/20000=50us ;   //TIM时钟预分频值 16位   8M/80=100K ==10US
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;               //时钟分频  16位
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   //计数器模式
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
    
   TIM_Cmd(TIM3, ENABLE);                                      //启动定时器
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); 
#endif
}


