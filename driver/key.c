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
/// ����ȷ����ʱ
static void Key_Delay(__IO uint32_t nCount)
{
	for(; nCount != 0; nCount--);
} 

/*****************************************************************************
�������� : Get_Count_Value
�������� : ��ȡTim2ֵ
������� : ��
���ز��� : Time2����ֵ
ʹ��˵�� : ��
*****************************************************************************/
uint32_t Get_Count_Value(void)
{
  return Timer_Count_Value;
}

void Key_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	
	/*���������˿ڣ�PA����ʱ��*/
	RCC_AHBPeriphClockCmd(macKEY1_GPIO_CLK,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = macKEY1_GPIO_PIN; 
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(macKEY1_GPIO_PORT, &GPIO_InitStructure);
	
	
	Key_State[0].Long_Press_Handle = Key1_LongPress_Handle;
  Key_State[0].Short_Press_Handle = Key1_ShortPress_Handle;
 
}

 /*
 * ��������Key_Scan
 * ����  ������Ƿ��а�������
 * ����  ��GPIOx��x ������ A��B��C��D���� E
 *		     GPIO_Pin������ȡ�Ķ˿�λ 	
 * ���  ��KEY_OFF(û���°���)��KEY_ON�����°�����
 */
uint8_t Key_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
{			
	/*����Ƿ��а������� */
	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON )  
	{	 
		/*�ȴ������ͷ� */
		while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON);   
		return 	KEY_ON;	 
	}
	else
		return KEY_OFF;
}
/*********************************************END OF FILE**********************/
/*****************************************************************************
�������� : Get_Key
�������� : ��ȡ����
������� : ��
���ز��� : ReadKey:����ֵ
ʹ��˵�� : ��
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
�������� : Get_Key_Press_Time
�������� : ��ȡ��������ʱ��
������� : last_time:�ϴΰ���ʱ��
���ز��� : 2�ΰ���֮���ֵ
ʹ��˵�� : ��
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
    //Time2���
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
        //�а�������
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
        //һֱ����
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
      //�ɿ�
     time = Get_Key_Press_Time(Key_State[i].Press_Time);
    
      Key_State[i].Press_Time = 0;
      Key_State[i].Status = KEY_NO;
      if((time >= TIME_PRESS_SHORT) && (time < TIME_PRESS_LONG))
      {
        //�̰�
        if(Key_State[i].Short_Press_Handle)
        {
          Key_State[i].Short_Press_Handle();
        }
      }
      else if(time >= TIME_PRESS_LONG)
      {
        //����
        if(Key_State[i].Long_Press_Handle)
        {
          Key_State[i].Long_Press_Handle();
        }
      }
    }
  }
}
