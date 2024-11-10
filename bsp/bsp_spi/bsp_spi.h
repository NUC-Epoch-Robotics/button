#ifndef __BSP_SPI_H
#define __BSP_SPI_H

#include "gpio.h"
#include "tim.h"

typedef enum
{
	SPI_Write_Mem = 0,//写寄存器
	SPI_Read_Mem,     //读寄存器
	
}SPI_Access_Mode;

typedef enum
{
SPI_BLACK_MODE=0,//阻塞模式
SPI_IT_MODE,     //中断模式
SPI_DMA_MODE,     //DMA传输

}SPI_Work_Mode;

typedef struct spi_temp_s
{
	SPI_HandleTypeDef* bsp_spi_Handle;
	
	GPIO_TypeDef*   bsp_spi_port;//SPI 对应的GPIO
	uint16_t        bsp_spi_pin; //SPI GPIO引脚号

	GPIO_TypeDef*   bsp_spi_GPIOx_cs;//片选信号对应的GPIO,如GPIOA,GPIOB等等
	uint16_t        bsp_spi_pin_cs;  //片选信号对应的引脚号
	
	uint16_t reg;                // 暂时只支持7位地址(还有一位是读写位),注意不需要左移
	
    SPI_Work_Mode   Work_Mode;   //SPI工作模式
//	SPI_Seq_Mode    Seq_Mode ;    
  	SPI_Access_Mode  Access_Mode;
	 void (*callback)(struct spi_temp_s *); // 接收完成后的回调函数
	
} SPIInstance;


void SPITransmit(SPIInstance* spi ,uint8_t *pData ,uint16_t size);


void SPIReceive(SPIInstance* spi ,uint8_t *pData ,uint16_t size);

//基于HAL库的SPI读写字节函数
void SPITransRecv(SPIInstance* spi ,uint8_t *txData ,uint8_t *rxData,uint16_t size);


//改变传输函数的模式
void SPISetMode(SPIInstance* spi,SPI_Work_Mode work_mode);


#endif

