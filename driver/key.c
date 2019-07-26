#include "bsp_key.h"

extern uint32_t Get_Count_Value(void);

extern uint8_t shortkey;
 uint8_t clearlcd;
uint32_t Timer_Count_Value = 0;
typedef void (* KEY_HANDLE)(void);
typedef struct
{
  uint8_t Status;
  uint32_t Press_Time;
  KEY_HANDLE Short_Press_Handle;
  KEY_HANDLE Long_Press_Handle;
} KEY_STATE_T;
static KEY_STATE_T Key_State[2];
/// 不精确的延时
static void Key_Delay(__IO uint32_t nCount)
{
	for(; nCount != 0; nCount--);
} 

/*****************************************************************************
函数名称 : Get_Count_Value
功能描述 : 获取Tim2值
输入参数 : 无
返回参数 : Time2计数值
使用说明 : 无
*****************************************************************************/
uint32_t Get_Count_Value(void)
{
  return Timer_Count_Value;
}

void Key_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	
	/*开启按键端口（PA）的时钟*/
	RCC_AHBPeriphClockCmd(macKEY1_GPIO_CLK,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = macKEY1_GPIO_PIN; 
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(macKEY1_GPIO_PORT, &GPIO_InitStructure);
	
	
	Key_State[0].Long_Press_Handle = Key1_LongPress_Handle;
  Key_State[0].Short_Press_Handle = Key1_ShortPress_Handle;
 
}

 /*
 * 函数名：Key_Scan
 * 描述  ：检测是否有按键按下
 * 输入  ：GPIOx：x 可以是 A，B，C，D或者 E
 *		     GPIO_Pin：待读取的端口位 	
 * 输出  ：KEY_OFF(没按下按键)、KEY_ON（按下按键）
 */
uint8_t Key_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
{			
	/*检测是否有按键按下 */
	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON )  
	{	 
		/*等待按键释放 */
		while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON);   
		return 	KEY_ON;	 
	}
	else
		return KEY_OFF;
}
/*********************************************END OF FILE**********************/
/*****************************************************************************
函数名称 : Get_Key
功能描述 : 读取按键
输入参数 : 无
返回参数 : ReadKey:按键值
使用说明 : 无
*****************************************************************************/
static uint8_t Get_Key(void) 
{
  uint8_t ReadKey = 0;
  
  if(!(GPIO_ReadInputDataBit(macKEY1_GPIO_PORT,macKEY1_GPIO_PIN)))
  {
    ReadKey |= PRESS_KEY1;
  }
  
  return ReadKey;
}
/*****************************************************************************
函数名称 : Get_Key_Press_Time
功能描述 : 获取按键按下时间
输入参数 : last_time:上次按下时间
返回参数 : 2次按键之间差值
使用说明 : 无
*****************************************************************************/
static uint32_t Get_Key_Press_Time(uint32_t last_time)
{
  uint32_t time;
  
  time = Get_Count_Value();
  if(time >= last_time)
  {
    time -= last_time;
  }
  else
  {
    //Time2溢出
    time += ~last_time;
  } 

  return time;
}

	uint8_t key;
void UserKey_Scan(void)
{
  uint8_t i;

	uint32_t time;
  
  key = Get_Key();
  for(i = 0; i < MAX_KEY; i ++)
  {
    switch(Key_State[i].Status)
    {
      case KEY_NO:
        //有按键按下
        if((key >> i) & 0x01)
        {
          Key_State[i].Status = KEY_DOWN;
          Key_State[i].Press_Time = Get_Count_Value();
					//test = Get_Count_Value();
        }
        break;
        
      case KEY_DOWN:
        if(((key >> i) & 0x01) == 0)
        {
          Key_State[i].Status = KEY_UP;
        }
        break;
                
      case KEY_LONG:
        if(((key >> i) & 0x01) == 0)
        {
          Key_State[i].Press_Time = 0;
          Key_State[i].Status = KEY_NO;
        }
        break;  
    }

    if((Key_State[i].Status == KEY_DOWN) || (Key_State[i].Status == KEY_LONG))
    {
      time = Get_Key_Press_Time(Key_State[i].Press_Time);
      
			if(time >= TIME_PRESS_LONG)
      {
        //一直长按
        Key_State[i].Press_Time = Get_Count_Value();
        Key_State[i].Status = KEY_LONG;
        if(Key_State[i].Long_Press_Handle)
        {
          Key_State[i].Long_Press_Handle();
        }
      }          
    }
    else if(Key_State[i].Status == KEY_UP)
    {
      //松开
     time = Get_Key_Press_Time(Key_State[i].Press_Time);
    
      Key_State[i].Press_Time = 0;
      Key_State[i].Status = KEY_NO;
      if((time >= TIME_PRESS_SHORT) && (time < TIME_PRESS_LONG))
      {
        //短按
        if(Key_State[i].Short_Press_Handle)
        {
          Key_State[i].Short_Press_Handle();
        }
      }
      else if(time >= TIME_PRESS_LONG)
      {
        //长按
        if(Key_State[i].Long_Press_Handle)
        {
          Key_State[i].Long_Press_Handle();
        }
      }
    }
  }
}
