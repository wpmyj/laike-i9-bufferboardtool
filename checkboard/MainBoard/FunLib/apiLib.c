/****************************************************************************************************/
/*			@FileName		:	APILIB.c														  	*/
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
#include "math.h"

/* FreeRTOS --------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "portmacro.h"
#include "projdefs.h"
#include "task.h"
#include "queue.h"
#include "event_groups.h"
#include "semphr.h"

/* Project ---------------------------------------------------------------------*/
#include "maindef.h"
#include "bsp.h"
#include "gpioredef.h"

#include "apiLib.h"

#include "TK_TimerController.h"


/* Variable --------------------------------------------------------------------*/

/* Function --------------------------------------------------------------------*/
/*  时间相关控制库                                                  */
u32 api_GetSystemTimePeriod(u32 StartTime);
u32 api_GetCurrentSystemTime(void);
void api_UART1_MainBoard_SendCMDData(u8 seq,u8 mode,u8 cmd);
void api_UART1_ReceiveData(u8 vData);
void api_UART4_DEBUG_SendCMDData(u8 seq,u8 mode,u8 cmd);
void api_UART4_ReceiveData(u8 vData);
void api_UART5_Display_SendCMDData(u8 mode,u8 cmd,u8 status,u16 dat1,u16 dat2,u8 flag);
void api_UART5_ReceiveData(u8 vData);
void api_MachinePowerOn(void);
u8 api_ReadGpioInputStauts(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,u8 Val,uint8_t Cont);
u8 api_Check_PWM(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,u8 cycle,u8 differ);
u8 api_Test_ADC(__IO uint16_t *adcval,uint16_t detectval,uint16_t diffval);
void api_Send_Signal(u8 mCmd,u8 OpenFlag);
void api_Control_Motor(u8 mCmd,u8 OpenFlag);
void api_Led_Status_Set(u8 led,u8 status);
void api_Led_Control(u8 Led,u8 OpenFlag);
void api_UART1_SendData(u16 vCont, u8 *pData);
void api_UART4_SendData(u16 vCont, u8 *pData);
void api_ShortToStr(u8* str,u16 data);



/*--------------------------------------------------------------------------------

        时间相关库函数

---------------------------------------------------------------------------------*/

/*****************************************************************************
 * @name       :u32 api_GetSystemTimePeriod(u32 StartTime)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :返回此时刻与开始时刻的时间间隔
 * @parameters :开始时间
 * @retvalue   :时间间隔
******************************************************************************/
u32 api_GetSystemTimePeriod(u32 StartTime)
{
	u32 res;
	
	if(mMaininf.mSystem.mSystemTick >= StartTime)
	{
		res = mMaininf.mSystem.mSystemTick - StartTime;
	}
	else
	{
		res = SYSTEMTIMEMAX - StartTime + mMaininf.mSystem.mSystemTick;
	}
	
	return res;
}

/*****************************************************************************
 * @name       :u32 api_GetCurrentSystemTime(void)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :返回此刻的时间滴答
 * @parameters :None
 * @retvalue   :此时时间
******************************************************************************/
u32 api_GetCurrentSystemTime(void)
{
	return mMaininf.mSystem.mSystemTick;
}


/*------------------------------------------------------------------------------

        主板uart通讯

--------------------------------------------------------------------------------*/


/*****************************************************************************
 * @name       :void api_UART1_SendData(u16 vCont, u8 *pData)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :send data
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void api_UART1_SendData(u16 vCont, u8 *pData)
{
	u16 iCont ;
	mMaininf.mUart1.mSendSize = vCont;
	for (iCont=0;iCont < vCont;iCont ++)
	{
		mMaininf.mUart1.TxBuffer[iCont] = *(pData + iCont);
	}
	USART_ITConfig(USART1,USART_IT_RXNE,DISABLE);
	USART_ITConfig(USART1,USART_IT_TC,ENABLE);
}

/*****************************************************************************
 * @name       :void api_UART1_MainBoard_SendCMDData(u8 seq,u8 mode,u8 cmd)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :send data
 * @parameters :cmd
 * @retvalue   :None
******************************************************************************/
void api_UART1_MainBoard_SendCMDData(u8 seq,u8 mode,u8 cmd)
{
	mMaininf.mUart1.SendBuf[0] = 0xAA;
	mMaininf.mUart1.SendBuf[1] = 0xDD;
	mMaininf.mUart1.SendBuf[2] = seq;
	mMaininf.mUart1.SendBuf[3] = mode;
	mMaininf.mUart1.SendBuf[4] = cmd;
	mMaininf.mUart1.SendBuf[5] = mMaininf.mUart1.SendBuf[0] ^ mMaininf.mUart1.SendBuf[1] ^
								 mMaininf.mUart1.SendBuf[2] ^ mMaininf.mUart1.SendBuf[3] ^
								 mMaininf.mUart1.SendBuf[4];
	
	api_UART1_SendData(6,mMaininf.mUart1.SendBuf);
}

/*****************************************************************************
 * @name       :void api_ClearUART1ReceiveFlag(void)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :clear uart2 receive flag
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void api_ClearUART1ReceiveFlag(void)
{
	mMaininf.mUart1.mReceiveStep = ST_MAIN_0xAA;
	mMaininf.mUart1.mReceiveBCC = 0;
	mMaininf.mUart1.mReceiveSize = 0;
}

/*****************************************************************************
 * @name       :void api_UART1_ReceiveData(u8 vData)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :receive data
 * @parameters :data
 * @retvalue   :None
******************************************************************************/
void api_UART1_ReceiveData(u8 vData)
{
	if(mMaininf.mUart1.mReceiveTimeOut == 0)
	{
		api_ClearUART1ReceiveFlag();
	}
	switch(mMaininf.mUart1.mReceiveStep)
	{
		case ST_MAIN_0xAA :
			if(vData == 0xAA)
			{
				mMaininf.mUart1.mReceiveStep = ST_MAIN_0xDD;
				mMaininf.mUart1.mReceiveBCC = mMaininf.mUart1.mReceiveBCC ^ vData;
				mMaininf.mUart1.mReceiveTimeOut = 10;
			}
			else mMaininf.mUart1.mReceiveTimeOut = 0;
			break;
		case ST_MAIN_0xDD :
			if(vData == 0xDD)
			{
				mMaininf.mUart1.mReceiveStep = ST_MAIN_SEQ;
				mMaininf.mUart1.mReceiveBCC = mMaininf.mUart1.mReceiveBCC ^ vData;
				mMaininf.mUart1.mReceiveTimeOut = 10;
			}
			else mMaininf.mUart1.mReceiveTimeOut = 0;
			break;
		case ST_MAIN_SEQ :
			mMaininf.mUart1.mReceiveStep = ST_MAIN_MODE;
			mMaininf.mUart1.mReceiveTimeOut = 10;
			mMaininf.mUart1.RxBuffer[mMaininf.mUart1.mReceiveSize ++] = vData;
			mMaininf.mUart1.mReceiveBCC = mMaininf.mUart1.mReceiveBCC ^ vData;
			break;
		case ST_MAIN_MODE :
			mMaininf.mUart1.mReceiveStep = ST_MAIN_CMD;
			mMaininf.mUart1.mReceiveTimeOut = 10;
			mMaininf.mUart1.RxBuffer[mMaininf.mUart1.mReceiveSize ++] = vData;
			mMaininf.mUart1.mReceiveBCC = mMaininf.mUart1.mReceiveBCC ^ vData;
			break;
		case ST_MAIN_CMD :
			mMaininf.mUart1.mReceiveStep = ST_MAIN_LEN;
			mMaininf.mUart1.mReceiveTimeOut = 10;
			mMaininf.mUart1.RxBuffer[mMaininf.mUart1.mReceiveSize ++] = vData;
			mMaininf.mUart1.mReceiveBCC = mMaininf.mUart1.mReceiveBCC ^ vData;
			break;
		case ST_MAIN_LEN :
			mMaininf.mUart1.mReceiveStep = ST_MAIN_DATA;
			mMaininf.mUart1.mReceiveTimeOut = 10;
			mMaininf.mUart1.mReceiveLen = vData;
			mMaininf.mUart1.RxBuffer[mMaininf.mUart1.mReceiveSize ++] = vData;
			mMaininf.mUart1.mReceiveBCC = mMaininf.mUart1.mReceiveBCC ^ vData;
			break;
		case ST_MAIN_DATA :
			mMaininf.mUart1.mReceiveLen --;
			if(mMaininf.mUart1.mReceiveLen == 0)
			{
				mMaininf.mUart1.mReceiveStep = ST_MAIN_BCC;
			}
			mMaininf.mUart1.mReceiveTimeOut = 10;
			mMaininf.mUart1.RxBuffer[mMaininf.mUart1.mReceiveSize ++] = vData;
			mMaininf.mUart1.mReceiveBCC = mMaininf.mUart1.mReceiveBCC ^ vData;
			break;
		case ST_MAIN_BCC :
			if(mMaininf.mUart1.mReceiveBCC == vData)
			{
				for(mMaininf.mUart1.mReceiveCont = 0;mMaininf.mUart1.mReceiveCont < (mMaininf.mUart1.RxBuffer[3] + 4);mMaininf.mUart1.mReceiveCont ++)
				{
					mMaininf.mUart1.ReceiveBuf[mMaininf.mUart1.mReceiveCont] = mMaininf.mUart1.RxBuffer[mMaininf.mUart1.mReceiveCont];
				}
				
				mMaininf.mUart1.mReceiveFlag = 1;       //接收到数据
				mMaininf.mUart1.mReceiveTimeOut = 0;
				
			}
			else mMaininf.mUart1.mReceiveTimeOut = 0;
			break;
		default :
			break;
	}
}

/*****************************************************************************
 * @name       :void api_UART4_SendData(u16 vCont, u8 *pData)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :send data
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void api_UART4_SendData(u16 vCont, u8 *pData)
{
	u16 iCont ;
	mMaininf.mUart4.mSendSize = vCont;
	for (iCont=0;iCont < vCont;iCont ++)
	{
		mMaininf.mUart4.TxBuffer[iCont] = *(pData + iCont);
	}
//	USART_ITConfig(USART1,USART_IT_RXNE,DISABLE);
	USART_ITConfig(UART4,USART_IT_TC,ENABLE);
}

/*****************************************************************************
 * @name       :void api_UART4_DEBUG_SendCMDData(u8 seq,u8 mode,u8 cmd)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :send data
 * @parameters :cmd
 * @retvalue   :None
******************************************************************************/
void api_UART4_DEBUG_SendCMDData(u8 seq,u8 mode,u8 cmd)
{
	mMaininf.mUart4.SendBuf[0] = 0xAA;
	mMaininf.mUart4.SendBuf[1] = 0xDD;
	mMaininf.mUart4.SendBuf[2] = seq;
	mMaininf.mUart4.SendBuf[3] = mode;
	mMaininf.mUart4.SendBuf[4] = cmd;
	mMaininf.mUart4.SendBuf[5] = mMaininf.mUart4.SendBuf[0] ^ mMaininf.mUart4.SendBuf[1] ^
								 mMaininf.mUart4.SendBuf[2] ^ mMaininf.mUart4.SendBuf[3] ^
								 mMaininf.mUart4.SendBuf[4];
	
	api_UART4_SendData(6,mMaininf.mUart4.SendBuf);
}

/*****************************************************************************
 * @name       :void api_ClearUART4ReceiveFlag(void)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :clear uart2 receive flag
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void api_ClearUART4ReceiveFlag(void)
{
	mMaininf.mUart4.mReceiveStep = ST_MAIN_0xAA;
	mMaininf.mUart4.mReceiveBCC = 0;
	mMaininf.mUart4.mReceiveSize = 0;
}

/*****************************************************************************
 * @name       :void api_UART4_ReceiveData(u8 vData)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :receive data
 * @parameters :data
 * @retvalue   :None
******************************************************************************/
void api_UART4_ReceiveData(u8 vData)
{
	if(mMaininf.mUart4.mReceiveTimeOut == 0)
	{
		api_ClearUART4ReceiveFlag();
	}
	switch(mMaininf.mUart4.mReceiveStep)
	{
		case ST_MAIN_0xAA :
			if(vData == 0xAA)
			{
				mMaininf.mUart4.mReceiveStep = ST_MAIN_0xDD;
				mMaininf.mUart4.mReceiveBCC = mMaininf.mUart4.mReceiveBCC ^ vData;
				mMaininf.mUart4.mReceiveTimeOut = 10;
			}
			else mMaininf.mUart4.mReceiveTimeOut = 0;
			break;
		case ST_MAIN_0xDD :
			if(vData == 0xDD)
			{
				mMaininf.mUart4.mReceiveStep = ST_MAIN_SEQ;
				mMaininf.mUart4.mReceiveBCC = mMaininf.mUart4.mReceiveBCC ^ vData;
				mMaininf.mUart4.mReceiveTimeOut = 10;
			}
			else mMaininf.mUart4.mReceiveTimeOut = 0;
			break;
		case ST_MAIN_SEQ :
			mMaininf.mUart4.mReceiveStep = ST_MAIN_MODE;
			mMaininf.mUart4.mReceiveTimeOut = 10;
			mMaininf.mUart4.RxBuffer[mMaininf.mUart4.mReceiveSize ++] = vData;
			mMaininf.mUart4.mReceiveBCC = mMaininf.mUart4.mReceiveBCC ^ vData;
			break;
		case ST_MAIN_MODE :
			mMaininf.mUart4.mReceiveStep = ST_MAIN_CMD;
			mMaininf.mUart4.mReceiveTimeOut = 10;
			mMaininf.mUart4.RxBuffer[mMaininf.mUart4.mReceiveSize ++] = vData;
			mMaininf.mUart4.mReceiveBCC = mMaininf.mUart4.mReceiveBCC ^ vData;
			break;
		case ST_MAIN_CMD :
			mMaininf.mUart4.mReceiveStep = ST_MAIN_LEN;
			mMaininf.mUart4.mReceiveTimeOut = 10;
			mMaininf.mUart4.RxBuffer[mMaininf.mUart4.mReceiveSize ++] = vData;
			mMaininf.mUart4.mReceiveBCC = mMaininf.mUart4.mReceiveBCC ^ vData;
			break;
		case ST_MAIN_LEN :
			mMaininf.mUart4.mReceiveStep = ST_MAIN_DATA;
			mMaininf.mUart4.mReceiveTimeOut = 10;
			mMaininf.mUart4.mReceiveLen = vData;
			mMaininf.mUart4.RxBuffer[mMaininf.mUart4.mReceiveSize ++] = vData;
			mMaininf.mUart4.mReceiveBCC = mMaininf.mUart4.mReceiveBCC ^ vData;
			break;
		case ST_MAIN_DATA :
			mMaininf.mUart4.mReceiveLen --;
			if(mMaininf.mUart4.mReceiveLen == 0)
			{
				mMaininf.mUart4.mReceiveStep = ST_MAIN_BCC;
			}
			mMaininf.mUart4.mReceiveTimeOut = 10;
			mMaininf.mUart4.RxBuffer[mMaininf.mUart4.mReceiveSize ++] = vData;
			mMaininf.mUart4.mReceiveBCC = mMaininf.mUart4.mReceiveBCC ^ vData;
			break;
		case ST_MAIN_BCC :
			if(mMaininf.mUart4.mReceiveBCC == vData)
			{
				for(mMaininf.mUart4.mReceiveCont = 0;mMaininf.mUart4.mReceiveCont < (mMaininf.mUart4.RxBuffer[3] + 4);mMaininf.mUart4.mReceiveCont ++)
				{
					mMaininf.mUart4.ReceiveBuf[mMaininf.mUart4.mReceiveCont] = mMaininf.mUart4.RxBuffer[mMaininf.mUart4.mReceiveCont];
				}
				
				mMaininf.mUart4.mReceiveFlag = 1;       //接收到数据
				mMaininf.mUart4.mReceiveTimeOut = 0;
				
			}
			else mMaininf.mUart4.mReceiveTimeOut = 0;
			break;
		default :
			break;
	}
}

/*****************************************************************************
 * @name       :void api_UART5_SendData(u16 vCont, u8 *pData)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :send data
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void api_UART5_SendData(u16 vCont, u8 *pData)
{
	u16 iCont ;
	mMaininf.mUart5.mSendSize = vCont;
	for (iCont=0;iCont < vCont;iCont ++)
	{
		mMaininf.mUart5.TxBuffer[iCont] = *(pData + iCont);
	}
	//USART_ITConfig(UART5,USART_IT_RXNE,DISABLE);
	USART_ITConfig(UART5,USART_IT_TC,ENABLE);
}

/*****************************************************************************
 * @name       :void api_UART5_Display_SendCMDData(u8 mode,u8 cmd,u8 status,u16 dat1,u16 dat2,u8 flag)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :send data
 * @parameters :cmd
 * @retvalue   :None
******************************************************************************/
void api_UART5_Display_SendCMDData(u8 mode,u8 cmd,u8 status,u16 dat1,u16 dat2,u8 flag)
{
	mMaininf.mUart5.SendBuf[0] = 0x55;
	mMaininf.mUart5.SendBuf[1] = 0xAA;
	mMaininf.mUart5.SendBuf[2] = mode;
	mMaininf.mUart5.SendBuf[3] = cmd;
	mMaininf.mUart5.SendBuf[4] = status;
	mMaininf.mUart5.SendBuf[5] = dat1 >> 8;
	mMaininf.mUart5.SendBuf[6] = dat1 & 0xff;
	mMaininf.mUart5.SendBuf[7] = dat2 >> 8;
	mMaininf.mUart5.SendBuf[8] = dat2 & 0xff;
	mMaininf.mUart5.SendBuf[9] = flag;
	mMaininf.mUart5.SendBuf[10] = mMaininf.mUart5.SendBuf[2] ^ mMaininf.mUart5.SendBuf[3] ^
								 mMaininf.mUart5.SendBuf[4] ^ mMaininf.mUart5.SendBuf[5] ^
								 mMaininf.mUart5.SendBuf[6] ^ mMaininf.mUart5.SendBuf[7] ^
								 mMaininf.mUart5.SendBuf[8] ^ mMaininf.mUart5.SendBuf[9];
	
	api_UART5_SendData(11,mMaininf.mUart5.SendBuf);
}

/*****************************************************************************
 * @name       :void api_ClearUART5ReceiveFlag(void)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :clear uart2 receive flag
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void api_ClearUART5ReceiveFlag(void)
{
	mMaininf.mUart5.mReceiveStep = ST_0x55;
	mMaininf.mUart5.mReceiveBCC = 0;
	mMaininf.mUart5.mReceiveSize = 0;
}

/*****************************************************************************
 * @name       :void api_UART5_ReceiveData(u8 vData)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :receive data
 * @parameters :data
 * @retvalue   :None
******************************************************************************/
// char dddd[50] = {0};
// char ffff =0 ;
void api_UART5_ReceiveData(u8 vData)
{
//	dddd[ffff++] = vData;
	if(mMaininf.mUart5.mReceiveTimeOut == 0)
	{
		api_ClearUART5ReceiveFlag();
	}
	switch(mMaininf.mUart5.mReceiveStep)
	{
		case ST_0x55 :
			if(vData == 0x55)
			{
				mMaininf.mUart5.mReceiveStep = ST_0xAA;
				mMaininf.mUart5.mReceiveTimeOut = 10;
			}
			else mMaininf.mUart5.mReceiveTimeOut = 0;
			break;
		case ST_0xAA :
			if(vData == 0xAA)
			{
				mMaininf.mUart5.mReceiveStep = ST_MODE;
				mMaininf.mUart5.mReceiveTimeOut = 10;
			}
			else mMaininf.mUart5.mReceiveTimeOut = 0;
			break;
		case ST_MODE :
			mMaininf.mUart5.mReceiveStep = ST_CMD;
			mMaininf.mUart5.mReceiveTimeOut = 10;
			mMaininf.mUart5.RxBuffer[mMaininf.mUart5.mReceiveSize ++] = vData;
			mMaininf.mUart5.mReceiveBCC = mMaininf.mUart5.mReceiveBCC ^ vData;
			break;
		case ST_CMD :
			mMaininf.mUart5.mReceiveStep = ST_STA;
			mMaininf.mUart5.mReceiveTimeOut = 10;
			mMaininf.mUart5.RxBuffer[mMaininf.mUart5.mReceiveSize ++] = vData;
			mMaininf.mUart5.mReceiveBCC = mMaininf.mUart5.mReceiveBCC ^ vData;
			break;
		case ST_STA :
			mMaininf.mUart5.mReceiveStep = ST_BCC;
			mMaininf.mUart5.mReceiveTimeOut = 10;
			mMaininf.mUart5.RxBuffer[mMaininf.mUart5.mReceiveSize ++] = vData;
			mMaininf.mUart5.mReceiveBCC = mMaininf.mUart5.mReceiveBCC ^ vData;
			break;
		case ST_BCC :
			if(mMaininf.mUart5.mReceiveBCC == vData)
			{
				for(mMaininf.mUart5.mReceiveCont = 0;mMaininf.mUart5.mReceiveCont < 3;mMaininf.mUart5.mReceiveCont ++)
				{
					mMaininf.mUart5.ReceiveBuf[mMaininf.mUart5.mReceiveCont] = mMaininf.mUart5.RxBuffer[mMaininf.mUart5.mReceiveCont];
				}
				
				mMaininf.mUart5.mReceiveFlag = 1;       //接收到数据
				mMaininf.mUart5.mReceiveTimeOut = 0;
				
			}
			else mMaininf.mUart5.mReceiveTimeOut = 0;
			break;
		default :
			break;
	}
}

/*****************************************************************************
 * @name       :void api_MachinePowerOn(void)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :上电
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void api_MachinePowerOn(void)
{
	mMaininf.mSystem.mPowerOnStartTime = api_GetCurrentSystemTime();
	
	/*while(1)
	{
		if(mMaininf.mUart5.mReceiveFlag == 1)            //    接收到反馈
		{
			mMaininf.mUart5.mReceiveFlag = 0;
			
			if((mMaininf.mUart5.ReceiveBuf[0] == 0) && (mMaininf.mUart5.ReceiveBuf[1] == 0) &&
			   (mMaininf.mUart5.ReceiveBuf[2] == 1))
			{
				api_UART5_Display_SendCMDData(0,0,1);
				mMaininf.mWork.mWorkInitFlag = 1;
				break;
			}
		}
		
		if(api_GetSystemTimePeriod(mMaininf.mSystem.mPowerOnStartTime) > 5000)
		{
			
			mMaininf.mWork.mWorkInitFlag = 0;
			break;
		}
	}*/
}



// /*****************************************************************************
//  * @name       :void api_Control_Motor(u8 mCmd,u8 OpenFlag)
//  * @date       :2020-09-19 
//  * @author     :zhengbaiqiu 
//  * @function   :控制继电器及气缸
//  * @parameters :0:陀螺仪继电器  1：按键1气缸  2：按键2气缸  3：按键3气缸
// 				4：滚刷继电器   5：吸尘电机   6：放电继电器 7：充电继电器
// 				8：水箱继电器
//  * @retvalue   :None
// ******************************************************************************/
// void api_Control_Motor(u8 mCmd,u8 OpenFlag)
// {
// 	switch(mCmd)
// 	{
// 		case 0://陀螺仪继电器
// 			if(OpenFlag == 1)
// 			{
// 				PDout(7) = 1;
// 			}
// 			else
// 			{
// 				PDout(7) = 0;
// 			}
// 			break;
// 		case 1://按键1气缸
// 			if(OpenFlag == 1)
// 			{
// 				PGout(15) = 1;
// 			}
// 			else
// 			{
// 				PGout(15) = 0;
// 			}
// 			break;
// 		case 2://按键2气缸
// 			if(OpenFlag == 1)
// 			{
// 				PGout(14) = 1;
// 			}
// 			else
// 			{
// 				PGout(14) = 0;
// 			}
// 			break;
// 		case 3://按键3气缸
// 			if(OpenFlag == 1)
// 			{
// 				PGout(13) = 1;
// 			}
// 			else
// 			{
// 				PGout(13) = 0;
// 			}
// 			break;
// 		case 4://滚刷继电器
// 			if(OpenFlag == 1)
// 			{
// 				PDout(4) = 1;
// 			}
// 			else
// 			{
// 				PDout(4) = 0;
// 			}
// 			break;
// 		case 5://吸尘电机
// 			if(OpenFlag == 1)
// 			{
// 				PDout(3) = 1;
// 			}
// 			else
// 			{
// 				PDout(3) = 0;
// 			}
// 			break;
// 		case 6://放电继电器
// 			if(OpenFlag == 1)
// 			{
// 				PDout(1) = 1;
// 			}
// 			else
// 			{
// 				PDout(1) = 0;
// 			}
// 			break;
// 		case 7://充电继电器
// 			if(OpenFlag == 1)
// 			{
// 				PAout(11) = 1;
// 			}
// 			else
// 			{
// 				PAout(11) = 0;
// 			}
// 			break;
// 		case 8://水箱继电器
// 			if(OpenFlag == 1)
// 			{
// 				PDout(0) = 1;
// 			}
// 			else
// 			{
// 				PDout(0) = 0;
// 			}
// 			break;
// 		default :
// 			break;
// 	}
// }

/*****************************************************************************
 * @name       :u8 api_ReadGpioInputStauts(GPIO_TypeDef* GPIOx, 
											uint16_t GPIO_Pin,uint8_t Cont)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :GPIOx：GPIO
				GPIO_Pin：Pin脚
				val:检查电平值
				Cont：检测次数
 * @parameters :None
 * @retvalue   :0:检测失败   1：检测成功
******************************************************************************/
//u8 iiiiiii;
u8 api_ReadGpioInputStauts(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,u8 Val,uint8_t Cont)
{
	uint8_t mCont;
	uint8_t mNum = 0;
	
// 	iiiiiii = GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_15);
// 	iiiiiii = GPIO_ReadInputDataBit(GPIOx,GPIO_Pin);
	
	for(mCont = 0;mCont < Cont;mCont ++)
	{
		//mNum = mNum + GPIO_ReadInputDataBit(GPIOx,GPIO_Pin);
		if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == Val)
		{
			mNum ++;
		}
		vTaskDelay(10);
	}
	
	mNum = mNum / Cont;
	
	if(mNum == 1)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}



/*****************************************************************************
 * @name       :u8 api_Check_PWM(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,u8 cycle,u8 differ)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :GPIOx：GPIO
				GPIO_Pin：Pin脚
				cycle:周期
				differ：误差范围
 * @parameters :None
 * @retvalue   :0:检测失败   1：检测成功
******************************************************************************/
//u8 iiiiiii;
u8 api_Check_PWM(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,u8 cycle,u8 differ)
{
	u8 mGpioPinStatus;
	u32 mTimeOut = 0;
	u32 mCont = 0;
	
	taskENTER_CRITICAL();
	TIM_Cmd(TIM3,ENABLE);
	mGpioPinStatus = GPIO_ReadInputDataBit(GPIOx,GPIO_Pin);
	while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == mGpioPinStatus)
	{
		if(++ mTimeOut > 0xfffff)
		{
			break;
		}
	}
//	iiiiiii = GPIO_ReadInputDataBit(GPIOx,GPIO_Pin);
	mMaininf.mSystem.mSystem200usTick = 0;
	while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) != mGpioPinStatus)
	{
		if(++ mTimeOut > 0xfffff)
		{
			break;
		}
	}
//	iiiiiii = GPIO_ReadInputDataBit(GPIOx,GPIO_Pin);
	mTimeOut = 0;
	while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == mGpioPinStatus)
	{
		if(++ mTimeOut > 0xfffff)
		{
			break;
		}
	}
	mCont = mMaininf.mSystem.mSystem200usTick;
	TIM_Cmd(TIM3,DISABLE);
	
	taskEXIT_CRITICAL();
	
	
	if((mCont >= (cycle - differ)) && (mCont <= (cycle + differ)))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}



/*****************************************************************************
 * @name       :u8 api_Test_ADC_Status(__IO uint16_t *adcval,uint16_t 
				detectval,uint16_t diffval)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :adcval：输入参数
				detectval：比较值
				diffval：偏差
 * @parameters :None
 * @retvalue   :0:检测失败   1：检测成功
******************************************************************************/
u8 api_Test_ADC(__IO uint16_t *adcval,uint16_t detectval,uint16_t diffval)
{
	u8 mCont;
	uint16_t mNum = 0;
// 	if(detectval >= 25)
// 	{
// 		float mValL = ((57.835 - 5.2017*(detectval - diffval)) * 4095) / 33;
// 		float mValH = ((57.835 - 5.2017*(detectval + diffval)) * 4095) / 33;
// 	}
// 	else
// 	{
// 		float mValL = ((24.26 + 7.24*(detectval - diffval)) * 4095) / 33;
// 		float mValH = ((24.26 + 7.24*(detectval + diffval)) * 4095) / 33;
// 	}
	
	for(mCont = 0;mCont < 5;mCont ++)
	{
		mNum = mNum + (*adcval & 0x0fff);
		vTaskDelay(10);
	}
	
	mNum = mNum / 5;
	
	if((mNum > (detectval - diffval)) && (mNum < (detectval + diffval)))
	{
		return 1;
	}
	
	return 0;
}



/*****************************************************************************
 * @name       :u8 api_Check_Avoid_Bump_Status(u8 AvoidBump)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :AvoidBump
 * @parameters :AvoidBump：0:left    1:right   2:touch_vcc_a  touch_vcc_b
 * @retvalue   :0:检测失败   1：检测成功
******************************************************************************/
u8 api_Check_Avoid_Bump_Status(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,u8 AvoidBump)
{
	u8 mGpioPinStatus;
	u32 mTimeOut = 0;
	u32 mCont = 0;
	
	if(AvoidBump == 0)
	{
		PFout(11) = 1;
	}
	else if(AvoidBump == 1)
	{
		PBout(1) = 1;
	}
	else if(AvoidBump == 2)
	{
		PFout(4) = 1;
		PFout(5) = 1;
	}
	
	TIM1_PWM_Configure(200,719);
	TIM_SetCompare2(TIM1,20);
	
	taskENTER_CRITICAL();
	TIM_Cmd(TIM3,ENABLE);
	mGpioPinStatus = GPIO_ReadInputDataBit(GPIOx,GPIO_Pin);
	while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == mGpioPinStatus)
	{
		if(++ mTimeOut > 0xfffff)
		{
			break;
		}
	}
	mMaininf.mSystem.mSystem200usTick = 0;
	while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) != mGpioPinStatus)
	{
		if(++ mTimeOut > 0xfffff)
		{
			break;
		}
	}
	mTimeOut = 0;
	while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == mGpioPinStatus)
	{
		if(++ mTimeOut > 0xfffff)
		{
			break;
		}
	}
	mCont = mMaininf.mSystem.mSystem200usTick;
	TIM_Cmd(TIM3,DISABLE);
	
	taskEXIT_CRITICAL();
	
	PFout(11) = 0;
	PBout(1) = 0;
	TIM1_PWM_Configure(26,71);
	TIM_SetCompare2(TIM1,0);
	
	if((mCont >= 8) && (mCont <= 12))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/*****************************************************************************
 * @name       :void api_Send_Signal(u8 mCmd)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :发送命令
 * @parameters :mCmd ：
				0x01: 上引导   0x02: 前引导  0x03:左免碰撞  0x04: 右免碰撞  0x05:前免碰撞
 * @retvalue   :None
******************************************************************************/
void api_Send_Signal(u8 mCmd,u8 OpenFlag)
{
	switch(mCmd)
	{
		case 0x01 :
			if(OpenFlag == 1)
			{
				mMaininf.mWork.mWorkGuideSendFlag = 1;
				TIM_SetCompare2(TIM1,13);
			}
			else
			{
				TIM_SetCompare2(TIM1,0);
				mMaininf.mWork.mWorkGuideSendFlag = 0;
				PCout(0) = 1;
			}
			break;
		case 0x02 :
			if(OpenFlag == 1)
			{
				mMaininf.mWork.mWorkGuideSendFlag = 2;
				TIM_SetCompare2(TIM1,13);
			}
			else
			{
				TIM_SetCompare2(TIM1,0);
				mMaininf.mWork.mWorkGuideSendFlag = 0;
				PCout(0) = 1;
			}
			break;
		case 0x03 :
			if(OpenFlag == 1)
			{
				mMaininf.mWork.mWorkSensorSendAvoidBumpFlag = 1;
				PFout(11) = 0;
// 				TIM1_PWM_Configure(200,719);
// 				TIM_SetCompare2(TIM1,20);
			}
			else
			{
				mMaininf.mWork.mWorkSensorSendAvoidBumpFlag = 0;
				PFout(11) = 1;
				
// 				TIM1_PWM_Configure(26,71);
// 				TIM_SetCompare2(TIM1,0);
			}
			break;
		case 0x04 :
			if(OpenFlag == 1)
			{
				mMaininf.mWork.mWorkSensorSendAvoidBumpFlag = 1;
				PBout(1) = 0;
// 				TIM1_PWM_Configure(200,719);
// 				TIM_SetCompare2(TIM1,20);
			}
			else
			{
				mMaininf.mWork.mWorkSensorSendAvoidBumpFlag = 0;
				PBout(1) = 1;
// 				TIM1_PWM_Configure(26,71);
// 				TIM_SetCompare2(TIM1,0);
			}
			break;
		case 0x05 :
			if(OpenFlag == 1)
			{
				mMaininf.mWork.mWorkSensorSendAvoidBumpFlag = 1;
				PFout(5) = 0;
				PFout(4) = 0;
// 				TIM1_PWM_Configure(200,719);
// 				TIM_SetCompare2(TIM1,20);
			}
			else
			{
				mMaininf.mWork.mWorkSensorSendAvoidBumpFlag = 0;
				PFout(5) = 1;
				PFout(4) = 1;
// 				TIM1_PWM_Configure(26,71);
// 				TIM_SetCompare2(TIM1,0);
			}
			break;
		default :
			break;
	}
}

/*****************************************************************************
 * @name       :void api_Send_Signal(u8 mCmd)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :发送命令
 * @parameters :mCmd ：
				免碰撞气缸控制
 * @retvalue   :None
******************************************************************************/
void api_Control_Motor(u8 mCmd,u8 OpenFlag)
{
	switch(mCmd)
	{
		case 0://
			if(OpenFlag == 1)
			{
				PEout(0) = 1;
			}
			else
			{
				PEout(0) = 0;
			}
			break;
		case 1://
			if(OpenFlag == 1)
			{
				PBout(9) = 1;
			}
			else
			{
				PBout(9) = 0;
			}
			break;
		case 2://
			if(OpenFlag == 1)
			{
				PBout(8) = 1;
			}
			else
			{
				PBout(8) = 0;
			}
			break;
		case 3://
			if(OpenFlag == 1)
			{
				PBout(7) = 1;
			}
			else
			{
				PBout(7) = 0;
			}
			break;
		case 4://
			if(OpenFlag == 1)
			{
				PBout(6) = 1;
			}
			else
			{
				PBout(6) = 0;
			}
			break;
		case 5://
			if(OpenFlag == 1)
			{
				PBout(5) = 1;
			}
			else
			{
				PBout(5) = 0;
			}
			break;
		case 6://
			if(OpenFlag == 1)
			{
				PBout(4) = 1;
			}
			else
			{
				PBout(4) = 0;
			}
			break;
		case 7://
			if(OpenFlag == 1)
			{
				PBout(3) = 1;
			}
			else
			{
				PBout(3) = 0;
			}
			break;
		case 8://
			if(OpenFlag == 1)
			{
				PGout(15) = 1;
			}
			else
			{
				PGout(15) = 0;
			}
			break;
		case 9://
			if(OpenFlag == 1)
			{
				PGout(14) = 1;
			}
			else
			{
				PGout(14) = 0;
			}
			break;
		case 10://
			if(OpenFlag == 1)
			{
				
			}
			else
			{
				
			}
			break;
		default :
			break;
	}
}

/*****************************************************************************
 * @name       :void api_Led_Status_Set(u8 led,u8 status)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :0:B0 1:B1 2:B2 3:B3 4:B4 5:B5  10:A0  11:all     0:关  1：开   2：闪
 * @retvalue   :None
******************************************************************************/
void api_Led_Status_Set(u8 led,u8 status)
{
	mMaininf.mWork.mWorkLedStatus[led] = status;
	
// 	if(led < 10)
// 	{
// 		mMaininf.mWork.mWorkLedStatus[led] = status;
// 	}
// 	else if(led == 10)
// 	{
// 		if(status == 0)
// 		{
// 			PAout(4) = 0;
// 		}
// 		else
// 		{
// 			PAout(4) = 1;
// 		}
// 	}
// 	else
// 	{
// 		mMaininf.mWork.mWorkLedStatus[LED_B0] = status;
// 		mMaininf.mWork.mWorkLedStatus[LED_B1] = status;
// 		mMaininf.mWork.mWorkLedStatus[LED_B2] = status;
// 		mMaininf.mWork.mWorkLedStatus[LED_B3] = status;
// 		mMaininf.mWork.mWorkLedStatus[LED_B4] = status;
// 	}
}

/*****************************************************************************
 * @name       :void api_Led_Control(u8 Led,u8 OpenFlag)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :控制继电器及LED
				0:TO_GPIO1     1:TO_GPIO2     2:TO_GPIO3      3:TO_GPIO4
				4:TO_GPIO5     5:TO_GPIO6     6:TO_GPIO7      7:TO_GPIO8
				8:TO_GPIO9     9:TO_GPIO10    10:TO_GPIO11    11:TO_GPIO12
				12:TO_GPIO13   13:TO_GPIO14  14:LED_A0       15:LED_A1
				16:LED_A2      17:LED_B0      18:LED_B1       19:LED_B2
				20:LED_B3      21:LED_B4      22:LED_B5       23:LED_PASS
 * @retvalue   :None
******************************************************************************/
void api_Led_Control(u8 Led,u8 OpenFlag)
{
	switch(Led)
	{
		case LED_A0:
			if(OpenFlag == 0)
			{
				PCout(5) = 0;
			}
			else if(OpenFlag == 1)
			{
				PCout(5) = 1;
			}
			else
			{
				PCout(5) = ~PCout(5);
			}
			break;
		default :
			break;
	}
		
}

/*****************************************************************************
 * @name       :char NumToChar(u16 num)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :判断信号状态
 * @parameters :相应
 * @retvalue   :None
******************************************************************************/
char NumToChar(u16 num)
{
	char temp;
	
	switch(num)
	{
		case 0 :
			temp = '0';
			break;
		case 1 :
			temp = '1';
			break;
		case 2 :
			temp = '2';
			break;
		case 3 :
			temp = '3';
			break;
		case 4 :
			temp = '4';
			break;
		case 5 :
			temp = '5';
			break;
		case 6 :
			temp = '6';
			break;
		case 7 :
			temp = '7';
			break;
		case 8 :
			temp = '8';
			break;
		case 9 :
			temp = '9';
			break;
		default :
			break;
	}
	
	return temp;
}


/*****************************************************************************
 * @name       :void api_ShortToStr(u8* str,u16 data)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :判断信号状态
 * @parameters :相应
 * @retvalue   :None
******************************************************************************/
void api_ShortToStr(u8* str,u16 data)
{
	u16 tempcont;
	
	tempcont = data;
	*(str + 4) = NumToChar(tempcont % 10);
	tempcont = tempcont / 10;
	*(str + 3) = NumToChar(tempcont % 10);
	tempcont = tempcont / 10;
	*(str + 2) = NumToChar(tempcont % 10);
	tempcont = tempcont / 10;
	*(str + 1) = NumToChar(tempcont % 10);
	tempcont = tempcont / 10;
	*str = NumToChar(tempcont % 10);
}



