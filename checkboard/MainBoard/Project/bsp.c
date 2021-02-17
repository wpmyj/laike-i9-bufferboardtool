/****************************************************************************************************/
/*			@FileName		:	bsp.c														    	*/
/*			@Author			:	zhengbaiqiu															*/
/*			@Version		:	v1.0																*/
/*			@Date			:	2019.11.12  														*/
/****************************************************************************************************/


/* Includes --------------------------------------------------------------------*/
#include "stm32f10x.h"
#include <stdio.h>
#include <string.h>

/* Project ---------------------------------------------------------------------*/
#include "bsp.h"
//#include "delay.h"
#include "apiLib.h"

/* Variable --------------------------------------------------------------------*/

/* Function --------------------------------------------------------------------*/
void prvSetupHardware(void);
void TIM1_PWM_Configure(u16 arr,u16 pre);
void TIM1_GPIO_Configure(void);



/*****************************************************************************
 * @name       :void NVIC_Configure(void)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :nvic configuration
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void NVIC_Configure(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);            //  16抢断优先级    0响应优先级
}

/*****************************************************************************
 * @name       :u8 HSI_SystemClock(void)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :HSI SystemClock configuration
 * @parameters :None
 * @retvalue   :0:fail    1:pass      
******************************************************************************/
u8 HSI_SystemClock(void)
{
	u8 sta;
	RCC_HSICmd(ENABLE);
	sta=RCC_GetFlagStatus(RCC_FLAG_HSIRDY);
	if(sta==RESET)return 0;
	else
	{
		RCC_PLLConfig(RCC_PLLSource_HSI_Div2,RCC_PLLMul_9);         //PLL36MHz
		RCC_PLLCmd(ENABLE);
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY)==RESET)
		{
		}
		sta=RCC_GetFlagStatus(RCC_FLAG_PLLRDY);
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);                  //SYSCLK36MHz
		
		RCC_HCLKConfig(RCC_SYSCLK_Div1);                            //AHB时钟等于系统时钟                  36M
		RCC_PCLK1Config(RCC_HCLK_Div1);                             //低速AHB时钟APB1等于AHB时钟的一半     36M
		RCC_PCLK2Config(RCC_HCLK_Div1);                             //高速AHB时钟APB2等于AHB时钟           36M
		return 1;
	}
		
}

/*****************************************************************************
 * @name       :void HSE_SystemClock(void)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :HSE SystemClock configuration
 * @parameters :None
 * @retvalue   :None  
******************************************************************************/
void HSE_SystemClock(void)
{
	RCC_DeInit();
	
	RCC_HSEConfig(RCC_HSE_ON);
	HSEStartUpStatus=RCC_WaitForHSEStartUp();
	if(HSEStartUpStatus==0)
	{
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1,RCC_PLLMul_9);               //            72M        
		RCC_PLLCmd(ENABLE);
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY)==RESET)
		{
		}
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		
		RCC_HCLKConfig(RCC_SYSCLK_Div1);         //AHB时钟等于系统时钟                  72M
		RCC_PCLK1Config(RCC_HCLK_Div2);          //低速AHB时钟APB1等于AHB时钟的一半     36M
		RCC_PCLK2Config(RCC_HCLK_Div1);          //高速AHB时钟APB2等于AHB时钟           72M
	}
	
}

/*****************************************************************************
 * @name       :void RCC_Configure(void)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :RCC configuration
 * @parameters :None
 * @retvalue   :None  
******************************************************************************/
void RCC_Configure(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* Enable peripheral clocks ------------------------------------------------*/
	/* Enable AHB clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	/* Enable APB1 clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM3 | RCC_APB1Periph_TIM4 | RCC_APB1Periph_TIM5 |
												 RCC_APB1Periph_TIM6 | RCC_APB1Periph_USART2 | RCC_APB1Periph_USART3 | 
												 RCC_APB1Periph_UART4 | RCC_APB1Periph_UART5 | RCC_APB1Periph_WWDG,ENABLE);
	
	/* Enable APB2 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | 
												  RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | 
												  RCC_APB2Periph_GPIOF | RCC_APB2Periph_GPIOG | 
												  RCC_APB2Periph_AFIO | RCC_APB2Periph_TIM1 | RCC_APB2Periph_USART1, ENABLE);
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	
	/* Enable GPIO */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	GPIO_Init(GPIOG, &GPIO_InitStructure);
}

/*****************************************************************************
 * @name       :void IWDG_Configure(void)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :IWDG configuration
 * @parameters :None
 * @retvalue   :None  
******************************************************************************/
void IWDG_Configure(void)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	IWDG_SetPrescaler(IWDG_Prescaler_256);
	IWDG_SetReload(0xFFF);
	IWDG_Enable();
}

/*****************************************************************************
 * @name       :void Systick_Configure(void)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :Systick configuration
 * @parameters :None
 * @retvalue   :None  
******************************************************************************/
void Systick_Configure(void)                             //1ms
{
	if(SysTick_Config(SystemCoreClock/1000))
	while(1);
}

/*****************************************************************************
 * @name       :void TIM2_Configure(void)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :TIM2 configuration
 * @parameters :None
 * @retvalue   :None  
******************************************************************************/
void TIM2_Configure(void)                                      //1ms interrupt
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	TIM_TimeBaseStructure.TIM_Period =99;
	TIM_TimeBaseStructure.TIM_Prescaler =(720-1);
	TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode =TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel =TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =TIM2_TIME_PRIORITY;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;
	NVIC_InitStructure.NVIC_IRQChannelCmd =ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	
	TIM_Cmd(TIM2,ENABLE);
}

/*****************************************************************************
 * @name       :void TIM3_Configure(void)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :TIM3 configuration
 * @parameters :None
 * @retvalue   :None  
******************************************************************************/
void TIM3_Configure(void)                                //TIME interupt          200us
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	TIM_TimeBaseStructure.TIM_Period =19;
	TIM_TimeBaseStructure.TIM_Prescaler =(720-1);
	TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode =TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel =TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =TIM3_TIME_PRIORITY;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;
	NVIC_InitStructure.NVIC_IRQChannelCmd =ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	
//	TIM_Cmd(TIM3,ENABLE);
	TIM_Cmd(TIM3,DISABLE);
}

/*****************************************************************************
 * @name       :void TIM4_Configure(void)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :TIM3 configuration
 * @parameters :None
 * @retvalue   :None  
******************************************************************************/
void TIM4_Configure(void)                                //TIME interupt          10us
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	TIM_TimeBaseStructure.TIM_Period =9;
	TIM_TimeBaseStructure.TIM_Prescaler =71;
	TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode =TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel =TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =TIM4_TIME_PRIORITY;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;
	NVIC_InitStructure.NVIC_IRQChannelCmd =ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
	
//	TIM_Cmd(TIM4,ENABLE);
	TIM_Cmd(TIM4,DISABLE);
}

/*****************************************************************************
 * @name       :void TIM4_Configure(void)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :TIM3 configuration
 * @parameters :None
 * @retvalue   :None  
******************************************************************************/
void TIM5_Configure(void)                                //TIME interupt          250us
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	TIM_TimeBaseStructure.TIM_Period =25;
	TIM_TimeBaseStructure.TIM_Prescaler =719;
	TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode =TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel =TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =TIM5_TIME_PRIORITY;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;
	NVIC_InitStructure.NVIC_IRQChannelCmd =ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);
	
	TIM_Cmd(TIM5,ENABLE);
//	TIM_Cmd(TIM5,DISABLE);
}

/*****************************************************************************
 * @name       :void GPIO_Configure(void)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :gpio configuration
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void GPIO_Configure(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
// 	EXTI_InitTypeDef   EXTI_InitStructure;
// 	NVIC_InitTypeDef   NVIC_InitStructure;
	
	/******************system configure*******************/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//气泵
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOC, &GPIO_InitStructure);
	PCout(0) = 1;
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;//端口检测
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_6;//气泵
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOE, &GPIO_InitStructure);
	PEout(0) = 0;
	PEout(6) = 1;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_11;//touch_vcc_b touch_vcc_a  引导检测发射控制脚  左免碰撞发射控制脚
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOF, &GPIO_InitStructure);
	PFout(4) = 1;
	PFout(5) = 1;
	PFout(6) = 1;
	PFout(7) = 1;
	PFout(11) = 1;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | 
								  GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;//右免碰撞发射控制脚
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	PBout(1) = 1;
	PBout(3) = 0;
	PBout(4) = 0;
	PBout(5) = 0;
	PBout(6) = 0;
	PBout(7) = 0;
	PBout(8) = 0;
	PBout(9) = 0;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;//气泵
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOG, &GPIO_InitStructure);
	PGout(13) = 0;
	PGout(14) = 0;
	PGout(15) = 0;
	
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_15;//充电控制
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	PAout(11) = 0;
	PAout(12) = 0;
	PAout(15) = 0;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;//引导检测
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
// 	/* Connect EXTI8 Line to PA.8 pin */
// 	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource8);

// 	/* Configure EXTI8 line */
// 	EXTI_InitStructure.EXTI_Line = EXTI_Line8;
// 	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
// 	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
// 	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
// 	EXTI_Init(&EXTI_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;//引导检测
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
// 	/* Connect EXTI9 Line to PC.9 pin */
// 	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource9);

// 	/* Configure EXTI9 line */
// 	EXTI_InitStructure.EXTI_Line = EXTI_Line9;
// 	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
// 	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
// 	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
// 	EXTI_Init(&EXTI_InitStructure);

// 	/* Enable and set EXTI9_5 Interrupt to the lowest priority */
// 	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
// 	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
// 	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
// 	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
// 	NVIC_Init(&NVIC_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8;//免碰撞检测接收
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
//  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_8;//tof power reset
//  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  	GPIO_Init(GPIOA, &GPIO_InitStructure);
// 	
// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_15;//充电控制
//  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  	GPIO_Init(GPIOA, &GPIO_InitStructure);
// 	PAout(11) = 0;
// 	PAout(12) = 0;
// 	PAout(15) = 0;
// 	
// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;//右沿边接收
//  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  	GPIO_Init(GPIOB, &GPIO_InitStructure);
// 	PBout(1) = 1;
// 	
// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;//免碰撞B信号 免碰撞A信号 沿边B信号 沿边A信号
//  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  	GPIO_Init(GPIOC, &GPIO_InitStructure);
// 	
// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;//免碰撞发射左、右、前
//  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  	GPIO_Init(GPIOC, &GPIO_InitStructure);
// 	PCout(0) = 1;
// 	PCout(1) = 1;
// 	PCout(2) = 1;
// 	PCout(3) = 1;
// 	PCout(4) = 1;
// 	PCout(5) = 1;
// 	
// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_7;//水箱控制 放电控制 吸尘电机控制 滚刷控制  陀螺仪继电器
//  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  	GPIO_Init(GPIOD, &GPIO_InitStructure);
// 	PDout(0) = 0;
// 	PDout(1) = 0;
// 	PDout(3) = 0;
// 	PDout(4) = 0;
// 	PDout(7) = 0;
// 	
// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | //右、左行走轮检测
// 								  GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;//右、左码盘供电检测 右、左碰撞供电、沿边供电
//  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  	GPIO_Init(GPIOE, &GPIO_InitStructure);
// 	
// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;//尘盒检测 开机键检测 离地右、左  碰撞右、左发射
//  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  	GPIO_Init(GPIOE, &GPIO_InitStructure);
// 	PEout(0) = 1;
// 	PEout(1) = 0;
// 	PEout(3) = 1;
// 	PEout(4) = 1;
// 	PEout(5) = 1;
// 	PEout(6) = 1;
// 	
// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_9;//尘盒供电   按键
//  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  	GPIO_Init(GPIOF, &GPIO_InitStructure);
// 	
// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | 
// 								  GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_10 | GPIO_Pin_11;//
//  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  	GPIO_Init(GPIOF, &GPIO_InitStructure);
// 	PFout(1) = 1;
// 	PFout(2) = 1;
// 	PFout(3) = 0;
// 	PFout(4) = 0;
// 	PFout(5) = 0;
// 	PFout(6) = 0;
// 	PFout(7) = 0;
// 	PFout(8) = 1;
// 	PFout(10) = 1;
// 	PFout(11) = 1;
// 	
// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;//key3\2\1控制
//  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  	GPIO_Init(GPIOG, &GPIO_InitStructure);
// 	PGout(13) = 0;
// 	PGout(14) = 0;
// 	PGout(15) = 0;
// 	
// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8;//灰尘供电检测 充电检测  BAT+  吸尘电机发射波形检测  后、前、右、左下视波形检测脚
//  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  	GPIO_Init(GPIOG, &GPIO_InitStructure);
// 	
// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
// 	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/*****************************************************************************
 * @name       :void UART1_Configure(void)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :uart1 configure
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void UART1_Configure(void)//主板
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* USART1_TX */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    /* USART1_RX */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate =115200;
	USART_InitStructure.USART_WordLength =USART_WordLength_8b;
	USART_InitStructure.USART_StopBits =USART_StopBits_1;
	USART_InitStructure.USART_Parity =USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl =USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode =USART_Mode_Rx|USART_Mode_Tx;
	USART_Init(USART1,&USART_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel =USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =UART1_MAINBOARD_PRIORITY;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;
	NVIC_InitStructure.NVIC_IRQChannelCmd =ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	
	USART_Cmd(USART1,ENABLE);
}

/*****************************************************************************
 * @name       :void UART3_Configure(void)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :uart3 configure
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void UART3_Configure(void)//大核串口
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* USART3_TX */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    /* USART3_RX */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate =115200;
	USART_InitStructure.USART_WordLength =USART_WordLength_8b;
	USART_InitStructure.USART_StopBits =USART_StopBits_1;
	USART_InitStructure.USART_Parity =USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl =USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode =USART_Mode_Rx|USART_Mode_Tx;
	USART_Init(USART3,&USART_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel =USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =UART3_BIGBOARD_PRIORITY;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;
	NVIC_InitStructure.NVIC_IRQChannelCmd =ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
	
	USART_Cmd(USART3,ENABLE);
}

/*****************************************************************************
 * @name       :void UART4_Configure(void)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :gpio configuration
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void UART4_Configure(void)//DEBUG
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* USART4_TX */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    /* USART4_RX */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate =115200;
	USART_InitStructure.USART_WordLength =USART_WordLength_8b;
	USART_InitStructure.USART_StopBits =USART_StopBits_1;
	USART_InitStructure.USART_Parity =USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl =USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode =USART_Mode_Rx|USART_Mode_Tx;
	USART_Init(UART4,&USART_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel =UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =UART4_DEBUG_PRIORITY;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;
	NVIC_InitStructure.NVIC_IRQChannelCmd =ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_ITConfig(UART4,USART_IT_RXNE,ENABLE);
	
	USART_Cmd(UART4,ENABLE);
}

/*****************************************************************************
 * @name       :void UART5_Configure(void)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :gpio configuration
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void UART5_Configure(void)//显示
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* USART5_TX */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    /* USART5_RX */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate =115200;
	USART_InitStructure.USART_WordLength =USART_WordLength_8b;
	USART_InitStructure.USART_StopBits =USART_StopBits_1;
	USART_InitStructure.USART_Parity =USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl =USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode =USART_Mode_Rx|USART_Mode_Tx;
	USART_Init(UART5,&USART_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel =UART5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =UART5_DISPLAY_PRIORITY;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;
	NVIC_InitStructure.NVIC_IRQChannelCmd =ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_ITConfig(UART5,USART_IT_RXNE,ENABLE);
	
	USART_Cmd(UART5,ENABLE);
}

/*****************************************************************************
 * @name       :void ADC1_Configure(void)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :ADC1 configuration
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void ADC1_Configure(void)//TOF电源检测
{
	GPIO_InitTypeDef GPIO_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div4);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE );
	
 	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | 
								GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;          
 	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_AIN;
 	GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA,&GPIO_InitStructure);

	/* DMA1 channel1 configuration ----------------------------------------------*/
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&ADC1->DR);
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADCValue;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = 8;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//缓存地址是否递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);

	/* Enable DMA1 channel1 */
	DMA_Cmd(DMA1_Channel1, ENABLE);

	/* ADC1 configuration ------------------------------------------------------*/
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 8;
	ADC_Init(ADC1, &ADC_InitStructure);

	/* ADC1 regular channel14 configuration */ 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 5, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 6, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 7, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 8, ADC_SampleTime_55Cycles5);

	/* Enable ADC1 DMA */
	ADC_DMACmd(ADC1, ENABLE);

	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);

	/* Enable ADC1 reset calibration register */   
	ADC_ResetCalibration(ADC1);
	/* Check the end of ADC1 reset calibration register */
	while(ADC_GetResetCalibrationStatus(ADC1));

	/* Start ADC1 calibration */
	ADC_StartCalibration(ADC1);
	/* Check the end of ADC1 calibration */
	while(ADC_GetCalibrationStatus(ADC1));
	 
	/* Start ADC1 Software Conversion */ 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

/*****************************************************************************
 * @name       :void PWM_Configure(void)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :pwm configuration
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void TIM1_PWM_Configure(u16 arr,u16 pre)//38k载波 arr = 26  pre = 71   免碰撞500hz 10%占空比  arr = 200  pre = 719
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM1, ENABLE);     //   TIM1 部分重映射
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* Time Base configuration */
	TIM_TimeBaseStructure.TIM_Period =arr;                           //    重载周期值     0x0000-0xffff
	TIM_TimeBaseStructure.TIM_Prescaler =pre;                 //    分频值         0x0000-0xffff
	TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode =TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0; //重复计数
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);
	
	/* Channel 1, 2,3 and 4 Configuration in PWM mode */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;      //  起始高电平时   PWM1 : 定时器小于比较器设定值输出高  PWM2 : 定时器小于比较器设定值输出低
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;              //   输出极性   起始电平为高电平
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);	
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
	
	/* TIM1 counter enable */
	TIM_Cmd(TIM1, ENABLE);

	/* TIM1 Main Output Enable */
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
}

/*****************************************************************************
 * @name       :void TIM1_GPIO_Configure(void)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :gpio configuration
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void TIM1_GPIO_Configure(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//右免碰撞发射控制脚
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	PBout(0) = 0;
}

/*****************************************************************************
 * @name       :void BSP_Init(void)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :Chip configuration
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void BSP_Init(void)
{
	NVIC_Configure();
//	HSE_SystemClock();
//	HSI_SystemClock();
	RCC_Configure();
	IWDG_Configure();
	GPIO_Configure();
	TIM2_Configure();
	TIM3_Configure();
	TIM5_Configure();
 	ADC1_Configure();
 	TIM1_PWM_Configure(26,71);
	UART1_Configure();
//	UART3_Configure();
	UART4_Configure();
	UART5_Configure();
}

/*****************************************************************************
 * @name       :void Param_Init(void)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :Parameter initialization
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Param_Init(void)
{
	//    时间相关
	mMaininf.mSystem.mSystemTick = 0;
	mMaininf.mSystem.mPowerOnStartTime = 0;
	
// 	mMaininf.mSystem.mSystem200usTick = 0;
// 	mMaininf.mSystem.mSystem10usTick = 0;
	mMaininf.mSystem.mGuideReceiveTime = 0;
	mMaininf.mSystem.mGuideCheckTime = 0;
	mMaininf.mSystem.mSystem200usTick = 0;
	mMaininf.mSystem.mDisplayStartTime = 0;
	
	//    串口相关
	//UART1
	memset(mMaininf.mUart1.TxBuffer, 0, sizeof(mMaininf.mUart1.TxBuffer));     //  int arr[10]={0};
	memset(mMaininf.mUart1.RxBuffer, 0, sizeof(mMaininf.mUart1.RxBuffer));
	memset(mMaininf.mUart1.SendBuf, 0, sizeof(mMaininf.mUart1.SendBuf));
	memset(mMaininf.mUart1.ReceiveBuf, 0, sizeof(mMaininf.mUart1.ReceiveBuf));
	
	mMaininf.mUart1.mSendSize = 0;
	mMaininf.mUart1.mSendCont = 0;
	mMaininf.mUart1.mReceiveStep = ST_0x55;
	mMaininf.mUart1.mReceiveBCC = 0;
	mMaininf.mUart1.mReceiveSize = 0;
	mMaininf.mUart1.mReceiveCont = 0;
	mMaininf.mUart1.mReceiveTimeOut = 0;
	mMaininf.mUart1.mReceiveFlag = 0;      //    没有接收导数据
	//UART4
	memset(mMaininf.mUart4.TxBuffer, 0, sizeof(mMaininf.mUart4.TxBuffer));     //  int arr[10]={0};
	memset(mMaininf.mUart4.RxBuffer, 0, sizeof(mMaininf.mUart4.RxBuffer));
	memset(mMaininf.mUart4.SendBuf, 0, sizeof(mMaininf.mUart4.SendBuf));
	memset(mMaininf.mUart4.ReceiveBuf, 0, sizeof(mMaininf.mUart4.ReceiveBuf));
	
	mMaininf.mUart4.mSendSize = 0;
	mMaininf.mUart4.mSendCont = 0;
	mMaininf.mUart4.mReceiveStep = ST_0x55;
	mMaininf.mUart4.mReceiveBCC = 0;
	mMaininf.mUart4.mReceiveSize = 0;
	mMaininf.mUart4.mReceiveCont = 0;
	mMaininf.mUart4.mReceiveTimeOut = 0;
	mMaininf.mUart4.mReceiveFlag = 0;      //    没有接收导数据
	//UART5
	memset(mMaininf.mUart5.TxBuffer, 0, sizeof(mMaininf.mUart5.TxBuffer));     //  int arr[10]={0};
	memset(mMaininf.mUart5.RxBuffer, 0, sizeof(mMaininf.mUart5.RxBuffer));
	memset(mMaininf.mUart5.SendBuf, 0, sizeof(mMaininf.mUart5.SendBuf));
	memset(mMaininf.mUart5.ReceiveBuf, 0, sizeof(mMaininf.mUart5.ReceiveBuf));
	
	mMaininf.mUart5.mSendSize = 0;
	mMaininf.mUart5.mSendCont = 0;
	mMaininf.mUart5.mReceiveStep = ST_0x55;
	mMaininf.mUart5.mReceiveBCC = 0;
	mMaininf.mUart5.mReceiveSize = 0;
	mMaininf.mUart5.mReceiveCont = 0;
	mMaininf.mUart5.mReceiveTimeOut = 0;
	mMaininf.mUart5.mReceiveFlag = 0;      //    没有接收导数据
	
	
	//    工作相关
	memset(&mMaininf.mWork, 0, sizeof(mMaininf.mWork));
	
// 	mMaininf.mWork.mWorkInitFlag = 0;
// 	mMaininf.mWork.mWorkChecking = 0;
// 	mMaininf.mWork.mWorkCheckStatus = 0;
// 	
// 	mMaininf.mWork.mWorkStep = 0;
// 	mMaininf.mWork.mWorkStepOld = 0;
// 	mMaininf.mWork.mWorkFileStep = 0;
// 	
// 	mMaininf.mWork.mWorkTouchStep = 0;
// 	
// 	mMaininf.mWork.mWorkGuideSendFlag = 0;
// 	mMaininf.mWork.mWorkGuideSendCont = 0;
// 	mMaininf.mWork.mWorkGuideReceiveFlag = 0;
// 	mMaininf.mWork.mWorkGuideFirstReceiveFlag = 0;
// 	mMaininf.mWork.mWorkGuideReceiveSuccessFlag = 0;
// 	
//  	mMaininf.mWork.mWorkKeyPressFlag = 0;
// 	mMaininf.mWork.mWorkKeyPressFlag1 = 0;
// 	mMaininf.mWork.mWorkKeyCont = 0;
// 	
// 	mMaininf.mWork.mWorkThingHight = 0;
// 	
// 	mMaininf.mWork.mWorkLedStatus[0] = 0;
// 	mMaininf.mWork.mWorkLedTime = 0;
// 	mMaininf.mWork.mWorkLedCont = 0;
// 	
// 	mMaininf.mWork.mWorkTestSelectFlag = 0;
// 	mMaininf.mWork.mWorkCheckErrorFlag = 0;
// 	mMaininf.mWork.mWorkCheck8Or9Flag = 0;
}

/*****************************************************************************
 * @name       :void prvSetupHardware(void)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :Initialize the chip
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void prvSetupHardware(void)
{
	BSP_Init();
	Param_Init();
}



// /*--------------------------------------------------------------------------

//         打印log

// ---------------------------------------------------------------------------*/



// /*-----------------------------------------------------------------------*/
// /* UART1串口设置                                                         */
// /*-----------------------------------------------------------------------*/


// void UART1_Configure(void)
// {
// 	GPIO_InitTypeDef GPIO_InitStructure;
// 	USART_InitTypeDef USART_InitStructure;
// 	
// 	/* USART1_TX */
//     GPIO_InitStructure.GPIO_Pin = DEBUG_UART_TX_PIN;
//     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
// 	GPIO_Init(DEBUG_UART_GPIO, &GPIO_InitStructure);
//     
//     /* USART1_RX */
//     GPIO_InitStructure.GPIO_Pin = DEBUG_UART_RX_PIN;
//     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//     GPIO_Init(DEBUG_UART_GPIO, &GPIO_InitStructure);
// 	
// 	USART_InitStructure.USART_BaudRate =115200;
// 	USART_InitStructure.USART_WordLength =USART_WordLength_8b;
// 	USART_InitStructure.USART_StopBits =USART_StopBits_1;
// 	USART_InitStructure.USART_Parity =USART_Parity_No;
// 	USART_InitStructure.USART_HardwareFlowControl =USART_HardwareFlowControl_None;
// 	USART_InitStructure.USART_Mode =USART_Mode_Rx|USART_Mode_Tx;
// 	USART_Init(USART1,&USART_InitStructure);
// 	
// 	USART_Cmd(USART1,ENABLE);
// }


// int fputc(int ch, FILE *f)
// {
//   USART_SendData(USART1, (uint8_t) ch);
//   /* Loop until the end of transmission */
//   while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
//   {}
//   return ch;
// }

// int fgetc(FILE *f)
// {
//   
//    while(!(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET))
//    {
//    }
//   
//     
//    return (USART_ReceiveData(USART1));
// }

// void assert_failed(u8* file, u32 line)
// {
//     /* User can add his own implementation to report the file name and line number,
//      ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
//     /* Infinite loop */
//     printf("assert failed at %s:%d \n", file, line);
//     while (1) {
//     }
// }

// void delay(uint32_t nCount)
// {
//   for(; nCount!= 0;nCount--);
// }










