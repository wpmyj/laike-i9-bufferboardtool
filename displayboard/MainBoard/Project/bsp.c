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
#include "spi.h"
#include "lcd_driver.h"	
#include "touch.h"
#include "apiLib.h"
#include "delay.h"

/* Variable --------------------------------------------------------------------*/

/* Function --------------------------------------------------------------------*/
void prvSetupHardware(void);




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
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM3 | RCC_APB1Periph_TIM4 | 
												 RCC_APB1Periph_TIM6 | RCC_APB1Periph_USART2 | RCC_APB1Periph_USART3 | 
												 RCC_APB1Periph_UART4 | RCC_APB1Periph_UART5 | RCC_APB1Periph_WWDG,ENABLE);
	
	/* Enable APB2 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | 
												  RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE |
												  RCC_APB2Periph_AFIO | RCC_APB2Periph_TIM1 | 
												  RCC_APB2Periph_SPI1 | RCC_APB2Periph_USART1, ENABLE);
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	
	/* Enable GPIO */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//充电控制
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	PAout(0) = 1;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOD, &GPIO_InitStructure);
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
	TIM_TimeBaseInitTypeDef TIM_TimeBase_Soure;
	NVIC_InitTypeDef NVIC_Soure;
	
	TIM_TimeBase_Soure.TIM_Period =99;
	TIM_TimeBase_Soure.TIM_Prescaler =(720-1);
	TIM_TimeBase_Soure.TIM_ClockDivision =TIM_CKD_DIV1;
	TIM_TimeBase_Soure.TIM_CounterMode =TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBase_Soure);
	
	NVIC_Soure.NVIC_IRQChannel =TIM2_IRQn;
	NVIC_Soure.NVIC_IRQChannelPreemptionPriority =2;
	NVIC_Soure.NVIC_IRQChannelSubPriority =0;
	NVIC_Soure.NVIC_IRQChannelCmd =ENABLE;
	NVIC_Init(&NVIC_Soure);
	
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
void TIM3_Configure(void)                                //TIME interupt          1ms 
{
	TIM_TimeBaseInitTypeDef TIM_TimeBase_Soure;
	NVIC_InitTypeDef NVIC_Soure;
	
	TIM_TimeBase_Soure.TIM_Period =9;
	TIM_TimeBase_Soure.TIM_Prescaler =(7200-1);
	TIM_TimeBase_Soure.TIM_ClockDivision =TIM_CKD_DIV1;
	TIM_TimeBase_Soure.TIM_CounterMode =TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBase_Soure);
	
	NVIC_Soure.NVIC_IRQChannel =TIM3_IRQn;
	NVIC_Soure.NVIC_IRQChannelPreemptionPriority =3;
	NVIC_Soure.NVIC_IRQChannelSubPriority =0;
	NVIC_Soure.NVIC_IRQChannelCmd =ENABLE;
	NVIC_Init(&NVIC_Soure);
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	
	TIM_Cmd(TIM3,ENABLE);
	
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
// 	GPIO_InitTypeDef GPIO_InitStructure;

// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
// 	GPIO_Init(GPIOA, &GPIO_InitStructure);
// 	GPIO_Init(GPIOB, &GPIO_InitStructure);
// 	GPIO_Init(GPIOC, &GPIO_InitStructure);
// 	GPIO_Init(GPIOD, &GPIO_InitStructure);
// 	
// 	
// 	
	/******************system run sd*******************/
	
}

/*****************************************************************************
 * @name       :void UART1_Configure(void)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :uart1 configure
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void UART1_Configure(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* USART1_TX */
    GPIO_InitStructure.GPIO_Pin = DEBUG_UART_TX_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(DEBUG_UART_GPIO, &GPIO_InitStructure);
    
    /* USART1_RX */
    GPIO_InitStructure.GPIO_Pin = DEBUG_UART_RX_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(DEBUG_UART_GPIO, &GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate =115200;
	USART_InitStructure.USART_WordLength =USART_WordLength_8b;
	USART_InitStructure.USART_StopBits =USART_StopBits_1;
	USART_InitStructure.USART_Parity =USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl =USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode =USART_Mode_Rx|USART_Mode_Tx;
	USART_Init(USART1,&USART_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel =USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;
	NVIC_InitStructure.NVIC_IRQChannelCmd =ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	
	USART_Cmd(USART1,ENABLE);
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
	UART1_Configure();
	
	delay_init(72);
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
	//时间相关
	mMaininf.mSystem.mSystemTick = 0;
	mMaininf.mSystem.mPowerOnStartTime = 0;
	mMaininf.mSystem.mReadUartStatusStartTime = 0;
	
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
	
	//模式相关
	mMaininf.mMode.mKeyPressFlag = 0;
	mMaininf.mMode.mScreenPressFlag = 0;
	mMaininf.mMode.mModePage = 0;
	
	//工作相关
	mMaininf.mWork.mWorkMODE = 0;
	mMaininf.mWork.mWorkCMD = 0;
	mMaininf.mWork.mWorkSTATUS = 0;
	
	mMaininf.mWork.mWorkInitFlag = 0;
	mMaininf.mWork.mWorkChecking = 0;
	
	mMaininf.mWork.mWorkKeyCont = 0;
	
	mMaininf.mWork.mWorkPageFlag = 0;
	
	mMaininf.mWork.mWorkFileCont = 0;
	
	mMaininf.mWork.mWorkAvoidBumpDat1 = 0;
	mMaininf.mWork.mWorkAvoidBumpDat2 = 0;
	mMaininf.mWork.mWorkAvoidBumpErrorFlag = 0;
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
	LCD_Init();
	TP_Init();
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










