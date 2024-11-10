#ifndef __BSP_USART_H
#define __BSP_USART_H

#include "gpio.h"


#define BUF_SIZE     64      //单次最大接收字节数
typedef enum
{
    USART_TRANSFER_NONE=0,
    USART_TRANSFER_BLOCKING,
    USART_TRANSFER_IT,
    USART_TRANSFER_DMA,
	  USART_TRANSFER_IDLE
} USART_TRANSFER_MODE;

typedef struct
{
  UART_HandleTypeDef   *usart_handle;
  USART_TRANSFER_MODE  TRANSFER_MODE;
} USARTInstance;

extern DMA_HandleTypeDef hdma_usart1_rx;

//串口初始化
void bsp_usart_init(USARTInstance* usart , UART_HandleTypeDef   *usart_handle,USART_TRANSFER_MODE  TRANSFER_MODE);
              
//基于HAL库usart发送函数封装
void UartSend(USARTInstance *usart, uint8_t *send_buf, uint16_t send_size,USART_TRANSFER_MODE mode);
 void UartReceive(USARTInstance *usart, uint8_t *send_buf, uint16_t send_size,USART_TRANSFER_MODE mode );
void UART_Receive_IT_enable( USARTInstance *usart,uint32_t InterruptDefinitions);
void  Uart_Idle_rcDMA(UART_HandleTypeDef *huart,uint8_t* DataBuff);
#endif


