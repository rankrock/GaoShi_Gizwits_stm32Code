/**
  ******************************************************************************
  * @file    IO_Toggle/stm32f0xx_it.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    23-March-2012
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_it.h"
extern uint32_t Timer_Count_Value;
extern void UserKey_Scan(void);
uint32_t keyvalue;
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}
void USART1_IRQHandler(void)
{
		
	 if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
   USART_ClearITPendingBit(USART1, USART_IT_RXNE);            
   }

	
}
//void USART2_IRQHandler(void)
//{
//  if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
//                 USART_ClearITPendingBit(USART2, USART_IT_RXNE);
//               
//         }
//}

///*定时器2中断处理函数*/
//void TIM2_IRQHandler(void)
//{
//   if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
//    {
//        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);/*清除标志位*/	
//			Timer_Count_Value++;
//		UserKey_Scan();
//    }
//}
///*定时器3中断处理函数*/
//void TIM3_IRQHandler(void)
//{
//    if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
//    {
//        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);/*清除标志位*/
//        
//    }
//}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
