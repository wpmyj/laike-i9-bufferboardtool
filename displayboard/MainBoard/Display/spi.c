/****************************************************************************************************/
/*			@FileName		:	spi.c															  	*/
/*			@Author			:	zhengbaiqiu															*/
/*			@Version		:	v1.0																*/
/*			@Date			:	2019.11.12  														*/
/****************************************************************************************************/


/* Includes --------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Project ---------------------------------------------------------------------*/
#include "spi.h"

/* Variable --------------------------------------------------------------------*/
/* Function --------------------------------------------------------------------*/
void SPI1_Configure(void);
void SPI_SetSpeed(SPI_TypeDef* SPIx,u8 Speeds);
u8 SPI_WriteByte(SPI_TypeDef* SPIx,u8 Byte);




/*****************************************************************************
 * @name       :void SPI2_Configure(void)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :spi configure
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void SPI1_Configure(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	 
	//配置SPI1管脚
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6;    
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
	GPIO_Init(GPIOA, &GPIO_InitStructure);  
	
	//SPI1配置选项
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &SPI_InitStructure);

	//使能SPI1
	SPI_Cmd(SPI1, ENABLE);   
}

/*****************************************************************************
 * @name       :void SPI_SetSpeed(SPI_TypeDef* SPIx,u8 Speeds)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :set spi speed
 * @parameters :SPIx: SPI type,x for 1,2,3
                Speed:SPIx speed = APB1 / 2^(Speeds + 1)
 * @retvalue   :None
******************************************************************************/
void SPI_SetSpeed(SPI_TypeDef* SPIx,u8 Speeds)
{
	SPIx->CR1 &= 0xFFC7;
	SPIx->CR1 |= Speeds<<3;
	SPIx->CR1|=1<<6;        //SPI设备使能
//	SPI_Cmd(SPIx,ENABLE);
}

/*****************************************************************************
 * @name       :u8 SPI_WriteByte(SPI_TypeDef* SPIx,u8 Byte)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :Write a byte of data using STM32's hardware SPI
 * @parameters :SPIx: SPI type,x for 1,2,3
                Byte:Data to be written
 * @retvalue   :Data received by the bus
******************************************************************************/
u8 SPI_WriteByte(SPI_TypeDef* SPIx,u8 Byte)
{
	while((SPIx->SR&SPI_I2S_FLAG_TXE)==RESET);		//等待发送区空	  
	SPIx->DR=Byte;	 								//发送一个byte   
	while((SPIx->SR&SPI_I2S_FLAG_RXNE)==RESET);		//等待接收完一个byte  
	return SPIx->DR;          	    			    //返回收到的数据			
} 





// void SPI_SetSpeed(SPI_TypeDef* SPIx,u8 SpeedSet)
// {
// 	SPIx->CR1&=0XFFC7;
// 	if(SpeedSet==1)//高速
// 	{
// 		SPIx->CR1|=SPI_BaudRatePrescaler_2;//Fsck=Fpclk/2	
// 	}
// 	else//低速
// 	{
// 		SPIx->CR1|=SPI_BaudRatePrescaler_32; //Fsck=Fpclk/32
// 	}
// 	SPIx->CR1|=1<<6; //SPI设备使能
// } 

























