/****************************************************************************************************/
/*			@FileName		:	SPI.h																																		   	*/
/*			@Author			:	zhengbaiqiu																																		*/
/*			@Version		:	v1.0																																					*/
/*			@Date			:	2019.11.12  																																	*/
/****************************************************************************************************/

#ifndef	__SPI_H__
#define	__SPI_H__
#include "stm32f10x.h"

#ifdef __cplusplus
extern "C" {
#endif


void SPI1_Configure(void);
void SPI_SetSpeed(SPI_TypeDef* SPIx,u8 Speeds);
u8 SPI_WriteByte(SPI_TypeDef* SPIx,u8 Byte);
	
	
#ifdef _cplusplus
extern }
#endif

#endif /*__SPI_H__*/	


