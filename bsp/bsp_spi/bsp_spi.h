#ifndef __BSP_SPI_H
#define __BSP_SPI_H

#include "gpio.h"
#include "tim.h"

typedef enum
{
	SPI_Write_Mem = 0,//д�Ĵ���
	SPI_Read_Mem,     //���Ĵ���
	
}SPI_Access_Mode;

typedef enum
{
SPI_BLACK_MODE=0,//����ģʽ
SPI_IT_MODE,     //�ж�ģʽ
SPI_DMA_MODE,     //DMA����

}SPI_Work_Mode;

typedef struct spi_temp_s
{
	SPI_HandleTypeDef* bsp_spi_Handle;
	
	GPIO_TypeDef*   bsp_spi_port;//SPI ��Ӧ��GPIO
	uint16_t        bsp_spi_pin; //SPI GPIO���ź�

	GPIO_TypeDef*   bsp_spi_GPIOx_cs;//Ƭѡ�źŶ�Ӧ��GPIO,��GPIOA,GPIOB�ȵ�
	uint16_t        bsp_spi_pin_cs;  //Ƭѡ�źŶ�Ӧ�����ź�
	
	uint16_t reg;                // ��ʱֻ֧��7λ��ַ(����һλ�Ƕ�дλ),ע�ⲻ��Ҫ����
	
    SPI_Work_Mode   Work_Mode;   //SPI����ģʽ
//	SPI_Seq_Mode    Seq_Mode ;    
  	SPI_Access_Mode  Access_Mode;
	 void (*callback)(struct spi_temp_s *); // ������ɺ�Ļص�����
	
} SPIInstance;


void SPITransmit(SPIInstance* spi ,uint8_t *pData ,uint16_t size);


void SPIReceive(SPIInstance* spi ,uint8_t *pData ,uint16_t size);

//����HAL���SPI��д�ֽں���
void SPITransRecv(SPIInstance* spi ,uint8_t *txData ,uint8_t *rxData,uint16_t size);


//�ı䴫�亯����ģʽ
void SPISetMode(SPIInstance* spi,SPI_Work_Mode work_mode);


#endif

