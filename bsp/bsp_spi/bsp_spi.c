#include "bsp_spi.h"

int bsp_spi_init(	SPIInstance* spi,SPI_HandleTypeDef*  bsp_spi_Handle, SPI_Work_Mode Work_Mode )
{
	if(spi->bsp_spi_Handle!=NULL )
	{
		spi->Work_Mode = Work_Mode;
		spi->bsp_spi_Handle = bsp_spi_Handle;
	  return 0;
	 }
	return -1;
}    



//基于HAL库的SPI发送函数的封装
void SPITransmit(SPIInstance* spi ,uint8_t *pData ,uint16_t size)
{
		HAL_GPIO_WritePin(spi->bsp_spi_GPIOx_cs,spi->bsp_spi_pin_cs ,GPIO_PIN_RESET );
		switch(spi->Work_Mode )
		{
      case SPI_IT_MODE:
			 HAL_SPI_Transmit_IT(spi->bsp_spi_Handle , pData, size);
			break;
			
			 case SPI_DMA_MODE:
			 HAL_SPI_Transmit_DMA(spi->bsp_spi_Handle , pData, size);
			break;
			
			case SPI_BLACK_MODE:
		  	HAL_SPI_Transmit(spi->bsp_spi_Handle , pData, size,100);
		 	HAL_GPIO_WritePin(spi->bsp_spi_GPIOx_cs,spi->bsp_spi_pin_cs ,GPIO_PIN_SET );
			break;
			
			default:
	    while(1); //状态不在所设模式，停止传输；
			
		}
}
//基于HAL库的SPI接收函数的封装
void SPIReceive(SPIInstance* spi ,uint8_t *pData ,uint16_t size)
{
		HAL_GPIO_WritePin(spi->bsp_spi_GPIOx_cs,spi->bsp_spi_pin_cs ,GPIO_PIN_RESET );
		switch( spi->Work_Mode )
		{
      case SPI_IT_MODE:
			 HAL_SPI_Receive_IT(spi->bsp_spi_Handle , pData, size);
		  	break;
			
			case SPI_DMA_MODE:
			 HAL_SPI_Receive_DMA(spi->bsp_spi_Handle , pData, size);
			 break;
			
			case SPI_BLACK_MODE:
		   HAL_SPI_Receive(spi->bsp_spi_Handle , pData, size,100);
			 // 阻塞模式不会调用回调函数,传输完成后直接拉高片选结束
		 	 HAL_GPIO_WritePin(spi->bsp_spi_GPIOx_cs,spi->bsp_spi_pin_cs ,GPIO_PIN_SET );
		   break;
			
		  default:
	     while(1); //状态不在所设模式，停止传输；
			 
		}
}

//基于HAL库的SPI读写字节函数
void SPITransRecv(SPIInstance* spi ,uint8_t *txData ,uint8_t *rxData,uint16_t size)
{
	
	
 	HAL_GPIO_WritePin(spi->bsp_spi_GPIOx_cs,spi->bsp_spi_pin_cs ,GPIO_PIN_RESET );
		switch(spi->Work_Mode )
		{
      case SPI_IT_MODE:
			 HAL_SPI_TransmitReceive_IT(spi->bsp_spi_Handle , txData, rxData, size);
		  	break;
			
			case SPI_DMA_MODE:
			 HAL_SPI_TransmitReceive_DMA(spi->bsp_spi_Handle , txData, rxData, size);
			 break;
			
			case SPI_BLACK_MODE:
		   HAL_SPI_TransmitReceive(spi->bsp_spi_Handle, txData, rxData, size,1000);
			 // 阻塞模式不会调用回调函数,传输完成后直接拉高片选结束
		 	 HAL_GPIO_WritePin(spi->bsp_spi_GPIOx_cs,spi->bsp_spi_pin_cs ,GPIO_PIN_SET );
		   break;
			
		  default:
	     while(1); //状态不在所设模式，停止传输；
		
		}
	}

//改变传输函数的模式
void SPISetMode(SPIInstance* spi,SPI_Work_Mode work_mode)
{
	if (spi->Work_Mode != SPI_DMA_MODE && spi->Work_Mode != SPI_IT_MODE && spi->Work_Mode  != SPI_BLACK_MODE)
        while (1)
            ; // error mode! 请查看是否正确设置模式，或出现指针越界导致模式被异常修改的情况

 if(spi->Work_Mode !=work_mode)
 {
  spi->Work_Mode =work_mode;
 }
}



