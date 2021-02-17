/****************************************************************************************************/
/*			@FileName		:	DISPLAY.h																																		   	*/
/*			@Author			:	zhengbaiqiu																																		*/
/*			@Version		:	v1.0																																					*/
/*			@Date			:	2019.11.12  																																	*/
/****************************************************************************************************/

#ifndef	__DISPLAY_H__
#define	__DISPLAY_H__
#include "stm32f10x.h"

#ifdef __cplusplus
extern "C" {
#endif

	
void DrawTestPage(u8 *str);
void Test_Display(void);
void Display_Test(u8 test_mode,u8 test_cmd,u8 test_status,u16 dat1,u16 dat2,u8 flag);
	
	
#ifdef _cplusplus
extern }
#endif

#endif /*__DISPLAY_H__*/	


