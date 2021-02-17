/****************************************************************************************************/
/*			@FileName		:	apiLib.h																																    	*/
/*			@Author			:	zhengbaiqiu																																		*/
/*			@Version		:	v1.0																																					*/
/*			@Date				:	2019.11.12  																																	*/
/****************************************************************************************************/

#ifndef	__APILIB_H__
#define	__APILIB_H__
#include "stm32f10x.h"
//#include <stdint.h>
#include "gpioredef.h"
#include "maindef.h"

#ifdef __cplusplus
extern "C" {
#endif


	
u32 api_GetSystemTimePeriod(u32 StartTime);
u32 api_GetCurrentSystemTime(void);
void api_SendCMDData(u8 mode,u8 cmd,u8 status);
void api_UART1_ReceiveData(u8 vData);
void api_MachinePowerOn(void);
	


	
	
#ifdef _cplusplus
extern }
#endif

#endif /*__APILIB_H__*/	

