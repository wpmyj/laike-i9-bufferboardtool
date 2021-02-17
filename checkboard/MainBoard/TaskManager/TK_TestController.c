/****************************************************************************************************/
/*			@FileName		:	TK_TestController.c												  	*/
/*			@Author			:	zhengbaiqiu															*/
/*			@Version		:	v1.0																*/
/*			@Date			:	2019.11.12  														*/
/****************************************************************************************************/


/* Includes --------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Project ---------------------------------------------------------------------*/
#include "maindef.h"
#include "apiLib.h"
#include "bsp.h"
#include "TK_TestController.h"


/* Variable --------------------------------------------------------------------*/
/* Function --------------------------------------------------------------------*/
void TK_TestController( void *pvParameters );
void api_SendCMD_To_Display(u8 mode,u8 cmd,u8 status,u16 dat1,u16 dat2,u8 falg,u16 waittime,u8 count);
u8 api_SendCMD_To_MainBoard(u8 seq,u8 mode,u8 cmd,u16 waittime,u8 type,u8 num);
u8 iiiiiiiii = 0;

u8 str[20];


/*****************************************************************************
 * @name       :void TK_TestController( void *pvParameters )
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :时间控制任务
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
u8 iiiii;
void TK_TestController( void *pvParameters )
{
	//api_MachinePowerOn();
	for(;;)
	{
		IWDG_ReloadCounter();
		
// 		PGout(13) = 1;
// 		
// 		PAout(12) = 1;
// 		PAout(15) = 1;
// 		vTaskDelay(1000);
		
// 		PAout(12) = 0;
// 		//PAout(15) = 0;
// 		vTaskDelay(1000);
// 		
// 		for(iiiii = 0;iiiii < 10;iiiii++)
// 		{
// 			api_Control_Motor(iiiii,1);
// 			vTaskDelay(3000);
// 		}
		
// 		PFout(7) = 0;
// 		PFout(6) = 0;
// 		PFout(5) = 0;
// 		PFout(4) = 0;

// 		if(iiiiiiiii == 0)
// 		{
// 			iiiiiiiii ++;
// 			PGout(13) = 1;   //压合板子
// 			PAout(12) = 1;
// 			PAout(15) = 1;
// 			api_Send_Signal(4,1);
// 		}



// 		if(iiiiiiiii == 0)
// 		{
// 			iiiiiiiii++;
//  			PGout(13) = 1;   //压合板子
// 			PAout(12) = 1;
// 			PAout(15) = 1;
// 			TIM1_GPIO_Configure();
// 			api_Send_Signal(5,1);
// 		}
// 		PGout(13) = 1;   //压合板子
// 		PAout(12) = 1;
// 		PAout(15) = 1;
// 		api_Send_Signal(1,1);
		
		if(mMaininf.mWork.mWorkTestSelectFlag == 0)
		{
			if(mMaininf.mUart5.mReceiveFlag == 1)
			{
				mMaininf.mUart5.mReceiveFlag = 0;
				
				if((mMaininf.mUart5.ReceiveBuf[0] == 2) && ((mMaininf.mUart5.ReceiveBuf[1] == 1) || 
					(mMaininf.mUart5.ReceiveBuf[1] == 2)) && (mMaininf.mUart5.ReceiveBuf[2] == 1))
				{
					api_UART5_Display_SendCMDData(2,mMaininf.mUart5.ReceiveBuf[1],1,0,0,0);
				}
				if((mMaininf.mUart5.ReceiveBuf[0] == 2) && (mMaininf.mUart5.ReceiveBuf[1] == 4) && 
					(mMaininf.mUart5.ReceiveBuf[2] == 1))//i8主板
				{
					mMaininf.mWork.mWorkTestSelectFlag = 1;
					mMaininf.mWork.mWorkCheck8Or9Flag = 1;
					//mMaininf.mWork.mWorkKeyPressFlag = 1;
				}
				if((mMaininf.mUart5.ReceiveBuf[0] == 2) && (mMaininf.mUart5.ReceiveBuf[1] == 5) && 
					(mMaininf.mUart5.ReceiveBuf[2] == 1))//i9主板
				{
					mMaininf.mWork.mWorkTestSelectFlag = 1;
					mMaininf.mWork.mWorkCheck8Or9Flag = 2;
					//mMaininf.mWork.mWorkKeyPressFlag = 1;
				}
			}
		}
		else
		{			
			if(mMaininf.mUart4.mReceiveFlag == 1)   //调试
			{
				mMaininf.mUart4.mReceiveFlag = 0;
				//api_UART5_Display_SendCMDData(2,1,1);
				//api_Led_Status_Set(LED_A0,0);
				api_SendCMD_To_Display(2,3,1,0,0,0,1000,3);
				vTaskDelay(1000);
				mMaininf.mWork.mWorkChecking = 1;
				PAout(12) = 1;
				PAout(15) = 1;
				//PEout(1) = 0;
				vTaskDelay(1000);
				//api_SendCMD_To_MainBoard(0,1,1,100,1,5);
				mMaininf.mWork.mWorkStep = mMaininf.mUart4.ReceiveBuf[2];
			}
			
			if((mMaininf.mWork.mWorkKeyPressFlag == 1) && (mMaininf.mWork.mWorkChecking == 0))
			{
				PGout(13) = 1;   //压合板子
				mMaininf.mWork.mWorkKeyPressFlag = 0;
				mMaininf.mWork.mWorkKeyPressFlag1 = 0;
				if(mMaininf.mWork.mWorkCheck8Or9Flag == 1)//i8主板
				{
					PAout(11) = 1;//i8特有
					PAout(12) = 1;
					PAout(15) = 1;
				}
				else if(mMaininf.mWork.mWorkCheck8Or9Flag == 2)//i9主板
				{
					PAout(12) = 1;
					PAout(15) = 1;
				}
				api_SendCMD_To_Display(2,3,1,0,0,0,1000,3);
				vTaskDelay(1000);
				mMaininf.mWork.mWorkChecking = 1;
				mMaininf.mWork.mWorkStep = TEST_START;
			}
			
			
			if(mMaininf.mWork.mWorkChecking == 1)
			{
				if(mMaininf.mWork.mWorkStep != mMaininf.mWork.mWorkStepOld)
				{
					mMaininf.mWork.mWorkStepOld = mMaininf.mWork.mWorkStep;
					
					switch(mMaininf.mWork.mWorkStep)
					{

						case TEST_START ://01
							mMaininf.mWork.mWorkStep = TEST_GUIDE1;
							break;
						case TEST_GUIDE1 ://02
							if(api_ReadGpioInputStauts(GPIOE,GPIO_Pin_14,1,10) == 1)
							{
								api_SendCMD_To_Display(1,2,1,0,0,0,1000,3);
							}
							else
							{
								api_SendCMD_To_Display(1,2,0,0,0,0,1000,3);
								mMaininf.mWork.mWorkCheckErrorFlag = 1;
							}
							mMaininf.mWork.mWorkStep = TEST_GUIDE2;
							break;
						case TEST_GUIDE2 ://03
							if(api_ReadGpioInputStauts(GPIOE,GPIO_Pin_13,1,10) == 1)
							{
								api_SendCMD_To_Display(1,3,1,0,0,0,1000,3);
							}
							else
							{
								api_SendCMD_To_Display(1,3,0,0,0,0,1000,3);
								mMaininf.mWork.mWorkCheckErrorFlag = 1;
							}
							mMaininf.mWork.mWorkStep = TEST_GUIDE3;
							break;
						case TEST_GUIDE3 ://04
							api_Send_Signal(1,1);
							vTaskDelay(100);
							if(api_Check_PWM(GPIOA,GPIO_Pin_8,100,20) == 1)
							{
								api_SendCMD_To_Display(1,4,1,0,0,0,1000,3);
							}
							else
							{
								api_SendCMD_To_Display(1,4,0,0,0,0,1000,3);
								mMaininf.mWork.mWorkCheckErrorFlag = 1;
							}
							//api_Send_Signal(1,0);
							vTaskDelay(100);
							mMaininf.mWork.mWorkStep = TEST_GUIDE4;
							break;
						case TEST_GUIDE4 ://05
// 							api_Send_Signal(2,1);
// 							vTaskDelay(100);
							if(api_Check_PWM(GPIOC,GPIO_Pin_9,100,20) == 1)
							{
								api_SendCMD_To_Display(1,5,1,0,0,0,1000,3);
							}
							else
							{
								api_SendCMD_To_Display(1,5,0,0,0,0,1000,3);
								mMaininf.mWork.mWorkCheckErrorFlag = 1;
							}
							api_Send_Signal(1,0);
							mMaininf.mWork.mWorkStep = TEST_LEFT_AVOID_BUMP;
							break;
						case TEST_LEFT_AVOID_BUMP ://06
							TIM1_GPIO_Configure();
						
							mMaininf.mWork.mWorkAvoidBumpDatFar = mMaininf.mWork.mWorkSensorDataAvl[2];
							if(mMaininf.mWork.mWorkAvoidBumpDatFar < 124)
							{
// 								TIM1_GPIO_Configure();
								api_Send_Signal(3,1);
								vTaskDelay(100);
								mMaininf.mWork.mWorkAvoidBumpDatNear = mMaininf.mWork.mWorkSensorDataAvl[2];
								if(mMaininf.mWork.mWorkAvoidBumpDatNear > 500)
								{
									api_SendCMD_To_Display(1,6,1,mMaininf.mWork.mWorkAvoidBumpDatFar,mMaininf.mWork.mWorkAvoidBumpDatNear,0,1000,3);
								}
								else
								{
									api_SendCMD_To_Display(1,6,0,mMaininf.mWork.mWorkAvoidBumpDatFar,mMaininf.mWork.mWorkAvoidBumpDatNear,2,1000,3);
									mMaininf.mWork.mWorkCheckErrorFlag = 1;
								}
							}
							else
							{
								api_SendCMD_To_Display(1,6,0,mMaininf.mWork.mWorkAvoidBumpDatFar,0,3,1000,3);
								mMaininf.mWork.mWorkCheckErrorFlag = 1;
							}
							api_Send_Signal(3,0);
							mMaininf.mWork.mWorkStep = TEST_RIGHT_AVOID_BUMP;
							break;
						case TEST_RIGHT_AVOID_BUMP ://07
							mMaininf.mWork.mWorkAvoidBumpDatFar = mMaininf.mWork.mWorkSensorDataAvl[3];
							if(mMaininf.mWork.mWorkAvoidBumpDatFar < 124)
							{
								api_Send_Signal(4,1);
								vTaskDelay(100);
								mMaininf.mWork.mWorkAvoidBumpDatNear = mMaininf.mWork.mWorkSensorDataAvl[3];
								if(mMaininf.mWork.mWorkAvoidBumpDatNear > 500)
								{
									api_SendCMD_To_Display(1,7,1,mMaininf.mWork.mWorkAvoidBumpDatFar,mMaininf.mWork.mWorkAvoidBumpDatNear,0,1000,3);
								}
								else
								{
									api_SendCMD_To_Display(1,7,0,mMaininf.mWork.mWorkAvoidBumpDatFar,mMaininf.mWork.mWorkAvoidBumpDatNear,2,1000,3);
									mMaininf.mWork.mWorkCheckErrorFlag = 1;
								}
							}
							else
							{
								api_SendCMD_To_Display(1,7,0,mMaininf.mWork.mWorkAvoidBumpDatFar,0,3,1000,3);
								mMaininf.mWork.mWorkCheckErrorFlag = 1;
							}
							api_Send_Signal(4,0);
							mMaininf.mWork.mWorkStep = TEST_FRONT_AVOID_BUMP0;
							break;
						case TEST_FRONT_AVOID_BUMP0 ://08
							api_Send_Signal(5,1);
							vTaskDelay(200);
							mMaininf.mWork.mWorkAvoidBumpDatFar = mMaininf.mWork.mWorkSensorDataAvl[4];
							if(mMaininf.mWork.mWorkAvoidBumpDatFar < 3000)
							{
								mMaininf.mWork.mWorkAmidAvoidBumpFlag = 1;
								api_Control_Motor(0,1);
								vTaskDelay(1500);
								mMaininf.mWork.mWorkAvoidBumpDatNear = mMaininf.mWork.mWorkSensorDataAvl[4];
								if(mMaininf.mWork.mWorkAvoidBumpDatNear > 1200)
								{
									#ifdef zbq_Debug
									api_UART4_SendData(2,"m1");
									vTaskDelay(50);
									api_ShortToStr(str,mMaininf.mWork.mWorkSensorDataAvl[4]);
									api_UART4_SendData(5,str);
									vTaskDelay(50);
									api_UART4_SendData(2,"\r\n");
									vTaskDelay(50);
									#endif
									
									api_SendCMD_To_Display(1,8,1,mMaininf.mWork.mWorkAvoidBumpDatFar,mMaininf.mWork.mWorkAvoidBumpDatNear,0,1000,3);
								}
								else
								{
									#ifdef zbq_Debug
									api_UART4_SendData(2,"m1");
									vTaskDelay(50);
									api_ShortToStr(str,mMaininf.mWork.mWorkSensorDataAvl[4]);
									api_UART4_SendData(5,str);
									vTaskDelay(50);
									api_UART4_SendData(2,"\r\n");
									vTaskDelay(50);
									#endif
									
									api_SendCMD_To_Display(1,8,0,mMaininf.mWork.mWorkAvoidBumpDatFar,mMaininf.mWork.mWorkAvoidBumpDatNear,2,1000,3);
									mMaininf.mWork.mWorkCheckErrorFlag = 1;
								}
							}
							else
							{
								api_SendCMD_To_Display(1,8,0,mMaininf.mWork.mWorkAvoidBumpDatFar,0,3,1000,3);
								mMaininf.mWork.mWorkCheckErrorFlag = 1;
							}
							api_Control_Motor(0,0);
							mMaininf.mWork.mWorkStep = TEST_FRONT_AVOID_BUMP1;
							break;
						case TEST_FRONT_AVOID_BUMP1 ://09
							if(mMaininf.mWork.mWorkAmidAvoidBumpFlag == 1)
							{
								api_Control_Motor(1,1);
								vTaskDelay(1500);
								
								mMaininf.mWork.mWorkAvoidBumpDatNear = mMaininf.mWork.mWorkSensorDataAvl[4];
								if(mMaininf.mWork.mWorkAvoidBumpDatNear > 1200)
								{
									#ifdef zbq_Debug
									api_UART4_SendData(2,"m2");
									vTaskDelay(50);
									api_ShortToStr(str,mMaininf.mWork.mWorkSensorDataAvl[4]);
									api_UART4_SendData(5,str);
									vTaskDelay(50);
									api_UART4_SendData(2,"\r\n");
									vTaskDelay(50);
									#endif
									
									api_SendCMD_To_Display(1,9,1,mMaininf.mWork.mWorkAvoidBumpDatFar,mMaininf.mWork.mWorkAvoidBumpDatNear,0,1000,3);
								}
								else
								{
									#ifdef zbq_Debug
									api_UART4_SendData(2,"m2");
									vTaskDelay(50);
									api_ShortToStr(str,mMaininf.mWork.mWorkSensorDataAvl[4]);
									api_UART4_SendData(5,str);
									vTaskDelay(50);
									api_UART4_SendData(2,"\r\n");
									vTaskDelay(50);
									#endif
									
									api_SendCMD_To_Display(1,9,0,mMaininf.mWork.mWorkAvoidBumpDatFar,mMaininf.mWork.mWorkAvoidBumpDatNear,2,1000,3);
									mMaininf.mWork.mWorkCheckErrorFlag = 1;
								}
							}
							else
							{
								api_SendCMD_To_Display(1,9,0,mMaininf.mWork.mWorkAvoidBumpDatFar,0,3,1000,3);
								mMaininf.mWork.mWorkCheckErrorFlag = 1;
							}
							api_Control_Motor(1,0);
							mMaininf.mWork.mWorkStep = TEST_FRONT_AVOID_BUMP2;
							break;
						case TEST_FRONT_AVOID_BUMP2 ://10
							if(mMaininf.mWork.mWorkAmidAvoidBumpFlag == 1)
							{
								api_Control_Motor(2,1);
								vTaskDelay(1500);
								
								mMaininf.mWork.mWorkAvoidBumpDatNear = mMaininf.mWork.mWorkSensorDataAvl[4];
								if(mMaininf.mWork.mWorkAvoidBumpDatNear > 1200)
								{
									#ifdef zbq_Debug
									api_UART4_SendData(2,"m3");
									vTaskDelay(50);
									api_ShortToStr(str,mMaininf.mWork.mWorkSensorDataAvl[4]);
									api_UART4_SendData(5,str);
									vTaskDelay(50);
									api_UART4_SendData(2,"\r\n");
									vTaskDelay(50);
									#endif
									
									api_SendCMD_To_Display(1,10,1,mMaininf.mWork.mWorkAvoidBumpDatFar,mMaininf.mWork.mWorkAvoidBumpDatNear,0,1000,3);
								}
								else
								{
									#ifdef zbq_Debug
									api_UART4_SendData(2,"m3");
									vTaskDelay(50);
									api_ShortToStr(str,mMaininf.mWork.mWorkSensorDataAvl[4]);
									api_UART4_SendData(5,str);
									vTaskDelay(50);
									api_UART4_SendData(2,"\r\n");
									vTaskDelay(50);
									#endif
									
									api_SendCMD_To_Display(1,10,0,mMaininf.mWork.mWorkAvoidBumpDatFar,mMaininf.mWork.mWorkAvoidBumpDatNear,2,1000,3);
									mMaininf.mWork.mWorkCheckErrorFlag = 1;
								}
							}
							else
							{
								api_SendCMD_To_Display(1,10,0,mMaininf.mWork.mWorkAvoidBumpDatFar,0,3,1000,3);
								mMaininf.mWork.mWorkCheckErrorFlag = 1;
							}
							api_Control_Motor(2,0);
							mMaininf.mWork.mWorkStep = TEST_FRONT_AVOID_BUMP3;
							break;
						case TEST_FRONT_AVOID_BUMP3 ://11
							if(mMaininf.mWork.mWorkAmidAvoidBumpFlag == 1)
							{
								api_Control_Motor(3,1);
								vTaskDelay(1500);
								
								mMaininf.mWork.mWorkAvoidBumpDatNear = mMaininf.mWork.mWorkSensorDataAvl[4];
								if(mMaininf.mWork.mWorkAvoidBumpDatNear > 1200)
								{
									#ifdef zbq_Debug
									api_UART4_SendData(2,"m4");
									vTaskDelay(50);
									api_ShortToStr(str,mMaininf.mWork.mWorkSensorDataAvl[4]);
									api_UART4_SendData(5,str);
									vTaskDelay(50);
									api_UART4_SendData(2,"\r\n");
									vTaskDelay(50);
									#endif
									
									api_SendCMD_To_Display(1,11,1,mMaininf.mWork.mWorkAvoidBumpDatFar,mMaininf.mWork.mWorkAvoidBumpDatNear,0,1000,3);
								}
								else
								{
									#ifdef zbq_Debug
									api_UART4_SendData(2,"m4");
									vTaskDelay(50);
									api_ShortToStr(str,mMaininf.mWork.mWorkSensorDataAvl[4]);
									api_UART4_SendData(5,str);
									vTaskDelay(50);
									api_UART4_SendData(2,"\r\n");
									vTaskDelay(50);
									#endif
									
									api_SendCMD_To_Display(1,11,0,mMaininf.mWork.mWorkAvoidBumpDatFar,mMaininf.mWork.mWorkAvoidBumpDatNear,2,1000,3);
									mMaininf.mWork.mWorkCheckErrorFlag = 1;
								}
							}
							else
							{
								api_SendCMD_To_Display(1,11,0,mMaininf.mWork.mWorkAvoidBumpDatFar,0,3,1000,3);
								mMaininf.mWork.mWorkCheckErrorFlag = 1;
							}
							api_Control_Motor(3,0);
							mMaininf.mWork.mWorkStep = TEST_FRONT_AVOID_BUMP4;
							break;
						case TEST_FRONT_AVOID_BUMP4 ://12
							if(mMaininf.mWork.mWorkAmidAvoidBumpFlag == 1)
							{
								api_Control_Motor(4,1);
								vTaskDelay(1500);
								
								mMaininf.mWork.mWorkAvoidBumpDatNear = mMaininf.mWork.mWorkSensorDataAvl[4];
								if(mMaininf.mWork.mWorkAvoidBumpDatNear > 1200)
								{
									#ifdef zbq_Debug
									api_UART4_SendData(2,"m5");
									vTaskDelay(50);
									api_ShortToStr(str,mMaininf.mWork.mWorkSensorDataAvl[4]);
									api_UART4_SendData(5,str);
									vTaskDelay(50);
									api_UART4_SendData(2,"\r\n");
									vTaskDelay(50);
									#endif
									
									api_SendCMD_To_Display(1,12,1,mMaininf.mWork.mWorkAvoidBumpDatFar,mMaininf.mWork.mWorkAvoidBumpDatNear,0,1000,3);
								}
								else
								{
									#ifdef zbq_Debug
									api_UART4_SendData(2,"m5");
									vTaskDelay(50);
									api_ShortToStr(str,mMaininf.mWork.mWorkSensorDataAvl[4]);
									api_UART4_SendData(5,str);
									vTaskDelay(50);
									api_UART4_SendData(2,"\r\n");
									vTaskDelay(50);
									#endif
									
									api_SendCMD_To_Display(1,12,0,mMaininf.mWork.mWorkAvoidBumpDatFar,mMaininf.mWork.mWorkAvoidBumpDatNear,2,1000,3);
									mMaininf.mWork.mWorkCheckErrorFlag = 1;
								}
							}
							else
							{
								api_SendCMD_To_Display(1,12,0,mMaininf.mWork.mWorkAvoidBumpDatFar,0,3,1000,3);
								mMaininf.mWork.mWorkCheckErrorFlag = 1;
							}
							api_Control_Motor(4,0);
							mMaininf.mWork.mWorkStep = TEST_FRONT_AVOID_BUMP5;
							break;
						case TEST_FRONT_AVOID_BUMP5 ://13
							if(mMaininf.mWork.mWorkAmidAvoidBumpFlag == 1)
							{
								api_Control_Motor(5,1);
								vTaskDelay(1500);
								
								mMaininf.mWork.mWorkAvoidBumpDatNear = mMaininf.mWork.mWorkSensorDataAvl[4];
								if(mMaininf.mWork.mWorkAvoidBumpDatNear > 1200)
								{
									#ifdef zbq_Debug
									api_UART4_SendData(2,"m6");
									vTaskDelay(50);
									api_ShortToStr(str,mMaininf.mWork.mWorkSensorDataAvl[4]);
									api_UART4_SendData(5,str);
									vTaskDelay(50);
									api_UART4_SendData(2,"\r\n");
									vTaskDelay(50);
									#endif
									
									api_SendCMD_To_Display(1,13,1,mMaininf.mWork.mWorkAvoidBumpDatFar,mMaininf.mWork.mWorkAvoidBumpDatNear,0,1000,3);
								}
								else
								{
									#ifdef zbq_Debug
									api_UART4_SendData(2,"m6");
									vTaskDelay(50);
									api_ShortToStr(str,mMaininf.mWork.mWorkSensorDataAvl[4]);
									api_UART4_SendData(5,str);
									vTaskDelay(50);
									api_UART4_SendData(2,"\r\n");
									vTaskDelay(50);
									#endif
									
									api_SendCMD_To_Display(1,13,0,mMaininf.mWork.mWorkAvoidBumpDatFar,mMaininf.mWork.mWorkAvoidBumpDatNear,2,1000,3);
									mMaininf.mWork.mWorkCheckErrorFlag = 1;
								}
							}
							else
							{
								api_SendCMD_To_Display(1,13,0,mMaininf.mWork.mWorkAvoidBumpDatFar,0,3,1000,3);
								mMaininf.mWork.mWorkCheckErrorFlag = 1;
							}
							api_Control_Motor(5,0);
							mMaininf.mWork.mWorkStep = TEST_FRONT_AVOID_BUMP6;
							break;
						case TEST_FRONT_AVOID_BUMP6 ://14
							if(mMaininf.mWork.mWorkAmidAvoidBumpFlag == 1)
							{
								api_Control_Motor(6,1);
								vTaskDelay(1500);
								
								mMaininf.mWork.mWorkAvoidBumpDatNear = mMaininf.mWork.mWorkSensorDataAvl[4];
								if(mMaininf.mWork.mWorkAvoidBumpDatNear > 1200)
								{
									#ifdef zbq_Debug
									api_UART4_SendData(2,"m7");
									vTaskDelay(50);
									api_ShortToStr(str,mMaininf.mWork.mWorkSensorDataAvl[4]);
									api_UART4_SendData(5,str);
									vTaskDelay(50);
									api_UART4_SendData(2,"\r\n");
									vTaskDelay(50);
									#endif
									
									api_SendCMD_To_Display(1,14,1,mMaininf.mWork.mWorkAvoidBumpDatFar,mMaininf.mWork.mWorkAvoidBumpDatNear,0,1000,3);
								}
								else
								{
									#ifdef zbq_Debug
									api_UART4_SendData(2,"m7");
									vTaskDelay(50);
									api_ShortToStr(str,mMaininf.mWork.mWorkSensorDataAvl[4]);
									api_UART4_SendData(5,str);
									vTaskDelay(50);
									api_UART4_SendData(2,"\r\n");
									vTaskDelay(50);
									#endif
									
									api_SendCMD_To_Display(1,14,0,mMaininf.mWork.mWorkAvoidBumpDatFar,mMaininf.mWork.mWorkAvoidBumpDatNear,2,1000,3);
									mMaininf.mWork.mWorkCheckErrorFlag = 1;
								}
							}
							else
							{
								api_SendCMD_To_Display(1,14,0,mMaininf.mWork.mWorkAvoidBumpDatFar,0,3,1000,3);
								mMaininf.mWork.mWorkCheckErrorFlag = 1;
							}
							api_Control_Motor(6,0);
							mMaininf.mWork.mWorkStep = TEST_FRONT_AVOID_BUMP7;
							break;
						case TEST_FRONT_AVOID_BUMP7 ://15
							if(mMaininf.mWork.mWorkAmidAvoidBumpFlag == 1)
							{
								api_Control_Motor(7,1);
								vTaskDelay(1500);
								
								mMaininf.mWork.mWorkAvoidBumpDatNear = mMaininf.mWork.mWorkSensorDataAvl[4];
								if(mMaininf.mWork.mWorkAvoidBumpDatNear > 1200)
								{
									#ifdef zbq_Debug
									api_UART4_SendData(2,"m8");
									vTaskDelay(50);
									api_ShortToStr(str,mMaininf.mWork.mWorkSensorDataAvl[4]);
									api_UART4_SendData(5,str);
									vTaskDelay(50);
									api_UART4_SendData(2,"\r\n");
									vTaskDelay(50);
									#endif
									
									api_SendCMD_To_Display(1,15,1,mMaininf.mWork.mWorkAvoidBumpDatFar,mMaininf.mWork.mWorkAvoidBumpDatNear,0,1000,3);
								}
								else
								{
									#ifdef zbq_Debug
									api_UART4_SendData(2,"m8");
									vTaskDelay(50);
									api_ShortToStr(str,mMaininf.mWork.mWorkSensorDataAvl[4]);
									api_UART4_SendData(5,str);
									vTaskDelay(50);
									api_UART4_SendData(2,"\r\n");
									vTaskDelay(50);
									#endif
									
									api_SendCMD_To_Display(1,15,0,mMaininf.mWork.mWorkAvoidBumpDatFar,mMaininf.mWork.mWorkAvoidBumpDatNear,2,1000,3);
									mMaininf.mWork.mWorkCheckErrorFlag = 1;
								}
							}
							else
							{
								api_SendCMD_To_Display(1,15,0,mMaininf.mWork.mWorkAvoidBumpDatFar,0,3,1000,3);
								mMaininf.mWork.mWorkCheckErrorFlag = 1;
							}
							api_Control_Motor(7,0);
							mMaininf.mWork.mWorkStep = TEST_FRONT_AVOID_BUMP8;
							break;
						case TEST_FRONT_AVOID_BUMP8 ://16
							if(mMaininf.mWork.mWorkAmidAvoidBumpFlag == 1)
							{
								api_Control_Motor(8,1);
								vTaskDelay(1500);
								
								mMaininf.mWork.mWorkAvoidBumpDatNear = mMaininf.mWork.mWorkSensorDataAvl[4];
								if(mMaininf.mWork.mWorkAvoidBumpDatNear > 1200)
								{
									#ifdef zbq_Debug
									api_UART4_SendData(2,"m9");
									vTaskDelay(50);
									api_ShortToStr(str,mMaininf.mWork.mWorkSensorDataAvl[4]);
									api_UART4_SendData(5,str);
									vTaskDelay(50);
									api_UART4_SendData(2,"\r\n");
									vTaskDelay(50);
									#endif
									
									api_SendCMD_To_Display(1,16,1,mMaininf.mWork.mWorkAvoidBumpDatFar,mMaininf.mWork.mWorkAvoidBumpDatNear,0,1000,3);
								}
								else
								{
									#ifdef zbq_Debug
									api_UART4_SendData(2,"m9");
									vTaskDelay(50);
									api_ShortToStr(str,mMaininf.mWork.mWorkSensorDataAvl[4]);
									api_UART4_SendData(5,str);
									vTaskDelay(50);
									api_UART4_SendData(2,"\r\n");
									vTaskDelay(50);
									#endif
									
									api_SendCMD_To_Display(1,16,0,mMaininf.mWork.mWorkAvoidBumpDatFar,mMaininf.mWork.mWorkAvoidBumpDatNear,2,1000,3);
									mMaininf.mWork.mWorkCheckErrorFlag = 1;
								}
							}
							else
							{
								api_SendCMD_To_Display(1,16,0,mMaininf.mWork.mWorkAvoidBumpDatFar,0,3,1000,3);
								mMaininf.mWork.mWorkCheckErrorFlag = 1;
							}
							api_Control_Motor(8,0);
							mMaininf.mWork.mWorkStep = TEST_FRONT_AVOID_BUMP9;
							break;
						case TEST_FRONT_AVOID_BUMP9 ://17
							if(mMaininf.mWork.mWorkAmidAvoidBumpFlag == 1)
							{
								api_Control_Motor(9,1);
								vTaskDelay(1500);
								
								mMaininf.mWork.mWorkAvoidBumpDatNear = mMaininf.mWork.mWorkSensorDataAvl[4];
								if(mMaininf.mWork.mWorkAvoidBumpDatNear > 1200)
								{
									#ifdef zbq_Debug
									api_UART4_SendData(3,"m10");
									vTaskDelay(50);
									api_ShortToStr(str,mMaininf.mWork.mWorkSensorDataAvl[4]);
									api_UART4_SendData(5,str);
									vTaskDelay(50);
									api_UART4_SendData(2,"\r\n");
									vTaskDelay(50);
									#endif
									
									api_SendCMD_To_Display(1,17,1,mMaininf.mWork.mWorkAvoidBumpDatFar,mMaininf.mWork.mWorkAvoidBumpDatNear,0,1000,3);
								}
								else
								{
									#ifdef zbq_Debug
									api_UART4_SendData(3,"m10");
									vTaskDelay(50);
									api_ShortToStr(str,mMaininf.mWork.mWorkSensorDataAvl[4]);
									api_UART4_SendData(5,str);
									vTaskDelay(50);
									api_UART4_SendData(2,"\r\n");
									vTaskDelay(50);
									#endif
									
									api_SendCMD_To_Display(1,17,0,mMaininf.mWork.mWorkAvoidBumpDatFar,mMaininf.mWork.mWorkAvoidBumpDatNear,2,1000,3);
									mMaininf.mWork.mWorkCheckErrorFlag = 1;
								}
							}
							else
							{
								api_SendCMD_To_Display(1,17,0,mMaininf.mWork.mWorkAvoidBumpDatFar,0,3,1000,3);
								mMaininf.mWork.mWorkCheckErrorFlag = 1;
							}
							api_Control_Motor(9,0);
							TIM1_PWM_Configure(26,71);
							
							if(mMaininf.mWork.mWorkCheck8Or9Flag == 1)//i8
							{
								mMaininf.mWork.mWorkStep = TEST_LAST;
							}
							else//i9
							{
								mMaininf.mWork.mWorkStep = TEST_POWER_DC5V;
							}
							break;
						case TEST_POWER_DC5V ://18   5V
							if(api_ReadGpioInputStauts(GPIOE,GPIO_Pin_12,1,5) == 1)
							{
								api_SendCMD_To_Display(1,18,1,0,0,0,1000,3);
							}
							else
							{
								api_SendCMD_To_Display(1,18,0,0,0,0,1000,3);
								mMaininf.mWork.mWorkCheckErrorFlag = 1;
							}
							mMaininf.mWork.mWorkStep = TEST_POWER_VCC3V3;
							break;
						case TEST_POWER_VCC3V3 ://19   3.3V
							if(api_ReadGpioInputStauts(GPIOE,GPIO_Pin_11,1,5) == 1)
							{
								api_SendCMD_To_Display(1,19,1,0,0,0,1000,3);
							}
							else
							{
								api_SendCMD_To_Display(1,19,0,0,0,0,1000,3);
								mMaininf.mWork.mWorkCheckErrorFlag = 1;
							}
							mMaininf.mWork.mWorkStep = TEST_POWER_VCC1V8;
							break;
						case TEST_POWER_VCC1V8 ://20   1.8V
							if(api_Test_ADC(&ADCValue[5],3391,300) == 1)
							{
								api_SendCMD_To_Display(1,20,1,0,0,0,1000,3);
							}
							else
							{
								api_SendCMD_To_Display(1,20,0,0,0,0,1000,3);
								mMaininf.mWork.mWorkCheckErrorFlag = 1;
							}
							mMaininf.mWork.mWorkStep = TEST_POWER_VCC1V3;
							break;
						case TEST_POWER_VCC1V3 ://21   1.3V
							if(api_Test_ADC(&ADCValue[6],2182,300) == 1)
							{
								api_SendCMD_To_Display(1,21,1,0,0,0,1000,3);
							}
							else
							{
								api_SendCMD_To_Display(1,21,0,0,0,0,1000,3);
								mMaininf.mWork.mWorkCheckErrorFlag = 1;
							}
							mMaininf.mWork.mWorkStep = TEST_POWER_DC3V3;
							break;
						case TEST_POWER_DC3V3 ://22   3.3V
							if(api_ReadGpioInputStauts(GPIOE,GPIO_Pin_10,1,5) == 1)
							{
								api_SendCMD_To_Display(1,22,1,0,0,0,1000,3);
							}
							else
							{
								api_SendCMD_To_Display(1,22,0,0,0,0,1000,3);
								mMaininf.mWork.mWorkCheckErrorFlag = 1;
							}
// 							if(mMaininf.mWork.mWorkCheck8Or9Flag == 1)//i8
// 							{
// 								mMaininf.mWork.mWorkStep = TEST_LAST;
// 							}
// 							else
// 							{
// 								mMaininf.mWork.mWorkStep = TEST_TOF_VSEL4V6;
// 							}
							mMaininf.mWork.mWorkStep = TEST_TOF_VSEL4V6;
							break;
						case TEST_TOF_VSEL4V6 ://23   4.6V
							if(api_Test_ADC(&ADCValue[0],2054,200) == 1)
							{
								api_SendCMD_To_Display(1,23,1,0,0,0,1000,3);
							}
							else
							{
								api_SendCMD_To_Display(1,23,0,0,0,0,1000,3);
								mMaininf.mWork.mWorkCheckErrorFlag = 1;
							}
							mMaininf.mWork.mWorkStep = TEST_TOF_CSI_AVDD;
							break;
						case TEST_TOF_CSI_AVDD ://24    3.3V
							if(api_Test_ADC(&ADCValue[1],2760,200) == 1)
							{
								api_SendCMD_To_Display(1,24,1,0,0,0,1000,3);
							}
							else
							{
								api_SendCMD_To_Display(1,24,0,0,0,0,1000,3);
								mMaininf.mWork.mWorkCheckErrorFlag = 1;
							}
							mMaininf.mWork.mWorkStep = TEST_TOF_CSI_DVDD;
							break;
						case TEST_TOF_CSI_DVDD ://25   3.3V
							if(api_Test_ADC(&ADCValue[2],2735,200) == 1)
							{
								api_SendCMD_To_Display(1,25,1,0,0,0,1000,3);
							}
							else
							{
								api_SendCMD_To_Display(1,25,0,0,0,0,1000,3);
								mMaininf.mWork.mWorkCheckErrorFlag = 1;
							}
							mMaininf.mWork.mWorkStep = TEST_TOF_INIT;
							break;
						case TEST_TOF_INIT ://26
	 						if(api_SendCMD_To_MainBoard(0,1,2,100,0,5) == 1)
	 						{
								api_SendCMD_To_Display(1,26,1,0,0,0,1000,3);
	 						}
	 						else
	 						{
								api_SendCMD_To_Display(1,26,0,0,0,0,1000,3);
								mMaininf.mWork.mWorkCheckErrorFlag = 1;
	 						}
							mMaininf.mWork.mWorkStep = TEST_LAST;
							break;
// 						case TEST_TOF_CHECK ://29
// // 	 						if(api_SendCMD_To_MainBoard(0,1,3,100,1,5) == 1)
// // 	 						{
// // 	 							mMaininf.mWork.mWorkThingHight = ((mMaininf.mUart1.ReceiveBuf[4] << 24) | (mMaininf.mUart1.ReceiveBuf[5] << 16) |
// // 	 																(mMaininf.mUart1.ReceiveBuf[6] << 8) | mMaininf.mUart1.ReceiveBuf[7]);
// // 	 							if((mMaininf.mWork.mWorkThingHight > 5) && (mMaininf.mWork.mWorkThingHight < 100))
// // 	 							{
// // 									api_SendCMD_To_Display(1,29,1);
// // 	 							}
// // 	 							else
// // 	 							{
// // 									api_SendCMD_To_Display(1,29,0);
// // 									mMaininf.mWork.mWorkCheckErrorFlag = 1;
// // 	 							}
// // 	 						}
// // 	 						else
// // 	 						{
// // 								api_SendCMD_To_Display(1,29,0);
// // 								mMaininf.mWork.mWorkCheckErrorFlag = 1;
// // 	 						}
// 							mMaininf.mWork.mWorkStep = TEST_LAST;
// 							break;
						
						
						case TEST_LAST :
// 							api_Send_Signal(3,0);
// 							api_Send_Signal(4,0);
							api_Send_Signal(5,0);
							PAout(11) = 0;//i8特有
							PAout(12) = 0;
							PAout(15) = 0;
							//PEout(1) = 1;
							if(mMaininf.mWork.mWorkCheckErrorFlag == 0)
							{
								api_SendCMD_To_Display(1,0xfe,0,0,0,0,1000,3);
							}
							mMaininf.mWork.mWorkKeyPressFlag = 0;
							mMaininf.mWork.mWorkCheckStatus = 0;
							mMaininf.mWork.mWorkStep = 0;
							mMaininf.mWork.mWorkFileStep = 0;
							mMaininf.mWork.mWorkChecking = 0;
							mMaininf.mWork.mWorkCheckErrorFlag = 0;
							mMaininf.mWork.mWorkAmidAvoidBumpFlag = 0;
							PGout(13) = 0;   //抬起板子
							break;
// 						case TEST_FILE ://test file
// 							api_Send_Signal(5,0);
// 							PAout(12) = 0;
// 							PAout(15) = 0;
// 							PEout(1) = 1;
// 						
// 							api_UART4_DEBUG_SendCMDData(0,0,mMaininf.mWork.mWorkFileStep);
// 							
// 							mMaininf.mWork.mWorkCheckStatus = 0;
// 							mMaininf.mWork.mWorkStep = 0;
// 							mMaininf.mWork.mWorkFileStep = 0;
// 							mMaininf.mWork.mWorkChecking = 0;
// 							api_Led_Status_Set(LED_A0,2);
// 							PGout(13) = 0;   //抬起板子
// 							//api_UART5_Display_SendCMDData(1,mMaininf.mWork.mWorkFileStep,0);
// 							break;
// 						case TEST_OK ://test ok
// 							api_Send_Signal(5,0);
// 							PAout(12) = 0;
// 							PAout(15) = 0;
// 							PEout(1) = 1;
// 							mMaininf.mWork.mWorkCheckStatus = 0;
// 							mMaininf.mWork.mWorkStep = 0;
// 							mMaininf.mWork.mWorkFileStep = 0;
// 							mMaininf.mWork.mWorkChecking = 0;
// 							api_Led_Status_Set(LED_A0,1);
// 							PGout(13) = 0;   //抬起板子
// 							//api_UART5_Display_SendCMDData(1,0xfe,1);
// 							//mMaininf.mWork.mWorkBigUartFlag = 0;
// 							break;
						default :
							break;
					}
				}
			}
		
		}
	}
}

/*****************************************************************************
 * @name       :void api_SendCMD_To_Display(u8 mode,u8 cmd,u8 status,u16 dat1,u16 dat2,u8 falg,u16 waittime,u8 count)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :send data
 * @parameters :cmd
 * @retvalue   :None
******************************************************************************/
void api_SendCMD_To_Display(u8 mode,u8 cmd,u8 status,u16 dat1,u16 dat2,u8 flag,u16 waittime,u8 count)
{
	u8 mCont = 0;
	u8 mStatus = 0;
	
	do
	{
		if(++mCont > count)
		{
			return;
		}
		
		api_UART5_Display_SendCMDData(mode,cmd,status,dat1,dat2,flag);
		mMaininf.mSystem.mDisplayStartTime = api_GetCurrentSystemTime();
		//vTaskDelay(1000);
		while(api_GetSystemTimePeriod(mMaininf.mSystem.mDisplayStartTime) < waittime)
		{
			if(mMaininf.mUart5.mReceiveFlag == 1)
			{
				mMaininf.mUart5.mReceiveFlag = 0;
				
				if((mMaininf.mUart5.ReceiveBuf[0] == mode) && (mMaininf.mUart5.ReceiveBuf[1] == cmd) &&
					(mMaininf.mUart5.ReceiveBuf[2] == 1))
				{
					mStatus = 1;
					break;
				}
			}
		}
		
		mMaininf.mUart5.mReceiveFlag = 0;
		
	}while(mStatus == 0);
	
}

/*****************************************************************************
 * @name       :void api_SendCMD_To_MainBoard(u8 seq,u8 mode,u8 cmd,u16 waittime)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :发送命令到主板
 * @parameters :testtype : 0:读取数据   1:检测发射    2:检测接收   waittime:单次检查时间  
				type:类型0：状态  1：数据     num:重发次数
 * @retvalue   :0:主板无数据返回     1：主板有数据返回
******************************************************************************/
u8 api_SendCMD_To_MainBoard(u8 seq,u8 mode,u8 cmd,u16 waittime,u8 type,u8 num)
{
	u8 mCont = 0;
	
	mMaininf.mUart1.ReceiveBuf[4] = 0;
	
	if(type == 0)
	{
		do{
			if(++mCont > num) return 0;//未接收到数据
			api_UART1_MainBoard_SendCMDData(seq,mode,cmd);
			vTaskDelay(waittime);
		}while((mMaininf.mUart1.mReceiveFlag == 0) || ((mMaininf.mUart1.mReceiveFlag == 1) && (mMaininf.mUart1.ReceiveBuf[4] == 0)));
		
	}
	else
	{
		do{
			if(++mCont > num) return 0;//未接收到数据
			api_UART1_MainBoard_SendCMDData(seq,mode,cmd);
			vTaskDelay(waittime);
		}while(mMaininf.mUart1.mReceiveFlag == 0);
		
	}
	
	mMaininf.mUart1.mReceiveFlag = 0;
	
	return 1;//接收到数据
}

