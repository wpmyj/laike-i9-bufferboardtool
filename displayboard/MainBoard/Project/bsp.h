/****************************************************************************************************/
/*			@FileName		:	BSP.h																																		    	*/
/*			@Author			:	zhengbaiqiu																																		*/
/*			@Version		:	v1.0																																					*/
/*			@Date				:	2019.11.12  																																	*/
/****************************************************************************************************/

#ifndef	__BSP_H__
#define	__BSP_H__
#include "stm32f10x.h"

#ifdef __cplusplus
extern "C" {
#endif

	
	
	
/*-----------------------------------------------------------------------*/
/*    中断优先级                                                         */
/*-----------------------------------------------------------------------*/
	
#define WIFI_UART_PRIORITY                        5
#define TIME_TIM2_PRIORITY                        2
#define WIFI_UART_DMA_PRIORITY                    6
	
	
/*-----------------------------------------------------------------------*/
/*    端口定义                                                           */
/*-----------------------------------------------------------------------*/

/* UART configuration ----------------------------------------------*/
	
#define DEBUG_UART_GPIO                GPIOA               //      uart1
#define DEBUG_UART_TX_PIN         	   GPIO_Pin_9
#define DEBUG_UART_RX_PIN              GPIO_Pin_10


#define WIFI_UART_GPIO             	   GPIOA               //      uart2
#define WIFI_UART_TX_PIN               GPIO_Pin_2
#define WIFI_UART_RX_PIN               GPIO_Pin_3	
	
	
	
	
	
	
static volatile ErrorStatus HSEStartUpStatus = SUCCESS;	

void prvSetupHardware(void);
	


	
	
#ifdef _cplusplus
extern }
#endif

#endif /*__BSP_H__*/	

