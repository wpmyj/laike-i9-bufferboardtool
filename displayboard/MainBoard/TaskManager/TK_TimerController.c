/****************************************************************************************************/
/*			@FileName		:	TK_TIMERCONTROLLER.c											  	*/
/*			@Author			:	zhengbaiqiu															*/
/*			@Version		:	v1.0																*/
/*			@Date			:	2019.11.12  														*/
/****************************************************************************************************/


/* Includes --------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include <stdio.h>
#include <stdlib.h>
#include "string.h"

/* FreeRTOS --------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "FreeRTOSConfig.h"
//#include "death.h"
#include "semphr.h"
#include "projdefs.h"

/* Project ---------------------------------------------------------------------*/
#include "maindef.h"
#include "bsp.h"
#include "gpioredef.h"
#include "touch.h"

#include "apiLib.h"

#include "TK_TimerController.h"


/* Variable --------------------------------------------------------------------*/
u8 smgduan[16]={0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07,
             0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};

/* Function --------------------------------------------------------------------*/
void TK_TimerController( void *pvParameters );
void TimerDetection(void);


/*****************************************************************************
 * @name       :void TK_TimerController( void *pvParameters )
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :时间控制任务
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void TK_TimerController( void *pvParameters )
{
// 	u8 mDelay;
// 	u32 mTickTime;
	
	for(;;)
	{
// 		/*    获取当前时刻     */
// 		mTickTime = api_GetCurrentSystemTime();
		
		/*    定时复位看门狗   */
		IWDG_ReloadCounter();
		
// 		/*    定时检测外设     */
 		TimerDetection();
// 		
// 		/*    获取运行时间     */
// 		if(api_GetSystemTimePeriod(mTickTime) < 10) mDelay = 10 - api_GetSystemTimePeriod(mTickTime);
// 		else mDelay = TIMEOUT_0ms;
		
// 		if(PAout(0) == 0)
// 		{
// 			PAout(0) = 1;
// 		}
// 		else
// 		{
// 			PAout(0) = 0;
// 		}
		
		/*    进行任务延时     */
		vTaskDelay(50);
	}
}

/*****************************************************************************
 * @name       :void TimerDetection(void)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :按键检测
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void TimerDetection(void)
{
//	Read_Touch();
	
// 	if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_11) == 0)
// 	{
// 		mMaininf.mWork.mWorkKeyCont ++;
// 		if(mMaininf.mWork.mWorkKeyCont > 3)
// 		{
// 			mMaininf.mMode.mScreenPressFlag = 1;
// 		}
// 	}
// 	else
// 	{
// 		mMaininf.mWork.mWorkKeyCont = 0;
// 	}
}








